#if !defined(AFX_CONTENTLISTVIEW_H__49D168BF_DB95_4BF8_8555_A21F03A53621__INCLUDED_)
#define AFX_CONTENTLISTVIEW_H__49D168BF_DB95_4BF8_8555_A21F03A53621__INCLUDED_

#include "ProgressChart.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ContentListView.h : header file
//



//define Content Listview Type
#define CHART_CONTENT_TYPE	1
#define INFO_CONTENT_TYPE	3
#define LINK_CONTENT_TYPE	4
#define JETLOG_CONTENT_TYPE_BASE 10


/////////////////////////////////////////////////////////////////////////////
// CContentListView view

class CListItem;

class CContentListView : public CListView
{
protected:
	CContentListView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CContentListView)

// Attributes
public:

// Operations
public:
	void ReloadResource();
	void RefreshView();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CContentListView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CContentListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	CListItem * m_pCurListItem;
	CImageList m_ImageList;
	int m_iOldParentIndex;
	CProgressChart m_ProgressChart;
	//{{AFX_MSG(CContentListView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void prv_FillJetInfo(CListItem *pListItem);
	void prv_FillChart(CListItem *pListItem);
	void prv_FillDetailInformation(CListItem *pListItem);
	void prv_RefreshView(CListItem *pListItem);
	void prv_SetupView();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTENTLISTVIEW_H__49D168BF_DB95_4BF8_8555_A21F03A53621__INCLUDED_)
