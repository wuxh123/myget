#if !defined(AFX_CATEGORYDLG_H__06C12443_3D58_11D5_9A67_0020E0101471__INCLUDED_)
#define AFX_CATEGORYDLG_H__06C12443_3D58_11D5_9A67_0020E0101471__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CategoryDlg.h : header file
//
#include "CategoryTree.h"
/////////////////////////////////////////////////////////////////////////////
// CCategoryDlg dialog
//define Category Dialog Type
#define CATEGORY_NEW_DLG		1
#define CATEGORY_MOVETO_DLG		2
#define CATEGORY_PROPERTY_DLG	3

class CCategoryDlg : public CDialog
{
// Construction
public:
	void ReloadResource();
	int m_iNewItemID;
	BOOL m_bCanMove;
	CCategoryTree *m_pMainCategoryTree;
	int m_typeDlg;
	CCategoryDlg(CWnd* pParent = NULL);   // standard constructor
	HTREEITEM m_hCurrentItem;

// Dialog Data
	//{{AFX_DATA(CCategoryDlg)
	enum { IDD = IDD_CATEGORY };
	CString	m_textTotalFiles;
	CString	m_textTitle;
	CString	m_textSubCategoryCount;
	CString	m_textDownloadedSize;
	CString	m_textTotalDownloadedFiles;
	CString	m_strCategoryName;
	CString	m_strDefaultDir;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCategoryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CTreeCtrl	m_treeCategory;

	// Generated message map functions
	//{{AFX_MSG(CCategoryDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedCategoryTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDefaultDirEdit();
	afx_msg void OnChangeCategoryNameEdit();
	virtual void OnOK();
	afx_msg void OnBrowseFolderButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void prv_FinalUpdateCategory();
	void prv_FinalMoveCategory();
	void prv_FinalNewCategory();
	CString m_strBaseDefaultDir;
	void prv_MakeDefaultDir();
	BOOL m_bIsDefaultDirModifiedByUser;
	void prv_CopyTreeItem(HTREEITEM hSrcItem, HTREEITEM hDestParentItem);
	void prv_CopyTree();
	void prv_FillTreeDlg();
	void GetItemStatistic(HTREEITEM hItem, int *piTotalSubCategory, int *piTotalFiles, int *piTotalDownloadedFiles, int *piDownloadedSize);
	BOOL prv_FillPropDlg();
	BOOL prv_SetupDlg();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CATEGORYDLG_H__06C12443_3D58_11D5_9A67_0020E0101471__INCLUDED_)
