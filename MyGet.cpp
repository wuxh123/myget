// MyGet.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MyGet.h"

#include "MainFrm.h"
#include "MyGetDoc.h"
#include "MyGetView.h"
#include "TE_Socket.h"
//#include "MainScheduleThread.h"
#include "DownloadSchedulerThread.h"
#include "TreeItem.h"
#include "JCDFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyGetApp

BEGIN_MESSAGE_MAP(CMyGetApp, CWinApp)
	//{{AFX_MSG_MAP(CMyGetApp)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	//}}AFX_MSG_MAP
	// Standard file based document commands
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyGetApp construction

CMyGetApp::CMyGetApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMyGetApp object

CMyGetApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMyGetApp initialization

BOOL CMyGetApp::InitInstance()
{

	if (TE_InitLibrary() == SOCKET_ERROR)
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	CRegGeneral *pGenReg = new CRegGeneral;
	CRegProxy	*pProxyReg = new CRegProxy;

	m_AppRegs.Add(pGenReg);
	m_AppRegs.Add(pProxyReg);

	m_AppRegs.Load();

	//Save lauched dir.
	TCHAR szFullPath[MAX_PATH];
	TCHAR szDir[_MAX_DIR];
	TCHAR szDrive[_MAX_DRIVE];
	::GetModuleFileName(NULL, szFullPath, MAX_PATH);

	_splitpath(szFullPath, szDrive, szDir, NULL, NULL);

	m_strLauchFolder.Format(_T("%s%s"), szDrive, szDir);

	//Temporay assignment;
	//m_strLauchFolder = _T("G:\\Program Files\\FlashGet\\");

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMyGetDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CMyGetView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	m_pDownloadSchedulerThread = (CDownloadSchedulerThread *)AfxBeginThread(RUNTIME_CLASS(CDownloadSchedulerThread), THREAD_PRIORITY_IDLE);
	m_pDownloadSchedulerThread->SetNotifyWnd(m_pMainWnd->GetSafeHwnd());
	
	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();


//	OnFileOpen();

//	return TRUE;
	//If open default DB failed, Create a new default.DB
	if (!prv_OpenDB("Default.jcd"))
	{
		if (prv_NewDefaultDB())
		{
			prv_OpenDB("Default.jcd");
		}
	}

//	prv_OpenDB("test.jcd");
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog

/////////////////////////////////////////////////////////////////////////////
// CMyGetApp message handlers



void CMyGetApp::OnFileOpen() 
{

	CFileDialog dlgFile(
		TRUE,
		_T(".jcd"),
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("FlashGet Files (*.jcd)|*.jcd|All Files (*.*)|*.*||"));
	
	if (dlgFile.DoModal() != IDOK)
		return;

	//m_pMainScheduleThread->SuspendThread();
	GetDownloadSchedulerThread()->StopDownload();

	CString strFileName = dlgFile.GetPathName();
	
	prv_OpenDB(strFileName);
}

BOOL CMyGetApp::prv_OpenDB(LPCTSTR lpszDBName)
{

	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();
	CCategoryTree *pCategoryTree = pMainFrame->m_pMainCategoryTree;
	
	/*
	if (!m_strDBName.IsEmpty())
	{
		pCategoryTree->SaveToFile(m_strDBName);
	}
*/

	if (pCategoryTree->LoadFromFile(lpszDBName))
	{
		m_strDBName = lpszDBName;
		GetDownloadSchedulerThread()->SetDownloadTreeItem(
			(CTreeItem *)pCategoryTree->GetItemData(pCategoryTree->GetHTREEITEMByID(DOWNLOAD_CATEGORY_ID)));

		//GetDownloadScheduler()->m_eventQuitSuccess.ResetEvent();
		//while(m_pMainScheduleThread->ResumeThread() >1);
		
		GetDownloadSchedulerThread()->StartDownload();

		return TRUE;
	}

	GetDownloadSchedulerThread()->StopDownload();

	return FALSE;

}


BOOL CMyGetApp::SaveDB()
{
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();

	return pMainFrame->m_pMainCategoryTree->SaveToFile(m_strDBName);
}



int CMyGetApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	//m_pMainScheduleThread->Quit();
//	delete GetDownloadSchedulerThread();
	m_AppRegs.Save();
	TE_CleanupLibrary();
	return CWinApp::ExitInstance();
}

BOOL CMyGetApp::prv_NewDefaultDB()
{
#define INIT_TABLE_SIZE 8

	int iarInitDefault[INIT_TABLE_SIZE][4] = 
	{
		{1, 0, -1, 1},
		{2, 1, 1, 2},
		{3, 3, 1, 4},
		{8, 5, 1, 6},
		{4, 6, 3, 0},
		{5, 6, 3, 0},
		{6, 6, 3, 0},
		{7, 6, 3, 0}
	};

	char szInitDefaultCategoryName[INIT_TABLE_SIZE][20] =
	{
		{"FlashGet"},
		{"Download"},
		{"Downloaded"},
		{"Deleted"},
		{"Software"},
		{"Game"},
		{"Driver"},
		{"Mp3"},
	};

	char szInitDefaultDownloadFolder[INIT_TABLE_SIZE][30] =
	{
		{"C:\\Downloads"},
		{"C:\\Downloads"},
		{"C:\\Downloads"},
		{"C:\\Downloads"},
		{"C:\\Downloads\\Software"},
		{"C:\\Downloads\\Game"},
		{"C:\\Downloads\\Driver"},
		{"C:\\Downloads\\Mp3"},
	};
	

	CJCDFile jcdFile("Default.jcd", WRITE_JCD_FILE, INIT_TABLE_SIZE);

	CTreeItem TreeItem;

	for (int i = 0; i < INIT_TABLE_SIZE; i ++)
	{
		TreeItem.SetID			(iarInitDefault[i][0]);
		TreeItem.SetImageIndex	(iarInitDefault[i][1]);
		TreeItem.SetParentID	(iarInitDefault[i][2]);
		TreeItem.SetType		(iarInitDefault[i][3]);

		TreeItem.SetCategoryName(szInitDefaultCategoryName[i]);

		TreeItem.SetDefaultDir	(szInitDefaultDownloadFolder[i]);

		TreeItem.SaveToFile(&jcdFile);
	}

	return TRUE;
}

CAppRegs * CMyGetApp::GetAppRegs()
{
	return &m_AppRegs;
}

LPCTSTR CMyGetApp::GetRscStr(LPCTSTR lpszSection, DWORD dwKey)
{
	CString strKey;
	strKey.Format("%d", dwKey);

	return GetRscStr(lpszSection, strKey);
}

LPCTSTR CMyGetApp::GetRscStr(LPCTSTR lpszSection, LPCTSTR lpszKey)
{
	static char szRscStr[MAX_PATH];
	LPTSTR lpszLanguageFile = NULL;
	CString strLanguageFile;

	memset(szRscStr, 0, sizeof(szRscStr));

	m_AppRegs.GetVal(REG_GENERAL_LANGUAGEEX, &lpszLanguageFile);
	strLanguageFile.Format(m_strLauchFolder + "Language\\%s", lpszLanguageFile);
	
	GetPrivateProfileString(lpszSection, lpszKey, NULL, szRscStr, sizeof(szRscStr), strLanguageFile);

	if (strlen(szRscStr) == 0)
	{
		return NULL;
	}
	else
	{
		return szRscStr;
	}
}


CDownloadSchedulerThread * CMyGetApp::GetDownloadSchedulerThread()
{
	return m_pDownloadSchedulerThread;
}
