// AddBatchDownloadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "myget.h"
#include "AddBatchDownloadDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddBatchDownloadDlg dialog


CAddBatchDownloadDlg::CAddBatchDownloadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddBatchDownloadDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddBatchDownloadDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAddBatchDownloadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddBatchDownloadDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddBatchDownloadDlg, CDialog)
	//{{AFX_MSG_MAP(CAddBatchDownloadDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddBatchDownloadDlg message handlers

BOOL CAddBatchDownloadDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	ReLoadResource();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAddBatchDownloadDlg::ReLoadResource()
{
	//common
	//SetDlgItemText(1, APP_GET_RSCSTR("Dialog_Common", 1));
	//SetDlgItemText(2, APP_GET_RSCSTR("Dialog_Common", 2));
	SET_DLG_ITEM_TEXT_EX(1, "Dialog_Common", 1);
	SET_DLG_ITEM_TEXT_EX(2, "Dialog_Common", 2);

	//Dlg Items
	LPCTSTR lpszDlgID = _T("AddBatch_DLG");
	//SetWindowText(APP_GET_RSCSTR(lpszDlgID, "Title"));
	SET_WINDOW_TITLE;
	SET_DLG_ITEM_TEXT(1197);
	SET_DLG_ITEM_TEXT(1198);
	SET_DLG_ITEM_TEXT(1199);
	SET_DLG_ITEM_TEXT(1200);
	SET_DLG_ITEM_TEXT(1201);
	SET_DLG_ITEM_TEXT(1202);

}
