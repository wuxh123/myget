#if !defined(AFX_OPTIONSHEET_H__A4F56FEC_7655_4648_86FC_0F6E10F32ABA__INCLUDED_)
#define AFX_OPTIONSHEET_H__A4F56FEC_7655_4648_86FC_0F6E10F32ABA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionSheet.h : header file
//

#include "OptionsConnectionPage.h"
#include "OptionsDialUpPage.h"
#include "OptionsFileManagementPage.h"
#include "OptionsGeneralPage.h"
#include "OptionsGraphLogPage.h"
#include "OptionsLoginPage.h"
#include "OptionsMirrorPage.h"
#include "OptionsMonitorPage.h"
#include "OptionsOtherPage.h"
#include "OptionsProtocolPage.h"
#include "OptionsProxyPage.h"
#include "OptionsSchedulePage.h"
#include "OptionsSoundPage.h"

/////////////////////////////////////////////////////////////////////////////
// COptionSheet

class COptionSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(COptionSheet)

// Construction
public:
	COptionSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	COptionSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionSheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COptionSheet();
	COptionsConnectionPage		m_pageConnection;
	COptionsDialUpPage			m_pageDialUp;
	COptionsFileManagementPage	m_pageFileManagement;
	COptionsGeneralPage			m_pageGeneral;
	COptionsGraphLogPage		m_pageGraphLog;
	COptionsLoginPage			m_pageLogin;
	COptionsMirrorPage			m_pageMirror;
	COptionsMonitorPage			m_pageMonitor;
	COptionsOtherPage			m_pageOther;
	COptionsProtocolPage		m_pageProtocol;
	COptionsProxyPage			m_pageProxy;
	COptionsSchedulePage		m_pageSchedule;
	COptionsSoundPage			m_pageSound;
	// Generated message map functions
protected:
	//{{AFX_MSG(COptionSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void prv_SetupSheet();
	void prv_AddPages();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSHEET_H__A4F56FEC_7655_4648_86FC_0F6E10F32ABA__INCLUDED_)
