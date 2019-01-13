// DownloadSchedulerThread.cpp : implementation file
//

#include "stdafx.h"
#include "myget.h"
#include "TreeItem.h"
#include "DownloadScheduler.h"
#include "DownloadSchedulerThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDownloadSchedulerThread

IMPLEMENT_DYNCREATE(CDownloadSchedulerThread, CWinThread)

CDownloadSchedulerThread::CDownloadSchedulerThread()
{
	static CString strMyClass = AfxRegisterWndClass(
     CS_VREDRAW | CS_HREDRAW);

	m_pDownloadScheduler = new CDownloadScheduler();
	m_pMainWnd = m_pDownloadScheduler;

//	m_pDownloadScheduler->CreateEx(0, strMyClass, "a", WS_POPUP, 
//		CRect(5, 5, 3, 5), CWnd::GetDesktopWindow(), 0, NULL);

		m_pDownloadScheduler->CreateEx(0, strMyClass, "a", WS_POPUP, 
		CRect(5, 5, 3, 5), NULL, 0, NULL);

	

}

CDownloadSchedulerThread::~CDownloadSchedulerThread()
{
	TRACE("DownloadSchedulerThread exists!\n");
	//m_pDownloadScheduler->DestroyWindow();
	delete m_pDownloadScheduler;
}

BOOL CDownloadSchedulerThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	TRACE("Thread InitInstance is called!\n");
		
	TRACE("Thread InitInstance quit!\n");
	return TRUE;
}

int CDownloadSchedulerThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
//	delete m_pDownloadScheduler;
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CDownloadSchedulerThread, CWinThread)
	//{{AFX_MSG_MAP(CDownloadSchedulerThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDownloadSchedulerThread message handlers

void CDownloadSchedulerThread::StopDownload()
{
	m_pDownloadScheduler->StopDownload();
}

void CDownloadSchedulerThread::SetDownloadTreeItem(CTreeItem *pTreeItem)
{
	m_pDownloadScheduler->SetDownloadTreeItem(pTreeItem);
}

void CDownloadSchedulerThread::StartDownload()
{
	m_pDownloadScheduler->StartDownload();
}



void CDownloadSchedulerThread::SetNotifyWnd(HWND hNotifyWnd)
{
	m_hNotifyWnd = hNotifyWnd;
	m_pDownloadScheduler->SetNotifyWnd(hNotifyWnd);
}


