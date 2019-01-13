// ProxyData.cpp: implementation of the CProxyData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "myget.h"
#include "ProxyData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProxyData::CProxyData()
{
	m_pProxyStruct = new PROXYSTRUCT;
	memset(m_pProxyStruct, 0, sizeof(PROXYSTRUCT));

	SetTitle("");
	SetServer("");
	SetUser("");
	m_pProxyStruct->dwFlag = PASV_MASK | NO_CACHE_MASK;	
}

CProxyData::~CProxyData()
{
	if (!m_pProxyStruct)
	{
		return;
	}

	if (m_pProxyStruct->pszTitle)
	{
		delete m_pProxyStruct->pszTitle;
		m_pProxyStruct->pszTitle = NULL;
	}
	
	if (m_pProxyStruct->pszServer)
	{
		delete m_pProxyStruct->pszServer;
		m_pProxyStruct->pszServer = NULL;
	}
	

	if (m_pProxyStruct->pszUserName)
	{
		delete m_pProxyStruct->pszUserName;
		m_pProxyStruct->pszUserName = NULL;
	}


	
	if (m_pProxyStruct->pszPassword)
	{
		delete m_pProxyStruct->pszPassword;
		m_pProxyStruct->pszPassword = NULL;
	}

	delete m_pProxyStruct;
}

DWORD CProxyData::GetFlag()
{
	return m_pProxyStruct->dwFlag;
}

void CProxyData::SetFlag(DWORD dwFlag)
{
	m_pProxyStruct->dwFlag = dwFlag;
}

UINT CProxyData::GetPort()
{
	return (UINT)m_pProxyStruct->dwPort;
}

void CProxyData::SetPort(DWORD dwPort)
{
	m_pProxyStruct->dwPort = dwPort;
}

DWORD CProxyData::GetType()
{
	return m_pProxyStruct->dwType;
}

void CProxyData::SetType(DWORD dwType)
{
	m_pProxyStruct->dwType = dwType;
}

LPTSTR CProxyData::GetServer()
{
	return m_pProxyStruct->pszServer;
}

void CProxyData::SetServer(LPCTSTR pszServer)
{
	if (pszServer)// && (strlen(pszServer) > 0))
	{
		if (m_pProxyStruct->pszServer)
		{
			delete(m_pProxyStruct->pszServer);
		}
		
		m_pProxyStruct->pszServer = new TCHAR[strlen(pszServer) + 1];
		
		if (!m_pProxyStruct->pszServer)
		{
			return;
		}

		strcpy(m_pProxyStruct->pszServer, pszServer);
	}
}

LPTSTR CProxyData::GetTitle()
{
	return m_pProxyStruct->pszTitle;
}

void CProxyData::SetTitle(LPCTSTR pszTitle)
{
	if (pszTitle)// && (strlen(pszTitle) > 0))
	{
		if (m_pProxyStruct->pszTitle)
		{
			delete(m_pProxyStruct->pszTitle);
		}

		m_pProxyStruct->pszTitle = new TCHAR[strlen(pszTitle) + 1];

		if (!m_pProxyStruct->pszTitle)
		{
			return;
		}
	

		strcpy(m_pProxyStruct->pszTitle, pszTitle);
	}
}

LPTSTR CProxyData::GetUser()
{
	return m_pProxyStruct->pszUserName;
}

void CProxyData::SetUser(LPCTSTR pszUserName)
{
	if (pszUserName)// && (strlen(pszUserName) > 0))
	{
		if (m_pProxyStruct->pszUserName)
		{
			delete(m_pProxyStruct->pszUserName);
		}

		m_pProxyStruct->pszUserName = new TCHAR[strlen(pszUserName) + 1];

		if (!m_pProxyStruct->pszUserName)
		{
			return;
		}
		

		strcpy(m_pProxyStruct->pszUserName, pszUserName);
	}

}

LPTSTR CProxyData::GetPassword()
{
	return m_pProxyStruct->pszPassword;
}

void CProxyData::SetPassword(LPCTSTR pszPassword)
{
	if (pszPassword)
	{
		if (m_pProxyStruct->pszPassword)
		{
			delete m_pProxyStruct->pszPassword;
			m_pProxyStruct->pszPassword = NULL;

		}

		m_pProxyStruct->pszPassword = new TCHAR[strlen(pszPassword) + 1];

		if (m_pProxyStruct->pszPassword)
		{
			strcpy(m_pProxyStruct->pszPassword, pszPassword);
		}
		

		
	}
}

PPROXYSTRUCT CProxyData::GetProxyStruct()
{
	return m_pProxyStruct;
}
