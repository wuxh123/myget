// CategoryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "myget.h"
#include "CategoryDlg.h"
#include "TreeItem.h"
#include "ListItem.h"
#include "MainFrm.h"
#include "FolderSelectionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCategoryDlg dialog


CCategoryDlg::CCategoryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCategoryDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCategoryDlg)
	m_strCategoryName = _T("");
	m_textTotalFiles = _T("");
	m_textTitle = _T("");
	m_textSubCategoryCount = _T("");
	m_textDownloadedSize = _T("");
	m_strDefaultDir = _T("");
	m_textTotalDownloadedFiles = _T("");
	//}}AFX_DATA_INIT
	m_bIsDefaultDirModifiedByUser = FALSE;
	m_strBaseDefaultDir = _T("");
}


void CCategoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCategoryDlg)
	DDX_Text(pDX, IDC_CATEGORY_NAME_EDIT, m_strCategoryName);
	DDX_Text(pDX, IDC_TOTAL_FILES, m_textTotalFiles);
	DDX_Text(pDX, IDC_TITLE_STATIC, m_textTitle);
	DDX_Text(pDX, IDC_SUB_CATEGORY_COUNT, m_textSubCategoryCount);
	DDX_Text(pDX, IDC_DOWNLOADED_SIZE, m_textDownloadedSize);
	DDX_Text(pDX, IDC_DEFAULT_DIR_EDIT, m_strDefaultDir);
	DDX_Control(pDX, IDC_CATEGORY_TREE, m_treeCategory);
	DDX_Text(pDX, IDC_COUNT_DOWNLOADED_FILES, m_textTotalDownloadedFiles);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCategoryDlg, CDialog)
	//{{AFX_MSG_MAP(CCategoryDlg)
	ON_NOTIFY(TVN_SELCHANGED, IDC_CATEGORY_TREE, OnSelchangedCategoryTree)
	ON_EN_CHANGE(IDC_DEFAULT_DIR_EDIT, OnChangeDefaultDirEdit)
	ON_EN_CHANGE(IDC_CATEGORY_NAME_EDIT, OnChangeCategoryNameEdit)
	ON_BN_CLICKED(IDC_BROWSE_FOLDER_BUTTON, OnBrowseFolderButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCategoryDlg message handlers

BOOL CCategoryDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	ReloadResource();
	prv_SetupDlg();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCategoryDlg::prv_SetupDlg()
{
	switch(m_typeDlg)
	{
	case CATEGORY_NEW_DLG:
		prv_FillTreeDlg();
		break;
	case CATEGORY_MOVETO_DLG:
		prv_FillTreeDlg();
		break;
	case CATEGORY_PROPERTY_DLG:
		prv_FillPropDlg();
		break;
	default:
		return FALSE;
	}

	return TRUE;
}

BOOL CCategoryDlg::prv_FillPropDlg()
{
	//fill title;
//	CString strTitle;
//	strTitle.LoadString(IDS_PROP_CATEGORY_TITLE);
//	SetWindowText(strTitle);


	
	HTREEITEM hItem = m_hCurrentItem;

	CTreeItem *pTreeItem = (CTreeItem *)m_pMainCategoryTree->GetItemData(hItem);

	m_strCategoryName = pTreeItem->GetCategoryName();
	m_strDefaultDir = pTreeItem->GetDefaultDir();

	int iTotalSubCategory = 0, iTotalFiles = 0, 
		iTotalDownloadedFiles = 0, iDownloadedSize = 0;

	GetItemStatistic(hItem, &iTotalSubCategory, &iTotalFiles, 
		&iTotalDownloadedFiles, &iDownloadedSize);

	m_textSubCategoryCount.Format("%d", iTotalSubCategory);
	m_textTotalFiles.Format("%d", iTotalFiles);
	m_textTotalDownloadedFiles.Format("%d", iTotalDownloadedFiles);
	m_textDownloadedSize.Format("%d", iDownloadedSize);


	CRect rcDlg;
	GetWindowRect(&rcDlg);

	CRect rcLastText;
	GetDlgItem(IDC_DOWNLOADED_SIZE)->GetWindowRect(&rcLastText);
	

	MoveWindow(CRect(rcDlg.left, rcDlg.top, rcDlg.right, rcLastText.bottom + 2 * rcLastText.Height()));

	if (pTreeItem->IsSystemItem())
	{
		GetDlgItem(IDC_CATEGORY_NAME_EDIT)->EnableWindow(FALSE);
	}


	m_treeCategory.ShowWindow(SW_HIDE);
	UpdateData(FALSE);

	return TRUE;
}

void CCategoryDlg::GetItemStatistic(HTREEITEM hItem, int *piTotalSubCategory, int *piTotalFiles, int *piTotalDownloadedFiles, int *piDownloadedSize)
{
	//首先统计所有LISTITEM的书据。
	CTreeItem *pTreeItem = (CTreeItem *)m_pMainCategoryTree->GetItemData(hItem);

	CListItem *pListItem;

	for (int i = 0; i < pTreeItem->GetSubListItemCount(); i ++)
	{
		pListItem = pTreeItem->GetSubListItem(i);

		(*piTotalFiles) ++;

		if (pListItem->IsCompleted())
		{
			(piTotalDownloadedFiles) ++;
		}

		(*piDownloadedSize) += pListItem->GetFileDownloadedSize();
	}

	//然后处理所有的子TREEITEM.

	HTREEITEM hChildItem = m_pMainCategoryTree->GetChildItem(hItem);
 
	if (hChildItem == NULL)
	{
		return;
	}

	if (m_pMainCategoryTree->ItemHasChildren(hItem))
	{
		hItem = m_pMainCategoryTree->GetChildItem(hItem);

		while (hItem)
		{
			(*piTotalSubCategory) ++;

			GetItemStatistic(hItem, 
					piTotalSubCategory, 
					piTotalFiles, 
					piTotalDownloadedFiles, 
					piDownloadedSize);

			hItem = m_pMainCategoryTree->GetNextSiblingItem(hItem);
		}
	}

}

void CCategoryDlg::prv_FillTreeDlg()
{
	//填充CATEGORY TREE。
	prv_CopyTree();

/*
	if (m_typeDlg == CATEGORY_MOVETO_DLG)
	{
		GetDlgItem(IDC_CATEGORY_NAME_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_DEFAULT_DIR_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BROWSE_FOLDER_BUTTON)->EnableWindow(FALSE);

		CString strTitle;
		strTitle.LoadString(IDS_MOVE_CATEGORY_TITLE);
		SetWindowText(strTitle);

		strTitle.LoadString(IDS_MOVE_CATEGORY_DESC);
		GetDlgItem(IDC_TITLE_STATIC)->SetWindowText(strTitle);
	}
	else
	{
		CString strTitle;
		strTitle.LoadString(IDS_NEW_CATEGORY_TITLE);
		SetWindowText(strTitle);

		strTitle.LoadString(IDS_NEW_CATEGORY_DESC);
		GetDlgItem(IDC_TITLE_STATIC)->SetWindowText(strTitle);
	
	}
	*/
}

void CCategoryDlg::prv_CopyTree()
{
	m_treeCategory.DeleteAllItems();

	m_treeCategory.SetImageList(m_pMainCategoryTree->GetImageList(TVSIL_NORMAL), TVSIL_NORMAL);

	prv_CopyTreeItem(m_pMainCategoryTree->GetRootItem(), TVI_ROOT);
}

void CCategoryDlg::prv_CopyTreeItem(HTREEITEM hSrcItem, HTREEITEM hDestParentItem)
{
	if (hSrcItem == NULL)
	{
		return;
	}

	//Copy current Tree Item
	{
		TCHAR szText[1024];
		TVITEM tvItem;
		tvItem.hItem = hSrcItem;
		//tvItem.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM | TVIF_STATE;
		tvItem.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
		tvItem.stateMask = TVIS_SELECTED;
		tvItem.pszText = szText;
		tvItem.cchTextMax = 1023;


		if (!m_pMainCategoryTree->GetItem(&tvItem))
		{
			return;
		}
		
		TVINSERTSTRUCT tvIns;
		tvIns.hParent = hDestParentItem;
		tvIns.hInsertAfter = TVI_LAST;
		tvIns.item = tvItem;
		
		hDestParentItem = m_treeCategory.InsertItem(&tvIns);

		if (hSrcItem == m_hCurrentItem)
		{
			m_treeCategory.SelectItem(hDestParentItem);
		}

		ASSERT(hDestParentItem != NULL);
	}

	//Copy All the children items then.
	if (m_pMainCategoryTree->ItemHasChildren(hSrcItem))
	{
		hSrcItem = m_pMainCategoryTree->GetChildItem(hSrcItem);
		while(hSrcItem)
		{
			prv_CopyTreeItem(hSrcItem, hDestParentItem);
			hSrcItem = m_pMainCategoryTree->GetNextSiblingItem(hSrcItem);
		}
	}

	m_treeCategory.Expand(hDestParentItem, TVE_EXPAND);

}

void CCategoryDlg::OnSelchangedCategoryTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	HTREEITEM hItem = m_treeCategory.GetSelectedItem();
	
	ASSERT(hItem != NULL);

	CTreeItem *pTreeItem = (CTreeItem *)m_treeCategory.GetItemData(hItem);
	
	UpdateData(TRUE);

	switch(m_typeDlg)
	{
	case CATEGORY_NEW_DLG:
		if (!m_bIsDefaultDirModifiedByUser)
		{
			m_strBaseDefaultDir = pTreeItem->GetDefaultDir();
			prv_MakeDefaultDir();
		}
		break;
	case CATEGORY_MOVETO_DLG:
		m_strCategoryName = pTreeItem->GetCategoryName();
		m_strDefaultDir = pTreeItem->GetDefaultDir();
		break;

	}
	

	UpdateData(FALSE);
	*pResult = 0;
}

void CCategoryDlg::OnChangeDefaultDirEdit() 
{
	m_bIsDefaultDirModifiedByUser = TRUE;
}

void CCategoryDlg::prv_MakeDefaultDir()
{
	//this function do not need call the 
	//updatedate function pair.

	m_strDefaultDir = m_strBaseDefaultDir;
	
	if (!m_strCategoryName.IsEmpty())
	{
		if (m_strDefaultDir.ReverseFind(TCHAR('\\')) != m_strDefaultDir.GetLength() - 1)
		{
			m_strDefaultDir += CString("\\");
		}
		 
		m_strDefaultDir += m_strCategoryName;
	}
	
}

void CCategoryDlg::OnChangeCategoryNameEdit() 
{
	if (!m_bIsDefaultDirModifiedByUser 
		&& (m_typeDlg != CATEGORY_PROPERTY_DLG))
	{
		UpdateData(TRUE);

		prv_MakeDefaultDir();

		UpdateData(FALSE);
	}
}

void CCategoryDlg::OnOK() 
{
	//如果NAME和DFAULT FOLDER中任何一个为空，不允许退出。
	UpdateData(TRUE);

	if (m_strCategoryName.IsEmpty() 
		|| m_strDefaultDir.IsEmpty())
	{
		return;
	}

	switch(m_typeDlg)
	{
	case CATEGORY_NEW_DLG:
		prv_FinalNewCategory();
		break;

	case CATEGORY_MOVETO_DLG:
		prv_FinalMoveCategory();
		break;

	case CATEGORY_PROPERTY_DLG:
		prv_FinalUpdateCategory();
		break;
	}
	
	CDialog::OnOK();
}

void CCategoryDlg::prv_FinalNewCategory()
{
	//首先获得DIALOG中的TREE中的htreeitem。
	//HTREEITEM hSelectedItem = m_hCurrentItem;
	HTREEITEM hSelectedItem = m_treeCategory.GetSelectedItem();
	
	//然后在根据这个HTREEITEM（想要新建的ITEM的父结点）来找到
	//MAIN CATEGORY TREE中的对应项。
	CTreeItem *pParentItem = (CTreeItem *)m_treeCategory.GetItemData(hSelectedItem);

	HTREEITEM hParentItem = m_pMainCategoryTree->GetHTREEITEMByID(pParentItem->GetID());

	//插入新的节点。
	CTreeItem *pNewTreeItem = new CTreeItem;

	pNewTreeItem->SetID(m_pMainCategoryTree->GetUniqueID());
	//实际上应该是ITEM_CLASS_TREEITEM_WITH_LAST_PROPERTY_ON
	//但是由于不知道他的用途，所以暂时设置成为ITEM_CLASS_TREEITEM

	//pNewTreeItem->SetClass(ITEM_CLASS_TREEITEM);
	pNewTreeItem->SetType(OTHER_ITEMS);
	pNewTreeItem->SetImageIndex(ITEM_OTHER_IMAGE_INDEX);
	pNewTreeItem->SetParentID(pParentItem->GetID());
	//pNewTreeItem->SetEndFlag();
//	pNewTreeItem->SetHeaderUnknown();
	pNewTreeItem->SetCategoryName(m_strCategoryName);
	pNewTreeItem->SetDefaultDir(m_strDefaultDir);
	
	TVITEM tvItem;
	tvItem.mask			= TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	tvItem.pszText		= (LPTSTR)pNewTreeItem->GetCategoryName();
	tvItem.cchTextMax	= strlen(tvItem.pszText);
	tvItem.iImage		= tvItem.iSelectedImage = pNewTreeItem->GetImageIndex();;
	tvItem.lParam		= (LPARAM)pNewTreeItem;

	TVINSERTSTRUCT tvIns;
	tvIns.hParent		= hParentItem;
	tvIns.hInsertAfter	= TVI_LAST;
	tvIns.item			= tvItem;

	HTREEITEM hNewItem = m_pMainCategoryTree->InsertItem(&tvIns);

	m_iNewItemID = pNewTreeItem->GetID();

}

void CCategoryDlg::prv_FinalMoveCategory()
{
	//首先在MAIN CATEGORY TREE中找到需要移动的ITEM。
	HTREEITEM hMovedItem = m_hCurrentItem;
	CTreeItem *pMovedTreeItem = (CTreeItem *)m_pMainCategoryTree->GetItemData(hMovedItem);

	//再把当前选中的ITEM在MAIN CATEGORY TREE中找到对应项。
	HTREEITEM hSelectedItem = m_treeCategory.GetSelectedItem();
	CTreeItem *pTreeItem = (CTreeItem *)m_treeCategory.GetItemData(hSelectedItem);

	HTREEITEM hMoveToItem = m_pMainCategoryTree->GetHTREEITEMByID(pTreeItem->GetID());

	CTreeItem *pMovetoItem = (CTreeItem *)m_pMainCategoryTree->GetItemData(hMoveToItem);

	//判断是否可以进行移动。
	CMainFrame *pMainFrame;
	pMainFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	m_bCanMove = pMainFrame->IsCanMove(hMovedItem, hMoveToItem);

	//移动节点。
	if (m_bCanMove)
	{
		HTREEITEM hNewItem = m_pMainCategoryTree->MoveCategory(hMovedItem, hMoveToItem);

		pMovedTreeItem->SetParentID(pMovetoItem->GetID());

		m_pMainCategoryTree->SelectItem(hNewItem);
	}
	
}

void CCategoryDlg::prv_FinalUpdateCategory()
{
	//比较一下CATEGORY NAME和DEFAULT FOLDER是否被改变。
	//如果改变，找到CATEGORY中的对应项。
	HTREEITEM hChangedItem = m_hCurrentItem;

	CTreeItem *pChangedTreeItem = (CTreeItem *)m_pMainCategoryTree->GetItemData(hChangedItem);
	//更新节点。

	if (strcmp(pChangedTreeItem->GetCategoryName(), m_strCategoryName) != 0)
	{
		pChangedTreeItem->SetCategoryName(m_strCategoryName);
		m_pMainCategoryTree->SetItemText(hChangedItem, m_strCategoryName);
	}

	if (strcmp(pChangedTreeItem->GetDefaultDir(), m_strDefaultDir) != 0)
	{
		pChangedTreeItem->SetDefaultDir(m_strDefaultDir);
	}
}

void CCategoryDlg::OnBrowseFolderButton() 
{
	// TODO: Add your control notification handler code here
	CFolderSelectionDlg dlg;

	UpdateData(TRUE);
	dlg.m_strSelectedFolder = m_strDefaultDir;

	if (dlg.DoModal() == IDOK)
	{
		m_strDefaultDir = dlg.m_strSelectedFolder;
		m_bIsDefaultDirModifiedByUser = TRUE;
		UpdateData(FALSE);
	}

}

void CCategoryDlg::ReloadResource()
{
	//Title;
	switch(m_typeDlg)
	{

	case CATEGORY_MOVETO_DLG:
		{
			GetDlgItem(IDC_CATEGORY_NAME_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_DEFAULT_DIR_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_BROWSE_FOLDER_BUTTON)->EnableWindow(FALSE);
			
			SetWindowText(APP_GET_RSCSTR("String", IDS_MOVE_CATEGORY_TITLE));
			
			//SetDlgItemText(IDC_TITLE_STATIC, APP_GET_RSCSTR("String", IDS_MOVE_CATEGORY_DESC));
			SET_DLG_ITEM_TEXT_EX(IDC_TITLE_STATIC, "String", IDS_MOVE_CATEGORY_DESC);
			
			break;
		}
		
	case CATEGORY_NEW_DLG:
		{
			
			SetWindowText(APP_GET_RSCSTR("String", IDS_NEW_CATEGORY_TITLE));
			
			//SetDlgItemText(IDC_TITLE_STATIC, APP_GET_RSCSTR("String", IDS_NEW_CATEGORY_DESC));
			SET_DLG_ITEM_TEXT_EX(IDC_TITLE_STATIC, "String", IDS_NEW_CATEGORY_DESC);
			break;
		}
		
	case CATEGORY_PROPERTY_DLG:
		{
			SetWindowText(APP_GET_RSCSTR("String", IDS_PROP_TITLE));
			break;
		}

	}

	//common
	//SetDlgItemText(1, APP_GET_RSCSTR("Dialog_Common", 1));
	//SetDlgItemText(2, APP_GET_RSCSTR("Dialog_Common", 2));
	SET_DLG_ITEM_TEXT_EX(1, "Dialog_Common", 1);
	SET_DLG_ITEM_TEXT_EX(2, "Dialog_Common", 2);

	//Dlg Items
	LPCTSTR lpszDlgID = _T("Category_Dlg");
	//Category
	SET_DLG_ITEM_TEXT(1197);
	//Default folder
	SET_DLG_ITEM_TEXT(1198);
	//Sub category
	SET_DLG_ITEM_TEXT(1199);
	//Total file count
	SET_DLG_ITEM_TEXT(1200);
	//Downloaded file count
	SET_DLG_ITEM_TEXT(1201);
	//Downloaded data count
	SET_DLG_ITEM_TEXT(1202);
}
