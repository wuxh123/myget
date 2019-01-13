#if !defined(AFX_CATEGORYTREE_H__E3CCA786_3EB8_11D5_9A6A_0020E0101471__INCLUDED_)
#define AFX_CATEGORYTREE_H__E3CCA786_3EB8_11D5_9A6A_0020E0101471__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CategoryTree.h : header file
//
class CTreeItem;
class CListItem;
class CJCDFile;
/////////////////////////////////////////////////////////////////////////////
// CCategoryTree window

#define ROOT_OF_FLASHGET ((HTREEITEM)-1)

class CCategoryTree : public CTreeCtrl
{
// Construction
public:
	CCategoryTree();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCategoryTree)
	//}}AFX_VIRTUAL

// Implementation
public:
	void ReloadResource();
	void MoveDownloadedItemToDownloadedCategory(CListItem *pListItem);
	void AddNewDownloadItem(CListItem *pListItem);
	HTREEITEM MoveCategory(HTREEITEM hMovedItem, HTREEITEM hMoveToItem);
	void DeleteCategory(HTREEITEM hItem = TVI_ROOT, BOOL bFreeMEM = TRUE);
	DWORD GetTotalItemCount(HTREEITEM hItem = TVI_ROOT);
	BOOL SaveToFile(LPCTSTR lpszFileName);
	BOOL InsertTreeItem(CTreeItem *pTreeItem);
	BOOL AttachListItem(CListItem *pListItem);
	BOOL prv_LoadAndInsertItem(CJCDFile *pJCDFile);
	BOOL LoadFromFile(LPCTSTR lpszFileName);
	HTREEITEM m_hSelectedTreeItem;
	HTREEITEM GetHTREEITEMByID(int iID, HTREEITEM hCurrentItem = TVI_ROOT);
	int GetUniqueID();
	BOOL CreateTree(CWnd *Parent, UINT nId);
	virtual ~CCategoryTree();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCategoryTree)
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	BOOL prv_SaveItemToFile(HTREEITEM hItem, CJCDFile *pJCDFile);
	int prv_GetMaxID(HTREEITEM hItem);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CATEGORYTREE_H__E3CCA786_3EB8_11D5_9A6A_0020E0101471__INCLUDED_)
