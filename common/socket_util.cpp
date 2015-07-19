/*
 * socket_util.cpp
 *
 *  Created on: Jul 19, 2015
 *      Author: jacken
 */
#include "socket_util.h"
#include "debug.h"
#include <sys/socket.h>
#include <string.h>

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

bool SocketUtil::Setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen)
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


