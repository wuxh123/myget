#if !defined(AFX_OPTIONSPROTOCOLPAGE_H__3190A3D7_CC4E_4DC5_9C9C_CA674399009C__INCLUDED_)
#define AFX_OPTIONSPROTOCOLPAGE_H__3190A3D7_CC4E_4DC5_9C9C_CA674399009C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsProtocolPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionsProtocolPage dialog

#define DEFAULT_EMAIL	"yourname@yourcompany.com"

class COptionsProtocolPage : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionsProtocolPage)

// Construction
public:
	void ReloadResource();
	COptionsProtocolPage();
	~COptionsProtocolPage();

// Dialog Data
	//{{AFX_DATA(COptionsProtocolPage)
	enum { IDD = IDD_OPTIONS_PROTOCOL };
	BOOL	m_boolEnableCookie;
	CString	m_strAsciiMode;
	CString	m_strEmail;
	CString	m_strUserDefineAgent;
	int		m_iUserAgent;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionsProtocolPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionsProtocolPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void prv_FillDlg();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSPROTOCOLPAGE_H__3190A3D7_CC4E_4DC5_9C9C_CA674399009C__INCLUDED_)
