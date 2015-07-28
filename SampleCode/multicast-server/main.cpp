/*
 * main.cpp
 *
 *  Created on: Jul 28, 2015
 *      Author: Jacken
 */

#include "network_util.h"
#include "debug.h"

bool TestMulticastServer()
{
	const char *sMcastIP = "239.255.255.250";
	const char *sLocalIP = "10.0.0.100";
	unsigned int uPort = 3702;
	char sData[1024] = {0};

	snprintf(sData, sizeof(sData), "Hello!");

	bool bRet = NetworkUtil::Multicast(sLocalIP, sMcastIP, uPort, sData, sizeof(sData));
	if (bRet == false) {
		ERR_PRINT("NetworkUtil::Multicast() error!\n");
		return false;
	}
	return true;
}

int main(int argc, char* argv[])
{
	bool bRet = TestMulticastServer();
	if (bRet == false) {
		ERR_PRINT("TestMulticastServer() error!\n");
		return -1;
	}
	return 0;
}
