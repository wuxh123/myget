// MyGet.h : main header file for the MYGET application
//

#if !defined(AFX_MYGET_H__A95B2DED_7FD0_4DB9_8AB0_DC0C7355BA0F__INCLUDED_)
#define AFX_MYGET_H__A95B2DED_7FD0_4DB9_8AB0_DC0C7355BA0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "CommonUtils.h"
#include "resource.h"       // main symbols
#include "AppRegs.h"
//class CMainScheduleThread;
class CDownloadSchedulerThread;
/////////////////////////////////////////////////////////////////////////////
// CMyGetApp:
// See MyGet.cpp for the implementation of this class
//



class CMyGetApp : public CWinApp
{
public:
	CDownloadSchedulerThread * GetDownloadSchedulerThread();
	CString m_strLauchFolder;
	LPCTSTR GetRscStr(LPCTSTR lpszSection, LPCTSTR lpszKey);
	LPCTSTR GetRscStr(LPCTSTR lpszSection, DWORD dwKey);
	CAppRegs * GetAppRegs();
	CString m_strDBName;
	CMyGetApp();

	BOOL SaveDB();
	//CDownloadScheduler *m_pDownloadScheduler;
	CDownloadSchedulerThread *m_pDownloadSchedulerThread;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyGetApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMyGetApp)
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CAppRegs m_AppRegs;
	BOOL prv_NewDefaultDB();
	BOOL prv_OpenDB(LPCTSTR lpszDBName);
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYGET_H__A95B2DED_7FD0_4DB9_8AB0_DC0C7355BA0F__INCLUDED_)
