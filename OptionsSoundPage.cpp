// OptionsSoundPage.cpp : implementation file
//

#include "stdafx.h"
#include "myget.h"
#include "OptionsSoundPage.h"
#include <Mmsystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsSoundPage property page

IMPLEMENT_DYNCREATE(COptionsSoundPage, CPropertyPage)

COptionsSoundPage::COptionsSoundPage() : CPropertyPage(COptionsSoundPage::IDD)
{
	//{{AFX_DATA_INIT(COptionsSoundPage)
	m_bOpenDlgWhenDownloadAll = FALSE;
	m_bOpenDlgWhenDownloadFailed = FALSE;
	m_bOpenDlgWhenDownloadSucceed = FALSE;
	m_bEnableSound = FALSE;
	//}}AFX_DATA_INIT
	//_psp.dwFlags |= PSP_USETITLE;
	//static CString strCaption;
	//strCaption = APP_GET_RSCSTR("Snd_PPG", "Title");
	//m_psp.pszTitle = strCaption;
	SET_PROPERTYPAGE_TITLE("Snd_PPG");

}

COptionsSoundPage::~COptionsSoundPage()
{
}

void COptionsSoundPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsSoundPage)
	DDX_Control(pDX, IDC_LV_SOUNDS, m_lvSounds);
	DDX_Check(pDX, IDC_CHK_DOWNLOADALL, m_bOpenDlgWhenDownloadAll);
	DDX_Check(pDX, IDC_CHK_DOWNLOADFAILED, m_bOpenDlgWhenDownloadFailed);
	DDX_Check(pDX, IDC_CHK_DOWNLOADSUCCEED, m_bOpenDlgWhenDownloadSucceed);
	DDX_Check(pDX, IDC_CHK_ENABLESOUND, m_bEnableSound);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsSoundPage, CPropertyPage)
	//{{AFX_MSG_MAP(COptionsSoundPage)
	ON_BN_CLICKED(IDC_BTN_CLEARSOUND, OnBtnClearsound)
	ON_BN_CLICKED(IDC_BTN_PLAYSOUND, OnBtnPlaysound)
	ON_BN_CLICKED(IDC_BTN_BROWSESOUND, OnBtnBrowsesound)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsSoundPage message handlers

BOOL COptionsSoundPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	ReloadResource();
	prv_FillDlg();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptionsSoundPage::prv_FillDlg()
{
	CAppRegs *pAppRegs = ((CMyGetApp *)AfxGetApp())->GetAppRegs();

	//Open Dialog When Succeed
	m_bOpenDlgWhenDownloadSucceed = pAppRegs->IsFlagOn(REG_FLAG_GENERAL_OPENDIALOGWHENDOWNLOADSUCCEED);

	//Open Dialog when Failed
	m_bOpenDlgWhenDownloadFailed = pAppRegs->IsFlagOn(REG_FLAG_GENERAL_OPENDIALOGWHENDOWNLOADFAILED);

	//Open Dialog when all done
	m_bOpenDlgWhenDownloadAll = pAppRegs->IsFlagOn(REG_FLAG_GENERAL_OPENDIALOGWHENDOWNLOADALL);

	//Enable Sound
	m_bEnableSound = pAppRegs->IsFlagOn(REG_FLAG_GENERAL_ENABLESOUND);



	LPTSTR lpszVal;
	for (int i = 0; i < 4; i ++)
	{
		pAppRegs->GetVal(REG_SOUND_SOUND0 + i, &lpszVal);
		m_lvSounds.SetItemText(i, 1, lpszVal);
	}

	m_lvSounds.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	UpdateData(FALSE);
}

void COptionsSoundPage::OnBtnClearsound() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_lvSounds.GetNextItem(-1, LVNI_SELECTED);

	if (iItem > -1)
	{
		m_lvSounds.SetItemText(iItem, 1, "");
	}
}

void COptionsSoundPage::OnBtnPlaysound() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_lvSounds.GetNextItem(-1, LVNI_SELECTED);

	if (iItem > -1)
	{
		//Play Sound File.
		PlaySound(m_lvSounds.GetItemText(iItem, 1), NULL, SND_FILENAME | SND_ASYNC);
	}

}

void COptionsSoundPage::OnBtnBrowsesound() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_lvSounds.GetNextItem(-1, LVNI_SELECTED);

	if (iItem == -1)
	{
		return;
	}

	CFileDialog dlgFile(
	TRUE,
	_T(".exe"),
	m_lvSounds.GetItemText(iItem, 1),
	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
	_T("Sound Files (*.WAV)|*.wav" ));

	if (dlgFile.DoModal() == IDOK)
	{
		m_lvSounds.SetItemText(iItem, 1, dlgFile.GetPathName());
	}

}

void COptionsSoundPage::ReloadResource()
{
	//Dlg Items
	LPCTSTR lpszDlgID = _T("Snd_PPG");
	int i = 0;

	SET_DLG_ITEM_TEXT(1095);
	SET_DLG_ITEM_TEXT(1197);
	SET_DLG_ITEM_TEXT(1094);
	SET_DLG_ITEM_TEXT(1096);
	SET_DLG_ITEM_TEXT(1003);
	SET_DLG_ITEM_TEXT(1219);

	CComposedStrings strHeaderTitles(61278);

	m_lvSounds.InsertColumn(0, strHeaderTitles.GetSubString(0), LVCFMT_LEFT, 100);
	m_lvSounds.InsertColumn(1, strHeaderTitles.GetSubString(1), LVCFMT_LEFT, 600);

	CComposedStrings strSoundItems(61279);

	for (int i = 0; i < 4; i ++)
	{
		m_lvSounds.InsertItem(i, strSoundItems.GetSubString(i));
	}

	for (i = 1; i < 4; i ++)
	{
		SetDlgItemText(IDC_CHK_DOWNLOADSUCCEED + i - 1, strSoundItems.GetSubString(i));
	}
	
}

void COptionsSoundPage::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);

	CAppRegs *pAppRegs = ((CMyGetApp *)AfxGetApp())->GetAppRegs();

	//Open Dialog When Succeed
	pAppRegs->SetFlag(REG_FLAG_GENERAL_OPENDIALOGWHENDOWNLOADSUCCEED, m_bOpenDlgWhenDownloadSucceed);

	//Open Dialog when Failed
	pAppRegs->SetFlag(REG_FLAG_GENERAL_OPENDIALOGWHENDOWNLOADFAILED, m_bOpenDlgWhenDownloadFailed);

	//Open Dialog when all done
	pAppRegs->SetFlag(REG_FLAG_GENERAL_OPENDIALOGWHENDOWNLOADALL, m_bOpenDlgWhenDownloadAll);

	//Enable Sound
	pAppRegs->SetFlag(REG_FLAG_GENERAL_ENABLESOUND, m_bEnableSound);

	CString strSoundFile;

	for (int i = 0; i < 4; i ++)
	{
		strSoundFile = m_lvSounds.GetItemText(i, 1);
		pAppRegs->SetVal(REG_SOUND_SOUND0 + i, strSoundFile);
	}

	CPropertyPage::OnOK();
}
