// FlashGetTray.cpp: implementation of the CFlashGetTray class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "myget.h"
#include "FlashGetTray.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFlashGetTray::CFlashGetTray(CWnd *pParentWnd, UINT uID, UINT uFlags, UINT uCallbackMessage, LPCTSTR lpszTip)
{
	m_ilTray.Create(32, 32, ILC_COLOR8 | ILC_MASK, 3, 0);

	for (int i = 0; i < 3; i ++)
	{
		m_ilTray.Add(AfxGetApp()->LoadIcon(IDI_TRAY1 + i));
	}

	//m_ilTray.SetBkColor(GetSysColor(COLOR_BTNFACE));

	CMainFrame *pMainFrame				= (CMainFrame *)pParentWnd;
	m_NotifyIconData.cbSize				= sizeof(NOTIFYICONDATA);
	m_NotifyIconData.hWnd				= pParentWnd->GetSafeHwnd();
	m_NotifyIconData.uFlags				= uFlags;
	m_NotifyIconData.uID				= uID;
	m_NotifyIconData.uCallbackMessage	= uCallbackMessage;
	m_NotifyIconData.hIcon				= m_ilTray.ExtractIcon(0);
	strncpy(m_NotifyIconData.szTip, lpszTip, sizeof(m_NotifyIconData.szTip));

	::Shell_NotifyIcon(NIM_ADD, &m_NotifyIconData);
}

CFlashGetTray::~CFlashGetTray()
{
	::Shell_NotifyIcon(NIM_DELETE, &m_NotifyIconData);
	
}


