// OptionsProxyPage.cpp : implementation file
//

#include "stdafx.h"
#include "myget.h"
#include "OptionsProxyPage.h"
#include "AppRegs.h"
#include "ProxyData.h"
#include "ProxySettingsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsProxyPage property page


IMPLEMENT_DYNCREATE(COptionsProxyPage, CPropertyPage)

COptionsProxyPage::COptionsProxyPage() : CPropertyPage(COptionsProxyPage::IDD)
{
	//{{AFX_DATA_INIT(COptionsProxyPage)
	m_boolAllJobsUseDefaultProxy = FALSE;
	m_iRollBackSize = 0;
	//}}AFX_DATA_INIT
	//m_psp.dwFlags |= PSP_USETITLE;
	//static CString strCaption;
	//strCaption = APP_GET_RSCSTR("Proxy_PPG", "Title");
	//m_psp.pszTitle = strCaption;
	SET_PROPERTYPAGE_TITLE("Proxy_PPG");

}

COptionsProxyPage::~COptionsProxyPage()
{
}

void COptionsProxyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsProxyPage)
	DDX_Control(pDX, IDC_LIST_PROXY, m_listctrlProxy);
	DDX_Check(pDX, IDC_CHK_ALL_JOBS_USE_DEFAULT_PROXY, m_boolAllJobsUseDefaultProxy);
	DDX_Text(pDX, IDC_EDT_ROLL_BACK_SIZE, m_iRollBackSize);
	DDV_MinMaxInt(pDX, m_iRollBackSize, 1024, 8192);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsProxyPage, CPropertyPage)
	//{{AFX_MSG_MAP(COptionsProxyPage)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PROXY, OnItemchangedListProxy)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PROXY, OnClickListProxy)
	ON_BN_CLICKED(IDC_BTN_EDIT_PROXY, OnBtnEditProxy)
	ON_BN_CLICKED(IDC_BTN_ADD_PROXY, OnBtnAddProxy)
	ON_BN_CLICKED(IDC_BTN_DELETE, OnBtnDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsProxyPage message handlers

BOOL COptionsProxyPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	ReloadResource();
	prv_SetupListColumns();
	prv_FillProxyList();


	//CRegProcess RegProcess;
	CAppRegs *pAppRegs = ((CMyGetApp *)AfxGetApp())->GetAppRegs();

	pAppRegs->GetVal(REG_PROXY_ROLLBACK, (DWORD *)&m_iRollBackSize);

	m_boolAllJobsUseDefaultProxy = pAppRegs->IsFlagOn(REG_FLAG_GENERAL_ALL_JOBS_USE_DEFAULT_PROXY);

	pAppRegs->GetVal(REG_PROXY_DEFAULTFORFTP, (DWORD *)&m_iDefaultForFtp);
	pAppRegs->GetVal(REG_PROXY_DEFAULTFORHTTP, (DWORD *)&m_iDefaultForHttp);

	prv_RefreshDefaultIndex();
	UpdateData(FALSE);
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionsProxyPage::prv_FillProxyList()
{
	//CRegProcess RegProcess;
	CAppRegs *pAppRegs = ((CMyGetApp *)AfxGetApp())->GetAppRegs();

	CProxyData *pProxyData;
	DWORD dwProxyCount;
	pAppRegs->GetVal(REG_PROXY_COUNT, &dwProxyCount);

	for (DWORD dwIndex = 0; dwIndex < dwProxyCount; dwIndex ++)
	{
		pProxyData = new CProxyData;
		pAppRegs->GetProxyData(pProxyData->GetProxyStruct(), dwIndex);

		LVITEM lvItem;
		lvItem.iItem = dwIndex;

		lvItem.iSubItem = 0;
		lvItem.mask =  LVIF_IMAGE | LVIF_PARAM;
		lvItem.iImage = 1;
		lvItem.lParam = (ULONG)pProxyData;
		m_listctrlProxy.InsertItem(&lvItem);

		lvItem.mask =  LVIF_IMAGE;
		lvItem.iSubItem = 1;
		lvItem.iImage = 1;
		m_listctrlProxy.SetItem(&lvItem);

		lvItem.iSubItem = 2;
		lvItem.mask =  LVIF_TEXT;

		if (dwIndex == 0)
		{
			LPCTSTR lpszTitle = APP_GET_RSCSTR("String", 61307);
			
			if (lpszTitle != NULL)
			{
				pProxyData->SetTitle(lpszTitle);
			}
		}

		lvItem.pszText = pProxyData->GetTitle();
		lvItem.cchTextMax = strlen(lvItem.pszText);

		m_listctrlProxy.SetItem(&lvItem);


	}

	m_listctrlProxy.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
}



void COptionsProxyPage::prv_RefreshDefaultIndex()
{
	m_listctrlProxy.SetRedraw(FALSE);

	for (int i = 0; i < m_listctrlProxy.GetItemCount(); i ++)
	{
		LVITEM lvItem;
		lvItem.iItem = i;
		lvItem.mask = LVIF_IMAGE;

		lvItem.iSubItem = 0;
		
		m_listctrlProxy.GetItem(&lvItem);

		if (m_iDefaultForHttp == i)
		{			
			lvItem.iImage = 0;	
		}
		else
		{
			lvItem.iImage = 1;
		}

		m_listctrlProxy.SetItem(&lvItem);

		lvItem.iSubItem = 1;
		
		m_listctrlProxy.GetItem(&lvItem);

		if (m_iDefaultForFtp == i)
		{
			lvItem.iImage = 0;	
		}
		else
		{
			lvItem.iImage = 1;
		}

		m_listctrlProxy.SetItem(&lvItem);
	}

	m_listctrlProxy.SetRedraw(TRUE);
}

void COptionsProxyPage::prv_SetupListColumns()
{
	m_ilChoiceType.Create(IDB_CHOICE_TYPE_BMP, 16, 1, CLR_DEFAULT);
	m_listctrlProxy.SetImageList(&m_ilChoiceType, LVSIL_SMALL);


	m_listctrlProxy.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);
	m_listctrlProxy.InsertColumn(0, _T("Http Default"), LVCFMT_LEFT, 100);
	m_listctrlProxy.InsertColumn(1, _T("Ftp Default"), LVCFMT_LEFT, 100);
	m_listctrlProxy.InsertColumn(2, _T("Name"), LVCFMT_LEFT, 200);

}

void COptionsProxyPage::prv_FreeMEM()
{
	for (int i = 0; i < m_listctrlProxy.GetItemCount(); i ++)
	{
		CProxyData *pProxyData = (CProxyData *)m_listctrlProxy.GetItemData(i);

		delete pProxyData;
	}
}

void COptionsProxyPage::OnCancel() 
{
	// TODO: Add your specialized code here and/or call the base class
	prv_FreeMEM();
	
	CPropertyPage::OnCancel();
}

void COptionsProxyPage::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);

	CAppRegs *pAppRegs = ((CMyGetApp *)AfxGetApp())->GetAppRegs();

	pAppRegs->ClearProxyData();

	CProxyData *pProxyData;

	DWORD dwProxyCount = m_listctrlProxy.GetItemCount();
	for (DWORD i = 0; i < dwProxyCount; i ++)
	{
		pProxyData = (CProxyData *)m_listctrlProxy.GetItemData(i);
		pAppRegs->SetProxyData(pProxyData->GetProxyStruct(), i);

		LVITEM lvItem;
		lvItem.iItem = i;
		lvItem.mask = LVIF_IMAGE;

		lvItem.iSubItem = 0;
		
		m_listctrlProxy.GetItem(&lvItem);

		if (lvItem.iImage == 0)
		{
			m_iDefaultForHttp = i;
		}

		lvItem.iSubItem = 1;
		
		m_listctrlProxy.GetItem(&lvItem);

		if (lvItem.iImage == 0)
		{
			m_iDefaultForFtp = i;
		}

	}

	pAppRegs->SetVal(REG_PROXY_ROLLBACK, (DWORD)m_iRollBackSize);
	pAppRegs->SetFlag(REG_FLAG_GENERAL_ALL_JOBS_USE_DEFAULT_PROXY, m_boolAllJobsUseDefaultProxy);

	pAppRegs->SetVal(REG_PROXY_COUNT, dwProxyCount);

	pAppRegs->SetVal(REG_PROXY_DEFAULTFORFTP, (DWORD)m_iDefaultForFtp);
	pAppRegs->SetVal(REG_PROXY_DEFAULTFORHTTP, (DWORD)m_iDefaultForHttp);

	prv_FreeMEM();
	CPropertyPage::OnOK();
}

void COptionsProxyPage::OnItemchangedListProxy(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if (pNMListView->uNewState != 0)
	{
		GetDlgItem(IDC_BTN_DELETE)->EnableWindow(pNMListView->iItem != 0);
	}

	*pResult = 0;
}

void COptionsProxyPage::OnClickListProxy(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CPoint pt;
	GetCursorPos(&pt);

	m_listctrlProxy.ScreenToClient(&pt);

	LVHITTESTINFO lvHitTestInfo;
	lvHitTestInfo.flags = 0;
	lvHitTestInfo.pt = pt;

	if ((m_listctrlProxy.SubItemHitTest(&lvHitTestInfo) != -1)
		&& (lvHitTestInfo.flags == LVHT_ONITEMICON))
	{
		if (lvHitTestInfo.iSubItem == 0)
		{
			//Here a check is needed, because The Ftp Proxy Server can not be able to
			//be the proxy of HTTP, though the Http server can proxy ftp.
			//Is it unfair? Who care? I don't care!

			CProxyData *pProxyData = (CProxyData *)m_listctrlProxy.GetItemData(lvHitTestInfo.iItem);
			switch (pProxyData->GetType())
			{
			case REG_FTP_OPEN_HOST:
			case REG_FTP_USER_HOST_PORT1:
			case REG_FTP_USER_HOST_PORT2:
			case REG_FTP_SITE_HOST:	
			case REG_FTP_SITE_USER_HOST:
				return;
			}

			m_iDefaultForHttp = lvHitTestInfo.iItem;
		}
		else if (lvHitTestInfo.iSubItem == 1)
		{
			m_iDefaultForFtp = lvHitTestInfo.iItem;
		}

		prv_RefreshDefaultIndex();
	}

	
	*pResult = 0;
}

void COptionsProxyPage::OnBtnEditProxy() 
{
	// TODO: Add your control notification handler code here
	CProxySettingsDlg dlg;
	//CProxyData *pProxyData;

	int iItem = m_listctrlProxy.GetNextItem(-1, LVNI_SELECTED);

	if (iItem == -1)
	{
		return;
	}

	dlg.m_pProxyData = (CProxyData *)m_listctrlProxy.GetItemData(iItem);

	if (iItem == 0)
	{
		dlg.m_bIsDirectConn = TRUE;
	}

	if (dlg.DoModal() == IDOK)
	{
		m_listctrlProxy.SetItemText(iItem, 2, dlg.m_pProxyData->GetTitle());
	}
}

void COptionsProxyPage::ReloadResource()
{
	//Dlg Items
	LPCTSTR lpszDlgID = _T("Proxy_PPG");
	SET_DLG_ITEM_TEXT(1155);
	SET_DLG_ITEM_TEXT(1156);
	SET_DLG_ITEM_TEXT(1157);
	SET_DLG_ITEM_TEXT(1197);
	SET_DLG_ITEM_TEXT(1198);
	SET_DLG_ITEM_TEXT(1210);
	

}

void COptionsProxyPage::OnBtnAddProxy() 
{
	// TODO: Add your control notification handler code here
	CProxySettingsDlg dlg;

	if (dlg.DoModal() == IDOK)
	{
		CProxyData *pProxyData = dlg.m_pProxyData;

		int iItem = m_listctrlProxy.InsertItem(m_listctrlProxy.GetItemCount(), "", 1);
		m_listctrlProxy.SetItemData(iItem, (DWORD)pProxyData);

		LVITEM lvItem;
		lvItem.iItem = iItem;
		lvItem.iSubItem = 1;
		lvItem.mask = LVIF_IMAGE;
		lvItem.iImage = 1;

		m_listctrlProxy.SetItem(&lvItem);

		lvItem.iSubItem = 2;
		lvItem.mask =  LVIF_TEXT;
		lvItem.pszText = pProxyData->GetTitle();
		lvItem.cchTextMax = strlen(lvItem.pszText);

		m_listctrlProxy.SetItem(&lvItem);

		m_listctrlProxy.SetItemState(iItem, LVNI_SELECTED, LVNI_SELECTED);
	}
	else
	{
		delete dlg.m_pProxyData;
	}
}

void COptionsProxyPage::OnBtnDelete() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_listctrlProxy.GetNextItem(-1, LVNI_SELECTED);

	if (iItem >0)
	{
		LVITEM lvItem;
		lvItem.mask = LVIF_IMAGE;
		lvItem.iItem = iItem;
		lvItem.iSubItem = 0;
		
		m_listctrlProxy.GetItem(&lvItem);

		//If current item is default for HTTP, then set the first one "Http default".
		if (lvItem.iImage == 0)
		{
			lvItem.iItem = 0;
			m_listctrlProxy.SetItem(&lvItem);
		}

		lvItem.iItem = iItem;
		lvItem.iSubItem = 1;

		m_listctrlProxy.GetItem(&lvItem);

		//If current item is default for FTP, then set the first one "Ftp default".
		if (lvItem.iImage == 0)
		{
			lvItem.iItem = 0;
			m_listctrlProxy.SetItem(&lvItem);
		}

		CProxyData *pProxyData = (CProxyData *)m_listctrlProxy.GetItemData(iItem);
		delete pProxyData;

		m_listctrlProxy.DeleteItem(iItem);
	}
}
