#if !defined(AFX_OPTIONSOTHERPAGE_H__C09D37B8_D815_4AFA_9E3D_3C05B4703E9B__INCLUDED_)
#define AFX_OPTIONSOTHERPAGE_H__C09D37B8_D815_4AFA_9E3D_3C05B4703E9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsOtherPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionsOtherPage dialog

#define COMMON_ITEM			0
#define CHECKBOX_ITEM		1
#define RADIOBUTTON_ITEM	2

struct InitOtherTable
{
	int		iDepth;
	int		iInitImage; // for Check Box / Radio button control, it is the default(unselected) image
						// When it is selected, the image index should be decrease by 1.
	LPCTSTR lpszSecton;
	UINT	uiKeyID;
	int		iType;	//0: common, 1: Check Box; 2: Radio Button.
	int		iGroupIndex; // the item index inside a group(just for radio button); it is zero-based.
	int		*piVal;	//The value store the state of check box or a group of radion button.

	LPCTSTR lpszDefaultLabel;
	HTREEITEM	hTvItem;
};

class COptionsOtherPage : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionsOtherPage)

// Construction
public:
	COptionsOtherPage();
	~COptionsOtherPage();

// Dialog Data
	//{{AFX_DATA(COptionsOtherPage)
	enum { IDD = IDD_OPTIONS_OTHER };
	CTreeCtrl	m_tvOthers;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionsOtherPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CImageList m_ilOther;
	BOOL m_bConfirmQuitWhileDownloading;
	BOOL m_bConfirmQuitWhileOpenMultipleWindows;
	BOOL m_bConfirmAddJobUseIEMenu;
	BOOL m_bConfirmAddMultipleJobs;

	BOOL m_bShowSubCategoryInAlphaOrder;
	BOOL m_bHideWindowWhenMinimize;
	BOOL m_bMinimizeWhenX;
	BOOL m_bQuitAfterFinishedDownload;

	int	 m_iDoubleClickDownloadingOrReadyItem;
	int  m_iDoubleClickDownloadedItem;
	int  m_iDoubleClickFailedItem;

	struct InitOtherTable *m_pInitTable;
	int	m_iSizeOfInitTable;
	// Generated message map functions
	//{{AFX_MSG(COptionsOtherPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickTvOthers(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void prv_FillDlg();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSOTHERPAGE_H__C09D37B8_D815_4AFA_9E3D_3C05B4703E9B__INCLUDED_)
