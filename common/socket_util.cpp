/*
 * socket_util.cpp
 *
 *  Created on: Jul 19, 2015
 *      Author: jacken
 */
#include "socket_util.h"
#include "network_util.h"
#include "debug.h"
#include <sys/socket.h>
#include <string.h>
#include <net/if.h>
#include <netinet/in.h>

bool SocketUtil::Socket(int domain, int type, int protocol, int *sFD)
{
	*sFD = socket(domain, type, protocol);
	if (*sFD < 0) {
		*sFD = errno;
		ERR_PRINT("%s\n", strerror(*sFD));
		return false;
	}
	return true;
}

bool SocketUtil::Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	int nRet;
	nRet = bind(sockfd, addr, addrlen);
	if (nRet < 0) {
		nRet = errno;
		ERR_PRINT("%s\n", strerror(nRet));
		return false;
	}
	return true;
}

bool SocketUtil::BindInterface(int nSocketFD, const char *sInterfaceName)
{
	struct ifreq ifr;
	bool bRet;
	memset(&ifr, 0, sizeof(ifr));
	strncpy(ifr.ifr_name, sInterfaceName, sizeof(ifr.ifr_name));
	bRet = SocketUtil::SetSockOpt(nSocketFD, SOL_SOCKET, SO_BINDTODEVICE, (void *)&ifr, sizeof(ifr));
	if (bRet == false) {
		ERR_PRINT("SocketUtil::Setsockopt() error!\n");
		return false;
	}
	return true;
}

bool SocketUtil::BindPortOnly(int sockfd, unsigned int port)
{
	struct sockaddr_in sAddr;
	sAddr.sin_port = NetworkUtil::HostToNetworkByteOrder((unsigned short)port);
	sAddr.sin_family = AF_INET;
	sAddr.sin_addr.s_addr = NetworkUtil::HostToNetworkByteOrder(INADDR_ANY);
	bool bRet = SocketUtil::Bind(sockfd, (struct sockaddr*)&sAddr, sizeof(sAddr));
	if (bRet == false) {
		ERR_PRINT("SocketUtil::Bind() error!\n");
		return false;
	}
	return true;
}

bool SocketUtil::Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	int nRet;
	nRet = connect(sockfd, addr, addrlen);
	if (nRet < 0) {
		nRet = errno;
		ERR_PRINT("%s\n", strerror(nRet));
		return false;
	}
	return true;
}

bool SocketUtil::SetSockOpt(int sockfd, int level, int optname, const void *optval, socklen_t optlen)
{
	int nRet;
	nRet = setsockopt(sockfd, level, optname, optval, optlen);
	if (nRet < 0) {
		nRet = errno;
		ERR_PRINT("%s\n", strerror(nRet));
		return false;
	}
	return true;
}

bool SocketUtil::GetSocketOpt(int sockfd, int level, int optname, void *optval, socklen_t *optlen)
{
	int nRet;
	nRet = getsockopt(sockfd, level, optname, optval, optlen);
	if (nRet < 0) {
		nRet = errno;
		ERR_PRINT("%s\n", strerror(nRet));
		return false;
	}
	return true;
}

bool SocketUtil::Close(int sockfd)
{
	int nRet;
	if (sockfd >= 0) {
		nRet = close(sockfd);
		if (nRet < 0) {
			nRet = errno;
			ERR_PRINT("%s\n", strerror(nRet));
			return false;
		}
	}
	return true;
}

bool SocketUtil::Wait(time_t tMS, vector<int> sRegisterFD, vector<int> &sEventFD)
{
	struct timeval tv;
	tv.tv_sec = tMS / 1000;
	tv.tv_usec = (tMS % 1000) * 1000;

	fd_set readfds;
	FD_ZERO(&readfds);
	for (vector<int>::iterator it = sRegisterFD.begin(); it != sRegisterFD.end(); it++) {
		FD_SET(*it, &readfds);
	}

	int nRet = select(sRegisterFD.back() + 1, &readfds, NULL, NULL, &tv);
	if (nRet == 0) { // Timeout
		return false ;
	}

	if (nRet < 0) {
		nRet = errno;
		ERR_PRINT("%s!\n", strerror(nRet));
		return false;
	}

	for (vector<int>::iterator it = sRegisterFD.begin(); it != sRegisterFD.end(); it++) {
		nRet = FD_ISSET(*it, &readfds);
		if (nRet == 1) {
			sEventFD.push_back(*it);
			break;
		}
	}
	return true;
}

bool SocketUtil::RecvFrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *RemoteAddr, socklen_t *RemoteAddrLen, unsigned int *uRecvDataLen)
{
	int nRet = recvfrom(sockfd, buf, len, 0, (sockaddr*)RemoteAddr, RemoteAddrLen);
	if (nRet < 0) {
		nRet = errno;
		ERR_PRINT("%s!\n", strerror(nRet));
		return false;
	}
	*uRecvDataLen = nRet;
	return true;
}

Socket::Socket()
{
}

Socket::~Socket()
{
}

int Socket::GetFD()
{
	return m_sFD;
}


