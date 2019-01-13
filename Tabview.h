#if !defined(__TAB_VIEW_H__)
#define __TAB_VIEW_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// mybar.h : header file
//

#include "stdafx.h"

#include "sizecbar.h"
//#include "TreeControl.h"
#include "TabCtrl.h"

//#include "scbarg.h"
//#include "common.h"


/////////////////////////////////////////////////////////////////////////////
// CMyBar window

#ifndef baseCTabView
#define  baseCTabView CSizingControlBar
#endif

class CTabView : public baseCTabView
{
// Construction
public:
             CTabView();
   virtual  ~CTabView();

// Attributes
public:

   void           AddTab                 (char *chptr);
   CTreeCtrl* GetTreeByIndex         (int index  );  
   void           prvSetTreeRect         (CRect *rect);
   void           ShowTree               (CTreeCtrl *Treeptr);

// Overridables
    virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTabView)
    //}}AFX_VIRTUAL

// Implementation
public:
	void SetDefaultLParam ( LPARAM param);
	
   LPARAM GetDefaultLParam();

   LPARAM ReturnLParam  (HTREEITEM hItem);
	void   SetDbClickMsg (UINT msg);
	void   SetSelChangedMsg(UINT msg);
	void   SetMenu       (UINT Resenu);
	

	void           NormalizeVisibleTree   (void);
	CTreeCtrl*  GetVisibleTree         (void);
   
  

  
   

protected:
   // CButton m_wndChild;
   // CFont   m_font;
    
    UINT       m_IdMenuResource;
    UINT       m_DbClickMsg;
	UINT		m_SelChangedMsg;

    CTabSheet  m_TabCtrl;
    CFont      m_font;
    CObList    m_TreeList;

    LPARAM     m_DefLParam;
    // Generated message map functions
protected:
	 void DeleteAllTrees     (void);
	
    //{{AFX_MSG(CTabView)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnDestroy();
    afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(__TAB_VIEW_H__)
