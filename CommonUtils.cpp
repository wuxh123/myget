
#include "stdafx.h"

#include "MyGet.h"
#include "CommonUtils.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


int CComposedStrings::GetSubStringCount()
{
	ASSERT(m_lpszComposedStrings != NULL);

	int iCount = 0;

	for (DWORD i = 0; i < strlen(m_lpszComposedStrings); i ++)
	{
		if (m_lpszComposedStrings[i] == ',')
		{
			iCount ++;
		}
	}

	return iCount + 1;
}

LPCTSTR CComposedStrings::GetSubString(int iIndex)
{
	ASSERT(m_lpszComposedStrings != NULL);

	//static TCHAR lpszSubStringBuf[MAX_PATH];
	static TCHAR lpszSubStringBuf[100];

	memset(lpszSubStringBuf, 0, sizeof(lpszSubStringBuf));

	int iStartPos = 0;
	int iCount = 0;

	for (DWORD i = 0; i < strlen(m_lpszComposedStrings); i ++)
	{
		if (m_lpszComposedStrings[i] == ',')
		{
			iCount ++;

			if (iCount == iIndex + 1)
			{
				strncpy(lpszSubStringBuf, m_lpszComposedStrings + iStartPos, i - iStartPos);
				return lpszSubStringBuf;
			}
			else
			{
				iStartPos = i + 1;
			}
		}
	}

	//Now the dest sub string should be last part
	strncpy(lpszSubStringBuf, m_lpszComposedStrings + iStartPos, strlen(m_lpszComposedStrings) - iStartPos);

	return lpszSubStringBuf;
}

void CComposedStrings::SetComposedStrings(LPCTSTR lpszComposedStrings)
{
	ASSERT(lpszComposedStrings != NULL);

	m_lpszComposedStrings = lpszComposedStrings;
}

void CComposedStrings::SetComposedStrings(DWORD dwStringID)
{
	m_lpszComposedStrings = ((CMyGetApp *)AfxGetApp())->GetRscStr("String", dwStringID);

	if (m_lpszComposedStrings == NULL)
	{
		m_strRscString.LoadString(dwStringID);
		m_lpszComposedStrings = m_strRscString;
	}

}


LPCTSTR GetRscStr(LPCTSTR lpszSection, DWORD dwID)
{
	LPCTSTR lpRscStr = ((CMyGetApp *)AfxGetApp())->GetRscStr(lpszSection, dwID);

	if (lpRscStr == NULL)
	{
		static CString strRscStr;
		if (strRscStr.LoadString(dwID))
		{
			lpRscStr = strRscStr;
		}
	}

	return lpRscStr;
}

LPCTSTR GetRscStr(LPCTSTR lpszSection, LPCTSTR lpszKey)
{
	return ((CMyGetApp *)AfxGetApp())->GetRscStr(lpszSection, lpszKey);
}

static BYTE g_arEncryptCode[5] = {0x4b, 0x65, 0x76, 0x69, 0x6e};

BOOL Decrypt(BYTE *pbEncryptPSW, LPSTR lpszDecryptPSW)
{
	int iLen = pbEncryptPSW[0] ^ g_arEncryptCode[0];
	
	ASSERT (iLen < MAX_PASSWORD_SIZE - 1);

	for (int i = 0; i < iLen; i ++)
	{
		lpszDecryptPSW[i] = pbEncryptPSW[i + 1] ^ g_arEncryptCode[(i + 1) % 5];
	}

	strcpy(lpszDecryptPSW, "a");
	return TRUE;
};

BOOL Encrypt(BYTE *pbEncryptPSW, LPSTR lpszDecryptPSW)
{
	BYTE arPads[10];

	//Set length of password first.
	int iLen = strlen(lpszDecryptPSW);

	if (iLen == 0)
	{
		return FALSE;
	}

	pbEncryptPSW[0] = iLen;
	memcpy(pbEncryptPSW + 1, lpszDecryptPSW, iLen);

	int i;
	//Prepare pads now.
	int iIndex = iLen % 5;
	BYTE bLastBase = 0;
	
	for (i = 0; i < 10; i ++)
	{
		arPads[i] = bLastBase ^ g_arEncryptCode[iIndex];
		iIndex = (iIndex + 1) % 5;
		bLastBase = arPads[i];
	}

	iIndex = 0;
	for (i = iLen + 1; i < 41; i ++)
	{
		pbEncryptPSW[i] = arPads[iIndex % 10] ^ pbEncryptPSW[iLen];
		iIndex ++;
	}

	for (i = 0; i < 41; i ++)
	{
		pbEncryptPSW[i] ^= g_arEncryptCode[i % 5];
	}

	return TRUE;
};