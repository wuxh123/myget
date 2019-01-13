// OptionsGeneralPage.cpp : implementation file
//

#include "stdafx.h"
#include "myget.h"
#include "OptionsGeneralPage.h"
#include "AppRegs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsGeneralPage property page

IMPLEMENT_DYNCREATE(COptionsGeneralPage, CPropertyPage)

COptionsGeneralPage::COptionsGeneralPage() : CPropertyPage(COptionsGeneralPage::IDD)
{
	//{{AFX_DATA_INIT(COptionsGeneralPage)
	m_boolStartMainframeMinimized = FALSE;
	m_boolAutoSave = FALSE;
	m_boolBackupEveryDay = FALSE;
	m_boolErrorStop = FALSE;
	m_boolFileSizeInBytes = FALSE;
	m_boolGetFileDateFromServer = FALSE;
	m_boolLogForEachDownloadedFile = FALSE;
	m_boolPromptWhenDrop = FALSE;
	m_boolStartDownloadOnStartup = FALSE;
	m_boolStillDownloadEvenNonResumable = FALSE;
	m_boolUseFileExt = FALSE;
	m_iAutoSaveInterval = 0;
	m_iBlockSize = 0;
	//}}AFX_DATA_INIT
	
	//m_psp.dwFlags |= PSP_USETITLE;
	//static CString strCaption;
	//strCaption = APP_GET_RSCSTR("Gen_PPG", "Title");
	//m_psp.pszTitle = strCaption;
	SET_PROPERTYPAGE_TITLE("Gen_PPG");

}

COptionsGeneralPage::~COptionsGeneralPage()
{
}

void COptionsGeneralPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsGeneralPage)
	DDX_Control(pDX, IDC_SPN_BLOCK_SIZE, m_spnBlockSize);
	DDX_Check(pDX, IDC_CHK_START_MAINFRAME_MINIMIZED, m_boolStartMainframeMinimized);
	DDX_Check(pDX, IDC_CHK_AUTO_SAVE, m_boolAutoSave);
	DDX_Check(pDX, IDC_CHK_BACKUP_EVERY_DAY, m_boolBackupEveryDay);
	DDX_Check(pDX, IDC_CHK_ERROR_STOP, m_boolErrorStop);
	DDX_Check(pDX, IDC_CHK_FILE_SIZE_IN_BYTES, m_boolFileSizeInBytes);
	DDX_Check(pDX, IDC_CHK_GET_FILE_DATE_FROM_SERVER, m_boolGetFileDateFromServer);
	DDX_Check(pDX, IDC_CHK_LOG_FOR_EACH_DOWNLOADED_FILE, m_boolLogForEachDownloadedFile);
	DDX_Check(pDX, IDC_CHK_PROMPT_WHEN_DROP, m_boolPromptWhenDrop);
	DDX_Check(pDX, IDC_CHK_START_DOWNLOAD_ON_STARTUP, m_boolStartDownloadOnStartup);
	DDX_Check(pDX, IDC_CHK_STILL_DOWNLOAD_EVEN_NONRESUMABLE, m_boolStillDownloadEvenNonResumable);
	DDX_Check(pDX, IDC_CHK_USE_FILE_EXT, m_boolUseFileExt);
	DDX_Text(pDX, IDC_EDT_AUTO_SAVE_INTERVAL, m_iAutoSaveInterval);
	DDX_Text(pDX, IDC_EDT_BLOCK_SIZE, m_iBlockSize);
	DDV_MinMaxInt(pDX, m_iBlockSize, 10, 102400);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsGeneralPage, CPropertyPage)
	//{{AFX_MSG_MAP(COptionsGeneralPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsGeneralPage message handlers

BOOL COptionsGeneralPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	ReloadResource();
	prv_FillDlg();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionsGeneralPage::prv_FillDlg()
{
	CAppRegs *pAppRegs = ((CMyGetApp *)AfxGetApp())->GetAppRegs();

	m_boolStartMainframeMinimized = pAppRegs->IsFlagOn(REG_FLAG_GENERAL_START_MAINFRAME_MIIMIZED);
	m_boolStillDownloadEvenNonResumable = !pAppRegs->IsFlagOn(REG_FLAG_GENERAL_STOP_DOWNLOAD_EVERN_NON_RESUMABLE);
	m_boolErrorStop = pAppRegs->IsFlagOn(REG_FLAG_GENERAL_ERROR_STOP);
	m_boolPromptWhenDrop = pAppRegs->IsFlagOn(REG_FLAG_GENERAL_PROMPT_WHEN_DROP);
	m_boolAutoSave = pAppRegs->IsFlagOn(REG_FLAG_GENERAL_AUTO_SAVE);
	m_boolGetFileDateFromServer = pAppRegs->IsFlagOn(REG_FLAG_GENERAL_GET_FILE_DATE_FROM_SERVER);
	m_boolStartDownloadOnStartup = pAppRegs->IsFlagOn(REG_FLAG_GENERAL_START_DOWNLOAD_ON_STARTUP);
	m_boolUseFileExt = pAppRegs->IsFlagOn(REG_FLAG_GENERAL_USE_FILE_EXT);
	m_boolFileSizeInBytes = pAppRegs->IsFlagOn(REG_FLAG_GENERAL_FILE_SIZE_IN_BYTES);
	m_boolLogForEachDownloadedFile = pAppRegs->IsFlagOn(REG_FLAG_GENERAL_LOG_FOR_EACH_DOWNLOADED_FILE);
	
	

	pAppRegs->GetVal(REG_GENERAL_AUTOSAVETIME, (DWORD *)&m_iAutoSaveInterval);

	UDACCEL udAccel;
	m_spnBlockSize.GetAccel(1, &udAccel);
	udAccel.nInc = 10;
	m_spnBlockSize.SetAccel(1, &udAccel);
	m_spnBlockSize.SetRange(10, 10240);

	pAppRegs->GetVal(REG_GENERAL_BLOCKSIZE, (DWORD *)&m_iBlockSize);
	m_iBlockSize = m_iBlockSize / 1024;

	m_boolBackupEveryDay = pAppRegs->IsFlagOn(REG_FLAG_GENERAL_EXT_BACKUP_EVERY_DAY);


	UpdateData(FALSE);

}

void COptionsGeneralPage::ReloadResource()
{

	//Dlg Items
	LPCTSTR lpszDlgID = _T("Gen_PPG");
	SET_DLG_ITEM_TEXT(1055);
	SET_DLG_ITEM_TEXT(1056);
	SET_DLG_ITEM_TEXT(1058);
	SET_DLG_ITEM_TEXT(1059);
	SET_DLG_ITEM_TEXT(1062);
	SET_DLG_ITEM_TEXT(1197);
	SET_DLG_ITEM_TEXT(1100);
	SET_DLG_ITEM_TEXT(1120);
	SET_DLG_ITEM_TEXT(1123);
	SET_DLG_ITEM_TEXT(1124);
	SET_DLG_ITEM_TEXT(1179);
	SET_DLG_ITEM_TEXT(1255);
	SET_DLG_ITEM_TEXT(1198);
}

void COptionsGeneralPage::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);

	CAppRegs *pAppRegs = ((CMyGetApp *)AfxGetApp())->GetAppRegs();

	pAppRegs->SetFlag(REG_FLAG_GENERAL_START_MAINFRAME_MIIMIZED,			m_boolStartMainframeMinimized);
	pAppRegs->SetFlag(REG_FLAG_GENERAL_STOP_DOWNLOAD_EVERN_NON_RESUMABLE,	!m_boolStillDownloadEvenNonResumable);
	pAppRegs->SetFlag(REG_FLAG_GENERAL_ERROR_STOP,							m_boolErrorStop);
	pAppRegs->SetFlag(REG_FLAG_GENERAL_PROMPT_WHEN_DROP,					m_boolPromptWhenDrop);
	pAppRegs->SetFlag(REG_FLAG_GENERAL_AUTO_SAVE,							m_boolAutoSave);
	pAppRegs->SetFlag(REG_FLAG_GENERAL_GET_FILE_DATE_FROM_SERVER,			m_boolGetFileDateFromServer);
	pAppRegs->SetFlag(REG_FLAG_GENERAL_START_DOWNLOAD_ON_STARTUP,			m_boolStartDownloadOnStartup);
	pAppRegs->SetFlag(REG_FLAG_GENERAL_USE_FILE_EXT,						m_boolUseFileExt);
	pAppRegs->SetFlag(REG_FLAG_GENERAL_FILE_SIZE_IN_BYTES,					m_boolFileSizeInBytes);
	pAppRegs->SetFlag(REG_FLAG_GENERAL_LOG_FOR_EACH_DOWNLOADED_FILE,		m_boolLogForEachDownloadedFile);
	pAppRegs->SetFlag(REG_FLAG_GENERAL_EXT_BACKUP_EVERY_DAY,				m_boolBackupEveryDay);

	pAppRegs->SetVal(REG_GENERAL_AUTOSAVETIME, (DWORD)m_iAutoSaveInterval);
	pAppRegs->SetVal(REG_GENERAL_BLOCKSIZE, (DWORD)(m_iBlockSize * 1024));
	
	CPropertyPage::OnOK();
}
