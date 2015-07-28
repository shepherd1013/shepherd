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
#include "socket_util.h"

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

bool NetworkUtil::IPv4AddressBinaryToText(struct in_addr sAddr, char *sIP, unsigned int uIPLen)
{
	char *pRet = inet_ntoa(sAddr);
	if (pRet == NULL) {
		ERR_PRINT("inet_ntoa() error!\n");
		return false;
	}
	strncpy(sIP, pRet, uIPLen);
	return true;
}

bool NetworkUtil::Multicast(const char* sLocalIP, const char* sMcastAddr, unsigned int uTargetPort, const void* sSendData, unsigned int uSendDataSize)
{
	bool bRet;
	int nSocketFD;

	bRet = SocketUtil::Socket(AF_INET, SOCK_DGRAM, 0, &nSocketFD);
	if (bRet  == false) {
		ERR_PRINT("SocketUtil::Socket() error!\n");
		return false;
	}

	struct in_addr stLocalIP;
	bRet = NetworkUtil::IPv4AddressTextToBinary(sLocalIP, &stLocalIP);
	if (bRet  == false) {
		ERR_PRINT("NetworkUtil::IPv4AddressTextToBinary() error!\n");
		return false;
	}

	bRet = SocketUtil::SetSockOpt(nSocketFD, IPPROTO_IP, IP_MULTICAST_IF, &stLocalIP, sizeof(stLocalIP));
	if (bRet  == false) {
		ERR_PRINT("SocketUtil::SetSockOpt() error!\n");
		return false;
	}

	struct sockaddr_in stRemoteAddr;
	stRemoteAddr.sin_port = HostToNetworkByteOrder((unsigned short)uTargetPort);
	stRemoteAddr.sin_family = AF_INET;
	bRet = NetworkUtil::IPv4AddressTextToBinary(sMcastAddr, &(stRemoteAddr.sin_addr));
	if (bRet == false) {
		ERR_PRINT("NetworkUtil::IPv4AddressTextToBinary() error!\n");
		return false;
	}

	bRet = SocketUtil::SendTo(nSocketFD, sSendData, uSendDataSize, 0, (sockaddr*)&stRemoteAddr, sizeof(stRemoteAddr));
	if (bRet  == false) {
		ERR_PRINT("SocketUtil::SendTo() error!\n");
		return false;
	}

	return true;
}

bool NetworkUtil::Broadcast(unsigned int uTargetPort, const void* sSendData, unsigned int uSendDataSize)
{
	bool bRet;
	int nSocketFD;

	bRet = SocketUtil::Socket(AF_INET, SOCK_DGRAM, 0, &nSocketFD);
	if (bRet  == false) {
		ERR_PRINT("SocketUtil::Socket() error!\n");
		return false;
	}

	int bIsBcastEnable = 1;
	bRet = SocketUtil::SetSockOpt(nSocketFD, SOL_SOCKET, SO_BROADCAST, &bIsBcastEnable, sizeof(bIsBcastEnable));
	if (bRet  == false) {
		ERR_PRINT("SocketUtil::SetSockOpt() error!\n");
		return false;
	}

	string sIP;
	struct in_addr uBcastIP;
	uBcastIP.s_addr = INADDR_BROADCAST;

	bRet = NetworkUtil::IPv4AddressBinaryToText(uBcastIP, sIP);
	if (bRet  == false) {
		ERR_PRINT("NetworkUtil::IPv4AddressBinaryToText() error!\n");
		return false;
	}
	DBG_PRINT("Bcast IP:%s\n", sIP.c_str());

	struct sockaddr_in stRemoteAddr;
	stRemoteAddr.sin_port = HostToNetworkByteOrder((unsigned short)uTargetPort);
	stRemoteAddr.sin_family = AF_INET;
	stRemoteAddr.sin_addr.s_addr = uBcastIP.s_addr;

	bRet = SocketUtil::SendTo(nSocketFD, sSendData, uSendDataSize, 0, (sockaddr*)&stRemoteAddr, sizeof(stRemoteAddr));
	if (bRet  == false) {
		ERR_PRINT("SocketUtil::SendTo() error!\n");
		return false;
	}

	return true;
}

bool NetworkUtil::BroadcastSubnet(const char* sIfName, bool bIsBindInterface, unsigned int uTargetPort, const void* sSendData, unsigned int uSendDataSize)
{
	bool bRet;
	int nSocketFD;

	bRet = SocketUtil::Socket(AF_INET, SOCK_DGRAM, 0, &nSocketFD);
	if (bRet  == false) {
		ERR_PRINT("SocketUtil::Socket() error!\n");
		return false;
	}

	int bIsBcastEnable = 1;
	bRet = SocketUtil::SetSockOpt(nSocketFD, SOL_SOCKET, SO_BROADCAST, &bIsBcastEnable, sizeof(bIsBcastEnable));
	if (bRet  == false) {
		ERR_PRINT("SocketUtil::SetSockOpt() error!\n");
		return false;
	}

	if (bIsBindInterface) {
		bRet = SocketUtil::BindInterface(nSocketFD, sIfName);
		if (bRet  == false) {
			ERR_PRINT("SocketUtil::BindInterface() error!\n");
			return false;
		}
	}

	struct in_addr uLocalIP;
	bRet = NetworkUtil::GetInterfaceIPIPv4Binary(sIfName, &uLocalIP);
	if (bRet  == false) {
		ERR_PRINT("NetworkUtil::GetInterfaceIPIPv4Binary() error!\n");
		return false;
	}

	string sIP;
	bRet = NetworkUtil::IPv4AddressBinaryToText(uLocalIP, sIP);
	if (bRet  == false) {
		ERR_PRINT("NetworkUtil::IPv4AddressBinaryToText() error!\n");
		return false;
	}
	DBG_PRINT("Local IP:%s\n", sIP.c_str());

	struct in_addr uLocalNetmaskIP;
	bRet = NetworkUtil::GetInterfaceNetmaskIPIPv4Binary(sIfName, &uLocalNetmaskIP);
	if (bRet  == false) {
		ERR_PRINT("NetworkUtil::GetInterfaceNetmaskIPIPv4Binary() error!\n");
		return false;
	}

	struct in_addr uBcastIP;
	uBcastIP.s_addr = uLocalIP.s_addr | (~(uLocalNetmaskIP.s_addr));

	bRet = NetworkUtil::IPv4AddressBinaryToText(uBcastIP, sIP);
	if (bRet  == false) {
		ERR_PRINT("NetworkUtil::IPv4AddressBinaryToText() error!\n");
		return false;
	}
	DBG_PRINT("Bcast IP:%s\n", sIP.c_str());

	struct sockaddr_in stRemoteAddr;
	stRemoteAddr.sin_port = HostToNetworkByteOrder((unsigned short)uTargetPort);
	stRemoteAddr.sin_family = AF_INET;
	stRemoteAddr.sin_addr.s_addr = uBcastIP.s_addr;

	bRet = SocketUtil::SendTo(nSocketFD, sSendData, uSendDataSize, 0, (sockaddr*)&stRemoteAddr, sizeof(stRemoteAddr));
	if (bRet  == false) {
		ERR_PRINT("SocketUtil::SendTo() error!\n");
		return false;
	}

	return true;
}

bool NetworkUtil::GetInterfaceIPIPv4Text(const char *sIfName, char* sIP, unsigned int uIPLen)
{
	struct in_addr stAddr;
	bool bRet = NetworkUtil::GetInterfaceIPIPv4Binary(sIfName, &stAddr);
	if (bRet  == false) {
		ERR_PRINT("NetworkUtil::GetInterfaceIPIPv4Binary() error!\n");
		return false;
	}

	bRet = NetworkUtil::IPv4AddressBinaryToText(stAddr, sIP, uIPLen);
	if (bRet  == false) {
		ERR_PRINT("NetworkUtil::IPv4AddressBinaryToText() error!\n");
		return false;
	}
	return true;
}

bool NetworkUtil::GetInterfaceIPIPv4Binary(const char *sIfName, struct in_addr* pAddr)
{
	struct ifreq ifr;
	int sockfd;
	int nRet;
	bool bRet;

	bRet = SocketUtil::Socket(AF_INET, SOCK_DGRAM, 0, &sockfd);
	if (bRet  == false) {
		ERR_PRINT("SocketUtil::Socket() error!\n");
		return false;
	}

	bzero(&ifr, sizeof(ifr));
	strncpy(ifr.ifr_name, sIfName, sizeof(ifr.ifr_name));

	nRet = ioctl(sockfd, SIOCGIFADDR, &ifr);
	if (nRet < 0) {
		nRet = errno;
		ERR_PRINT("%s\n",strerror(nRet));
		return false;
	}

	bRet = SocketUtil::Close(sockfd);
	if (bRet  == false) {
		ERR_PRINT("SocketUtil::Close() error!\n");
		return false;
	}

	memcpy(pAddr, &(((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr), sizeof(struct in_addr));
//	*pAddr = ((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr;

	return true;
}

bool NetworkUtil::GetInterfaceNetmaskIPIPv4Binary(const char *sIfName, struct in_addr* pAddr)
{
	struct ifreq ifr;
	int sockfd;
	int nRet;
	bool bRet;

	bRet = SocketUtil::Socket(AF_INET, SOCK_DGRAM, 0, &sockfd);
	if (bRet  == false) {
		ERR_PRINT("SocketUtil::Socket() error!\n");
		return false;
	}

	bzero(&ifr, sizeof(ifr));
	strncpy(ifr.ifr_name, sIfName, sizeof(ifr.ifr_name));

	nRet = ioctl(sockfd, SIOCGIFNETMASK, &ifr);
	if (nRet < 0) {
		nRet = errno;
		ERR_PRINT("%s\n",strerror(nRet));
		return false;
	}

	bRet = SocketUtil::Close(sockfd);
	if (bRet  == false) {
		ERR_PRINT("SocketUtil::Close() error!\n");
		return false;
	}

	memcpy(pAddr, &(((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr), sizeof(struct in_addr));
//	*pAddr = ((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr.s_addr;

	return true;
}

bool NetworkUtil::IPBinaryToText(int nAddressFamily, const void* stAddr, char* sIP)
{
	const char *pRet;
	int nRet;

	switch (nAddressFamily) {
	case AF_INET:
		pRet = inet_ntop(nAddressFamily, stAddr, sIP, INET_ADDRSTRLEN);
		break;
	case AF_INET6:
		pRet = inet_ntop(nAddressFamily, stAddr, sIP, INET6_ADDRSTRLEN);
		break;
	default:
		ERR_PRINT("Unknown address family (%d)!", nAddressFamily);
		return false;
	}

	if (pRet == NULL) {
		nRet = errno;
		ERR_PRINT("%s\n",strerror(nRet));
		return false;
	}
	return true;
}

