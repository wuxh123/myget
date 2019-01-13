// SetupAlternativeURLsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "myget.h"
#include "SetupAlternativeURLsDlg.h"
#include "ListItem.h"
#include "InputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetupAlternativeURLsDlg dialog


CSetupAlternativeURLsDlg::CSetupAlternativeURLsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetupAlternativeURLsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetupAlternativeURLsDlg)
	m_strUrl = _T("");
	//}}AFX_DATA_INIT
}


void CSetupAlternativeURLsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetupAlternativeURLsDlg)
	DDX_Control(pDX, IDC_LIST_ALTERNATE_URLS, m_lstAlternativeUrls);
	DDX_Text(pDX, IDC_TEXT_URL, m_strUrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetupAlternativeURLsDlg, CDialog)
	//{{AFX_MSG_MAP(CSetupAlternativeURLsDlg)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_DELETE, OnBtnDelete)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetupAlternativeURLsDlg message handlers

BOOL CSetupAlternativeURLsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	ReloadResource();
	prv_SetupDlg();
	prv_FillDlg();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetupAlternativeURLsDlg::prv_FillDlg()
{
	for (int i = 0; i < m_parSubStitutedUrl->GetSize(); i ++)
	{
		m_lstAlternativeUrls.InsertItem(i, m_parSubStitutedUrl->GetAt(i)->m_strSubstitutedURL);
	}

	UpdateData(FALSE);
}

void CSetupAlternativeURLsDlg::prv_SetupDlg()
{
	m_lstAlternativeUrls.InsertColumn(0, "", LVCFMT_LEFT, 1000);
}



void CSetupAlternativeURLsDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	prv_SaveURLs();

	CDialog::OnClose();
}

void CSetupAlternativeURLsDlg::OnOK() 
{
	// TODO: Add extra validation here
	prv_SaveURLs();
	CDialog::OnOK();
}

void CSetupAlternativeURLsDlg::prv_SaveURLs()
{
	int i;

	PSUBSTITUTEDURL pSubsUrl;
	for (i = 0; i < m_parSubStitutedUrl->GetSize(); i ++)
	{
		pSubsUrl = m_parSubStitutedUrl->GetAt(i);

		delete pSubsUrl;
	}

	m_parSubStitutedUrl->RemoveAll();

	for (i = 0; i < m_lstAlternativeUrls.GetItemCount(); i ++)
	{
		pSubsUrl = new SUBSTITUTEDURL;

		pSubsUrl->m_strSubstitutedURL	= m_lstAlternativeUrls.GetItemText(i, 0);
		*(int *)pSubsUrl->pbUnknown		= 0;

		m_parSubStitutedUrl->Add(pSubsUrl);

	}
	
}

void CSetupAlternativeURLsDlg::OnBtnDelete() 
{
	// TODO: Add your control notification handler code here
	int iSelectIndex = m_lstAlternativeUrls.GetNextItem(-1, LVNI_SELECTED);

	if (iSelectIndex == -1)
	{
		return;
	}

	m_lstAlternativeUrls.DeleteItem(iSelectIndex);
}

void CSetupAlternativeURLsDlg::OnBtnAdd() 
{
	// TODO: Add your control notification handler code here
	CInputDlg dlg;
	dlg.m_iDlgType = ADD_ALTER_URL;

	if (dlg.DoModal() == IDOK)
	{
		m_lstAlternativeUrls.InsertItem(m_lstAlternativeUrls.GetItemCount(), dlg.m_strInput);
	}
}

void CSetupAlternativeURLsDlg::ReloadResource()
{
	//common
	//SetDlgItemText(1, APP_GET_RSCSTR("Dialog_Common", 1));
	SET_DLG_ITEM_TEXT_EX(1, "Dialog_Common", 1);
	SET_DLG_ITEM_TEXT_EX(2, "Dialog_Common", 2);

	//Dlg Items
	LPCTSTR lpszDlgID = _T("AltUrls_Dlg");
	//SetWindowText(APP_GET_RSCSTR(lpszDlgID, "Title"));
	SET_WINDOW_TITLE;

	SET_DLG_ITEM_TEXT(1155);
	SET_DLG_ITEM_TEXT(1157);

}
