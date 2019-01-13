#if !defined(AFX_OPTIONSFILEMANAGEMENTPAGE_H__9818C242_6E83_4555_BED7_EC364968BA92__INCLUDED_)
#define AFX_OPTIONSFILEMANAGEMENTPAGE_H__9818C242_6E83_4555_BED7_EC364968BA92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsFileManagementPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionsFileManagementPage dialog

class COptionsFileManagementPage : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionsFileManagementPage)

// Construction
public:
	void ReloadResource();
	COptionsFileManagementPage();
	~COptionsFileManagementPage();

// Dialog Data
	//{{AFX_DATA(COptionsFileManagementPage)
	enum { IDD = IDD_OPTIONS_FILE_MANAGEMENT };
	int		m_iStoragesCBX;
	BOOL	m_bMoveSomeFilesToPortableStorage;
	BOOL	m_bOpenFilesWhenDownloaded;
	BOOL	m_bVirusScanDownloadedFiles;
	CString	m_strOpenTypes;
	CString	m_strVirsuScanner;
	CString	m_strVirsuScannerArgs;
	CString	m_strVirsuScannerTypes;
	int		m_iDestinationExists;
	int		m_iMoveDownloadedItem;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionsFileManagementPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionsFileManagementPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnBrowsevirusscanner();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void prv_FillDlg();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSFILEMANAGEMENTPAGE_H__9818C242_6E83_4555_BED7_EC364968BA92__INCLUDED_)
