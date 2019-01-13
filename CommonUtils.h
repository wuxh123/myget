
#ifndef COMMON_UTILS_H_
#define COMMON_UTILS_H_

LPCTSTR GetRscStr(LPCTSTR lpszSection, DWORD dwID);
LPCTSTR GetRscStr(LPCTSTR lpszSection, LPCTSTR lpszKey);

#define APP_GET_RSCSTR(lpszSection, Index) GetRscStr(lpszSection, Index)

#define SET_DLG_ITEM_TEXT(nID) \
do {\
	if (APP_GET_RSCSTR(lpszDlgID, nID) != NULL)\
	{\
		SetDlgItemText(nID, APP_GET_RSCSTR(lpszDlgID, nID)); \
	}\
}while(0);

#define SET_DLG_ITEM_TEXT_EX(nID, lpszSection, nRscID) \
do {\
	if (APP_GET_RSCSTR(lpszSection, nRscID) != NULL)\
	{\
		SetDlgItemText(nID, APP_GET_RSCSTR(lpszSection, nRscID)); \
	}\
}while(0);

#define SET_WINDOW_TITLE \
do{\
	LPCTSTR lpszTitle = APP_GET_RSCSTR(lpszDlgID, "Title");\
	if (lpszTitle != NULL)\
	{\
		SetWindowText(lpszTitle);\
	}\
}while(0);

#define SET_PROPERTYPAGE_TITLE(lpszPageName) \
do {\
	static CString strCaption;\
	strCaption = APP_GET_RSCSTR(lpszPageName, "Title");\
	if (strCaption.GetLength() != 0)\
	{\
		m_psp.dwFlags |= PSP_USETITLE;\
		m_psp.pszTitle = strCaption;\
	}\
}while(0);


class CComposedStrings
{
public:
	CComposedStrings(LPCTSTR lpszComposedStrings = NULL)
	{
		m_lpszComposedStrings = lpszComposedStrings;
	}

	CComposedStrings(DWORD dwStringID)
	{
		SetComposedStrings(dwStringID);
	}


private:
	LPCTSTR m_lpszComposedStrings;
	CString m_strRscString;
public:
	void SetComposedStrings(DWORD dwStringID);
	void SetComposedStrings(LPCTSTR lpszComposedStrings);
	LPCTSTR GetSubString(int iIndex);
	int GetSubStringCount();
};

BOOL Decrypt(BYTE *pbEncryptPSW, LPSTR lpszDecryptPSW);
BOOL Encrypt(BYTE *pbEncryptPSW, LPSTR lpszDecryptPSW);
#endif //COMMON_UTILS_H_