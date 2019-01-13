
#include "stdafx.h"
#include "JCDFile.h"
#include "TreeItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL CJCDFile::prv_LoadFile()
{
	if (m_strFileName.IsEmpty())
	{
		return FALSE;
	}

	CFile fileIn;
	if (fileIn.Open(m_strFileName, CFile::modeRead) == 0)
	{
		return FALSE;
	}

//	m_pTreeCtrl->DeleteAllItems();
	
	int iFileSize = fileIn.GetLength();

	if (m_pbFileBuf)
	{
		delete m_pbFileBuf;
		m_pbFileBuf = NULL;
	}

	m_pbFileBuf = new BYTE[iFileSize];
	memset(m_pbFileBuf, 0, iFileSize);

	fileIn.Read((void *)m_pbFileBuf, iFileSize);

	fileIn.Close();

	CJCDFile *pJCDFile = this;
	m_pbPos = m_pbFileBuf;

	JCD_LOAD_BYTE(m_bVer);

	JCD_LOAD_INT(m_dwItemCount);

	return TRUE;
}




BOOL CJCDFile::ReadFile(void *pData, DWORD dwCount)
{
	memcpy(pData, m_pbPos, dwCount);

	m_pbPos += dwCount;

	return TRUE;
}

BOOL CJCDFile::SaveFile(void *pData, DWORD dwCount)
{
	m_fileWrite.Write(pData, dwCount);

	return TRUE;
}

DWORD CJCDFile::GetItemCount()
{
	return m_dwItemCount;
}

int CJCDFile::GetNextItemClass()
{
	return *(int *)m_pbPos;
}

BOOL CJCDFile::prv_FillFileHeader()
{
	CJCDFile *pJCDFile = this;

	JCD_SAVE_BYTE(m_bVer);
	JCD_SAVE_INT(m_dwItemCount);

	return TRUE;
}
