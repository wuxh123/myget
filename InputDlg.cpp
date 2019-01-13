// InputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "myget.h"
#include "InputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputDlg dialog


CInputDlg::CInputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInputDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputDlg)
	m_strInput = _T("");
	//}}AFX_DATA_INIT
}


void CInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputDlg)
	DDX_Text(pDX, IDC_EDIT_INPUT, m_strInput);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputDlg, CDialog)
	//{{AFX_MSG_MAP(CInputDlg)
	ON_EN_CHANGE(IDC_EDIT_INPUT, OnChangeEditInput)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputDlg message handlers

BOOL CInputDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInputDlg::OnChangeEditInput() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	prv_UpdateOKBtnStatus();
}

void CInputDlg::prv_UpdateOKBtnStatus()
{
	BOOL bEnable = FALSE;

	switch(m_iDlgType)
	{
	case ADD_ALTER_URL:
		bEnable = prv_CheckAlterUrl();
		break;

	case RENAME_FILE:
		if (!m_strInput.IsEmpty())
		{
			bEnable = TRUE;
		}

		break;

	}

	GetDlgItem(IDOK)->EnableWindow(bEnable);
}

BOOL CInputDlg::prv_CheckAlterUrl()
{
	//condition:	
	//1. begin with "http://" or "ftp://",
	//2. After the prefix, one more char exist, and it is not "/" or ":"

	CString strUrl = m_strInput;
	int iPos = -1;

	strUrl.MakeLower();
	
	iPos = strUrl.Find("http://");

	//first try to find "http://" or "ftp://"
	if (iPos == -1)
	{
		iPos = strUrl.Find("ftp://");

		if (iPos == -1)
		{
			return FALSE;
		}
		else
		{
			iPos = strlen("ftp://");
		}
	}
	else
	{
		iPos = strlen("http://");
	}

	//If no more text contained in the URL, or the next char is '\'(illegal), just retrun;
	if (iPos == strUrl.GetLength()
		|| strUrl.GetAt(iPos) == TCHAR('/')
		|| strUrl.GetAt(iPos) == TCHAR(':'))
	{
		return FALSE;
	}

	return TRUE;
}
