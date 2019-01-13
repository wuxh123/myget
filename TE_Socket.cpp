////////////////////////////////////////////////////////////////////////////////
//	Copyright (C) 2000-2001 Softelf Inc. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
//
//	Author	: Telan 
//	Date	: 2000-10-04
//	Purpose	: Encapsulate winsock2 functions to make it more easily used 
//	History	: 
//		1.0	: 2000-03-10 - First Edition of this source code ( called:FE_SOCKET )
//		2.0	: 2000-06-25 - Second Edition ( name changed to TE_SOCKET )
//						 - Add Error Control
//		3.0	: 2000-09-21 - Third Edition ( name changed to TE_SOCKET )
//						 - Change the errors' process mechanism
//						 - Add BufSocket Model
//						 - Add TE_ConnectEx(...)
//						 - Add TE_BSocketGetData(...) for specail usage
//		3.1 : 2000-10-04 - Add TE_AcceptEx(...)
//						 - Add TE_GetIP(...) to fix NT DNS resolve cache problem
//						 - Modify TE_ConnectEx
//						 - Fix several bugs in NetEvent process
//
//	Mailto	: telan@263.net ( Bugs' Report or Comments )
//	Notes	: This source code may be used in any form in any way you desire. It is
//			  provided "as is" without express or implied warranty. Use it at your own
//			  risk! The author accepts no liability for any damage/loss of business 
//			  that this product may cause.
//
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "TE_Socket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//需要ws2_32.lib库
#pragma comment(lib,"ws2_32.lib")

//错误代号
static __declspec(thread) int _iThreadedErrorNo = 0;

// 辅助函数
//获取最近一次操作的错误代码
int TE_GetLastError()
{
	return (_iThreadedErrorNo);
}

//设置错误代码
void TE_SetLastError(int iErrorCode)
{
	_iThreadedErrorNo = iErrorCode;
}

/////////////////////////////////////////////////////////////////////////
//		Winsock2 函数
/////////////////////////////////////////////////////////////////////////
//初始化Winsock2动态连接库
int TE_InitLibrary()
{
	WSADATA         WSD;
	WORD wVersionRequired = MAKEWORD( TE_SOCKET_MAJOR_VERSION,TE_SOCKET_MINOR_VERSION );
	
	ZeroMemory(&WSD,sizeof(WSADATA));

	int nErrorNo = WSAStartup(wVersionRequired, &WSD);

	if ( SOCKET_SUCCESS != nErrorNo )
	{
		TE_SetLastError( nErrorNo );
		return ( SOCKET_ERROR );
	}

	if ( LOBYTE( WSD.wVersion ) != TE_SOCKET_MINOR_VERSION ||
		 HIBYTE( WSD.wVersion ) != TE_SOCKET_MAJOR_VERSION ) 
	{
		WSACleanup( );
		TE_SetLastError( WSAVERNOTSUPPORTED );
		return (SOCKET_ERROR); 
	}
 
	//成功初始化
	return (SOCKET_SUCCESS);
}


//释放Winsock2动态连接库
void TE_CleanupLibrary(void)
{
    WSACleanup();
}


//设置套接字属性
int TE_SetSocketOption(SOCKET hSocket)
{

    int             iActivate = 1;

	//允许地址重用
    if (setsockopt(hSocket, SOL_SOCKET, SO_REUSEADDR, (const char *) &iActivate,sizeof(iActivate)) == SOCKET_ERROR )
    {
        TE_SetLastError( WSAGetLastError() );
        return (SOCKET_ERROR);
    }

	//  如果支持，设置KEEPALIVE属性 (这样做会带来其他不良后果)
	//setsockopt(hSocket, SOL_SOCKET, SO_KEEPALIVE, (const char *) &iActivate,sizeof(iActivate));

	return (SOCKET_SUCCESS);
}

//创建具有重叠IO能力的套接字
SOCKET TE_CreateSocket(int iAddressFamily /*= AF_INET*/, int iType/*= SOCK_STREAM*/,int iProtocol/*= 0*/)
{
	SOCKET hSocket = WSASocket(iAddressFamily, iType, iProtocol, NULL,0,WSA_FLAG_OVERLAPPED);
	if ( hSocket == INVALID_SOCKET )
	{
		TE_SetLastError( WSAGetLastError() );
		return (INVALID_SOCKET);
	}

	//设置套接字选项
	if ( SOCKET_ERROR == TE_SetSocketOption(hSocket) )	//设置属性失败
	{
		TE_CloseSocket(hSocket, TRUE);
		return (INVALID_SOCKET);
	}
	return (hSocket);
}


//关闭套接字
void TE_CloseSocket(SOCKET hSocket, BOOL bHardClose)
{
	// 不需要捕获错误
	if (!bHardClose) // 优雅关闭 Graceful close
	{
		// 不再发送数据，对于TCP套接字，在所有的数据都发送完毕之后，
		// 将发送一个 FIN ，通知接收方所有数据已经发送完毕。
		shutdown(hSocket, SD_SEND);

		// 接收缓冲区有可能还有未接收的数据，在关闭套接字之前应该先
		// 读取残留的数据。
		int		iRecvResult;
		HANDLE	hSocketEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		char	szBuffer[256];
		do
		{
			if (hSocketEvent != NULL)
			{
				//注册网络事件
				WSAEventSelect(hSocket, (WSAEVENT) hSocketEvent, FD_READ | FD_CLOSE);
				WSAWaitForMultipleEvents(1, &hSocketEvent, TRUE,TE_SHUTDOWN_RECV_TIMEOUT, TRUE);
				//清除网络事件
				WSAEventSelect(hSocket, (WSAEVENT) hSocketEvent, 0);
			}
			ZeroMemory(szBuffer,256);
			iRecvResult = TE_RecvLL(hSocket, szBuffer, sizeof(szBuffer));
		} while (iRecvResult > 0);

		if (hSocketEvent != NULL)
			CloseHandle(hSocketEvent);

		//不再允许接收和发送
		shutdown(hSocket, SD_BOTH);
	}
	
	// 关闭套接字
	closesocket(hSocket);
}


// 一次性接收数据(重叠IO)
int TE_RecvLL(SOCKET hSocket, char *pszBuffer, int iBufferSize)
{
	DWORD		dwRtxBytes = 0,
				dwRtxFlags = 0;
	WSABUF		WSABuff;

	//清空缓冲
	ZeroMemory(&WSABuff,sizeof(WSABUF));

	WSABuff.len = iBufferSize;
	WSABuff.buf = pszBuffer;

	return ((WSARecv(hSocket, &WSABuff, 1, &dwRtxBytes, &dwRtxFlags,NULL, NULL) == SOCKET_SUCCESS) ? (int) dwRtxBytes : -WSAGetLastError());
}

// 接收数据(阻塞直至收到数据为止)
int TE_RecvData(SOCKET hSocket, char *pszBuffer, int iBufferSize, DWORD dwTimeout)
{
	HANDLE hReadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	if (hReadEvent == NULL)
	{
		TE_SetLastError( (int)GetLastError() );
		return ( SOCKET_ERROR );
	}

	int		iRecvBytes = 0;
	DWORD	dwWaitResult ;
	for (;;)
	{
		// 注册FD_READ | FD_CLOSE 事件 
		// (因为可能在等待FD_READ事件中，对方关闭套接字，所以要关注FD_CLOSE)
		if( WSAEventSelect(hSocket, (WSAEVENT) hReadEvent, FD_READ | FD_CLOSE) == SOCKET_ERROR)
		{
			CloseHandle(hReadEvent);
			TE_SetLastError( WSAGetLastError() );
			return (SOCKET_ERROR);
		}
	
		// 等等FD_READ | FD_CLOSE事件的发生
		dwWaitResult = WSAWaitForMultipleEvents(1, &hReadEvent, TRUE,dwTimeout, TRUE);

		if (dwWaitResult != WSA_WAIT_EVENT_0)
		{
			// 清除事件
			WSAEventSelect(hSocket, (WSAEVENT) hReadEvent, 0);
			CloseHandle(hReadEvent);
			TE_SetLastError( WSAGetLastError() );
			return (SOCKET_ERROR);
		}
		
		////////////////////////////////////////////////////////////// 
		///	注意：即使 dwWaitResult == WSA_WAIT_EVENT0 ，也应该 
		///			进一步检查网络是否发生错误
		///////////////////////////////////////////////////////////////
		WSANETWORKEVENTS NetEvent;
		if(WSAEnumNetworkEvents(hSocket,(WSAEVENT)hReadEvent,&NetEvent) == SOCKET_ERROR)
		{
			// 清除事件
			WSAEventSelect(hSocket, (WSAEVENT) hReadEvent, 0);
			CloseHandle(hReadEvent);
			TE_SetLastError( WSAGetLastError() );
			return (SOCKET_ERROR);
		}
		//判断发生了什么事件 FD_READ 或 FD_CLOSE	
		if( ( NetEvent.lNetworkEvents == FD_CLOSE ) ||
			 ( NetEvent.lNetworkEvents	== FD_READ && 
			   NetEvent.iErrorCode[FD_READ_BIT] !=0 ) )	// 发生错误
		{
			// 清除事件
			WSAEventSelect(hSocket, (WSAEVENT) hReadEvent, 0);
			CloseHandle(hReadEvent);
			TE_SetLastError(WSAGetLastError() );
			return (SOCKET_ERROR);
		}
		////////////////////////////////////////////////////////////////
		// 清除事件
		WSAEventSelect(hSocket, (WSAEVENT) hReadEvent, 0);
	
		// 接收数据
		if ((iRecvBytes = TE_RecvLL(hSocket, pszBuffer, iBufferSize)) >= 0)
			break;	// 跳出循环

		int iErrorCode = -iRecvBytes;

		if ( iErrorCode != WSAEWOULDBLOCK )	//太多的未完成重叠操作
		{
			CloseHandle(hReadEvent);
			TE_SetLastError( iErrorCode );
			return (SOCKET_ERROR);
		}

		//阻塞住了
		////////////////////////////////////////////////////////////////////////
		//  如果发生阻塞,就等待一定时间后重试,以免CPU轮询浪费时间
		////////////////////////////////////////////////////////////////////////
		Sleep(TE_BLOCKED_SNDRCV_SLEEP);
    }

    CloseHandle(hReadEvent);
    return (iRecvBytes);
}


// 接收数据直至达到规定的长度(缓冲区满)或超时或没有数据可读取时
int TE_Recv(SOCKET hSocket, char *pszBuffer, int iBufferSize, DWORD dwTimeout)
{
	int	iRtxBytes	= 0;
	int iRtxCurrent	= 0;

	while (iRtxBytes < iBufferSize)
	{
		iRtxCurrent = TE_RecvData(hSocket, (pszBuffer + iRtxBytes),(iBufferSize - iRtxBytes), dwTimeout);

		if (iRtxCurrent < 0)	//没有数据可读取
			return (iRtxBytes);

		iRtxBytes += iRtxCurrent;
	}

	return (iRtxBytes);
}


//一次性发送数据(重叠IO)
int TE_SendLL(SOCKET hSocket, char const * pszBuffer, int iBufferSize)
{
	DWORD	dwRtxBytes = 0;
	WSABUF	WSABuff;

	ZeroMemory(&WSABuff,sizeof(WSABUF));
	WSABuff.len = iBufferSize;
	WSABuff.buf = (char *) pszBuffer;

	return ((WSASend(hSocket, &WSABuff, 1, &dwRtxBytes, 0,NULL, NULL) == SOCKET_SUCCESS) ? (int) dwRtxBytes : -WSAGetLastError());

}

// 发送数据
int TE_SendData(SOCKET hSocket, char const * pszBuffer, int iBufferSize, DWORD dwTimeout)
{

	HANDLE	hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	if (hWriteEvent == NULL)
	{
		TE_SetLastError( (int)GetLastError() );
		return (SOCKET_ERROR);
	}

	int iSendBytes = 0;

	for (;;)
	{
		////////////////////////////////////////////////////////////////
		// 发送数据成功
		if ((iSendBytes = TE_SendLL(hSocket, pszBuffer, iBufferSize)) >= 0)
			break;

		int iErrorCode = -iSendBytes;

		if (iErrorCode != WSAEWOULDBLOCK)
		{
			CloseHandle(hWriteEvent);
			TE_SetLastError( WSAGetLastError() );
			return (SOCKET_ERROR);
		}

		
		///////////////////////////////////////////////////////////////////////////////
		//  睡眠一段时间
		///////////////////////////////////////////////////////////////////////////////
		Sleep(TE_BLOCKED_SNDRCV_SLEEP);


		// 注册FD_WRITE | FD_CLOSE 事件
		if( WSAEventSelect(hSocket, (WSAEVENT) hWriteEvent, FD_WRITE|FD_CLOSE) == SOCKET_ERROR)
		{
			CloseHandle(hWriteEvent);
			TE_SetLastError( WSAGetLastError() );
			return (SOCKET_ERROR);
		}

		// 等待事件发生
		DWORD dwWaitResult = WSAWaitForMultipleEvents(1, &hWriteEvent, TRUE,dwTimeout, TRUE);

		if (dwWaitResult != WSA_WAIT_EVENT_0)
		{
			// 清除网络事件
			WSAEventSelect(hSocket, (WSAEVENT) hWriteEvent, 0);
			CloseHandle(hWriteEvent);
			TE_SetLastError( WSAGetLastError() );
			return (SOCKET_ERROR);
		}

		////////////////////////////////////////////////////////////// 
		///	注意：即使 dwWaitResult == WSA_WAIT_EVENT0 ，也应该 
		///			进一步检查网络是否发生错误
		///////////////////////////////////////////////////////////////
		WSANETWORKEVENTS NetEvent;
		if(WSAEnumNetworkEvents(hSocket,(WSAEVENT)hWriteEvent,&NetEvent) == SOCKET_ERROR)
		{
  			// 清除网络事件
			WSAEventSelect(hSocket, (WSAEVENT) hWriteEvent, 0);
			CloseHandle(hWriteEvent);
			TE_SetLastError( WSAGetLastError() );
			return (SOCKET_ERROR);
		}
		if( ( NetEvent.lNetworkEvents == FD_CLOSE ) ||
			 ( NetEvent.lNetworkEvents == FD_WRITE   &&
				NetEvent.iErrorCode[FD_WRITE_BIT] !=0 ) )	// 发生错误
		{
			// 清除网络事件
			WSAEventSelect(hSocket, (WSAEVENT) hWriteEvent, 0);
			CloseHandle(hWriteEvent);
			TE_SetLastError( WSAGetLastError() );
			return (SOCKET_ERROR);
		}
		// 清除网络事件
		WSAEventSelect(hSocket, (WSAEVENT) hWriteEvent, 0);
   }

    CloseHandle(hWriteEvent);
    return (iSendBytes);
}

//发送完所有数据或超时
int TE_Send(SOCKET hSocket, char const * pszBuffer, int iBufferSize, DWORD dwTimeout)
{

	int iRtxBytes	= 0;
	int iRtxCurrent = 0;

	while (iRtxBytes < iBufferSize)
	{
		iRtxCurrent = TE_SendData(hSocket, (pszBuffer + iRtxBytes),(iBufferSize - iRtxBytes), dwTimeout);
		if (iRtxCurrent < 0)
			return (iRtxBytes);

		iRtxBytes += iRtxCurrent;
	}
	return (iRtxBytes);
}

// 建立连接
int TE_Connect(SOCKET hSocket, const struct sockaddr * pSocketAddress, int iNameLen,DWORD dwTimeout)
{

	HANDLE hConnectEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	if (hConnectEvent == NULL)
	{
		TE_SetLastError( (int)GetLastError() );
		return (SOCKET_ERROR);
	}

	// 注册FD_CONNECT事件
	if( WSAEventSelect(hSocket, (WSAEVENT) hConnectEvent, FD_CONNECT) == SOCKET_ERROR)
	{
		CloseHandle(hConnectEvent);
		TE_SetLastError( WSAGetLastError() );
		return (SOCKET_ERROR);
	}

	int	iConnectResult = WSAConnect(hSocket, pSocketAddress, iNameLen, NULL, NULL, NULL, NULL);
	int	iConnectError  = WSAGetLastError();
	
	if ((iConnectResult == SOCKET_ERROR) && (iConnectError == WSAEWOULDBLOCK))
	{
		DWORD dwWaitResult = WSAWaitForMultipleEvents(1, &hConnectEvent, TRUE,dwTimeout, TRUE);
		
		if (dwWaitResult != WSA_WAIT_EVENT_0)
		{
			TE_SetLastError( WSAGetLastError() );
			iConnectResult = SOCKET_ERROR;
		}
		else
		{
			////////////////////////////////////////////////////////////// 
			///	注意：即使 dwWaitResult == WSA_WAIT_EVENT0 ，也应该 
			///			进一步检查网络是否发生错误
			///////////////////////////////////////////////////////////////
			WSANETWORKEVENTS NetEvent;
			if(WSAEnumNetworkEvents(hSocket,(WSAEVENT)hConnectEvent,&NetEvent) == SOCKET_ERROR)
			{
				TE_SetLastError( WSAGetLastError() );
				iConnectResult = SOCKET_ERROR;
			}
			else if(NetEvent.iErrorCode[FD_CONNECT_BIT] !=0 )	// 发生错误
			{
				TE_SetLastError( NetEvent.iErrorCode[FD_CONNECT_BIT] );
				iConnectResult = SOCKET_ERROR;
			}
			else
				iConnectResult = SOCKET_SUCCESS;
			////////////////////////////////////////////////////////////////
		}
	}

	// 注销网络事件
	WSAEventSelect(hSocket, (WSAEVENT) hConnectEvent, 0);
	CloseHandle(hConnectEvent);
	return (iConnectResult);
}


///////////////////////////////////////////////////////////
//
//	引入该函数的目的是为了避免NT下对域名解析的CACHE造成的问题
//
///////////////////////////////////////////////////////////
DWORD WINAPI DNSThread(  LPVOID pParam )
{
	DWORD dwIP = INADDR_NONE;
	PHOSTENT pHost = gethostbyname( (char *)pParam );
	if(pHost == NULL)
		return INADDR_NONE;
	dwIP = inet_addr( inet_ntoa(*(IN_ADDR *)*pHost->h_addr_list) );
	return dwIP;
}

DWORD TE_GetIP(const char* name,BOOL fFixNtDNS /* = FALSE*/) // Used to Fix NT DNS Problem
{
	DWORD dwIP = inet_addr(name);
	if( dwIP != INADDR_NONE )
		return dwIP;

	if( fFixNtDNS )
	{
		OSVERSIONINFO		osVersion;
		osVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		if( GetVersionEx(&osVersion) )
		{
			if(osVersion.dwPlatformId == VER_PLATFORM_WIN32_NT)
			{
				DWORD	dwThreadId = 0;
				HANDLE	hThread = CreateThread(NULL,0,DNSThread,(LPVOID)name,0,&dwThreadId);
				if( hThread != NULL)
				{
					WaitForSingleObject(hThread,INFINITE);
					if( GetExitCodeThread(hThread,&dwIP))
						return dwIP;
				}
			}
		}
	}

	PHOSTENT pHost = gethostbyname(name);
	if(pHost == NULL)
		return INADDR_NONE;
		
	dwIP = inet_addr( inet_ntoa(*(IN_ADDR *)*pHost->h_addr_list) );
	return dwIP;
}

// 建立连接(扩展函数)
int TE_ConnectEx(SOCKET hSocket, char const * pszServer, int nPort,DWORD dwTimeout,BOOL fFixNtDNS /*= FALSE*/)
{
	/////////////////////////////////////////////////////////////////////////////
	SOCKADDR_IN sockAddr;
	ZeroMemory(&sockAddr,sizeof(sockAddr));

	sockAddr.sin_family			= AF_INET;
	sockAddr.sin_port			= htons((u_short)nPort);
	sockAddr.sin_addr.s_addr	= TE_GetIP(pszServer,fFixNtDNS);
	
	if (sockAddr.sin_addr.s_addr == INADDR_NONE)
	{
		TE_SetLastError( WSAGetLastError() );
		return (SOCKET_ERROR);
	}
	//////////////////////////////////////////////////////////////////////

	return TE_Connect(hSocket, (SOCKADDR *)&sockAddr,sizeof(sockAddr),dwTimeout);
}

// 绑定套接字
int TE_BindSocket(SOCKET hSocket, const struct sockaddr * SockName, int iNameLen)
{

	if (bind(hSocket, SockName, iNameLen) == SOCKET_ERROR)
	{
		TE_SetLastError( WSAGetLastError() );
		return (SOCKET_ERROR);
	}

	return (SOCKET_SUCCESS);
}

// 绑定套接字
int TE_BindSocketEx(SOCKET hSocket,int nPort)
{
	SOCKADDR_IN sockAddr;
	ZeroMemory(&sockAddr,sizeof(sockAddr));

	sockAddr.sin_family			= AF_INET;
	sockAddr.sin_addr.s_addr	= htonl(INADDR_ANY);
	sockAddr.sin_port			= htons((u_short)nPort);

	return TE_BindSocket(hSocket,(SOCKADDR *)&sockAddr, sizeof(sockAddr));
}


// 监听套接字
int TE_ListenSocket(SOCKET hSocket, int iConnections)
{
	if(listen(hSocket, iConnections) == SOCKET_ERROR)
	{
		TE_SetLastError( WSAGetLastError() );
		return (SOCKET_ERROR);
	}
	
	return (SOCKET_SUCCESS);
}


// 接受套接字连接
SOCKET TE_Accept(SOCKET hSocket, struct sockaddr * pSocketAddress, int *iNameLen,DWORD dwTimeout)
{

	HANDLE hAcceptEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (hAcceptEvent == NULL)
	{
		TE_SetLastError( (int)GetLastError() );
		return (INVALID_SOCKET);
	}

	// 注册FD_ACCEPT事件
	if( WSAEventSelect(hSocket, (WSAEVENT) hAcceptEvent, FD_ACCEPT) == SOCKET_ERROR)
	{
		CloseHandle(hAcceptEvent);
		TE_SetLastError( WSAGetLastError() );
		return (INVALID_SOCKET);
	}

	SOCKET hSocketAccept = WSAAccept(hSocket, pSocketAddress, iNameLen, NULL, 0);
	int	   iConnectError = WSAGetLastError();

	if ((hSocketAccept == INVALID_SOCKET) && (iConnectError == WSAEWOULDBLOCK))
	{

		// 阻塞
		DWORD dwWaitResult = WSAWaitForMultipleEvents(1, &hAcceptEvent, TRUE,dwTimeout, TRUE);

		if (dwWaitResult == WSA_WAIT_EVENT_0)
		{
			////////////////////////////////////////////////////////////// 
			///	注意：即使 dwWaitResult == WSA_WAIT_EVENT0 ，也应该 
			///			进一步检查网络是否发生错误
			///////////////////////////////////////////////////////////////
			WSANETWORKEVENTS NetEvent;
			if(WSAEnumNetworkEvents(hSocket,hAcceptEvent,&NetEvent) == SOCKET_ERROR)
				TE_SetLastError( WSAGetLastError() );
			else if(NetEvent.iErrorCode[FD_ACCEPT_BIT] !=0 )	// 发生错误
				TE_SetLastError( NetEvent.iErrorCode[FD_ACCEPT_BIT] );
			else
				hSocketAccept = WSAAccept(hSocket, pSocketAddress, iNameLen, NULL, 0);
		}
		else
			TE_SetLastError( WSAGetLastError() );
	}
	
	// 注销网络事件
	WSAEventSelect(hSocket, (WSAEVENT) hAcceptEvent, 0);
	CloseHandle(hAcceptEvent);

	if (hSocketAccept != INVALID_SOCKET)
	{
		// 设置套接字的属性为地址可重用并且为非阻塞的
		if (	(TE_BlockSocket(hSocketAccept, 0)  == SOCKET_ERROR ) ||
				(TE_SetSocketOption(hSocketAccept) == SOCKET_ERROR ) )
		{
			TE_CloseSocket(hSocketAccept,TRUE);
			return (INVALID_SOCKET);
		}
	}
	return (hSocketAccept);
}

// 接受套接字连接(允许中断)
SOCKET	TE_AcceptEx(SOCKET hSocket, struct sockaddr * pSockName, int *iNameLen,HANDLE hEndEvent,DWORD dwTimeout /*= TE_DEFAULT_TIMEOUT*/)
{
	if( hEndEvent == NULL)
		return TE_Accept(hSocket,pSockName,iNameLen,dwTimeout);

	HANDLE	hAcceptEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (hAcceptEvent == NULL)
	{
		TE_SetLastError( (int)GetLastError() );
		return (INVALID_SOCKET);
	}

	WSAEVENT hEvent[2]; 
	hEvent[0] = (WSAEVENT)hAcceptEvent;
	hEvent[1] = (WSAEVENT)hEndEvent;

	// 注册FD_ACCEPT事件
	if( WSAEventSelect(hSocket, (WSAEVENT) hAcceptEvent, FD_ACCEPT) == SOCKET_ERROR)
	{
		CloseHandle(hAcceptEvent);
		TE_SetLastError( WSAGetLastError() );
		return (INVALID_SOCKET);
	}

	SOCKET hSocketAccept = WSAAccept(hSocket, pSockName, iNameLen, NULL, 0);
	int	   iConnectError = WSAGetLastError();

	if ((hSocketAccept == INVALID_SOCKET) && (iConnectError == WSAEWOULDBLOCK))
	{
		// 阻塞
		DWORD dwWaitResult = WSAWaitForMultipleEvents(2, hEvent, FALSE,dwTimeout, TRUE);
		if (dwWaitResult == WSA_WAIT_EVENT_0)
		{
			////////////////////////////////////////////////////////////// 
			///	注意：即使 dwWaitResult == WSA_WAIT_EVENT0 ，也应该 
			///			进一步检查网络是否发生错误
			///////////////////////////////////////////////////////////////
			WSANETWORKEVENTS NetEvent;
			if(WSAEnumNetworkEvents(hSocket,hAcceptEvent,&NetEvent) == SOCKET_ERROR)
				TE_SetLastError( WSAGetLastError() );
			else if(NetEvent.iErrorCode[FD_ACCEPT_BIT] !=0 )	// 发生错误
				TE_SetLastError( NetEvent.iErrorCode[FD_ACCEPT_BIT] );
			else
				hSocketAccept = WSAAccept(hSocket, pSockName, iNameLen, NULL, 0);
		}
		else
			TE_SetLastError( WSAGetLastError() );
	}
	
	// 注销网络事件
	WSAEventSelect(hSocket, (WSAEVENT) hAcceptEvent, 0);
	CloseHandle(hAcceptEvent);

	if (hSocketAccept != INVALID_SOCKET)
	{
		// 设置套接字的属性为地址可重用并且为非阻塞的
		if (	(TE_BlockSocket(hSocketAccept, 0) < 0) ||
				(TE_SetSocketOption(hSocketAccept) < 0) )
		{
			TE_CloseSocket(hSocketAccept,TRUE);
			return (INVALID_SOCKET);
	
		}
	}
	return (hSocketAccept);

}

// 设置套接字是否为阻塞的
int      TE_BlockSocket(SOCKET hSocket, BOOL bBlock)
{
	u_long  IoctlLong = (bBlock) ? 0 : 1;

	if (ioctlsocket(hSocket, FIONBIO, &IoctlLong) == SOCKET_ERROR)
	{
		TE_SetLastError( WSAGetLastError() );
		return (SOCKET_ERROR);
    }

	return (SOCKET_SUCCESS);

}

// 数据报接收函数
int TE_RecvDataFrom( SOCKET hSocket, struct sockaddr * pFrom, int iFromlen,
					 char *pszBuffer, int iBufferSize, DWORD dwTimeout)
{
	HANDLE hReadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (hReadEvent == NULL)
	{
		TE_SetLastError((int)GetLastError() );
		return (SOCKET_ERROR);
	}

	DWORD		dwRtxBytes = 0,
				dwRtxFlags = 0;
	WSABUF		WSABuff;

	ZeroMemory(&WSABuff,sizeof(WSABUF));
	WSABuff.len = iBufferSize;
	WSABuff.buf = pszBuffer;

	for (;;)
	{
		// 注册FD_READ事件
		if( WSAEventSelect(hSocket, (WSAEVENT) hReadEvent, FD_READ) == SOCKET_ERROR)
		{
			CloseHandle(hReadEvent);
			TE_SetLastError(  WSAGetLastError() );
			return (SOCKET_ERROR);
		}
		DWORD dwWaitResult = WSAWaitForMultipleEvents(1, &hReadEvent, TRUE,	dwTimeout, TRUE);

		if( dwWaitResult != WSA_WAIT_EVENT_0 )
		{
			// 注销事件
			WSAEventSelect(hSocket, (WSAEVENT) hReadEvent, 0);
			CloseHandle(hReadEvent);
			TE_SetLastError( WSAGetLastError());
			return (SOCKET_ERROR);
		}

		////////////////////////////////////////////////////////////// 
		///	注意：即使 dwWaitResult == WSA_WAIT_EVENT0 ，也应该 
		///			进一步检查网络是否发生错误
		///////////////////////////////////////////////////////////////
		WSANETWORKEVENTS NetEvent;
		if(WSAEnumNetworkEvents(hSocket,(WSAEVENT)hReadEvent,&NetEvent) == SOCKET_ERROR)
		{
			// 注销事件
			WSAEventSelect(hSocket, (WSAEVENT) hReadEvent, 0);
			CloseHandle(hReadEvent);
			TE_SetLastError( WSAGetLastError() );
			return (SOCKET_ERROR);
		}
		if(NetEvent.iErrorCode[FD_READ_BIT] !=0 )	// 发生错误
		{
			// 注销事件
			WSAEventSelect(hSocket, (WSAEVENT) hReadEvent, 0);
			CloseHandle(hReadEvent);
			TE_SetLastError(NetEvent.iErrorCode[FD_READ_BIT]);
			return (SOCKET_ERROR);
		}
		////////////////////////////////////////////////////////////////
		// 注销事件
		WSAEventSelect(hSocket, (WSAEVENT) hReadEvent, 0);

		int FromLen = iFromlen;
		if ( WSARecvFrom(hSocket, &WSABuff, 1, &dwRtxBytes, &dwRtxFlags,pFrom, &FromLen, NULL, NULL) == SOCKET_SUCCESS )
			break;

		if ( WSAGetLastError() != WSAEWOULDBLOCK)
		{
			CloseHandle(hReadEvent);
			TE_SetLastError( WSAGetLastError() );
			return (SOCKET_ERROR);
		}

		///////////////////////////////////////////////////////////////////////////
		//	睡眠一段时间
		//////////////////////////////////////////////////////////////////////////
		Sleep(TE_BLOCKED_SNDRCV_SLEEP);
	}
	CloseHandle(hReadEvent);
	return ((int) dwRtxBytes);
}

// 数据报发送数据报
int TE_SendDataTo(SOCKET hSocket, const struct sockaddr * pTo,int iToLen,
				  char const * pszBuffer, int iBufferSize, DWORD dwTimeout)
{
	HANDLE hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (hWriteEvent == NULL)
	{
		TE_SetLastError( (int)GetLastError() );
		return (SOCKET_ERROR);
	}

	DWORD	dwRtxBytes = 0,
            dwRtxFlags = 0;
	WSABUF	WSABuff;

	ZeroMemory(&WSABuff,sizeof(WSABUF));
	WSABuff.len = iBufferSize;
	WSABuff.buf = (char *) pszBuffer;

	for (;;)
	{
		if (WSASendTo( hSocket, &WSABuff, 1, &dwRtxBytes, dwRtxFlags,pTo, iToLen, NULL, NULL) == SOCKET_SUCCESS)
			break;

		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			CloseHandle(hWriteEvent);
			TE_SetLastError(  WSAGetLastError() );
			return (SOCKET_ERROR);
		}

		//////////////////////////////////////////////////////////////////////////
		//	睡眠一段时间
		/////////////////////////////////////////////////////////////////////////
		Sleep(TE_BLOCKED_SNDRCV_SLEEP);

		// 注册FD_WRITE事件  
		if( WSAEventSelect(hSocket, (WSAEVENT) hWriteEvent, FD_WRITE) == SOCKET_ERROR)
		{
			CloseHandle(hWriteEvent);
			TE_SetLastError( WSAGetLastError() );
			return (SOCKET_ERROR);
		}
		DWORD dwWaitResult = WSAWaitForMultipleEvents(1, &hWriteEvent, TRUE,dwTimeout, TRUE);
		
		if( dwWaitResult != WSA_WAIT_EVENT_0 )
		{
			// 注销事件
			WSAEventSelect(hSocket, (WSAEVENT) hWriteEvent, 0);
			CloseHandle(hWriteEvent);
			TE_SetLastError(  WSAGetLastError() );
			return (SOCKET_ERROR);
		}

		////////////////////////////////////////////////////////////// 
		///	注意：即使 dwWaitResult == WSA_WAIT_EVENT0 ，也应该 
		///			进一步检查网络是否发生错误
		///////////////////////////////////////////////////////////////
		WSANETWORKEVENTS NetEvent;
		if(WSAEnumNetworkEvents(hSocket,(WSAEVENT)hWriteEvent,&NetEvent) == SOCKET_ERROR)
		{
			// 注销事件
			WSAEventSelect(hSocket, (WSAEVENT) hWriteEvent, 0);
			CloseHandle(hWriteEvent);
			TE_SetLastError(  WSAGetLastError() );
			return (SOCKET_ERROR);
		}
		if(NetEvent.iErrorCode[FD_WRITE_BIT] !=0 )	// 发生错误
		{
			// 注销事件
			WSAEventSelect(hSocket, (WSAEVENT) hWriteEvent, 0);
			CloseHandle(hWriteEvent);
			TE_SetLastError(NetEvent.iErrorCode[FD_WRITE_BIT]);
			return (SOCKET_ERROR);
		}
		////////////////////////////////////////////////////////////////
		// 注销事件
		WSAEventSelect(hSocket, (WSAEVENT) hWriteEvent, 0);
	}

	CloseHandle(hWriteEvent);
	return ((int) dwRtxBytes);
}



////////////////////////////////////////////////////////////////////////////////
//
//	BSocket函数
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
//		功能: 绑定套接字
//		参数: 
//				hSocket		-- SOCKET(待绑定套接字)
//				iBufferSize	-- 缓冲区长度(缺省为TE_SOCKET_BUFFER_SIZE)
//		返回: 
//				结构指针(失败时为NULL)
//
////////////////////////////////////////////////////////////////////////////////
PBSD TE_BSocketAttach(SOCKET hSocket, int iBufferSize /* = TE_SOCKET_BUFFER_SIZE */ )
{

	if( hSocket == INVALID_SOCKET)
		return NULL;

	// 分配内存
	PBSD pBSD = (PBSD) malloc(sizeof(BSD));

	if (pBSD == NULL)
		return NULL;

	char *pszBuffer = (char *) malloc(iBufferSize);

	if (pszBuffer == NULL)
	{
		free(pBSD);
		return NULL;
	}

	// 设置结构成员
	ZeroMemory(pBSD,sizeof(BSD));
	ZeroMemory(pszBuffer,iBufferSize);

	pBSD->hSocket			= hSocket;
	pBSD->iBufferSize		= iBufferSize;
	pBSD->pszBuffer			= pszBuffer;
	pBSD->iBytesInBuffer	= 0;
	pBSD->iReadIndex		= 0;
	pBSD->iBufferIndex		= 0;

	return pBSD;
}

////////////////////////////////////////////////////////////////////////////
//
//		功能: 解开套接字
//		参数: 
//				pBSD				--	BufSocketData结构指针
//				bCloseSocket		--	是否在解开套接字同时关闭套接字(缺省为FALSE)
//		返回: 
//				若bCloseSocket为FALSE,返回解开的套接字
//				若bCloseSocket为TRUE ,返回INVALID_SOCKET
//
///////////////////////////////////////////////////////////////////////////
SOCKET TE_BSocketDetach(PBSD pBSD, BOOL bCloseSocket /* = FALSE */ )
{
	SOCKET		 hSocket = pBSD->hSocket;

	// 释放内存
	free(pBSD->pszBuffer);
	free(pBSD);

	// 是否关闭套接字
	if (bCloseSocket)
	{
		TE_CloseSocket(hSocket);
		return (INVALID_SOCKET);
	}
	return (hSocket);
}


////////////////////////////////////////////////////////////////////
//
//		功能: 从缓冲区读取数据
//		参数: 
//				pBSD		 -- BufSocketData结构指针
//				dwTimeOut	 -- 接收超时(缺省为TE_RECV_TIMEOUT)	
//		返回: 
//				 0	---	成功 (SOCKET_SUCCESS)
//				-1	---	失败 (SOCKET_ERROR)	
//
////////////////////////////////////////////////////////////////////
int TE_BSocketReadData(PBSD pBSD, DWORD dwTimeout /* = TE_RECV_TIMEOUT */ )
{
	// 计算缓冲区空闲区域
	int	iMaxRead = pBSD->iBufferSize - pBSD->iBytesInBuffer;
	char  *pszBuffer = (char *) malloc(iMaxRead + 1);

	if (pszBuffer == NULL)
		return (SOCKET_ERROR);
	
	// 清空
	ZeroMemory(pszBuffer,iMaxRead+1);

	// 接收数据
	int iReadedBytes = TE_RecvData(pBSD->hSocket, pszBuffer, iMaxRead, dwTimeout);

	if (iReadedBytes > 0)
	{
		int iHeadSize = Min(pBSD->iBufferSize - pBSD->iBufferIndex, iReadedBytes);

		if (iHeadSize > 0)
			memcpy(pBSD->pszBuffer + pBSD->iBufferIndex, pszBuffer, iHeadSize);

		pBSD->iBufferIndex += iHeadSize;

		if (pBSD->iBufferIndex == pBSD->iBufferSize)
			pBSD->iBufferIndex = 0;

		int iBackSize = iReadedBytes - iHeadSize;

		if (iBackSize > 0)
			memcpy(pBSD->pszBuffer + pBSD->iBufferIndex, pszBuffer + iHeadSize, iBackSize);

		pBSD->iBufferIndex += iBackSize;
		pBSD->iBytesInBuffer += iReadedBytes;
	}

	free(pszBuffer);
	return (iReadedBytes);
}


////////////////////////////////////////////////////////////////////
//
//		功能: 从缓冲区读取一个字符
//		参数: 
//				pBSD		 -- BufSocketData结构指针
//				dwTimeOut -- 接收超时(缺省为TE_RECV_TIMEOUT)	
//		返回: 
//				一个字符(错误时返回TE_EOF)
//
////////////////////////////////////////////////////////////////////
int TE_BSocketGetChar(PBSD pBSD, DWORD dwTimeout /* = TE_RECV_TIMEOUT */ )
{
	if ((pBSD->iBytesInBuffer == 0) && (TE_BSocketReadData(pBSD, dwTimeout) <= 0))
		return (TE_EOF);

	int iChar =( (int) pBSD->pszBuffer[pBSD->iReadIndex]) & 0x000000ff;

	pBSD->iReadIndex = INext(pBSD->iReadIndex, pBSD->iBufferSize);

	--pBSD->iBytesInBuffer;

	return (iChar);
}


////////////////////////////////////////////////////////////////////////////
//
//		功能: 从缓冲区读取一行(注意，结尾的回车换行0x0D,0x0A并不包括在其中)
//		参数: 
//				pBSD		 -- BufSocketData结构指针
//				pszBuffer	 -- 接收缓冲区
//				iBufferSize	 -- 缓冲大小
//				iStatus		 -- 状态 (0:正常,1:缓冲不够,-1:错误)
//				dwTimeOut	 -- 接收超时(缺省为TE_RECV_TIMEOUT)	
//		返回: 
//				 0	---	成功 (SOCKET_SUCCESS)
//				-1	---	失败 (SOCKET_ERROR)	
//
////////////////////////////////////////////////////////////////////////////
int TE_BSocketGetString(PBSD pBSD, char *pszBuffer, int iBufferSize, int* iStatus, DWORD dwTimeout /* = TE_RECV_TIMEOUT */ )
{
	
	*iStatus = 1;		//缓冲长度不足

	int ii,iChar;
	for (ii = 0; ii < (iBufferSize - 1);)
	{
		iChar = TE_BSocketGetChar(pBSD, dwTimeout);
		if (iChar == TE_EOF)
		{
			*iStatus = (-1) ;
			return SOCKET_ERROR;
		}

		if (iChar == 0x0D)	// 回车符号
		{
			iChar = TE_BSocketGetChar(pBSD, dwTimeout);
			if (iChar == TE_EOF)
			{
				*iStatus = (-1);
				return (SOCKET_ERROR);
			}

			if (iChar == 0x0A)	// 换行
			{
				*iStatus = 0;	//正常情况
				break;
			}
			else
				pszBuffer[ii++] = 0x0D;
		}
		else
			pszBuffer[ii++] = (char) iChar;
	}
	pszBuffer[ii] = '\0';

	return (SOCKET_SUCCESS);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//		功能: 从缓冲区读取一行(包括单换行0x0A)(注意，结尾的回车换行不包括在其中)
//		参数: 
//				pBSD		-- BufSocketData结构指针
//				pszBuffer	 -- 接收缓冲区
//				iBufferSize	 -- 缓冲大小
//				iStatus		-- 状态 (0:正常,1:缓冲不够,-1:错误)
//				dwTimeOut	-- 接收超时(缺省为TE_RECV_TIMEOUT)	
//		返回: 
//				 0	---	成功 (SOCKET_SUCCESS)
//				-1	---	失败 (SOCKET_ERROR)	
//
//////////////////////////////////////////////////////////////////////////////////////////
int TE_BSocketGetStringEx(PBSD pBSD, char *pszBuffer, int iBufferSize, int* iStatus, DWORD dwTimeout /* = TE_RECV_TIMEOUT */ )
{
	
	*iStatus = 1;		//缓冲长度不足

	int ii,iChar;
	int LastChar = TE_EOF;
	for (ii = 0; ii < (iBufferSize - 1);)
	{
		iChar = TE_BSocketGetChar(pBSD, dwTimeout);

		if (iChar == TE_EOF)
		{
			*iStatus = (-1) ;
			return SOCKET_ERROR;
		}

		if (iChar == 0x0A)	// 换行符号
		{
			*iStatus = 0;	//正常情况
			if (LastChar == 0x0D)
				ii-- ;	
			break;
		}
		else
			pszBuffer[ii++] = (char) iChar;

		LastChar = iChar;
			
	}
	pszBuffer[ii] = '\0';

	return (SOCKET_SUCCESS);
}



////////////////////////////////////////////////////////////////////
//
//		功能: 发送一行 : 自动在最后加上“回车换行符(0x0D,0x0A)”
//		参数: 
//				pBSD		-- BufSocketData结构指针
//				pszBuffer	-- 待发送字符串
//				dwTimeOut	-- 发送超时(缺省为TE_SEND_TIMEOUT)	
//		返回: 
//				 0	---	成功 (SOCKET_SUCCESS)
//				-1	---	失败 (SOCKET_ERROR)	
//
////////////////////////////////////////////////////////////////////
int TE_BSocketSendString(PBSD pBSD, const char *pszBuffer, DWORD dwTimeout /* = TE_SEND_TIMEOUT */ )
{

	char *pszSendBuffer = (char *) malloc(strlen(pszBuffer) + 3);

	if (pszSendBuffer == NULL)
		return (SOCKET_ERROR);

	ZeroMemory( pszSendBuffer,strlen(pszBuffer) + 3);
	sprintf(pszSendBuffer, "%s\r\n", pszBuffer);

	int iSendLength = strlen(pszSendBuffer);
	if (TE_Send(pBSD->hSocket, pszSendBuffer, iSendLength, dwTimeout) != iSendLength)
	{
        free(pszSendBuffer);
		return (SOCKET_ERROR);
    }
	free(pszSendBuffer);
	return (iSendLength);
}


////////////////////////////////////////////////////////////////////
//
//		功能: 获取套接字
//		参数: 
//				pBSD -- BufSocketData结构指针
//		返回: 
//				与此结构绑定在一起的SOCKET	
//
////////////////////////////////////////////////////////////////////
SOCKET TE_BSocketGetAttachedSocket(PBSD pBSD)
{
	return (pBSD->hSocket);
}

////////////////////////////////////////////////////////////////////
//
//		功能: 从缓冲区读取一定数量的数据
//		参数: 
//				pBSD		 -- BufSocketData结构指针
//				pszBuffer	 -- 接收缓冲区
//				iBufferSize	 -- 缓冲大小
//				dwTimeOut	 -- 接收超时(缺省为TE_RECV_TIMEOUT)	
//		返回: 
//				读的数据的字节数
////////////////////////////////////////////////////////////////////
int TE_BSocketGetData(PBSD pBSD, char *pszBuffer, int iBufferSize,DWORD dwTimeout /*= TE_RECV_TIMEOUT*/)
{
	int iReadBytes	 = 0;
	int iHeadSize,iBackSize;

	if ((pBSD->iBytesInBuffer == 0) && (TE_BSocketReadData(pBSD, dwTimeout) <= 0))
		return 0; 

	if( pBSD->iBytesInBuffer < iBufferSize ) // 数据不够多
	{
		iHeadSize = Min( pBSD->iBufferSize - pBSD->iReadIndex ,pBSD->iBytesInBuffer );
		memcpy(pszBuffer+iReadBytes,pBSD->pszBuffer+pBSD->iReadIndex,iHeadSize);
			
		pBSD->iReadIndex += iHeadSize;
		if( pBSD->iReadIndex == pBSD->iBufferSize )
			pBSD->iReadIndex = 0;

		iReadBytes += iHeadSize;
		iBackSize = pBSD->iBytesInBuffer - iHeadSize;
			
		if( iBackSize > 0)
			memcpy(pszBuffer+iReadBytes,pBSD->pszBuffer+pBSD->iReadIndex,iBackSize);

		iReadBytes			 += iBackSize;
		pBSD->iReadIndex	 += iBackSize;
		pBSD->iBytesInBuffer = 0; // 数据全部读完
	}
	else // 这次的数据足够多了
	{
		iHeadSize = Min( pBSD->iBufferSize - pBSD->iReadIndex,iBufferSize - iReadBytes );
		memcpy(pszBuffer+iReadBytes,pBSD->pszBuffer+pBSD->iReadIndex,iHeadSize);
			
		pBSD->iReadIndex += iHeadSize;
		if( pBSD->iReadIndex == pBSD->iBufferSize )
			pBSD->iReadIndex = 0;

		iReadBytes += iHeadSize;
		iBackSize = iBufferSize - iReadBytes;

		if( iBackSize > 0)
			memcpy(pszBuffer+iReadBytes,pBSD->pszBuffer+pBSD->iReadIndex,iBackSize);

		iReadBytes			 += iBackSize;
		pBSD->iReadIndex	 += iBackSize;
		pBSD->iBytesInBuffer -= (iHeadSize+iBackSize);
	}
	return iReadBytes;
}
