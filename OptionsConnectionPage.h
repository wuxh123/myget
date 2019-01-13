#if !defined(AFX_OPTIONSCONNECTIONPAGE_H__B1985D06_D89C_45A7_ABCD_462A0C31912D__INCLUDED_)
#define AFX_OPTIONSCONNECTIONPAGE_H__B1985D06_D89C_45A7_ABCD_462A0C31912D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsConnectionPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionsConnectionPage dialog

class COptionsConnectionPage : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionsConnectionPage)

// Construction
public:
	void ReloadResource();
	COptionsConnectionPage();
	~COptionsConnectionPage();

// Dialog Data
	//{{AFX_DATA(COptionsConnectionPage)
	enum { IDD = IDD_OPTIONS_CONNECTION };
	CSpinButtonCtrl	m_spinRetryDelay;
	CSpinButtonCtrl	m_spinNumOfRetries;
	CSpinButtonCtrl	m_spinDataTimeOut;
	CSpinButtonCtrl	m_spinMaxSimJobs;
	CSpinButtonCtrl	m_spinConnectionTimeOut;
	int		m_iConnectionTimeOut;
	int		m_iDataTimeOut;
	BOOL	m_boolStopAfter99NoData;
	BOOL	m_boolAlwaysRetry;
	BOOL	m_boolAlwaysObeySpeedLimit;
	int		m_iMaxSimJobs;
	int		m_iNumOfRetries;
	int		m_iRetryDelay;
	double	m_dblSpeedLimit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionsConnectionPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionsConnectionPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckAlwaysRetry();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void prv_FillDlg();
	void prv_UpdateRetryOptions();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSCONNECTIONPAGE_H__B1985D06_D89C_45A7_ABCD_462A0C31912D__INCLUDED_)
