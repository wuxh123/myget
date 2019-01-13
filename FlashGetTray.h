// FlashGetTray.h: interface for the CFlashGetTray class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLASHGETTRAY_H__C3CBF9CA_B1D7_41E2_B5C5_420E82765755__INCLUDED_)
#define AFX_FLASHGETTRAY_H__C3CBF9CA_B1D7_41E2_B5C5_420E82765755__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFlashGetTray
{
public:
	CFlashGetTray(CWnd *pParentWnd, UINT uID, UINT uFlags, UINT uCallbackMessage, LPCTSTR lpszTip);
	virtual ~CFlashGetTray();

protected:
	CImageList m_ilTray;
	NOTIFYICONDATA m_NotifyIconData;
};

#endif // !defined(AFX_FLASHGETTRAY_H__C3CBF9CA_B1D7_41E2_B5C5_420E82765755__INCLUDED_)
