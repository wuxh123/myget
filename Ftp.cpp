// Ftp.cpp: implementation of the CFtp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Ftp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const DWORD DATA_BUF_SIZE = 4096*4;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFtp::CFtp()
{
	m_dwDataBufSize	= DATA_BUF_SIZE;
	m_dwDataCount	= 0;
	m_pControlFunc	= NULL;
	m_pDataFunc		= NULL;
//建立控制连接的socket.
	m_socketCtrl			= ::socket(PF_INET, SOCK_STREAM, 0);
//	m_dwTransDirectionFlag	= SEND_INFO_TO_SERVER_ICON;
	m_pData					= new char[DATA_BUF_SIZE];
}

CFtp::~CFtp()
{
	if (m_socketCtrl)
	{
		::closesocket(m_socketCtrl);
	}

	if (m_pData)
	{
		delete m_pData;
	}

}

void CFtp::SetControlFunc(void *pVoidData, BOOL (*pfn)(void *pVoidData, const char *lpszBuf))
{
	m_pControlFuncData = pVoidData;
	m_pControlFunc = pfn;
}

void CFtp::SetDataFunc(void *pVoidData, BOOL (*pfn)(void *pVoidData, const char *lpszBuf))
{
	m_pDataFuncData = pVoidData;
	m_pDataFunc = pfn;
}

int CFtp::SendBuffer()
{
	CString strBuf;
//	m_dwTransDirectionFlag = SEND_INFO_TO_SERVER_ICON;
	strcat(m_szBuf, "\r\n");
	::send(m_socketCtrl, m_szBuf, strlen(m_szBuf), 0);

	strBuf = m_szBuf;
	strBuf.TrimRight("\r\n");
	if(m_pControlFunc)
	{
		(*m_pControlFunc)(m_pControlFuncData, strBuf);
	}
	return TRUE;
}

int CFtp::RecvBuffer()
{
//	m_dwTransDirectionFlag = RECV_INFO_FROM_SERVER_ICON;
	memset(m_szBuf, 0, sizeof(m_szBuf));

	CString strBuf;
	CString strTemp;
	int iPos = 0;
	int i =0 ;

	do
	{
		i = ::recv(m_socketCtrl, m_szBuf, sizeof(m_szBuf), 0);
		strBuf = m_szBuf;
		while((iPos = strBuf.Find("\r\n")) > 0)
		{
			strTemp = strBuf.Left(iPos);

			if(m_pControlFunc)
			{
				(*m_pControlFunc)(m_pControlFuncData, strTemp);
			}

			strBuf.Delete(0, iPos+2);
		}

	}while(strTemp.GetAt(3) == '-');

	return i;
}


int CFtp::CheckReplyCode(const char *lpszBuf)
{
	if (lpszBuf)
	{
		return lpszBuf[0] - '0';
	}
	else
	{
		return m_szBuf[0] - '0';
	}
}


int CFtp::SendGeneralCommand(BOOL bCanWait)
{
	SendBuffer();

	RecvBuffer();

	for (int i = 0; i < 2; i ++)
	{
		switch(CheckReplyCode())
		{
		case 1:
			if (!bCanWait)
			{
				return STATUS_ERROR;
			}
			else
			{
				bCanWait = FALSE;
				RecvBuffer();
			}

			break;
		case 3:
			return STATUS_ERROR;
			break;

		case 2:
			return STATUS_SUCCESS;
			break;

		case 4:
		case 5:
			return STATUS_FAILED;
			break;

		}

	}
	return STATUS_FAILED;
}

int CFtp::ConnectToServer(const char *lpszServerIP, unsigned short uPort)
{
	struct sockaddr_in siServer;
	int iRet;

	const WORD wMinVer = 0x0101;
	WSADATA wsaData;

	if(0 != ::WSAStartup(wMinVer, &wsaData))
	{
		WSACleanup();
		return -2;
	}

	struct hostent *pHost;
	pHost = gethostbyname(lpszServerIP);

	if (!pHost)
	{
		return -1;
	}

	siServer.sin_family = AF_INET;
	memcpy(&(siServer.sin_addr.S_un.S_addr),  pHost->h_addr_list[0], pHost->h_length);
	siServer.sin_port	 = htons(uPort);

//解析服务器的域名地址
	CString strServerNumIP,strServerIP;
	strServerIP = lpszServerIP;
	if(atoi(strServerIP) == 0)
	{
		strServerNumIP = inet_ntoa(siServer.sin_addr);
		strServerNumIP = "Connecting " + strServerIP+"[IP="+strServerNumIP+":21]";
		const char *pServerNumIP = strServerNumIP;
//		SendMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_USER_SEND_CONTROL_LINK_COMMAND, 
//			SEND_INFO_TO_SERVER_ICON, (LPARAM)pServerNumIP); 
	}

//连接到服务器控制端。
	iRet = ::connect(m_socketCtrl, (struct sockaddr *)&siServer, sizeof(struct sockaddr_in));

	if (iRet == SOCKET_ERROR)
	{
		return iRet;
	}
	else
	{
		RecvBuffer();
		return 0;
	}
}


int CFtp::Login(const char *lpszUserName, const char *lpszPassword)
{
	sprintf(m_szBuf, "USER %s", lpszUserName);

	SendBuffer();

	RecvBuffer();

	switch(CheckReplyCode(m_szBuf))
	{
		case 1:
			return STATUS_ERROR;
			break;
		case 2:
			return STATUS_SUCCESS;
			break;
		case 3:
			break;
		case 4:
			return STATUS_FAILED;
			break;
		case 5:
			return STATUS_FAILED;
			break;
	}

	//Now we arrive here means that we need to send the password to the server. the reply code is beginning with '3'.
	sprintf(m_szBuf, "PASS %s", lpszPassword);

	SendBuffer();

	RecvBuffer();

	switch(CheckReplyCode(m_szBuf))
	{
		case 1:
			return STATUS_ERROR;
			break;
		case 2:
			return STATUS_SUCCESS;
			break;
		case 3: //实际上，这个分支应该继续ACCT命令，但是没有实现。
			//break;
		case 4:
			return STATUS_FAILED;
			break;
		case 5:
			return STATUS_FAILED;
			break;
	}

	return STATUS_SUCCESS;
}



int CFtp::SetType(char chType)
{
	sprintf(m_szBuf, "TYPE %c", chType);
	
	return SendGeneralCommand();
}


int CFtp::Help()
{
	strcpy(m_szBuf, "HELP");

	return SendGeneralCommand();
}


int CFtp::Abort()
{
	strcpy(m_szBuf, "ABOR");

	return SendGeneralCommand();
}



int CFtp::DeleteFile(const char *lpszFilePath)
{
	sprintf(m_szBuf, "DELE %s", lpszFilePath);

	return SendGeneralCommand();
}



int CFtp::ChangeWorkingDir(const char *lpszWorkingDir)
{
	sprintf(m_szBuf, "CWD %s", lpszWorkingDir);

	return SendGeneralCommand();
}


int CFtp::CDUP()
{
	strcpy(m_szBuf, "CDUP");

	return SendGeneralCommand();
}



int CFtp::NOOP()
{
	strcpy(m_szBuf, "NOOP");

	return SendGeneralCommand();
}


int CFtp::Passive()
{
	strcpy(m_szBuf, "PASV");

	return SendGeneralCommand();
}

int CFtp::Quit()
{
	strcpy(m_szBuf, "QUIT");

	return SendGeneralCommand();
}


int CFtp::Port(const char *lpszHost, unsigned short uPort)
{
	strcpy(m_szBuf, lpszHost);
	
	for (DWORD i = 0; i < strlen(m_szBuf); i ++)
	{
		if (m_szBuf[i] == '.')
		{
			m_szBuf[i] = ',';
		}
	}
	
	unsigned short uPartPort;
	uPartPort = uPort / 256;
	sprintf(m_szBuf, "%s,%d", m_szBuf, uPartPort);

	uPartPort = uPort - uPartPort;
	sprintf(m_szBuf, "%s,%d", m_szBuf, uPartPort);

	return SendGeneralCommand();

}

int CFtp::RemoveDir(const char *lpszDir)
{
	sprintf(m_szBuf, "RMD %s", lpszDir);

	return SendGeneralCommand();
}

int CFtp::MakeDir(const char *lpszDir)
{
	sprintf(m_szBuf, "MKD %s", lpszDir);

	return SendGeneralCommand();
}

int CFtp::PWD()
{
	strcpy(m_szBuf, "PWD");

	return SendGeneralCommand();
}

int CFtp::CWD(CString str)
{
	sprintf(m_szBuf, "CWD %s", str+"/");
	return SendGeneralCommand();
}


void CFtp::GetIPandPort()
{
	char  strPort[10], strP[10];
	char  *pDest, *pCur;
	int	  result1, result2;
	int i = 0;

	pDest = strchr( m_szBuf, '(' );
	memset(m_szDataIP, 0, sizeof(m_szDataIP));
	memset(strPort, 0, sizeof(strPort));
	pCur = pDest+1;
	pDest = pCur;

	for(int i = 0; i < 4; i++)
	{
		while(*(pCur++) != ',');
	 	strncat(m_szDataIP, pDest, pCur-pDest-1);
		while(i < 3)
		{
 			strcat(m_szDataIP, ".");
			break;
		}
		pDest = pCur;
	}
 
	for( i=0; i < 2; i++)
	{
		while(*(pCur) != ',' && *(pCur) != ')')
		{
			pCur++;
		}


		if(i == 0)
		{
			memset(strP, 0, sizeof(strP));
			strncpy(strP, pDest, pCur-pDest);
			result1 = atoi(strP)*256;
		}

 		if( i == 1)
		{
			memset(strP, 0, sizeof(strP));
			strncpy(strP, pDest, pCur-pDest);
			result2 = atoi(strP);
		}

		pCur++;
		pDest = pCur;
	}
	
	m_uDataPort = (unsigned short)(result1 + result2);
}

int CFtp::List()
{
	struct sockaddr_in siServer;
	int iRet;
	int iRecvSize = 0;
	char szRecvBuf[500];

	DWORD dwCurBufSize = m_dwDataBufSize;

	siServer.sin_family				= AF_INET;
	siServer.sin_addr.S_un.S_addr	= inet_addr(m_szDataIP);
	siServer.sin_port				= htons(m_uDataPort);

	m_socketData = ::socket(PF_INET, SOCK_STREAM, 0);

//连接到服务器数据端。
	iRet = ::connect(m_socketData, (struct sockaddr *)&siServer, sizeof(struct sockaddr_in));

	memset(m_szBuf, 0, sizeof(m_szBuf));

	strcpy(m_szBuf, "LIST");

	SendBuffer();

	RecvBuffer();

	memset(m_pData, 0, m_dwDataBufSize);
	switch(CheckReplyCode())
	{
	case 1:
		do
		{
			memset(szRecvBuf, 0, sizeof(szRecvBuf));
			iRecvSize = ::recv(m_socketData, szRecvBuf, sizeof(szRecvBuf), 0);
			if(iRecvSize > 0)
			{
				while((m_dwDataCount + iRecvSize) > m_dwDataBufSize)
				{
					m_dwDataBufSize += 500;
				}

				if(dwCurBufSize != m_dwDataBufSize)
				{
					char *pTempBuf = new char[m_dwDataBufSize];
					memset(pTempBuf, 0, m_dwDataBufSize);
					strcpy(pTempBuf, m_pData);
					delete m_pData;
					m_pData = pTempBuf;
					dwCurBufSize = m_dwDataBufSize;
				}

				m_dwDataCount += iRecvSize;
				strncat(m_pData, szRecvBuf, iRecvSize);
			}
		}while(iRecvSize > 0);

		::closesocket(m_socketData);

		if(m_pDataFunc)
		{
			(*m_pDataFunc)(m_pDataFuncData, m_pData);
		}
		RecvBuffer();
		break;
	case 3:
		return STATUS_ERROR;
		break;

	case 2:
		return STATUS_SUCCESS;
		break;

	case 4:
		return STATUS_FAILED;
		break;

	case 5:
		return STATUS_FAILED;
		break;
	}
	return STATUS_SUCCESS;
}



