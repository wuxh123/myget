#if !defined(AFX_INPUTDLG_H__1C2B83AF_95F4_4DD6_A6C2_9981F01918E0__INCLUDED_)
#define AFX_INPUTDLG_H__1C2B83AF_95F4_4DD6_A6C2_9981F01918E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputDlg dialog
#define ADD_ALTER_URL	0
#define RENAME_FILE		1

class CInputDlg : public CDialog
{
// Construction
public:
	BOOL prv_CheckAlterUrl();
	int m_iDlgType;
	CInputDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputDlg)
	enum { IDD = 135 };
	CString	m_strInput;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditInput();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void prv_UpdateOKBtnStatus();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTDLG_H__1C2B83AF_95F4_4DD6_A6C2_9981F01918E0__INCLUDED_)
