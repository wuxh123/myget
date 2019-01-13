#if !defined(AFX_INDEXTREEVIEW_H__B39622E0_202F_4A7A_BA72_DB42363B7F5C__INCLUDED_)
#define AFX_INDEXTREEVIEW_H__B39622E0_202F_4A7A_BA72_DB42363B7F5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IndexItemView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIndexTreeView view

class CListItem;
class CIndexTreeView : public CTreeView
{
protected:
	CIndexTreeView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CIndexTreeView)

// Attributes
public:

// Operations
public:
	void ReloadResource();
	void RefreshView();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIndexTreeView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CIndexTreeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	CListItem *m_pCurListItem;
	CImageList *m_pImageList;
	int m_iOldParentType;
	//{{AFX_MSG(CIndexTreeView)
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void prv_RefreshView(CListItem *pListItem);
	void prv_SetupList(BOOL bIsForDownloading);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INDEXTREEVIEW_H__B39622E0_202F_4A7A_BA72_DB42363B7F5C__INCLUDED_)
