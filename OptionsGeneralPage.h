#if !defined(AFX_OPTIONSGENERALPAGE_H__720C2843_604C_475F_8C33_77D07976C7C2__INCLUDED_)
#define AFX_OPTIONSGENERALPAGE_H__720C2843_604C_475F_8C33_77D07976C7C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsGeneralPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionsGeneralPage dialog

class COptionsGeneralPage : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionsGeneralPage)

// Construction
public:
	void ReloadResource();
	COptionsGeneralPage();
	~COptionsGeneralPage();

// Dialog Data
	//{{AFX_DATA(COptionsGeneralPage)
	enum { IDD = IDD_OPTIONS_GENERAL };
	CSpinButtonCtrl	m_spnBlockSize;
	BOOL	m_boolStartMainframeMinimized;
	BOOL	m_boolAutoSave;
	BOOL	m_boolBackupEveryDay;
	BOOL	m_boolErrorStop;
	BOOL	m_boolFileSizeInBytes;
	BOOL	m_boolGetFileDateFromServer;
	BOOL	m_boolLogForEachDownloadedFile;
	BOOL	m_boolPromptWhenDrop;
	BOOL	m_boolStartDownloadOnStartup;
	BOOL	m_boolStillDownloadEvenNonResumable;
	BOOL	m_boolUseFileExt;
	int		m_iAutoSaveInterval;
	int		m_iBlockSize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionsGeneralPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionsGeneralPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void prv_FillDlg();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSGENERALPAGE_H__720C2843_604C_475F_8C33_77D07976C7C2__INCLUDED_)
