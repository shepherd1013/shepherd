/*
 * spd_tcp_server.h
 *
 *  Created on: Mar 23, 2016
 *      Author: Jacken
 */

#ifndef SPD_TCP_SERVER_H_
#define SPD_TCP_SERVER_H_

#include "socket_util.h"

class TcpServer: public Socket
{
public:
	TcpServer(const char* sIP, unsigned int uPort);
	~TcpServer();
	bool IsInitPass();
	bool Accept();
	bool WaitClient(time_t tMS);
	virtual bool Recv(char* sBuf, unsigned int uBufSize);
	void CloseClient();

protected:
	sockaddr_in	m_RemoteAddr;
	bool		m_bIsInitPass;
//	int			m_nClientSocket;
//	vector<int>	m_ClientFDList;
	list<int>	m_ClientFDList;
	int 		m_nEventFD;
};


#endif /* SPD_TCP_SERVER_H_ */
