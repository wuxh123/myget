#if !defined(AFX_OPTIONSPROXYPAGE_H__00EEBD51_BAE0_4558_B4AA_B30A18A08467__INCLUDED_)
#define AFX_OPTIONSPROXYPAGE_H__00EEBD51_BAE0_4558_B4AA_B30A18A08467__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsProxyPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionsProxyPage dialog

class COptionsProxyPage : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionsProxyPage)

// Construction
public:
	void ReloadResource();
	COptionsProxyPage();
	~COptionsProxyPage();

// Dialog Data
	//{{AFX_DATA(COptionsProxyPage)
	enum { IDD = IDD_OPTIONS_PROXY };
	CListCtrl	m_listctrlProxy;
	BOOL	m_boolAllJobsUseDefaultProxy;
	int		m_iRollBackSize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionsProxyPage)
	public:
	virtual void OnCancel();
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_iDefaultForHttp;
	int m_iDefaultForFtp;
	CImageList m_ilChoiceType;
	// Generated message map functions
	//{{AFX_MSG(COptionsProxyPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedListProxy(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListProxy(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnEditProxy();
	afx_msg void OnBtnAddProxy();
	afx_msg void OnBtnDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void prv_FreeMEM();
	void prv_SetupListColumns();
	void prv_RefreshDefaultIndex();
	void prv_FillProxyList();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSPROXYPAGE_H__00EEBD51_BAE0_4558_B4AA_B30A18A08467__INCLUDED_)
