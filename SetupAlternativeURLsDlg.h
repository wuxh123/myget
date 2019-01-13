#if !defined(AFX_SETUPALTERNATIVEURLSDLG_H__5952F6B9_D4F3_490B_879E_AB221EB91D14__INCLUDED_)
#define AFX_SETUPALTERNATIVEURLSDLG_H__5952F6B9_D4F3_490B_879E_AB221EB91D14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetupAlternativeURLsDlg.h : header file
//
#include "ListItem.h"
/////////////////////////////////////////////////////////////////////////////
// CSetupAlternativeURLsDlg dialog

class CListItem;

class CSetupAlternativeURLsDlg : public CDialog
{
// Construction
public:
	void ReloadResource();
	CArray <PSUBSTITUTEDURL, PSUBSTITUTEDURL> *m_parSubStitutedUrl;
	CSetupAlternativeURLsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetupAlternativeURLsDlg)
	enum { IDD = IDD_SETUP_ALTERNATE_URLS };
	CListCtrl	m_lstAlternativeUrls;
	CString	m_strUrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetupAlternativeURLsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetupAlternativeURLsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	virtual void OnOK();
	afx_msg void OnBtnDelete();
	afx_msg void OnBtnAdd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void prv_SaveURLs();
	void prv_SetupDlg();
	void prv_FillDlg();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETUPALTERNATIVEURLSDLG_H__5952F6B9_D4F3_490B_879E_AB221EB91D14__INCLUDED_)
