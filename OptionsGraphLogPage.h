#if !defined(AFX_OPTIONSGRAPHLOGPAGE_H__41A749A9_FDFF_400A_98C0_9B4462B9B33C__INCLUDED_)
#define AFX_OPTIONSGRAPHLOGPAGE_H__41A749A9_FDFF_400A_98C0_9B4462B9B33C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsGraphLogPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionsGraphLogPage dialog
#include "ColorButton.h"

class COptionsGraphLogPage : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionsGraphLogPage)

// Construction
public:
	void ReloadResource();
	COptionsGraphLogPage();
	~COptionsGraphLogPage();

// Dialog Data
	//{{AFX_DATA(COptionsGraphLogPage)
	enum { IDD = IDD_OPTIONS_GRAPH_LOG };
	CSpinButtonCtrl	m_spnGraphScale;
	CSpinButtonCtrl	m_spnBlockSize;
	CColorButton	m_clrbtnLogState;
	CColorButton	m_clrbtnLogServer;
	CColorButton	m_clrbtnLogError;
	CColorButton	m_clrbtnLogCommand;
	CColorButton	m_clrbtnGraphFore;
	CColorButton	m_clrbtnGraphBack;
	CColorButton	m_clrbtnBlockBack;
	BOOL	m_bDropZoneDisplayGraph;
	BOOL	m_bDropZoneTransparent;
	BOOL	m_bLogSettingBkColor;
	int		m_iBlockSize;
	int		m_iGraphScale;
	DWORD	m_dwGraphFore;
	DWORD	m_dwGraphBack;
	DWORD	m_dwLogCommand;
	DWORD	m_dwLogCommandBk;
	DWORD	m_dwLogState;
	DWORD	m_dwLogStateBk;
	DWORD	m_dwLogError;
	DWORD	m_dwLogErrorBk;
	DWORD	m_dwLogServer;
	DWORD	m_dwLogServerBk;
	DWORD	m_dwBlockBack;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionsGraphLogPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionsGraphLogPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnChkLogsettingbkcolor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void prv_FillDlg();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSGRAPHLOGPAGE_H__41A749A9_FDFF_400A_98C0_9B4462B9B33C__INCLUDED_)
