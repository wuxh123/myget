#if !defined(AFX_FLASHGETTOOLBAR_H__FCF8336F_8B89_4B81_967E_51AF2814340D__INCLUDED_)
#define AFX_FLASHGETTOOLBAR_H__FCF8336F_8B89_4B81_967E_51AF2814340D__INCLUDED_

#include "HistogramCtrl.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FlashGetToolBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFlashGetToolBar window

class CFlashGetToolBar : public CToolBar
{
// Construction
public:
	CFlashGetToolBar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlashGetToolBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetSpeed(float fSpeedByKB);
	void ResizeBar(int cx);
	virtual ~CFlashGetToolBar();

	// Generated message map functions
protected:
	CHistogramCtrl m_ctlDownloadSpeed;
	//{{AFX_MSG(CFlashGetToolBar)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLASHGETTOOLBAR_H__FCF8336F_8B89_4B81_967E_51AF2814340D__INCLUDED_)
