#if !defined(AFX_OPTIONSLOGINPAGE_H__4A5B0E30_65E7_4688_B2E0_01EC1E9FB05B__INCLUDED_)
#define AFX_OPTIONSLOGINPAGE_H__4A5B0E30_65E7_4688_B2E0_01EC1E9FB05B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsLoginPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionsLoginPage dialog

class COptionsLoginPage : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionsLoginPage)

// Construction
public:
	COptionsLoginPage();
	~COptionsLoginPage();

// Dialog Data
	//{{AFX_DATA(COptionsLoginPage)
	enum { IDD = IDD_OPTIONS_LOGIN };
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionsLoginPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionsLoginPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSLOGINPAGE_H__4A5B0E30_65E7_4688_B2E0_01EC1E9FB05B__INCLUDED_)
