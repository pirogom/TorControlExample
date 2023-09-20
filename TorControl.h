#ifndef __TOR_CONTROL_H__
#define __TOR_CONTROL_H__

#define TOR_CONTROL_SERVER		"127.0.0.1"
#define TOR_CONTROL_PORT		9151

/**
*	CTorControl
**/
class CTorControl
{
public:
	CTorControl();
	~CTorControl();
	int Init();

	SOCKET		m_nSock;

	void		nonblock( SOCKET s );
	SOCKET		ConnectToServer( char *host, int port );
	int			SendCmd( const char *buf );
	int			Response( char *szBuf, int nLen );

	int			Auth( char *szAuthFile );
	int			GetAuthCookie( char *szCookieFile, char *szCookie );
	int			bin2hex( char *buf, char *outbuf, int len );
	int			Shutdown();

	int			WaitResponse( char *resBuff, int nResSize );
};

#endif