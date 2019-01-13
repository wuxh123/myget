// MyGetView.h : interface of the CMyGetView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYGETVIEW_H__D151B792_FDA8_4C4A_A9F6_CE50A42EB5AA__INCLUDED_)
#define AFX_MYGETVIEW_H__D151B792_FDA8_4C4A_A9F6_CE50A42EB5AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMyGetView : public CView
{
protected: // create from serialization only
	CMyGetView();
	DECLARE_DYNCREATE(CMyGetView)

// Attributes
public:
	CMyGetDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyGetView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyGetView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMyGetView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MyGetView.cpp
inline CMyGetDoc* CMyGetView::GetDocument()
   { return (CMyGetDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYGETVIEW_H__D151B792_FDA8_4C4A_A9F6_CE50A42EB5AA__INCLUDED_)
