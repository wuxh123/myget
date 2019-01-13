#if !defined(AFX_NEWFOLDERDLG_H__4FF2CF52_1C2F_499D_A6EF_3ACE93289F58__INCLUDED_)
#define AFX_NEWFOLDERDLG_H__4FF2CF52_1C2F_499D_A6EF_3ACE93289F58__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewFolderDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewFolderDlg dialog

class CNewFolderDlg : public CDialog
{
// Construction
public:
	void ReloadResource();
	CString m_strNewPath;
	CNewFolderDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewFolderDlg)
	enum { IDD = IDD_NEW_FOLDER };
	CString	m_strCurrentFolderName;
	CString	m_strNewFolderName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewFolderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewFolderDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWFOLDERDLG_H__4FF2CF52_1C2F_499D_A6EF_3ACE93289F58__INCLUDED_)
