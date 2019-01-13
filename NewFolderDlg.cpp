// NewFolderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "myget.h"
#include "NewFolderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewFolderDlg dialog


CNewFolderDlg::CNewFolderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewFolderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewFolderDlg)
	m_strCurrentFolderName = _T("");
	m_strNewFolderName = _T("");
	//}}AFX_DATA_INIT
}


void CNewFolderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewFolderDlg)
	DDX_Text(pDX, IDC_TEXT_CURRENT_FOLDER, m_strCurrentFolderName);
	DDX_Text(pDX, IDC_EDT_NEW_FOLDER_NAME, m_strNewFolderName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewFolderDlg, CDialog)
	//{{AFX_MSG_MAP(CNewFolderDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewFolderDlg message handlers

void CNewFolderDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	if (m_strNewFolderName.IsEmpty())
	{
		return;
	}

	m_strNewPath = m_strCurrentFolderName;

	if (m_strNewPath.ReverseFind(TCHAR('\\')) != m_strNewPath.GetLength() - 1)
	{
		m_strNewPath += "\\";
	}

	m_strNewPath += m_strNewFolderName;


	if (!CreateDirectory(m_strNewPath, NULL))
	{
		CString strError;
		strError.LoadString(IDS_COULD_CREATE_FOLDER);

		MessageBox(strError, NULL, MB_ICONERROR | MB_OK);
		return;
	}

	CDialog::OnOK();
}

void CNewFolderDlg::ReloadResource()
{
	//common
	//SetDlgItemText(1, APP_GET_RSCSTR("Dialog_Common", 1));
	//SetDlgItemText(2, APP_GET_RSCSTR("Dialog_Common", 2));
	SET_DLG_ITEM_TEXT_EX(1, "Dialog_Common", 1);
	SET_DLG_ITEM_TEXT_EX(2, "Dialog_Common", 2);

	//Dlg Items
	LPCTSTR lpszDlgID = _T("NewFolder_Dlg");
	//SetWindowText(APP_GET_RSCSTR(lpszDlgID, "Title"));
	SET_WINDOW_TITLE;
	SET_DLG_ITEM_TEXT(1197);
	SET_DLG_ITEM_TEXT(1198);
}

BOOL CNewFolderDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	ReloadResource();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
