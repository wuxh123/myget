// CategoryTree.cpp : implementation file
//

#include "stdafx.h"
#include "myget.h"
#include "CategoryTree.h"
#include "MainFrm.h"
#include "TreeItem.h"
#include "ListItem.h"
#include "JCDFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCategoryTree

CCategoryTree::CCategoryTree()
{
	m_hSelectedTreeItem = TVI_ROOT;
}

CCategoryTree::~CCategoryTree()
{
}


BEGIN_MESSAGE_MAP(CCategoryTree, CTreeCtrl)
	//{{AFX_MSG_MAP(CCategoryTree)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCategoryTree message handlers

BOOL CCategoryTree::CreateTree(CWnd *Parent, UINT nId)
{
   int ret = Create( WS_CHILD| TVS_HASBUTTONS | TVS_LINESATROOT | 
                     TVS_HASLINES | WS_EX_ACCEPTFILES | TVS_SHOWSELALWAYS,
				   	   CRect(0,0,10,10), Parent, nId );

   ModifyStyleEx(0, WS_EX_CLIENTEDGE);
   
   return ret;

}


void CCategoryTree::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_hSelectedTreeItem = pNMTreeView->itemNew.hItem;
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	pMainFrame->RefreshViews();

	*pResult = 0;
}



int CCategoryTree::GetUniqueID()
{
	return prv_GetMaxID(GetRootItem()) + 1;
}

int CCategoryTree::prv_GetMaxID(HTREEITEM hItem)
{
	int iMaxID = -1;

	CTreeItem *pTreeItem = (CTreeItem *)GetItemData(hItem);

	int iID = pTreeItem->GetID();

	if (iID > iMaxID)
	{
		iMaxID = iID;
	}

	CListItem *pListItem;

	for (int i = 0; i < pTreeItem->GetSubListItemCount(); i ++)
	{
		pListItem = pTreeItem->GetSubListItem(i);

		iID = pListItem->GetID();
	
		if (iID > iMaxID)
		{
			iMaxID = iID;
		}
	}

	if (ItemHasChildren(hItem))
	{
		hItem = GetChildItem(hItem);

		while (hItem)
		{
			iID = prv_GetMaxID(hItem);

			if (iID > iMaxID)
			{
				iMaxID = iID;
			}

			hItem = GetNextSiblingItem(hItem);
		}
	}

	return iMaxID;
}

HTREEITEM CCategoryTree::GetHTREEITEMByID(int iID, HTREEITEM hCurrentItem)
{
	if (hCurrentItem == TVI_ROOT)
	{
		hCurrentItem = GetRootItem();
	}


	CTreeItem *pTreeItem = (CTreeItem *)GetItemData(hCurrentItem);

	if (pTreeItem->GetID() == iID)
	{
		return hCurrentItem;
	}

	//Find All its chidren.
	if (ItemHasChildren(hCurrentItem))
	{
		HTREEITEM hNextItem;
		HTREEITEM hChildItem = GetChildItem(hCurrentItem);
		
		while (hChildItem != NULL)
		{
			HTREEITEM hRet = GetHTREEITEMByID(iID, hChildItem);
			if ( hRet > 0)
			{
				return hRet;
			}

			hNextItem =GetNextItem(hChildItem, TVGN_NEXT);
			hChildItem = hNextItem;
		}
	}

	return NULL;

}



void CCategoryTree::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CPoint point;
	GetCursorPos(&point);
	
	CPoint testPoint(point);
	ScreenToClient(&testPoint);

	m_hSelectedTreeItem = HitTest(testPoint);

	if (m_hSelectedTreeItem == NULL)
	{
		return;
	}

	CMenu menuContext;
	menuContext.LoadMenu(IDM_CATEGORY_CONTEXT);
	CMenu *menuPopup = menuContext.GetSubMenu(0);

	menuPopup->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON , point.x, point.y, pMainFrame);
	SetCapture();	
	*pResult = 1;
}

void CCategoryTree::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (GetCapture() == this)
	{
		ReleaseCapture();
		m_hSelectedTreeItem = GetSelectedItem();	
	}

	CTreeCtrl::OnMouseMove(nFlags, point);

}

BOOL CCategoryTree::LoadFromFile(LPCTSTR lpszFileName)
{

	if (GetFileAttributes(lpszFileName) == - 1)
	{
		return FALSE;
	}

	//should inform  the downloading thread firsr
	//....

	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();
	pMainFrame->m_pItemListView->GetListCtrl().DeleteAllItems();

	DeleteCategory();

	DeleteAllItems();

	CJCDFile JCDFile(lpszFileName, READ_JCD_FILE);

	for (DWORD dwIndex = 0; dwIndex < JCDFile.GetItemCount(); dwIndex ++)
	{
		prv_LoadAndInsertItem(&JCDFile);
	}

	ReloadResource();
	//try to select the "download" category. 
	//maybe the code is not good, but it does work.
	SelectItem(GetChildItem(GetRootItem()));

	return TRUE;
}

BOOL CCategoryTree::prv_LoadAndInsertItem(CJCDFile *pJCDFile)
{
	if (pJCDFile->GetNextItemClass() != ITEM_CLASS_LISTITEM)
	{ // Tree Item
		CTreeItem *pTreeItem = new CTreeItem;
		pTreeItem->LoadFromFile(pJCDFile);

		InsertTreeItem(pTreeItem);
	}
	else
	{ // List Item
		CListItem *pListItem = new CListItem;
		pListItem->LoadFromFile(pJCDFile);

		AttachListItem(pListItem);
	}

	return TRUE;
}


BOOL CCategoryTree::AttachListItem(CListItem *pListItem)
{
	HTREEITEM hTreeItem = GetHTREEITEMByID(pListItem->GetParentID());

	//Temporary code, 
	//when we can not find a parent tree item for a list item, there must be someting wrong,
	//So we have to ingore this list item temporarily.

	if (!hTreeItem)
	{
		return TRUE;
	}

	CTreeItem *pTreeItem = (CTreeItem *)GetItemData(hTreeItem);
	 
	pTreeItem->AddSubListItem(pListItem);

	return TRUE;
}

BOOL CCategoryTree::InsertTreeItem(CTreeItem *pTreeItem)
{
	TVITEM tvItem;
	TVINSERTSTRUCT tvIns;

	tvItem.pszText		= (LPSTR)pTreeItem->GetCategoryName();
	tvItem.cchTextMax	= strlen(tvItem.pszText);
	tvItem.mask			= TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvItem.lParam		= (LPARAM)pTreeItem;
	tvItem.iImage		= tvItem.iSelectedImage = pTreeItem->GetImageIndex();

	if (pTreeItem->GetParentID() == (int)ROOT_OF_FLASHGET)
	{
		tvIns.hParent = 0;
	}
	else
	{
		tvIns.hParent = GetHTREEITEMByID(pTreeItem->GetParentID());

	}

	tvIns.hInsertAfter = TVI_LAST;
	tvIns.item = tvItem;

	InsertItem(&tvIns);

	Expand(tvIns.hParent, TVE_EXPAND);
	return TRUE;
}

BOOL CCategoryTree::SaveToFile(LPCTSTR lpszFileName)
{
	CJCDFile JCDFile(lpszFileName, WRITE_JCD_FILE, GetTotalItemCount());

	return prv_SaveItemToFile(GetRootItem(), &JCDFile);	
}

DWORD CCategoryTree::GetTotalItemCount(HTREEITEM hItem)
{
	int iItemCount = 0;

	if (hItem == TVI_ROOT)
	{
		hItem = GetRootItem();
	}

	CTreeItem *pTreeItem = (CTreeItem *)GetItemData(hItem);

	iItemCount = pTreeItem->GetSubListItemCount();

	if (ItemHasChildren(hItem))
	{
		hItem = GetChildItem(hItem);
		
		while(hItem)
		{
			iItemCount += GetTotalItemCount(hItem);

			hItem = GetNextSiblingItem(hItem);
		}
	}

	
	return iItemCount + 1;

}



BOOL CCategoryTree::prv_SaveItemToFile(HTREEITEM hItem, CJCDFile *pJCDFile)
{
	CTreeItem *pTreeItem = (CTreeItem *)GetItemData(hItem);

	pTreeItem->SaveToFile(pJCDFile);

	if (ItemHasChildren(hItem))
	{
		hItem = GetChildItem(hItem);

		while (hItem)
		{
			prv_SaveItemToFile(hItem, pJCDFile);

			hItem = GetNextSiblingItem(hItem);
		}
	}

	return TRUE;
}

void CCategoryTree::DeleteCategory(HTREEITEM hItem, BOOL bFreeMEM)
{
	if (hItem == TVI_ROOT)
	{
		hItem = GetRootItem();
	}

	if (hItem == NULL)
	{
		return;
	}

	if (bFreeMEM)
	{
		CTreeItem *pTreeItem = (CTreeItem *)GetItemData(hItem);
		
		delete pTreeItem;
	}

	if (ItemHasChildren(hItem))
	{
		HTREEITEM hChildItem = GetChildItem(hItem);

		while(hChildItem)
		{
			DeleteCategory(hChildItem, bFreeMEM);

			hChildItem = GetNextSiblingItem(hChildItem);
		}
	}

//	DeleteItem(hItem);

}

HTREEITEM CCategoryTree::MoveCategory(HTREEITEM hMovedItem, HTREEITEM hMoveToItem)
{
	TCHAR szText[1024];
	TVITEM tvItem;
	tvItem.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	tvItem.hItem = hMovedItem;
	tvItem.pszText = szText;
	tvItem.cchTextMax = 1024;
	GetItem(&tvItem);

	TVINSERTSTRUCT tvIns;
	tvIns.hParent = hMoveToItem;
	tvIns.hInsertAfter = TVI_LAST;
	tvIns.item = tvItem;

	hMoveToItem = InsertItem(&tvIns);
	
	if (ItemHasChildren(hMovedItem))
	{
		HTREEITEM hChildItem = GetChildItem(hMovedItem);

		while (hChildItem)
		{
			MoveCategory(hChildItem, hMoveToItem);

			hChildItem = GetNextSiblingItem(hChildItem);
		}
	}

	DeleteItem(hMovedItem);

	return hMoveToItem;
}

void CCategoryTree::AddNewDownloadItem(CListItem *pListItem)
{
	HTREEITEM hItem = GetHTREEITEMByID(DOWNLOAD_CATEGORY_ID);

	if (hItem == NULL)
	{
		return;
	}

	CTreeItem *pTreeItem = (CTreeItem *)GetItemData(hItem);

	pListItem->SetID(GetUniqueID());
	pListItem->SetParentID(pTreeItem->GetID());

	CTime time;
	time.GetCurrentTime();

	pListItem->SetItemCreateTime(time.GetTime());

	//Now insert the list item into the first positon.
	//then it will be displayed in the last
	pTreeItem->AddSubListItem(pListItem);

	
	if (hItem == GetSelectedItem())
	{
		CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();

		pMainFrame->m_pItemListView->InsertItem(pListItem);
	}
	
}

void CCategoryTree::MoveDownloadedItemToDownloadedCategory(CListItem *pListItem)
{
	//if DownloadedToCategory is equal to "downloading" category, do not need be moved.
	if (pListItem->GetDownloadedToCategory() == DOWNLOADING_ITEM)
	{
		return;
	}

	HTREEITEM hDownloadingCategory = GetHTREEITEMByID(DOWNLOADING_ITEM); 
	HTREEITEM hDownloadToCategory = GetHTREEITEMByID(pListItem->GetDownloadedToCategory());

	//if the "DownloadedToCategory" can not be found, return! Maybe need warning???
	if (hDownloadToCategory == NULL)
	{
		return;
	}

	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();
	CItemListView *pItemListView = pMainFrame->m_pItemListView;

	CTreeItem *pCurTreeItem = (CTreeItem *)GetItemData(GetSelectedItem());

	//if current selected category is downloading category
	//then we need to delete the download item from ItemListView first.
	if (pCurTreeItem->GetID() == DOWNLOADING_ITEM)
	{
		pItemListView->DeleteItem(pListItem);
	}

	//Remove completed item from downloading category.
	CTreeItem *pDownloadingTreeItem = (CTreeItem *)GetItemData(hDownloadingCategory);
	pDownloadingTreeItem->RemoveSubListItem(pListItem);

	//Insert Completed item to the "DownloadedTo" Category.
	CTreeItem *pDownloadedToTreeItem = (CTreeItem *)GetItemData(hDownloadToCategory);
	pDownloadedToTreeItem->AddSubListItem(pListItem);

	//If current category is equal to "DownloadedTo" category, refreshing is needed.
	if (pCurTreeItem == pDownloadedToTreeItem)
	{
		pItemListView->InsertItem(pListItem);
	}
}





void CCategoryTree::ReloadResource()
{
	static int iarID[] = {2, 3, 8};
	static int iSizeOfID = sizeof(iarID) / sizeof(iarID[0]);
	static int iResourceID[] = {61204, 61206, 61231};

	HTREEITEM hItem;
	CString strCaption;
	for (int i = 0; i < iSizeOfID; i ++)
	{
		strCaption = APP_GET_RSCSTR("String", iResourceID[i]);

		if (strCaption.GetLength() == 0)
		{
			if (!strCaption.LoadString(iResourceID[i]))
			{
				continue;
			}
		}

		hItem = GetHTREEITEMByID(iarID[i]);

		ASSERT(hItem != NULL);

		CTreeItem *pTreeItem = (CTreeItem *)GetItemData(hItem);

		pTreeItem->SetCategoryName(strCaption);

		SetItemText(hItem, strCaption);
	}
}
