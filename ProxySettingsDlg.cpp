// ProxySettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "myget.h"
#include "ProxySettingsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProxySettingsDlg dialog
static const int iarProxyTypeMatrix[] =
{
	IDC_RIDIO_DIR_CONNECTION,
	IDC_RIDIAO_FTP_OPEN_HOST,
	IDC_RIDIAO_FTP_USER_HOST_PORT1,
	IDC_RIDIAO_HTTP_GET,
	IDC_RIDIAO_FTP_USER_HOST_PORT2,
	IDC_RIDIAO_FTP_SITE_HOST,
	IDC_RIDIAO_FTP_SITE_USER_HOST,
	IDC_RIDIAO_SOCKS5,
	IDC_RIDIAO_HTTP_CONNECT
};

static int iSizeOfMatrix = sizeof(iarProxyTypeMatrix) / sizeof(iarProxyTypeMatrix[0]);

CProxySettingsDlg::CProxySettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProxySettingsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProxySettingsDlg)
	m_iType = 0;
	m_boolNoCache = TRUE;
	m_boolPasvMode = TRUE;
	m_strPassword = _T("");
	m_uiPort = 0;
	m_strServer = _T("");
	m_strTitle = _T("");
	m_strUser = _T("");
	m_boolAuth = FALSE;
	//}}AFX_DATA_INIT
	m_pProxyData = NULL;
	m_bIsDirectConn = FALSE;
}


void CProxySettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProxySettingsDlg)
	DDX_Radio(pDX, IDC_RIDIO_DIR_CONNECTION, m_iType);
	DDX_Check(pDX, IDC_CHECK_NO_CACHE, m_boolNoCache);
	DDX_Check(pDX, IDC_CHECK_PASV_MODE, m_boolPasvMode);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	DDX_Text(pDX, IDC_EDIT_PORT, m_uiPort);
	DDV_MinMaxUInt(pDX, m_uiPort, 0, 65535);
	DDX_Text(pDX, IDC_EDIT_SERVER, m_strServer);
	DDX_Text(pDX, IDC_EDIT_TITILE, m_strTitle);
	DDX_Text(pDX, IDC_EDIT_USER, m_strUser);
	DDX_Check(pDX, IDC_CHECK_AUTH, m_boolAuth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProxySettingsDlg, CDialog)
	//{{AFX_MSG_MAP(CProxySettingsDlg)
	ON_BN_CLICKED(IDC_CHECK_AUTH, OnCheckAuth)
	//ON_COMMAND_RANGE(IDC_RADIO_DIR_CONNECTION, IDC_RIDIAO_SOCKS5, OnTypeClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProxySettingsDlg message handlers

BOOL CProxySettingsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	ReloadResource();
	prv_FillDlg();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProxySettingsDlg::prv_FillDlg()
{
	if (!m_pProxyData)
	{
		m_pProxyData = new CProxyData;
	}

	m_strTitle		= m_pProxyData->GetTitle();
	m_strServer		= m_pProxyData->GetServer();
	m_strUser		= m_pProxyData->GetUser();
	m_strPassword	= m_pProxyData->GetPassword();
	
	m_uiPort		= m_pProxyData->GetPort();
	
	m_boolNoCache	= (m_pProxyData->GetFlag() & NO_CACHE_MASK) ? TRUE : FALSE;
	m_boolPasvMode	= (m_pProxyData->GetFlag() & PASV_MASK) ? TRUE : FALSE;
	m_boolAuth		= (m_pProxyData->GetFlag() & AUTH_MASK) ? TRUE : FALSE;
	
	DWORD dwType = m_pProxyData->GetType();
	ASSERT(dwType >=0 && dwType <= 9);
	
	m_iType = iarProxyTypeMatrix[dwType] - IDC_RIDIO_DIR_CONNECTION;


	UpdateData(FALSE);
	OnTypeClick();

	if (m_bIsDirectConn)
	{
		for (int i = IDC_RIDIO_DIR_CONNECTION; i < IDC_RIDIAO_SOCKS5 + 1; i ++)
		{
			GetDlgItem(i)->EnableWindow(FALSE);
		}
	}
}

void CProxySettingsDlg::OnCheckAuth() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	GetDlgItem(IDC_EDIT_USER)->EnableWindow(m_boolAuth);
	GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow(m_boolAuth);
}

void CProxySettingsDlg::ReloadResource()
{
	//common
	//SetDlgItemText(1, APP_GET_RSCSTR("Dialog_Common", 1));
	//SetDlgItemText(2, APP_GET_RSCSTR("Dialog_Common", 2));
	SET_DLG_ITEM_TEXT_EX(1, "Dialog_Common", 1);
	SET_DLG_ITEM_TEXT_EX(2, "Dialog_Common", 2);

	//Dlg Items
	LPCTSTR lpszDlgID = _T("ProxyInfo_Dlg");
	//SetWindowText(APP_GET_RSCSTR(lpszDlgID, "Title"));
	SET_WINDOW_TITLE;

	SET_DLG_ITEM_TEXT(1197);
	SET_DLG_ITEM_TEXT(1198);
	SET_DLG_ITEM_TEXT(1199);
	SET_DLG_ITEM_TEXT(1200);
	SET_DLG_ITEM_TEXT(1161);
	
	SET_DLG_ITEM_TEXT(1061);
	
	SET_DLG_ITEM_TEXT(1201);
	SET_DLG_ITEM_TEXT(1202);
	
	//SetDlgItemText(1256, APP_GET_RSCSTR(lpszDlgID, 1169));
	//SetDlgItemText(1257, APP_GET_RSCSTR(lpszDlgID, 1170));
	SET_DLG_ITEM_TEXT_EX(1256, lpszDlgID, 1169);
	SET_DLG_ITEM_TEXT_EX(1257, lpszDlgID, 1170);
}

void CProxySettingsDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	if (m_pProxyData)
	{
		m_pProxyData->SetTitle(m_strTitle);
		m_pProxyData->SetServer(m_strServer);
		m_pProxyData->SetUser(m_strUser);
		m_pProxyData->SetPassword(m_strPassword);
		m_pProxyData->SetPort(m_uiPort);
		
		DWORD dwFlag = m_pProxyData->GetFlag();

		if (m_boolNoCache)
		{
			dwFlag |= NO_CACHE_MASK;
		}
		else
		{
			dwFlag &= ~NO_CACHE_MASK;
		}

		if (m_boolPasvMode)
		{
			dwFlag |= PASV_MASK;
		}
		else
		{
			dwFlag &= ~PASV_MASK;
		}

		if (m_boolAuth)
		{
			dwFlag |= AUTH_MASK;
		}
		else
		{
			dwFlag &= ~AUTH_MASK;
		}

		m_pProxyData->SetFlag(dwFlag);

		for (int i = 0; i < iSizeOfMatrix; i ++)
		{
			if (m_iType + IDC_RADIO_DIR_CONNECTION == iarProxyTypeMatrix[i])
			{
				m_pProxyData->SetType(i);
				break;
			}
		}

	}

		
	CDialog::OnOK();
}

void CProxySettingsDlg::OnTypeClick() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	switch(IDC_RIDIO_DIR_CONNECTION + m_iType)
	{
	case IDC_RIDIO_DIR_CONNECTION:
		GetDlgItem(IDC_EDIT_SERVER)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_NO_CACHE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_PASV_MODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_AUTH)->EnableWindow(FALSE);
		break;

	case IDC_RIDIAO_FTP_OPEN_HOST:
	case IDC_RIDIAO_FTP_USER_HOST_PORT1:
	case IDC_RIDIAO_FTP_USER_HOST_PORT2:
	case IDC_RIDIAO_FTP_SITE_HOST:
	case IDC_RIDIAO_FTP_SITE_USER_HOST:
		GetDlgItem(IDC_EDIT_SERVER)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_NO_CACHE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_PASV_MODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_AUTH)->EnableWindow(TRUE);
		break;

	case IDC_RIDIAO_HTTP_GET:
	case IDC_RIDIAO_SOCKS5:
	case IDC_RIDIAO_HTTP_CONNECT:
		GetDlgItem(IDC_EDIT_SERVER)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_NO_CACHE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_PASV_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_AUTH)->EnableWindow(TRUE);
		break;
	}

	UpdateData(FALSE);
	OnCheckAuth();

}
