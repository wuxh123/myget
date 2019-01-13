#if !defined(AFX_ADDBATCHDOWNLOADDLG_H__37F74E75_C1E5_4733_831E_473D273F648F__INCLUDED_)
#define AFX_ADDBATCHDOWNLOADDLG_H__37F74E75_C1E5_4733_831E_473D273F648F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddBatchDownloadDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddBatchDownloadDlg dialog

class CAddBatchDownloadDlg : public CDialog
{
// Construction
public:
	void ReLoadResource();
	CAddBatchDownloadDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAddBatchDownloadDlg)
	enum { IDD = IDD_ADD_BATCH_DOWNLOAD };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddBatchDownloadDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddBatchDownloadDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDBATCHDOWNLOADDLG_H__37F74E75_C1E5_4733_831E_473D273F648F__INCLUDED_)
