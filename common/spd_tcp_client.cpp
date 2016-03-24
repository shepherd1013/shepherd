/*
 * spd_tcp_client.cpp
 *
 *  Created on: Mar 23, 2016
 *      Author: Jacken
 */

#include "debug.h"
#include "spd_tcp_client.h"
#include "network_util.h"
#include <fcntl.h>

TcpClient::TcpClient()
{
	m_bIsConnected = false;
}

TcpClient::~TcpClient()
{
}

TcpClient::TcpClient(const char* sRemoteIP, unsigned int uRemotePort, unsigned int uLocalPort)
{
	m_bIsConnected = false;
	if (this->Connect(sRemoteIP, uRemotePort, uLocalPort) == false) {
		return;
	}
	m_bIsConnected = true;
}

bool TcpClient::Init(const char* sRemoteIP, unsigned int uRemotePort)
{
	m_uRemoteAddrLen = sizeof(m_RemoteAddr);
	bzero(&m_RemoteAddr, m_uRemoteAddrLen);

	if (SocketUtil::IsPortValid(uRemotePort) == false) {
		ERR_PRINT("Invalid port number (%u)!\n", uRemotePort);
		return false;
	}

	m_RemoteAddr.sin_family = AF_INET;
	if (NetworkUtil::IPv4AddressTextToBinary(sRemoteIP, &(m_RemoteAddr.sin_addr)) == false) {
		ERR_PRINT("Convert address text to binary error!\n");
		return false;
	}
	m_RemoteAddr.sin_port = NetworkUtil::HostToNetworkByteOrder((unsigned short)uRemotePort);
	m_pRemoteAddr = (struct sockaddr*)&m_RemoteAddr;

//	int flags;
//	if ((flags = fcntl(m_sFD, F_GETFL)) < 0) { //获取当前的flags标志
//		ERR_PRINT("F_GETFL error!");
//	}
//	flags |= O_NONBLOCK; //修改非阻塞标志位
//	if (fcntl(m_sFD, F_SETFL, flags) < 0) {
//		ERR_PRINT("F_SETFL error!");
//	}
//	int enable = 1;
//	if (SocketUtil::SetSockOpt(this->GetFD(), SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) == false) {
//		ERR_PRINT("Set socket option error!\n");
//		return;
//	}

	return true;
}

bool TcpClient::Connect(const char* sRemoteIP, unsigned int uRemotePort, unsigned int uLocalPort)
{
	if (m_bIsConnected) {
		return true;
	}

	if (this->Init(sRemoteIP, uRemotePort) == false) {
		ERR_PRINT("Init error!\n");
		return false;
	}

	if (uLocalPort != 0) {
		if (SocketUtil::IsPortValid(uLocalPort) == false) {
			ERR_PRINT("Invalid port number (%u)!\n", uLocalPort);
			return false;
		}
		if (SocketUtil::BindPortOnly(m_sFD, uLocalPort) == false) {
			ERR_PRINT("Bind port error!\n");
			return false;
		}
	}

	if (SocketUtil::Connect(m_sFD, m_pRemoteAddr, m_uRemoteAddrLen) == false) {
		ERR_PRINT("Connect socket error!\n");
		return false;
	}
	m_bIsConnected = true;
	return true;
}

