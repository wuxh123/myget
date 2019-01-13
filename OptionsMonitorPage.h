#if !defined(AFX_OPTIONSMONITORPAGE_H__FD9DD468_5763_42E9_902C_3AEEEFADBE73__INCLUDED_)
#define AFX_OPTIONSMONITORPAGE_H__FD9DD468_5763_42E9_902C_3AEEEFADBE73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsMonitorPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionsMonitorPage dialog

class COptionsMonitorPage : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionsMonitorPage)

// Construction
public:
	void ReloadResource();
	COptionsMonitorPage();
	~COptionsMonitorPage();

// Dialog Data
	//{{AFX_DATA(COptionsMonitorPage)
	enum { IDD = IDD_OPTIONS_MONITOR };
	BOOL	m_bBrowserConfirm;
	BOOL	m_bBrowserFillCommentWithClipboardContent;
	BOOL	m_bBrowserIE;
	BOOL	m_bBrowserIEWidMatch;
	BOOL	m_bBrowserNetscape;
	BOOL	m_bBrowserOpenCancelledUrls;
	BOOL	m_bBrowserUseAlt;
	BOOL	m_bClipboardConfirm;
	BOOL	m_bClipboardMonitor;
	CString	m_strMonitorTypes;
	int		m_iBorwserIEMethod;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionsMonitorPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionsMonitorPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnChkBrowserie();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void prv_UpdateIEOptions();
	void prv_FillDlg();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSMONITORPAGE_H__FD9DD468_5763_42E9_902C_3AEEEFADBE73__INCLUDED_)
