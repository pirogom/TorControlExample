#include "stdafx.h"
#include "TorControl.h"


/**
*	CTorControl
**/
CTorControl::CTorControl()
{
	m_nSock = INVALID_SOCKET;
}


/**
*	CTorControl
**/
CTorControl::~CTorControl()
{
	if( m_nSock != INVALID_SOCKET && m_nSock != -1 )
		closesocket( m_nSock );
}

/**
*	Init
**/
int CTorControl::Init()
{
	m_nSock = ConnectToServer( TOR_CONTROL_SERVER, TOR_CONTROL_PORT );

	if( m_nSock == -1 || m_nSock == INVALID_SOCKET )
		return 0;

	return 1;
}

/**
*	nonblock
**/
void CTorControl::nonblock( SOCKET s )
{
	u_long            ulOn = 1L;

	ioctlsocket( s, FIONBIO, (unsigned long*)&ulOn );
}

/**
*	ConnectToServer
**/
SOCKET CTorControl::ConnectToServer( char *host, int port )
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
int		CTorControl::SendCmd( const char *buf )
{
	return send( m_nSock, buf, strlen( buf ), 0 );
}


/**
*	Response
**/
int		CTorControl::Response( char *szBuf, int nLen )
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
*	WaitResponse
**/
int			CTorControl::WaitResponse( char *resBuff, int nResSize )
{
	int nLen = 0;

	while( 1 )
	{
		nLen = Response( resBuff, nResSize );

		if( nLen )
		{
			if( nLen == -1 )
				return 0;
			break;
		}
	}
	return nLen;
}

/**
*	Auth
**/
int		CTorControl::Auth( char *szAuthFile )
{
	char szCookie[2048];

	if( GetAuthCookie( szAuthFile, szCookie ) == 0 )
		return 0;

	char szCmd[512];
	sprintf( szCmd, "AUTHENTICATE %s\r\n", szCookie );
	SendCmd( szCmd );

	char szResp[512];

	int nLen = WaitResponse( szResp, 512 );

	if( nLen > 0 && !strncmp( szResp, "250 OK", strlen( "250 OK" ) ) )
		return 1;

	return 0;
}

/**
*	Shutdown
**/
int			CTorControl::Shutdown()
{
	SendCmd( "SIGNAL SHUTDOWN\r\n" );

	char szResp[512];

	int nLen = WaitResponse( szResp, 512 );

	if( nLen > 0 && !strncmp( szResp, "250 OK", strlen( "250 OK" ) ) )
		return 1;

	return 0;
}

/**
*	GetAuthCookie
**/
int		CTorControl::GetAuthCookie( char *szCookieFile, char *szCookie )
{
	*szCookie = NULL;
	int nCnt = 0;

	FILE *fp = fopen( szCookieFile, "rb" );

	if( fp )
	{
		char szReadBuf[512];

		int nRead = fread( &szReadBuf, sizeof( char ), 512, fp );

		nCnt = bin2hex( szReadBuf, szCookie, nRead );

		fclose( fp );
	}

	return nCnt;
}

/**
*	bin2hex
**/
int			CTorControl::bin2hex( char *buf, char *outbuf, int len )
{
	char hexs[] = "0123456789ABCDEF";
	int i;
	int j = 0;
	for( i = 0; i < len; i++ )
	{
		outbuf[j++] = hexs[(buf[i] >> 4) & 0x0f];
		outbuf[j++] = hexs[buf[i] & 0x0f];
	}
	outbuf[j] = 0;

	return j;
}

