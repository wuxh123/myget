// IndexItemView.cpp : implementation file
//

#include "stdafx.h"
#include "MyGet.h"
#include "IndexTreeView.h"
#include "ListItem.h"
#include "MainFrm.h"
#include "TreeItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIndexTreeView

IMPLEMENT_DYNCREATE(CIndexTreeView, CTreeView)

CIndexTreeView::CIndexTreeView()
{
	m_iOldParentType= -1;
	m_pImageList	= NULL;
	m_pCurListItem	= NULL;
}

CIndexTreeView::~CIndexTreeView()
{
}


BEGIN_MESSAGE_MAP(CIndexTreeView, CTreeView)
	//{{AFX_MSG_MAP(CIndexTreeView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIndexTreeView drawing

void CIndexTreeView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CIndexTreeView diagnostics

#ifdef _DEBUG
void CIndexTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CIndexTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CIndexTreeView message handlers

void CIndexTreeView::RefreshView()
{
//	SetRedraw(FALSE);

	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;

	CCategoryTree *pTreeCtrl = (CCategoryTree *)pMainFrame->m_wndIndexBar.GetTreeByIndex(0);

	HTREEITEM hSelectedItem = pTreeCtrl->GetSelectedItem();

	ASSERT(hSelectedItem != NULL);

	CTreeItem *pTreeItem = (CTreeItem *)pTreeCtrl->GetItemData(hSelectedItem);
	

	int iParentTreeItemType = pTreeItem->GetType();

	//prv_SetupList(iParentTreeItemType == 2);

	if (m_iOldParentType != iParentTreeItemType)
	{
		//‘2’在这里代表‘正在下载’。
		if (m_iOldParentType == 2 || iParentTreeItemType == 2 || m_iOldParentType == -1)
		{	
			prv_SetupList(iParentTreeItemType == 2);
			m_iOldParentType = iParentTreeItemType;
			ReloadResource();
		}


	}


//	m_iOldParentType = pTreeItem->GetType();
//	prv_SetupList( m_iOldParentType == 2);			

	//ReloadResource();

	int iFocusedItem = pMainFrame->m_pItemListView->GetFocusedItem();
	
	CListItem *pListItem;
	if (iFocusedItem >= 0)
	{
		pListItem = (CListItem *)pMainFrame->m_pItemListView->GetListCtrl().GetItemData(iFocusedItem);
	}
	else
	{
		pListItem = NULL;
	}
		
	prv_RefreshView(pListItem);
	
//	SetRedraw(TRUE);	
	
}

void CIndexTreeView::prv_SetupList(BOOL bIsForDownloading)
{
	CTreeCtrl &TreeCtrl = GetTreeCtrl();

	TreeCtrl.DeleteAllItems();


	if (bIsForDownloading)
	{
		//图表/日志节点暂定1
		HTREEITEM hParentItem = TreeCtrl.InsertItem(TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM, "Chart/Log",
							7, 7, 0, 0, CHART_CONTENT_TYPE, TVI_ROOT, TVI_LAST);

		//将来在这个节点下面还要加入Jet1，Jet2,...,Jet8等孩子。
		//暂定10-17。
	}
	else
	{
		//详细信息暂定3
		TreeCtrl.InsertItem(TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM, "Information",
							9, 9, 0, 0, INFO_CONTENT_TYPE, TVI_ROOT, TVI_LAST);

		//链接暂定4
		TreeCtrl.InsertItem(TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM, "Links",
							10, 10, 0, 0, LINK_CONTENT_TYPE, TVI_ROOT, TVI_LAST);
				
	}

	TreeCtrl.SelectItem(TreeCtrl.GetRootItem());

	((CMainFrame *)AfxGetMainWnd())->m_pContentListView->RefreshView();
}

void CIndexTreeView::OnInitialUpdate() 
{
	CTreeView::OnInitialUpdate();
	
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;

	CTreeCtrl *pTreeCtrl = pMainFrame->m_wndIndexBar.GetTreeByIndex(0);

	m_pImageList = pTreeCtrl->GetImageList(TVSIL_NORMAL);

	ASSERT(m_pImageList != NULL);

	GetTreeCtrl().SetImageList(m_pImageList, TVSIL_NORMAL);
		
}

void CIndexTreeView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;

	CContentListView *pContentListView = pMainFrame->m_pContentListView;

	pContentListView->RefreshView();
	*pResult = 0;
}

BOOL CIndexTreeView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= TVS_SHOWSELALWAYS | TVS_HASLINES;
	return CTreeView::PreCreateWindow(cs);
}

void CIndexTreeView::prv_RefreshView(CListItem *pListItem)
{
	CTreeCtrl &TreeCtrl = GetTreeCtrl();

	if (m_iOldParentType == 2) //downloading
	{
		if (m_pCurListItem != pListItem)
		{
			m_pCurListItem = pListItem;
			HTREEITEM hItem = TreeCtrl.GetChildItem(TVI_ROOT);

			TreeCtrl.SelectItem(hItem);

			while (TreeCtrl.ItemHasChildren(hItem))
			{
				TreeCtrl.DeleteItem(TreeCtrl.GetChildItem(hItem));
			}
		}

		if (!pListItem)
		{
			return;
		}

		if (TreeCtrl.GetCount() < (DWORD)(pListItem->GetCountOfJets() + 1))
		{
			int iJetIndex = JETLOG_CONTENT_TYPE_BASE;

			CString strJetName;
			HTREEITEM hParentItem = TreeCtrl.GetChildItem(TVI_ROOT);
			HTREEITEM hItem = hParentItem;
		
			int iBeginIndex = TreeCtrl.GetCount() - 1;
			for (int i = iBeginIndex; i < pListItem->GetCountOfJets(); i ++)
			{
				strJetName.Format("Jet%d", i);
				hItem = TreeCtrl.InsertItem(TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM, strJetName,
					8, 8, 0, 0, iJetIndex + i + 1, hParentItem, TVI_LAST);
	
			}

			TreeCtrl.Expand(hParentItem, TVE_EXPAND);
		}
		else if (TreeCtrl.GetCount() > (DWORD)(pListItem->GetCountOfJets() + 1))
		{
			HTREEITEM hRootItem = TreeCtrl.GetRootItem();

			TreeCtrl.SelectItem(hRootItem);

			while (TreeCtrl.ItemHasChildren(hRootItem))
			{
				TreeCtrl.DeleteItem(TreeCtrl.GetChildItem(hRootItem));
			}
		}
	}

}

void CIndexTreeView::ReloadResource()
{
	HTREEITEM hItem = GetTreeCtrl().GetRootItem();
	CString strVal;

	ASSERT(hItem != NULL);

	if (m_iOldParentType == 2)
	{
		//Chart/Log
		strVal = APP_GET_RSCSTR("String", 61209);

		if (!strVal.IsEmpty())
		{
			GetTreeCtrl().SetItemText(hItem, strVal);
		}
	}
	else
	{
		//Information
		strVal = APP_GET_RSCSTR("String", 61303);

		if (!strVal.IsEmpty())
		{
			GetTreeCtrl().SetItemText(hItem, strVal);
		}

		//Link
		hItem = GetTreeCtrl().GetNextSiblingItem(hItem);

		ASSERT(hItem != NULL);
		/*if (hItem == NULL)
		{
			return;
		}
		*/
		strVal = APP_GET_RSCSTR("String", 61309);

		if (!strVal.IsEmpty())
		{
			GetTreeCtrl().SetItemText(hItem, strVal);
		}

	}

}
