// DownloadPropertyPage.cpp : implementation file
//

#include "stdafx.h"
#include "myget.h"
#include "DownloadPropertyPage.h"
#include "DownloadDlg.h"
#include "AppRegs.h"
#include "MainFrm.h"
#include "TreeItem.h"
#include "ProxyData.h"
#include "CategoryDlg.h"
#include "FolderSelectionDlg.h"
#include "ListItem.h"
#include "SetupAlternativeURLsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDownloadPropertyPage property page

IMPLEMENT_DYNCREATE(CDownloadPropertyPage, CPropertyPage)

CDownloadPropertyPage::CDownloadPropertyPage() : CPropertyPage(CDownloadPropertyPage::IDD)
{
	//{{AFX_DATA_INIT(CDownloadPropertyPage)
	m_iStart = 0;
	m_strUrl = _T("");
	m_boolEnableMirrorAndFtpSearch = FALSE;
	m_strRefer = _T("");
	m_strPath = _T("");
	m_iSplit = 0;
	m_boolLogin = FALSE;
	m_strPassword = _T("");
	m_strUserName = _T("");
	m_strRename = _T("");
	m_strComment = _T("");
	//}}AFX_DATA_INIT

	m_pListItem = NULL;
	m_boolNeedFreeMEM = TRUE;

	//m_psp.dwFlags |= PSP_USETITLE;
	//m_psp.pszTitle = APP_GET_RSCSTR("Download_Dlg", "Title");
	SET_PROPERTYPAGE_TITLE("Download_Dlg");
}

CDownloadPropertyPage::~CDownloadPropertyPage()
{
	prv_FreeMEM();
}

void CDownloadPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDownloadPropertyPage)
	DDX_Control(pDX, IDC_CBX_HTTP_PROXY, m_cbxHttpProxy);
	DDX_Control(pDX, IDC_CBX_FTP_PROXY, m_cbxFtpProxy);
	DDX_Radio(pDX, IDC_RADIO_MANUAL, m_iStart);
	DDX_Control(pDX, IDC_SPIN_SPLIT, m_spinSplit);
	DDX_Control(pDX, IDC_CBX_CATEGORY, m_cbxCategory);
	DDX_Text(pDX, IDC_EDIT_URL, m_strUrl);
	DDX_Check(pDX, IDC_CHECK_ENABLE_MIRROR_AND_FTP_SEARCH, m_boolEnableMirrorAndFtpSearch);
	DDX_Text(pDX, IDC_EDIT_REFERER, m_strRefer);
	DDX_Text(pDX, IDC_EDIT_PATH, m_strPath);
	DDX_Text(pDX, IDC_EDIT_SPLIT, m_iSplit);
	DDV_MinMaxInt(pDX, m_iSplit, 1, 10);
	DDX_Check(pDX, IDC_CHECK_LOGIN, m_boolLogin);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_RENAME, m_strRename);
	DDX_Text(pDX, IDC_EDIT_COMMENT, m_strComment);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDownloadPropertyPage, CPropertyPage)
	//{{AFX_MSG_MAP(CDownloadPropertyPage)
	ON_CBN_SELCHANGE(IDC_CBX_CATEGORY, OnSelchangeCbxCategory)
	ON_BN_CLICKED(IDC_CHECK_LOGIN, OnCheckLogin)
	ON_BN_CLICKED(IDC_BTN_CREATE_NEW_CATEGORY, OnBtnCreateNewCategory)
	ON_BN_CLICKED(IDC_BTN_SELECT_NEW_FOLDER, OnBtnSelectNewFolder)
	ON_EN_CHANGE(IDC_EDIT_URL, OnChangeEditUrl)
	ON_BN_CLICKED(IDC_BTN_SET_SUBSTITUTED_URLS, OnBtnSetSubstitutedUrls)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDownloadPropertyPage message handlers

BOOL CDownloadPropertyPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	prv_SetupDlg();
	
	prv_FillDlg();

	ReloadResource();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDownloadPropertyPage::prv_SetupDlg()
{
	if (m_iDlgType == TYPE_DEFAULT_DOWNLOAD_PROPERTY)
	{
		GetDlgItem(IDC_TEXT_URL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_URL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_SET_SUBSTITUTED_URLS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TEXT_RENAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RENAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_SAVE_AS_DEFAULT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TEXT_COMMENT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_COMMENT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TEXT_NOTICE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TEXT_NOTICE)->ShowWindow(SW_SHOW);
	}

}

LPSTR CDownloadPropertyPage::prv_GetClipText()
{
#define EXIT {goto EXIT_ENTRY;};

	HANDLE hClipData;
	LPSTR lpClipData;
	LPSTR lpszText = NULL;

	if (OpenClipboard())
	{
		if (!(hClipData = GetClipboardData(CF_TEXT)))
		{
			EXIT;
		}

		if (!(lpClipData = (LPSTR)GlobalLock(hClipData)))
		{
			EXIT;
		}

		if (!(lpszText = new TCHAR[GlobalSize(hClipData)]))
		{
			EXIT;
		}

		strcpy(lpszText, lpClipData);
	}

EXIT_ENTRY:
	CloseClipboard();
	GlobalUnlock(hClipData);
	return lpszText;
}

void CDownloadPropertyPage::prv_FillDlg()
{
	if (m_iDlgType != TYPE_DOWNLOAD_ITEM_PROPERTY)
	{
		prv_PrepareDefaultListItem();
	}
	
	prv_FillDlgByItemProperty();
	
	UpdateData(FALSE);

}


void CDownloadPropertyPage::prv_FillCategoryCBX()
{
	m_cbxCategory.ResetContent();

	CCategoryTree *pCategoryTree = ((CMainFrame *)AfxGetMainWnd())->m_pMainCategoryTree;
	
	m_cbxCategory.SetImageList(pCategoryTree->GetImageList(TVSIL_NORMAL));

	int iIndex = 0;
	prv_FillCategoryTreeItem(pCategoryTree, pCategoryTree->GetRootItem(), 0, &iIndex);
}

void CDownloadPropertyPage::prv_FillCategoryTreeItem(CCategoryTree *pCategoryTree, HTREEITEM hItem, int iIndent, int *piIndex)
{
	TVITEM tvItem;
	
	TCHAR szText[MAX_PATH];

	tvItem.mask			= TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE | TVIF_TEXT;
	tvItem.hItem		= hItem;
	tvItem.pszText		= szText;
	tvItem.cchTextMax	= MAX_PATH;

	pCategoryTree->GetItem(&tvItem);


	
	COMBOBOXEXITEM cbxItem;

	cbxItem.mask			= CBEIF_IMAGE | CBEIF_INDENT | CBEIF_LPARAM | CBEIF_SELECTEDIMAGE | CBEIF_TEXT;
	cbxItem.pszText			= szText;
	cbxItem.cchTextMax		= tvItem.cchTextMax;
	cbxItem.iItem			= *piIndex;
	cbxItem.iImage			= tvItem.iImage;
	cbxItem.iSelectedImage	= tvItem.iSelectedImage;
	cbxItem.iIndent			= iIndent;
	cbxItem.lParam			= tvItem.lParam;

	m_cbxCategory.InsertItem(&cbxItem);

	(*piIndex)++;

	if (pCategoryTree->ItemHasChildren(hItem))
	{
		hItem = pCategoryTree->GetChildItem(hItem);

		while (hItem)
		{
			prv_FillCategoryTreeItem(pCategoryTree, hItem, iIndent+1, piIndex);

			hItem = pCategoryTree->GetNextSiblingItem(hItem);
		}
	}


}

void CDownloadPropertyPage::prv_SelectCategoryByID(int iID)
{
	for (int i = 0; i < m_cbxCategory.GetCount(); i++)
	{
		CTreeItem *pTreeItem = (CTreeItem *)m_cbxCategory.GetItemData(i);

		if (pTreeItem->GetID() == iID)
		{
			m_cbxCategory.SetCurSel(i);

			return;
		}
	}
}

void CDownloadPropertyPage::OnSelchangeCbxCategory() 
{
	// TODO: Add your control notification handler code here
	prv_UpdatePathByCategoryCBX();
	UpdateData(FALSE);
}

void CDownloadPropertyPage::prv_UpdatePathByCategoryCBX()
{
	int iCurSel = m_cbxCategory.GetCurSel();

	CTreeItem *pTreeItem = (CTreeItem *)m_cbxCategory.GetItemData(iCurSel);

	m_strPath = pTreeItem->GetDefaultDir();

}


void CDownloadPropertyPage::prv_UpdateUserAndPasswordByLoginCHK()
{
	GetDlgItem(IDC_EDIT_USERNAME)->EnableWindow(m_boolLogin);
	GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow(m_boolLogin);
}

void CDownloadPropertyPage::OnCheckLogin() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	prv_UpdateUserAndPasswordByLoginCHK();
}

void CDownloadPropertyPage::prv_FillProxyCBXs()
{
//	CRegProcess RegProcess;
	CAppRegs *pAppRegs = ((CMyGetApp *)AfxGetApp())->GetAppRegs();

	CProxyData *pProxyData;
	int iItemIndex;
	DWORD dwProxyCount;
	pAppRegs->GetVal(REG_PROXY_COUNT, &dwProxyCount);

	for (DWORD dwIndex = 0; dwIndex < dwProxyCount; dwIndex++)
	{
		pProxyData = new CProxyData;
		pAppRegs->GetProxyData(pProxyData->GetProxyStruct(), dwIndex);
	
		iItemIndex = m_cbxFtpProxy.AddString(pProxyData->GetTitle());
		m_cbxFtpProxy.SetItemData(iItemIndex, dwIndex);

		if (pProxyData->GetType() == REG_DIR_CONNECTION
			|| pProxyData->GetType() == REG_HTTP_GET
			|| pProxyData->GetType() == REG_DIR_HTTP_CONNECT
			|| pProxyData->GetType() == REG_DIR_SOCKS5)
		{
			iItemIndex = m_cbxHttpProxy.AddString(pProxyData->GetTitle());
			m_cbxHttpProxy.SetItemData(iItemIndex, dwIndex);
		}

		delete pProxyData;
		pProxyData = NULL;	
	}
}


void CDownloadPropertyPage::prv_SelectProxyCBXs(int iDefaultFtpProxyIndex, int iDefaultHttpProxyIndex)
{
	int i = 0;

	for (int i = 0; i < m_cbxFtpProxy.GetCount(); i ++)
	{
		if (m_cbxFtpProxy.GetItemData(i) == (DWORD)iDefaultFtpProxyIndex)
		{
			m_cbxFtpProxy.SetCurSel(i);

			break;
		}
	}

	for (i = 0; i < m_cbxHttpProxy.GetCount(); i ++)
	{
		if (m_cbxHttpProxy.GetItemData(i) == (DWORD)iDefaultHttpProxyIndex)
		{
			m_cbxHttpProxy.SetCurSel(i);

			break;
		}
	}

}

void CDownloadPropertyPage::OnBtnCreateNewCategory() 
{
	CCategoryTree *pCategoryTree = ((CMainFrame *)AfxGetMainWnd())->m_pMainCategoryTree;
	CCategoryDlg dlgCategory;
	dlgCategory.m_typeDlg			= CATEGORY_NEW_DLG;
	dlgCategory.m_pMainCategoryTree = pCategoryTree;

	int iIndex = m_cbxCategory.GetCurSel();

	if (iIndex > -1)
	{
		CTreeItem *pTreeItem = (CTreeItem *)m_cbxCategory.GetItemData(iIndex);

		dlgCategory.m_hCurrentItem		= pCategoryTree->GetHTREEITEMByID(pTreeItem->GetID());
	}
	else
	{
		dlgCategory.m_hCurrentItem = NULL;
	}

	
	if (dlgCategory.DoModal() == IDOK)
	{
		prv_FillCategoryCBX();
		prv_SelectCategoryByID(dlgCategory.m_iNewItemID);
		prv_UpdatePathByCategoryCBX();
	}	
}

void CDownloadPropertyPage::OnBtnSelectNewFolder() 
{
	// TODO: Add your control notification handler code here
	CFolderSelectionDlg dlg;

	UpdateData(TRUE);
	dlg.m_strSelectedFolder = m_strPath;

	if (dlg.DoModal() == IDOK)
	{
		m_strPath = dlg.m_strSelectedFolder;
		UpdateData(FALSE);
	}

}

void CDownloadPropertyPage::OnChangeEditUrl() 
{
	UpdateData(TRUE);

	m_strRename = CListItem::GetRenameByURL(m_strUrl);

	UpdateData(FALSE);
}




void CDownloadPropertyPage::prv_PrepareDefaultListItem()
{
	
	if (m_pListItem)
	{
		delete m_pListItem;

		m_pListItem = NULL;
	}

	//Note: After New, the m_pListItem own the default properties already.
	m_pListItem = new CListItem;

	
	//Fill the URL Edit box
	LPSTR lpszText = prv_GetClipText();

	if (lpszText)
	{
		m_pListItem->SetUrl(lpszText);

		delete lpszText;
		lpszText = NULL;
	}

	m_arSubStitutedUrl.RemoveAll();

}

void CDownloadPropertyPage::prv_FillDlgByItemProperty()
{
	if (m_iDlgType == TYPE_DOWNLOAD_ITEM_PROPERTY)
	{
		CMainFrame *pMainFrame = (CMainFrame *)AfxGetApp()->m_pMainWnd;

		CListCtrl &ListCtrl = pMainFrame->m_pItemListView->GetListCtrl();

		int iFocusedItem = ListCtrl.GetNextItem(-1, LVNI_FOCUSED);

		if (iFocusedItem == -1)
		{
			return;
		}

		if (m_pListItem)
		{
			delete m_pListItem;

			m_pListItem = NULL;
		}

		m_pListItem = (CListItem *)ListCtrl.GetItemData(iFocusedItem);

		//Prevent from delete this pointer while desctory.
		m_boolNeedFreeMEM = FALSE;

		//copy the substituedURS from the selected ListItem
		PSUBSTITUTEDURL pSubsUrl;
		for (int i = 0; i < m_pListItem->GetCountOfSubstitutedURLs(); i ++)
		{
			pSubsUrl = new SUBSTITUTEDURL;

			pSubsUrl->m_strSubstitutedURL	= m_pListItem->GetSubstitutedURLs(i);
			*(int *)pSubsUrl->pbUnknown		= 0;

			m_arSubStitutedUrl.Add(pSubsUrl);
		}

	}

	//Fill URL
	m_strUrl = m_pListItem->GetURL();
	
	//Fill Rename 
	m_strRename = m_pListItem->GetRename();

	//Fill "Mirror and Ftp seach"
	m_boolEnableMirrorAndFtpSearch = !m_pListItem->IsAttrOn(ATTR_NOTUSEMIRROR);

	//Fill Referer.
	m_strRefer = m_pListItem->GetREF();

	//Fill Category comboxex
	prv_FillCategoryCBX();
	prv_SelectCategoryByID(m_pListItem->GetDownloadedToCategory());

	//Fill Downloaded Path	
	m_strPath = m_pListItem->GetDownloadedFolder();

	//Fill split;
	m_iSplit = m_pListItem->GetSplitsCount();
	m_spinSplit.SetRange(1, 10);

	//Fill Login
	m_boolLogin = m_pListItem->IsAttrOn(ATTR_NEEDLOGIN);
	prv_UpdateUserAndPasswordByLoginCHK();

	//Fill UserName
	m_strUserName = m_pListItem->GetUserName();

	//Fill Password
	m_strPassword = m_pListItem->GetPasswd();

	//Fill Start
	m_iStart = m_pListItem->GetStartState();

	//Fill Http and Ftp Proxy CBX
	prv_FillProxyCBXs();
	prv_SelectProxyCBXs(m_pListItem->GetFtpProxyIndex(), 
						m_pListItem->GetHttpProxyIndex());

	//Fill Comment
	m_strComment = m_pListItem->GetComment();

}

void CDownloadPropertyPage::OnBtnSetSubstitutedUrls() 
{
	// TODO: Add your control notification handler code here
	CSetupAlternativeURLsDlg dlg;

	dlg.m_strUrl			= m_strUrl;
	dlg.m_parSubStitutedUrl = &m_arSubStitutedUrl;
	dlg.DoModal();
}

void CDownloadPropertyPage::prv_FreeMEM()
{
	if (m_pListItem && m_boolNeedFreeMEM)
	{
		delete m_pListItem;

		m_pListItem = NULL;
	}

	PSUBSTITUTEDURL pSubsUrl;

	for (int i = 0; i < m_arSubStitutedUrl.GetSize(); i ++)
	{
		pSubsUrl = m_arSubStitutedUrl[i];

		delete pSubsUrl;
	}

}

void CDownloadPropertyPage::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);

	/*
	if (!prv_CheckSetting())
	{
		return;
	}
*/
	prv_UpdateListItemSetting();

	prv_SaveItemList();

	CPropertyPage::OnOK();
}

void CDownloadPropertyPage::prv_UpdateListItemSetting()
{
	//URL
	m_pListItem->SetUrl(m_strUrl);

	//Mirro And Ftp Search
	m_pListItem->SetAttr(ATTR_NOTUSEMIRROR, !m_boolEnableMirrorAndFtpSearch);

	//REF
	m_pListItem->SetREF(m_strRefer);

	//Category
	int iIndex = m_cbxCategory.GetCurSel();

	if (iIndex != -1)
	{
		CTreeItem *pTreeItem = (CTreeItem *)m_cbxCategory.GetItemData(iIndex);

		m_pListItem->SetDownloadedToCategory(pTreeItem->GetID());
	}

	//Path
	m_pListItem->SetDownloadedFolder(m_strPath);

	//Rename
	LPCTSTR lpszRename = CListItem::GetRenameByURL(m_strUrl);
	if (lpszRename 
		&& strlen(lpszRename) > 0
		&& m_strRename.Compare(lpszRename))
	{
		m_pListItem->SetRename(m_strRename);
	}
	else
	{
		m_pListItem->SetRename("");
	}

	//splits
	m_pListItem->SetSplitsCount(m_iSplit);

	//Login in
	m_pListItem->SetAttr(ATTR_NEEDLOGIN, m_boolLogin);

	//UserName
	m_pListItem->SetUserName(m_strUserName);

	//Password
	m_pListItem->SetPassword(m_strPassword);

	//Start State
	m_pListItem->SetStartState(m_iStart);

	//Ftp proxy.
	iIndex = m_cbxFtpProxy.GetCurSel();
	m_pListItem->SetFtpProxyIndex((WORD)m_cbxFtpProxy.GetItemData(iIndex));

	//Http PRoxy
	iIndex = m_cbxHttpProxy.GetCurSel();
	m_pListItem->SetHttpProxyIndex((WORD)m_cbxHttpProxy.GetItemData(iIndex));

	//Comment
	m_pListItem->SetComment(m_strComment);

	//Subs URLs
	m_pListItem->ClearSubstitutedURLs();

	for (int i = 0; i < m_arSubStitutedUrl.GetSize(); i ++)
	{
		m_pListItem->AddSubstitutedURL(m_arSubStitutedUrl[i]->m_strSubstitutedURL);
	}

}


void CDownloadPropertyPage::prv_SaveItemList()
{
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();
	CCategoryTree *pCateGoryTree = pMainFrame->m_pMainCategoryTree;
	CItemListView *pItemListView = pMainFrame->m_pItemListView;

	switch(m_iDlgType)
	{
	case TYPE_NEW_DOWNLOAD:
		m_boolNeedFreeMEM = FALSE;		
		pCateGoryTree->AddNewDownloadItem(m_pListItem);	

		break;

	case TYPE_DOWNLOAD_ITEM_PROPERTY:
		break;

	case TYPE_DEFAULT_DOWNLOAD_PROPERTY:
		break;
	}
	
	pItemListView->RefreshItem(m_pListItem);

}

void CDownloadPropertyPage::ReloadResource()
{
	//common
	//SetDlgItemText(1, APP_GET_RSCSTR("Dialog_Common", 1));
	//SetDlgItemText(2, APP_GET_RSCSTR("Dialog_Common", 2));
	SET_DLG_ITEM_TEXT_EX(1, "Dialog_Common", 1);
	SET_DLG_ITEM_TEXT_EX(2, "Dialog_Common", 2);

	//Dlg Items
	LPCTSTR lpszDlgID = _T("Download_Dlg");

	SET_DLG_ITEM_TEXT(1197);
	SET_DLG_ITEM_TEXT(1198);
	SET_DLG_ITEM_TEXT(1199);
	SET_DLG_ITEM_TEXT(1200);
	SET_DLG_ITEM_TEXT(1201);
	SET_DLG_ITEM_TEXT(1202);


	SET_DLG_ITEM_TEXT(1009);
	SET_DLG_ITEM_TEXT(1203);
	SET_DLG_ITEM_TEXT(1206);
	SET_DLG_ITEM_TEXT(1207);
	SET_DLG_ITEM_TEXT(1208);
	SET_DLG_ITEM_TEXT(1209);

	SET_DLG_ITEM_TEXT(1132);
	SET_DLG_ITEM_TEXT(1133);
	SET_DLG_ITEM_TEXT(1134);
	SET_DLG_ITEM_TEXT(1177);
	SET_DLG_ITEM_TEXT(1180);
	SET_DLG_ITEM_TEXT(1249);


}

