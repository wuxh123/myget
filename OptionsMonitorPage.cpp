// OptionsMonitorPage.cpp : implementation file
//

#include "stdafx.h"
#include "myget.h"
#include "OptionsMonitorPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsMonitorPage property page

IMPLEMENT_DYNCREATE(COptionsMonitorPage, CPropertyPage)

COptionsMonitorPage::COptionsMonitorPage() : CPropertyPage(COptionsMonitorPage::IDD)
{
	//{{AFX_DATA_INIT(COptionsMonitorPage)
	m_bBrowserConfirm = FALSE;
	m_bBrowserFillCommentWithClipboardContent = FALSE;
	m_bBrowserIE = FALSE;
	m_bBrowserIEWidMatch = FALSE;
	m_bBrowserNetscape = FALSE;
	m_bBrowserOpenCancelledUrls = FALSE;
	m_bBrowserUseAlt = FALSE;
	m_bClipboardConfirm = FALSE;
	m_bClipboardMonitor = FALSE;
	m_strMonitorTypes = _T("");
	m_iBorwserIEMethod = -1;
	//}}AFX_DATA_INIT
	//m_psp.dwFlags |= PSP_USETITLE;
	//static CString strCaption;
	//strCaption = APP_GET_RSCSTR("Mon_PPG", "Title");
	//m_psp.pszTitle = strCaption;
	SET_PROPERTYPAGE_TITLE("Mon_PPG");

}

COptionsMonitorPage::~COptionsMonitorPage()
{
}

void COptionsMonitorPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsMonitorPage)
	DDX_Check(pDX, IDC_CHK_BROWSERCONFIRM, m_bBrowserConfirm);
	DDX_Check(pDX, IDC_CHK_BROWSERFILLCOMMENTWITHCLIPBOARDCONTENT, m_bBrowserFillCommentWithClipboardContent);
	DDX_Check(pDX, IDC_CHK_BROWSERIE, m_bBrowserIE);
	DDX_Check(pDX, IDC_CHK_BROWSERIEWILDMATCH, m_bBrowserIEWidMatch);
	DDX_Check(pDX, IDC_CHK_BROWSERNETSCAPE, m_bBrowserNetscape);
	DDX_Check(pDX, IDC_CHK_BROWSEROPENCANCELLEDURLS, m_bBrowserOpenCancelledUrls);
	DDX_Check(pDX, IDC_CHK_BROWSERUSEALT, m_bBrowserUseAlt);
	DDX_Check(pDX, IDC_CHK_CLIPBOARDCONFIRM, m_bClipboardConfirm);
	DDX_Check(pDX, IDC_CHK_CLIPBOARDMONITOR, m_bClipboardMonitor);
	DDX_Text(pDX, IDC_EDT_MONITORTYPES, m_strMonitorTypes);
	DDX_Radio(pDX, IDC_RD_BROWSERIEMETHOD1, m_iBorwserIEMethod);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsMonitorPage, CPropertyPage)
	//{{AFX_MSG_MAP(COptionsMonitorPage)
	ON_BN_CLICKED(IDC_CHK_BROWSERIE, OnChkBrowserie)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsMonitorPage message handlers


BOOL COptionsMonitorPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	ReloadResource();
	prv_FillDlg();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void COptionsMonitorPage::prv_FillDlg()
{
	CAppRegs *pAppRegs = ((CMyGetApp *)AfxGetApp())->GetAppRegs();
	
					
	m_bClipboardMonitor = pAppRegs->IsFlagOn(REG_FLAG_MONITOR_CLIPBOARDMONITOR);
	m_bClipboardConfirm = pAppRegs->IsFlagOn(REG_FLAG_MONITOR_CLIPBOARDCONFIRM);
	m_bBrowserNetscape	= pAppRegs->IsFlagOn(REG_FLAG_MONITOR_BROWSERNETSCAPE);
	m_bBrowserIE		= !pAppRegs->IsFlagOn(REG_FLAG_MONITOR_BROWSERIE);
	m_bBrowserConfirm	= pAppRegs->IsFlagOn(REG_FLAG_MONITOR_BROWSERCONFIRM);
	m_bBrowserUseAlt	= pAppRegs->IsFlagOn(REG_FLAG_MONITOR_BROWSERUSEALT);
	m_bBrowserOpenCancelledUrls
						= pAppRegs->IsFlagOn(REG_FLAG_MONITOR_BROWSEROPENCANCELLEDURLS);
	m_bBrowserFillCommentWithClipboardContent
						= pAppRegs->IsFlagOn(REG_FLAG_MONITOR_BROWSERFILLCOMMENTWITHCLIPBOARDCONTENT);
	m_iBorwserIEMethod  = pAppRegs->IsFlagOn(REG_FLAG_MONITOR_BROWSERIEMETHOD);
	m_bBrowserIEWidMatch= pAppRegs->IsFlagOn(REG_FLAG_MONITOR_BROWSERWILDMATCH);

	prv_UpdateIEOptions();



	//Monitor types
	LPSTR lpszMonitorTypes = NULL;
	pAppRegs->GetVal(REG_GENERAL_MONITORTYPE, &lpszMonitorTypes);
	m_strMonitorTypes = lpszMonitorTypes;

	UpdateData(FALSE);
}

void COptionsMonitorPage::ReloadResource()
{
	//Dlg Items
	LPCTSTR lpszDlgID = _T("Mon_PPG");

	SET_DLG_ITEM_TEXT(1197);
	SET_DLG_ITEM_TEXT(1066);
	SET_DLG_ITEM_TEXT(1065);
	SET_DLG_ITEM_TEXT(1198);
	SET_DLG_ITEM_TEXT(1124);
	SET_DLG_ITEM_TEXT(1125);

	SET_DLG_ITEM_TEXT(1126);
	SET_DLG_ITEM_TEXT(1127);
	SET_DLG_ITEM_TEXT(1199);
	SET_DLG_ITEM_TEXT(1218);
	SET_DLG_ITEM_TEXT(1008);
	SET_DLG_ITEM_TEXT(1229);

	SET_DLG_ITEM_TEXT(1230);
	SET_DLG_ITEM_TEXT(1095);
	SET_DLG_ITEM_TEXT(1272);
}

void COptionsMonitorPage::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);
	
	CAppRegs *pAppRegs = ((CMyGetApp *)AfxGetApp())->GetAppRegs();
						
	pAppRegs->SetFlag(REG_FLAG_MONITOR_CLIPBOARDMONITOR, m_bClipboardMonitor);
	pAppRegs->SetFlag(REG_FLAG_MONITOR_CLIPBOARDCONFIRM, m_bClipboardConfirm);
	pAppRegs->SetFlag(REG_FLAG_MONITOR_BROWSERNETSCAPE, m_bBrowserNetscape);
	pAppRegs->SetFlag(REG_FLAG_MONITOR_BROWSERIE, !m_bBrowserIE);
	pAppRegs->SetFlag(REG_FLAG_MONITOR_BROWSERCONFIRM, m_bBrowserConfirm);
	pAppRegs->SetFlag(REG_FLAG_MONITOR_BROWSERUSEALT, m_bBrowserUseAlt);
	pAppRegs->SetFlag(REG_FLAG_MONITOR_BROWSEROPENCANCELLEDURLS, m_bBrowserOpenCancelledUrls);
	pAppRegs->SetFlag(REG_FLAG_MONITOR_BROWSERFILLCOMMENTWITHCLIPBOARDCONTENT, m_bBrowserFillCommentWithClipboardContent);
	pAppRegs->SetFlag(REG_FLAG_MONITOR_BROWSERIEMETHOD, m_iBorwserIEMethod);
	pAppRegs->SetFlag(REG_FLAG_MONITOR_BROWSERWILDMATCH, m_bBrowserIEWidMatch);




	//Monitor types
	//LPSTR lpszMonitorTypes = NULL;
	//pAppRegs->GetVal(REG_GENERAL_MONITORTYPE, &lpszMonitorTypes);
	//m_strMonitorTypes = lpszMonitorTypes;
	pAppRegs->SetVal(REG_GENERAL_MONITORTYPE, m_strMonitorTypes);

	CPropertyPage::OnOK();
}

void COptionsMonitorPage::prv_UpdateIEOptions()
{
	GetDlgItem(IDC_RD_BROWSERIEMETHOD1)->EnableWindow(m_bBrowserIE);
	GetDlgItem(IDC_RD_BROWSERIEMETHOD2)->EnableWindow(m_bBrowserIE);
	GetDlgItem(IDC_CHK_BROWSERIEWILDMATCH)->EnableWindow(m_bBrowserIE);
	GetDlgItem(IDC_BTN_DEFAULTMANAGER)->EnableWindow(m_bBrowserIE);
}

void COptionsMonitorPage::OnChkBrowserie() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	prv_UpdateIEOptions();
}
