// OptionsDialUp.cpp : implementation file
//

#include "stdafx.h"
#include "myget.h"
#include "OptionsDialUpPage.h"
#include <Raserror.h>
#include <Ras.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsDialUpPage property page

IMPLEMENT_DYNCREATE(COptionsDialUpPage, CPropertyPage)

COptionsDialUpPage::COptionsDialUpPage() : CPropertyPage(COptionsDialUpPage::IDD)
{
	//{{AFX_DATA_INIT(COptionsDialUpPage)
	m_iConnection = -1;
	m_bHandupWhenDone = FALSE;
	m_bRedialIfFailedConnected = FALSE;
	m_strConnectionWindowTitle = _T("");
	m_strPassword = _T("");
	m_strUserName = _T("");
	m_dwRetryFailedConnection = 0;
	m_dwWaitBetweenRetry = 0;
	//}}AFX_DATA_INIT
	
	//m_psp.dwFlags |= PSP_USETITLE;
	//static CString strCaption;
	//strCaption = APP_GET_RSCSTR("Dial_PPG", "Title");
	//m_psp.pszTitle = strCaption;
	SET_PROPERTYPAGE_TITLE("Dial_PPG");

}

COptionsDialUpPage::~COptionsDialUpPage()
{
}

void COptionsDialUpPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsDialUpPage)
	DDX_Control(pDX, IDC_SPN_WAITBETWEENRETRY, m_spn_WaitBetweenRetry);
	DDX_Control(pDX, IDC_SPN_RETRYFAILEDCONNECTION, m_spnRetryFailedConnection);
	DDX_CBIndex(pDX, IDC_CBX_CONNECTIONS, m_iConnection);
	DDX_Check(pDX, IDC_CHK_HANGUPWHENDONE, m_bHandupWhenDone);
	DDX_Check(pDX, IDC_CHK_REDIALIFDISCONNECTED, m_bRedialIfFailedConnected);
	DDX_Text(pDX, IDC_EDT_CONNECTIONWINDOWTITLE, m_strConnectionWindowTitle);
	DDX_Text(pDX, IDC_EDT_PASSWORD, m_strPassword);
	DDX_Text(pDX, IDC_EDT_USERNAME, m_strUserName);
	DDX_Text(pDX, IDC_EDT_RETRYFAILEDCONNECTION, m_dwRetryFailedConnection);
	DDV_MinMaxDWord(pDX, m_dwRetryFailedConnection, 0, 99);
	DDX_Text(pDX, IDC_EDT_WAITBETWEENRETRY, m_dwWaitBetweenRetry);
	DDV_MinMaxDWord(pDX, m_dwWaitBetweenRetry, 0, 600);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsDialUpPage, CPropertyPage)
	//{{AFX_MSG_MAP(COptionsDialUpPage)
	ON_BN_CLICKED(IDC_CHK_REDIALIFDISCONNECTED, OnChkRedialifdisconnected)
	ON_CBN_SELCHANGE(IDC_CBX_CONNECTIONS, OnSelchangeCbxConnections)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsDialUpPage message handlers

BOOL COptionsDialUpPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	ReloadResource();
	prv_FillDlg();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionsDialUpPage::ReloadResource()
{
	//Dlg Items
	LPCTSTR lpszDlgID = _T("Dial_PPG");

	SET_DLG_ITEM_TEXT(1082);
	SET_DLG_ITEM_TEXT(1197);
	SET_DLG_ITEM_TEXT(1198);
	SET_DLG_ITEM_TEXT(1199);
	SET_DLG_ITEM_TEXT(1200);
	SET_DLG_ITEM_TEXT(1081);
	SET_DLG_ITEM_TEXT(1201);
	SET_DLG_ITEM_TEXT(1202);
	SET_DLG_ITEM_TEXT(1203);
	SET_DLG_ITEM_TEXT(1204);
	SET_DLG_ITEM_TEXT(1205);

}

void COptionsDialUpPage::prv_FillDlg()
{
	
	
	//Get All the RAS Entries.
	DWORD cb, cEntries;
	int nRet;

	CComboBox *pCBXEntries = (CComboBox *)GetDlgItem(IDC_CBX_CONNECTIONS);
	pCBXEntries->AddString(APP_GET_RSCSTR("String", 61274));
	
	cb = 0;
	LPRASENTRYNAME lpEntry;
	LPRASENTRYNAME lpRasEntryName = (LPRASENTRYNAME)GlobalAlloc(GPTR, sizeof(RASENTRYNAME));
	lpRasEntryName->dwSize = sizeof(RASENTRYNAME);
	if ((nRet = RasEnumEntries(NULL, NULL, lpRasEntryName, &cb, &cEntries)) 
		== ERROR_BUFFER_TOO_SMALL)
	{
		GlobalFree(lpRasEntryName);
		lpRasEntryName = lpEntry = (LPRASENTRYNAME)GlobalAlloc(GPTR, cb);
		lpRasEntryName->dwSize = sizeof(RASENTRYNAME);
		
		// Calling RasEnumEntries to enumerate the phone-book entries    
		nRet = RasEnumEntries(NULL, NULL, lpRasEntryName, &cb, &cEntries);
		
		if (nRet == ERROR_SUCCESS)
		{
			for(DWORD i=0;i < cEntries;i++)
			{
				pCBXEntries->AddString(lpEntry->szEntryName);
				lpEntry++;
			}
		}

		GlobalFree(lpRasEntryName);
	}
	
	CAppRegs *pAppRegs = ((CMyGetApp *)AfxGetApp())->GetAppRegs();
	LPTSTR lpszVal;

	//Hand up when done
	m_bHandupWhenDone = pAppRegs->IsFlagOn(REG_FLAG_DIALUP_HANDUPWHENDONE);

	//Redial if disconnected.
	m_bRedialIfFailedConnected = pAppRegs->IsFlagOn(REG_FLAG_DIALUP_REDIALIFDISCONNECTED);



	//Dial Entry
	pAppRegs->GetVal(REG_DIALUP_ENTRY, &lpszVal);

	CString strEntry;
	m_iConnection = 0; 

	//这儿是针对FlashGet的一段修正代码。实现了完全匹配，而不是部分匹配。
	//请注意：如果使用CComboBox的Find函数，就会出现仅仅匹配开始部分字串的情况。
	for (int i = 0; i < pCBXEntries->GetCount(); i ++)
	{
		pCBXEntries->GetLBText(i, strEntry);

		if (strEntry.Compare(lpszVal) == 0)
		{
			m_iConnection = i;
			break;
		}
	}
	
	//UserName
	pAppRegs->GetVal(REG_DIALUP_USERNAME, &lpszVal);
	m_strUserName = lpszVal;

	//Password
	DWORD dwPSWLen = 0;
	pAppRegs->GetVal(REG_DIALUP_PASSWORD, (BYTE **)&lpszVal, &dwPSWLen);
	if (dwPSWLen > 0)
	{
		m_strPassword = lpszVal;
		m_strPassword = m_strPassword.Left(dwPSWLen);
	}

	UDACCEL udAccel;
	//Retry
	pAppRegs->GetVal(REG_DIALUP_RETRY, &m_dwRetryFailedConnection);
	
	m_spnRetryFailedConnection.GetAccel(1, &udAccel);
	udAccel.nInc = 1;
	m_spnRetryFailedConnection.SetAccel(1, &udAccel);
	m_spnRetryFailedConnection.SetRange(0, 99);

	//Wait
	pAppRegs->GetVal(REG_DIALUP_WT, &m_dwWaitBetweenRetry);
	m_spn_WaitBetweenRetry.GetAccel(1, &udAccel);
	udAccel.nInc = 1;
	m_spn_WaitBetweenRetry.SetAccel(1, &udAccel);
	m_spn_WaitBetweenRetry.SetRange(1, 600);


	//Title
	pAppRegs->GetVal(REG_DIALUP_TITLE, &lpszVal);
	m_strConnectionWindowTitle = lpszVal; 
	
	prv_UpdateDialEntry();
	prv_UpdateRedial();
	UpdateData(FALSE);
}

void COptionsDialUpPage::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);

	CAppRegs *pAppRegs = ((CMyGetApp *)AfxGetApp())->GetAppRegs();
	pAppRegs->SetFlag(REG_FLAG_DIALUP_HANDUPWHENDONE, m_bHandupWhenDone);
	pAppRegs->SetFlag(REG_FLAG_DIALUP_REDIALIFDISCONNECTED, m_bRedialIfFailedConnected);

	CString strEntry;
	CComboBox *pCBXEntries = (CComboBox *)GetDlgItem(IDC_CBX_CONNECTIONS);

	pCBXEntries->GetWindowText(strEntry);
	pAppRegs->SetVal(REG_DIALUP_ENTRY, strEntry);

	pAppRegs->SetVal(REG_DIALUP_USERNAME, m_strUserName);
	pAppRegs->SetVal(REG_DIALUP_PASSWORD, (BYTE *)(LPCTSTR)m_strPassword, m_strPassword.GetLength());

	pAppRegs->SetVal(REG_DIALUP_RETRY, m_dwRetryFailedConnection);
	pAppRegs->SetVal(REG_DIALUP_WT, m_dwWaitBetweenRetry);
	pAppRegs->SetVal(REG_DIALUP_TITLE, m_strConnectionWindowTitle);


	CPropertyPage::OnOK();
}

void COptionsDialUpPage::OnChkRedialifdisconnected() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	prv_UpdateRedial();
}

void COptionsDialUpPage::prv_UpdateRedial()
{
	GetDlgItem(IDC_EDT_RETRYFAILEDCONNECTION)->EnableWindow(m_bRedialIfFailedConnected);
	GetDlgItem(IDC_SPN_RETRYFAILEDCONNECTION)->EnableWindow(m_bRedialIfFailedConnected);
	GetDlgItem(IDC_EDT_WAITBETWEENRETRY)->EnableWindow(m_bRedialIfFailedConnected);
	GetDlgItem(IDC_SPN_WAITBETWEENRETRY)->EnableWindow(m_bRedialIfFailedConnected);
}

void COptionsDialUpPage::OnSelchangeCbxConnections() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	prv_UpdateDialEntry();
}

void COptionsDialUpPage::prv_UpdateDialEntry()
{
	GetDlgItem(IDC_EDT_USERNAME)->EnableWindow(m_iConnection);
	GetDlgItem(IDC_EDT_PASSWORD)->EnableWindow(m_iConnection);
}
