#if !defined(AFX_TABCTRL_H__E3D71444_FDA6_11D3_9FA5_006097D3A30A__INCLUDED_)
#define AFX_TABCTRL_H__E3D71444_FDA6_11D3_9FA5_006097D3A30A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabSheet window

class CTabSheet : public CTabCtrl
{
// Construction
public:
	CTabSheet();

// Attributes
public:
   BOOL  InsertTab         (char *TabString);

   int m_nNumOfTabs;
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabSheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	int GetCount  (void);
	virtual ~CTabSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTabSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABCTRL_H__E3D71444_FDA6_11D3_9FA5_006097D3A30A__INCLUDED_)
