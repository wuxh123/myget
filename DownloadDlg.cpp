// DownloadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "myget.h"
#include "DownloadDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDownloadDlg

IMPLEMENT_DYNAMIC(CDownloadDlg, CPropertySheet)

CDownloadDlg::CDownloadDlg(LPCTSTR pszCaption, int iDlgType, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	InitDlg(iDlgType);
}

CDownloadDlg::~CDownloadDlg()
{
}


BEGIN_MESSAGE_MAP(CDownloadDlg, CPropertySheet)
	//{{AFX_MSG_MAP(CDownloadDlg)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDownloadDlg message handlers

void CDownloadDlg::prv_SetupSheet()
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW; 
	m_psh.dwFlags &= ~PSH_HASHELP; 

}

void CDownloadDlg::prv_AddPages()
{
	AddPage(&m_pageProperty);
}

void CDownloadDlg::InitDlg(int iDlgType)
{
	m_pageProperty.m_iDlgType = iDlgType;
	prv_SetupSheet();
	prv_AddPages();

}

