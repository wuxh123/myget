#if !defined(AFX_ITEMLISTVIEW_H__3A77D8BD_A0FE_4E3F_B1DB_CA67CCEDC95F__INCLUDED_)
#define AFX_ITEMLISTVIEW_H__3A77D8BD_A0FE_4E3F_B1DB_CA67CCEDC95F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ItemListView.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CItemListView view

#define REFRESH_INTERVAL	50

class CTreeItem;
class CListItem;

UINT ThreadRefreshItemListView(LPVOID pParam );
class CItemListView : public CListView
{
protected:
	CItemListView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CItemListView)

// Attributes
public:

// Operations
public:
	void ReloadResource();
	void ClearResource();
	BOOL IsMoving();
	void InsertItem(CListItem *pListItem);
	void DeleteItem(CListItem *pListItem);
	void SetFocusedItem(int iItem);
	void SetMoving(BOOL bIsMoving);
	void RefreshItem(CListItem *pListItem, BOOL bRedraw = TRUE);
	void RefreshItem(int iIndex, BOOL bRedraw = TRUE);
	BOOL IsFocusedDownLoadItemEnable(DWORD dwMenuItemIndex);
	int GetFocusedItem();
	void RefreshView();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CItemListView)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CItemListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	CEvent m_eventStopRefresh[3];
	int m_iOldParentType;
	int m_iFocusedItem;
	//{{AFX_MSG(CItemListView)
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	LPINT m_piVisibleColumn;
	BOOL m_bIsMoving;
	CImageList m_ListItemIconList;
	CImageList m_ilHeader;
	void prv_SetupHeaders();
	void prv_FillListView(CTreeItem *pTreeItem);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ITEMLISTVIEW_H__3A77D8BD_A0FE_4E3F_B1DB_CA67CCEDC95F__INCLUDED_)
