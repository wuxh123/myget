#if !defined(AFX_DOWNLOADDLG_H__BBD8D55D_7A99_446F_AC70_622B13DAD17E__INCLUDED_)
#define AFX_DOWNLOADDLG_H__BBD8D55D_7A99_446F_AC70_622B13DAD17E__INCLUDED_

#include "DownloadPropertyPage.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DownloadDlg.h : header file
//
#define TYPE_NEW_DOWNLOAD				0
#define TYPE_DOWNLOAD_ITEM_PROPERTY		1
#define TYPE_DEFAULT_DOWNLOAD_PROPERTY	2
/////////////////////////////////////////////////////////////////////////////
// CDownloadDlg

class CListItem;
class CDownloadDlg : public CPropertySheet
{
	DECLARE_DYNAMIC(CDownloadDlg)

// Construction
public:
	CDownloadDlg(LPCTSTR pszCaption, int iDlgType, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDownloadDlg)
	//}}AFX_VIRTUAL

// Implementation
public:
	//PLISTITEMSTRUCT m_pListItemStruct;
	CDownloadPropertyPage m_pageProperty;

	virtual ~CDownloadDlg();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDownloadDlg)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void InitDlg(int iDlgType);
	void prv_AddPages();
	void prv_SetupSheet();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOWNLOADDLG_H__BBD8D55D_7A99_446F_AC70_622B13DAD17E__INCLUDED_)
