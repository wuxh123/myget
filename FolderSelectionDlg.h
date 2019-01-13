#if !defined(AFX_FOLDERSELECTIONDLG_H__D203E784_2EA2_496A_BA6C_448B817E706A__INCLUDED_)
#define AFX_FOLDERSELECTIONDLG_H__D203E784_2EA2_496A_BA6C_448B817E706A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FolderSelectionDlg.h : header file
//
#include "ShellTree.h"

/////////////////////////////////////////////////////////////////////////////
// CFolderSelectionDlg dialog

class CFolderSelectionDlg : public CDialog
{
// Construction
public:
	void ReloadResource();
	CFolderSelectionDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFolderSelectionDlg)
	enum { IDD = IDD_FOLDER_SELECTION };
	CComboBox	m_FileListCtl;
	CString	m_strSelectedFolder;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFolderSelectionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CShellTree* m_TreeCtl;

	// Generated message map functions
	//{{AFX_MSG(CFolderSelectionDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedBrowseTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemexpandingBrowseTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeleteitemBrowseTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickBrowseTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSynchFolders();
	virtual void OnOK();
	afx_msg void OnBtnNewFolder();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FOLDERSELECTIONDLG_H__D203E784_2EA2_496A_BA6C_448B817E706A__INCLUDED_)
