// FolderSelectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "myget.h"
#include "FolderSelectionDlg.h"
#include "NewFolderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFolderSelectionDlg dialog


CFolderSelectionDlg::CFolderSelectionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFolderSelectionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFolderSelectionDlg)
	m_strSelectedFolder = _T("");
	//}}AFX_DATA_INIT
}


void CFolderSelectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFolderSelectionDlg)
	DDX_Control(pDX, IDC_FILELIST, m_FileListCtl);
	DDX_CBString(pDX, IDC_FILELIST, m_strSelectedFolder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFolderSelectionDlg, CDialog)
	//{{AFX_MSG_MAP(CFolderSelectionDlg)
	ON_NOTIFY(TVN_SELCHANGED, IDC_BROWSE_TREE, OnSelchangedBrowseTree)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_BROWSE_TREE, OnItemexpandingBrowseTree)
	ON_NOTIFY(TVN_DELETEITEM, IDC_BROWSE_TREE, OnDeleteitemBrowseTree)
	ON_NOTIFY(NM_RCLICK, IDC_BROWSE_TREE, OnRclickBrowseTree)
	ON_BN_CLICKED(IDC_SYNCH_FOLDERS, OnSynchFolders)
	ON_BN_CLICKED(IDC_BTN_NEW_FOLDER, OnBtnNewFolder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFolderSelectionDlg message handlers

BOOL CFolderSelectionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	ReloadResource();

	m_TreeCtl = (CShellTree*)GetDlgItem(IDC_BROWSE_TREE);
	m_TreeCtl->EnableImages();
	m_TreeCtl->PopulateTree();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFolderSelectionDlg::OnSelchangedBrowseTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString szPath;
	if(m_TreeCtl->OnFolderSelected(pNMHDR,pResult,szPath))
	{
		m_FileListCtl.SetWindowText(szPath);
	}
	*pResult = 0;

	*pResult = 0;
}

void CFolderSelectionDlg::OnItemexpandingBrowseTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_TreeCtl->OnFolderExpanding(pNMHDR,pResult);

	*pResult = 0;
}

void CFolderSelectionDlg::OnDeleteitemBrowseTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_TreeCtl->OnDeleteShellItem(pNMHDR,pResult);
	
	*pResult = 0;
}

void CFolderSelectionDlg::OnRclickBrowseTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	m_TreeCtl->GetContextMenu(pNMHDR,pResult);
	
	*pResult = 0;
}

void CFolderSelectionDlg::OnSynchFolders() 
{
	// TODO: Add your control notification handler code here
	CString szFindFolder;
	m_FileListCtl.GetWindowText(szFindFolder);
	m_TreeCtl->TunnelTree(szFindFolder);
	
}

void CFolderSelectionDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	if (m_strSelectedFolder.IsEmpty())
	{
		return;
	}

	CDialog::OnOK();
}

void CFolderSelectionDlg::OnBtnNewFolder() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if (m_strSelectedFolder.IsEmpty())
	{
		return;
	}

	CNewFolderDlg dlg;
	dlg.m_strCurrentFolderName = m_strSelectedFolder;

	if (dlg.DoModal() == IDOK)
	{
		m_strSelectedFolder = dlg.m_strNewPath;

		UpdateData(FALSE);

		m_TreeCtl->SetRedraw(FALSE);
		m_TreeCtl->DeleteAllItems();
		m_TreeCtl->PopulateTree();
		
		OnSynchFolders();

		m_TreeCtl->SetRedraw(TRUE);
	}
}



void CFolderSelectionDlg::ReloadResource()
{
	//common
	//SetDlgItemText(1, APP_GET_RSCSTR("Dialog_Common", 1));
	//SetDlgItemText(2, APP_GET_RSCSTR("Dialog_Common", 2));
	SET_DLG_ITEM_TEXT_EX(1, "Dialog_Common", 1);
	SET_DLG_ITEM_TEXT_EX(2, "Dialog_Common", 2);

	//Dlg Items
	LPCTSTR lpszDlgID = _T("SelFolder_Dlg");
	//SetWindowText(APP_GET_RSCSTR(lpszDlgID, "Title"));
	SET_WINDOW_TITLE;
	SET_DLG_ITEM_TEXT(1197);
	SET_DLG_ITEM_TEXT(1198);
	SET_DLG_ITEM_TEXT(1046);
}
