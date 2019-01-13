#if !defined(AFX_OPTIONSDIALUP_H__5BC8B55B_3122_46AC_BFC0_649E425D6E8B__INCLUDED_)
#define AFX_OPTIONSDIALUP_H__5BC8B55B_3122_46AC_BFC0_649E425D6E8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsDialUp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionsDialUpPage dialog

class COptionsDialUpPage : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionsDialUpPage)

// Construction
public:
	void ReloadResource();
	COptionsDialUpPage();
	~COptionsDialUpPage();

private:
	void prv_UpdateDialEntry();
	void prv_UpdateRedial();
	void prv_FillDlg();

// Dialog Data
	//{{AFX_DATA(COptionsDialUpPage)
	enum { IDD = IDD_OPTIONS_DIAL_UP };
	CSpinButtonCtrl	m_spn_WaitBetweenRetry;
	CSpinButtonCtrl	m_spnRetryFailedConnection;
	int		m_iConnection;
	BOOL	m_bHandupWhenDone;
	BOOL	m_bRedialIfFailedConnected;
	CString	m_strConnectionWindowTitle;
	CString	m_strPassword;
	CString	m_strUserName;
	DWORD	m_dwRetryFailedConnection;
	DWORD	m_dwWaitBetweenRetry;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionsDialUpPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionsDialUpPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnChkRedialifdisconnected();
	afx_msg void OnSelchangeCbxConnections();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSDIALUP_H__5BC8B55B_3122_46AC_BFC0_649E425D6E8B__INCLUDED_)
