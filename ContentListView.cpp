// ContentListView.cpp : implementation file
//

#include "stdafx.h"
#include "MyGet.h"
#include "ContentListView.h"
#include "MainFrm.h"
#include "ListItem.h"
#include "DownloadHttpJet.h"
#include "CommonUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContentListView

IMPLEMENT_DYNCREATE(CContentListView, CListView)

CContentListView::CContentListView()
{
	m_iOldParentIndex = -1;
	m_pCurListItem = NULL;
}

CContentListView::~CContentListView()
{
}


BEGIN_MESSAGE_MAP(CContentListView, CListView)
	//{{AFX_MSG_MAP(CContentListView)
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContentListView drawing

void CContentListView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CContentListView diagnostics

#ifdef _DEBUG
void CContentListView::AssertValid() const
{
	CListView::AssertValid();
}

void CContentListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CContentListView message handlers

void CContentListView::RefreshView()
{

	CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;
	ASSERT(pMainFrame != NULL);

	CIndexTreeView *pIndexTreeView = pMainFrame->m_pIndexTreeView;

	HTREEITEM hTreeItem = pIndexTreeView->GetTreeCtrl().GetSelectedItem();
	
	if (!hTreeItem)
	{
		return;
	}

	int iParentIndex;
	iParentIndex = pIndexTreeView->GetTreeCtrl().GetItemData(hTreeItem);

	
	if (m_iOldParentIndex != iParentIndex)
	{
		m_iOldParentIndex = iParentIndex;
		prv_SetupView();
		ReloadResource();
	}


	CItemListView *pItemListView = pMainFrame->m_pItemListView;
	ASSERT(pItemListView != NULL);


	int iFocusedItem = pItemListView->GetFocusedItem();
	CListItem *pListItem;

	if (iFocusedItem == -1)
	{
		pListItem = NULL;
	}
	else
	{
		pListItem = (CListItem *)pItemListView->GetListCtrl().GetItemData(iFocusedItem);
	}

	SetRedraw(FALSE);

	prv_RefreshView(pListItem);

	SetRedraw(TRUE);
}

void CContentListView::prv_SetupView()
{
	CListCtrl &ListCtrl = GetListCtrl();

	int nColumnCount = ListCtrl.GetHeaderCtrl()->GetItemCount();

	// Delete all of the columns.
	for (int i=0; i < nColumnCount; i++)
	{
		GetListCtrl().DeleteColumn(0);
	}

	ListCtrl.DeleteAllItems();

	m_ProgressChart.ShowWindow(SW_HIDE);

	ModifyStyle (LVS_TYPEMASK, LVS_REPORT);


	switch(m_iOldParentIndex)
	{
	case CHART_CONTENT_TYPE: //图表/日志
		//显示图表ProgressChart，并且缺省设置。
		ModifyStyle (LVS_REPORT, 0);
		m_ProgressChart.ShowWindow(SW_SHOW);
		break;
	case INFO_CONTENT_TYPE: //详细信息
		ListCtrl.SetExtendedStyle(ListCtrl.GetExtendedStyle() | LVS_EX_GRIDLINES);
		ListCtrl.InsertColumn(0, "Name", LVCFMT_LEFT, 100);
		ListCtrl.InsertColumn(1, "Value", LVCFMT_LEFT, 600);
		break;
	case LINK_CONTENT_TYPE: //链接
		ListCtrl.SetExtendedStyle(ListCtrl.GetExtendedStyle() & ~LVS_EX_GRIDLINES);
		ListCtrl.InsertColumn(0, "Name", LVCFMT_LEFT, 100);
		ListCtrl.InsertColumn(1, "URL", LVCFMT_LEFT, 600);
		break;
	default: //Jet10-Jet17
		ListCtrl.SetExtendedStyle(ListCtrl.GetExtendedStyle() & ~LVS_EX_GRIDLINES);
		ListCtrl.InsertColumn(0, "Time", LVCFMT_LEFT, 150);
		ListCtrl.InsertColumn(1, "Info", LVCFMT_LEFT, 550);

		break;
	}
}

void CContentListView::prv_RefreshView(CListItem *pListItem)
{
	CListCtrl &ListCtrl = GetListCtrl();

	
	switch(m_iOldParentIndex)
	{
	case 1: //图表/日志
		prv_FillChart(pListItem);
		break;
	case 3: //详细信息
		prv_FillDetailInformation(pListItem);
		break;
	case 4: //链接
		break;
	default: //Jet10-Jet17
		prv_FillJetInfo(pListItem);
		break;
	}
	
}

void CContentListView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();
	
	ModifyStyle (LVS_TYPEMASK, LVS_REPORT);	
	GetListCtrl().SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_ImageList.Create(IDB_CONTENT_ITEM_ICON, 16, 5, CLR_DEFAULT);
	GetListCtrl().SetImageList(&m_ImageList, LVSIL_SMALL);

	CRect rect;
	GetWindowRect(rect);
	ScreenToClient(rect);
	m_ProgressChart.Create(WS_CHILD | WS_VSCROLL | WS_TABSTOP, rect, this, IDC_PROGRESS_CHART);
}

void CContentListView::prv_FillDetailInformation(CListItem *pListItem)
{
	CListCtrl &ListCtrl = GetListCtrl();
//	ListCtrl.SetRedraw(FALSE);

	ListCtrl.DeleteAllItems();

	if (pListItem == NULL)
	{
		return;
	}

	LVITEM lvItem;

	int iIndex = 0;
	lvItem.mask = LVIF_IMAGE | LVIF_TEXT;
	lvItem.iSubItem = 0;
	lvItem.iImage = 4;
	
	//Set URL
	lvItem.iItem = iIndex;
	lvItem.pszText = (char *)"URL";
	lvItem.cchTextMax = strlen(lvItem.pszText);
	ListCtrl.InsertItem(&lvItem);

	ListCtrl.SetItemText(iIndex, 1, pListItem->GetURL());

	//Set Comment
	lvItem.iItem = ++iIndex;
	lvItem.pszText = (char *)"Comment";
	lvItem.cchTextMax = strlen(lvItem.pszText);
	ListCtrl.InsertItem(&lvItem);

	ListCtrl.SetItemText(iIndex, 1, pListItem->GetComment());

	//Set REF
	lvItem.iItem = ++iIndex;
	lvItem.pszText = (char *)"Reference";
	lvItem.cchTextMax = strlen(lvItem.pszText);
	ListCtrl.InsertItem(&lvItem);

	ListCtrl.SetItemText(iIndex, 1, pListItem->GetREF());

	//Set Resume Supported
	lvItem.iItem = ++iIndex;
	lvItem.pszText = (char *)"Resume";
	lvItem.cchTextMax = strlen(lvItem.pszText);
	ListCtrl.InsertItem(&lvItem);

	switch(pListItem->IsSupportResumed())
	{
	case -1:
		ListCtrl.SetItemText(iIndex, 1, "");
		break;
	case 0:
		ListCtrl.SetItemText(iIndex, 1, "Yes");
		break;
	case 1:
		ListCtrl.SetItemText(iIndex, 1, "No");
		break;
	}
	
	//Set Local location.
	lvItem.iItem = ++iIndex;
	lvItem.pszText = (char *)"Local File";
	lvItem.cchTextMax = strlen(lvItem.pszText);
	ListCtrl.InsertItem(&lvItem);

	ListCtrl.SetItemText(iIndex, 1, CString(pListItem->GetDownloadedFolder()) + pListItem->GetRename());

	//Set File original time.
	lvItem.iItem = ++iIndex;
	lvItem.pszText = (char *)"Date";
	lvItem.cchTextMax = strlen(lvItem.pszText);
	ListCtrl.InsertItem(&lvItem);

	CTime timeFile;
	timeFile = pListItem->GetFileTime();
	ListCtrl.SetItemText(iIndex, 1, timeFile.Format("%A, %B %d, %Y %H:%M:%S"));


	//Set File Size;
	lvItem.iItem = ++iIndex;
	lvItem.pszText = (char *)"Size";
	lvItem.cchTextMax = strlen(lvItem.pszText);
	ListCtrl.InsertItem(&lvItem);

	TCHAR szFileSize[21];
	pListItem->GetFileTotalLengthByString(szFileSize, 20);
	ListCtrl.SetItemText(iIndex, 1, szFileSize);
	

	//Set File Completed Size;
	lvItem.iItem = ++iIndex;
	lvItem.pszText = (char *)"Completed Size";
	lvItem.cchTextMax = strlen(lvItem.pszText);
	ListCtrl.InsertItem(&lvItem);

	int iFileDownloadedSize = pListItem->GetFileDownloadedSize();

	if (iFileDownloadedSize > 0)
	{
		CString strDownloadedSize;
		strDownloadedSize.Format("%d", iFileDownloadedSize);
		ListCtrl.SetItemText(iIndex, 1, strDownloadedSize);	
	}

	//Set totoal downloading time cost.
	lvItem.iItem = ++iIndex;
	lvItem.pszText = (char *)"Time costed";
	lvItem.cchTextMax = strlen(lvItem.pszText);
	ListCtrl.InsertItem(&lvItem);

	int iTimeCosted = pListItem->GetDownloadingTimeCosted();

	if (iTimeCosted > 0)
	{
		CString strTimeCosted;
		int iHour = iTimeCosted / 3600;
		int iMin  = iTimeCosted / 60 - iHour * 60;
		int iSec  = iTimeCosted % 60;

		strTimeCosted.Format("%02d:%02d:%02d", iHour, iMin, iSec);

		ListCtrl.SetItemText(iIndex, 1, strTimeCosted);	
	}

	//set average speed.
	lvItem.iItem = ++iIndex;
	lvItem.pszText = (char *)"Average Speed";
	lvItem.cchTextMax = strlen(lvItem.pszText);
	ListCtrl.InsertItem(&lvItem);

	if ((iFileDownloadedSize > 0)
		&& (iTimeCosted > 0))
	{
		CString strAverageSpeed;
		strAverageSpeed.Format("%.2f KB/S", ((float)(iFileDownloadedSize) / iTimeCosted / 1024));

		ListCtrl.SetItemText(iIndex, 1, strAverageSpeed);	
	}

	//Set Local File Create time.
	lvItem.iItem = ++iIndex;
	lvItem.pszText = (char *)"Created at";
	lvItem.cchTextMax = strlen(lvItem.pszText);
	ListCtrl.InsertItem(&lvItem);

	timeFile = pListItem->GetLocalFileCreatedTime();
	ListCtrl.SetItemText(iIndex, 1, timeFile.Format("%A, %B %d, %Y %H:%M:%S"));

	//Set File complete time.
	lvItem.iItem = ++iIndex;
	lvItem.pszText = (char *)"Completed at";
	lvItem.cchTextMax = strlen(lvItem.pszText);
	ListCtrl.InsertItem(&lvItem);

	timeFile = pListItem->GetLocalFileCompletedTime();
	ListCtrl.SetItemText(iIndex, 1, timeFile.Format("%A, %B %d, %Y %H:%M:%S"));

//	ListCtrl.SetRedraw(TRUE);
}

void CContentListView::prv_FillChart(CListItem *pListItem)
{

//	m_ProgressChart.SetRedraw(FALSE);

	if (pListItem == NULL)
	{
		m_pCurListItem = pListItem;

		m_ProgressChart.SetTotalBlocks(0, 0);
		m_ProgressChart.ResetBlocks();

		return;
	}

	int iFileSize = pListItem->GetFileSize();

	int iCountOfBlocks = (iFileSize + BLOCK_BYTE_SIZE - 1) / BLOCK_BYTE_SIZE;

	if (m_pCurListItem != pListItem || iCountOfBlocks != m_ProgressChart.GetTotalBlocks() )
	{
		m_pCurListItem = pListItem;

		m_ProgressChart.SetTotalBlocks(iCountOfBlocks, 0);
		m_ProgressChart.ResetBlocks();
	}


	int iBreaksCount = pListItem->GetCountOfBreaks();

	int iBeginPos, iEndPos;
	for (int i = 0; i < iBreaksCount; i++)
	{
		PBREAKPOSITION pBrkPos = pListItem->GetBreakPos(i);
		iBeginPos = (pListItem->GetBreakPos(i)->iStart + BLOCK_BYTE_SIZE - 1) / BLOCK_BYTE_SIZE;

		ASSERT(pListItem->GetBreakPos(i)->iEnd <= iFileSize);

		if (pListItem->GetBreakPos(i)->iEnd == iFileSize)
		{
			iEndPos = (pListItem->GetBreakPos(i)->iEnd + BLOCK_BYTE_SIZE - 1) / BLOCK_BYTE_SIZE;
		}
		else
		{
			iEndPos = (pListItem->GetBreakPos(i)->iEnd) / BLOCK_BYTE_SIZE;
		}

		for (int j = iBeginPos; j < iEndPos; j++)
		{
			m_ProgressChart.SetBlock(j, BLOCK_COMPLETED);
		}

		if (iEndPos < iCountOfBlocks && m_ProgressChart.GetBlock(iEndPos) != BLOCK_COMPLETED)
		{
			if (pListItem->GetStatus() == ITEM_STATUS_DOWNLOADING)
			{
				m_ProgressChart.SetBlock(iEndPos, BLOCK_DOWNLOADING);
			}
			else
			{
				m_ProgressChart.SetBlock(iEndPos, BLOCK_UNCOMPLETED);
			}
		}
	}

//	m_ProgressChart.SetRedraw(TRUE);
}

void CContentListView::OnSize(UINT nType, int cx, int cy) 
{
	CListView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	m_ProgressChart.Resize();
}




void CContentListView::prv_FillJetInfo(CListItem *pListItem)
{
	CListCtrl &ListCtrl = GetListCtrl();
//	ListCtrl.SetRedraw(FALSE);

//	ListCtrl.DeleteAllItems();

	//Get the index of Jet.
	ASSERT(m_iOldParentIndex > JETLOG_CONTENT_TYPE_BASE && m_iOldParentIndex <= JETLOG_CONTENT_TYPE_BASE + 8);

	if (pListItem == NULL)
	{
		ListCtrl.DeleteAllItems();
		return;
	}

	CString strJetInfo = pListItem->GetJetInfo(m_iOldParentIndex - JETLOG_CONTENT_TYPE_BASE - 1);

	int iBlockPos = strJetInfo.Find('\t');
	int iPos;
	CString strMsgBlock;
	int iStateIndex;
	CTime time;
	CString strTime;

	int iOldLineCount = ListCtrl.GetItemCount();

	int iLineIndex = 0;
	while (iBlockPos != -1)
	{
		int i = strJetInfo.GetLength();
		strMsgBlock = strJetInfo.Left(iBlockPos);
		strJetInfo = strJetInfo.Mid(iBlockPos + 1);

		//first, Get the state index;
		iPos = strMsgBlock.Find(':');
		iStateIndex		= atoi(strMsgBlock.Left(iPos));

		strMsgBlock = strMsgBlock.Mid(iPos + 1);
		//second, get time
		iPos = strMsgBlock.Find(':');
		time			= (time_t)(ULONG)atoi(strMsgBlock.Left(iPos));
		strTime = time.Format("%a %b %d %H:%I:%M %Y");
		strMsgBlock = strMsgBlock.Mid(iPos + 1);

		iPos = strMsgBlock.Find("\r\n");

		CString strLine;

		while(iPos != -1)
		{
			strLine = strMsgBlock.Left(iPos);
			strMsgBlock = strMsgBlock.Mid(iPos + 2);

			if (!strLine.IsEmpty())
			{
				if (iLineIndex >= iOldLineCount)
				{
					LVITEM lvItem;
					lvItem.mask		= TVIF_IMAGE | TVIF_PARAM;
					lvItem.iItem	= iLineIndex;
					lvItem.iSubItem = 0;
					lvItem.iImage	= iStateIndex;
					lvItem.lParam	= iStateIndex;

					ListCtrl.InsertItem(&lvItem);

					ListCtrl.SetItemText(iLineIndex, 0, strTime);
					ListCtrl.SetItemText(iLineIndex, 1, strLine);
				}

				iLineIndex++;
			}

			iPos = strMsgBlock.Find("\r\n");
		}

		iBlockPos = strJetInfo.Find('\t');
	}

//	ListCtrl.SetRedraw(TRUE);

}

void CContentListView::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult) 
{

	if (m_iOldParentIndex <= JETLOG_CONTENT_TYPE_BASE)
	{
		*pResult = 0;

		return;
	}
	// first, lets extract data from
	// the message for ease of use later
	NMLVCUSTOMDRAW* pNMLVCUSTOMDRAW = (NMLVCUSTOMDRAW*)pNMHDR;
	
	// here is the item info
	// note that we don't get the subitem
	// number here, as this may not be
	// valid data except when we are
	// handling a sub item notification
	// so we'll do that separately in
	// the appropriate case statements
	// below.
	int nItem = pNMLVCUSTOMDRAW->nmcd.dwItemSpec;
	LPARAM lParam = pNMLVCUSTOMDRAW->nmcd.lItemlParam;

	//temporily use the hardcode value,
	//later will be replaced by registry.
	if (pNMLVCUSTOMDRAW->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
	{
		switch(lParam)
		{
		case STATE_MSG:
			pNMLVCUSTOMDRAW->clrTextBk = 0x98fb98;
			break;

		case COMMAND_MSG:
			pNMLVCUSTOMDRAW->clrTextBk = 0xeeeeaf;
			break;

		case ERROR_MSG:
			pNMLVCUSTOMDRAW->clrTextBk = 0xcbc0ff;
			break;

		case SERVER_MSG:
			pNMLVCUSTOMDRAW->clrTextBk = 0xfae6e6;
			break;


		}
		
	}

	*pResult = CDRF_NOTIFYITEMDRAW;
}

void CContentListView::ReloadResource()
{
	CComposedStrings ComposedStrings;

	switch(m_iOldParentIndex)
	{
	case INFO_CONTENT_TYPE: //详细信息
		{
			//ComposedStrings.SetComposedStrings(APP_GET_RSCSTR("String", 61302));
			ComposedStrings.SetComposedStrings(61302);

			ASSERT(ComposedStrings.GetSubStringCount() == 2);

			GetListCtrl().InsertColumn(0, ComposedStrings.GetSubString(0), LVCFMT_LEFT, 100);
			GetListCtrl().InsertColumn(1, ComposedStrings.GetSubString(1), LVCFMT_LEFT, 600);
		}
		break;
	case LINK_CONTENT_TYPE: //链接
		{
			//ComposedStrings.SetComposedStrings(APP_GET_RSCSTR("String", 61310));
			ComposedStrings.SetComposedStrings(61310);

			ASSERT(ComposedStrings.GetSubStringCount() == 2);

			GetListCtrl().InsertColumn(0, ComposedStrings.GetSubString(0), LVCFMT_LEFT, 100);
			GetListCtrl().InsertColumn(1, ComposedStrings.GetSubString(1), LVCFMT_LEFT, 600);

		}
		break;
	default: //Jet10-Jet17
		{
			//ComposedStrings.SetComposedStrings(APP_GET_RSCSTR("String", 61301));
			ComposedStrings.SetComposedStrings(61301);

			ASSERT(ComposedStrings.GetSubStringCount() == 2);

			GetListCtrl().InsertColumn(0, ComposedStrings.GetSubString(0), LVCFMT_LEFT, 150);
			GetListCtrl().InsertColumn(1, ComposedStrings.GetSubString(1), LVCFMT_LEFT, 550);
		}
		break;

	}
}
