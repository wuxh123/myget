// mybar.cpp : implementation file
//

#include "stdafx.h"
#include "tabview.h"
#include "tabdef.h"
#include "CategoryTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabView

CTabView::CTabView()
{
   m_IdMenuResource = NULL;
   m_DbClickMsg = NULL;
   m_SelChangedMsg = NULL;
   m_DefLParam = TABVIEW_DEFAULT_LPARAM;

}

CTabView::~CTabView()
{
   
}


BEGIN_MESSAGE_MAP(CTabView, baseCTabView)
    //{{AFX_MSG_MAP(CTabView)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_DESTROY()
    ON_NOTIFY(TCN_SELCHANGE, TAB_CTRL_ID, OnSelchangeTab)
    ON_NOTIFY(TCN_SELCHANGING, TAB_CTRL_ID, OnSelchangingTab)

    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTabView message handlers

int CTabView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    if (baseCTabView::OnCreate(lpCreateStruct) == -1)
        return -1;

    SetSCBStyle(GetSCBStyle() | SCBS_SHOWEDGES | SCBS_SIZECHILD);

    m_TabCtrl.Create(WS_CHILD|WS_VISIBLE|TCS_BOTTOM,
                     CRect(10,10,50,50),this, TAB_CTRL_ID);   
    
    //m_TabCtrl.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
    m_font.CreateStockObject(DEFAULT_GUI_FONT);

    m_TabCtrl.SetFont(&m_font);
    
    //AddTab("Bad");
    //AddTab("IS");
    //AddTab("Good");

    //SetDirToTree(1,"C:\\Novell\\*.*");
   
    return 0;
}

void CTabView::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
    baseCTabView::OnUpdateCmdUI(pTarget, bDisableIfNoHndler);

    UpdateDialogControls(pTarget, bDisableIfNoHndler);
}

void CTabView::OnSize(UINT nType, int cx, int cy) 
{
   CRect rc;
   
   baseCTabView::OnSize(nType, cx, cy);
   
   GetClientRect(rc);
   m_TabCtrl.MoveWindow(rc);
   NormalizeVisibleTree();
   
}


void CTabView::AddTab (char *chptr)
{
	CCategoryTree  *Tree1         ; 
	Tree1  =    new CCategoryTree ;
	
	//Add Tab :
	m_TabCtrl.InsertTab(chptr);

	//Create and add Tree :
	Tree1->CreateTree(&m_TabCtrl, TREE_VIEW_ID + m_TabCtrl.GetCount());

  	Tree1->DragAcceptFiles(TRUE);  //for drop commands. 


	m_TreeList.AddTail(Tree1);
}



void CTabView::OnDestroy() 
{
   baseCTabView::OnDestroy();

   DeleteAllTrees();
}


CTreeCtrl* CTabView::GetTreeByIndex(int index)
{ 
   if((index >= 0)&&(index <= m_TreeList.GetCount()))
   {
	   POSITION       pos  = m_TreeList.FindIndex( index );
      return ( (CTreeCtrl*)m_TreeList.GetAt( pos ) );
   }

   return NULL;
}




void CTabView::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CRect        rect;
	CTreeCtrl *Treeptr   = GetVisibleTree();
   
	ShowTree(Treeptr);
	*pResult = 0;
}




void CTabView::OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CTreeCtrl *Treeptr  = GetVisibleTree();

   Treeptr -> ShowWindow(SW_HIDE);
	*pResult = 0;
}







void CTabView::prvSetTreeRect(CRect *rect)
{
   CRect    lTabRect;
	CPoint   pointTL,pointBR;

	m_TabCtrl.GetClientRect(rect);
	m_TabCtrl.GetItemRect(m_TabCtrl.GetCurSel(), &lTabRect);
	pointTL = rect -> TopLeft();
	pointBR.x = rect->Width();
	pointBR.y = lTabRect.top;
	
   rect->SetRect(pointTL, pointBR);  //final rect of tree view.
   
   rect->InflateRect(2,2);
   rect->DeflateRect(FRAME_SIZE, FRAME_SIZE);	
}





void CTabView::ShowTree(CTreeCtrl *Treeptr)
{
   CRect rect;

   prvSetTreeRect(&rect);
   Treeptr -> MoveWindow(rect);
   Treeptr->ShowWindow(SW_SHOW);
}





CTreeCtrl* CTabView::GetVisibleTree()
{
   int selected = m_TabCtrl.GetCurSel();
   
   if(selected  ==  CB_ERR)
   {
      return NULL;
   }
   return(GetTreeByIndex(selected));
}





void CTabView::NormalizeVisibleTree()
{
   CTreeCtrl *Treeptr = GetVisibleTree();
   
   if(Treeptr != NULL)
   {
      ShowTree(Treeptr);
   }
}



void CTabView::DeleteAllTrees()
{
   int index;
   int count = m_TreeList.GetCount();
   CTreeCtrl *Treeptr;

   for(index = count-1; index >= 0; index--)
   {
      Treeptr = GetTreeByIndex(index);
      delete Treeptr;
   }
    
   m_TreeList.RemoveAll(); 
}





void CTabView::SetMenu(UINT ResMenu)
{
   m_IdMenuResource = ResMenu;
}

void CTabView::SetDbClickMsg(UINT msg)
{
   m_DbClickMsg = msg;
}

void CTabView::SetSelChangedMsg(UINT msg)
{
	m_SelChangedMsg = msg;
}




LPARAM CTabView::GetDefaultLParam()
{
   return  m_DefLParam;
}

void CTabView::SetDefaultLParam(LPARAM param)
{
   m_DefLParam = param;
}
