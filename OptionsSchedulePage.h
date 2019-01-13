#if !defined(AFX_OPTIONSSCHEDULEPAGE_H__D6EF6D89_8036_46DA_B84A_DFA0A8740AEB__INCLUDED_)
#define AFX_OPTIONSSCHEDULEPAGE_H__D6EF6D89_8036_46DA_B84A_DFA0A8740AEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsSchedulePage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionsSchedulePage dialog

class COptionsSchedulePage : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionsSchedulePage)

// Construction
public:
	void ReloadResource();
	COptionsSchedulePage();
	~COptionsSchedulePage();

// Dialog Data
	//{{AFX_DATA(COptionsSchedulePage)
	enum { IDD = IDD_OPTIONS_SCHEDULE };
	BOOL	m_bEnableStopTimer;
	BOOL	m_bStartDownloadAtMonday;
	BOOL	m_bStartDownloadAtTuesday;
	BOOL	m_bStartDownloadAtWednesday;
	BOOL	m_bStartDownloadAtThursday;
	BOOL	m_bStartDownloadAtFriday;
	BOOL	m_bStartDownloadAtSaturday;
	BOOL	m_bStartDownloadAtSunday;
	CTime	m_tDownloadStartAt;
	CTime	m_tDownloadStopAt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionsSchedulePage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionsSchedulePage)
	virtual BOOL OnInitDialog();
	afx_msg void OnChkEnablestoptimer();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void prv_UpdateStopTimer();
	void prv_FillDlg();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSSCHEDULEPAGE_H__D6EF6D89_8036_46DA_B84A_DFA0A8740AEB__INCLUDED_)
