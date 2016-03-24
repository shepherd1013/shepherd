/*
 * spd_tcp.cpp
 *
 *  Created on: Mar 24, 2016
 *      Author: Jacken
 */
#include "spd_tcp.h"
#include "debug.h"

TcpSocket::TcpSocket()
{
	if (SocketUtil::Socket(AF_INET, SOCK_STREAM, 0, &m_sFD) == false) {
		ERR_PRINT("Create socket error!\n");
		return;
	}
}

TcpSocket::~TcpSocket()
{
}
