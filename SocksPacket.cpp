////////////////////////////////////////////////////////////////////////////////
//	Copyright (C) 2000-2001 Softelf Inc. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
//
//	Author	: Telan
//	Date	: 2000-10-05
//	Purpose	: Implement a SOCKS functions' lib that support socks4,socks4a,socks5
//	History	: 
//		1.0	: 2000-10-04 - Basic Functions Lib
//		1.1	: 2000-10-05 - Fix several bugs in Pack&UnPack
//
//	Mailto	: telan@263.net ( Bugs' Report or Comments )
//	Notes	: This source code may be used in any form in any way you desire. It is
//			  provided "as is" without express or implied warranty.Use it at your own
//			  risk! The author accepts no liability for any damage/loss of business 
//			  that this product may cause.
//
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "TE_Socket.h"
#include "SocksPacket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


////////////////////////////////////////////////////////////////////
//
//		功能: 接收并解包
//		参数: 
//				pBSD		-- BufSocketData数据结构
//				pPacket		-- 数据包指针(用来保存接收数据包)
//				nPacketType	-- 包类型
//				dwTimeOut	-- 接收超时(缺省为SOP_RECV_TIMEOUT)	
//		返回: 
//				 0	---	成功 (SOCKET_SUCCESS)
//				-1	---	失败 (SOCKET_ERROR)	
//
////////////////////////////////////////////////////////////////////
int SOP_RecvPacket(PBSD pBSD,PSOCKSREPPACKET pPacket,UINT nPacketType,DWORD dwTimeout /*= SOP_RECV_TIMEOUT*/)
{
	int		nRecv,n,nCount;
	WORD	wData	= 0;
	DWORD	dwData	= 0;
	char*	pszData	= NULL;

	//清空数据
	ZeroMemory(pPacket,sizeof(SOCKSREQPACKET));

	//接收数据
	switch( nPacketType )
	{
	case PACKET_SOCKS4REP:
		nRecv = TE_BSocketGetChar(pBSD,dwTimeout);
		if( nRecv == TE_EOF )
			return (SOCKET_ERROR);
		pPacket->socks4Rep.bVer = (BYTE)nRecv;

		nRecv = TE_BSocketGetChar(pBSD,dwTimeout);
		if( nRecv == TE_EOF )
			return (SOCKET_ERROR);
		pPacket->socks4Rep.bRep = (BYTE)nRecv;
		
		wData = 0;
		for( n = 0; n < 2; n++)
		{
			nRecv = TE_BSocketGetChar(pBSD,dwTimeout);
			if( nRecv == TE_EOF )
				return (SOCKET_ERROR);
		
			wData |= ( ((WORD)nRecv) << (8*n) );
		}
		pPacket->socks4Rep.wDestPort =ntohs(wData);

		dwData = 0;
		for( n = 0; n < 4; n++)
		{
			nRecv = TE_BSocketGetChar(pBSD,dwTimeout);
			if( nRecv == TE_EOF )
				return (SOCKET_ERROR);

			dwData |= ( ((DWORD)nRecv)<<(8*(3-n)) );
		}
		pPacket->socks4Rep.dwDestIP = ntohl(dwData);
		break;
	case PACKET_SOCKS4AREP:
		nRecv = TE_BSocketGetChar(pBSD,dwTimeout);
		if( nRecv == TE_EOF )
			return (SOCKET_ERROR);
		pPacket->socks4aRep.bVer = (BYTE)nRecv;

		nRecv = TE_BSocketGetChar(pBSD,dwTimeout);
		if( nRecv == TE_EOF )
			return (SOCKET_ERROR);
		pPacket->socks4aRep.bRep = (BYTE)nRecv;
		
		wData = 0;
		for( n = 0; n < 2; n++)
		{
			nRecv = TE_BSocketGetChar(pBSD,dwTimeout);
			if( nRecv == TE_EOF )
				return (SOCKET_ERROR);
		
			wData |= ( ((WORD)nRecv) << (8*(n)) );
		}
		pPacket->socks4aRep.wDestPort = ntohs(wData);

		dwData = 0;
		for( n = 0; n < 4; n++)
		{
			nRecv = TE_BSocketGetChar(pBSD,dwTimeout);
			if( nRecv == TE_EOF )
				return (SOCKET_ERROR);

			dwData |= ( ((DWORD)nRecv)<<(8*(3-n)) );
		}
		pPacket->socks4aRep.dwDestIP = ntohl(dwData);
		break;
	case PACKET_SOCKS5AUTHREP:
		nRecv = TE_BSocketGetChar(pBSD,dwTimeout);
		if( nRecv == TE_EOF )
			return (SOCKET_ERROR);
		pPacket->socks5AuthRep.bVer = (BYTE)nRecv;

		nRecv = TE_BSocketGetChar(pBSD,dwTimeout);
		if( nRecv == TE_EOF )
			return (SOCKET_ERROR);
		
		pPacket->socks5AuthRep.bAuth = (BYTE)nRecv;
		break;
	case PACKET_SOCKS5AUTHPASSWDREP:
		nRecv = TE_BSocketGetChar(pBSD,dwTimeout);
		if( nRecv == TE_EOF )
			return (SOCKET_ERROR);
		pPacket->socks5AuthPasswdRep.bVer = (BYTE)nRecv;

		nRecv = TE_BSocketGetChar(pBSD,dwTimeout);
		if( nRecv == TE_EOF )
			return (SOCKET_ERROR);
		
		pPacket->socks5AuthPasswdRep.bStatus = (BYTE)nRecv;
		break;
	case PACKET_SOCKS5REP:
		nRecv = TE_BSocketGetChar(pBSD,dwTimeout);
		if( nRecv == TE_EOF )
			return (SOCKET_ERROR);
		pPacket->socks5Rep.bVer = (BYTE)nRecv;

		nRecv = TE_BSocketGetChar(pBSD,dwTimeout);
		if( nRecv == TE_EOF )
			return (SOCKET_ERROR);
		pPacket->socks5Rep.bRep = (BYTE)nRecv;

		nRecv = TE_BSocketGetChar(pBSD,dwTimeout);
		if( nRecv == TE_EOF )
			return (SOCKET_ERROR);
		pPacket->socks5Rep.bRsv = (BYTE)nRecv;

		nRecv = TE_BSocketGetChar(pBSD,dwTimeout);
		if( nRecv == TE_EOF )
			return (SOCKET_ERROR);
		pPacket->socks5Rep.bAtyp = (BYTE)nRecv;

		if( pPacket->socks5Rep.bRep != REP5_SUCCESS )
		{
			pPacket->socks5Rep.pszBindAddr = NULL;
			pPacket->socks5Rep.wBindPort   = WORD_NULL;
			break;
		}
		
		switch( pPacket->socks5Rep.bAtyp )
		{
		case ATYP_IPV4ADDR:
		/*
			dwData = 0;
			for( n = 0; n < 4; n++)
			{
				nRecv = TE_BSocketGetChar(pBSD,dwTimeout);
				if( nRecv == TE_EOF )
					return (SOCKET_ERROR);

				dwData |= ( ((DWORD)nRecv)<<(8*(3-n)) );
			}
			dwData = ntohl( dwData );

			pszData = (char *)malloc(5);
			if(pszData == NULL)
				return (SOCKET_ERROR);
			n = 0;
			pszData[n++] = (char)( (dwData>>24) &0x000000ff); 
			pszData[n++] = (char)( (dwData>>16) &0x000000ff); 
			pszData[n++] = (char)( (dwData>>8 ) &0x000000ff); 
			pszData[n++] = (char)( dwData &0x000000ff); 
			pszData[n]	 = '\0';
			*/
			pszData = (char *)malloc(5);
			if(pszData == NULL)
				return (SOCKET_ERROR);
			for( n = 0; n < 4; n++)
			{
				nRecv = TE_BSocketGetChar(pBSD,dwTimeout);
				if( nRecv == TE_EOF )
					return (SOCKET_ERROR);

				pszData[n] = (char)nRecv;
			}
			pszData[n] = '\0';
			pPacket->socks5Rep.pszBindAddr = pszData;
			break;
		case ATYP_IPV6ADDR:
			
			pszData = (char *)malloc(17);
			if(pszData == NULL)
				return (SOCKET_ERROR);
			for( n = 0; n < 16; n++)
			{
				nRecv = TE_BSocketGetChar(pBSD,dwTimeout);
				if( nRecv == TE_EOF )
					return (SOCKET_ERROR);

				pszData[n] = (char)nRecv;
			}
			pszData[n] = '\0';
			pPacket->socks5Rep.pszBindAddr = pszData;
			break;
		case ATYP_HOSTNAME:
			nRecv = TE_BSocketGetChar(pBSD,dwTimeout);
			if( nRecv == TE_EOF )
				return (SOCKET_ERROR);
			
			nCount = nRecv;
			pszData = (char *)malloc(nCount+1 );
			if(pszData == NULL)
				return (SOCKET_ERROR);
			for( n = 0; n < nCount; n++)
			{
				nRecv = TE_BSocketGetChar(pBSD,dwTimeout);
				if( nRecv == TE_EOF )
					return (SOCKET_ERROR);

				pszData[n] = (char)nRecv;
			}
			pszData[n] = '\0';
			pPacket->socks5Rep.pszBindAddr = pszData;
			break;
		default:
			pPacket->socks5Rep.pszBindAddr = NULL;
			break;
		}

		wData = 0;
		for( n = 0; n < 2; n++)
		{
			nRecv = TE_BSocketGetChar(pBSD,dwTimeout);
			if( nRecv == TE_EOF )
				return (SOCKET_ERROR);
		
			wData |= ( ((WORD)nRecv) << (8*n) );
		}
		pPacket->socks5Rep.wBindPort = ntohs(wData);
		break;
	default:
		break;
	}
	return (SOCKET_SUCCESS);
}		

////////////////////////////////////////////////////////////////////
//
//		功能: 计算包的尺寸
//		参数: 
//				pPacket		-- 数据包指针
//				nPacketType	-- PACKET类型
//		返回: 数据包长度
//
////////////////////////////////////////////////////////////////////
int SOP_PacketSize(PSOCKSREQPACKET pPacket,UINT nPacketType)
{
	int nLen = 0;
	switch( nPacketType )
	{
	case PACKET_SOCKS4REQ:
		nLen = 9 + strlen(pPacket->socks4Req.pszUserID);
		break;
	case PACKET_SOCKS4AREQ:
		nLen = 10 + strlen(pPacket->socks4aReq.pszUserID) + strlen(pPacket->socks4aReq.pszDestHostName);
		break;
	case PACKET_SOCKS5AUTHREQ:
		nLen = 2 + (BYTE)pPacket->socks5AuthReq.bNAuth;
		break;
	case PACKET_SOCKS5AUTHPASSWDREQ:
		nLen = 3 + strlen(pPacket->socks5AuthPasswdReq.pszUserName) + strlen(pPacket->socks5AuthPasswdReq.pszPasswd);
		break;
	case PACKET_SOCKS5REQ:
		nLen = 6;
		switch( pPacket->socks5Req.bAtyp )
		{
		case ATYP_IPV4ADDR:
			nLen += 4;
			break;
		case ATYP_IPV6ADDR:
			nLen += 16;
			break;
		case ATYP_HOSTNAME:
			nLen += ((BYTE)pPacket->socks5Req.pszDestAddr[0]);
			nLen += 1;
			break;
		}
		break;
	}
	return nLen;
}


/////////////////////////////////////////////////////////////////////////////////////
//
//		功能: 将数据包打包成字符串
//		参数: 
//				pszDest		-- 字符串指针,用于保存打包数据(必须预先分配足够的内存空间)
//				pPacket		-- 数据包指针(待打包的数据包)
//				nPacketType	-- 包类型
//		返回: 
//				 0	---	成功 (SOCKET_SUCCESS)
//
/////////////////////////////////////////////////////////////////////////////////////
int SOP_Pack(char *pszDest,PSOCKSREQPACKET pPacket,UINT nPacketType)
{

	int n,nIndex=0;
	WORD  wData;
	DWORD dwData;

	switch( nPacketType )
	{
	case PACKET_SOCKS4REQ:
		pszDest[nIndex++] = (char)pPacket->socks4Req.bVer;
		pszDest[nIndex++] = (char)pPacket->socks4Req.bCmd;
		
		// 端口
		wData = htons(pPacket->socks4Req.wDestPort);
		pszDest[nIndex++] = (char)( wData & 0x00ff );
		pszDest[nIndex++] = (char)( (wData>>8) & 0x00ff);
		
		// IP地址
		dwData = htonl(pPacket->socks4Req.dwDestIP);
		pszDest[nIndex++] = (char)( (dwData>>24) &0x000000ff); 
		pszDest[nIndex++] = (char)( (dwData>>16) &0x000000ff); 
		pszDest[nIndex++] = (char)( (dwData>>8 ) &0x000000ff); 
		pszDest[nIndex++] = (char)( dwData &0x000000ff); 
		
		// UserID
		for( n = 0;n < (int)strlen(pPacket->socks4Req.pszUserID) ; n++ )
			pszDest[nIndex++] = (pPacket->socks4Req.pszUserID)[n];
		
		// NULL BYTE
		pszDest[nIndex++]	= (char)pPacket->socks4Req.bNULL;
		pszDest[nIndex]		= '\0';
		break;
	case PACKET_SOCKS4AREQ:
		pszDest[nIndex++] = (char)pPacket->socks4aReq.bVer;
		pszDest[nIndex++] = (char)pPacket->socks4aReq.bCmd;
				
		wData = htons(pPacket->socks4aReq.wDestPort);
		pszDest[nIndex++] = (char)( wData & 0x00ff );
		pszDest[nIndex++] = (char)( (wData>>8) & 0x00ff);
		
		dwData = htonl(pPacket->socks4aReq.dwDestIP);
		pszDest[nIndex++] = (char)( (dwData>>24) &0x000000ff); 
		pszDest[nIndex++] = (char)( (dwData>>16) &0x000000ff); 
		pszDest[nIndex++] = (char)( (dwData>>8 ) &0x000000ff); 
		pszDest[nIndex++] = (char)( dwData &0x000000ff); 
		
		// UserID
		for( n = 0;n < (int)strlen(pPacket->socks4aReq.pszUserID) ; n++ )
			pszDest[nIndex++] = (pPacket->socks4aReq.pszUserID)[n];
		
		// NULL1 BYTE
		pszDest[nIndex++]	= (char)pPacket->socks4aReq.bNULL1;

		// DestHostName
		for( n = 0;n < (int)strlen(pPacket->socks4aReq.pszDestHostName); n++ )
			pszDest[nIndex++] = (pPacket->socks4aReq.pszDestHostName)[n];
		
		// NULL2 BYTE
		pszDest[nIndex++]	= (char)pPacket->socks4aReq.bNULL2;
		pszDest[nIndex]		= '\0';
		break;
	case PACKET_SOCKS5AUTHREQ:
		pszDest[nIndex++]	= (char)pPacket->socks5AuthReq.bVer;
		pszDest[nIndex++]	= (char)pPacket->socks5AuthReq.bNAuth;
		for( n = 0; n < pPacket->socks5AuthReq.bNAuth;n++ )
			pszDest[nIndex++] = (pPacket->socks5AuthReq.pszAuth)[n];
		pszDest[nIndex]		= '\0';
		break;
	case PACKET_SOCKS5AUTHPASSWDREQ:
		pszDest[nIndex++]	= (char)pPacket->socks5AuthPasswdReq.bVer;
		pszDest[nIndex++]	= (char)pPacket->socks5AuthPasswdReq.bUserNameLen;

		for( n = 0 ; n < (int)strlen( pPacket->socks5AuthPasswdReq.pszUserName ); n++ )
			pszDest[nIndex++] = (pPacket->socks5AuthPasswdReq.pszUserName)[n];

		pszDest[nIndex++]	= (char)pPacket->socks5AuthPasswdReq.bPasswdLen;

		for( n = 0; n < (int)strlen(pPacket->socks5AuthPasswdReq.pszPasswd); n++ )
			pszDest[nIndex++] = (pPacket->socks5AuthPasswdReq.pszPasswd)[n];
		pszDest[nIndex]		= '\0';
		break;
	case PACKET_SOCKS5REQ:
		pszDest[nIndex++]	= (char)pPacket->socks5Req.bVer;
		pszDest[nIndex++]	= (char)pPacket->socks5Req.bCmd;
		pszDest[nIndex++]	= (char)pPacket->socks5Req.bRsv;
		pszDest[nIndex++]	= (char)pPacket->socks5Req.bAtyp;

		int nAddrLen;
		switch(pPacket->socks5Req.bAtyp)
		{
		case ATYP_IPV4ADDR:
			nAddrLen = 4;
			break;
		case ATYP_IPV6ADDR:
			nAddrLen = 16;
			break;
		case ATYP_HOSTNAME:
			nAddrLen =  1;
			nAddrLen += (BYTE) (pPacket->socks5Req.pszDestAddr[0]);
			break;
		default:
			nAddrLen = (int)strlen(pPacket->socks5Req.pszDestAddr);
			break;
		}

		for( n = 0; n <nAddrLen; n++ )
			pszDest[nIndex++] = (pPacket->socks5Req.pszDestAddr)[n];

		wData = htons(pPacket->socks5Req.wDestPort);
		pszDest[nIndex++] = (char)( wData & 0x00ff );
		pszDest[nIndex++] = (char)( (wData>>8) & 0x00ff);
		pszDest[nIndex]		= '\0';
		break;
	}

	//打包成功
	return (SOCKET_SUCCESS);
}

////////////////////////////////////////////////////////////////////
//
//		功能: 发送包
//		参数: 
//				hSocket		-- SOCKET(通讯套接字)
//				pPacket		-- 数据包指针(发送数据包)
//				nPacketType	-- 数据包类型
//				dwTimeout	-- 发送超时(缺省为SOP_SEND_TIMEOUT)	
//		返回: 
//				 0	---	成功 (SOCKET_SUCCESS)
//				-1	---	失败 (SOCKET_ERROR)	
//
////////////////////////////////////////////////////////////////////
int SOP_SendPacket(SOCKET hSocket,PSOCKSREQPACKET pPacket,UINT nPacketType,DWORD dwTimeout/* = SOP_SEND_TIMEOUT*/)
{
	if( hSocket == INVALID_SOCKET )
		return (SOCKET_ERROR);

	// 计算包大小
	int		nSize		= SOP_PacketSize(pPacket,nPacketType);
	char*	pSendBuf	= (char *)malloc(nSize+1);

	if(pSendBuf == NULL)
		return (SOCKET_ERROR);

	int nSend;
	int nRet = SOCKET_ERROR;
	
	// 打包并发送
	if(SOP_Pack(pSendBuf,pPacket,nPacketType) == SOCKET_SUCCESS )
	{
		nSend = TE_Send(hSocket,pSendBuf,nSize,dwTimeout);
		nRet  = (nSend == nSize)? SOCKET_SUCCESS:SOCKET_ERROR;
	}

	free(pSendBuf);
	return (nRet);
}

// 发送Socks4 Request
int SOP_SendSocks4Req(SOCKET hSocket,BYTE bCmd,WORD wDestPort,DWORD dwDestIP,const char * pszUserID /* = NULL */,DWORD dwTimeout /* = SOP_SEND_TIMEOUT */)
{
	SOCKSREQPACKET pack;
	pack.socks4Req.bVer			= VER_SOCKS4;
	pack.socks4Req.bCmd			= bCmd;
	pack.socks4Req.wDestPort	= wDestPort;
	pack.socks4Req.dwDestIP		= dwDestIP;
	pack.socks4Req.pszUserID	= pszUserID;
	pack.socks4Req.bNULL		= BYTE_NULL;
	
	return SOP_SendPacket(hSocket,&pack,PACKET_SOCKS4REQ,dwTimeout);
}


// 发送Socks4a Request
int SOP_SendSocks4aReq(SOCKET hSocket,BYTE bCmd,WORD wDestPort,const char* pszDestHostName,const char * pszUserID /* = NULL */,DWORD dwTimeout /* = SOP_SEND_TIMEOUT */)
{
	SOCKSREQPACKET pack;
	pack.socks4aReq.bVer			= VER_SOCKS4A;
	pack.socks4aReq.bCmd			= bCmd;
	pack.socks4aReq.wDestPort		= wDestPort;
	pack.socks4aReq.dwDestIP		= UNKNOWN_IP;
	pack.socks4aReq.pszUserID		= pszUserID;
	pack.socks4aReq.bNULL1			= BYTE_NULL;
	pack.socks4aReq.pszDestHostName = pszDestHostName;
	pack.socks4aReq.bNULL2			= BYTE_NULL;

	return SOP_SendPacket(hSocket,&pack,PACKET_SOCKS4AREQ,dwTimeout);
}

// 发送SOCKS5 Auth-Request
int SOP_SendSocks5AuthReq(SOCKET hSocket,BYTE bNAuth,const char* pszAuth,DWORD dwTimeout /* = SOP_SEND_TIMEOUT */)
{
	SOCKSREQPACKET pack;
	pack.socks5AuthReq.bVer			= VER_SOCKS5;
	pack.socks5AuthReq.bNAuth		= bNAuth;
	pack.socks5AuthReq.pszAuth		= pszAuth;

	return SOP_SendPacket(hSocket,&pack,PACKET_SOCKS5AUTHREQ,dwTimeout);
}

// 发送SOCKS5 USERNAME/PASSWORD Auth-Request
int SOP_SendSocks5AuthPasswdReq(SOCKET hSocket,const char* pszUserName,const char * pszPassword,DWORD dwTimeout /* = SOP_SEND_TIMEOUT */)
{
	SOCKSREQPACKET pack;
	pack.socks5AuthPasswdReq.bVer			= 0x01;
	pack.socks5AuthPasswdReq.bUserNameLen	= (BYTE)( (pszUserName == NULL)?0:strlen(pszUserName) );
	pack.socks5AuthPasswdReq.pszUserName	= pszUserName;
	pack.socks5AuthPasswdReq.bPasswdLen		= (BYTE)( (pszPassword == NULL)?0:strlen(pszPassword) );
	pack.socks5AuthPasswdReq.pszPasswd		= pszPassword;

	return SOP_SendPacket(hSocket,&pack,PACKET_SOCKS5AUTHPASSWDREQ,dwTimeout);
}


// 发送SOCKS5 REQUEST
//
// 注意: 若bAtyp = ATYP_IPV4ADDR pszDestAddr = IPV4_Address (4 Octects)
//		   bAtyp = ATYP_IPV6ADDR pszDestAddr = IPV6_Address (16 Octects) 	
//		   bAtyp = ATYP_HOSTNAME pszDestAddr = [1BYTE:(HostName)Length]+[HostName]	
//
int SOP_SendSocks5Req(SOCKET hSocket,BYTE bCmd,BYTE bAtyp,const char * pszDestAddr,WORD wDestPort,DWORD dwTimeout /* = SOP_SEND_TIMEOUT */)
{
	SOCKSREQPACKET pack;
	pack.socks5Req.bVer			= VER_SOCKS5;
	pack.socks5Req.bCmd			= bCmd;
	pack.socks5Req.bRsv			= BYTE_NULL;
	pack.socks5Req.bAtyp		= bAtyp;
	pack.socks5Req.pszDestAddr	= pszDestAddr;
	pack.socks5Req.wDestPort	= wDestPort;
	return SOP_SendPacket(hSocket,&pack,PACKET_SOCKS5REQ,dwTimeout);
}

// 检查SOCKS的返回值是否正确
BOOL SOP_IsSocksOK( PSOCKSREPPACKET pPacket,UINT nPacketType )
{
	switch ( nPacketType )
	{
	case PACKET_SOCKS4REP:
		return ( pPacket->socks4Rep.bRep == REP4_SUCCESS );
		break;
	case PACKET_SOCKS4AREP:
		return ( pPacket->socks4aRep.bRep == REP4_SUCCESS );
		break;
	case PACKET_SOCKS5AUTHREP:
		return ( pPacket->socks5AuthRep.bAuth == AUTH_NONE	 ||
				 pPacket->socks5AuthRep.bAuth == AUTH_GSSAPI ||
				 pPacket->socks5AuthRep.bAuth == AUTH_PASSWD ||
				 pPacket->socks5AuthRep.bAuth == AUTH_CHAP ) ;
		break;
	case PACKET_SOCKS5AUTHPASSWDREP:
		return ( pPacket->socks5AuthPasswdRep.bStatus == AUTH_OK );
		break;
	case PACKET_SOCKS5REP:
		return ( pPacket->socks5Rep.bRep == REP5_SUCCESS );
		break;
	default:
		return FALSE;
		break;
	}
}