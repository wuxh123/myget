// ItemListView.cpp : implementation file
//

#include "stdafx.h"
#include "MyGet.h"
#include "ItemListView.h"
#include "ListItem.h"
#include "MainFrm.h"
#include "ContentListView.h"
#include "TreeItem.h"
#include "CommonUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT ThreadRefreshItemListView(LPVOID pParam )
{
	CEvent *pEventStop = (CEvent *)pParam;
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetModuleState()->m_pCurrentWinApp->m_pMainWnd;
	CItemListView *pItemListView = pMainFrame->m_pItemListView;
//	DWORD dwCountVisibleItems;
	DWORD dwEventState;
	HANDLE hEvent[3];
	hEvent[0] = pEventStop[0].m_hObject;
	hEvent[1] = pEventStop[1].m_hObject;
	hEvent[2] = pEventStop[2].m_hObject;

__THREAD_BEGIN:
	//Event 0 stands for "Restart Refresh";
	//Event 2 stands for "Start Refresh".
	//Event 2 stands for "Quit".

	//dwEventState = WaitForMultipleObjects(3, hEvent, FALSE, INFINITE);
	
	//while (TRUE)//dwEventState != WAIT_OBJECT_0 + 1)
	for (;;)
	{
		dwEventState = WaitForMultipleObjects(3, hEvent, FALSE, INFINITE);

		switch(dwEventState)
		{
		case WAIT_OBJECT_0:
			pEventStop[0].ResetEvent();		
			break;

		case WAIT_OBJECT_0 + 1:
			goto __START_REFRESH;
			break;

		case WAIT_OBJECT_0 + 2:

			return 0;
			break;
		
		}

		//dwEventState = WaitForMultipleObjects(3, hEvent, FALSE, INFINITE);
	}


__START_REFRESH:
	//pEventStop[1].ResetEvent();		
	TRACE("Checking now 1...\n");
	if (pItemListView->IsMoving())
	{
		goto __THREAD_BEGIN;
	}

	//新的刷新策略。循环监视ListItem,如果Dirty，才刷新。
	/*
	DWORD dwItemIndex = pItemListView->GetListCtrl().GetTopIndex();
	DWORD dwLastIndex = dwItemIndex + pItemListView->GetListCtrl().GetCountPerPage() + 1;
	DWORD dwItemCount = pItemListView->GetListCtrl().GetItemCount();
	*/

	DWORD dwItemIndex;
	DWORD dwItemCount = pItemListView->GetListCtrl().GetItemCount();
	TRACE("Checking now 2...\n");
	//for (; dwItemIndex < dwLastIndex && dwItemIndex < dwItemCount; dwItemIndex ++)
	for (dwItemIndex = 0; dwItemIndex < dwItemCount; dwItemIndex ++)
	{
		dwEventState = WaitForMultipleObjects(3, hEvent, FALSE, 0);
	
		if (dwEventState == WAIT_OBJECT_0 || dwEventState == WAIT_OBJECT_0 + 1)
		{
			//Event 0 is set, Restart;
			pEventStop[0].ResetEvent();		

			goto __THREAD_BEGIN;
		}
		else if (dwEventState == WAIT_OBJECT_0 + 2)
		{
			//Event 2 is set, quit.
			return 0;
		}

		try
		{
			if (pItemListView->IsMoving())
			{
				goto __THREAD_BEGIN;
			}
			CListItem *pListItem = (CListItem *)pItemListView->GetListCtrl().GetItemData(dwItemIndex);

			if (pListItem->IsDirty())
			{
				if (pItemListView->IsMoving())
				{
					TRACE("Moving now....\n");
					goto __THREAD_BEGIN;
				}
//				pItemListView->RefreshItem(dwItemIndex, FALSE);
				pItemListView->RefreshItem(dwItemIndex, TRUE);
				pListItem->SetDirty(FALSE);
			}
		}
		catch(CException *e)
		{
			e;
			goto __THREAD_BEGIN;
		}
		
	}

	/*
	DWORD dwItemCount = pItemListView->GetListCtrl().GetItemCount();
	dwCountVisibleItems = pItemListView->GetListCtrl().GetCountPerPage();

	for (DWORD i = 0; i < dwItemCount; i ++)
	{
		dwEventState = WaitForMultipleObjects(3, hEvent, FALSE, 0);
	
		if (dwEventState == WAIT_OBJECT_0 || dwEventState == WAIT_OBJECT_0 + 1)
		{
			//Event 0 is set, Restart;
			pEventStop[0].ResetEvent();		

			goto __THREAD_BEGIN;
		}
		else if (dwEventState == WAIT_OBJECT_0 + 2)
		{
			//Event 2 is set, quit.
			return 0;
		}

		if (i == dwCountVisibleItems + 2)
		{
			pItemListView->GetListCtrl().SetRedraw(TRUE);
		}

		pItemListView->RefreshItem(i, FALSE);

	}
*/
	//pItemListView->SetMoving(FALSE);
	//pItemListView->Invalidate();
	//pItemListView->SetRedraw(TRUE);

//	pEventStop[1].SetEvent();
	goto __THREAD_BEGIN;

	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// CItemListView

IMPLEMENT_DYNCREATE(CItemListView, CListView)

CItemListView::CItemListView()
{
	m_iOldParentType	= -1;
	m_iFocusedItem		= -1;
	m_bIsMoving			= FALSE;
	m_piVisibleColumn	= NULL;

	m_eventStopRefresh[0].ResetEvent();
	m_eventStopRefresh[1].ResetEvent();
	m_eventStopRefresh[2].ResetEvent();
	
}

CItemListView::~CItemListView()
{
	if (m_piVisibleColumn)
	{
		delete m_piVisibleColumn;

		m_piVisibleColumn = NULL;
	}

}


BEGIN_MESSAGE_MAP(CItemListView, CListView)
	//{{AFX_MSG_MAP(CItemListView)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CItemListView drawing

void CItemListView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CItemListView diagnostics

#ifdef _DEBUG
void CItemListView::AssertValid() const
{
	CListView::AssertValid();
}

void CItemListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CItemListView message handlers


void CItemListView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();
	
	ModifyStyle (LVS_TYPEMASK, LVS_REPORT);

	GetListCtrl().SetExtendedStyle(LVS_EX_SUBITEMIMAGES | LVS_EX_FULLROWSELECT );
	GetListCtrl().SetImageList(&m_ListItemIconList, LVSIL_SMALL);

	GetListCtrl().GetHeaderCtrl()->SetImageList(&m_ilHeader);

	AfxBeginThread(ThreadRefreshItemListView, (LPVOID)&m_eventStopRefresh, THREAD_PRIORITY_TIME_CRITICAL);

//	SetTimer(6, 1000, 0);
	UINT uiTimerID = SetTimer(6, REFRESH_INTERVAL, 0);
	if (uiTimerID == 0)
	{
		TRACE("SetTimer Error....\n");
	}
	else
	{
		TRACE("Get Time ID is -- [%d]\n", uiTimerID);
	}

}

BOOL CItemListView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= LVS_SHOWSELALWAYS;

	return CListView::PreCreateWindow(cs);
}


void CItemListView::prv_SetupHeaders()
{
	m_iFocusedItem		= -1;

	int nColumnCount = GetListCtrl().GetHeaderCtrl()->GetItemCount();

	// Delete all of the columns.
	for (int i=0; i < nColumnCount; i++)
	{
		GetListCtrl().DeleteColumn(0);
	}



	// Add columns.

	CAppRegs *pAppRegs = ((CMyGetApp *)AfxGetApp())->GetAppRegs();
	DWORD dwColumnType;
	DWORD dwColumnIndex;
	DWORD dwColumnAlignStyle;
	DWORD dwColumnWidth;

	LVCOLUMN lvCol;
	lvCol.mask  = LVCF_IMAGE | LVCF_WIDTH | LVCF_FMT;
	lvCol.fmt	= LVCFMT_IMAGE;
	if (m_iOldParentType == 2)
	{
		dwColumnType = DOWNLOAD_CATEGORY_COLUMN;
	}
	else
	{
		dwColumnType = OTHER_CATEGORY_COLUMN;
	}

	if (m_piVisibleColumn)
	{
		delete m_piVisibleColumn;
		m_piVisibleColumn = NULL;
	}

	m_piVisibleColumn = new int[pAppRegs->GetVisibleColumnCount(dwColumnType)];
	for (DWORD dwVisibleColumnIndex = 0; dwVisibleColumnIndex < pAppRegs->GetVisibleColumnCount(dwColumnType); dwVisibleColumnIndex ++)
	{
		dwColumnIndex = pAppRegs->GetColumnIndexByScreenOrder(dwColumnType, dwVisibleColumnIndex);
		dwColumnAlignStyle = pAppRegs->GetColumnAlignStyle(dwColumnType, dwColumnIndex);
		dwColumnWidth = pAppRegs->GetColumnWidth(dwColumnType, dwColumnIndex);

		m_piVisibleColumn[dwVisibleColumnIndex] = dwColumnIndex;

		lvCol.fmt	= LVCFMT_IMAGE | dwColumnAlignStyle;
		lvCol.cx	= dwColumnWidth;

		switch(dwColumnIndex)
		{
		case COLUMN_STATUS:
			lvCol.iImage = 0;	
			GetListCtrl().InsertColumn(dwVisibleColumnIndex, &lvCol);
			break;
		case COLUMN_RESUME:
			lvCol.iImage = 1;
			GetListCtrl().InsertColumn(dwVisibleColumnIndex, &lvCol);
			break;
		default:
			GetListCtrl().InsertColumn(dwVisibleColumnIndex, "", dwColumnAlignStyle, dwColumnWidth);
			break;
		}
	}

}


//关于IndexListView的常量定义工作有待注册表解决之后。
//暂时不需要太多处理。
void CItemListView::prv_FillListView(CTreeItem *pTreeItem)
{
	CListCtrl &ListCtrl = GetListCtrl();
	//KillTimer(6);
	SetMoving(TRUE);
	m_eventStopRefresh[1].ResetEvent();
	//m_eventStopRefresh[0].SetEvent();
	

//	GetListCtrl().DeleteAllItems();

	int iItemCount = ListCtrl.GetItemCount();

	int iItem;

	if (iItemCount > pTreeItem->GetSubListItemCount())
	{
		for (iItem = iItemCount - 1; iItem >= pTreeItem->GetSubListItemCount(); iItem --)
		{
			ListCtrl.DeleteItem(iItem);
		}
	}
	else if (pTreeItem->GetSubListItemCount() > iItemCount)
	{
		for (iItem = iItemCount; iItem < pTreeItem->GetSubListItemCount(); iItem ++)
		{
			ListCtrl.InsertItem(iItem, "");
		}

	}

	
	CListItem *pListItem;

	m_bIsMoving = TRUE;

	for (int i = 0; i < pTreeItem->GetSubListItemCount(); i ++)
	{
	
		pListItem = pTreeItem->GetSubListItem(i);
		pListItem->SetDirty(TRUE);

		if (m_iOldParentType == DOWNLOADING_ITEM)
		{
			ListCtrl.SetItemData(pTreeItem->GetSubListItemCount() - i - 1, (DWORD)pListItem);
		}
		else
		{
			ListCtrl.SetItemData(i, (DWORD)pListItem);
		}
		
	}

	ListCtrl.EnsureVisible(0, FALSE);
	m_eventStopRefresh[0].SetEvent();
	m_eventStopRefresh[1].SetEvent();

	SetMoving(FALSE);

	/*
	UINT uiTimerID = SetTimer(6, REFRESH_INTERVAL, 0);
	if (uiTimerID == 0)
	{
		TRACE("SetTimer Error....\n");
	}
	else
	{
		TRACE("Get Time ID is -- [%d]\n", uiTimerID);
	}
	*/
}

BOOL CItemListView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	CBitmap bitmap;
	bitmap.LoadMappedBitmap(IDB_LIST_ITEM_ICON); 
	m_ListItemIconList.Create(16, 16, ILC_COLOR32 | ILC_MASK, 13, 1);
	m_ListItemIconList.Add(&bitmap, RGB(255, 0, 255));
	//m_ListItemIconList.Create(16, 16, ILC_COLOR32 | ILC_MASK, 13, 1);
	//m_ListItemIconList.Add(&bitmap, RGB(255, 0, 255));

	m_ilHeader.Create(IDB_LIST_ITEM_ICON, 16, 2, CLR_DEFAULT);
	
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CItemListView::RefreshView()
{

	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	CCategoryTree *pTreeCtrl = (CCategoryTree *)pMainFrame->m_wndIndexBar.GetTreeByIndex(0);

	HTREEITEM hSelectedItem = pTreeCtrl->GetSelectedItem();
	ASSERT(hSelectedItem != NULL);

	CTreeItem *pTreeItem = (CTreeItem *)pTreeCtrl->GetItemData(hSelectedItem);
	SetRedraw(FALSE);

	int iParentTreeItemType = pTreeItem->GetType();

	if (m_iOldParentType != iParentTreeItemType)
	{
		m_iOldParentType = iParentTreeItemType;
		prv_SetupHeaders();			

		ReloadResource();
	}

	m_iFocusedItem = -1;
	prv_FillListView(pTreeItem);
}



void CItemListView::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;

	if (m_bIsMoving)
	{
		return;
	}

	if (pNMListView->uNewState & LVIS_SELECTED)
	{
		m_iFocusedItem = pNMListView->iItem;
	}

	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;

	pMainFrame->m_pIndexTreeView->RefreshView();
	pMainFrame->m_pContentListView->RefreshView();


//	TRACE("Current focused item is : [%d] \r\n", GetListCtrl().GetNextItem(-1, LVIS_FOCUSED));
	
}

int CItemListView::GetFocusedItem()
{
	return m_iFocusedItem;
}

void CItemListView::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CMenu menuPop;
	if (m_iOldParentType == DOWNLOADING_ITEM)
	{
		VERIFY(menuPop.LoadMenu(IDM_DOWNLOAD_JOB_CONTEXT));
	}
	else
	{
		VERIFY(menuPop.LoadMenu(IDM_COMMON_JOB_CONTEXT));
	}

	CMenu *pPopup = menuPop.GetSubMenu(0);

	CPoint point;
	GetCursorPos(&point);

	pPopup->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON , point.x, point.y, AfxGetMainWnd());
	*pResult = 0;
}




BOOL CItemListView::IsFocusedDownLoadItemEnable(DWORD dwMenuItemIndex)
{
	//条件：1。属于“正在下载”Category。
	//2。当前Download Item的状态是“暂停”或者“错误”。
	BOOL bEnable = FALSE;

	int iItem = GetFocusedItem();

	switch (dwMenuItemIndex)
	{
	case IDM_JOB_START:
		if (iItem != -1 && m_iOldParentType == DOWNLOADING_ITEM)
		{
			CListItem *pListItem = (CListItem *)GetListCtrl().GetItemData(iItem);
			
			if (pListItem->GetStatus() == ITEM_STATUS_DOWNLOAD_ERROR
				|| pListItem->GetStatus() == ITEM_STATUS_PAUSED)
			{
				bEnable = TRUE;
			}
		}

		break;

	case IDM_JOB_PAUSE:
		if (iItem != -1 && m_iOldParentType == DOWNLOADING_ITEM)
		{
			CListItem *pListItem = (CListItem *)GetListCtrl().GetItemData(iItem);
			
			if (pListItem->GetStatus() == ITEM_STATUS_DOWNLOADING
				|| pListItem->GetStatus() == ITEM_STATUS_SCHEDULED
				|| pListItem->GetStatus() == ITEM_STATUS_READY)
			{
				bEnable = TRUE;
			}
		}

		break;

	case IDM_JOB_SCHEDULE:
		if (iItem != -1)
		{
			CListItem *pListItem = (CListItem *)GetListCtrl().GetItemData(iItem);
			
			if (pListItem->GetStatus() == ITEM_STATUS_DOWNLOAD_ERROR
				|| pListItem->GetStatus() == ITEM_STATUS_PAUSED)
			{
				bEnable = TRUE;
			}
		}

		break;

	case IDM_JOB_MOVE_TO:
		if (iItem != -1)
		{
			bEnable = TRUE;
		}

		break;

	case IDM_JOB_DELETE:
		if (iItem != -1)
		{
			bEnable = TRUE;
		}

		break;

	case IDM_JOB_PROPERTIES:
		if (iItem != -1)
		{
			CListItem *pListItem = (CListItem *)GetListCtrl().GetItemData(iItem);
			
			if (pListItem->GetStatus() == ITEM_STATUS_DOWNLOAD_ERROR
				|| pListItem->GetStatus() == ITEM_STATUS_COMPLETED
				|| pListItem->GetStatus() == ITEM_STATUS_PAUSED)
			{
				bEnable = TRUE;
			}
		}

		break;

	case IDM_JOB_MOVE_UP:
		//Browse all the selected Item, if one of them can be move up, it is ok.
		if (m_iOldParentType == DOWNLOADING_ITEM)
		{
			//Get last selected item;
			DWORD dwLastSelectedItem = 0;
			POSITION pos = GetListCtrl().GetFirstSelectedItemPosition();

			if (pos == NULL)
			{
				bEnable = FALSE;

				break;
			}

			while (pos)
			{
				dwLastSelectedItem = GetListCtrl().GetNextSelectedItem(pos);
			}

			//if the last selected item is larger than the selected count, then they can be moved up.
			if (dwLastSelectedItem >= GetListCtrl().GetSelectedCount())
			{
				bEnable = TRUE;
			}
		}

		break;

	case IDM_JOB_MOVE_DOWN:
		//Browse all the selected Item, if one of them can be moved down, it is ok.
		if (m_iOldParentType == DOWNLOADING_ITEM)
		{
			//Get first selected item;
			DWORD dwFirstSelectedItem = 0;
			POSITION pos = GetListCtrl().GetFirstSelectedItemPosition();

			if (pos)
			{
				dwFirstSelectedItem = GetListCtrl().GetNextSelectedItem(pos);
			}
			else
			{
				bEnable = FALSE;

				break;
			}

			

			if (((DWORD)GetListCtrl().GetItemCount()) > dwFirstSelectedItem + GetListCtrl().GetSelectedCount())
			{
				bEnable = TRUE;
			}
			
		}

		break;

	case IDM_JOB_CHECK_FOR_UPDATE:
		if (iItem != -1)
		{
			CListItem *pListItem = (CListItem *)GetListCtrl().GetItemData(iItem);
			
			if (pListItem->GetStatus() == ITEM_STATUS_COMPLETED)
			{
				bEnable = TRUE;
			}
		}

		break;

	case IDM_JOB_DOWNLOAD_AGAIN:
		if (iItem != -1 && m_iOldParentType != DOWNLOADING_ITEM)
		{
			bEnable = TRUE;
		}

		break;

	case IDM_JOB_OPEN_DOWNLOADED_FILE:
		if (iItem != -1)
		{
			CListItem *pListItem = (CListItem *)GetListCtrl().GetItemData(iItem);
			
			if (pListItem->GetStatus() == ITEM_STATUS_COMPLETED)
			{
				bEnable = TRUE;
			}
		}

		break;

	case IDM_JOB_BROWSE_REFERRER:
		if (iItem != -1)
		{
			CListItem *pListItem = (CListItem *)GetListCtrl().GetItemData(iItem);
			
			if (pListItem->GetREF())
			{
				bEnable = TRUE;
			}

		}

		break;

	case IDM_JOB_COMMENT:
		if (iItem != -1)
		{
			bEnable = TRUE;
		}

		break;

	default:
		break;

	}

	return bEnable;
}


//The functon will be called by download thread only.
void CItemListView::RefreshItem(CListItem *pListItem, BOOL bRedraw /* = TRUE */)
{
	for (int i = 0; i < GetListCtrl().GetItemCount(); i ++)
	{
		if ((CListItem *)GetListCtrl().GetItemData(i) == pListItem)
		{
			//Update the ListView
			
			if (bRedraw)
			{
				SetRedraw(FALSE);
			}

			RefreshItem(i, FALSE);
			
			if (bRedraw)
			{
				SetRedraw(TRUE);			
			}

			if (!m_bIsMoving && i == GetFocusedItem())
			{
				CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;

				pMainFrame->m_pIndexTreeView->RefreshView();
				pMainFrame->m_pContentListView->RefreshView();
			}

		}
	}
}


void CItemListView::RefreshItem(int iIndex, BOOL bRedraw /* = TRUE */)
{

	if (bRedraw)
	{
		SetRedraw(FALSE);
	}

	if (iIndex >= GetListCtrl().GetItemCount())
	{
		return;
	}
	//fist refresh the icon
	CListItem *pListItem;

	try
	{
		pListItem = (CListItem *)GetListCtrl().GetItemData(iIndex);
	}
	catch (CException *e)
	{
		e;
		return;
	}

	if (pListItem == NULL)
	{
		return;
	}


	CAppRegs *pAppRegs = ((CMyGetApp *)AfxGetApp())->GetAppRegs();
	DWORD dwColumnType;
	DWORD dwColumnIndex;
	char szVal[MAX_PATH];

	LVITEM lvItem;
	lvItem.iItem = iIndex;

	if (m_iOldParentType == 2)
	{
		dwColumnType = DOWNLOAD_CATEGORY_COLUMN;
	}
	else
	{
		dwColumnType = OTHER_CATEGORY_COLUMN;
	}


	for (int dwVisibleColumnIndex = 0; dwVisibleColumnIndex < GetListCtrl().GetHeaderCtrl()->GetItemCount(); dwVisibleColumnIndex ++)
	{
		dwColumnIndex = m_piVisibleColumn[dwVisibleColumnIndex];

		lvItem.iSubItem = dwVisibleColumnIndex;

		switch(dwColumnIndex)
		{
		case COLUMN_STATUS:
			lvItem.mask = LVIF_IMAGE;
			GetListCtrl().GetItem(&lvItem);

			if (lvItem.iImage != pListItem->GetItemStatusIconIndex())
			{
				lvItem.iImage	= pListItem->GetItemStatusIconIndex();
				GetListCtrl().SetItem(&lvItem);
			}

			break;
		case COLUMN_RESUME:
			{
				lvItem.mask = LVIF_IMAGE;
				GetListCtrl().GetItem(&lvItem);
				
				int iSupportResumed = pListItem->IsSupportResumed();
				
				if (lvItem.iImage != iSupportResumed + 2)
				{
					if (iSupportResumed >= 0)
					{
						lvItem.iImage = iSupportResumed + 2;
					}
					else
					{
						lvItem.iImage = -1;
					}
					
					GetListCtrl().SetItem(&lvItem);
				}
			}
			
			break;

		case COLUMN_NAME:
			GetListCtrl().GetItemText(iIndex, dwVisibleColumnIndex, szVal, MAX_PATH);

			if (strcmp(pListItem->GetRename(), szVal) != 0)
			{
				GetListCtrl().SetItemText(iIndex, dwColumnIndex, pListItem->GetRename());
			}
			
			break;

		case COLUMN_SIZE:
			{
				CString strFileSize = GetListCtrl().GetItemText(iIndex, dwVisibleColumnIndex);
				
				pListItem->GetFileTotalLengthByString(szVal, 10);
				
				if (strFileSize.CompareNoCase(szVal) != 0)
				{
					GetListCtrl().SetItemText(iIndex, dwVisibleColumnIndex, szVal);
				}
			}
			break;

		case COLUMN_COMPLETEDSIZE:
			{

				int iCompletedSize = pListItem->GetFileDownloadedSize();
				CString strCompletedSize;

				if (iCompletedSize != 0)
				{
					strCompletedSize.Format("%d", iCompletedSize);
				}
				
				if (strCompletedSize.CompareNoCase(GetListCtrl().GetItemText(iIndex, dwVisibleColumnIndex)))
				{
					GetListCtrl().SetItemText(iIndex, dwVisibleColumnIndex, strCompletedSize);
				}


			}
			break;

		case COLUMN_PERCENTAGE:
			{
				int iCompletedSize	= pListItem->GetFileDownloadedSize();
				int iFileSize		= pListItem->GetFileSize();
				CString strPercentage;

				if (iFileSize > 0)
				{
					strPercentage.Format("%d%%", (DWORD)(((double)iCompletedSize / iFileSize) * 100));
				}

				GetListCtrl().SetItemText(iIndex, dwVisibleColumnIndex, strPercentage);
			}
			break;

		case COLUMN_COSTEDTIME:

			break;

		case COLUMN_LEFTTIME:
			break;

		case COLUMN_SPEED:
			break;

		case COLUMN_BLOCKS:
			break;

		case COLUMN_RETRIES:
			break;

		case COLUMN_URL:
			GetListCtrl().SetItemText(iIndex, dwVisibleColumnIndex, pListItem->GetURL());
			break;

		case COLUMN_COMMENT:
			GetListCtrl().SetItemText(iIndex, dwVisibleColumnIndex, pListItem->GetComment());
			break;

		case COLUMN_CREATETIME:
			break;

		case COLUMN_COMPLETEDTIME:
			break;
		}
	}

	////////////////////////////
	////////////////////////////
	if (bRedraw)
	{
		SetRedraw(TRUE);
	}

	return;

}




void CItemListView::SetMoving(BOOL bIsMoving)
{
	m_bIsMoving = bIsMoving;
}

void CItemListView::SetFocusedItem(int iItem)
{
	m_iFocusedItem = iItem;
}

void CItemListView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (GetListCtrl().HitTest(point) == -1)
	{
		m_iFocusedItem = -1;
	}

	CListView::OnLButtonDown(nFlags, point);
}

void CItemListView::DeleteItem(CListItem *pListItem)
{
	LVFINDINFO lvFindInfo;

	lvFindInfo.flags	= LVFI_PARAM;
	lvFindInfo.lParam	= (DWORD)pListItem;

	int iDelItem = GetListCtrl().FindItem(&lvFindInfo);

	if (iDelItem != -1)
	{
		if (iDelItem == m_iFocusedItem)
		{
			m_iFocusedItem = -1;
		}

		GetListCtrl().DeleteItem(iDelItem);
	}

}

void CItemListView::InsertItem(CListItem *pListItem)
{
	int iIndex;

	if (m_iOldParentType != DOWNLOADING_ITEM)
	{
		iIndex = 0;
	}
	else
	{
		iIndex = GetListCtrl().GetItemCount();
	}

	GetListCtrl().InsertItem(iIndex, "");
	GetListCtrl().SetItemData(iIndex, (DWORD)pListItem);

	RefreshItem(iIndex, TRUE);

}

BOOL CItemListView::IsMoving()
{
	return m_bIsMoving;
}


void CItemListView::ClearResource()
{
	m_eventStopRefresh[2].SetEvent();
}

void CItemListView::ReloadResource()
{
	CAppRegs *pAppRegs = ((CMyGetApp *)AfxGetApp())->GetAppRegs();
	DWORD dwColumnIndex;
	DWORD dwColumnType;
	LVCOLUMN lvColumn;

	if (m_iOldParentType == 2)
	{
		dwColumnType = DOWNLOAD_CATEGORY_COLUMN;
	}
	else
	{
		dwColumnType = OTHER_CATEGORY_COLUMN;
	}
	
	lvColumn.mask = LVCF_TEXT;

	//CComposedStrings ComposeStrings(APP_GET_RSCSTR("String", 61208));
	CComposedStrings ComposeStrings(61208);

	for (int i = 0; i < GetListCtrl().GetHeaderCtrl()->GetItemCount(); i ++)
	{
		//dwColumnIndex = pAppRegs->GetColumnIndexByScreenOrder(dwColumnType, i);
		dwColumnIndex = m_piVisibleColumn[i];

		lvColumn.pszText = (char *)ComposeStrings.GetSubString(dwColumnIndex);
		lvColumn.cchTextMax = strlen(lvColumn.pszText);
		GetListCtrl().SetColumn(i, &lvColumn);

	}
}


void CItemListView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	TRACE("Checking now 0...\n");
	if (!IsMoving())
	{
		m_eventStopRefresh[1].SetEvent();
	}
	else
	{
		TRACE("It is moving now...\n");
	}

	//CListView::OnTimer(nIDEvent);
}

void CItemListView::OnDestroy() 
{
	CListView::OnDestroy();
	
	// TODO: Add your message handler code here
	TRACE("Time 6 is off!\n");
	KillTimer(6);	
}
