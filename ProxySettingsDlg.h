#if !defined(AFX_PROXYSETTINGSDLG_H__EF2B7C82_8199_4085_8A29_31709303B2F8__INCLUDED_)
#define AFX_PROXYSETTINGSDLG_H__EF2B7C82_8199_4085_8A29_31709303B2F8__INCLUDED_

#include "ProxyData.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProxySettingsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProxySettingsDlg dialog




class CProxySettingsDlg : public CDialog
{
// Construction
public:
	BOOL m_bIsDirectConn;
	void ReloadResource();
	CProxyData *m_pProxyData;
	CProxySettingsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProxySettingsDlg)
	enum { IDD = IDD_PROXY_SETTINGS };
	int		m_iType;
	BOOL	m_boolNoCache;
	BOOL	m_boolPasvMode;
	CString	m_strPassword;
	UINT	m_uiPort;
	CString	m_strServer;
	CString	m_strTitle;
	CString	m_strUser;
	BOOL	m_boolAuth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProxySettingsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProxySettingsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckAuth();
	virtual void OnOK();
	afx_msg void OnTypeClick();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void prv_FillDlg();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROXYSETTINGSDLG_H__EF2B7C82_8199_4085_8A29_31709303B2F8__INCLUDED_)
