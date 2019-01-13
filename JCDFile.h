
#ifndef __LOAD_JCD_H_
#define __LOAD_JCD_H_

#define CUR_VER			0x48

#define READ_JCD_FILE	0
#define WRITE_JCD_FILE	1

#define JCD_SAVE_VAL(pVal, wSize) \
do {\
	if (!pJCDFile->SaveFile((pVal), wSize)) \
	{\
		return FALSE;\
	}\
}while(0);

#define JCD_SAVE_BYTE(bVal) JCD_SAVE_VAL(&(bVal), sizeof(BYTE))

#define JCD_SAVE_WORD(wVal) JCD_SAVE_VAL(&(wVal), sizeof(WORD))

#define JCD_SAVE_INT(iVal) JCD_SAVE_VAL(&(iVal), sizeof(int))

#define JCD_SAVE_STR(strVal) \
do {\
	WORD wLength = strVal.GetLength();\
	if (wLength == 0)\
	{\
		JCD_SAVE_BYTE(wLength);\
		break;\
	}\
	if (wLength >= 0xFF) \
	{\
		BYTE bLength = 0xFF;\
		JCD_SAVE_BYTE(bLength); \
		JCD_SAVE_WORD(wLength);\
	}\
	else\
	{\
		JCD_SAVE_BYTE(wLength);\
	}\
	JCD_SAVE_VAL((void *)(LPCTSTR)strVal, wLength);\
}while(0);



#define JCD_LOAD_VAL(pVal, wSize) \
do {\
	if (!pJCDFile->ReadFile((pVal), wSize)) \
	{\
		return FALSE;\
	}\
}while(0);

#define JCD_LOAD_BYTE(bVal) JCD_LOAD_VAL(&(bVal), sizeof(BYTE))

#define JCD_LOAD_WORD(wVal) JCD_LOAD_VAL(&(wVal), sizeof(WORD))

#define JCD_LOAD_INT(iVal) JCD_LOAD_VAL(&(iVal), sizeof(int))


#define JCD_LOAD_STR(strVal) \
do {\
	BYTE bLength;\
	WORD wLength;\
	JCD_LOAD_BYTE(bLength); \
	if (bLength == 0xFF) \
	{\
		JCD_LOAD_WORD(wLength);\
	}\
	else \
	{\
		wLength = bLength; \
	}\
	if (wLength > 0) \
	{ \
		TCHAR szBuf[MAX_PATH]; \
		memset(szBuf, 0, MAX_PATH);\
		JCD_LOAD_VAL(szBuf, wLength);\
		szBuf[wLength] = '\0';\
		strVal = szBuf;\
	}\
}while(0);


typedef struct{
} JCDHEADER, *PJCDHEADER;


class CJCDFile
{
public:
	CJCDFile(LPCTSTR lpszFileName, DWORD dwFlag, DWORD dwItemCount = 0)
		: m_strFileName(lpszFileName), m_dwFlag(dwFlag), m_dwItemCount(dwItemCount)
	{
		m_pbFileBuf = NULL;

		if (m_dwFlag == READ_JCD_FILE)
		{
			prv_LoadFile();
		}
		else
		{
			m_fileWrite.Open(m_strFileName, CFile::modeCreate | CFile::modeWrite);
			m_bVer = CUR_VER;
			prv_FillFileHeader();
		}
	};


	
	~CJCDFile()
	{
		if (m_dwFlag == READ_JCD_FILE)
		{
			if (m_pbFileBuf)
			{
				delete m_pbFileBuf;
				m_pbFileBuf = NULL;
			}
		}
		else
		{
			m_fileWrite.Flush();
			m_fileWrite.Close();
		}
	};

public:
	BOOL	prv_FillFileHeader();
	int		GetNextItemClass();
	DWORD	GetItemCount();
	BOOL	SaveFile(void *pData, DWORD dwCount);
	BOOL	ReadFile(void *pData, DWORD dwCount);
	

private:
	BOOL	prv_LoadFile();
	CString m_strFileName;
	BYTE	*m_pbFileBuf;
	BYTE	*m_pbPos;
	CFile	m_fileWrite;
	DWORD	m_dwFlag;

	BYTE	m_bVer;
	DWORD	m_dwItemCount;

};

#endif //__LOAD_JCD_H_
