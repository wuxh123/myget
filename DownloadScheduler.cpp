// DownloadSchudler.cpp : implementation file
//

#include "stdafx.h"
#include "myget.h"
#include "DownloadJet.h"
#include "DownloadItemManager.h"
#include "TreeItem.h"
#include "ListItem.h"
#include "MainFrm.h"
#include "DownloadScheduler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDownloadScheduler
IMPLEMENT_DYNCREATE(CDownloadScheduler, CWnd)

CDownloadScheduler::CDownloadScheduler() : CWnd(), 
		m_slockCanGetDataCount(&m_crtsectCanGetDataCount)
{
	//m_pMainFrame = (CMainFrame *)pMainFrame;
	//m_hMainWnd = hMainWnd;
	m_mapDownloadItemMNGs.RemoveAll();
	m_iCountOfLastDownloadData = 0;
	
}

CDownloadScheduler::~CDownloadScheduler()
{
	POSITION pos = m_mapDownloadItemMNGs.GetStartPosition();

	CListItem *pListItem;
	CDownloadItemManager *pMNG;
	while(pos)
	{
		m_mapDownloadItemMNGs.GetNextAssoc(pos, pListItem, pMNG);

		delete pMNG;
	}

}


BEGIN_MESSAGE_MAP(CDownloadScheduler, CWnd)
	//{{AFX_MSG_MAP(CDownloadScheduler)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_WM_TIMER()
	ON_MESSAGE(WM_USER_ASYNC_SOCKET_MSG, OnAsyncSocketMsg)
	ON_MESSAGE(WM_DOWNLOAD_ITEM_MSG, OnDownloadItemMsg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDownloadScheduler message handlers

void CDownloadScheduler::IncLastDownloadDataCounter(int iInc)
{
	m_slockCanGetDataCount.Lock();
	m_iCountOfLastDownloadData += iInc;
	m_slockCanGetDataCount.Unlock();
}

void CDownloadScheduler::MoveCompletedItem(CListItem *pListItem)
{
	RefreshMainframe(pListItem, 1);
}

void CDownloadScheduler::prv_ClearLastDownloadDataCounter()
{
	m_slockCanGetDataCount.Lock();
	m_iCountOfLastDownloadData = 0;
	m_slockCanGetDataCount.Unlock();	
}

void CDownloadScheduler::Quit()
{

}

void CDownloadScheduler::SetDownloadTreeItem(CTreeItem *pTreeItem)
{
	m_pTreeItem = pTreeItem;
}

void CDownloadScheduler::RefreshMainframe(CListItem *pListItem, UINT uiState)
{
	if (pListItem)
	{
		pListItem->SetDirty(TRUE);
		if (uiState == 1)
		{
			::PostMessage(m_hMainWnd, WM_USER_DOWNLOAD_THREAD_REFRESH_MAIN_FRAME, (WPARAM)pListItem, uiState);
		}
		//::PostMessage(m_hMainWnd, WM_USER_DOWNLOAD_THREAD_REFRESH_MAIN_FRAME, (WPARAM)pListItem, uiState);
		//::SendMessage(m_hMainWnd, WM_USER_DOWNLOAD_THREAD_REFRESH_MAIN_FRAME, (WPARAM)pListItem, uiState);
	}

}

int CDownloadScheduler::prv_GetLastDownloadDataCount()
{
	return m_iCountOfLastDownloadData;
}

void CDownloadScheduler::prv_RemoveDownloadEntry(CDownloadItemManager *pMNG)
{
	CListItem *pListItem  = pMNG->GetListItem();

	m_mapDownloadItemMNGs.RemoveKey(pListItem);
}

BOOL CDownloadScheduler::prv_IsValidateDownloadEntry(CDownloadItemManager *pMNG)
{
	CListItem *pListItem;
	CDownloadItemManager *pMNGEntry;

	BOOL bIsValidateEntry = FALSE;

	POSITION pos = m_mapDownloadItemMNGs.GetStartPosition();

	while (pos)
	{
		m_mapDownloadItemMNGs.GetNextAssoc(pos, pListItem, pMNGEntry);

		if (pMNGEntry == pMNG)
		{
			bIsValidateEntry= TRUE;

			break;
		}
		
	}

	return bIsValidateEntry;}

void CDownloadScheduler::prv_StopDownload(CListItem *pListItem)
{

	CDownloadItemManager *pMNG;
	m_mapDownloadItemMNGs.Lookup(pListItem, pMNG);
	
	prv_RemoveDownloadEntry(pMNG);
	pMNG->StopDownload();
	delete pMNG;

}

void CDownloadScheduler::prv_NeedPauseDownloadingItems()
{
	CListItem *pListItem;

	for (int i = m_pTreeItem->GetSubListItemCount() - 1; i >= 0; i --)
	{
		pListItem = m_pTreeItem->GetSubListItem(i);

		if (prv_IsItemInDownloadList(pListItem))
		{
			if (pListItem->GetStartState() == START_MANUAL)
			{
				prv_StopDownload(pListItem);
			}
		}

	}

}

void CDownloadScheduler::prv_DoSchedule()
{

	//TRACE("Main Schedule Thread are running!\r\n");	
		
	//首先检查计划下载的item.
	prv_CheckScheduledItem();

	//然后让第一个准备好的item（如果有的,而且不在当前下载队列中话，前提是没有达到下载job的上限）进行下载。
	prv_StartDownloadReadyItems();
	prv_NeedPauseDownloadingItems();

}

void CDownloadScheduler::prv_StartDownload(CListItem *pListItem)
{
	CDownloadItemManager *pMNG = new CDownloadItemManager(this, pListItem);
	
	m_mapDownloadItemMNGs.SetAt(pListItem, pMNG);

	pMNG->StartDownload();
}

BOOL CDownloadScheduler::prv_IsItemInDownloadList(CListItem *pListItem)
{
	CDownloadItemManager *pMNG;
	
	return m_mapDownloadItemMNGs.Lookup(pListItem, pMNG);
}

void CDownloadScheduler::prv_StartDownloadReadyItems()
{
	CListItem *pListItem;

	for (int i = m_pTreeItem->GetSubListItemCount() - 1; i >= 0; i --)
	{
		pListItem = m_pTreeItem->GetSubListItem(i);

		if (pListItem->GetStatus() == ITEM_STATUS_READY
			&& !prv_IsItemInDownloadList(pListItem))
		{
			prv_StartDownload(pListItem);
		}

	}
}

void CDownloadScheduler::prv_CheckScheduledItem()
{

}

void CDownloadScheduler::StartDownload()
{
	//生成Timer.
	SetTimer(ID_1sTimer, 1000, 0);
	SetTimer(CDownloadScheduler::ID_SchedulerTimer, SchedulerTimerSlot, 0);
}

void CDownloadScheduler::StopDownload()
{
	//停止Timer.
	KillTimer(ID_SchedulerTimer);
	KillTimer(CDownloadScheduler::ID_1sTimer);

	prv_ClearLastDownloadDataCounter();
	//通知所有的DownloadItemManager停止下载。
	POSITION pos = m_mapDownloadItemMNGs.GetStartPosition();

	CListItem *pListItem;
	CDownloadItemManager *pMNG;
	while(pos)
	{
		m_mapDownloadItemMNGs.GetNextAssoc(pos, pListItem, pMNG);

		pMNG->StopDownload();
	}
}

void CDownloadScheduler::OnTimer(UINT nIDEvent)
{

	switch(nIDEvent)
	{
		case ID_SchedulerTimer:
			prv_DoSchedule();
			break;

		case ID_1sTimer:
//			m_pMainFrame->PostMessage(WM_USER_REFRESH_SPEED, 0, prv_GetLastDownloadDataCount());
			::PostMessage(m_hMainWnd, WM_USER_REFRESH_SPEED, 0, prv_GetLastDownloadDataCount());

			prv_ClearLastDownloadDataCounter();
			prv_CheckTimeout();
			break;

		default:
			break;
	}

	CWnd::OnTimer(nIDEvent);
}




BOOL CDownloadScheduler::AddDownloadEntry(CDownloadItemManager *pMNG, CDownloadJet *pJet, SOCKET socket)
{
	//在这里，要把(DownloadJet *, DownloadIteNanager*)加入到m_mapDownloadJets中。
	m_mapDownloadJets.SetAt(pJet, pMNG);

	//并把DownloadJet的socket加入到异步选择中去。
	//if (WSAAsyncSelect(pJet->GetSocket(), GetSafeHwnd(), WM_USER_ASYNC_SOCKET_MSG, FD_CONNECT | FD_READ | FD_CLOSE)
	//	== SOCKET_ERROR)
	if (WSAAsyncSelect(socket, GetSafeHwnd(), WM_USER_ASYNC_SOCKET_MSG, FD_CONNECT | FD_READ | FD_CLOSE)
		== SOCKET_ERROR)
	{
		return FALSE;
	}

	return TRUE;
}


LRESULT CDownloadScheduler::OnAsyncSocketMsg(WPARAM wParam, LPARAM lParam)
{
	POSITION pos = m_mapDownloadJets.GetStartPosition();

	CDownloadJet *pJet;
	CDownloadItemManager *pMNG;

	while(pos)
	{
		m_mapDownloadJets.GetNextAssoc(pos, pJet, pMNG);

		//if ((int)wParam == pJet->GetSocket())
		if (pJet->OwnSocket((SOCKET)wParam))
		{
			//WSAAsyncSelect(pJet->GetSocket(), GetSafeHwnd(), 0, 0);
			WSAAsyncSelect((SOCKET)wParam, GetSafeHwnd(), 0, 0);

			int rcEvent = pJet->DownloadHandler(wParam, lParam);
			if (rcEvent != 0)
			{
				//WSAAsyncSelect(pJet->GetSocket(), GetSafeHwnd(), WM_USER_ASYNC_SOCKET_MSG, rcEvent);
				WSAAsyncSelect((SOCKET)wParam, GetSafeHwnd(), WM_USER_ASYNC_SOCKET_MSG, rcEvent);
			}

			break;
		}
	}



	return CWnd::Default();
	//return 1;
}

void CDownloadScheduler::RegisterTimeout(CDownloadJet *pJet, DWORD dwTimeout)
{
	m_mapJetRegestedTimeout.SetAt(pJet, dwTimeout);
}

void CDownloadScheduler::prv_CheckTimeout()
{
	POSITION pos = m_mapJetRegestedTimeout.GetStartPosition();

	CDownloadJet *pJet;
	DWORD dwTimeout;

	while(pos)
	{
		m_mapJetRegestedTimeout.GetNextAssoc(pos, pJet, dwTimeout);

		dwTimeout -= 1000;

		if (dwTimeout <= 0)
		{
			UnregisterTimeout(pJet);

			pJet->Timeout();
		}
		else
		{
			m_mapJetRegestedTimeout.SetAt(pJet, dwTimeout);
		}
	}
}

void CDownloadScheduler::UnregisterTimeout(CDownloadJet *pJet)
{
	DWORD dwTimeout;

	if (m_mapJetRegestedTimeout.Lookup(pJet, dwTimeout))
	{
		m_mapJetRegestedTimeout.RemoveKey(pJet);
	}
}

void CDownloadScheduler::RemoveDownloadEntry(CDownloadJet *pJet, CDownloadItemManager *pMNG)
{
	CDownloadItemManager *pTmpMNG;

	if (m_mapDownloadJets.Lookup(pJet, pTmpMNG))
	{
		m_mapDownloadJets.RemoveKey(pJet);
	}
}


LRESULT CDownloadScheduler::OnDownloadItemMsg(WPARAM wParam, LPARAM lParam)
{
	RefreshMainframe((CListItem *)wParam, lParam);
	return 1;
}

BOOL CDownloadScheduler::PreCreateWindow(CREATESTRUCT &cs)
{
	return CWnd::PreCreateWindow(cs); 
}

void CDownloadScheduler::SetNotifyWnd(HWND hNotifyWnd)
{
	m_hMainWnd = hNotifyWnd;
}
