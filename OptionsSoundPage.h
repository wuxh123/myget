#if !defined(AFX_OPTIONSSOUNDPAGE_H__A3F4D127_8DC9_4626_A649_D3C765B1AA26__INCLUDED_)
#define AFX_OPTIONSSOUNDPAGE_H__A3F4D127_8DC9_4626_A649_D3C765B1AA26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsSoundPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionsSoundPage dialog

class COptionsSoundPage : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionsSoundPage)

// Construction
public:
	void ReloadResource();
	COptionsSoundPage();
	~COptionsSoundPage();

// Dialog Data
	//{{AFX_DATA(COptionsSoundPage)
	enum { IDD = IDD_OPTIONS_SOUND };
	CListCtrl	m_lvSounds;
	BOOL	m_bOpenDlgWhenDownloadAll;
	BOOL	m_bOpenDlgWhenDownloadFailed;
	BOOL	m_bOpenDlgWhenDownloadSucceed;
	BOOL	m_bEnableSound;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionsSoundPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionsSoundPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnClearsound();
	afx_msg void OnBtnPlaysound();
	afx_msg void OnBtnBrowsesound();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void prv_FillDlg();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSSOUNDPAGE_H__A3F4D127_8DC9_4626_A649_D3C765B1AA26__INCLUDED_)
