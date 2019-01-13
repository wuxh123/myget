// MyGetDoc.h : interface of the CMyGetDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYGETDOC_H__BA20CDF6_AE19_4D3F_8BEC_17FFC902B59B__INCLUDED_)
#define AFX_MYGETDOC_H__BA20CDF6_AE19_4D3F_8BEC_17FFC902B59B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMyGetDoc : public CDocument
{
protected: // create from serialization only
	CMyGetDoc();
	DECLARE_DYNCREATE(CMyGetDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyGetDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyGetDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMyGetDoc)
	afx_msg void OnFileSaveas();
	afx_msg void OnFileSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYGETDOC_H__BA20CDF6_AE19_4D3F_8BEC_17FFC902B59B__INCLUDED_)
