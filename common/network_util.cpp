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
#include <arpa/inet.h>

bool NetworkUtil::EnumerateUpInterfaceIPv4(list<string> &lList)
{
	list<string> lAllIf;
	bool bRet;
	short int nIFFlags;

	bRet = NetworkUtil::EnumerateAllInterfaceIPv4(lAllIf);
	if (bRet == false) {
		ERR_PRINT("EnumerateAllInterfaceIPv4() error!\n");
		return bRet;
	}

	for (list<string>::iterator it = lAllIf.begin(); it != lAllIf.end(); it++) {
		bRet = NetworkUtil::GetFlagsIPv4(it->c_str(), &nIFFlags);
		if (bRet == false) {
			continue;
		}

		if (nIFFlags & IFF_UP) {
			lList.push_back(*it);
		}
	}

	return true;
}

bool NetworkUtil::EnumerateAllInterfaceIPv4(list<string> &lList)
{
	DIR *dFD;
	struct dirent *pDir = NULL;
	int nRet;
	const char *SYSTEM_NET_PATH = "/sys/class/net";
	string sInterfaceName;

	dFD = opendir(SYSTEM_NET_PATH);
	if(dFD == NULL){
		nRet = errno;
		ERR_PRINT("%s\n",strerror(nRet));
		return false;
	}

	if (dFD) {
		while ((pDir = readdir(dFD)) != NULL) {
			if (strncmp(".", pDir->d_name, strlen(".")) == 0) {
				continue;
			}

			if (strncmp("..", pDir->d_name, strlen("..")) == 0) {
				continue;
			}

//			DBG_PRINT("interface name =%s\n", pDir->d_name);
			sInterfaceName = pDir->d_name;
			lList.push_back(sInterfaceName);
		}

		nRet = closedir(dFD);
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

bool NetworkUtil::InterfaceIndexToName(int nIfIndex, char *sIfName)
{
	char *pRet = if_indextoname(nIfIndex, sIfName);
	int nRet;
	if (pRet == NULL) {
		nRet = errno;
		ERR_PRINT("%s\n", strerror(nRet));
		return false;
	}
	return true;
}

unsigned int NetworkUtil::HostToNetworkByteOrder(unsigned int uHost)
{
	return htonl(uHost);
}

unsigned short NetworkUtil::HostToNetworkByteOrder(unsigned short uHost)
{
	return htons(uHost);
}

unsigned int NetworkUtil::NetworkToHostByteOrder(unsigned int uHost)
{
	return ntohl(uHost);
}

unsigned short NetworkUtil::NetworkToHostByteOrder(unsigned short uHost)
{
	return ntohs(uHost);
}

bool NetworkUtil::IPv4AddressTextToBinary(const char *sIP, struct in_addr *sAddr)
{
	int nRet;
	nRet = inet_aton(sIP, sAddr);
	if (nRet == 0) {
		ERR_PRINT("inet_aton() error!\n");
		return false;
	}
	return true;
}

bool NetworkUtil::IPv4AddressBinaryToText(struct in_addr sAddr, string &sIP)
{
	char *pRet = inet_ntoa(sAddr);
	if (pRet == NULL) {
		ERR_PRINT("inet_ntoa() error!\n");
		return false;
	}
	sIP = pRet;
	return true;
}
