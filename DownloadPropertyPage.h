#if !defined(AFX_DOWNLOADPROPERTYPAGE_H__D9DC367D_DC1C_4291_AFE8_D3B06E331822__INCLUDED_)
#define AFX_DOWNLOADPROPERTYPAGE_H__D9DC367D_DC1C_4291_AFE8_D3B06E331822__INCLUDED_

#include "ListItem.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DownloadPropertyPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDownloadPropertyPage dialog

class CCategoryTree;
class CDownloadPropertyPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CDownloadPropertyPage)

// Construction
public:
	void ReloadResource();
	int m_iDlgType;
	CListItem *m_pListItem;
	CDownloadPropertyPage();
	~CDownloadPropertyPage();

// Dialog Data
	//{{AFX_DATA(CDownloadPropertyPage)
	enum { IDD = IDD_DOWNLOAD_PROP };
	CComboBox	m_cbxHttpProxy;
	CComboBox	m_cbxFtpProxy;
	int m_iStart;
	CSpinButtonCtrl	m_spinSplit;
	CComboBoxEx	m_cbxCategory;
	CString	m_strUrl;
	BOOL	m_boolEnableMirrorAndFtpSearch;
	CString	m_strRefer;
	CString	m_strPath;
	int		m_iSplit;
	BOOL	m_boolLogin;
	CString	m_strPassword;
	CString	m_strUserName;
	CString	m_strRename;
	CString	m_strComment;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDownloadPropertyPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDownloadPropertyPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCbxCategory();
	afx_msg void OnCheckLogin();
	afx_msg void OnBtnCreateNewCategory();
	afx_msg void OnBtnSelectNewFolder();
	afx_msg void OnChangeEditUrl();
	afx_msg void OnBtnSetSubstitutedUrls();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	CArray <PSUBSTITUTEDURL, PSUBSTITUTEDURL> m_arSubStitutedUrl;

private:
	void prv_SaveItemList();
	BOOL m_boolNeedFreeMEM;
	void prv_UpdateListItemSetting();
	void prv_FreeMEM();
	void prv_PrepareDefaultListItem();
	void prv_FillDlgByItemProperty();
	void prv_SelectProxyCBXs(int iDefaultFtpProxyIndex, int iDefaultHttpProxyIndex);
	void prv_FillProxyCBXs();
	void prv_UpdateUserAndPasswordByLoginCHK();
	void prv_UpdatePathByCategoryCBX();
	void prv_SelectCategoryByID(int iID);
	void prv_FillCategoryTreeItem(CCategoryTree *pCategoryTree, HTREEITEM hItem, int iIndent, int *piIndex);
	void prv_FillCategoryCBX();
	LPSTR prv_GetClipText();
	void prv_FillDlg();
	void prv_SetupDlg();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOWNLOADPROPERTYPAGE_H__D9DC367D_DC1C_4291_AFE8_D3B06E331822__INCLUDED_)
