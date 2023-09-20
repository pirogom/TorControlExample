#ifndef __TOR_CONNECTER_H__
#define __TOR_CONNECTER_H__

#define TOR_CONNECTER_SERVER			"127.0.0.1"
#define TOR_CONNECTER_PORT				9150

/**
*	TOR CONNECTER
**/
class CTorConnecter
{
public:
	SOCKET m_nSock;

public:
	CTorConnecter();
	~CTorConnecter();

	int			Init();
	void		Destroy();
	void		nonblock( SOCKET s );
	SOCKET		ConnectToServer( char *host, int port );
	int			SendCmd( const char *buf, int nLen );
	int			Response( char *szBuf, int nLen );
	int			Auth();
	int			Connection( char *szAddr, short nPort );
};

#endif