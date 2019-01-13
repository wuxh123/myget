// OptionsFileManagementPage.cpp : implementation file
//

#include "stdafx.h"
#include "myget.h"
#include "OptionsFileManagementPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsFileManagementPage property page

IMPLEMENT_DYNCREATE(COptionsFileManagementPage, CPropertyPage)

COptionsFileManagementPage::COptionsFileManagementPage() : CPropertyPage(COptionsFileManagementPage::IDD)
{
	//{{AFX_DATA_INIT(COptionsFileManagementPage)
	m_iStoragesCBX = -1;
	m_bMoveSomeFilesToPortableStorage = FALSE;
	m_bOpenFilesWhenDownloaded = FALSE;
	m_bVirusScanDownloadedFiles = FALSE;
	m_strOpenTypes = _T("");
	m_strVirsuScanner = _T("");
	m_strVirsuScannerArgs = _T("");
	m_strVirsuScannerTypes = _T("");
	m_iDestinationExists = -1;
	m_iMoveDownloadedItem = -1;
	//}}AFX_DATA_INIT

	//m_psp.dwFlags |= PSP_USETITLE;
	//static CString strCaption;
	//strCaption = APP_GET_RSCSTR("File_PPG", "Title");
	//m_psp.pszTitle = strCaption;
	SET_PROPERTYPAGE_TITLE("File_PPG");

}

COptionsFileManagementPage::~COptionsFileManagementPage()
{
}

void COptionsFileManagementPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsFileManagementPage)
	DDX_CBIndex(pDX, IDC_CBX_STORAGES, m_iStoragesCBX);
	DDX_Check(pDX, IDC_CHK_MOVESOMEFILESTOPORTABLESTORAGE, m_bMoveSomeFilesToPortableStorage);
	DDX_Check(pDX, IDC_CHK_OPENFILEWHENDOWNLOADED, m_bOpenFilesWhenDownloaded);
	DDX_Check(pDX, IDC_CHK_VIRUSSCANDOWNLOADEDFILES, m_bVirusScanDownloadedFiles);
	DDX_Text(pDX, IDC_EDT_OPENTYPES, m_strOpenTypes);
	DDX_Text(pDX, IDC_EDT_VIRUSSCANNER, m_strVirsuScanner);
	DDX_Text(pDX, IDC_EDT_VIRUSSCANNERARGS, m_strVirsuScannerArgs);
	DDX_Text(pDX, IDC_EDT_VIRUSSCANNERTYPES, m_strVirsuScannerTypes);
	DDX_Radio(pDX, IDC_RD_DESTINATIONEXISTS_OVERWRITE, m_iDestinationExists);
	DDX_Radio(pDX, IDC_RD_MOVEDOWNLOADEDITEM_DONOTMOVE, m_iMoveDownloadedItem);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsFileManagementPage, CPropertyPage)
	//{{AFX_MSG_MAP(COptionsFileManagementPage)
	ON_BN_CLICKED(IDC_BTN_BROWSEVIRUSSCANNER, OnBtnBrowsevirusscanner)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsFileManagementPage message handlers

BOOL COptionsFileManagementPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	ReloadResource();
	prv_FillDlg();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionsFileManagementPage::prv_FillDlg()
{
	CAppRegs *pAppRegs = ((CMyGetApp *)AfxGetApp())->GetAppRegs();
	LPTSTR lpszVal;

	//下载完进行病毒检查
	m_bVirusScanDownloadedFiles = pAppRegs->IsFlagOn(REG_FLAG_FILEMANAGER_SCANVIRUSWHENDOWNLOADED);

	//下载完毕打开或察看已下载的文件
	m_bOpenFilesWhenDownloaded = pAppRegs->IsFlagOn(REG_FLAG_GENERAL_OPENDOWNLOADEDFILE);

	//某些文件移动到可移动的存储器
	m_bMoveSomeFilesToPortableStorage = pAppRegs->IsFlagOn(REG_FLAG_GENERAL_MOVESOMEFILETOSTORAGE);

	//Virus Scanner
	lpszVal = NULL;
	pAppRegs->GetVal(REG_GENERAL_VIRUSSCANNER, &lpszVal);
	m_strVirsuScanner = lpszVal;

	//Virus Scanner Args
	lpszVal = NULL;
	pAppRegs->GetVal(REG_GENERAL_VIRUSSCANNERARGS, &lpszVal);
	m_strVirsuScannerArgs = lpszVal;	

	//Virus Scanner File types
	lpszVal = NULL;
	pAppRegs->GetVal(REG_GENERAL_VIRUSSCANTYPE, &lpszVal);
	m_strVirsuScannerTypes = lpszVal;	

	//Virus Scanner File types
	lpszVal = NULL;
	pAppRegs->GetVal(REG_GENERAL_OPENTYPES, &lpszVal);
	m_strOpenTypes = lpszVal;	

	//Fill Storage ComboBox
	char szStorage[2] = {"A"};

	for (char cStorage = 'A'; cStorage <= 'Z'; cStorage++)
	{
		szStorage[0] = cStorage;

		((CComboBox *)GetDlgItem(IDC_CBX_STORAGES))->AddString(szStorage);
	}

	m_iMoveDownloadedItem = pAppRegs->GetFileManagerFlag(REG_FILEMANAGER_MOVEDOWNLOADEDFILES);
	m_iDestinationExists  = pAppRegs->GetFileManagerFlag(REG_FILEMANAGER_DESTINATIONEXISTS);
	m_iStoragesCBX = pAppRegs->GetFileManagerFlag(REG_FILEMANAGER_MOVETOSTORAGE, 8);

	
	UpdateData(FALSE);
	
}

void COptionsFileManagementPage::OnBtnBrowsevirusscanner() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CFileDialog dlgFile(
	TRUE,
	_T(".exe"),
	m_strVirsuScanner,
	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
	_T("Program Files (*.exe)|*.exe" ));

	if (dlgFile.DoModal() == IDOK)
	{
		m_strVirsuScanner = dlgFile.GetPathName();
		UpdateData(FALSE);
	}

}

void COptionsFileManagementPage::ReloadResource()
{
	//Dlg Items
	LPCTSTR lpszDlgID = _T("File_PPG");

	SET_DLG_ITEM_TEXT(1197);
	SET_DLG_ITEM_TEXT(1076);
	SET_DLG_ITEM_TEXT(1077);
	SET_DLG_ITEM_TEXT(1078);
	SET_DLG_ITEM_TEXT(1198);
	SET_DLG_ITEM_TEXT(1128);
	SET_DLG_ITEM_TEXT(1129);
	SET_DLG_ITEM_TEXT(1130);
	SET_DLG_ITEM_TEXT(1153);
	SET_DLG_ITEM_TEXT(1003);
	SET_DLG_ITEM_TEXT(1199);
	SET_DLG_ITEM_TEXT(1200);
	SET_DLG_ITEM_TEXT(1253);
	SET_DLG_ITEM_TEXT(1273);
	SET_DLG_ITEM_TEXT(1201);

}

void COptionsFileManagementPage::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);

	CAppRegs *pAppRegs = ((CMyGetApp *)AfxGetApp())->GetAppRegs();
	
	//下载完进行病毒检查
	pAppRegs->SetFlag(REG_FLAG_FILEMANAGER_SCANVIRUSWHENDOWNLOADED, m_bVirusScanDownloadedFiles);

	//下载完毕打开或察看已下载的文件
	pAppRegs->SetFlag(REG_FLAG_GENERAL_OPENDOWNLOADEDFILE, m_bOpenFilesWhenDownloaded);

	//某些文件移动到可移动的存储器
	pAppRegs->SetFlag(REG_FLAG_GENERAL_MOVESOMEFILETOSTORAGE, m_bMoveSomeFilesToPortableStorage);

	//Virus Scanner
	pAppRegs->SetVal(REG_GENERAL_VIRUSSCANNER, m_strVirsuScanner);

	//Virus Scanner Args
	pAppRegs->SetVal(REG_GENERAL_VIRUSSCANNERARGS, m_strVirsuScannerArgs);

	//Virus Scanner File types
	pAppRegs->SetVal(REG_GENERAL_VIRUSSCANTYPE, m_strVirsuScannerTypes);

	//Virus Scanner File types
	pAppRegs->SetVal(REG_GENERAL_OPENTYPES, m_strOpenTypes);

	pAppRegs->SetFileManagerFlag(REG_FILEMANAGER_MOVEDOWNLOADEDFILES, 4, m_iMoveDownloadedItem);
	pAppRegs->SetFileManagerFlag(REG_FILEMANAGER_DESTINATIONEXISTS, 4, m_iDestinationExists);
	pAppRegs->SetFileManagerFlag(REG_FILEMANAGER_MOVETOSTORAGE, 8, m_iStoragesCBX);

	CPropertyPage::OnOK();
}
