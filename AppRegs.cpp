// RegColection.cpp: implementation of the CAppRegs class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AppRegs.h"
#include <atlbase.h>
#include "CommonUtils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CAppRegs Class
//////////////////////////////////////////////////////////////////////

CAppRegs::CAppRegs()
{
	m_arRegBase.RemoveAll();
}

CAppRegs::~CAppRegs()
{
	for (int i = 0; i < m_arRegBase.GetSize(); i ++)
	{
		delete m_arRegBase[i];
	}

	m_arRegBase.RemoveAll();
}


void CAppRegs::Load()
{
	for (int i = 0; i < m_arRegBase.GetSize(); i ++)
	{
		m_arRegBase[i]->Load();
	}
	
}

void CAppRegs::Save()
{
	for (int i = 0; i < m_arRegBase.GetSize(); i ++)
	{
		m_arRegBase[i]->Save();
	}
	
}


//////////////////////////////////////////////////////////////////////
// CInfoBase Class
//////////////////////////////////////////////////////////////////////

CInfoBase::CInfoBase()
{
	m_pbCurVal = NULL;
	m_pbDefVal = NULL;

	m_iCurValLen = 0;
	m_iDefValLen = 0;

	m_dwItemType = REG_ITEM_PRECISE;
}

CInfoBase::~CInfoBase()
{
	if (m_pbCurVal)
	{
		delete m_pbCurVal;
		m_pbCurVal = NULL;
	}

	if (m_pbDefVal)
	{
		delete m_pbDefVal;
		m_pbDefVal = NULL;
	}
}


CRegBase::CRegBase(HKEY hkeyBase)
{
	m_hkeyBase = hkeyBase;
	m_arInfoBase.RemoveAll();
}

CRegBase::~CRegBase()
{
	for (int i = 0; i < m_arInfoBase.GetSize(); i ++)
	{
		delete m_arInfoBase[i];
	}

	m_arInfoBase.RemoveAll();
}

void CRegBase::Load()
{
	CRegKey regKey;
	DWORD	dwRet = ERROR_SUCCESS;
	
	for (int i = 0; i < m_arInfoBase.GetSize(); i ++)
	{
		switch(m_arInfoBase[i]->GetItemType())
		{
		case REG_ITEM_PRECISE:
			{
				dwRet = GetPreciseItem(m_arInfoBase[i]);
				break;
			}
		case REG_ITEM_SERIAL:
			{
				DWORD dwFoundItems = 1;

				dwRet = GetSerialItems(i, m_arInfoBase[i], &dwFoundItems);

				i += dwFoundItems;

				break;
			}
		case REG_ITEM_GROUP_START:
			{
				DWORD dwSkipItems = 1;
				dwRet = GetGroupItems(i, m_arInfoBase[i], &dwSkipItems);

				i += dwSkipItems;
				break;
			}
		}


	}
}

DWORD CRegBase::GetSerialItems(DWORD dwStartIndex, CInfoBase *pStartInfoBase, DWORD *pdwTotalFoundItemsCount)
{
	DWORD dwRet;
	HKEY hKey = m_hkeyBase;
	char	pchName[1024];
	BYTE	pbData[1024];
	DWORD	dwType;
	DWORD	dwIndex = 0;
	DWORD	dwCbName, dwCbData;

	*pdwTotalFoundItemsCount = 0;

	dwRet = RegOpenKey(m_hkeyBase, pStartInfoBase->GetSubKey(), &hKey);


	while (dwRet == ERROR_SUCCESS)
	{
		dwCbName = sizeof(pchName);
		dwCbData = sizeof(pbData);

		dwRet = RegEnumValue(
			hKey,
			dwIndex,      
			pchName,
			&dwCbName,
			0,
			&dwType,
			pbData,
			&dwCbData
			);
		
		if(dwRet == ERROR_NO_MORE_ITEMS)
		{
			pchName[100] = 0;
			break;
		}
		else if(dwRet == ERROR_SUCCESS)
		{
			CInfoBase *pInfoBase;
			if (dwIndex == 0)
			{
				pInfoBase = pStartInfoBase;
			}
			else
			{
				pInfoBase = new CInfoBase;
				m_arInfoBase.InsertAt(dwStartIndex + dwIndex, pInfoBase);
			}

			(*pdwTotalFoundItemsCount) ++;

			pInfoBase->SetIndex(pStartInfoBase->GetIndex() + dwIndex);
			pInfoBase->SetSubKey(pStartInfoBase->GetSubKey());
			pInfoBase->SetName(pchName);

			pInfoBase->SetCurVal(dwType, pbData, dwCbData);

			pInfoBase->SetValType(dwType);

		}

		dwIndex ++;
        
	}

	if ((*pdwTotalFoundItemsCount) == 0)
	{
		(*pdwTotalFoundItemsCount) = 1;
	}

	return dwRet;
}

DWORD CRegBase::GetPreciseItem(CInfoBase *pInfoBase)
{
	DWORD dwRet;
	HKEY hKey = m_hkeyBase;
	BYTE	pbData[1024];
	DWORD	dwType;
	DWORD	dwCbData = sizeof(pbData);


	dwRet = RegOpenKey(m_hkeyBase, pInfoBase->GetSubKey(), &hKey);
	
	dwRet = RegQueryValueEx(hKey, pInfoBase->GetName(), NULL, &dwType, pbData, &dwCbData);
	
	if (dwRet == ERROR_SUCCESS)
	{
		pInfoBase->SetCurVal(dwType, pbData, dwCbData);
		pInfoBase->SetValType(dwType);
	}
	else
	{
		pInfoBase->SetDefault();
	}

	return dwRet;
}

void CRegBase::Save()
{
	DWORD dwRet;
	HKEY hKey = m_hkeyBase;
	DWORD	dwVal;
	LPTSTR	lpszVal;
	BYTE	*pbVal;
	DWORD	dwType;
	DWORD	dwCbData;
	DWORD	dwDisposition;
	CInfoBase *pInfoBase;

	for (int i = 0; i < m_arInfoBase.GetSize(); i ++)
	{
		pInfoBase = m_arInfoBase[i];
		dwRet = RegCreateKeyEx(m_hkeyBase, pInfoBase->GetSubKey(), 0, "", 0, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);

		//dwRet = RegOpenKeyEx(m_hkeyBase, pInfoBase->GetSubKey(), 0, KEY_ALL_ACCESS, &hKey);
		dwType = pInfoBase->GetValtype();

		switch(dwType)
		{
		case REG_SZ:
			pInfoBase->GetVal(&lpszVal);
			pbVal = (BYTE *)lpszVal;
			dwCbData = strlen(lpszVal) + 1;
			break;

		case REG_DWORD:
			pInfoBase->GetVal(&dwVal);
			pbVal = (BYTE *)&dwVal;
			dwCbData = sizeof(DWORD);

			break;
		case REG_BINARY:
			pInfoBase->GetVal(&pbVal, &dwCbData);
			break;

		}
		
		dwRet = RegSetValueEx(hKey, pInfoBase->GetName(), 0, dwType, pbVal, dwCbData);
	}
}


//////////////////////////////////////////////////////////////////////
// CProxyReg Class
//////////////////////////////////////////////////////////////////////

INFOBASE CRegProxy::m_InitTable[] =
{
	//Proxy Subkey
	{REG_PROXY_COUNT, "Software\\UniNet\\MyGet\\Proxy", "Count", 
		INT_N_R_, 1, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_PROXY_DEFAULTFORFTP, "Software\\UniNet\\MyGet\\Proxy", "DefaultForFtp", 
		INT_N_R_, 0, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_PROXY_DEFAULTFORHTTP, "Software\\UniNet\\MyGet\\Proxy", "DefaultForHttp", 
		INT_N_R_, 0, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_PROXY_ROLLBACK, "Software\\UniNet\\MyGet\\Proxy", "Rollback", 
		INT_N_R_, 2048, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},

	//Proxy group subkey
	{REG_PROXY_INFO_FLAGS, "Software\\UniNet\\MyGet\\Proxy", "Flags", 
		INT_N_R_, 6, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_GROUP_START},
	{REG_PROXY_INFO_PASSWORD, "Software\\UniNet\\MyGet\\Proxy", "Password", 
		INT_N_R_, INT_N_R_, STR_N_R_, STR_N_R_, (BYTE *) "", (BYTE *) "", 0, 0, REG_BINARY, REG_ITEM_GROUP_MEMBER},
	{REG_PROXY_INFO_PORT, "Software\\UniNet\\MyGet\\Proxy", "Port", 
		INT_N_R_, 1, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_GROUP_MEMBER},
	{REG_PROXY_INFO_SERVER, "Software\\UniNet\\MyGet\\Proxy", "Server", 
		INT_N_R_, INT_N_R_, "", "", BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_SZ, REG_ITEM_GROUP_MEMBER},
	{REG_PROXY_INFO_TITLE, "Software\\UniNet\\MyGet\\Proxy", "Title", 
		INT_N_R_, INT_N_R_, "", "Direct connection", BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_SZ, REG_ITEM_GROUP_MEMBER},
	{REG_PROXY_INFO_TYPE, "Software\\UniNet\\MyGet\\Proxy", "Type", 
		INT_N_R_, 0, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_GROUP_MEMBER},
	{REG_PROXY_INFO_USERNAME, "Software\\UniNet\\MyGet\\Proxy", "UserName", 
		INT_N_R_, INT_N_R_, "", "", BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_SZ, REG_ITEM_GROUP_END},
};
int CRegProxy::m_iSizeOfInitTable = sizeof(m_InitTable) / sizeof(m_InitTable[0]);

CRegProxy::CRegProxy()
{
	m_uID = REG_PROXY_ID;

	FillInfoBase(m_InitTable, m_iSizeOfInitTable);

}

CRegProxy::~CRegProxy()
{

}





//////////////////////////////////////////////////////////////////////
// CGeneralReg Class
//////////////////////////////////////////////////////////////////////

INFOBASE CRegGeneral::m_InitTable[] =
{
	//General subkey
	{REG_GENERAL_APPPATH, "Software\\UniNet\\MyGet\\General", "AppPath", 
		INT_N_R_, INT_N_R_, "", "", BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_SZ, REG_ITEM_PRECISE},
	{REG_GENERAL_AUTOSAVETIME, "Software\\UniNet\\MyGet\\General", "AutoSaveTime", 
		INT_N_R_, 10, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_GENERAL_BACKDATE, "Software\\UniNet\\MyGet\\General", "BackDate", 
		INT_N_R_, INT_N_R_,STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_GENERAL_BLOCKSIZE, "Software\\UniNet\\MyGet\\General", "BlockSize", 
		INT_N_R_, 10240, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_GENERAL_DOUBLECLICK, "Software\\UniNet\\MyGet\\General", "Double Click", 
		INT_N_R_, 16, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_GENERAL_FILEMANAGER, "Software\\UniNet\\MyGet\\General", "File Manager", 
		INT_N_R_, 273, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_GENERAL_FLAGS, "Software\\UniNet\\MyGet\\General", "Flags", 
		INT_N_R_, 0x00000d5c, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_GENERAL_FLAGS1, "Software\\UniNet\\MyGet\\General", "Flags1", 
		INT_N_R_, 0xfffafdf, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_GENERAL_FTPSEARCHMAXHIT, "Software\\UniNet\\MyGet\\General", "FtpSearchMaxHit", 
		INT_N_R_, 40, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_GENERAL_FTPSEARCHMINSIZE, "Software\\UniNet\\MyGet\\General", "FtpSearchMinSize", 
		INT_N_R_, 300, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_GENERAL_LANGUAGEEX, "Software\\UniNet\\MyGet\\General", "LanguageEx", 
		INT_N_R_, INT_N_R_, "", "jceng.ini", BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_SZ, REG_ITEM_PRECISE},
	{REG_GENERAL_MONITORFLAGS, "Software\\UniNet\\MyGet\\General", "Monitor Flags", 
		INT_N_R_, 351, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_GENERAL_MONITORTYPE, "Software\\UniNet\\MyGet\\General", "MonitorType", 
		INT_N_R_, INT_N_R_, "", ".ZIP;.EXE;.BIN;.GZ;.Z;.TAR;.ARJ;.LZH;.MP3;.A[0-9]?", BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_SZ, REG_ITEM_PRECISE},
	{REG_GENERAL_SCHEDULEFLAGS, "Software\\UniNet\\MyGet\\General", "Schedule flags", 
		INT_N_R_, 33023, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_GENERAL_SCHEDULESTARTTIME, "Software\\UniNet\\MyGet\\General", "Schedule Start Time", 
		INT_N_R_, 0x368bad90, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_GENERAL_SCHEDULESTOPTIME, "Software\\UniNet\\MyGet\\General", "Schedule Stop Time", 
		INT_N_R_, 0x368be5d0, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_GENERAL_VERSION, "Software\\UniNet\\MyGet\\General", "Version", 
		INT_N_R_, INT_N_R_, "", "0x95", BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_SZ, REG_ITEM_PRECISE},
	{REG_GENERAL_VIRUSSCANTYPE, "Software\\UniNet\\MyGet\\General", "Virus Scan Type", 
		INT_N_R_, INT_N_R_, "", ".ZIP;.EXE;.DO?;.XL?;.COM;.BIN;.GZ;.Z;.TAR;.ARJ;.LZH;.SIT;.HQX;.TGZ;.DLL;.OCX;.VBX", BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_SZ, REG_ITEM_PRECISE},
	{REG_GENERAL_VIRUSSCANNER, "Software\\UniNet\\MyGet\\General", "Virus Scanner", 
		INT_N_R_, INT_N_R_, "", "", BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_SZ, REG_ITEM_PRECISE},
	{REG_GENERAL_VIRUSSCANNERARGS, "Software\\UniNet\\MyGet\\General", "Virus Scanner Args", 
		INT_N_R_, INT_N_R_, "", "", BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_SZ, REG_ITEM_PRECISE},
	{REG_GENERAL_OPENTYPES, "Software\\UniNet\\MyGet\\General", "Open Types", 
		INT_N_R_, INT_N_R_, "", ".MP3;.MPG;.MPEG;.MOV;.AVI", BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_SZ, REG_ITEM_PRECISE},
	{REG_GENERAL_MINIMIZEWHENX, "Software\\UniNet\\MyGet\\General", "Scexit", 
		INT_N_R_, 0, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
		
	//Color Subkey.
	{REG_COLOR_BLOCKBACK, "Software\\UniNet\\MyGet\\Color", "BlockBack", 
		INT_N_R_, 0x00c8d0d4, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_COLOR_BLOCKSIZE, "Software\\UniNet\\MyGet\\Color", "BlockSize", 
		INT_N_R_, 4, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_COLOR_COMMAND, "Software\\UniNet\\MyGet\\Color", "Command", 
		INT_N_R_, 0x008b0000, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_COLOR_COMMANDBK, "Software\\UniNet\\MyGet\\Color", "CommandBk", 
		INT_N_R_, 0x00eeeeaf, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_COLOR_ERROR, "Software\\UniNet\\MyGet\\Color", "Error", 
		INT_N_R_, 0x0000008b, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_COLOR_ERRORBK, "Software\\UniNet\\MyGet\\Color", "ErrorBk", 
		INT_N_R_, 0x00cbc0ff, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_COLOR_GRAPHBACK, "Software\\UniNet\\MyGet\\Color", "GraphBack", 
		INT_N_R_, 0x00000000, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_COLOR_GRAPHFORE, "Software\\UniNet\\MyGet\\Color", "GraphFore", 
		INT_N_R_, 0x0000ff00, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_COLOR_SCALE, "Software\\UniNet\\MyGet\\Color", "Scale", 
		INT_N_R_, 0x00000008, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_COLOR_SERVER, "Software\\UniNet\\MyGet\\Color", "Server", 
		INT_N_R_, 0x00000000, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_COLOR_SERVERBK, "Software\\UniNet\\MyGet\\Color", "ServerBk", 
		INT_N_R_, 0x00fae6e6, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_COLOR_STATE, "Software\\UniNet\\MyGet\\Color", "State", 
		INT_N_R_, 0x00009300, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_COLOR_STATEBK, "Software\\UniNet\\MyGet\\Color", "StateBk", 
		INT_N_R_, 0x0098fb98, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},

	//Connection Subkey
	{REG_CONNECTION_CT, "Software\\UniNet\\MyGet\\Connection", "CT", 
		INT_N_R_, 30, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_CONNECTION_DT, "Software\\UniNet\\MyGet\\Connection", "DT", 
		INT_N_R_, 65, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_CONNECTION_RETRY, "Software\\UniNet\\MyGet\\Connection", "Retry", 
		INT_N_R_, 200, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_CONNECTION_SIMJOBS, "Software\\UniNet\\MyGet\\Connection", "SimJobs", 
		INT_N_R_, 3, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_CONNECTION_SPEEDLIMIT, "Software\\UniNet\\MyGet\\Connection", "Speed Limit", 
		INT_N_R_, 2560, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_CONNECTION_WT, "Software\\UniNet\\MyGet\\Connection", "WT", 
		INT_N_R_, 3, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},

	//Detail Status Subkey
	{REG_DETAILSTATUS_INFOWIDTH1, "Software\\UniNet\\MyGet\\Detail Status", "InforWidth1", 
		INT_N_R_, 90, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_DETAILSTATUS_INFOWIDTH2, "Software\\UniNet\\MyGet\\Detail Status", "InforWidth2", 
		INT_N_R_, 1000, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_DETAILSTATUS_LINKWIDTH1, "Software\\UniNet\\MyGet\\Detail Status", "LinkWidth1", 
		INT_N_R_, 90, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_DETAILSTATUS_LINKWIDTH2, "Software\\UniNet\\MyGet\\Detail Status", "LinkWidth2", 
		INT_N_R_, 1000, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_DETAILSTATUS_LOGWITH1, "Software\\UniNet\\MyGet\\Detail Status", "LogWidth1", 
		INT_N_R_, 164, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_DETAILSTATUS_LOGWITH2, "Software\\UniNet\\MyGet\\Detail Status", "LogWidth2", 
		INT_N_R_,664, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},


	//DialUp Subkey
	{REG_DIALUP_ENTRY, "Software\\UniNet\\MyGet\\DialUp", "Entry", 
		INT_N_R_, INT_N_R_, "", "", BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_SZ, REG_ITEM_PRECISE},
	{REG_DIALUP_FLAGS, "Software\\UniNet\\MyGet\\DialUp", "Flags", 
		INT_N_R_, 164, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_DIALUP_PASSWORD, "Software\\UniNet\\MyGet\\DialUp", "Password", 
		INT_N_R_, INT_N_R_, STR_N_R_, STR_N_R_, (BYTE *) "", (BYTE *) "", 0, 0, REG_BINARY, REG_ITEM_PRECISE},
	{REG_DIALUP_RETRY, "Software\\UniNet\\MyGet\\DialUp", "Retry", 
		INT_N_R_, 8, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_DIALUP_TITLE, "Software\\UniNet\\MyGet\\DialUp", "Title", 
		INT_N_R_, INT_N_R_, "", "Re-Create a new connection", BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_SZ, REG_ITEM_PRECISE},
	{REG_DIALUP_USERNAME, "Software\\UniNet\\MyGet\\DialUp", "UserName", 
		INT_N_R_, INT_N_R_, "", "", BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_SZ, REG_ITEM_PRECISE},
	{REG_DIALUP_WT, "Software\\UniNet\\MyGet\\DialUp", "WT", 
		INT_N_R_, 10, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},

	//Download default SubKey
	{REG_DOWNLOADDEFAULT_LOGIN, "Software\\UniNet\\MyGet\\Download default", "Login", 
		INT_N_R_, 0x0, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_DOWNLOADDEFAULT_PASSWORD, "Software\\UniNet\\MyGet\\Download default", "Password", 
		INT_N_R_, INT_N_R_, "", "", BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_SZ, REG_ITEM_PRECISE},
	{REG_DOWNLOADDEFAULT_PATH, "Software\\UniNet\\MyGet\\Download default", "Path", 
		INT_N_R_, INT_N_R_, "", "", BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_SZ, REG_ITEM_PRECISE},
	{REG_DOWNLOADDEFAULT_PROXY, "Software\\UniNet\\MyGet\\Download default", "Proxy", 
		INT_N_R_, 0x1, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_DOWNLOADDEFAULT_REFERER, "Software\\UniNet\\MyGet\\Download default", "Referer", 
		INT_N_R_, INT_N_R_, "", "", BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_SZ, REG_ITEM_PRECISE},
	{REG_DOWNLOADDEFAULT_SERVER, "Software\\UniNet\\MyGet\\Download default", "Server", 
		INT_N_R_, 0x0000024e, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_DOWNLOADDEFAULT_SPLIT, "Software\\UniNet\\MyGet\\Download default", "Split", 
		INT_N_R_, 0x3, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_DOWNLOADDEFAULT_START, "Software\\UniNet\\MyGet\\Download default", "Start", 
		INT_N_R_, 0x0, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_DOWNLOADDEFAULT_USERNAME, "Software\\UniNet\\MyGet\\Download default", "UserName", 
		INT_N_R_, INT_N_R_, "", "", BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_SZ, REG_ITEM_PRECISE},

	//DropZone SubKey
	{REG_DROPZONE_SHOW, "Software\\UniNet\\MyGet\\DropZone", "Show", 
		INT_N_R_, 0, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_DROPZONE_SX, "Software\\UniNet\\MyGet\\DropZone", "Sx", 
		INT_N_R_, 1171, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_DROPZONE_SY, "Software\\UniNet\\MyGet\\DropZone", "Sy", 
		INT_N_R_, 26, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_DROPZONE_TRANSPARENT, "Software\\UniNet\\MyGet\\DropZone", "Transparent", 
		INT_N_R_, 1, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
		
	//Find Subkey
	{REG_FIND_FLAGS, "Software\\UniNet\\MyGet\\Find", "Flags", 
		INT_N_R_, 0x00000403, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},

	//Login Subkey
	{REG_LOGIN_COUNT, "Software\\UniNet\\MyGet\\Login", "Count", 
		INT_N_R_, 0, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	
	//Main Frame Subkey
	{REG_MAINFRAME_FRMAE, "Software\\UniNet\\MyGet\\Main Frame", "Frame", 
		INT_N_R_, INT_N_R_, "", "0180 0195 1010 0811    1    0", BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_SZ, REG_ITEM_PRECISE},
	{REG_MAINFRAME_SPLITTER, "Software\\UniNet\\MyGet\\Main Frame", "Splitter", 
		INT_N_R_, INT_N_R_, "", "0158 0200 0460 0097 ", BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_SZ, REG_ITEM_PRECISE},


	//Main List Status Subkey
	{REG_MAINLISTSTATUS_ORDER1, "Software\\UniNet\\MyGet\\Main List Status", "Order1", 
		INT_N_R_, INT_N_R_, STR_N_R_, STR_N_R_, BYTE_N_R_, 
		(BYTE *)"\x000\x000\x001\x000\x002\x000\x003\x000\x004\x000\x005\x000\x006\x000\x007\x000\x008\x000\x009\x000\x00A\x000\x00B\x000\x00C\x000\x00D\x000\x00E\x000", 
		0, 30, REG_BINARY, REG_ITEM_PRECISE},
	{REG_MAINLISTSTATUS_ORDER2, "Software\\UniNet\\MyGet\\Main List Status", "Order2", 
		INT_N_R_, INT_N_R_, STR_N_R_, STR_N_R_, BYTE_N_R_, 
		(BYTE *)"\x000\x000\x001\x000\x002\x000\x003\x000\x004\x000\x005\x000\x006\x000\x007\x000\x008\x000\x009\x000\x00A\x000\x00B\x000\x00C\x000\x00D\x000\x00E\x000", 
		0, 30, REG_BINARY, REG_ITEM_PRECISE},
	{REG_MAINLISTSTATUS_SHOW1, "Software\\UniNet\\MyGet\\Main List Status", "Show1", 
		INT_N_R_, INT_N_R_, STR_N_R_, STR_N_R_, BYTE_N_R_, 
		(BYTE *)"\x023\x080\x000\x000\x031\x080\x000\x000\x082\x0c0\x000\x000\x050\x0a0\x000\x000\x050\x0a0\x000\x000\x036\x0a0\x000\x000\x041\x0c0\x000\x000\x041\x0c0\
				\x000\x000\x041\x0a0\x000\x000\x04d\x0a0\x000\x000\x01e\x0a0\x000\x000\x022\x0c1\x000\x000\x022\x0c1\x000\x000\x0c8\x040\x000\x000\x0c8\x040\x000\x000", 
		0, 60, REG_BINARY, REG_ITEM_PRECISE},
	{REG_MAINLISTSTATUS_SHOW2, "Software\\UniNet\\MyGet\\Main List Status", "Show2", 
		INT_N_R_, INT_N_R_, STR_N_R_, STR_N_R_, BYTE_N_R_, 
		(BYTE *)"\x019\x080\x000\x000\x019\x080\x000\x000\x095\x0c0\x000\x000\x050\x0a0\x000\x000\x050\x020\x000\x000\x020\x020\x000\x000\x041\x040\x000\x000\x041\x040\
				\x000\x000\x041\x020\x000\x000\x023\x020\x000\x000\x01e\x020\x000\x000\x022\x041\x000\x000\x02c\x0c1\x000\x000\x0c8\x040\x000\x000\x0c8\x040\x000\x000", 
		0, 60, REG_BINARY, REG_ITEM_PRECISE},

	//Main Menu Subkey
	{REG_MAINMENU_FLAGS, "Software\\UniNet\\MyGet\\Main Menu", "Flags", 
		INT_N_R_, 2, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},

	//Mirrors Subkey
	{REG_MIRRORS_LIST, "Software\\UniNet\\MyGet\\Mirrors", "List", 
		INT_N_R_, INT_N_R_, "", "", BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_SZ, REG_ITEM_PRECISE},

	//Procotol Subkey
	{REG_PROTOCOL_ASCIIMODE, "Software\\UniNet\\MyGet\\Procotol", "AsciiMode", 
		INT_N_R_, INT_N_R_, "", ".txt;.htm;.html", BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_SZ, REG_ITEM_PRECISE},
	{REG_PROTOCOL_EMAIL, "Software\\UniNet\\MyGet\\Email", "Email", 
		INT_N_R_, INT_N_R_, "", "yourname@yourcompany.com", BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_SZ, REG_ITEM_PRECISE},
	{REG_PROTOCOL_FLAGS, "Software\\UniNet\\MyGet\\Procotol", "Flags", 
		INT_N_R_, 0, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_PROTOCOL_USERAGENT, "Software\\UniNet\\MyGet\\Procotol", "User-Agent", 
		INT_N_R_, 1, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},
	{REG_PROTOCOL_USERAGENTTEXT, "Software\\UniNet\\MyGet\\Procotol", "User-Agent Text", 
		INT_N_R_, INT_N_R_, "", "", BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_SZ, REG_ITEM_PRECISE},

	//SelURL Subkey
	{REG_SELURL_WINDOWPLACEMENT, "Software\\UniNet\\MyGet\\SelURL", "WindowPlacement", 
		INT_N_R_, INT_N_R_, STR_N_R_, STR_N_R_, BYTE_N_R_, 
		(BYTE *)"\x02c\x000\x000\x000\x000\x000\x000\x000\x001\x000\x000\x000\x0FF\x0FF\x0FF\x0FF\x0FF\x0FF\x0FF\x0FF\x0FF\x0FF\x0FF\x0FF\x0FF\x0FF\x0FF\x0FF\
				\x062\x001\x000\x000\x07d\x001\x000\x000\x086\x003\x000\x000\x0d6\x002\x000\x000", 
		0, 30, REG_BINARY, REG_ITEM_PRECISE},

	//Settings Subkey


	//Site Explore Subkey

	//Sounds Subkey
	{REG_SOUND_SOUND0, "Software\\UniNet\\MyGet\\Sounds", "Sound0", 
		INT_N_R_, INT_N_R_, "", "", BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_SZ, REG_ITEM_PRECISE},
	{REG_SOUND_SOUND1, "Software\\UniNet\\MyGet\\Sounds", "Sound1", 
		INT_N_R_, INT_N_R_, "", "", BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_SZ, REG_ITEM_PRECISE},
	{REG_SOUND_SOUND2, "Software\\UniNet\\MyGet\\Sounds", "Sound2", 
		INT_N_R_, INT_N_R_, "", "", BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_SZ, REG_ITEM_PRECISE},
	{REG_SOUND_SOUND3, "Software\\UniNet\\MyGet\\Sounds", "Sound3", 
		INT_N_R_, INT_N_R_, "", "", BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_SZ, REG_ITEM_PRECISE},

	//Toolbar Subkey
	{REG_TOOLBAR_DEFAULT, "Software\\UniNet\\MyGet\\Toolbar", "",//* = THE DEFAULT VALUE FOR THE SUBKEY 
		INT_N_R_, INT_N_R_, STR_N_R_, STR_N_R_, BYTE_N_R_, 
		(BYTE *)"",	0, 0, REG_BINARY, REG_ITEM_PRECISE},
	{REG_TOOLBAR_STATE, "Software\\UniNet\\MyGet\\Toolbar", "State", 
		INT_N_R_, 0, STR_N_R_, STR_N_R_, BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_DWORD, REG_ITEM_PRECISE},


	//Recent File List Subkey
	{REG_RECENTFILELIST1, "Software\\UniNet\\MyGet\\Recent File List", "", 
		INT_N_R_, INT_N_R_, "", "", BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_SZ, REG_ITEM_SERIAL},

	//SelFolder Subkey
	{REG_SELFOLDER1, "Software\\UniNet\\MyGet\\SelFolder", "", 
		INT_N_R_, INT_N_R_, "", "", BYTE_N_R_, BYTE_N_R_, INT_N_R_, INT_N_R_, REG_SZ, REG_ITEM_SERIAL}


	
};

int CRegGeneral::m_iSizeOfInitTable = sizeof(m_InitTable) / sizeof(m_InitTable[0]);

CRegGeneral::CRegGeneral()
{
	m_uID = REG_GENERAL_ID;

	FillInfoBase(m_InitTable, m_iSizeOfInitTable);
}

CRegGeneral::~CRegGeneral()
{

}


void CInfoBase::SetIndex(int iIndex)
{
	m_iIndex = iIndex;
}

void CInfoBase::SetValType(int iValType)
{
	m_iValType = iValType;
}

void CInfoBase::SetCurValInt(int iVal)
{
	if (m_pbCurVal)
	{	
		delete m_pbCurVal;

		m_pbCurVal = NULL;
	}	

	m_iCurValLen = sizeof(int);

	m_pbCurVal = new BYTE[m_iCurValLen];

	*(int *)m_pbCurVal = iVal;
}

void CInfoBase::SetName(LPCTSTR lpszName)
{
	m_strName = lpszName;
}

void CInfoBase::SetDefValInt(int iVal)
{
	if (m_pbDefVal)
	{	
		delete m_pbDefVal;

		m_pbDefVal = NULL;
	}	

	m_iDefValLen = sizeof(int);

	m_pbDefVal = new BYTE[m_iDefValLen];

	*(int *)m_pbDefVal = iVal;
}

void CInfoBase::SetCurValStr(LPCTSTR lpszVal)
{
	if (m_pbCurVal)
	{	
		delete m_pbCurVal;

		m_pbCurVal = NULL;
	}	

	if (!lpszVal)
	{
		m_iCurValLen = 0;
	}
	else
	{
		m_iCurValLen = strlen(lpszVal);
	}


	m_pbCurVal = new BYTE[m_iCurValLen + 1];

	strcpy((char *)m_pbCurVal, lpszVal);

}

void CInfoBase::SetDefValStr(LPCTSTR lpszVal)
{
	if (m_pbDefVal)
	{	
		delete m_pbDefVal;

		m_pbDefVal = NULL;
	}	

	if (!lpszVal)
	{
		m_iDefValLen = 0;
	}
	else
	{
		m_iDefValLen = strlen(lpszVal);
	}


	m_pbDefVal = new BYTE[m_iDefValLen + 1];

	strcpy((char *)m_pbDefVal, lpszVal);

}

void CInfoBase::SetCurValByte(BYTE *lpbVal, int iValLen)
{
	if (m_pbCurVal)
	{	
		delete m_pbCurVal;

		m_pbCurVal = NULL;
	}	

	m_iCurValLen = iValLen;

	m_pbCurVal = new BYTE[m_iCurValLen];

	memcpy(m_pbCurVal, lpbVal, m_iCurValLen);


}

void CInfoBase::SetDefValByte(BYTE *lpbVal, int iValLen)
{
	if (m_pbDefVal)
	{	
		delete m_pbDefVal;

		m_pbDefVal = NULL;
	}	

	m_iDefValLen = iValLen;

	m_pbDefVal = new BYTE[m_iDefValLen];

	memcpy(m_pbDefVal, lpbVal, m_iDefValLen);

}

void CAppRegs::Add(CRegBase *pRegBase)
{
	m_arRegBase.Add(pRegBase);
}

void CInfoBase::SetSubKey(LPCTSTR lpszSubKey)
{
	m_strSubKey = lpszSubKey;
}

LPCTSTR CInfoBase::GetSubKey()
{
	return m_strSubKey;
}

LPCTSTR CInfoBase::GetName()
{
	return m_strName;
}

int CInfoBase::GetValtype()
{
	return m_iValType;
}

void CRegBase::FillInfoBase(PINFOBASE pInitTable, int iSizeOfInitTable)
{

	for (int i = 0; i < iSizeOfInitTable; i ++)
 	{
 		CInfoBase *pInfoBase = new CInfoBase;
 
 		pInfoBase->SetIndex(pInitTable[i].m_iIndex);
 		pInfoBase->SetValType(pInitTable[i].m_iValType);
 		pInfoBase->SetSubKey(pInitTable[i].m_pszSubKey);
 		pInfoBase->SetName(pInitTable[i].m_pszName);
		pInfoBase->SetItemType(pInitTable[i].m_dwItemType);

 		switch (pInitTable[i].m_iValType)
 		{
 		case REG_SZ:
 			pInfoBase->SetCurValStr(pInitTable[i].m_pszCurVal);
 			pInfoBase->SetDefValStr(pInitTable[i].m_pszDefVal);
 			break;
 		case REG_DWORD:
 			pInfoBase->SetCurValInt(pInitTable[i].m_iCurVal);
 			pInfoBase->SetDefValInt(pInitTable[i].m_iDefVal);
 			break;
 		case REG_BINARY:
 			pInfoBase->SetCurValByte(pInitTable[i].m_pbCurVal, pInitTable[i].m_iCurValLen);
 			pInfoBase->SetDefValByte(pInitTable[i].m_pbDefVal, pInitTable[i].m_iDefValLen);
 			break;
 		}
 
 		m_arInfoBase.Add(pInfoBase);
 	}
}


void CInfoBase::SetCurVal(DWORD dwType, BYTE *pbData, DWORD dwValLen)
{
	switch (dwType)
	{
	case REG_SZ:
		SetCurValStr((char *)pbData);
		break;
	case REG_DWORD:
		SetCurValInt(*(int *)pbData);
		break;
	case REG_BINARY:
		SetCurValByte(pbData, dwValLen);
		break;
	}

}


DWORD CInfoBase::GetIndex()
{
	return m_iIndex;
}

BOOL CRegBase::GetVal(DWORD dwRegItemIndex, DWORD *pdwVal)
{
	for (int i = 0; i < m_arInfoBase.GetSize(); i ++)
	{
		if (m_arInfoBase[i]->GetIndex() == dwRegItemIndex)
		{
			return m_arInfoBase[i]->GetVal(pdwVal);
		}
	}

	return FALSE;
}

BOOL CInfoBase::GetVal(DWORD *pdwVal)
{	
	ASSERT(m_iValType == REG_DWORD);

	*pdwVal = *(DWORD *)m_pbCurVal;

	return TRUE;
}

BOOL CAppRegs::GetVal(DWORD dwRegItemIndex, DWORD *pdwVal)
{
	for (int i = 0; i < m_arRegBase.GetSize(); i ++)
	{
		if (m_arRegBase[i]->GetVal(dwRegItemIndex, pdwVal))
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CInfoBase::GetVal(LPSTR *ppszBuf)
{
	ASSERT(m_iValType == REG_SZ);

	*ppszBuf = (LPSTR)m_pbCurVal;

	return TRUE;
}

BOOL CInfoBase::GetVal(BYTE **ppbBuf, DWORD *dwValLen)
{
	ASSERT(m_iValType == REG_BINARY);

	*ppbBuf = m_pbCurVal;

	*dwValLen = m_iCurValLen;
	return TRUE;
}

BOOL CRegBase::GetVal(DWORD dwRegItemIndex, LPSTR *ppszBuf)
{
	for (int i = 0; i < m_arInfoBase.GetSize(); i ++)
	{
		if (m_arInfoBase[i]->GetIndex() == dwRegItemIndex)
		{
			return m_arInfoBase[i]->GetVal(ppszBuf);
		}
	}

	return FALSE;

}

BOOL CAppRegs::GetVal(DWORD dwRegItemIndex, LPSTR *ppszBuf)
{
	for (int i = 0; i < m_arRegBase.GetSize(); i ++)
	{
		if (m_arRegBase[i]->GetVal(dwRegItemIndex, ppszBuf))
		{
			return TRUE;
		}
	}

	return FALSE;

}

BOOL CRegBase::GetVal(DWORD dwRegItemIndex, BYTE **ppbBuf, DWORD *pdwValLen)
{
	for (int i = 0; i < m_arInfoBase.GetSize(); i ++)
	{
		if (m_arInfoBase[i]->GetIndex() == dwRegItemIndex)
		{
			return m_arInfoBase[i]->GetVal(ppbBuf, pdwValLen);
		}
	}

	return FALSE;

}

BOOL CAppRegs::GetVal(DWORD dwRegItemIndex, BYTE **ppbBuf, DWORD *pdwValLen)
{
	for (int i = 0; i < m_arRegBase.GetSize(); i ++)
	{
		if (m_arRegBase[i]->GetVal(dwRegItemIndex, ppbBuf, pdwValLen))
		{
			return TRUE;
		}
	}

	return FALSE;

}

DWORD CInfoBase::GetItemType()
{
	return m_dwItemType;
}

void CInfoBase::SetItemType(DWORD dwItemType)
{
	ASSERT(dwItemType >= REG_ITEM_PRECISE && dwItemType <= REG_ITEM_GROUP_END);

	m_dwItemType = dwItemType;
}

DWORD CRegBase::GetGroupItems(DWORD dwStartIndex, CInfoBase *pStartInfoBase, DWORD *pdwSkipItemsCount)
{
	DWORD dwGroupCount, dwGroupSize, dwGroupIndexWidth;
	DWORD dwRet = ERROR_SUCCESS;
	CString strKey;
	char szGroupKey[MAX_PATH];
	CInfoBase *pInfoBase = NULL;
	CString strBaseSubKey = pStartInfoBase->GetSubKey();


	dwGroupCount		= GetGroupCount();
	dwGroupSize			= GetGroupSize();
	dwGroupIndexWidth	= GetGroupIndexWidth();

	(*pdwSkipItemsCount) = 0;

	for (DWORD i = 0; i < dwGroupCount; i++)
	{
		GetGroupKey(i, szGroupKey);

		strKey.Format("%s%s", strBaseSubKey, szGroupKey);


		for (DWORD j = 0; j < dwGroupSize; j++)
		{
			if (i > 0)
			{
				pInfoBase = CopyItem(dwStartIndex + j, i * dwGroupSize, i * dwGroupIndexWidth);
			}
			else
			{
				pInfoBase = m_arInfoBase.GetAt(dwStartIndex + j);
			}

			pInfoBase->SetSubKey(strKey);


			dwRet = GetPreciseItem(pInfoBase);

			(*pdwSkipItemsCount) ++;

		}

		
	}

	if ((*pdwSkipItemsCount) == 0)
	{
		(*pdwSkipItemsCount) = 1;
	}

	return ERROR_SUCCESS;
}


DWORD CRegProxy::GetGroupCount()
{
	DWORD dwCount = 0;

	GetVal(REG_PROXY_COUNT, &dwCount);

	return dwCount;
}

void CRegProxy::GetGroupKey(DWORD dwIndex, LPSTR lpszGroupKey)
{
	static const char szGroupKeyTemplate[30] = "\\Proxy%d";

	sprintf(lpszGroupKey, szGroupKeyTemplate, dwIndex + 1);
}

DWORD CRegProxy::GetGroupMemberIndex(DWORD dwGroupIndex, DWORD dwGroupMemberIndex)
{
	return (REG_PROXY_INFO_BASE + dwGroupIndex * REG_PROXY_INFO_WIDTH + dwGroupMemberIndex);
}

DWORD CRegProxy::GetGroupSize()
{
	return REG_PROXY_INFO_SIZE;
}

DWORD CRegProxy::GetGroupIndexWidth()
{
	return REG_PROXY_INFO_WIDTH;
}

CInfoBase * CRegBase::CopyItem(DWORD dwIndexCopyFrom, DWORD dwDistanceCopyTo, DWORD dwIndexIncrement)
{
	BYTE	*pbVal;
	DWORD	dwVal;
	LPSTR	lpszVal;
	DWORD	dwValLen = 0;

	CInfoBase *pInfoBaseCopyFrom = m_arInfoBase.GetAt(dwIndexCopyFrom);

	CInfoBase *pInfoBase = new CInfoBase;

	pInfoBase->SetIndex(pInfoBaseCopyFrom->GetIndex() + dwIndexIncrement);
	pInfoBase->SetSubKey(pInfoBaseCopyFrom->GetSubKey());
	pInfoBase->SetName(pInfoBaseCopyFrom->GetName());
	pInfoBase->SetValType(pInfoBaseCopyFrom->GetValtype());
	pInfoBase->SetItemType(pInfoBaseCopyFrom->GetItemType());

	switch(pInfoBaseCopyFrom->GetValtype())
	{
	case REG_SZ:
		pInfoBaseCopyFrom->GetVal(&lpszVal);
		pInfoBase->SetDefValStr(lpszVal);
		break;

	case REG_DWORD:
		pInfoBaseCopyFrom->GetVal(&dwVal);
		pInfoBase->SetDefValInt(dwVal);
		break;

	case REG_BINARY:
		pInfoBaseCopyFrom->GetVal(&pbVal, &dwValLen);
		pInfoBase->SetDefValByte(pbVal, dwValLen);
		break;
	}

	m_arInfoBase.InsertAt(dwIndexCopyFrom + dwDistanceCopyTo, pInfoBase);
	return pInfoBase;
}

BOOL CAppRegs::IsFlagOn(ULONG ulIndex)
{
	ULONG ulBaseIndex = ulIndex & REG_FLAG_BASE_MASK;
	DWORD dwFlag;

	switch(ulBaseIndex)
	{
	case REG_FLAG_GENERAL_FLAGS:
		GetVal(REG_GENERAL_FLAGS, &dwFlag);

		if (dwFlag & (1 << (ulIndex - REG_FLAG_GENERAL_FLAGS)))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
		
		break;

	case REG_FLAG_GENERAL_EXT_FLAGS:
		GetVal(REG_GENERAL_FLAGS1, &dwFlag);

		if (dwFlag & (1 << (ulIndex - REG_FLAG_GENERAL_EXT_FLAGS)))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
		
		break;

	case REG_FLAG_MONITOR_FLAGS:
		GetVal(REG_GENERAL_MONITORFLAGS, &dwFlag);
		if (dwFlag & (1 << (ulIndex - REG_FLAG_MONITOR_FLAGS)))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
		
		break;

	case REG_FLAG_FILEMANAGER_FLAGS:
		GetVal(REG_GENERAL_FILEMANAGER, &dwFlag);
		if (dwFlag & (1 << (ulIndex - REG_FLAG_FILEMANAGER_FLAGS)))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
		
		break;

	case REG_FLAG_SCHEDULE_FLAGS:
		GetVal(REG_GENERAL_SCHEDULEFLAGS, &dwFlag);
		if (dwFlag & (1 << (ulIndex - REG_FLAG_SCHEDULE_FLAGS)))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
		
		break;

	case REG_FLAG_DIALUP_FLAGS:
		GetVal(REG_DIALUP_FLAGS, &dwFlag);
		if (dwFlag & (1 << (ulIndex - REG_FLAG_DIALUP_FLAGS)))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
		
		break;

		
	default:
		return FALSE;

	}
}

BOOL CAppRegs::GetProxyData(PPROXYSTRUCT pProxyStruct, DWORD dwIndex)
{
	if (!pProxyStruct)
	{
		return FALSE;
	}

	LPSTR lpszVal;
	
	//Title
	lpszVal = NULL;
	GetVal(REG_PROXY_INFO_TITLE + dwIndex * REG_PROXY_INFO_WIDTH, &lpszVal);
	if (pProxyStruct->pszTitle)
	{
		delete pProxyStruct->pszTitle;
	}
	pProxyStruct->pszTitle = new char[strlen(lpszVal) + 1];
	strcpy(pProxyStruct->pszTitle, lpszVal);
	
	//Server
	lpszVal = NULL;
	GetVal(REG_PROXY_INFO_SERVER + dwIndex * REG_PROXY_INFO_WIDTH, &lpszVal);
	if (pProxyStruct->pszServer)
	{
		delete pProxyStruct->pszServer;
		pProxyStruct->pszServer = NULL;
	}

	pProxyStruct->pszServer = new char[strlen(lpszVal) + 1];
	strcpy(pProxyStruct->pszServer, lpszVal);
	
	
	//UserName
	lpszVal = NULL;
	GetVal(REG_PROXY_INFO_USERNAME + dwIndex * REG_PROXY_INFO_WIDTH, &lpszVal);
	if (pProxyStruct->pszUserName)
	{
		delete pProxyStruct->pszUserName;
		pProxyStruct->pszUserName = NULL;
	}

	pProxyStruct->pszUserName = new char[strlen(lpszVal) + 1];
	strcpy(pProxyStruct->pszUserName, lpszVal);
	

	//Port
	GetVal(REG_PROXY_INFO_PORT + dwIndex * REG_PROXY_INFO_WIDTH, &pProxyStruct->dwPort);

	//Type
	GetVal(REG_PROXY_INFO_TYPE + dwIndex * REG_PROXY_INFO_WIDTH, &pProxyStruct->dwType);

	//Flags
	GetVal(REG_PROXY_INFO_FLAGS + dwIndex * REG_PROXY_INFO_WIDTH, &pProxyStruct->dwFlag);

	BYTE *pbVal = NULL;
	DWORD dwValLen = 0;
	GetVal(REG_PROXY_INFO_PASSWORD + dwIndex * REG_PROXY_INFO_WIDTH, &pbVal, &dwValLen);

	if (pProxyStruct->pszPassword)
	{
		delete pProxyStruct->pszPassword;
		pProxyStruct->pszPassword = NULL;
	}

	if (dwValLen > 0)
	{
		ASSERT(dwValLen == MAX_PASSWORD_SIZE);

		pProxyStruct->pszPassword = new char[dwValLen];
		memset(pProxyStruct->pszPassword, 0, dwValLen);

		Decrypt(pbVal, pProxyStruct->pszPassword);
	}




	return TRUE;
}


void CInfoBase::SetDefault()
{
	switch (m_iValType)
	{
	case REG_SZ:
		SetCurValStr((char *)m_pbDefVal);
		break;

	case REG_DWORD:
		SetCurValInt(*(int *)m_pbDefVal);
		break;

	case REG_BINARY:
		SetCurValByte(m_pbDefVal, m_iDefValLen);
		break;
	}
}

BYTE CAppRegs::GetFileManagerFlag(DWORD dwIndex, DWORD dwBits)
{
	DWORD dwMask = (1 << dwBits) - 1;
	DWORD dwVal;

	GetVal(REG_GENERAL_FILEMANAGER, &dwVal);

	dwMask <<= dwIndex;
	dwVal &= dwMask;

	return (BYTE)(dwVal >> dwIndex);
	
}

BOOL CAppRegs::SetFileManagerFlag(DWORD dwIndex, DWORD dwBits, BYTE bVal)
{
	DWORD dwVal, dwSetVal;
	dwSetVal = bVal;
	GetVal(REG_GENERAL_FILEMANAGER, &dwVal);

	DWORD dwMask = (1 << dwBits) - 1;
	
	dwSetVal &= dwMask;
	dwMask <<= dwIndex;

	dwMask = ~dwMask;
	dwVal &= dwMask;

	dwVal |= dwSetVal << dwIndex;
	SetVal(REG_GENERAL_FILEMANAGER, dwVal);
	return TRUE;
}


BYTE CAppRegs::GetDoubleClickFlag(DWORD dwIndex, DWORD dwBits)
{
	DWORD dwMask = (1 << dwBits) - 1;
	DWORD dwVal;

	GetVal(REG_GENERAL_DOUBLECLICK, &dwVal);

	dwMask <<= dwIndex;
	dwVal &= dwMask;

	return (BYTE)(dwVal >> dwIndex);

}

DWORD CAppRegs::GetColumnCount(DWORD dwColumnType)
{
	BYTE	*pbVal = NULL;
	DWORD dwValLen = 0;
	switch(dwColumnType)
	{
		case DOWNLOAD_CATEGORY_COLUMN:
			GetVal(REG_MAINLISTSTATUS_ORDER1, &pbVal, &dwValLen);
			break;
		case OTHER_CATEGORY_COLUMN:
			GetVal(REG_MAINLISTSTATUS_ORDER2, &pbVal, &dwValLen);
			break;
	}

	return (dwValLen / 2);
}



DWORD CAppRegs::GetColumnAlignStyle(DWORD dwColumnType, DWORD dwColumnIndex)
{
	DWORD	dwShowInfo = GetColumnShowInfo(dwColumnType, dwColumnIndex);

	dwShowInfo &= 0x7000;
	dwShowInfo >>= 12;

	switch (dwShowInfo & 0x0FFF)
	{
	case 4:
		return LVCFMT_LEFT;
		break;

	case 2:
		return LVCFMT_RIGHT;
		break;

	default:
		return LVCFMT_CENTER;
		break;
	}
}

DWORD CAppRegs::GetColumnWidth(DWORD dwColumnType, DWORD dwColumnIndex)
{
	DWORD	dwShowInfo = GetColumnShowInfo(dwColumnType, dwColumnIndex);
	return (dwShowInfo & 0x07FF);
}

BOOL CAppRegs::IsColumnVisible(DWORD dwColumnType, DWORD dwColumnIndex)
{
	DWORD	dwShowInfo = GetColumnShowInfo(dwColumnType, dwColumnIndex);

	return (dwShowInfo & 0x8000);
}

DWORD CAppRegs::GetColumnShowInfo(DWORD dwColumnType, DWORD dwColumnIndex)
{
	BYTE	*pbVal = NULL;
	DWORD	dwValLen = 0;
	DWORD	dwShowInfo = 0;

	switch(dwColumnType)
	{
		case DOWNLOAD_CATEGORY_COLUMN:
			GetVal(REG_MAINLISTSTATUS_SHOW1, &pbVal, &dwValLen);
			break;
		case OTHER_CATEGORY_COLUMN:
			GetVal(REG_MAINLISTSTATUS_SHOW2, &pbVal, &dwValLen);
			break;
	}

	dwShowInfo = *(DWORD *)(pbVal + dwColumnIndex * sizeof(DWORD));

	return dwShowInfo;
}

DWORD CAppRegs::GetColumnIndexByOrder(DWORD dwColumnType, DWORD dwOrder)
{
	BYTE	*pbVal = NULL;
	DWORD dwValLen = 0;
	switch(dwColumnType)
	{
		case DOWNLOAD_CATEGORY_COLUMN:
			GetVal(REG_MAINLISTSTATUS_ORDER1, &pbVal, &dwValLen);
			break;
		case OTHER_CATEGORY_COLUMN:
			GetVal(REG_MAINLISTSTATUS_ORDER2, &pbVal, &dwValLen);
			break;
	}

	return (*(SHORT *)(pbVal + dwOrder * sizeof(SHORT)));
}

DWORD CAppRegs::GetColumnIndexByScreenOrder(DWORD dwColumnType, DWORD dwScreenOrder)
{
	DWORD dwVisibleColumnIndex = 0;

	for (DWORD i = 0; i < GetColumnCount(dwColumnType); i ++)
	{
		if (IsColumnVisible(dwColumnType, GetColumnIndexByOrder(dwColumnType, i)))		
		{
			if (dwVisibleColumnIndex == dwScreenOrder)
			{
				return GetColumnIndexByOrder(dwColumnType, i);
			}

			dwVisibleColumnIndex ++;
		}
	}

	//Should not arrive here.
	TRACE("Can not find the match column!");

	ASSERT(FALSE);

	return 0;
}

DWORD CAppRegs::GetVisibleColumnCount(DWORD dwColumnType)
{
	DWORD dwVisibleColumnCount = 0; 

	for (DWORD i = 0; i < GetColumnCount(dwColumnType); i ++)
	{
		if (IsColumnVisible(dwColumnType, i))
		{
			dwVisibleColumnCount ++;
		}
	}

	return dwVisibleColumnCount;
}



BOOL CRegBase::SetVal(DWORD dwRegItemIndex, LPCTSTR lpszVal)
{
	for (int i = 0; i < m_arInfoBase.GetSize(); i ++)
	{
		if (m_arInfoBase[i]->GetIndex() == dwRegItemIndex)
		{
			m_arInfoBase[i]->SetCurValStr(lpszVal);
			return TRUE;
		}
	}

	return FALSE;	
}

BOOL CRegBase::SetVal(DWORD dwRegItemIndex, DWORD dwVal)
{
	for (int i = 0; i < m_arInfoBase.GetSize(); i ++)
	{
		if (m_arInfoBase[i]->GetIndex() == dwRegItemIndex)
		{
			m_arInfoBase[i]->SetCurValInt(dwVal);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CRegBase::SetVal(DWORD dwRegItemIndex, BYTE *pbVal, DWORD dwValLen)
{
	for (int i = 0; i < m_arInfoBase.GetSize(); i ++)
	{
		if (m_arInfoBase[i]->GetIndex() == dwRegItemIndex)
		{
			m_arInfoBase[i]->SetCurValByte(pbVal, dwValLen);

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CAppRegs::SetVal(DWORD dwRegItemIndex, LPCTSTR lpszVal)
{
	for (int i = 0; i < m_arRegBase.GetSize(); i ++)
	{
		if (m_arRegBase[i]->SetVal(dwRegItemIndex, lpszVal))
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CAppRegs::SetVal(DWORD dwRegItemIndex, DWORD dwVal)
{
	for (int i = 0; i < m_arRegBase.GetSize(); i ++)
	{
		if (m_arRegBase[i]->SetVal(dwRegItemIndex, dwVal))
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CAppRegs::SetVal(DWORD dwRegItemIndex, BYTE *pbVal, DWORD dwValLen)
{
	for (int i = 0; i < m_arRegBase.GetSize(); i ++)
	{
		if (m_arRegBase[i]->SetVal(dwRegItemIndex, pbVal, dwValLen))
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CAppRegs::SetFlag(DWORD dwRegItemIndex, BOOL bOn)
{
	ULONG ulBaseIndex = dwRegItemIndex & REG_FLAG_BASE_MASK;
	DWORD dwFlag;

	switch(ulBaseIndex)
	{
	case REG_FLAG_GENERAL_FLAGS:
		GetVal(REG_GENERAL_FLAGS, &dwFlag);

		if (bOn)
		{
			dwFlag |= (1 << (dwRegItemIndex - REG_FLAG_GENERAL_FLAGS));
		}
		else
		{
			dwFlag &= ~(1 << (dwRegItemIndex - REG_FLAG_GENERAL_FLAGS));
		}

		SetVal(REG_GENERAL_FLAGS, dwFlag);
				
		break;

	case REG_FLAG_GENERAL_EXT_FLAGS:
		GetVal(REG_GENERAL_FLAGS1, &dwFlag);

		if (bOn)
		{
			dwFlag |= (1 << (dwRegItemIndex - REG_FLAG_GENERAL_EXT_FLAGS));
		}
		else
		{
			dwFlag &= ~(1 << (dwRegItemIndex - REG_FLAG_GENERAL_EXT_FLAGS));
		}

		SetVal(REG_GENERAL_FLAGS1, dwFlag);

		break;

	case REG_FLAG_MONITOR_FLAGS:
		GetVal(REG_GENERAL_MONITORFLAGS, &dwFlag);

		if (bOn)
		{
			dwFlag |= (1 << (dwRegItemIndex - REG_FLAG_MONITOR_FLAGS));
		}
		else
		{
			dwFlag &= ~(1 << (dwRegItemIndex - REG_FLAG_MONITOR_FLAGS));
		}

		SetVal(REG_GENERAL_MONITORFLAGS, dwFlag);

		break;

	case REG_FLAG_FILEMANAGER_FLAGS:
		GetVal(REG_GENERAL_FILEMANAGER, &dwFlag);
		if (bOn)
		{
			dwFlag |= (1 << (dwRegItemIndex - REG_FLAG_FILEMANAGER_FLAGS));
		}
		else
		{
			dwFlag &= ~(1 << (dwRegItemIndex - REG_FLAG_FILEMANAGER_FLAGS));
		}

		SetVal(REG_GENERAL_FILEMANAGER, dwFlag);

		break;

	case REG_FLAG_SCHEDULE_FLAGS:
		GetVal(REG_GENERAL_SCHEDULEFLAGS, &dwFlag);
		if (bOn)
		{
			dwFlag |= (1 << (dwRegItemIndex - REG_FLAG_SCHEDULE_FLAGS));
		}
		else
		{
			dwFlag &= ~(1 << (dwRegItemIndex - REG_FLAG_SCHEDULE_FLAGS));
		}

		SetVal(REG_GENERAL_SCHEDULEFLAGS, dwFlag);

		break;

	case REG_FLAG_DIALUP_FLAGS:
		GetVal(REG_DIALUP_FLAGS, &dwFlag);
		if (bOn)
		{
			dwFlag |= (1 << (dwRegItemIndex - REG_FLAG_DIALUP_FLAGS));
		}
		else
		{
			dwFlag &= ~(1 << (dwRegItemIndex - REG_FLAG_DIALUP_FLAGS));
		}

		SetVal(REG_DIALUP_FLAGS, dwFlag);

		break;

		
	default:
		break;

	}
}


BOOL CAppRegs::SetProxyData(PPROXYSTRUCT pProxyStruct, DWORD dwIndex)
{

	CRegProxy *pRegProxy = (CRegProxy *)GetRegBase(REG_PROXY_ID);

	if (pRegProxy)
	{
		return pRegProxy->SetProxyData(pProxyStruct, dwIndex);
	}

	return FALSE;


}

BOOL CAppRegs::ClearProxyData()
{
	CRegProxy *pRegProxy = (CRegProxy *)GetRegBase(REG_PROXY_ID);

	if (pRegProxy)
	{
		return pRegProxy->ClearProxyData();
	}

	return FALSE;
}

UINT CRegBase::GetID()
{
	return m_uID;
}

CRegBase * CAppRegs::GetRegBase(UINT uID)
{
	for (int i = 0; i < m_arRegBase.GetSize(); i ++)
	{
		if (m_arRegBase[i]->GetID() == uID)
		{
			return m_arRegBase[i];
		}
	}

	return NULL;
}

BOOL CRegProxy::ClearProxyData()
{
	for (int i = 4; i < m_arInfoBase.GetSize(); i ++)
	{
		delete m_arInfoBase[i];
	}

	m_arInfoBase.SetSize(4);

	SetVal(REG_PROXY_COUNT, (DWORD)1);
	SetVal(REG_PROXY_DEFAULTFORHTTP, (DWORD)0);
	SetVal(REG_PROXY_DEFAULTFORFTP, (DWORD)0);

	return TRUE;
}

BOOL CRegProxy::SetProxyData(PPROXYSTRUCT pProxyStruct, DWORD dwIndex)
{
	if (!pProxyStruct)
	{
		return FALSE;
	}


	DWORD dwGroupSize		= GetGroupSize();
	DWORD dwGroupIndexWidth	= GetGroupIndexWidth();
	char szGroupKey[MAX_PATH];
	GetGroupKey(dwIndex, szGroupKey);
	CString strKey;
	strKey.Format("%s%s", m_InitTable[0].m_pszSubKey, szGroupKey);
	

	for (int i = 4; i < m_iSizeOfInitTable; i ++)
	{
		CInfoBase *pInfoBase = new CInfoBase;
		
		pInfoBase->SetIndex(m_InitTable[i].m_iIndex + dwIndex * dwGroupIndexWidth);
		pInfoBase->SetValType(m_InitTable[i].m_iValType);
		pInfoBase->SetSubKey(strKey);
		pInfoBase->SetName(m_InitTable[i].m_pszName);
		pInfoBase->SetItemType(m_InitTable[i].m_dwItemType);
		
		switch (m_InitTable[i].m_iValType)
		{
		case REG_SZ:
			pInfoBase->SetCurValStr(m_InitTable[i].m_pszCurVal);
			pInfoBase->SetDefValStr(m_InitTable[i].m_pszDefVal);
			break;
		case REG_DWORD:
			pInfoBase->SetCurValInt(m_InitTable[i].m_iCurVal);
			pInfoBase->SetDefValInt(m_InitTable[i].m_iDefVal);
			break;
		case REG_BINARY:
			pInfoBase->SetCurValByte(m_InitTable[i].m_pbCurVal, m_InitTable[i].m_iCurValLen);
			pInfoBase->SetDefValByte(m_InitTable[i].m_pbDefVal, m_InitTable[i].m_iDefValLen);
			break;
		}
		
		m_arInfoBase.InsertAt(i + dwIndex * dwGroupSize, pInfoBase);

	}


	//Set each item.
	SetVal(REG_PROXY_INFO_FLAGS + dwIndex * dwGroupIndexWidth,			pProxyStruct->dwFlag);

	if (pProxyStruct->pszPassword)
	{
		BYTE arEncryptPSW[MAX_PASSWORD_SIZE];
		memset(arEncryptPSW, 0, sizeof(arEncryptPSW));

		if (Encrypt(arEncryptPSW, pProxyStruct->pszPassword))
		{
			SetVal(REG_PROXY_INFO_PASSWORD + dwIndex * dwGroupIndexWidth,	arEncryptPSW, MAX_PASSWORD_SIZE);
		}
		else
		{
			SetVal(REG_PROXY_INFO_PASSWORD + dwIndex * dwGroupIndexWidth,	arEncryptPSW, 0);
		}
	}

	SetVal(REG_PROXY_INFO_PORT + dwIndex * dwGroupIndexWidth,			pProxyStruct->dwPort);

	SetVal(REG_PROXY_INFO_SERVER + dwIndex * dwGroupIndexWidth,			pProxyStruct->pszServer);

	SetVal(REG_PROXY_INFO_TITLE + dwIndex * dwGroupIndexWidth,			pProxyStruct->pszTitle);

	SetVal(REG_PROXY_INFO_TYPE + dwIndex * dwGroupIndexWidth,			pProxyStruct->dwType);

	SetVal(REG_PROXY_INFO_USERNAME + dwIndex * dwGroupIndexWidth,		pProxyStruct->pszUserName);

	return TRUE;
}

