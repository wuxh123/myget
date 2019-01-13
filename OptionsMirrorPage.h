#if !defined(AFX_OPTIONSMIRRORPAGE_H__443D9E52_ACC9_4264_B270_A961A0B449A0__INCLUDED_)
#define AFX_OPTIONSMIRRORPAGE_H__443D9E52_ACC9_4264_B270_A961A0B449A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsMirrorPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionsMirrorPage dialog

class COptionsMirrorPage : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionsMirrorPage)

// Construction
public:
	void ReloadResource();
	COptionsMirrorPage();
	~COptionsMirrorPage();

// Dialog Data
	//{{AFX_DATA(COptionsMirrorPage)
	enum { IDD = IDD_OPTIONS_MIRROR };
	CSpinButtonCtrl	m_spinMinSize;
	CSpinButtonCtrl	m_spinMaxHit;
	BOOL	m_bAutoCalcAddMirrors;
	BOOL	m_bAutoSearchMirrorSites;
	BOOL	m_bAutoSelectMirror;
	BOOL	m_bEnableMirror;
	int		m_iFtpSearchMaxHit;
	int		m_iFtpSearchMinSize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionsMirrorPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionsMirrorPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void prv_FillDlg();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSMIRRORPAGE_H__443D9E52_ACC9_4264_B270_A961A0B449A0__INCLUDED_)
