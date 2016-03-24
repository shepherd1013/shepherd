/*
 * spd_tcp_client.h
 *
 *  Created on: Mar 23, 2016
 *      Author: Jacken
 */

#ifndef SPD_TCP_CLIENT_H_
#define SPD_TCP_CLIENT_H_

#include "spd_tcp.h"

class TcpClient: public TcpSocket
{
public:
	TcpClient();
	~TcpClient();
	TcpClient(const char* sRemoteIP, unsigned int uRemotePort, unsigned int uLocalPort = 0);
	bool Connect(const char* sRemoteIP, unsigned int uRemotePort, unsigned int uLocalPort = 0);

protected:
	sockaddr_in	m_RemoteAddr;
	bool		m_bIsInitPass;
	bool		m_bIsConnected;
	int			m_nClientSocket;

	bool		Init(const char* sRemoteIP, unsigned int uRemotePort);
};

#endif /* SPD_TCP_CLIENT_H_ */
