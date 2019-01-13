// OptionSheet.cpp : implementation file
//

#include "stdafx.h"
#include "myget.h"
#include "OptionSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionSheet

IMPLEMENT_DYNAMIC(COptionSheet, CPropertySheet)

COptionSheet::COptionSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	prv_SetupSheet();
	prv_AddPages();
}

COptionSheet::COptionSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	prv_SetupSheet();
	prv_AddPages();
}

COptionSheet::~COptionSheet()
{
}


BEGIN_MESSAGE_MAP(COptionSheet, CPropertySheet)
	//{{AFX_MSG_MAP(COptionSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionSheet message handlers

void COptionSheet::prv_AddPages()
{
	AddPage(&m_pageGeneral);
	AddPage(&m_pageProxy);
	AddPage(&m_pageConnection);
	AddPage(&m_pageProtocol);
	AddPage(&m_pageMonitor);
	AddPage(&m_pageMirror);
	AddPage(&m_pageGraphLog);
	AddPage(&m_pageFileManagement);
	AddPage(&m_pageDialUp);
	AddPage(&m_pageSchedule);
	AddPage(&m_pageSound);
	AddPage(&m_pageLogin);
	AddPage(&m_pageOther);

}

void COptionSheet::prv_SetupSheet()
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW; 
	m_psh.dwFlags &= ~PSH_HASHELP; 

	LPCTSTR lpszCaption = APP_GET_RSCSTR("String", 61227);
	if (lpszCaption != NULL)
	{
		m_psh.pszCaption = lpszCaption;
	}
}

