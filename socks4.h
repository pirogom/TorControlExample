#ifndef __SOCKS4_TEST_H__
#define __SOCKS4_TEST_H__

class CSocks4Test
{
public:
	CSocks4Test()
	{
	}

	~CSocks4Test()
	{
		closesocket( m_nSock );
	}


	SOCKET m_nSock;

	void		Init()
	{
		m_nSock = ConnectToServer( "127.0.0.1", 9150 );
	}

	/**
	*	nonblock
	**/
	void nonblock( SOCKET s )
	{
		u_long            ulOn = 1L;

		ioctlsocket( s, FIONBIO, (unsigned long*)&ulOn );
	}

	/**
	*	ConnectToServer
	**/
	SOCKET ConnectToServer( char *host, int port )
	{
		struct sockaddr_in s_addr_in;
		SOCKET sock;

		sock = socket( AF_INET, SOCK_STREAM, 0 );

		if( sock == -1 )
			return -1;

		s_addr_in.sin_family = AF_INET;
		s_addr_in.sin_port = htons( port );
		s_addr_in.sin_addr.s_addr = inet_addr( host );

		if( connect( sock, (struct sockaddr*)&s_addr_in, sizeof( s_addr_in ) ) != 0 )
			return -1;

		nonblock( sock );
		return sock;
	}

	/**
	*	SendCmd
	**/
	int		SendCmd( const char *buf, int nLen )
	{
		return send( m_nSock, buf, nLen, 0 );
	}


	/**
	*	Response
	**/
	int		Response( char *szBuf, int nLen )
	{
		int nRecvSize = recv( m_nSock, szBuf, nLen, 0 );

		if( nRecvSize <= 0 )
		{
			if( WSAGetLastError() == WSAEWOULDBLOCK )
				return 0;
			else
				return -1;
		}
		return nRecvSize;
	}

	/**
	*	Auth
	**/
	int			Auth()
	{
		char szReq[128];

		int nReqSize = 0;

		szReq[nReqSize] = 0x05;		// 프로토콜 버전
		nReqSize += 1;
		szReq[nReqSize] = 0x01;		// 인증 종류 ( 0x00 없음 )
		nReqSize += 1;
		szReq[nReqSize] = 0x00;		// 인증 정보 ( 0x00 없음 );
		nReqSize += 1;

		SendCmd( szReq, nReqSize );

		char szResp[512];

		while( 1 )
		{
			int nLen = Response( szResp, 512 );

			if( nLen )
			{
				if( nLen == -1 )
					return 0;
				break;
			}
		}

		if( szResp[0] == 0x05 && szReq[1] == 0x01 )
			return 1;

		return 0;
	}


	/**
	*	Connection
	**/
	int			Connection( char *szAddr, short nPort )
	{
		char szReq[128];

		int nAddLen = strlen( szAddr );
		int nReqSize = 0;

		szReq[nReqSize] = 0x05;				// 프로토콜 버전 0x05 
		nReqSize += 1;
		szReq[nReqSize] = 0x01;				// 커맨드 0x01 = CONNECT
		nReqSize += 1;
		szReq[nReqSize] = 0x00;				// RESERVED
		nReqSize += 1;
		szReq[nReqSize] = 0x03;				// 0x01 = IPv4 IP, 0x03 = DOMAIN, 0x04 = IPv6 IP
		nReqSize += 1;
		// 첫번째 1바이트는 주소의 길이 뒤에 주소
		szReq[nReqSize] = (char)nAddLen;
		nReqSize += 1;
		memcpy( &szReq[nReqSize], szAddr, nAddLen );	// 주소 
		nReqSize += nAddLen;
		memcpy( &szReq[nReqSize], &nPort, 2 );
		nReqSize += 2;

		SendCmd( szReq, nReqSize );

		char szResp[512];

		while( 1 )
		{
			int nLen = Response( szResp, 512 );

			if( nLen )
			{
				if( nLen == -1 )
					return 0;
				break;
			}
		}

		if( szResp[0] == 0x05 && szResp[1] == 0x00 ) // successed
			return 1;

		return 0;
	}

};

#endif