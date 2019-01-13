// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MyGet.h"

#include <io.h>

#include "CategoryDlg.h"
#include "TreeItem.h"
#include "MainFrm.h"
#include "OptionSheet.h"
#include "DownloadDlg.h"
#include "AddBatchDownloadDlg.h"
//#include "MainScheduleThread.h"
#include "DownloadSchedulerThread.h"
#include "FlashGetTray.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(IDM_CATEGORY_NEW, OnCategoryNew)
	ON_COMMAND(IDM_CATEGORY_PROP, OnCategoryProp)
	ON_COMMAND(IDM_CATEGORY_MOVE_TO, OnCategoryMoveTo)
	ON_UPDATE_COMMAND_UI(IDM_CATEGORY_MOVE_TO, OnUpdateCategoryMoveTo)
	ON_COMMAND(IDM_CATEGORY_CONTEXT_PROP, OnCategoryContextProp)
	ON_COMMAND(IDM_CATEGORY_CONTEXT_MOVE_TO, OnCategoryContextMoveTo)
	ON_COMMAND(IDM_CATEGORY_CONTEXT_NEW, OnCategoryContextNew)
	ON_UPDATE_COMMAND_UI(IDM_CATEGORY_CONTEXT_MOVE_TO, OnUpdateCategoryContextMoveTo)
	ON_UPDATE_COMMAND_UI(IDM_CATEGORY_CONTEXT_DELETE, OnUpdateCategoryContextDelete)
	ON_COMMAND(IDM_CATEGORY_CONTEXT_DELETE, OnCategoryContextDelete)
	ON_COMMAND(IDM_CATEGORY_DELETE, OnCategoryDelete)
	ON_UPDATE_COMMAND_UI(IDM_CATEGORY_DELETE, OnUpdateCategoryDelete)
	ON_COMMAND(IDM_CATEGORY_CONTEXT_OPEN, OnCategoryContextOpen)
	ON_COMMAND(IDM_TOOLS_OPTIONS, OnToolsOptions)
	ON_UPDATE_COMMAND_UI(IDM_CATEGORY_NEW, OnUpdateCategoryNew)
	ON_UPDATE_COMMAND_UI(IDM_CATEGORY_PROP, OnUpdateCategoryProp)
	ON_UPDATE_COMMAND_UI(IDM_JOB_START, OnUpdateJobMenu)
	ON_COMMAND(IDM_TOOLS_DEFAULT_DOWNLOAD_PROPERTIES, OnToolsDefaultDownloadProperties)
	ON_COMMAND(IDM_JOB_NEW_DOWNLOAD, OnJobNewDownload)
	ON_COMMAND(IDM_JOB_ADD_BATCH_DOWNLOAD, OnJobAddBatchDownload)
	ON_COMMAND(IDM_JOB_PROPERTIES, OnJobProperties)
	ON_COMMAND(IDM_JOB_START, OnJobStart)
	//ON_MESSAGE(WM_USER_DOWNLOAD_THREAD_REFRESH_MAIN_FRAME, OnDownloadRefreshMainFrame)
	//ON_MESSAGE(WM_USER_REFRESH_SPEED, OnUpdateSpeed)
	ON_WM_SIZE()
	ON_COMMAND(IDM_JOB_PAUSE, OnJobPause)
	ON_COMMAND(IDM_JOB_MOVE_UP, OnJobMoveUp)
	ON_COMMAND(IDM_JOB_MOVE_DOWN, OnJobMoveDown)
	ON_UPDATE_COMMAND_UI(IDM_JOB_PAUSE, OnUpdateJobMenu)
	ON_UPDATE_COMMAND_UI(IDM_JOB_SCHEDULE, OnUpdateJobMenu)
	ON_UPDATE_COMMAND_UI(IDM_JOB_DELETE, OnUpdateJobMenu)
	ON_UPDATE_COMMAND_UI(IDM_JOB_PROPERTIES, OnUpdateJobMenu)
	ON_UPDATE_COMMAND_UI(IDM_JOB_MOVE_TO, OnUpdateJobMenu)
	ON_UPDATE_COMMAND_UI(IDM_JOB_MOVE_DOWN, OnUpdateJobMenu)
	ON_UPDATE_COMMAND_UI(IDM_JOB_MOVE_UP, OnUpdateJobMenu)
	ON_UPDATE_COMMAND_UI(IDM_JOB_CHECK_FOR_UPDATE, OnUpdateJobMenu)
	ON_UPDATE_COMMAND_UI(IDM_JOB_DOWNLOAD_AGAIN, OnUpdateJobMenu)
	ON_UPDATE_COMMAND_UI(IDM_JOB_OPEN_DOWNLOADED_FILE, OnUpdateJobMenu)
	ON_UPDATE_COMMAND_UI(IDM_JOB_BROWSE_REFERRER, OnUpdateJobMenu)
	ON_UPDATE_COMMAND_UI(IDM_JOB_COMMENT, OnUpdateJobMenu)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDM_VIEW_LANGUAGE_ENGLISH, IDM_VIEW_LANGUAGE_ENGLISH + MAX_SUPPORTTED_LANGUAGES - 1, OnViewLanguage)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnDynamicTipText)
    ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnDynamicTipText) 
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_SPEED_BAR,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_pItemListView		= NULL;
	m_pIndexTreeView	= NULL;
	m_pContentListView	= NULL;
	m_pMainCategoryTree = NULL;
	m_pFlashGetTray		= NULL;

	memset(m_szarLanguageINIs, 0, sizeof(m_szarLanguageINIs));
	m_iCountOfLanguages = 0;
}

CMainFrame::~CMainFrame()
{
	if (m_pFlashGetTray)
	{
		delete m_pFlashGetTray;
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	prv_UpdateMenu();
	m_enImageList.Create(IDB_MAINFRAME_ENABLE, 22, 21, CLR_DEFAULT);
	m_disImageList.Create(IDB_MAINFRAME_DISABLE, 22, 21, CLR_DEFAULT);

	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_INDEX_ITEM_ICON); 
	m_ItemIconList.Create(16, 16, ILC_COLOR8|ILC_MASK, 13, 1);
	m_ItemIconList.Add(&bitmap, CLR_DEFAULT);


	//if (!m_wndToolBar.CreateEx(this) ||
	if (!m_wndToolBar.CreateEx(this, 
		TBSTYLE_FLAT | CCS_ADJUSTABLE, 
		WS_EX_RIGHTSCROLLBAR | WS_CHILD | WS_VISIBLE | CBRS_GRIPPER | CBRS_ALIGN_TOP 
		| CBRS_TOOLTIPS | CBRS_FLYBY) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))

	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CToolBarCtrl &ToolCtrl = m_wndToolBar.GetToolBarCtrl();
	ToolCtrl.SetImageList(&m_enImageList);
	ToolCtrl.SetDisabledImageList(&m_disImageList);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndStatusBar.SetPaneInfo(1, ID_SPEED_BAR, SBPS_NORMAL, 81);
	m_wndStatusBar.SetPaneText(1, "0.00KB/S");

	// TODO: Remove this if you don't want tool tips
	//m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
	//	CBRS_TOOLTIPS | CBRS_FLYBY);

	if (prv_CreateTabView() != TRUE)
	{
		return -1;
	}

	m_wndToolBar.EnableDocking(CBRS_ALIGN_TOP);
	EnableDocking(CBRS_ALIGN_TOP);
	DockControlBar(&m_wndToolBar);

	//ReloadResource();
	ReloadPopMenu(GetMenu());

	m_pFlashGetTray = new CFlashGetTray(this, IDR_MAINFRAME, NIF_MESSAGE|NIF_ICON|NIF_TIP,
								WM_USER_FLASH_GET_TRAY, "FlashGet");

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_wndSplitter.CreateStatic(this, 2, 1);
	m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CItemListView), CSize(100, 100), pContext);

	m_wndSplitterLower.CreateStatic(&m_wndSplitter, 1, 2, WS_CHILD | WS_VISIBLE, m_wndSplitter.IdFromRowCol(1, 0));
	m_wndSplitterLower.CreateView(0, 0, RUNTIME_CLASS(CIndexTreeView), CSize(100, 100), pContext);
	m_wndSplitterLower.CreateView(0, 1, RUNTIME_CLASS(CContentListView), CSize(100, 100), pContext);

	m_pItemListView = (CItemListView *)m_wndSplitter.GetPane(0, 0);
	m_pIndexTreeView = (CIndexTreeView *)m_wndSplitterLower.GetPane(0, 0);
	m_pContentListView = (CContentListView *)m_wndSplitterLower.GetPane(0, 1);

	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style &= ~FWS_ADDTOTITLE;
 
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


BOOL CMainFrame::prv_CreateTabView()
{
   if (!m_wndIndexBar.Create(_T("Index View"), this, TAB_CTRL_ID))
   {
        TRACE0("Failed to create Tab View\n");
        return -1;      // fail to create
	}

   m_wndIndexBar.SetBarStyle(m_wndIndexBar.GetBarStyle() |
        CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
   //wndIndexBar.SetBarStyle(m_wndIndexBar.GetBarStyle() | CBRS_SIZE_DYNAMIC);

   m_wndIndexBar.IsFloating();
   m_wndIndexBar.SetInitialDockSize(150 /*Vert*/, 200/*Horz*/, 120/*float*/);

   EnableDocking(CBRS_ALIGN_ANY);

   m_wndIndexBar.EnableDocking(CBRS_ALIGN_ANY);

   m_wndIndexBar.AddTab("FlashGet");

   m_pMainCategoryTree = (CCategoryTree *)m_wndIndexBar.GetTreeByIndex(0);
   m_pMainCategoryTree->SetImageList(&m_ItemIconList, TVSIL_NORMAL);
   
    // This call MUST be the last in this function!!!
   DockControlBar(&m_wndIndexBar, AFX_IDW_DOCKBAR_LEFT);  
   
   return TRUE;

}



 
void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	m_pItemListView->ClearResource();
	CMyGetApp *pApp = (CMyGetApp *)AfxGetApp();

	pApp->GetDownloadSchedulerThread()->StopDownload();
	
	delete pApp->GetDownloadSchedulerThread();
	
	m_pMainCategoryTree->DeleteCategory();

	for (int i = 0; i < m_iCountOfLanguages; i ++)
	{
		delete m_szarLanguageINIs[i][0];
		delete m_szarLanguageINIs[i][1];
	}

	CFrameWnd::OnClose();
}





void CMainFrame::RefreshViews()
{

	m_pItemListView->RefreshView();
	m_pIndexTreeView->RefreshView();
	m_pContentListView->RefreshView();
}



void CMainFrame::OnCategoryNew() 
{
	prv_ShowCategoryDlg(CATEGORY_NEW_DLG);
	m_pMainCategoryTree->m_hSelectedTreeItem = m_pMainCategoryTree->GetSelectedItem();
}


void CMainFrame::OnCategoryProp() 
{
	prv_ShowCategoryDlg(CATEGORY_PROPERTY_DLG);
	m_pMainCategoryTree->m_hSelectedTreeItem = m_pMainCategoryTree->GetSelectedItem();
}

void CMainFrame::OnCategoryMoveTo() 
{
	prv_ShowCategoryDlg(CATEGORY_MOVETO_DLG);
	m_pMainCategoryTree->m_hSelectedTreeItem = m_pMainCategoryTree->GetSelectedItem();
}

void CMainFrame::prv_ShowCategoryDlg(int iDlgType)
{
	CCategoryDlg dlgCategory;
	dlgCategory.m_typeDlg = iDlgType;
	dlgCategory.m_pMainCategoryTree = m_pMainCategoryTree;
	dlgCategory.m_hCurrentItem = m_pMainCategoryTree->m_hSelectedTreeItem;
	
	if (dlgCategory.DoModal() == IDOK)
	{
		if (iDlgType == CATEGORY_MOVETO_DLG
			&& dlgCategory.m_bCanMove == FALSE)
		{
			AfxMessageBox(IDS_CANNOT_MOVE_DELETE_CATEGORY);
		}
		else if (iDlgType == CATEGORY_NEW_DLG)
		{
			m_pMainCategoryTree->SelectItem(m_pMainCategoryTree->GetHTREEITEMByID(dlgCategory.m_iNewItemID));
		}
	}
}

void CMainFrame::OnUpdateCategoryMoveTo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if (m_pMainCategoryTree->m_hSelectedTreeItem == TVI_ROOT)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	
	pCmdUI->Enable(prv_IsCanMoveCategory());
}

BOOL CMainFrame::IsCanMove(HTREEITEM hMovedItem, HTREEITEM hMoveToItem)
{
	HTREEITEM hItem = hMoveToItem;

	while (hItem)
	{
		if (hItem == hMovedItem)
		{
			return FALSE;
		}

		hItem = m_pMainCategoryTree->GetParentItem(hItem);
	}

	return TRUE;
}



void CMainFrame::OnCategoryContextProp() 
{
	// TODO: Add your command handler code here
	prv_ShowCategoryDlg(CATEGORY_PROPERTY_DLG);

}

void CMainFrame::OnCategoryContextMoveTo() 
{
	// TODO: Add your command handler code here
	prv_ShowCategoryDlg(CATEGORY_MOVETO_DLG);	
}

void CMainFrame::OnCategoryContextNew() 
{
	// TODO: Add your command handler code here
	prv_ShowCategoryDlg(CATEGORY_NEW_DLG);	
}

void CMainFrame::OnUpdateCategoryContextMoveTo(CCmdUI* pCmdUI) 
{
	if (m_pMainCategoryTree->m_hSelectedTreeItem == TVI_ROOT)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(prv_IsCanMoveCategory());	
}


void CMainFrame::OnUpdateCategoryContextDelete(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if (m_pMainCategoryTree->m_hSelectedTreeItem == TVI_ROOT)	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(prv_IsCanDeleteCategory());
}

BOOL CMainFrame::prv_IsCanMoveCategory()
{
	HTREEITEM hSelectedItem = m_pMainCategoryTree->m_hSelectedTreeItem;
	BOOL bEnable = FALSE;

	if (hSelectedItem)
	{
		CTreeItem *pselTreeItem = (CTreeItem *)m_pMainCategoryTree->GetItemData(hSelectedItem);

		if (!pselTreeItem->IsSystemItem())
		{
			bEnable = TRUE;
		}
	}

	return bEnable;
}

void CMainFrame::OnCategoryContextDelete() 
{
	// TODO: Add your command handler code here
	prv_DeleteCategory();
}

void CMainFrame::OnCategoryDelete() 
{
	// TODO: Add your command handler code here
	prv_DeleteCategory();
}

void CMainFrame::OnUpdateCategoryDelete(CCmdUI* pCmdUI) 
{
	if (m_pMainCategoryTree->m_hSelectedTreeItem == TVI_ROOT)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(prv_IsCanDeleteCategory());
}

BOOL CMainFrame::prv_IsCanDeleteCategory()
{
	HTREEITEM hSelectedItem = m_pMainCategoryTree->m_hSelectedTreeItem;
	BOOL bEnable = FALSE;

	if (hSelectedItem)
	{
		CTreeItem *pselTreeItem = (CTreeItem *)m_pMainCategoryTree->GetItemData(hSelectedItem);

		if (!pselTreeItem->IsSystemItem())
		{
			bEnable = TRUE;
		}
	}

	return bEnable;
}

void CMainFrame::prv_DeleteCategory()
{
	if (AfxMessageBox(IDS_DELETE_CATEGORY_PROMPT, MB_OKCANCEL) 
		== IDCANCEL)
	{
		return;
	}

	HTREEITEM hDelItem = m_pMainCategoryTree->m_hSelectedTreeItem;

	m_pMainCategoryTree->DeleteItem(hDelItem);
}

void CMainFrame::OnCategoryContextOpen() 
{
	m_pMainCategoryTree->SelectItem(m_pMainCategoryTree->m_hSelectedTreeItem);
	
}

void CMainFrame::OnToolsOptions() 
{
	// TODO: Add your command handler code here
	COptionSheet m_OptionSheet(IDS_OPTION_TITLE);

	m_OptionSheet.DoModal();
}



void CMainFrame::OnUpdateCategoryNew(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if (m_pMainCategoryTree->m_hSelectedTreeItem == TVI_ROOT)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	
}

void CMainFrame::OnUpdateCategoryProp(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if (m_pMainCategoryTree->m_hSelectedTreeItem == TVI_ROOT)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

}

void CMainFrame::OnUpdateJobMenu(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_pItemListView->IsFocusedDownLoadItemEnable(pCmdUI->m_nID));
}


void CMainFrame::OnToolsDefaultDownloadProperties() 
{
	// TODO: Add your command handler code here
	LPCTSTR lpszTitle = APP_GET_RSCSTR("String", IDS_DEFAULT_DOWNLOAD_PROPERTY_TITLE);
	//CDownloadDlg dlg(IDS_DEFAULT_DOWNLOAD_PROPERTY_TITLE, TYPE_DEFAULT_DOWNLOAD_PROPERTY);
	CDownloadDlg dlg(lpszTitle, TYPE_DEFAULT_DOWNLOAD_PROPERTY);
	dlg.DoModal();
}

void CMainFrame::OnJobNewDownload() 
{
	// TODO: Add your command handler code here
	LPCTSTR lpszTitle = APP_GET_RSCSTR("String", IDS_NEW_DOWNLOAD_TILE);

	CDownloadDlg dlg(lpszTitle, TYPE_NEW_DOWNLOAD);
	dlg.DoModal();
}

void CMainFrame::OnJobAddBatchDownload() 
{
	// TODO: Add your command handler code here
	CAddBatchDownloadDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnJobProperties() 
{
	// TODO: Add your command handler code here
	LPCTSTR lpszTitle = APP_GET_RSCSTR("String", IDS_PROP_TITLE);

	CDownloadDlg dlg(lpszTitle, TYPE_DOWNLOAD_ITEM_PROPERTY);
	dlg.DoModal();
}

void CMainFrame::OnJobStart() 
{
	// TODO: Add your command handler code here
	int iItem = m_pItemListView->GetFocusedItem();

	if (iItem == -1)
	{
		return;
	}

	CListItem *pListItem = (CListItem *)m_pItemListView->GetListCtrl().GetItemData(iItem);
	
	pListItem->SetStartState(START_RIGHTNOW);

	m_pItemListView->RefreshItem(pListItem);
}


void CMainFrame::OnDownloadRefreshMainFrame(WPARAM wParam, LPARAM lParam)
{
	CListItem * pListItem = (CListItem *)wParam;

	if (lParam == 1)
	{
		m_pMainCategoryTree->MoveDownloadedItemToDownloadedCategory(pListItem);
	}

	//m_pItemListView->RefreshItem(pListItem);


}

LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(message){
		
	case WM_USER_FLASH_GET_TRAY:       
		//如果是用户定义的消息
		if(lParam==WM_LBUTTONUP)
		{ 
			if (IsWindowVisible())
			{
				ShowWindow(SW_HIDE | SW_MINIMIZE);
			}
			else
			{
				ShowWindow(SW_SHOW);// | SW_RESTORE);
				SetForegroundWindow();
			}
		}
		else if(lParam==WM_RBUTTONDOWN){ 
			//鼠标右键单击弹出菜单
			CMenu menu;
			
			SetForegroundWindow();

			menu.LoadMenu(IDM_TRAY_CONTEXT); 
			//载入事先定义的菜单
			CMenu* pMenu=menu.GetSubMenu(0);
			CPoint pos;
			GetCursorPos(&pos);
			pMenu->TrackPopupMenu
				(TPM_LEFTALIGN|TPM_RIGHTBUTTON,
				pos.x,pos.y,AfxGetMainWnd());

			PostMessage(WM_NULL, 0, 0); 
		}
		break;
	case WM_SYSCOMMAND:     
		//如果是系统消息
		if(wParam==SC_MINIMIZE){ 
			//接收到最小化消息时主窗口隐藏
			AfxGetApp()->m_pMainWnd->
				ShowWindow(SW_HIDE);
			return 0;
		}
		break;
	}

	return CFrameWnd::WindowProc(message, wParam, lParam);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	m_wndToolBar.ResizeBar(cx);
}

void CMainFrame::OnUpdateSpeed(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your message handler code here and/or call default
	int iLastDataCount = lParam;
	
	//Refresh the SpeedBar and the status bar.
	//use default speed upper limit 8K temproyly.
	float fCountByKB = ((float)iLastDataCount) / 1024;
	
	m_wndToolBar.SetSpeed(fCountByKB);

	CString strSpeedText;
	strSpeedText.Format("%.2fKB/S", fCountByKB);

	m_wndStatusBar.SetPaneText(1, strSpeedText);
	
	//in the future, refresh the TRAY ICON ALSO!!

}


void CMainFrame::OnJobPause() 
{
	// TODO: Add your command handler code here
	int iItem = m_pItemListView->GetFocusedItem();

	if (iItem == -1)
	{
		return;
	}

	CListItem *pListItem = (CListItem *)m_pItemListView->GetListCtrl().GetItemData(iItem);
	
	pListItem->SetStartState(START_MANUAL);
	pListItem->SetFileStatus(UNCOMPLETED);

	m_pItemListView->RefreshItem(pListItem);
	
}

void CMainFrame::OnJobMoveUp() 
{
	// TODO: Add your command handler code here
	
	m_pItemListView->SetRedraw(FALSE);
	m_pItemListView->SetMoving(TRUE);

	int iLastUnSelectedItem = m_pItemListView->GetListCtrl().GetItemCount() - 1;

	for (int i = 0; i < m_pItemListView->GetListCtrl().GetItemCount(); i++)
	{
		UINT uiSelItemState = m_pItemListView->GetListCtrl().GetItemState(i, -1);

		if (uiSelItemState & LVIS_SELECTED)
		{
			if (i > iLastUnSelectedItem)
			{
				if (i == m_pItemListView->GetFocusedItem())
				{
					m_pItemListView->SetFocusedItem(i - 1);
				}

				CListItem *pUnSelListItem = (CListItem *)m_pItemListView->GetListCtrl().GetItemData(i - 1);
				UINT uiUnSelItemState = m_pItemListView->GetListCtrl().GetItemState(i - 1, -1);//LVIS_FOCUSED | LVIS_SELECTED);
				CListItem *pSelListItem = (CListItem *)m_pItemListView->GetListCtrl().GetItemData(i);

				m_pItemListView->GetListCtrl().SetItemData(i - 1, (DWORD)pSelListItem);			
				m_pItemListView->GetListCtrl().SetItemData(i, (DWORD)pUnSelListItem);

				m_pItemListView->RefreshItem(pUnSelListItem, FALSE);
				m_pItemListView->RefreshItem(pSelListItem, FALSE);

				m_pItemListView->GetListCtrl().SetItemState(i - 1, uiSelItemState, -1);//LVIS_FOCUSED | LVIS_SELECTED);
				m_pItemListView->GetListCtrl().SetItemState(i, uiUnSelItemState, -1);//LVIS_FOCUSED | LVIS_SELECTED);


				iLastUnSelectedItem = i;
			}
		}
		else
		{
			iLastUnSelectedItem = i;
		}
	}

	m_pItemListView->SetMoving(FALSE);

	m_pItemListView->GetListCtrl().SetRedraw(TRUE);
	
	prv_UpdateDownloadCategory();
}

void CMainFrame::OnJobMoveDown() 
{
	// TODO: Add your command handler code here
		m_pItemListView->SetRedraw(FALSE);
	m_pItemListView->SetMoving(TRUE);

	int iLastUnSelectedItem = 0;

	for (int i = m_pItemListView->GetListCtrl().GetItemCount() - 1; i >= 0; i--)
	{
		UINT uiSelItemState = m_pItemListView->GetListCtrl().GetItemState(i, -1);

		if (uiSelItemState & LVIS_SELECTED)
		{
			if (i < iLastUnSelectedItem)
			{
				if (i == m_pItemListView->GetFocusedItem())
				{
					m_pItemListView->SetFocusedItem(i + 1);
				}

				CListItem *pUnSelListItem	= (CListItem *)m_pItemListView->GetListCtrl().GetItemData(i + 1);
				UINT uiUnSelItemState		= m_pItemListView->GetListCtrl().GetItemState(i + 1, -1);
				CListItem *pSelListItem		= (CListItem *)m_pItemListView->GetListCtrl().GetItemData(i);

				m_pItemListView->GetListCtrl().SetItemData(i + 1, (DWORD)pSelListItem);
				m_pItemListView->GetListCtrl().SetItemData(i, (DWORD)pUnSelListItem);

				m_pItemListView->RefreshItem(pUnSelListItem, FALSE);
				m_pItemListView->RefreshItem(pSelListItem, FALSE);

				m_pItemListView->GetListCtrl().SetItemState(i + 1, uiSelItemState, -1);
				m_pItemListView->GetListCtrl().SetItemState(i, uiUnSelItemState, -1);

				iLastUnSelectedItem = i;
			}

		}
		else
		{
			iLastUnSelectedItem = i;
		}
	}


	m_pItemListView->SetMoving(FALSE);

	m_pItemListView->GetListCtrl().SetRedraw(TRUE);

	prv_UpdateDownloadCategory();
	
}

void CMainFrame::prv_UpdateDownloadCategory()
{
	CTreeItem *pTreeItem = (CTreeItem *)m_pMainCategoryTree->GetItemData(m_pMainCategoryTree->GetHTREEITEMByID(DOWNLOADING_ITEM));

	pTreeItem->ClearSubListItems();

	for (int i = 0; i < m_pItemListView->GetListCtrl().GetItemCount(); i++)
	{
		CListItem *pListItem = (CListItem *)m_pItemListView->GetListCtrl().GetItemData(i);

		pTreeItem->AddSubListItem(pListItem);
	}
}


void CMainFrame::ReloadResource()
{
	ReloadPopMenu(GetMenu());

	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_FRAME);

	m_pMainCategoryTree->ReloadResource();
	m_pItemListView->ReloadResource();
	m_pIndexTreeView->ReloadResource();
	m_pContentListView->ReloadResource();
}

UINT CMainFrame::ReloadPopMenu(CMenu *pPopMenu)
{
	int iTabPos;
	CString strNewMenuItem, strOldMenuItem;
	UINT nID;
	CString strMenuID;


	for (DWORD dwMenuItemIndex = 0; dwMenuItemIndex < pPopMenu->GetMenuItemCount(); dwMenuItemIndex ++)
	{
		CMenu *pSubMenu = pPopMenu->GetSubMenu(dwMenuItemIndex);
		if (pSubMenu)
		{
			nID = ReloadPopMenu(pSubMenu);

			if (nID <= 0)
			{
				continue;
			}

			strMenuID.Format("%dp", nID);
			nID = -1;
		}
		else
		{
			nID = pPopMenu->GetMenuItemID(dwMenuItemIndex);

			if (nID <= 0)
			{
				continue;
			}
		
			strMenuID.Format("%d", nID);
		}
		
		pPopMenu->GetMenuString(dwMenuItemIndex, strOldMenuItem, MF_BYPOSITION);

		iTabPos = strOldMenuItem.Find("\t");
			
		strNewMenuItem = APP_GET_RSCSTR("Menu", strMenuID);

		if (strNewMenuItem.IsEmpty())
		{
			continue;
		}

		if (iTabPos > -1)
		{
			strNewMenuItem = strNewMenuItem + strOldMenuItem.Mid(iTabPos);
		}

		pPopMenu->ModifyMenu(dwMenuItemIndex, MF_BYPOSITION, nID, strNewMenuItem);

	}

	return pPopMenu->GetMenuItemID(0);;
}

void CMainFrame::GetMessageString( UINT nID, CString& rMessage ) const
{
	CString strPrompt = APP_GET_RSCSTR("String", nID);

	if (!strPrompt.IsEmpty())
	{
		rMessage = strPrompt;
	}
	else
	{
		CFrameWnd::GetMessageString(nID, rMessage);
	}
}


BOOL CMainFrame::OnDynamicTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{

	static UINT arIDTool[] =
	{
		32773,		32774,		32779,		32775,		32776,		32799,
		32800,		32777,		32778,		57636,		32817,		32848,
		32791,		32786,		32812,		32804,		32805,		32866,
		57664,		32796,		57665
	};

	static int iSizeOfToolTips = sizeof(arIDTool) / sizeof(arIDTool[0]);

	static UINT arIDToolTips[] =
	{
		61254,		61255,		61256,		61257,		61258,		61263,
		61264,		61258,		61259,		61347,		61348,		61345,
		61260,		61227,		61394,		61395,		61396,		61398,
		61261,		61262,		61397
	};


		// OnNeedText should only be called for TTN_NEEDTEXT notifications!
	ASSERT(pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);

	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	CString strTipText; // used to hold final tool tip text before UNICODE conversion

	UINT nID = pNMHDR->idFrom;
	if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
		pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
	{
		// idFrom is actually the HWND of the tool
		nID = ((UINT)(WORD)::GetDlgCtrlID((HWND)nID));
	}

	//Set strTipText;
	BOOL bFound = FALSE;
	for (int i = 0; i < iSizeOfToolTips; i ++)
	{
		if (arIDTool[i] == nID)
		{
			strTipText = APP_GET_RSCSTR("String", arIDToolTips[i]);

			if (!strTipText.IsEmpty())
			{
				bFound = TRUE;
			}

			break;
		}
	}

	if (!bFound)
	{
		return FALSE;
	}


// handle conditionally for both UNICODE and non-UNICODE apps
#define _countof(array) (sizeof(array)/sizeof(array[0]))

#ifndef _UNICODE
	if (pNMHDR->code == TTN_NEEDTEXTA)
		lstrcpyn(pTTTA->szText, strTipText, _countof(pTTTA->szText));
	else
		_mbstowcsz(pTTTW->szText, strTipText, _countof(pTTTW->szText));
#else
	if (pNMHDR->code == TTN_NEEDTEXTA)
		_wcstombsz(pTTTA->szText, strTipText, _countof(pTTTA->szText));
	else
		lstrcpyn(pTTTW->szText, strTipText, _countof(pTTTW->szText));
#endif
	*pResult = 0;

	// bring the tooltip window above other popup windows
	::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0,
		SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOMOVE);

	return TRUE;    // message was handled

}

void CMainFrame::prv_UpdateMenu()
{
	CMenu *pMainMenu = GetMenu();
	ASSERT(pMainMenu);

	CMenu *pViewMenu = pMainMenu->GetSubMenu(3);
	ASSERT(pViewMenu);

	CMenu *pLanguageMenu = pViewMenu->GetSubMenu(7);
	ASSERT(pLanguageMenu);

	//Get count of the Language INI files under "Language" subfolder.
	long hFile;
	struct _finddata_t c_file;

	int iRet = 0;
	CString strLanguageINI;
	strLanguageINI.Format("%sLanguage\\", ((CMyGetApp *)AfxGetApp())->m_strLauchFolder);

	hFile = _findfirst(strLanguageINI + "*.ini", &c_file);

	if (hFile == -1)
	{
		iRet = -1;
	}

	TCHAR szLanguage[_MAX_FNAME];

	while(iRet == 0)
	{
		GetPrivateProfileString("Info", "Language", "", szLanguage, sizeof(szLanguage), strLanguageINI + c_file.name);

		m_szarLanguageINIs[m_iCountOfLanguages][0] = new TCHAR[strlen(szLanguage) + 1];
		strcpy(m_szarLanguageINIs[m_iCountOfLanguages][0], szLanguage);
		
		m_szarLanguageINIs[m_iCountOfLanguages][1] = new TCHAR[strlen(c_file.name) + 1];
		strcpy(m_szarLanguageINIs[m_iCountOfLanguages][1], c_file.name);

		m_iCountOfLanguages ++;

		iRet = _findnext(hFile, &c_file);
	}

	//sort the Array.
	TCHAR *pLanguageName;
	TCHAR *pLanguageINI;

	for (int i = 0; i < m_iCountOfLanguages - 1; i ++)
	{
		for (int j = i + 1; j < m_iCountOfLanguages; j ++)
		{
			if (strcmp(m_szarLanguageINIs[i][0], "English") == 0)
			{
				break;
			}

			if (strcmp(m_szarLanguageINIs[j][0], "English") == 0
				|| strcmp(m_szarLanguageINIs[j][0], m_szarLanguageINIs[i][0]) < 0)
			{
				pLanguageName	= m_szarLanguageINIs[j][0];
				pLanguageINI	= m_szarLanguageINIs[j][1];

				m_szarLanguageINIs[j][0] = m_szarLanguageINIs[i][0];
				m_szarLanguageINIs[j][1] = m_szarLanguageINIs[i][1];

				m_szarLanguageINIs[i][0] = pLanguageName;
				m_szarLanguageINIs[i][1] = pLanguageINI;
			}
		}
	}

	for (int k = 1; k < m_iCountOfLanguages; k ++)
	{
		pLanguageMenu->InsertMenu(k, MF_BYPOSITION, IDM_VIEW_LANGUAGE_ENGLISH + k, m_szarLanguageINIs[k][0]);
	}

	//Initialize the checked item.
	LPTSTR lpszLanguage;
	CAppRegs *pAppRegs = ((CMyGetApp *)AfxGetApp())->GetAppRegs();

	pAppRegs->GetVal(REG_GENERAL_LANGUAGEEX, &lpszLanguage);

	BOOL bFound = FALSE;
	for (int m = 0; m < m_iCountOfLanguages; m ++)
	{
		if (strcmp(lpszLanguage, m_szarLanguageINIs[m][1]) == 0)
		{
			pLanguageMenu->CheckMenuItem(IDM_VIEW_LANGUAGE_ENGLISH + m, MF_BYCOMMAND | MF_CHECKED);
			bFound = TRUE;
			break;
		}
	}

	if (!bFound)
	{
		pLanguageMenu->CheckMenuItem(IDM_VIEW_LANGUAGE_ENGLISH + 0, MF_BYCOMMAND | MF_CHECKED);
	}
}

void CMainFrame::OnViewLanguage(UINT nID) 
{
	// TODO: Add your command handler code here

	if (m_iCountOfLanguages == 0)
	{
		return;
	}


	UINT nLanguageID = nID - IDM_VIEW_LANGUAGE_ENGLISH;
	ASSERT(nLanguageID >=0 && nLanguageID < MAX_SUPPORTTED_LANGUAGES);
	CMenu *pMainMenu = GetMenu();
	ASSERT(pMainMenu);

	CMenu *pViewMenu = pMainMenu->GetSubMenu(3);
	ASSERT(pViewMenu);

	CMenu *pLanguageMenu = pViewMenu->GetSubMenu(7);
	ASSERT(pLanguageMenu);

	UINT state = pLanguageMenu->GetMenuState(nID, MF_BYCOMMAND);
	ASSERT(state != 0xFFFFFFFF);

	if (state & MF_CHECKED)
	{
		return;
	}

	for (int i = 0; i < m_iCountOfLanguages; i ++)
	{
		pLanguageMenu->CheckMenuItem(IDM_VIEW_LANGUAGE_ENGLISH + i, MF_BYCOMMAND | MF_UNCHECKED);	
	}

	pLanguageMenu->CheckMenuItem(nID, MF_BYCOMMAND | MF_CHECKED);

	CAppRegs *pAppRegs = ((CMyGetApp *)AfxGetApp())->GetAppRegs();
	pAppRegs->SetVal(REG_GENERAL_LANGUAGEEX, m_szarLanguageINIs[nLanguageID][1]);

	ReloadResource();
}
