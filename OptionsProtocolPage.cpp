// OptionsProtocolPage.cpp : implementation file
//

#include "stdafx.h"
#include "myget.h"
#include "OptionsProtocolPage.h"
#include "AppRegs.h"
#include "CommonUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsProtocolPage property page

IMPLEMENT_DYNCREATE(COptionsProtocolPage, CPropertyPage)

COptionsProtocolPage::COptionsProtocolPage() : CPropertyPage(COptionsProtocolPage::IDD)
{
	//{{AFX_DATA_INIT(COptionsProtocolPage)
	m_boolEnableCookie = FALSE;
	m_strAsciiMode = _T("");
	m_strEmail = _T("");
	m_strUserDefineAgent = _T("");
	m_iUserAgent = -1;
	//}}AFX_DATA_INIT
	//m_psp.dwFlags |= PSP_USETITLE;
	//static CString strCaption;
	//strCaption = APP_GET_RSCSTR("Pro_PPG", "Title");
	//m_psp.pszTitle = strCaption;

	SET_PROPERTYPAGE_TITLE("Pro_PPG");
}

COptionsProtocolPage::~COptionsProtocolPage()
{
}

void COptionsProtocolPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsProtocolPage)
	DDX_Check(pDX, IDC_CHECK_ENABLE_COOKIE, m_boolEnableCookie);
	DDX_Text(pDX, IDC_EDIT_ASCII_MODE, m_strAsciiMode);
	DDX_Text(pDX, IDC_EDIT_EMAIL, m_strEmail);
	DDX_Text(pDX, IDC_EDIT_USER_DEFINE_AGENT, m_strUserDefineAgent);
	DDX_CBIndex(pDX, IDC_CBX_USER_AGENT, m_iUserAgent);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsProtocolPage, CPropertyPage)
	//{{AFX_MSG_MAP(COptionsProtocolPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsProtocolPage message handlers

BOOL COptionsProtocolPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	ReloadResource();
	prv_FillDlg();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionsProtocolPage::prv_FillDlg()
{
	CAppRegs *pAppRegs = ((CMyGetApp *)AfxGetApp())->GetAppRegs();

	LPTSTR lpszBuf;
	
	lpszBuf = NULL;
	pAppRegs->GetVal(REG_PROTOCOL_ASCIIMODE, &lpszBuf);
	m_strAsciiMode = lpszBuf;

	lpszBuf = NULL;
	pAppRegs->GetVal(REG_PROTOCOL_EMAIL, &lpszBuf);
	m_strEmail = lpszBuf;

	lpszBuf = NULL;
	pAppRegs->GetVal(REG_PROTOCOL_USERAGENTTEXT, &lpszBuf);
	m_strUserDefineAgent = lpszBuf;

	m_boolEnableCookie = pAppRegs->IsFlagOn(REG_FLAG_GENERAL_EXT_ENABLE_COOKIE);

	pAppRegs->GetVal(REG_PROTOCOL_USERAGENT, (DWORD *)&m_iUserAgent);
	UpdateData(FALSE);
}

void COptionsProtocolPage::ReloadResource()
{
	//Dlg Items
	LPCTSTR lpszDlgID = _T("Pro_PPG");

	SET_DLG_ITEM_TEXT(1198);
	SET_DLG_ITEM_TEXT(1200);
	SET_DLG_ITEM_TEXT(1201);
	SET_DLG_ITEM_TEXT(1182);
	SET_DLG_ITEM_TEXT(1202);

	CComposedStrings strAgens(61271);
	
	for (int i = 0; i < strAgens.GetSubStringCount(); i ++)
	{
		((CComboBox *)GetDlgItem(IDC_CBX_USER_AGENT))->AddString(strAgens.GetSubString(i));
	}
}

void COptionsProtocolPage::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);

	CAppRegs *pAppRegs = ((CMyGetApp *)AfxGetApp())->GetAppRegs();

	pAppRegs->SetVal(REG_PROTOCOL_ASCIIMODE, m_strAsciiMode);
	pAppRegs->SetVal(REG_PROTOCOL_EMAIL, m_strEmail);
	pAppRegs->SetVal(REG_PROTOCOL_USERAGENTTEXT, m_strUserDefineAgent);

	pAppRegs->SetFlag(REG_FLAG_GENERAL_EXT_ENABLE_COOKIE, m_boolEnableCookie);

	pAppRegs->SetVal(REG_PROTOCOL_USERAGENT, (DWORD)m_iUserAgent);

	CPropertyPage::OnOK();
}
