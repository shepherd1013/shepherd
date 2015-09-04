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
#include <sys/un.h>
#include <stddef.h>

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

bool SocketUtil::Wait(time_t tMS, list<int> sRegisterFD, int* sEventFD)
{
	struct timeval tv;
	tv.tv_sec = tMS / 1000;
	tv.tv_usec = (tMS % 1000) * 1000;

	fd_set readfds;
	FD_ZERO(&readfds);
	for (list<int>::iterator it = sRegisterFD.begin(); it != sRegisterFD.end(); it++) {
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

	for (list<int>::iterator it = sRegisterFD.begin(); it != sRegisterFD.end(); it++) {
		nRet = FD_ISSET(*it, &readfds);
		if (nRet == 1) {
			*sEventFD = *it;
			break;
		}
	}
	return true;
}

bool SocketUtil::Wait(time_t tMS, vector<int> sRegisterFD, int* sEventFD)
{
	if (sRegisterFD.empty()) {
		ERR_PRINT("Resgister socket FD is empty!\n");
		return false;
	}
	struct timeval tv;
	tv.tv_sec = tMS / 1000;
	tv.tv_usec = (tMS % 1000) * 1000;

	fd_set readfds;
	FD_ZERO(&readfds);
	for (vector<int>::iterator it = sRegisterFD.begin(); it != sRegisterFD.end(); it++) {
		if(*it < 0){
			ERR_PRINT("Invalid socket FD (%d)!\n", *it);
			continue;
		}
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
			*sEventFD = *it;
			break;
		}
	}
	return true;
}

bool SocketUtil::RecvFrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *RemoteAddr, socklen_t *RemoteAddrLen, unsigned int *uRecvDataLen)
{
	int nRet = recvfrom(sockfd, buf, len, 0, RemoteAddr, RemoteAddrLen);
	if (nRet < 0) {
		nRet = errno;
		ERR_PRINT("%s!\n", strerror(nRet));
		return false;
	}
	*uRecvDataLen = nRet;
	return true;
}

bool SocketUtil::SendTo(int sockfd, const void *buf, size_t len, int flags, struct sockaddr *RemoteAddr, socklen_t RemoteAddrLen)
{
	int nRet = sendto(sockfd, buf, len, 0, RemoteAddr, RemoteAddrLen);
	if (nRet < 0) {
		nRet = errno;
		ERR_PRINT("%s!\n", strerror(nRet));
		return false;
	}

	if (len != (unsigned int)nRet) {
		ERR_PRINT("Data size (%u) isn't equal to sent size (%d)!\n", len, nRet);
		return false;
	}

	return true;
}

bool SocketUtil::DuplicateFD(int nOldFD, int *nNewFD)
{
	int nRet = dup(nOldFD);
	if (nRet < 0) {
		nRet = errno;
		ERR_PRINT("%s!\n", strerror(nRet));
		return false;
	}
	*nNewFD = nRet;
	return true;
}

Socket::Socket()
{
}

Socket::~Socket()
{
	DBG_PRINT("Run %s() ...\n", __FUNCTION__);
	bool bRet = SocketUtil::Close(m_sFD);
	if (bRet == false) {
		ERR_PRINT("SocketUtil::Close() error!\n");
		return;
	}
}

int Socket::GetFD()
{
	return m_sFD;
}

SocketIPC::SocketIPC()
{
}

SocketIPC::~SocketIPC()
{
}

SocketIPCServer::SocketIPCServer(const char* sLocalPath)
{
	bool bRet;

	bRet = SocketUtil::Socket(AF_UNIX, SOCK_DGRAM, 0, &m_sFD);
	if (bRet == false) {
		ERR_PRINT("SocketUtil::Socket() error!\n");
		return;
	}

	int nRet = unlink(sLocalPath);
	if ( (nRet < 0) && (errno != ENOENT) ) {
		nRet = errno;
		ERR_PRINT("nRet: %d\n", nRet);
		ERR_PRINT("%s\n", strerror(nRet));
		return;
	}

	int size;
	struct sockaddr_un un;
	memset(&un, 0, sizeof(un));
	strncpy(un.sun_path, sLocalPath, sizeof(un.sun_path));
	un.sun_family = AF_UNIX;
	size = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);
	bRet = SocketUtil::Bind(m_sFD, (struct sockaddr *)&un, size);
	if (bRet == false) {
		ERR_PRINT("SocketUtil::Bind() error!\n");
		return;
	}
}

SocketIPCServer::~SocketIPCServer()
{
}

bool SocketIPCServer::Wait(time_t tMS)
{
	vector<int> sRegisterFD;
	int sEventFD;
	sRegisterFD.push_back(m_sFD);
	bool bRet = SocketUtil::Wait(tMS, sRegisterFD, &sEventFD);
	if (bRet == false) {
		return false;
	}
	return true;
}

bool SocketIPCServer::Recv()
{
	int nRet;
	nRet = recv(m_sFD, m_buffer, sizeof(m_buffer), 0);
	if(nRet < 0){
		nRet = errno;
		ERR_PRINT("%s\n", strerror(nRet));
		return false;
	}
	DBG_PRINT("Recv data: %s\n", m_buffer);
	return true;
}

SocketIPCClient::SocketIPCClient(const char* sRemotePath)
{
	bool bRet;
	bRet = SocketUtil::Socket(AF_UNIX, SOCK_DGRAM, 0, &m_sFD);
	if (bRet == false) {
		ERR_PRINT("SocketUtil::Socket() error!\n");
		return;
	}

	struct sockaddr_un sRemote;
	socklen_t RemoteLength = sizeof(sRemote);
	memset(&sRemote, 0, sizeof(sRemote));
	sRemote.sun_family = AF_UNIX;
	strncpy(sRemote.sun_path, sRemotePath, sizeof(sRemote.sun_path));
	bRet = SocketUtil::Connect(m_sFD, (struct sockaddr *)&sRemote, RemoteLength);
	if (bRet == false) {
		ERR_PRINT("SocketUtil::Connect() error!\n");
		return;
	}
}

SocketIPCClient::~SocketIPCClient()
{
}

bool SocketIPCClient::Send(const char *SendData, unsigned int uDataSize)
{
	int nRet = send(m_sFD, SendData, uDataSize, 0);
	if(nRet < 0){
		nRet = errno;
		ERR_PRINT("%s\n", strerror(nRet));
		return false;
	}
	DBG_PRINT("Send data size: %d\n", nRet);
	return true;
}
