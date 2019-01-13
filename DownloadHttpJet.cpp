////////////////////////////////////////////////////////////////////////////////
//	Copyright (C) 2000-2001 Softelf Inc. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
//
//	Author	: Telan 
//	Date	: 2000-10-04
//	Purpose	: Try to implement a Http Download Class that Support
//			  Resume Download,WWW Authorization,Proxy And Proxy
//			  Authorization,Redirect Support,Timeout Config,Retry
//			  Config,Notify Support,Job management.
//	TODO	: Job Management,Multi-Thread,Cookie Support
//	History	: 
//		1.0	: 2000-09-25 - Resume Download,Redirect Support,Proxy Support		
//		1.1	: 2000-09-26 - Timeout Config,Retry Config,Notify Support
//		1.2	: 2000-09-27 - WWW Authorization,Proxy Authorization
//		2.0 : 2000-10-04 - Change form using direct winsock to TE_Socket Functions
//						 - Add Socks-Proxy Support( socks4,socks4a,socks5 )
//						 - More Robust,More Extensible,More Wieldy 
//	Mailto	: telan@263.net ( Bugs' Report or Comments )
//	Notes	: This source code may be used in any form in any way you desire. It is
//			  provided "as is" without express or implied warranty.Use it at your own
//			  risk! The author accepts no liability for any damage/loss of business 
//			  that this product may cause.
//
////////////////////////////////////////////////////////////////////////////////
// DownloadHttpJet.cpp : implementation file
//

#include "stdafx.h"
#include "MyGet.h"
#include "TE_Socket.h"
#include "SocksPacket.h"	// Socks Proxy Support
#include "DownloadItemManager.h"
#include "DownloadHttpJet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDownloadItemManagerThread

//IMPLEMENT_DYNCREATE(CDownloadItemManager, CWinThread)


//DEL BOOL CDownloadItemManager::InitInstance()
//DEL {
//DEL 	// TODO:  perform and per-thread initialization here
//DEL 	UINT uiRetCode = StartDownload(m_strDownloadUrl);
//DEL 
//DEL 	switch(uiRetCode)
//DEL 	{
//DEL 	case DOWNLOAD_RESULT_FAIL:
//DEL 		prv_SendResultFailedMsg();
//DEL 		break;
//DEL 
//DEL 	case DOWNLOAD_RESULT_STOP:
//DEL 		prv_SendCancelMsg();
//DEL 		break;
//DEL 	
//DEL 	}
//DEL 
//DEL 	m_eventThreadStopped.SetEvent();
//DEL 
//DEL 	return TRUE;
//DEL }

//DEL int CDownloadItemManager::ExitInstance()
//DEL {
//DEL 	// TODO:  perform any per-thread cleanup here
//DEL 	return CWinThread::ExitInstance();
//DEL }



// 用于BASE64编码、解码的常量
CString CDownloadHttpJet::m_strBase64TAB = _T( "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/" );
UINT	CDownloadHttpJet::m_nBase64Mask[]= { 0, 1, 3, 7, 15, 31, 63, 127, 255 };

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDownloadHttpJet::CDownloadHttpJet(CDownloadItemManager *pParent)
{
	m_pParent = pParent;

	m_strDownloadUrl	= _T("");

	// 停止下载
	m_bStopDownload		= FALSE;

	// 强制重新下载(不管已有的文件是否与远程文件相同)
	m_bForceDownload	= FALSE;

	// 是否支持断点续传(假定不支持)
	m_bSupportResume	= FALSE;


	m_dwHeaderSize		= 0;		// HTTP协议头的长度
	m_strHeader			= _T("");	// HTTP协议头

	// Referer
	m_strReferer		= _T("");
	
	// UserAgent
	m_strUserAgent		= _T("HttpDownload/2.0");

	// 超时TIMEOUT	连接超时、发送超时、接收超时(单位：毫秒)
	m_dwConnectTimeout	= DOWNLOAD_CONNECT_TIMEOUT;	
	m_dwReceiveTimeout	= DOWNLOAD_RECV_TIMEOUT;
	m_dwSendTimeout		= DOWNLOAD_SEND_TIMEOUT;

	// 重试机制
	m_nRetryType		= RETRY_TYPE_NONE;	//重试类型(0:不重试 1:重试一定次数 2:总是重试)
	m_nRetryTimes		= 0;				//重试次数
	m_nRetryDelay		= 0;				//重试延迟(单位：毫秒)
	m_nRetryMax			= 0;				//重试最大次数

	// 错误处理
	m_nErrorCount		= 0;			//错误次数
	m_strError			= _T("");		//错误信息


	// 是否进行验证 : Request-Header: Authorization
	m_bAuthorization	= FALSE;
	m_strUsername		= _T("");
	m_strPassword		= _T("");

	// 是否使用代理 
	m_bProxy			= FALSE;
	m_strProxyServer	= _T("");
	m_nProxyPort		= 0;
	m_nProxyType		= PROXY_NONE;
	
	// 代理是否需要验证: Request-Header: Proxy-Authorization
	m_bProxyAuthorization = FALSE;
	m_strProxyUsername 	= _T("");
	m_strProxyPassword	= _T("");

	// 下载过程中所用的变量
	m_strServer			= _T("");
	m_strObject			= _T("");
	//m_strFileName		= _T("");
	m_nPort				= DEFAULT_HTTP_PORT ;
	
	// SOCKET 和 BufSocket
	m_hSocket			= INVALID_SOCKET;

	m_pBrkPos			= NULL;
		
	m_JetStatus			= CDownloadHttpJet::none;
}

// 析构函数
CDownloadHttpJet::~CDownloadHttpJet()
{
//	prv_CloseSocket();
}


// 创建SOCKET
//DEL BOOL CDownloadHttpJet::prv_CreateSocket()
//DEL {
//DEL 	prv_CloseSocket();
//DEL 
//DEL 	m_hSocket = TE_CreateSocket(AF_INET,SOCK_STREAM,0);
//DEL 	if (m_hSocket == INVALID_SOCKET)
//DEL 		return FALSE;
//DEL 	
//DEL 	m_pBSD = TE_BSocketAttach(m_hSocket, CDownloadJet::download_buffer_size);
//DEL 	if( m_pBSD == NULL )
//DEL 		return FALSE;
//DEL 
//DEL 	return TRUE;
//DEL }

// 关闭SOCKET
//DEL void CDownloadHttpJet::prv_CloseSocket()
//DEL {
//DEL 	if( m_pBSD != NULL )
//DEL 	{
//DEL 		TE_BSocketDetach(m_pBSD,FALSE);
//DEL 		m_pBSD = NULL;
//DEL 	}
//DEL 	
//DEL 	if (m_hSocket != INVALID_SOCKET)
//DEL 	{
//DEL 		TE_CloseSocket(m_hSocket,TRUE);
//DEL 		m_hSocket = INVALID_SOCKET;
//DEL 	}
//DEL }


// 下载入口
enum CDownloadHttpJet::JET_STATUS CDownloadHttpJet::prv_StartDownload()
{
	m_bStopDownload	  = FALSE;
	m_nRetryTimes	  = 0;

	// 检验要下载的URL是否为空
	m_strDownloadUrl.TrimLeft();
	m_strDownloadUrl.TrimRight();
	if( m_strDownloadUrl.IsEmpty() )
	{
		//return DOWNLOAD_RESULT_FAIL;
		return CDownloadHttpJet::close;
	}

	// 检验要下载的URL是否有效
	if ( !ParseURL(m_strDownloadUrl, m_strServer, m_strObject, m_nPort))
	{
		// 在前面加上"http://"再试
		m_strDownloadUrl = _T("http://") + m_strDownloadUrl;
		if ( !ParseURL(m_strDownloadUrl,m_strServer, m_strObject, m_nPort) )
		{
			TRACE(_T("Failed to parse the URL: %s\n"), m_strDownloadUrl);

			//return DOWNLOAD_RESULT_FAIL;
			return CDownloadHttpJet::close;
		}
	}


	BOOL bSendOnce = TRUE;		// 用于控制向hWndNotify窗口发送消息
	
ReDownload:

	UINT nRequestRet = prv_SendRequest( FALSE ) ;
	switch(nRequestRet)
	{
	case SENDREQUEST_SUCCESS:
		break;
	case SENDREQUEST_STOP:
		//return DOWNLOAD_RESULT_STOP;
		return CDownloadHttpJet::close;
		break;
	case SENDREQUEST_FAIL:
		//return DOWNLOAD_RESULT_FAIL;
		//return DOWNLOAD_RESULT_FAIL;
		//break;
	case SENDREQUEST_ERROR:

		switch( m_nRetryType )
		{
		case RETRY_TYPE_NONE:
//			return DOWNLOAD_RESULT_FAIL;
			return CDownloadHttpJet::close;
			break;
		case RETRY_TYPE_ALWAYS:
			if( m_nRetryDelay > 0 )
				Sleep(m_nRetryDelay);
			goto ReDownload;
			break;
		case RETRY_TYPE_TIMES:
			if( m_nRetryTimes > m_nRetryMax )
			{
			//	return DOWNLOAD_RESULT_FAIL;
				return CDownloadHttpJet::close;
			}
			m_nRetryTimes++;
		
			if( m_nRetryDelay > 0 )
				Sleep( m_nRetryDelay );
			goto ReDownload;
			break;
		default:
			//return DOWNLOAD_RESULT_FAIL;
			return CDownloadHttpJet::close;
			break;
		}
		break;
	default:
		//return DOWNLOAD_RESULT_FAIL;
		return CDownloadHttpJet::close;
		break;
	}

	if (m_dwContentLength == 0 /*|| m_dwHeaderSize == 0*/)
	{
	//	return DOWNLOAD_RESULT_FAIL;
		return CDownloadHttpJet::close;
	}

	if( bSendOnce)
	{
		//Login Successfully.
		SendStartDownloadMsg();
		bSendOnce = FALSE;
	}

	
	DWORD dwRequestDownloadSize;

	// 现在开始读取数据
	do
	{
	
		dwRequestDownloadSize = m_pBrkPos->iEnd - m_pBrkPos->iStart;
		if (dwRequestDownloadSize > CDownloadJet::download_buffer_size)
		{
			dwRequestDownloadSize = CDownloadJet::download_buffer_size;
		}

		ZeroMemory(m_szReadBuf, CDownloadJet::download_buffer_size+1);
//		m_iDownloadedBufSize = TE_BSocketGetData(m_pBSD, m_szReadBuf, dwRequestDownloadSize, m_dwReceiveTimeout);
		if (m_iDownloadedBufSize <= 0)
		{
			//Send Failed Message;
			SendFailedMsg();
			m_nErrorCount++;
			goto ReDownload; //再次发送请求
		}

		m_pBrkPos->iStart += m_iDownloadedBufSize;

		//下载速度瓶颈就在于发送RecvOK消息的等待消息这里，将来可以考虑不用等待Event的置位。
		SendRecvOKMsg();

	}while(m_pBrkPos->iStart < m_pBrkPos->iEnd - 1);


	//关闭SOCKET
//	prv_CloseSocket();

	//本线程的本次下载任务已经结束，发送结束消息。
	SendCompletedMsg();

	/*
	if (m_bStopDownload)
	{
		return DOWNLOAD_RESULT_STOP;
	}
*/
	//如果重新分配了下在区域，则跳转到Redownload;
	if (m_pBrkPos && m_pBrkPos->iStart < m_pBrkPos->iEnd)
	{
		//检查一下，下载区域是否被重新设定。
		goto ReDownload; //再次发送请求
	}
	
	// 不再进行其他操作
	//m_bStopDownload = TRUE;
	///return DOWNLOAD_RESULT_SUCCESS;
	return CDownloadHttpJet::close;
}
	
/*
// 发送请求
// 重定向的时候要加上Referer
UINT CDownloadHttpJet::prv_SendRequest(BOOL bHead )
{
	CString strVerb;
	if( bHead )
		strVerb = _T("HEAD ");
	else
		strVerb = _T("GET ");

	while (TRUE)
	{
		CString			strSend,strAuth,strAddr,strHeader;
		BYTE			bAuth,bAtyp;
		DWORD			dwIP;
		SOCKSREPPACKET	pack;
	
		int				iStatus,nRet;;
		char			szReadBuf[1025];
		DWORD			dwStatusCode;


		///////////////////////////////////////
		// 目前的版本中，此信息并没有用
		m_strHeader		= _T("");
		m_dwHeaderSize	= 0;
		//////////////////////////////////////
		if (!CreateSocket())
			return SENDREQUEST_FAIL;
	
		//send first command by name
		m_strHeader.Format("%s%s:%d\r\n", "Connect to ", m_strServer, m_nPort);
		SendHeaderInfoMsg(COMMAND_MSG);

		if (m_bStopDownload)
			return SENDREQUEST_STOP;

		switch( m_nProxyType )
		{
		case PROXY_NONE:
			if( TE_ConnectEx(m_hSocket,m_strServer,m_nPort,m_dwConnectTimeout,TRUE) == SOCKET_ERROR )
				return SENDREQUEST_ERROR;
			break;
		case PROXY_HTTPGET:
			if( TE_ConnectEx(m_hSocket,m_strProxyServer,m_nProxyPort,m_dwConnectTimeout,TRUE) == SOCKET_ERROR )
				return SENDREQUEST_ERROR;
			break;
		case PROXY_SOCKS4A:
			dwIP = TE_GetIP(m_strServer,TRUE);
			if( dwIP == INADDR_NONE )
			{
				if( TE_ConnectEx(m_hSocket,m_strProxyServer,m_nProxyPort,m_dwConnectTimeout,TRUE) == SOCKET_ERROR )
					return SENDREQUEST_ERROR;
				
				if( SOP_SendSocks4aReq(m_hSocket,CMD_CONNECT,m_nPort,m_strServer,m_strProxyUsername,m_dwSendTimeout) == SOCKET_ERROR )
					return SENDREQUEST_ERROR;

				ZeroMemory(&pack,sizeof(SOCKSREPPACKET));
				if( SOP_RecvPacket(m_pBSD,&pack,PACKET_SOCKS4AREP,m_dwReceiveTimeout) == SOCKET_ERROR )
					return SENDREQUEST_ERROR;

				if( !SOP_IsSocksOK(&pack,PACKET_SOCKS4AREP) )
					return SENDREQUEST_ERROR;

				break;// NOTICE:如果本地能够解析域名，可以使用SOCKS4 Proxy
			}
		case PROXY_SOCKS4:
			// 必须要得到Proxy Server的IP地址(不能为域名)
			dwIP = TE_GetIP(m_strServer,TRUE);
			if( dwIP == INADDR_NONE )
				return SENDREQUEST_ERROR;
			if( TE_ConnectEx(m_hSocket,m_strProxyServer,m_nProxyPort,m_dwConnectTimeout,TRUE) == SOCKET_ERROR )
				return SENDREQUEST_ERROR;
			if( SOP_SendSocks4Req(m_hSocket,CMD_CONNECT,m_nPort,dwIP,m_strProxyUsername,m_dwSendTimeout) == SOCKET_ERROR )
				return SENDREQUEST_ERROR;
		
			ZeroMemory(&pack,sizeof(SOCKSREPPACKET));
			if( SOP_RecvPacket(m_pBSD,&pack,PACKET_SOCKS4REP,m_dwReceiveTimeout) == SOCKET_ERROR )
				return SENDREQUEST_ERROR;
			
			if( !SOP_IsSocksOK(&pack,PACKET_SOCKS4REP) )
				return SENDREQUEST_ERROR;
			break;
		case PROXY_SOCKS5:
			if( TE_ConnectEx(m_hSocket,m_strProxyServer,m_nProxyPort,m_dwConnectTimeout,TRUE) == SOCKET_ERROR )
				return SENDREQUEST_ERROR;

			if( m_bProxyAuthorization )
			{
				strAuth =  _T("");
				char c	= (char)AUTH_NONE;
				strAuth += c;
				c 		= (char)AUTH_PASSWD;
				strAuth += c;
				
			}
			else
			{
				char c	= (char)AUTH_NONE;
				strAuth =  _T("");
				strAuth += c;
			}
			bAuth =(BYTE)strAuth.GetLength();
			
			if( SOP_SendSocks5AuthReq(m_hSocket,bAuth,(LPCTSTR)strAuth,m_dwSendTimeout) == SOCKET_ERROR )
				return SENDREQUEST_ERROR;

			ZeroMemory(&pack,sizeof(SOCKSREPPACKET));
			if( SOP_RecvPacket(m_pBSD,&pack,PACKET_SOCKS5AUTHREP,m_dwReceiveTimeout) == SOCKET_ERROR )
				return SENDREQUEST_ERROR;

			if( !SOP_IsSocksOK(&pack,PACKET_SOCKS5AUTHREP) )
				return SENDREQUEST_ERROR;

			switch( pack.socks5AuthRep.bAuth )
			{
			case AUTH_NONE:
				break;
			case AUTH_PASSWD:
				if( !m_bProxyAuthorization )
					return SENDREQUEST_FAIL;
				if( SOP_SendSocks5AuthPasswdReq(m_hSocket,m_strProxyUsername,m_strProxyPassword,m_dwSendTimeout) == SOCKET_ERROR )
					return SENDREQUEST_ERROR;

				ZeroMemory(&pack,sizeof(SOCKSREPPACKET));
				if( SOP_RecvPacket(m_pBSD,&pack,PACKET_SOCKS5AUTHPASSWDREP,m_dwReceiveTimeout) == SOCKET_ERROR )
					return SENDREQUEST_ERROR;

				if( !SOP_IsSocksOK(&pack,PACKET_SOCKS5AUTHPASSWDREP) )
					return SENDREQUEST_ERROR;
				break;
			case AUTH_GSSAPI:
			case AUTH_CHAP:
			case AUTH_UNKNOWN:
			default:
				return SENDREQUEST_FAIL;
				break;
			}

			dwIP = TE_GetIP(m_strServer,TRUE);
			if( dwIP != INADDR_NONE )
			{
				bAtyp = ATYP_IPV4ADDR;
				strAddr = _T("");
				// 转换字节序
				dwIP = htonl(dwIP);
				strAddr += (char)( (dwIP>>24) &0x000000ff); 
				strAddr += (char)( (dwIP>>16) &0x000000ff); 
				strAddr += (char)( (dwIP>>8 ) &0x000000ff); 
				strAddr += (char)( dwIP &0x000000ff); 

			}
			else
			{
				bAtyp = ATYP_HOSTNAME;
				char c = (char)m_strServer.GetLength();
				strAddr  = _T("");
				strAddr += c;
				strAddr += m_strServer;
			}
			if( SOP_SendSocks5Req(m_hSocket,CMD_CONNECT,bAtyp,(LPCTSTR)strAddr,m_nPort,m_dwSendTimeout) == SOCKET_ERROR )
				return SENDREQUEST_ERROR;

			ZeroMemory(&pack,sizeof(SOCKSREPPACKET));
			if( SOP_RecvPacket(m_pBSD,&pack,PACKET_SOCKS5REP,m_dwReceiveTimeout) == SOCKET_ERROR )
				return SENDREQUEST_ERROR;

			if( !SOP_IsSocksOK(&pack,PACKET_SOCKS5REP) )
				return SENDREQUEST_ERROR;

			break;
		case PROXY_HTTPCONNECT:
		default:
			return SENDREQUEST_FAIL;
			break;
		}

		//Send "Connected" msg;
		m_strHeader = "Connected.\r\n";
		SendHeaderInfoMsg(STATE_MSG);

		if (m_bStopDownload)
			return SENDREQUEST_STOP;
	
		if( m_nProxyType == PROXY_HTTPGET )
		{
			strSend  = strVerb  + m_strDownloadUrl + " HTTP/1.1\r\n";
			if( m_bProxyAuthorization )
			{
				strAuth = _T("");
				Base64Encode(m_strProxyUsername+":"+m_strProxyPassword,strAuth);
				strSend += "Proxy-Authorization: Basic "+strAuth+"\r\n";
			}
		}
		else	// No Proxy or not a HTTP_GET Proxy
			strSend  = strVerb  + m_strObject + " HTTP/1.1\r\n";
		
		if( m_bAuthorization )
		{
			strAuth = _T("");
			Base64Encode(m_strUsername+":"+m_strPassword,strAuth);
			strSend += "Authorization: Basic "+strAuth+"\r\n";
		}

		strSend += "Host: " + m_strServer + "\r\n";
		strSend += "Accept: *\r\n";
		strSend += "Pragma: no-cache\r\n"; 
		strSend += "Cache-Control: no-cache\r\n";
		strSend += "User-Agent: "+m_strUserAgent+"\r\n";
		if( !m_strReferer.IsEmpty() )
			strSend += "Referer: "+m_strReferer+"\r\n";
		strSend += "Connection: close\r\n";

		// 查看开始下载位置
		CString		strRange;
		strRange.Empty();

		//if (m_pBrkPos->iStart > 0)
		{
			strRange.Format(_T("Range: bytes=%d-\r\n"), m_pBrkPos->iStart );
		}

		strSend += strRange;
		//必须要加一个空行，否则Http服务器将不会应答
		strSend += "\r\n";

		//发送请求前，向屏幕输出发送的信息。
		m_strHeader = strSend;

		SendHeaderInfoMsg(COMMAND_MSG);

		//发送请求
		nRet = TE_Send(m_hSocket,(LPCTSTR)strSend,strSend.GetLength(),m_dwSendTimeout);
		if( nRet < strSend.GetLength() )
		{
			if ( TE_GetLastError() == WSAETIMEDOUT)	// 超时
				continue;
			else	// 其他错误，可能是网络断了，等待一段时间后重试
				return SENDREQUEST_ERROR;
		}

		if (m_bStopDownload)
			return SENDREQUEST_STOP;

		strHeader.Empty();
		while( TRUE )
		{
			ZeroMemory(szReadBuf,1025);
			if( TE_BSocketGetStringEx(m_pBSD,szReadBuf,1024,&iStatus,m_dwReceiveTimeout) == SOCKET_ERROR )
				return SENDREQUEST_ERROR;
			
			if( szReadBuf[0] == '\0' ) // We have encountered "\r\n\r\n"
				break; 

			strHeader += szReadBuf;
			if( iStatus == 0)
				strHeader += "\r\n";
		}
		
		///////////////////////////////////////
		// 目前的版本中，此信息并没有用
		// 但是被用来向屏幕输出信息
		m_strHeader		= strHeader;
		SendHeaderInfoMsg(SERVER_MSG);
		m_dwHeaderSize	= m_strHeader.GetLength();
		//////////////////////////////////////
				
		nRet = GetInfo(strHeader,m_dwContentLength,dwStatusCode,m_TimeLastModified);
		switch ( nRet )
		{
		case HTTP_FAIL:
			return SENDREQUEST_FAIL;
			break;
		case HTTP_ERROR:
			return SENDREQUEST_ERROR;
			break;
		case HTTP_REDIRECT:
			continue;
			break;
		case HTTP_OK:
			// 应该判断一下服务器是否支持断点续传
			if(!strRange.IsEmpty())
			{
				if ( dwStatusCode == 206 )	//支持断点续传
				{
					m_bSupportResume = TRUE;
				}
				else						//不支持断点续传
				{
					m_bSupportResume = FALSE;
					m_pBrkPos->iStart	= 0; //不支持断点续传，此值要设为0
				}
			}
			return SENDREQUEST_SUCCESS;
			break;
		default:
			return SENDREQUEST_FAIL;
			break;
		}

	}// WHILE LOOP

	return SENDREQUEST_SUCCESS;
}

*/

// 从URL里面拆分出Server和Object来
BOOL CDownloadHttpJet::ParseURL(LPCTSTR lpszURL, CString &strServer, CString &strObject,USHORT& nPort)
{
	CString strURL(lpszURL);
	strURL.TrimLeft();
	strURL.TrimRight();
	
	// 清除数据
	strServer = _T("");
	strObject = _T("");
	nPort	  = 0;

	int nPos = strURL.Find("://");
	if( nPos == -1 )
		return FALSE;

	// 进一步验证是否为http://
	CString strTemp = strURL.Left( nPos+lstrlen("://") );
	strTemp.MakeLower();
	if( strTemp.Compare("http://") != 0 )
		return FALSE;

	strURL = strURL.Mid( strTemp.GetLength() );
	nPos = strURL.Find('/');
	if ( nPos == -1 )
		return FALSE;

	strObject = strURL.Mid(nPos);
	strTemp   = strURL.Left(nPos);
	
	///////////////////////////////////////////////////////////////
	/// 注意：并没有考虑URL中有用户名和口令的情形和最后有#的情形
	/// 例如：http://abc@def:www.yahoo.com:81/index.html#link1
	/// 
	//////////////////////////////////////////////////////////////

	// 查找是否有端口号
	nPos = strTemp.Find(":");
	if( nPos == -1 )
	{
		strServer = strTemp;
		nPort	  = DEFAULT_HTTP_PORT;
	}
	else
	{
		strServer = strTemp.Left( nPos );
		strTemp	  = strTemp.Mid( nPos+1 );
		nPort	  = (USHORT)_ttoi((LPCTSTR)strTemp);
	}
	return TRUE;
}

// 从返回到头里获得必要的信息
UINT CDownloadHttpJet::prv_GetInfo(LPCTSTR lpszHeader, DWORD &dwContentLength, DWORD &dwStatusCode, CTime &TimeLastModified)
{
	dwContentLength = 0;
	dwStatusCode	= 0;
	TimeLastModified= CTime::GetCurrentTime();

	CString strHeader = lpszHeader;
	strHeader.MakeLower();

	//拆分出HTTP应答的头信息的第一行
	int nPos = strHeader.Find("\r\n");
	if (nPos == -1)
		return HTTP_FAIL;
	CString strFirstLine = strHeader.Left(nPos);

	// 获得返回码: Status Code
	strFirstLine.TrimLeft();
	strFirstLine.TrimRight();
	nPos = strFirstLine.Find(' ');
	if( nPos == -1 )
		return HTTP_FAIL;
	strFirstLine = strFirstLine.Mid(nPos+1);
	nPos = strFirstLine.Find(' ');
	if( nPos == -1 )
		return HTTP_FAIL;
	strFirstLine = strFirstLine.Left(nPos);
	dwStatusCode = (DWORD)_ttoi((LPCTSTR)strFirstLine);
	
	// 检查返回码
	if( dwStatusCode >= 300 && dwStatusCode < 400 ) //首先检测一下服务器的应答是否为重定向
	{
		nPos = strHeader.Find("location:");
		if (nPos == -1)
			return HTTP_FAIL;

		CString strRedirectFileName = strHeader.Mid(nPos + strlen("location:"));
		nPos = strRedirectFileName.Find("\r\n");
		if (nPos == -1)
			return HTTP_FAIL;

		strRedirectFileName = strRedirectFileName.Left(nPos);
		strRedirectFileName.TrimLeft();
		strRedirectFileName.TrimRight();
		
		// 设置Referer
		m_strReferer = m_strDownloadUrl;

		// 判断是否重定向到其他的服务器
		nPos = strRedirectFileName.Find("http://");
		if( nPos != -1 )
		{
			m_strDownloadUrl = strRedirectFileName;
			// 检验要下载的URL是否有效
			if ( !ParseURL(m_strDownloadUrl, m_strServer, m_strObject, m_nPort))
				return HTTP_FAIL;
			return HTTP_REDIRECT;
		}

		// 重定向到本服务器的其他地方
		strRedirectFileName.Replace("\\","/");
		
		// 是相对于根目录
		if( strRedirectFileName[0] == '/' )
		{
			m_strObject = strRedirectFileName;
			return HTTP_REDIRECT;
		}
		
		// 是相对当前目录
		int nParentDirCount = 0;
		nPos = strRedirectFileName.Find("../");
		while (nPos != -1)
		{
			strRedirectFileName = strRedirectFileName.Mid(nPos+3);
			nParentDirCount++;
			nPos = strRedirectFileName.Find("../");
		}
		for (int i=0; i<=nParentDirCount; i++)
		{
			nPos = m_strDownloadUrl.ReverseFind('/');
			if (nPos != -1)
				m_strDownloadUrl = m_strDownloadUrl.Left(nPos);
		}
		m_strDownloadUrl = m_strDownloadUrl+"/"+strRedirectFileName;

		if ( !ParseURL(m_strDownloadUrl, m_strServer, m_strObject, m_nPort))
			return HTTP_FAIL;
		return HTTP_REDIRECT;
	}

	// 服务器错误，可以重试
	if( dwStatusCode >=500 )
		return HTTP_ERROR;

	// 客户端错误，重试无用
	if( dwStatusCode >=400 && dwStatusCode <500 )
		return HTTP_FAIL;
		
	// 获取ContentLength
	nPos = strHeader.Find("content-length:");
	if (nPos == -1)
		return HTTP_FAIL;

	CString strDownFileLen = strHeader.Mid(nPos + strlen("content-length:"));	
	nPos = strDownFileLen.Find("\r\n");
	if (nPos == -1)
		return HTTP_FAIL;

	strDownFileLen = strDownFileLen.Left(nPos);	
	strDownFileLen.TrimLeft();
	strDownFileLen.TrimRight();

	// Content-Length:
	dwContentLength = (DWORD) _ttoi( (LPCTSTR)strDownFileLen );

	// 获取Last-Modified:
	nPos = strHeader.Find("last-modified:");
	if (nPos != -1)
	{
		CString strTime = strHeader.Mid(nPos + strlen("last-modified:"));
		nPos = strTime.Find("\r\n");
		if (nPos != -1)
		{
			strTime = strTime.Left(nPos);
			strTime.TrimLeft();
			strTime.TrimRight();
			TimeLastModified = prv_GetTime(strTime);
		}
	}
	return HTTP_OK;
}

// 将字符串转化成时间
CTime CDownloadHttpJet::prv_GetTime(LPCTSTR lpszTime)
{
	int nDay,nMonth,nYear,nHour,nMinute,nSecond;

	CString strTime = lpszTime;
	int nPos = strTime.Find(',');
	if (nPos != -1)
	{
		strTime = strTime.Mid(nPos+1);
		strTime.TrimLeft();

		CString strDay,strMonth,strYear,strHour,strMinute,strSecond;
		CString strAllMonth = "jan,feb,mar,apr,may,jan,jul,aug,sep,oct,nov,dec";
		strDay = strTime.Left(2);
		nDay = atoi(strDay);
		strMonth = strTime.Mid(3,3);
		strMonth.MakeLower();
		nPos = strAllMonth.Find(strMonth);
		if (nPos != -1)
		{
			strMonth.Format("%d",((nPos/4)+1));
			nMonth = atoi(strMonth);
		}
		else
			nMonth = 1;
		strTime = strTime.Mid(6);
		strTime.TrimLeft();
		nPos = strTime.FindOneOf(" \t");
		if (nPos != -1)
		{
			strYear = strTime.Left(nPos);
			nYear = atoi(strYear);
		}
		else
			nYear = 2000;
		strTime = strTime.Mid(nPos+1);
		strHour = strTime.Left(2);
		nHour = atoi(strHour);
		strMinute = strTime.Mid(3,2);
		nMinute = atoi(strMinute);
		strSecond = strTime.Mid(6,2);
		nSecond = atoi(strSecond);
	}
	
	CTime time(nYear,nMonth,nDay,nHour,nMinute,nSecond);
	return time;
}



// STATIC BASE64解码
int CDownloadHttpJet::Base64Decode(LPCTSTR lpszDecoding, CString &strDecoded)
{
	int nIndex =0;
	int nDigit;
    int nDecode[ 256 ];
	int nSize;
	int nNumBits = 6;
	int i = 0;

	if( lpszDecoding == NULL )
		return 0;
	
	if( ( nSize = lstrlen(lpszDecoding) ) == 0 )
		return 0;

	// Build Decode Table
	for( int i = 0; i < 256; i++ ) 
		nDecode[i] = -2; // Illegal digit
	for( i=0; i < 64; i++ )
	{
		nDecode[ m_strBase64TAB[ i ] ] = i;
		nDecode[ '=' ] = -1; 
    }

	// Clear the output buffer
	strDecoded = _T("");
	long lBitsStorage  =0;
	int nBitsRemaining = 0;
	int nScratch = 0;
	UCHAR c;
	
	// Decode the Input
	for( nIndex = 0, i = 0; nIndex < nSize; nIndex++ )
	{
		c = lpszDecoding[ nIndex ];

		// 忽略所有不合法的字符
		if( c> 0x7F)
			continue;

		nDigit = nDecode[c];
		if( nDigit >= 0 ) 
		{
			lBitsStorage = (lBitsStorage << nNumBits) | (nDigit & 0x3F);
			nBitsRemaining += nNumBits;
			while( nBitsRemaining > 7 ) 
			{
				nScratch = lBitsStorage >> (nBitsRemaining - 8);
				i++;
				nBitsRemaining -= 8;
			}
		}
    }	

	return strDecoded.GetLength();


}

// STATIC BASE64编码
int CDownloadHttpJet::Base64Encode(LPCTSTR lpszEncoding, CString &strEncoded)
{
	int nDigit;
	int nNumBits = 6;
	int nIndex = 0;
	int nInputSize;

	strEncoded = _T( "" );
	if( lpszEncoding == NULL )
		return 0;

	if( ( nInputSize = lstrlen(lpszEncoding) ) == 0 )
		return 0;

	int nBitsRemaining = 0;
	long lBitsStorage	=0;
	long lScratch		=0;
	int nBits;
	UCHAR c;

	while( nNumBits > 0 )
	{
		while( ( nBitsRemaining < nNumBits ) &&  ( nIndex < nInputSize ) ) 
		{
			c = lpszEncoding[ nIndex++ ];
			lBitsStorage <<= 8;
			lBitsStorage |= (c & 0xff);
			nBitsRemaining += 8;
		}
		if( nBitsRemaining < nNumBits ) 
		{
			lScratch = lBitsStorage << ( nNumBits - nBitsRemaining );
			nBits    = nBitsRemaining;
			nBitsRemaining = 0;
		}	 
		else 
		{
			lScratch = lBitsStorage >> ( nBitsRemaining - nNumBits );
			nBits	 = nNumBits;
			nBitsRemaining -= nNumBits;
		}
		nDigit = (int)(lScratch & m_nBase64Mask[nNumBits]);
		nNumBits = nBits;
		if( nNumBits <=0 )
			break;
		
		strEncoded += m_strBase64TAB[ nDigit ];
	}
	// Pad with '=' as per RFC 1521
	while( strEncoded.GetLength() % 4 != 0 )
		strEncoded += '=';

	return strEncoded.GetLength();

}


/////////////////////////////////////////////////////////////////////////////
// CDownloadItemManagerThread message handlers




void CDownloadHttpJet::StartDownload()
{
	DownloadHandler(0, 0);
}


// 停止下载
void CDownloadHttpJet::StopDownload()
{
	m_bStopDownload = TRUE;

	//closesocket(m_hSocket);
	prv_StopDownload();
}


// 设置代理及代理认证方式
void CDownloadHttpJet::SetProxy(LPCTSTR lpszProxyServer, USHORT nProxyPort, BOOL bProxy, BOOL bProxyAuthorization, LPCTSTR lpszProxyUsername, LPCTSTR lpszProxyPassword,UINT nProxyType /*= PROXY_HTTPGET*/)
{
	if( bProxy && lpszProxyServer != NULL)
	{
		m_bProxy			= TRUE;
		m_strProxyServer	= lpszProxyServer;
		m_nProxyPort		= nProxyPort;
		m_nProxyType		= nProxyType;

		if( bProxyAuthorization && lpszProxyUsername != NULL)
		{
			m_bProxyAuthorization	= TRUE;
			m_strProxyUsername		= lpszProxyUsername;
			m_strProxyPassword		= lpszProxyPassword;
		}
		else
		{
			m_bProxyAuthorization	= FALSE;
			m_strProxyUsername		= _T("");
			m_strProxyPassword		= _T("");
		}
	}
	else
	{
		m_bProxy				= FALSE;
		m_bProxyAuthorization	= FALSE;
		m_nProxyPort			= 0;
		m_nProxyType			= PROXY_NONE;
		m_strProxyServer		= _T("");
		m_strProxyUsername		= _T("");
		m_strProxyPassword		= _T("");
	}
}

int CDownloadHttpJet::DownloadHandler(WPARAM wParam, LPARAM lParam)
{
	int iSetEvent = 0;

	if (wParam == 0 && lParam == 0)
	{
		m_JetStatus = prv_ConnectServer();
		iSetEvent = FD_CONNECT | FD_CLOSE;
		
	}
	else if (WSAGETSELECTERROR(lParam))
	{
		m_JetStatus = CDownloadHttpJet::close;
		iSetEvent = 0;
	}
	else
	{
		switch(WSAGETSELECTEVENT(lParam))
		{
		case FD_CONNECT:
			if (m_JetStatus == CDownloadHttpJet::connect)
			{
				m_JetStatus = prv_SendRequest();
			}
			else
			{
				m_JetStatus = CDownloadHttpJet::close;
			}

			break;
		case FD_READ:
			if (m_JetStatus == CDownloadHttpJet::sent_request)
			{
				m_JetStatus = prv_ReceiveHeader();
			}
			else if (m_JetStatus == CDownloadHttpJet::receive)
			{
				m_JetStatus = prv_ReceiveData();
			}
			else
			{
				m_JetStatus = CDownloadHttpJet::close;
			}

			break;

		case FD_CLOSE:
			m_JetStatus = CDownloadHttpJet::close;
			break;

		}
		
		if (m_JetStatus == CDownloadHttpJet::none)
		{
			return DownloadHandler(0, 0);
		}
		else if (m_JetStatus != CDownloadHttpJet::close)
		{
			iSetEvent = FD_READ | FD_CLOSE;
		}
		else
		{
			//应当结束本次Jet下载
			prv_StopDownload();
		}
	
	}


	return iSetEvent;
}

enum CDownloadHttpJet::JET_STATUS CDownloadHttpJet::prv_ConnectServer()
{
	// 检验要下载的URL是否为空
	m_strDownloadUrl.TrimLeft();
	m_strDownloadUrl.TrimRight();
	if( m_strDownloadUrl.IsEmpty() )
	{
		//return DOWNLOAD_RESULT_FAIL;
		return CDownloadHttpJet::close;
	}

	// 检验要下载的URL是否有效
	if ( !ParseURL(m_strDownloadUrl, m_strServer, m_strObject, m_nPort))
	{
		// 在前面加上"http://"再试
		m_strDownloadUrl = _T("http://") + m_strDownloadUrl;
		if ( !ParseURL(m_strDownloadUrl,m_strServer, m_strObject, m_nPort) )
		{
			TRACE(_T("Failed to parse the URL: %s\n"), m_strDownloadUrl);

			//return DOWNLOAD_RESULT_FAIL;
			return CDownloadHttpJet::close;
		}
	}

	//首先建立一个sokcet,然后加入到DownloadScheduler的异步select中。
	closesocket(m_hSocket);
	m_hSocket = socket(AF_INET,SOCK_STREAM,0);

	m_pParent->AddDownloadEntry(this, m_hSocket);

	//连接服务器
	m_strHeader		= _T("");
	m_dwHeaderSize	= 0;

	m_strHeader.Format("%s%s:%d\r\n", "Connect to ", m_strServer, m_nPort);
	SendHeaderInfoMsg(COMMAND_MSG);

	//根据不同的代理模式，分别进行连接
	switch( m_nProxyType )
	{
	case PROXY_NONE:
		prv_ConnectEx(m_hSocket, m_strServer, m_nPort, m_dwConnectTimeout, TRUE);
		break;
	//为了简单起见，目前暂时不支持代理模式。
	default:
		return CDownloadHttpJet::close;
		break;
	}


    return CDownloadHttpJet::connect;
}

enum CDownloadHttpJet::JET_STATUS CDownloadHttpJet::prv_SendRequest(BOOL bHead /* = FALSE */)
{
	CString strVerb;
	if( bHead )
		strVerb = _T("HEAD ");
	else
		strVerb = _T("GET ");

	CString			strSend,strAuth,strAddr,strHeader;

	///////////////////////////////////////
	// 目前的版本中，此信息并没有用
	m_strHeader		= _T("");
	m_dwHeaderSize	= 0;
	//////////////////////////////////////
	
	//Send "Connected" msg;
	m_strHeader = "Connected.\r\n";
	SendHeaderInfoMsg(STATE_MSG);

	strSend  = strVerb  + m_strObject + " HTTP/1.1\r\n";
	
	if( m_bAuthorization )
	{
		strAuth = _T("");
		Base64Encode(m_strUsername+":"+m_strPassword,strAuth);
		strSend += "Authorization: Basic "+strAuth+"\r\n";
	}

	strSend += "Host: " + m_strServer + "\r\n";
	strSend += "Accept: */*\r\n";
	strSend += "Pragma: no-cache\r\n"; 
	strSend += "Cache-Control: no-cache\r\n";
	strSend += "User-Agent: "+m_strUserAgent+"\r\n";

	if( !m_strReferer.IsEmpty() )
	{
		strSend += "Referer: "+m_strReferer+"\r\n";
	}

	strSend += "Connection: close\r\n";

	// 查看开始下载位置
	CString		strRange;
	strRange.Empty();

	//if (m_pBrkPos->iStart > 0)
	{
		strRange.Format(_T("Range: bytes=%d-\r\n"), m_pBrkPos->iStart );
	}

	strSend += strRange;
	//必须要加一个空行，否则Http服务器将不会应答
	strSend += "\r\n";

	//发送请求前，向屏幕输出发送的信息。
	m_strHeader = strSend;

	SendHeaderInfoMsg(COMMAND_MSG);

	//发送请求
	//nRet = TE_Send(m_hSocket,(LPCTSTR)strSend,strSend.GetLength(),m_dwSendTimeout);
	prv_Send(m_hSocket, (LPCTSTR)strSend, strSend.GetLength(), m_dwSendTimeout);
	
	/*
	if( nRet < strSend.GetLength() )
	{
		if ( TE_GetLastError() == WSAETIMEDOUT)	// 超时
			continue;
		else	// 其他错误，可能是网络断了，等待一段时间后重试
			return SENDREQUEST_ERROR;
	}

	if (m_bStopDownload)
		return SENDREQUEST_STOP;

	strHeader.Empty();
	while( TRUE )
	{
		ZeroMemory(szReadBuf,1025);
		if( TE_BSocketGetStringEx(m_pBSD,szReadBuf,1024,&iStatus,m_dwReceiveTimeout) == SOCKET_ERROR )
			return SENDREQUEST_ERROR;
		
		if( szReadBuf[0] == '\0' ) // We have encountered "\r\n\r\n"
			break; 

		strHeader += szReadBuf;
		if( iStatus == 0)
			strHeader += "\r\n";
	}
	
	///////////////////////////////////////
	// 目前的版本中，此信息并没有用
	// 但是被用来向屏幕输出信息
	m_strHeader		= strHeader;
	SendHeaderInfoMsg(SERVER_MSG);
	m_dwHeaderSize	= m_strHeader.GetLength();
	//////////////////////////////////////
			
	nRet = GetInfo(strHeader,m_dwContentLength,dwStatusCode,m_TimeLastModified);
	switch ( nRet )
	{
	case HTTP_FAIL:
		return SENDREQUEST_FAIL;
		break;
	case HTTP_ERROR:
		return SENDREQUEST_ERROR;
		break;
	case HTTP_REDIRECT:
		continue;
		break;
	case HTTP_OK:
		// 应该判断一下服务器是否支持断点续传
		if(!strRange.IsEmpty())
		{
			if ( dwStatusCode == 206 )	//支持断点续传
			{
				m_bSupportResume = TRUE;
			}
			else						//不支持断点续传
			{
				m_bSupportResume = FALSE;
				m_pBrkPos->iStart	= 0; //不支持断点续传，此值要设为0
			}
		}
		return SENDREQUEST_SUCCESS;
		break;
	default:
		return SENDREQUEST_FAIL;
		break;
	}

  */

	
	return CDownloadHttpJet::sent_request;
}

enum CDownloadHttpJet::JET_STATUS CDownloadHttpJet::prv_ReceiveHeader()
{
	m_strHeader.Empty();
	char szHeaderBuf[1024];
	while (TRUE)
	{
		int iStatus = 0;
		if (prv_GetHeader(m_hSocket, szHeaderBuf, sizeof(szHeaderBuf) - 1, &iStatus)
			== SOCKET_ERROR)
		{

		}

		if (szHeaderBuf[0] == '\0')// We have encountered "\r\n\r\n"
		{
			break;
		}

		m_strHeader += szHeaderBuf;

		if (iStatus == 0)
		{
			m_strHeader += "\r\n";
		}
	}

	SendHeaderInfoMsg(SERVER_MSG);
	DWORD dwStatusCode;
	int nRet = prv_GetInfo(m_strHeader,m_dwContentLength,dwStatusCode,m_TimeLastModified);

	//对返回只进行判断，察看文件长度，以及是否支持断点续传。
	switch ( nRet )
	{
	case HTTP_FAIL:
		//return SENDREQUEST_FAIL;
		return prv_Failed(SENDREQUEST_FAIL);
		break;
	case HTTP_ERROR:
		//return SENDREQUEST_ERROR;
		return prv_Failed(SENDREQUEST_ERROR);
		break;
	//case HTTP_REDIRECT:
	//	continue;
	//	break;
	case HTTP_OK:
		// 应该判断一下服务器是否支持断点续传
		if ( dwStatusCode == 206 )	//支持断点续传
		{
			m_bSupportResume = TRUE;
		}
		else						//不支持断点续传
		{
			m_bSupportResume = FALSE;
			m_pBrkPos->iStart	= 0; //不支持断点续传，此值要设为0
		}
		
		
		break;
	default:
		//return SENDREQUEST_FAIL;
		return prv_Failed(SENDREQUEST_FAIL);

		break;
	}

	if (SendStartDownloadMsg())
	{
		return prv_ReceiveData();
	}
	else
	{
		return CDownloadHttpJet::close;
	}
}

enum CDownloadHttpJet::JET_STATUS CDownloadHttpJet::prv_ReceiveData()
{
	// 现在开始读取数据
	if (m_pBrkPos->iStart < m_pBrkPos->iEnd - 1)
	{
		DWORD dwRequestDownloadSize;	

		dwRequestDownloadSize = m_pBrkPos->iEnd - m_pBrkPos->iStart;
		if (dwRequestDownloadSize > CDownloadJet::download_buffer_size)
		{
			dwRequestDownloadSize = CDownloadJet::download_buffer_size;
		}

		ZeroMemory(m_szReadBuf, CDownloadJet::download_buffer_size+1);
		//m_dwDownloadedBufSize = TE_BSocketGetData(m_pBSD, m_szReadBuf, dwRequestDownloadSize, m_dwReceiveTimeout);
		m_iDownloadedBufSize = recv(m_hSocket, m_szReadBuf, dwRequestDownloadSize, 0);
		if (m_iDownloadedBufSize > 0)
		{

			m_pBrkPos->iStart += m_iDownloadedBufSize;

			//下载速度瓶颈就在于发送RecvOK消息的等待消息这里，将来可以考虑不用等待Event的置位。
			SendRecvOKMsg();
		}


	};
	//while(m_pBrkPos->iStart < m_pBrkPos->iEnd - 1);

	if (m_pBrkPos->iStart >= m_pBrkPos->iEnd - 1)
	{
		SendCompletedMsg();

		if (m_pBrkPos && m_pBrkPos->iStart < m_pBrkPos->iEnd - 1)
		{//新的下载范围被分配
			prv_StopDownload();
			return CDownloadHttpJet::none;
		}
		else
		{
			return CDownloadHttpJet::close;
		}
	}

	return CDownloadHttpJet::receive;
}

// 建立连接(扩展函数)
int CDownloadHttpJet::prv_ConnectEx(SOCKET hSocket, char const * pszServer, int nPort,DWORD dwTimeout,BOOL fFixNtDNS /*= FALSE*/)
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

	WSAConnect(hSocket, (SOCKADDR *)&sockAddr, sizeof(sockAddr), NULL, NULL, NULL, NULL);
	
	m_pParent->RegisterTimeout(this, dwTimeout);

	return 0;
}

void CDownloadHttpJet::Timeout()
{

}

void CDownloadHttpJet::prv_UnregisterTimeout()
{
	m_pParent->UnregisterTimeout(this);
}

int CDownloadHttpJet::prv_Send(SOCKET hSocket, const char *pszBuffer, int iBufferSize, DWORD dwTimeout)
{
	send(hSocket, pszBuffer, iBufferSize, 0);

	m_pParent->RegisterTimeout(this, dwTimeout);
	return 0;
}


int CDownloadHttpJet::prv_GetHeader(SOCKET Socket, char *pszBuffer, int iBufferSize, int* iStatus)
{
	
	*iStatus = 1;		//缓冲长度不足

	int ii;
	char curChar;
	char lastChar = 0;
	for (ii = 0; ii < (iBufferSize - 1);)
	{
		//iChar = TE_BSocketGetChar(pBSD, dwTimeout);
		recv(Socket, &curChar, 1, 0);

		if (curChar == TE_EOF)
		{
			*iStatus = (-1) ;
			return SOCKET_ERROR;
		}

		if (curChar == 0x0A)	// 换行符号
		{
			*iStatus = 0;	//正常情况
			if (lastChar == 0x0D)
				ii-- ;	
			break;
		}
		else
			pszBuffer[ii++] = (char) curChar;

		lastChar = curChar;
			
	}
	pszBuffer[ii] = '\0';

	return (SOCKET_SUCCESS);
}

void CDownloadHttpJet::prv_StopDownload()
{
	closesocket(m_hSocket);
	prv_UnregisterTimeout();
	m_pParent->RemoveDownloadEntry(this);

	//delete this;
}


enum CDownloadHttpJet::JET_STATUS CDownloadHttpJet::prv_Failed(int iErrorCode)
{
	return CDownloadHttpJet::close;
}
