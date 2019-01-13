// OptionsConnectionPage.cpp : implementation file
//

#include "stdafx.h"
#include "myget.h"
#include "OptionsConnectionPage.h"
#include "AppRegs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsConnectionPage property page

IMPLEMENT_DYNCREATE(COptionsConnectionPage, CPropertyPage)

COptionsConnectionPage::COptionsConnectionPage() : CPropertyPage(COptionsConnectionPage::IDD)
{
	//{{AFX_DATA_INIT(COptionsConnectionPage)
	m_iConnectionTimeOut = 0;
	m_iDataTimeOut = 0;
	m_boolStopAfter99NoData = FALSE;
	m_boolAlwaysRetry = FALSE;
	m_boolAlwaysObeySpeedLimit = FALSE;
	m_iMaxSimJobs = 0;
	m_iNumOfRetries = 0;
	m_iRetryDelay = 0;
	m_dblSpeedLimit = 0.0;
	//}}AFX_DATA_INIT
	//m_psp.dwFlags |= PSP_USETITLE;
	//static CString strCaption;
	//strCaption = APP_GET_RSCSTR("Conn_PPG", "Title");
	//m_psp.pszTitle = strCaption;
	SET_PROPERTYPAGE_TITLE("Conn_PPG");


}

COptionsConnectionPage::~COptionsConnectionPage()
{
}

void COptionsConnectionPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsConnectionPage)
	DDX_Control(pDX, IDC_SPIN_RETRY_DELAY, m_spinRetryDelay);
	DDX_Control(pDX, IDC_SPIN_NUM_OF_RETRIES, m_spinNumOfRetries);
	DDX_Control(pDX, IDC_SPIN_DATA_TIME_OUT, m_spinDataTimeOut);
	DDX_Control(pDX, IDC_SPIN_MAX_SIM_JOBS, m_spinMaxSimJobs);
	DDX_Control(pDX, IDC_SPIN_CONNECTION_TIME_OUT, m_spinConnectionTimeOut);
	DDX_Text(pDX, IDC_EDIT_CONNECTON_TIME_OUT, m_iConnectionTimeOut);
	DDV_MinMaxInt(pDX, m_iConnectionTimeOut, 30, 600);
	DDX_Text(pDX, IDC_EDIT_DATA_TIME_OUT, m_iDataTimeOut);
	DDV_MinMaxInt(pDX, m_iDataTimeOut, 30, 600);
	DDX_Check(pDX, IDC_CHECK_STOP_AFTER_99_NO_DATA, m_boolStopAfter99NoData);
	DDX_Check(pDX, IDC_CHECK_ALWAYS_RETRY, m_boolAlwaysRetry);
	DDX_Check(pDX, IDC_CHECK_ALWARYS_OBEY_SPEED_LIMIT, m_boolAlwaysObeySpeedLimit);
	DDX_Text(pDX, IDC_EDIT_MAX_SIM_JOBS, m_iMaxSimJobs);
	DDV_MinMaxInt(pDX, m_iMaxSimJobs, 1, 8);
	DDX_Text(pDX, IDC_EDIT_NUM_OF_RETRIES, m_iNumOfRetries);
	DDV_MinMaxInt(pDX, m_iNumOfRetries, 0, 9999);
	DDX_Text(pDX, IDC_EDIT_RETRY_DELAY, m_iRetryDelay);
	DDV_MinMaxInt(pDX, m_iRetryDelay, 1, 600);
	DDX_Text(pDX, IDC_EDIT_SPEED_LIMIT, m_dblSpeedLimit);
	DDV_MinMaxDouble(pDX, m_dblSpeedLimit, 0.1, 102400.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsConnectionPage, CPropertyPage)
	//{{AFX_MSG_MAP(COptionsConnectionPage)
	ON_BN_CLICKED(IDC_CHECK_ALWAYS_RETRY, OnCheckAlwaysRetry)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsConnectionPage message handlers

BOOL COptionsConnectionPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	ReloadResource();
	prv_FillDlg();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionsConnectionPage::prv_UpdateRetryOptions()
{
	GetDlgItem(IDC_EDIT_NUM_OF_RETRIES)->EnableWindow(!m_boolAlwaysRetry);
	GetDlgItem(IDC_CHECK_STOP_AFTER_99_NO_DATA)->EnableWindow(m_boolAlwaysRetry);
}

void COptionsConnectionPage::OnCheckAlwaysRetry() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	prv_UpdateRetryOptions();

	UpdateData(FALSE);
}

void COptionsConnectionPage::prv_FillDlg()
{
	//CRegProcess RegProcess;
	CAppRegs *pAppRegs = ((CMyGetApp *)AfxGetApp())->GetAppRegs();

	//m_iConnectionTimeOut	= RegProcess.GetDWORD(REG_CONN_CONNECTION_TIME_OUT);
	pAppRegs->GetVal(REG_CONNECTION_CT, (DWORD *)&m_iConnectionTimeOut);
	//m_iDataTimeOut			= RegProcess.GetDWORD(REG_CONN_DATA_TIME_OUT);
	pAppRegs->GetVal(REG_CONNECTION_DT, (DWORD *)&m_iDataTimeOut);

	//m_iRetryDelay			= RegProcess.GetDWORD(REG_CONN_RETRY_DELAY);
	pAppRegs->GetVal(REG_CONNECTION_WT, (DWORD *)&m_iRetryDelay);

	//m_iMaxSimJobs			= RegProcess.GetDWORD(REG_CONN_MAX_SIM_JOBS);
	pAppRegs->GetVal(REG_CONNECTION_SIMJOBS, (DWORD *)&m_iMaxSimJobs);

	//m_iNumOfRetries			= RegProcess.GetDWORD(REG_CONN_NUM_OF_RETRIES);
	pAppRegs->GetVal(REG_CONNECTION_RETRY, (DWORD *)&m_iNumOfRetries);

	//m_dblSpeedLimit			= (double)RegProcess.GetDWORD(REG_CONN_SPEED_LIMIT) / 1024;
	DWORD dwSpeedLimit;
	pAppRegs->GetVal(REG_CONNECTION_SPEEDLIMIT, &dwSpeedLimit);
	char szTempDouble[100];
	m_dblSpeedLimit = (double)dwSpeedLimit / 1024;
	sprintf(szTempDouble, "%.5f", m_dblSpeedLimit);
	m_dblSpeedLimit = atof(szTempDouble);


	m_spinConnectionTimeOut.SetRange(30, 600);
	m_spinDataTimeOut.SetRange(30, 600);
	m_spinMaxSimJobs.SetRange(1, 8);
	m_spinNumOfRetries.SetRange(0, 9999);
	m_spinRetryDelay.SetRange(1, 600);

	m_boolAlwaysObeySpeedLimit = pAppRegs->IsFlagOn(REG_FLAG_GENERAL_ALWAYS_OBEY_SPEED_LIMIT);
	m_boolAlwaysRetry = pAppRegs->IsFlagOn(REG_FLAG_GENERAL_ALWAYS_RETRY);
	prv_UpdateRetryOptions();

	m_boolStopAfter99NoData = pAppRegs->IsFlagOn(REG_FLAG_GENERAL_EXT_STOP_AFTER_99_NO_DATA);

	UpdateData(FALSE);
	
}

void COptionsConnectionPage::ReloadResource()
{
	//Dlg Items
	LPCTSTR lpszDlgID = _T("Conn_PPG");

	SET_DLG_ITEM_TEXT(1197);
	SET_DLG_ITEM_TEXT(1198);
	SET_DLG_ITEM_TEXT(1199);
	SET_DLG_ITEM_TEXT(1200);
	SET_DLG_ITEM_TEXT(1201);
	SET_DLG_ITEM_TEXT(1202);

	SET_DLG_ITEM_TEXT(1141);
	SET_DLG_ITEM_TEXT(1203);
	SET_DLG_ITEM_TEXT(1204);
	SET_DLG_ITEM_TEXT(1205);
	SET_DLG_ITEM_TEXT(1135);
	SET_DLG_ITEM_TEXT(1206);
	SET_DLG_ITEM_TEXT(1271);

}

void COptionsConnectionPage::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);

	CAppRegs *pAppRegs = ((CMyGetApp *)AfxGetApp())->GetAppRegs();

	pAppRegs->SetVal(REG_CONNECTION_CT,		(DWORD)m_iConnectionTimeOut);
	pAppRegs->SetVal(REG_CONNECTION_DT,		(DWORD)m_iDataTimeOut);
	pAppRegs->SetVal(REG_CONNECTION_WT,		(DWORD)m_iRetryDelay);
	pAppRegs->SetVal(REG_CONNECTION_SIMJOBS,(DWORD)m_iMaxSimJobs);
	pAppRegs->SetVal(REG_CONNECTION_RETRY,	(DWORD)m_iNumOfRetries);
	
	pAppRegs->SetVal(REG_CONNECTION_SPEEDLIMIT, (DWORD)(m_dblSpeedLimit * 1024));

	pAppRegs->SetFlag(REG_FLAG_GENERAL_ALWAYS_OBEY_SPEED_LIMIT, m_boolAlwaysObeySpeedLimit);
	pAppRegs->SetFlag(REG_FLAG_GENERAL_ALWAYS_RETRY, m_boolAlwaysRetry);
	pAppRegs->SetFlag(REG_FLAG_GENERAL_EXT_STOP_AFTER_99_NO_DATA, m_boolStopAfter99NoData);

	CPropertyPage::OnOK();
}
