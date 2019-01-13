// OptionsMirrorPage.cpp : implementation file
//

#include "stdafx.h"
#include "myget.h"
#include "OptionsMirrorPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsMirrorPage property page

IMPLEMENT_DYNCREATE(COptionsMirrorPage, CPropertyPage)

COptionsMirrorPage::COptionsMirrorPage() : CPropertyPage(COptionsMirrorPage::IDD)
{
	//{{AFX_DATA_INIT(COptionsMirrorPage)
	m_bAutoCalcAddMirrors = FALSE;
	m_bAutoSearchMirrorSites = FALSE;
	m_bAutoSelectMirror = FALSE;
	m_bEnableMirror = FALSE;
	m_iFtpSearchMaxHit = 0;
	m_iFtpSearchMinSize = 0;
	//}}AFX_DATA_INIT
	//m_psp.dwFlags |= PSP_USETITLE;
	//static CString strCaption;
	//strCaption = APP_GET_RSCSTR("Mirrors_PPG", "Title");
	//m_psp.pszTitle = strCaption;
	SET_PROPERTYPAGE_TITLE("Mirrors_PPG");

}

COptionsMirrorPage::~COptionsMirrorPage()
{
}

void COptionsMirrorPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsMirrorPage)
	DDX_Control(pDX, IDC_SPIN_MINSIZE, m_spinMinSize);
	DDX_Control(pDX, IDC_SPIN_MAXHIT, m_spinMaxHit);
	DDX_Check(pDX, IDC_CHK_AUTOCALCADDMIRRORS, m_bAutoCalcAddMirrors);
	DDX_Check(pDX, IDC_CHK_AUTOSEARCHMIRRORSITES, m_bAutoSearchMirrorSites);
	DDX_Check(pDX, IDC_CHK_AUTOSELECTMIRROR, m_bAutoSelectMirror);
	DDX_Check(pDX, IDC_CHK_ENABLEMIRROW, m_bEnableMirror);
	DDX_Text(pDX, IDC_EDT_FTPSEARCHMAXHIT, m_iFtpSearchMaxHit);
	DDV_MinMaxInt(pDX, m_iFtpSearchMaxHit, 5, 100);
	DDX_Text(pDX, IDC_EDT_FTPSEARCHMINSIZE, m_iFtpSearchMinSize);
	DDV_MinMaxInt(pDX, m_iFtpSearchMinSize, 10, 102400);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsMirrorPage, CPropertyPage)
	//{{AFX_MSG_MAP(COptionsMirrorPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COptionsMirrorPage message handlers

BOOL COptionsMirrorPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	ReloadResource();
	prv_FillDlg();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionsMirrorPage::prv_FillDlg()
{
		
	CAppRegs *pAppRegs = ((CMyGetApp *)AfxGetApp())->GetAppRegs();
	
					
	m_bEnableMirror				= pAppRegs->IsFlagOn(REG_FLAG_GENERAL_EXT_ENABLEMIRROWS);
	m_bAutoSearchMirrorSites	= pAppRegs->IsFlagOn(REG_FLAG_GENERAL_EXT_AUTOSEARCHMIRRORSITE);
	m_bAutoCalcAddMirrors		= pAppRegs->IsFlagOn(REG_FLAG_GENERAL_EXT_AUTOCALCADDMIRROR);
	m_bAutoSelectMirror			= pAppRegs->IsFlagOn(REG_FLAG_GENERAL_EXT_AUTOSELECTMIRRORSITE);

	pAppRegs->GetVal(REG_GENERAL_FTPSEARCHMINSIZE, (DWORD *)&m_iFtpSearchMinSize);

	UDACCEL udAccelMinSize;
	m_spinMinSize.GetAccel(1, &udAccelMinSize);
	udAccelMinSize.nInc = 100;
	m_spinMinSize.SetAccel(1, &udAccelMinSize);
	m_spinMinSize.SetRange32(10, 102400);


	pAppRegs->GetVal(REG_GENERAL_FTPSEARCHMAXHIT, (DWORD *)&m_iFtpSearchMaxHit);
	UDACCEL udAccelMaxHit;
	m_spinMaxHit.GetAccel(1, &udAccelMaxHit);
	udAccelMaxHit.nInc = 5;
	m_spinMaxHit.SetAccel(1, &udAccelMaxHit);
	m_spinMaxHit.SetRange(5, 100);


	UpdateData(FALSE);
}

void COptionsMirrorPage::ReloadResource()
{
	//Dlg Items
	LPCTSTR lpszDlgID = _T("Mirrors_PPG");

	SET_DLG_ITEM_TEXT(1236);
	SET_DLG_ITEM_TEXT(1237);
	SET_DLG_ITEM_TEXT(1197);
	SET_DLG_ITEM_TEXT(1239);
	SET_DLG_ITEM_TEXT(1199);
	SET_DLG_ITEM_TEXT(1202);
	SET_DLG_ITEM_TEXT(1200);
	SET_DLG_ITEM_TEXT(1201);
	SET_DLG_ITEM_TEXT(1244);
	SET_DLG_ITEM_TEXT(1203);


}
