/*
 * main.cpp
 *
 *  Created on: Jul 28, 2015
 *      Author: Jacken
 */

#include "network_util.h"
#include "socket_util.h"
#include "debug.h"

bool TestMulticastClient()
{
	bool bRet;
	int nSocketFD;

	bRet = SocketUtil::Socket(AF_INET, SOCK_DGRAM, 0, &nSocketFD);
	if (bRet  == false) {
		ERR_PRINT("SocketUtil::Socket() error!\n");
		return false;
	}

	int nReuse = 1;
	bRet = SocketUtil::SetSockOpt(nSocketFD, SOL_SOCKET, SO_REUSEADDR, &nReuse, sizeof(nReuse));
	if (bRet  == false) {
		ERR_PRINT("SocketUtil::SetSockOpt() error!\n");
		return false;
	}

	struct sockaddr_in stLocalAddr;
	stLocalAddr.sin_port = NetworkUtil::HostToNetworkByteOrder((unsigned short)3702);
	stLocalAddr.sin_family = AF_INET;
	stLocalAddr.sin_addr.s_addr = INADDR_ANY;

	bRet = SocketUtil::Bind(nSocketFD, (struct sockaddr*)&stLocalAddr, sizeof(stLocalAddr));
	if (bRet  == false) {
		ERR_PRINT("SocketUtil::Bind() error!\n");
		return false;
	}

	struct ip_mreq stMcastGroup;
	bRet = NetworkUtil::IPv4AddressTextToBinary("239.255.255.250", &(stMcastGroup.imr_multiaddr));
	if (bRet  == false) {
		ERR_PRINT("NetworkUtil::IPv4AddressTextToBinary() error!\n");
		return false;
	}

	bRet = NetworkUtil::IPv4AddressTextToBinary("10.0.0.100", &(stMcastGroup.imr_interface));
	if (bRet  == false) {
		ERR_PRINT("NetworkUtil::IPv4AddressTextToBinary() error!\n");
		return false;
	}

	bRet = SocketUtil::SetSockOpt(nSocketFD, IPPROTO_IP, IP_ADD_MEMBERSHIP, &stMcastGroup, sizeof(stMcastGroup));
	if (bRet  == false) {
		ERR_PRINT("SocketUtil::SetSockOpt() error!\n");
		return false;
	}

	struct sockaddr_in stRemoteAddr;
	socklen_t stRemoteAddrLen;
	char sData[1024] = {0};
	unsigned int uRecvLen;
	DBG_PRINT("Received data:\n", sData);
	do {
		bRet = SocketUtil::RecvFrom(nSocketFD, sData, sizeof(sData), 0, (struct sockaddr*)&stRemoteAddr, &stRemoteAddrLen, &uRecvLen);
		DBG_PRINT("%s\n", sData);
		usleep(33);
	} while (true);

	return true;
}

int main(int argc, char* argv[])
{
	bool bRet = TestMulticastClient();
	if (bRet == false) {
		ERR_PRINT("TestMulticastClient() error!\n");
		return -1;
	}
	return 0;
}
