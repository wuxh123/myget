// DownloadItemManager.cpp: implementation of the CDownloadItemManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "myget.h"
#include "DownloadItemManager.h"
#include "DownloadHttpJet.h"
#include "DownloadFtpJet.h"
//#include "MainScheduleThread.h"
#include "DownloadScheduler.h"

#include "AppRegs.h"

//tempory include, only for BLOCK_BYTE_SIZE.
//In the furtures, If we read the reg value from the registry, remove it!!!
#include "ProgressChart.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//IMPLEMENT_DYNCREATE(CDownloadItemManager, CWinThread)

//DEL BOOL CDownloadItemManager::InitInstance()
//DEL {
//DEL 	// TODO:  perform and per-thread initialization here
//DEL 	StartDownload();
//DEL 
//DEL 	return TRUE;
//DEL }






CDownloadItemManager::CDownloadItemManager(CDownloadScheduler *pDownloadScheduler, CListItem *pListItem) : 
		m_slockCanHandleMsg(&m_crtsectCanHandleMsg)
{
	//m_pMainScheduleThread = pMainScheduleThread;
	m_pDownloadScheduler = pDownloadScheduler;
	m_pListItem = pListItem;
	m_iSimJets = m_pListItem->GetSplitsCount();
	m_iResumeState = m_pListItem->IsSupportResumed();
	m_iDownloadFileSize = m_pListItem->GetFileSize();

	m_arJets.RemoveAll();

	m_bHasInitMaps = FALSE;

	m_pFile = NULL;

	m_bQuit = FALSE;

	if (m_iDownloadFileSize > 0)
	{
		m_bHasInitMaps = prv_InitMaps();
	}


}

CDownloadItemManager::~CDownloadItemManager()
{
	prv_ClearJob();

	CDownloadJet *pJet;
	for (int i = m_arJets.GetSize() - 1; i >= 0; i --)
	{
		pJet = m_arJets[i];

		if (pJet)
		{
			delete pJet;
		}
	}

	m_arJets.RemoveAll();

}


int CDownloadItemManager::StopDownload()
{
	prv_ClearJob();
	
	return 0;
}

int CDownloadItemManager::StartDownload()
{
	m_pListItem->SetFileStatus(DOWNLOADING);
	m_pListItem->ClearJetInfo();
	m_pDownloadScheduler->RefreshMainframe(m_pListItem);

	return prv_NewJet(TRUE);
}

int CDownloadItemManager::prv_GetThreadIndex(CDownloadJet *pJet)
{
	for (int i = 0; i < m_arJets.GetSize(); i ++)
	{
		if (m_arJets[i] == pJet)
		{
			return i;
		}
	}

	return -1;
}


void CDownloadItemManager::JetGeneralInfo(CDownloadJet *pJet, LPCTSTR lpszMsg)
{
	int iIndex = prv_GetThreadIndex(pJet);

	prv_RefreshItem(pJet, lpszMsg);
	TRACE("Thread <%d> sent header info \r\n", iIndex);

}

BOOL CDownloadItemManager::JetStartRecv(CDownloadJet *pJet, LPCTSTR lpszMsg)
{
	int iIndex = prv_GetThreadIndex(pJet);

	TRACE("Thread: %d Start to Recv\r\n", iIndex);

	prv_RefreshItem(pJet, lpszMsg);

	if (m_bQuit)
	{
		return FALSE;
	}

	m_pListItem->SetAttr(ATTR_EXCUTED, TRUE);

	if (pJet->IsSupportResume())
	{
		m_iResumeState = SUPPORTRESUME;
		m_pListItem->SetAttr(ATTR_SUPPORTRESUME, TRUE);
	}
	else if (iIndex > 0)
	{
		pJet->StopDownload();
	
		prv_MergeUndownloadedBrks();
		return FALSE;
	}
	
	//如果不支持断点续传的话，那么，只可是是第一个线程，并且从开始下载，
	//才能够进入这里，其他的情况应当在ResultFailed中处理。

	/*
	if (pJet->GetRange()->iStart > 0 && !pJet->IsSupportResume())
	{
		m_iResumeState = UNSUPPORTRESUME;
		m_pListItem->SetAttr(ATTR_SUPPORTRESUME, FALSE);

		
		if (iIndex == 0)
		{
			prv_MergeUndownloadedBrks();
		}
		
		if (iIndex > 0)
		{
			//这时候，后续的线程应该不允许下载。
			pJet->StopDownload();
	
			return FALSE;
		}

		
	}
	*/

	//如果是第一个线程，则进行一些初始化工作
	if (m_iDownloadFileSize == -1)
	{
		//试图从第一个返回的线程中得到下载的文件的尺寸，当进行FTP下载时，应该采用别的方法。
		m_iDownloadFileSize = m_pListItem->GetFileDownloadedSize() + pJet->GetContentLength();
		
		if (!m_bHasInitMaps)
		{
			if (!prv_InitMaps())
			{
				return FALSE;
			}
			
			m_bHasInitMaps = TRUE;

		}

		//对下载范围进行分配, 注意如果下载范围分配失败，实际上该线程已经被删除，相关的数据结构被更新。
		if (!prv_AssignDownloadRange(pJet))
		{
			pJet->StopDownload();
		}

	}

	//只有当文件尺寸已知，并且并不知道不支持断点续穿的时候，才可以开始后续的线程。
	//注意：当第2个线程发现不支持断点续穿的时候，会导致ResultFailed的调用，不会进入到这里。
	if (m_iResumeState != UNSUPPORTRESUME && m_iDownloadFileSize != -1)
	{
		prv_NewJet(FALSE);
	}

	return TRUE;
}

BOOL CDownloadItemManager::prv_InitDownloadJob()
{
	m_strFilePath= m_pListItem->GetDownloadedFolder();

	if (m_strFilePath.Right(1) != "\\")
	{
		m_strFilePath += "\\";
	}

	m_strFilePath += m_pListItem->GetRename();

	m_pFile = new CFile;

	m_pFile->Open(m_strFilePath + ".jc!", CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);

	m_pFile->SetLength(m_iDownloadFileSize);

	m_pListItem->SetFileSize(m_iDownloadFileSize);

	m_pFile->Flush();
	return TRUE;
}



void CDownloadItemManager::JetRecvOK(CDownloadJet *pJet, LPCTSTR lpszMsg)
{
	if (!m_bQuit)
	{
		m_pFile->Seek(pJet->GetCurPos() - pJet->GetCurDownloadedSize(), CFile::begin);
		m_pFile->Write(pJet->GetDownloadBuf(), pJet->GetCurDownloadedSize());

		prv_UpdateDownloadBrkMap(pJet);
		//m_pMainScheduleThread->IncLastDownloadDataCounter(pJet->GetCurDownloadedSize());
		m_pDownloadScheduler->IncLastDownloadDataCounter(pJet->GetCurDownloadedSize());

		prv_RefreshItem(pJet, lpszMsg);

	}
}

void CDownloadItemManager::JetCanceled(CDownloadJet *pJet, LPCTSTR lpszMsg)
{
	prv_RefreshItem(pJet, lpszMsg);
}
	
void CDownloadItemManager::JetResultFailed(CDownloadJet *pJet, LPCTSTR lpszMsg)
{
	prv_RefreshItem(pJet, lpszMsg);

	PBREAKPOSITION pBrkPos = pJet->GetRange();

	if (pBrkPos->iEnd == -1)
	{
		delete pBrkPos;
	}

	prv_ClearJob();

}

void CDownloadItemManager::JetFailed(CDownloadJet *pJet, LPCTSTR lpszMsg)
{
	prv_RefreshItem(pJet, lpszMsg);
}

void CDownloadItemManager::JetCompleted(CDownloadJet *pJet, LPCTSTR lpszMsg)
{
	TRACE("Thread: %d Completed Job\r\n", prv_GetThreadIndex(pJet));

	prv_RefreshItem(pJet, lpszMsg);

	prv_HandleThreadCompleted(pJet);

	if (m_bQuit)
	{
		return;
	}


	if (!prv_AssignDownloadRange(pJet))
	{
		prv_AttachJetInfo(pJet, lpszMsg);		
		pJet->StopDownload();
	}

	if (m_arUnDownloadedBrks.GetSize() == 0)
	{
		prv_ClearJob();
	}

}

BOOL CDownloadItemManager::prv_InitMaps()
{
	int i;

	if (m_iDownloadFileSize < 0)
	{
		return FALSE;
	}

	prv_InitDownloadJob();

	//首先从ListItem中得到已经下载的碎片信息。
	m_parDownloadBrks = (CArray <PBREAKPOSITION, PBREAKPOSITION> *)m_pListItem->GetBrkPosArray();
	
	//清空尚未下载的纪录
	m_arUnDownloadedBrks.RemoveAll();
	
	//由ListItem中的已经下载的碎片纪录来生成尚未分配的碎片纪录
	PBREAKPOSITION pBrkPos;
	PBREAKPOSITION pNewBrkPos;

	int iLastDowndPos = m_iDownloadFileSize;

	for (i = m_parDownloadBrks->GetSize() - 1; i >= 0; i --)
	{
		pBrkPos = m_parDownloadBrks->GetAt(i);
		
		if (pBrkPos->iEnd < iLastDowndPos)
		{
			pNewBrkPos = new BREAKPOSITION;

			pNewBrkPos->iEnd = iLastDowndPos;
			pNewBrkPos->iStart = pBrkPos->iEnd;

			m_arUnDownloadedBrks.InsertAt(0, pNewBrkPos);

			iLastDowndPos = pBrkPos->iStart;
		}
	}

	if (m_parDownloadBrks->GetSize() == 0)
	{
		//如果是一个新下载的文件，它没有以下载的纪录，就应该把这个文件长度作为一个已经下载的纪录。
		pNewBrkPos = new BREAKPOSITION;
		pNewBrkPos->iStart	= 0;
		pNewBrkPos->iEnd	= iLastDowndPos;

		m_arUnDownloadedBrks.InsertAt(0, pNewBrkPos);
	}

	//如果已知支持断点续传，那么可能需要对尚未下载的纪录进行分割
	CAppRegs *pAppRegs = ((CMyGetApp *)AfxGetApp())->GetAppRegs();

	//int iMinAssignSize = pAppRegs->GetVal(REG_GENERAL_BLOCKSIZE, &iMinAssignSize);
	//	RegProcess.GetDWORD(REG_GENERAL_BLOCK_SIZE);
	int iMinAssignSize;
	pAppRegs->GetVal(REG_GENERAL_BLOCKSIZE, (DWORD *)&iMinAssignSize);
	int iMaxBrkSize = -1;
	int iMaxBrkIndex = 0;

	
	//实际上，在刚开始，我们可能还不知道一个新的下载文件是否可以续传，
	//可以先进行分割。以后如果确切知道它不支持续传，可以再进行合并。
	/*
	//2002-03-29
	//如果不知道一个新的下载文件是否可以续传，就拒绝分割。
	if (m_iResumeState == SUPPORTRESUME)*/
	if (m_iResumeState != UNSUPPORTRESUME)
	{
		while(m_iSimJets > m_arUnDownloadedBrks.GetSize())
		{
			//找到最大的一块碎片
			for (i = 0; i < m_arUnDownloadedBrks.GetSize(); i ++)
			{
				pBrkPos = m_arUnDownloadedBrks[i];
				
				if (iMaxBrkSize < (pBrkPos->iEnd - pBrkPos->iStart))
				{
					iMaxBrkSize = pBrkPos->iEnd - pBrkPos->iStart;
					iMaxBrkIndex = i;
				}
			}

			if (iMaxBrkSize < iMinAssignSize * 2)
			{
				break;
			}

			pNewBrkPos = new BREAKPOSITION;
			pBrkPos = m_arUnDownloadedBrks[iMaxBrkIndex];
			pNewBrkPos->iEnd = pBrkPos->iEnd;
			pBrkPos->iEnd = ((pBrkPos->iStart + pBrkPos->iEnd) / 2 & (0xFFFFFFFF - BLOCK_BYTE_SIZE + 1));
			pNewBrkPos->iStart = pBrkPos->iEnd;

			m_arUnDownloadedBrks.InsertAt(iMaxBrkIndex + 1, pNewBrkPos);
		}
	}

	CString strDebug;

	TRACE("\r\n\r\n");
	for (int j = 0; j < m_arUnDownloadedBrks.GetSize(); j ++)
	{
		strDebug.Format("Download Range <%d>: From: %d --- To: %d \r\n", 
					j, m_arUnDownloadedBrks[j]->iStart,
					m_arUnDownloadedBrks[j]->iEnd);
		TRACE(strDebug);
	}

	TRACE("\r\n\r\n");

	return TRUE;
}

//线程申请新的下载区域。
BOOL CDownloadItemManager::prv_AssignDownloadRange(CDownloadJet *pJet)
{
	PBREAKPOSITION pOldBrkPos = pJet->GetRange();

	if (pOldBrkPos && pOldBrkPos->iEnd == -1)
	{
		//It is the temporty range, should be deleted.
		delete pOldBrkPos;
	}


	//如果有没有下载的碎片，则试图分配，否则返回FALSE，意味着该线程已经结束。
	if (m_arUnDownloadedBrks.GetSize() > 0)
	{
		//分配策略：如果有没有被线程使用的碎片，找到最大的一片，分配之。
		//否则，找到最大的一片，试图切分，成功，则将后一半分配之。
		PBREAKPOSITION pBrkPos;	

		int iFirstUnAssignedIndex = -1; 
		int iMaxAssignedIndex   = -1;

		//int iMaxUnAssignedSize	= -1;
		int iMaxAssignedSize	= -1;

		for (int i = 0; i < m_arUnDownloadedBrks.GetSize(); i ++)
		{	
			pBrkPos = m_arUnDownloadedBrks[i];

			if (prv_HasAssigned(pBrkPos))
			{
				if (iMaxAssignedSize < pBrkPos->iEnd - pBrkPos->iStart)
				{
					iMaxAssignedSize	= pBrkPos->iEnd - pBrkPos->iStart;
					iMaxAssignedIndex	= i;
				}
			}
			else
			{
				iFirstUnAssignedIndex	= i;

				break;
			}
		}

		if (iFirstUnAssignedIndex >= 0)
		{
			//如果有尚未分配的碎片，则分配给该线程。
			pBrkPos = m_arUnDownloadedBrks[iFirstUnAssignedIndex];

			pJet->SetRange(pBrkPos);

			CString strDebug;
			strDebug.Format("Assign Range to Thread: <%d>, Range from: %d --- TO: %d \r\n",
					prv_GetThreadIndex(pJet), pBrkPos->iStart, pBrkPos->iEnd);

			TRACE(strDebug);
		
			return TRUE;
		}
		else if (iMaxAssignedIndex >= 0)
		{
			//return FALSE;
			CAppRegs *pAppRegs = ((CMyGetApp *)AfxGetApp())->GetAppRegs();

			//int iMinAssignSize; = RegProcess.GetDWORD(REG_GENERAL_BLOCK_SIZE);
			int iMinAssignSize;
			pAppRegs->GetVal(REG_GENERAL_BLOCKSIZE, (DWORD *)&iMinAssignSize);

			//如果没有空余的碎片，则试图切割最大的一片。
			if (iMaxAssignedSize >= iMinAssignSize * 2)
			{
				PBREAKPOSITION pNewBrkPos = new BREAKPOSITION;
				pBrkPos = m_arUnDownloadedBrks[iMaxAssignedIndex];
				pNewBrkPos->iEnd = pBrkPos->iEnd;
				pBrkPos->iEnd = (pBrkPos->iStart + pBrkPos->iEnd) / 2;
				pNewBrkPos->iStart = pBrkPos->iEnd;

				m_arUnDownloadedBrks.InsertAt(iMaxAssignedIndex + 1, pNewBrkPos);

				pJet->SetRange(pNewBrkPos);

				CString strDebug;
				strDebug.Format("Assign Range to Thread: <%d>, Range from: %d --- TO: %d \r\n",
					prv_GetThreadIndex(pJet), pBrkPos->iStart, pBrkPos->iEnd);
	
				TRACE(strDebug);

				return TRUE;
			}
		}

	}
	

	return FALSE;
}


BOOL CDownloadItemManager::prv_HasAssigned(PBREAKPOSITION pBrkPos)
{
	for (int i = 0; i < m_arJets.GetSize(); i ++)
	{
		if (pBrkPos == m_arJets[i]->GetRange())
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CDownloadItemManager::prv_MergeUndownloadedBrks()
{
	//由于已经知道要下载的文件不支持续传，需要把所有的可以合并的碎片合成一块。
	//现在修改为，需要针对第一个线程为中心进行合并，因为有可能出现第一个线程已经在下载第3个区域的情况。
	//更严重的情况是，如果此时第一个线程已经停止的话，需要重新启动第一个线程。（以后再考虑）
	int iIndex = 0;

	PBREAKPOSITION pCurBrkPos;
	PBREAKPOSITION pNextBrkPos;

	CDownloadJet *pJet = m_arJets[0];
	PBREAKPOSITION pJet0Pos = pJet->GetRange();

	while (iIndex < m_arUnDownloadedBrks.GetSize() - 1)
	{
		pCurBrkPos	= m_arUnDownloadedBrks[iIndex];
		pNextBrkPos = m_arUnDownloadedBrks[iIndex + 1];

		if (pCurBrkPos->iEnd == pNextBrkPos->iStart && pJet0Pos != pNextBrkPos)
		{
			pCurBrkPos->iEnd = pNextBrkPos->iEnd;

			m_arUnDownloadedBrks.RemoveAt(iIndex + 1);

			delete pNextBrkPos;
		}
		else
		{
			iIndex ++;
		}
	}
}

BOOL CDownloadItemManager::prv_NewJet(BOOL bIsFirst)
{
	CDownloadJet *pJet;

	if (m_arJets.GetSize() >= m_iSimJets)
	{
		return FALSE;
	}

	if (!bIsFirst && (m_iDownloadFileSize == -1 || m_iResumeState == UNSUPPORTRESUME))
	{
		return FALSE;
	}

	//做一个判断，决定生成http或者ftp jet.
	CString strTemp = m_pListItem->GetURL();
	strTemp = strTemp.Left(6);
	strTemp.MakeLower();
	if( strTemp.Compare("ftp://") == 0 )
	{
		pJet = new CDownloadFtpJet(this);
	}
	else
	{
		pJet = new CDownloadHttpJet(this);
	}

//	pJet->SetNotifyWnd(m_pDownloadScheduler);

	pJet->SetURL(m_pListItem->GetURL());

	PBREAKPOSITION pBrkPos;
	if (m_iDownloadFileSize == -1)
	{
		pBrkPos = new BREAKPOSITION;
		pBrkPos->iStart = m_pListItem->GetFileDownloadedSize();
		pBrkPos->iEnd = -1;

		pJet->SetRange(pBrkPos);
	}
	else
	{
		if (!prv_AssignDownloadRange(pJet))
		{
			delete pJet;

			return FALSE;
		}
	}

	m_arJets.Add(pJet);

	pJet->SetRetry(RETRY_TYPE_ALWAYS, 3000);
	
	pJet->StartDownload();

	CString strNewThread;
	strNewThread.Format("New Thread <%d> ---- \r\n", prv_GetThreadIndex(pJet));

	TRACE(strNewThread);

	return TRUE;
}

void CDownloadItemManager::prv_ClearJob()
{
	int i;
	CDownloadJet *pJet;

	//Ask all the threads to stop.

	for (i = m_arJets.GetSize() - 1; i >= 0; i --)
	{
		pJet = m_arJets[i];

		pJet->StopDownload();
		
		//delete pJet;
	}


	//m_arJets.RemoveAll();

	

	if (m_pFile)
	{
		m_pFile->Flush();
		m_pFile->Close();

		if (m_pListItem->GetFileDownloadedSize() == m_pListItem->GetFileSize())
		{
			DeleteFile(m_strFilePath);

			m_pFile->Rename(m_strFilePath + ".jc!", m_strFilePath);
			m_pListItem->SetFileStatus(COMPLETED);

			m_pDownloadScheduler->RefreshMainframe(m_pListItem);

			//If the download item is completed, it should be moved to the specified category.
			m_pDownloadScheduler->MoveCompletedItem(m_pListItem);
		}

		delete m_pFile;
		m_pFile = NULL;
	}

	

	PBREAKPOSITION pBrkPos;
	for (i = 0 ; i < m_arUnDownloadedBrks.GetSize(); i ++)
	{	
		pBrkPos = m_arUnDownloadedBrks[i];

		delete pBrkPos;
		pBrkPos = NULL;
	}

	m_arUnDownloadedBrks.RemoveAll();
	
	//m_pMainScheduleThread->PostMessage(DOWNLOAD_JOB_END, (DWORD)this, 0);
	m_pDownloadScheduler->PostMessage(DOWNLOAD_JOB_END, (DWORD)this, 0);

}

void CDownloadItemManager::prv_UpdateDownloadBrkMap(CDownloadJet *pJet)
{
	//根据当前的线程的删除范围来更新已经下载的纪录，
	//注意：当前的线程的下载范围已经自动更新，如果该线程下载范围为空，不能删除，
	//要在CompleteJob事件中删除。

	PBREAKPOSITION pBrkPos = pJet->GetRange();

	int iStartPos = pBrkPos->iStart - pJet->GetCurDownloadedSize();

	//首先要在已经下载的纪录中查找同这次下载的开始地址相同的纪录。

	int iIndex = -1;
	BOOL bMatched = FALSE;

	for (int i = 0; i < m_parDownloadBrks->GetSize(); i ++)
	{
		if (m_parDownloadBrks->GetAt(i)->iEnd == iStartPos)
		{
			bMatched = TRUE;
			iIndex = i;

			break;
		}
		else if (m_parDownloadBrks->GetAt(i)->iEnd < iStartPos)
		{
			iIndex = i;
		}
	}

	if (bMatched)
	{
		m_parDownloadBrks->GetAt(iIndex)->iEnd = pBrkPos->iStart;
	}
	else
	{
		PBREAKPOSITION pNewBrkPos = new BREAKPOSITION;
	
		pNewBrkPos->iStart = iStartPos;
		pNewBrkPos->iEnd = pBrkPos->iStart;

		m_parDownloadBrks->InsertAt(iIndex + 1, pNewBrkPos);
	}
}

void CDownloadItemManager::prv_HandleThreadCompleted(CDownloadJet *pJet)
{
	PBREAKPOSITION pBrkPos = pJet->GetRange();

	for (int i = 0; i < m_arUnDownloadedBrks.GetSize(); i ++)
	{
		if (pBrkPos == m_arUnDownloadedBrks[i])
		{
			m_arUnDownloadedBrks.RemoveAt(i);

			break;
		}
	}

	delete pBrkPos;
	pJet->SetRange(NULL);


}


void CDownloadItemManager::prv_AttachJetInfo(CDownloadJet *pJet, LPCTSTR lpszMsg)
{
	int iIndex = prv_GetThreadIndex(pJet);

	if (iIndex > -1)
	{
		//GlobalLock((void *)lpszMsg);
		m_pListItem->AddJetInfo(iIndex, lpszMsg);
		//GlobalUnlock((void *)lpszMsg);
	}

	//GlobalFree((void *)lpszMsg);
	
}

CListItem * CDownloadItemManager::GetListItem()
{
	return m_pListItem;
}


/*
BOOL CDownloadItemManager::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	if (m_bQuit)
	{
		if (pMsg->message == WM_QUIT)
		{
			return CWinThread::PreTranslateMessage(pMsg);
		}
		else
		{
			return TRUE;
		}
	}

//	m_slockCanHandleMsg.Lock();


	CDownloadJet	*pJet = (CDownloadJet *)pMsg->wParam;
	LPCTSTR lpszMsg = (LPCTSTR)pMsg->lParam;

	switch(pMsg->message)
	{
	case WM_USER_DOWNLOAD_THREAD_GENERAL_INFO:
		prv_OnGeneralInfo(pJet, lpszMsg);
		break;
	
	case WM_USER_DOWNLOAD_THREAD_START_RECV:
		prv_OnStartRecv(pJet, lpszMsg);
		break;

	case WM_USER_DOWNLOAD_THREAD_RECV_OK:
		prv_OnRecvOK(pJet, lpszMsg);
		break;
		
	case WM_USER_DOWNLOAD_THREAD_COMPLETED:
		prv_OnJobCompleted(pJet, lpszMsg);
		break;

	case WM_USER_DOWNLOAD_THREAD_FAILED:
		prv_OnThreadFailed(pJet, lpszMsg);
		break;

	case WM_USER_DOWNLOAD_THREAD_CANCELED:
		prv_OnThreadCanceled(pJet, lpszMsg);
		break;

	case WM_USER_DOWNLOAD_THREAD_ERROR:
		prv_OnThreadResultFailed(pJet, lpszMsg);
		break;

	case DOWNLOAD_JOB_END:
		m_bQuit = TRUE;
		prv_ClearJob();
		//m_pMainScheduleThread->PostThreadMessage(DOWNLOAD_JOB_END, (DWORD)this, 0);		
		break;

	default:
//		m_slockCanHandleMsg.Unlock();
		return CWinThread::PreTranslateMessage(pMsg);

	}

	//m_pMainScheduleThread->RefreshMainFrame(m_pListItem);
	m_pDownloadScheduler->RefreshMainFrame(m_pListItem);

//	m_slockCanHandleMsg.Unlock();
		
	return TRUE;		
}
*/


BOOL CDownloadItemManager::AddDownloadEntry(CDownloadJet *pJet, SOCKET socket)
{
	return m_pDownloadScheduler->AddDownloadEntry(this, pJet, socket);
}

void CDownloadItemManager::RegisterTimeout(CDownloadJet *pJet, DWORD dwTimeout)
{
	m_pDownloadScheduler->RegisterTimeout(pJet, dwTimeout);
}

void CDownloadItemManager::UnregisterTimeout(CDownloadJet *pJet)
{
	m_pDownloadScheduler->UnregisterTimeout(pJet);
}



void CDownloadItemManager::RemoveDownloadEntry(CDownloadJet *pJet)
{
	m_pDownloadScheduler->RemoveDownloadEntry(pJet, this);
}

void CDownloadItemManager::prv_RefreshItem(CDownloadJet *pJet, LPCTSTR lpszMsg)
{
	if (lpszMsg)
	{
		prv_AttachJetInfo(pJet, lpszMsg);
	}

	m_pDownloadScheduler->RefreshMainframe(m_pListItem, 0);
	//m_pDownloadScheduler->PostMessage(WM_DOWNLOAD_ITEM_MSG, (WPARAM)m_pListItem, (LPARAM)0);
}
