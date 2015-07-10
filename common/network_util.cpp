/*
 * network_util.cpp
 *
 *  Created on: Jun 24, 2015
 *      Author: Jacken
 */
#include "network_util.h"
#include <dirent.h>
#include <string.h>
#include "debug.h"
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>

bool NetworkUtil::EnumerateInterfaceIPv4(list<string> &lList)
{
	DIR *dp;
	struct dirent *pDir;
	bool bRet;
	int nRet;
	const char *SYSTEM_NET_PATH = "/sys/class/net";
	short int nIFFlags;

	dp = opendir(SYSTEM_NET_PATH);
	if(dp == NULL){
		nRet = errno;
		ERR_PRINT("%s\n",strerror(nRet));
		return false;
	}

	if (dp) {
		while ((pDir = readdir(dp)) != NULL) {
			if (strncmp(".", pDir->d_name, strlen(".")) == 0) {
				continue;
			}

			if (strncmp("..", pDir->d_name, strlen("..")) == 0) {
				continue;
			}

//			DBG_PRINT("interface name =%s\n", pDir->d_name);

			bRet = NetworkUtil::GetFlagsIPv4(pDir->d_name, &nIFFlags);
			if(bRet == false){
				ERR_PRINT("GetFlagsIPv4() error!\n");
				return bRet;
			}

//			if (nIFFlags & IFF_LOOPBACK) {
//				DBG_PRINT("Loop back interface!\n");
//			}

			if (nIFFlags & IFF_UP) {
//				DBG_PRINT("%s up!\n", pDir->d_name);
				string sInterfaceName = pDir->d_name;
				lList.push_back(sInterfaceName);
			}
		}

		nRet = closedir(dp);
		if(nRet < 0){
			nRet = errno;
			ERR_PRINT("%s\n",strerror(nRet));
			return false;
		}
	}
	return true;
}

bool NetworkUtil::GetFlagsIPv4(const char* sInterfaceName, short int* nIFFlags)
{
	struct ifreq ifr;
	int sockfd;
	int nRet;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	bzero(&ifr, sizeof(ifr));
	strcpy(ifr.ifr_name, sInterfaceName);

	nRet = ioctl(sockfd, SIOCGIFFLAGS, &ifr);
	if (nRet < 0) {
		nRet = errno;
		ERR_PRINT("%s\n",strerror(nRet));
		return false;
	}
	*nIFFlags = ifr.ifr_flags;
	close(sockfd);
	return true;
}

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

Socket::Socket()
{
}

Socket::~Socket()
{
}

bool Socket::Wait(time_t tMS, vector<int> &sEventFD)
{
	struct timeval tv;
	tv.tv_sec = tMS / 1000;
	tv.tv_usec = (tMS % 1000) * 1000;

	fd_set readfds;
	FD_ZERO(&readfds);
	for (vector<int>::iterator it = m_sFD.begin(); it != m_sFD.end(); it++) {
		FD_SET(*it, &readfds);
	}

	int nRet = select(m_sFD.back() + 1, &readfds, NULL, NULL, &tv);
	if (nRet == 0) { // Timeout
		return false ;
	}

	if (nRet < 0) {
		nRet = errno;
		ERR_PRINT("%s!\n", strerror(nRet));
		return false;
	}

	for (vector<int>::iterator it = m_sFD.begin(); it != m_sFD.end(); it++) {
		nRet = FD_ISSET(*it, &readfds);
		if (nRet == 0) {
			continue;
		}
	}

	if (sEventFD.empty()) {
		return false;
	}

	return true;
}

//int Socket::GetFD()
//{
//	return m_sFD;
//}
