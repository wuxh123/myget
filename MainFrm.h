// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__5C41C8D3_D26B_4223_A02B_81991C75FCD7__INCLUDED_)
#define AFX_MAINFRM_H__5C41C8D3_D26B_4223_A02B_81991C75FCD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "TabView.h"
#include "TabDef.h" 

#include "itemlistview.h"
#include "indexTreeview.h"
#include "contentlistview.h"
#include "CategoryTree.h"
#include "FlashGetToolBar.h"

class CFlashGetTray;

#define WM_USER_TREECTRL_SEL_CHANGED				WM_USER + 102
#define WM_USER_DOWNLOAD_THREAD_REFRESH_MAIN_FRAME	WM_USER + 103
#define WM_USER_FLASH_GET_TRAY						WM_USER + 104

#define WM_USER_REFRESH_SPEED						WM_USER + 105

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnDynamicTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CFlashGetToolBar    m_wndToolBar;
//	CReBar      m_wndReBar;
//	CDialogBar  m_wndDlgBar;
public:
	UINT ReloadPopMenu(CMenu *pPopMenu);
	void ReloadResource();
	void RefreshViews();
	BOOL IsCanMove(HTREEITEM hMovedItem, HTREEITEM hMoveToItem);
	CTabView	m_wndIndexBar;
	CItemListView		*m_pItemListView;
	CIndexTreeView		*m_pIndexTreeView;
	CContentListView	*m_pContentListView;
	CCategoryTree	*m_pMainCategoryTree;
	virtual void GetMessageString( UINT nID, CString& rMessage ) const;
// Generated message map functions
protected:
	CFlashGetTray * m_pFlashGetTray;
	CImageList m_ItemIconList;
	CSplitterWnd m_wndSplitterLower;
	CSplitterWnd m_wndSplitter;
#define MAX_SUPPORTTED_LANGUAGES 100
	char *m_szarLanguageINIs[MAX_SUPPORTTED_LANGUAGES][2];
	int m_iCountOfLanguages;

	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnCategoryNew();
	afx_msg void OnCategoryProp();
	afx_msg void OnCategoryMoveTo();
	afx_msg void OnUpdateCategoryMoveTo(CCmdUI* pCmdUI);
	afx_msg void OnCategoryContextProp();
	afx_msg void OnCategoryContextMoveTo();
	afx_msg void OnCategoryContextNew();
	afx_msg void OnUpdateCategoryContextMoveTo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCategoryContextDelete(CCmdUI* pCmdUI);
	afx_msg void OnCategoryContextDelete();
	afx_msg void OnCategoryDelete();
	afx_msg void OnUpdateCategoryDelete(CCmdUI* pCmdUI);
	afx_msg void OnCategoryContextOpen();
	afx_msg void OnToolsOptions();
	afx_msg void OnUpdateCategoryNew(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCategoryProp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateJobMenu(CCmdUI* pCmdUI);
	afx_msg void OnToolsDefaultDownloadProperties();
	afx_msg void OnJobNewDownload();
	afx_msg void OnJobAddBatchDownload();
	afx_msg void OnJobProperties();
	afx_msg void OnJobStart();
	afx_msg void OnDownloadRefreshMainFrame(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
//	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnUpdateSpeed(WPARAM wParam, LPARAM lParam);
	afx_msg void OnJobPause();
	afx_msg void OnJobMoveUp();
	afx_msg void OnJobMoveDown();
	afx_msg void OnViewLanguage(UINT nID);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void prv_UpdateMenu();
	void prv_UpdateDownloadCategory();
	void prv_DeleteCategory();
	BOOL prv_IsCanDeleteCategory();
	BOOL prv_IsCanMoveCategory();
	void prv_ShowCategoryDlg(int iDlgType);
	CImageList m_enImageList, m_disImageList;
	BOOL prv_CreateTabView();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__5C41C8D3_D26B_4223_A02B_81991C75FCD7__INCLUDED_)
