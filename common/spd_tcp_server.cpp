/*
 * spd_tcp_server.cpp
 *
 *  Created on: Mar 23, 2016
 *      Author: Jacken
 */
#include "debug.h"
#include "spd_tcp_server.h"
#include "network_util.h"

TcpServer::TcpServer(const char* sIP, unsigned int uPort)
:m_bIsInitPass(false)
{

	if (SocketUtil::Socket(AF_INET, SOCK_STREAM, 0, &m_sFD) == false) {
		ERR_PRINT("Create socket error!\n");
		return;
	}

	struct sockaddr_in LocalAddr;
	socklen_t AddrLen = sizeof(LocalAddr);
	bzero(&LocalAddr, AddrLen);
	LocalAddr.sin_family = AF_INET;
	LocalAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	LocalAddr.sin_port = NetworkUtil::HostToNetworkByteOrder((unsigned short)uPort);

	if (SocketUtil::Bind(m_sFD, (struct sockaddr *)&LocalAddr, AddrLen) == false) {
		ERR_PRINT("Bind socket error!\n");
		return;
	}
	int nPendingNum = 5;
	if (SocketUtil::Listen(m_sFD, nPendingNum) == false) {
		ERR_PRINT("Listen socket error!\n");
		return;
	}
	m_uRemoteAddrLen = sizeof(m_RemoteAddr);
	bzero(&m_RemoteAddr, m_uRemoteAddrLen);
	m_pRemoteAddr = (struct sockaddr*)&m_RemoteAddr;
	m_bIsInitPass = true;
}

TcpServer::~TcpServer()
{
}

bool TcpServer::IsInitPass()
{
	return m_bIsInitPass;
}

bool TcpServer::Accept()
{
	int nClientSocket = -1;
	if (SocketUtil::Accept(m_sFD, (struct sockaddr*)&m_RemoteAddr, &m_uRemoteAddrLen, &nClientSocket) == false) {
		ERR_PRINT("Accept socket error!\n");
		return false;
	}
	m_ClientFDList.push_back(nClientSocket);
	return true;
}

bool TcpServer::WaitClient(time_t tMS)
{
	if (SocketUtil::Wait(tMS, m_ClientFDList, &m_nEventFD) == false) {
		return false;
	}
	return true;
}

bool TcpServer::Recv(char* sBuf, unsigned int uBufSize)
{
	unsigned int uRecvLen;
	if (SocketUtil::RecvFrom(m_nEventFD, sBuf, uBufSize, 0, m_pRemoteAddr, &m_uRemoteAddrLen, &uRecvLen) == false) {
		return false;
	}
	if (uRecvLen == 0) {
		return false;
	}
	DBG_PRINT("uRecvLen: %u\n", uRecvLen);
	return true;
}

void TcpServer::CloseClient()
{
	for (list<int>::iterator it = m_ClientFDList.begin(); it != m_ClientFDList.end();) {
		if (*it == m_nEventFD) {
			if (SocketUtil::Close(m_nEventFD) == false) {
				return;
			}
			it = m_ClientFDList.erase(it);
			break;
		}
		else {
			++it;
		}
	}
	return;
}
