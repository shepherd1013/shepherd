/*
 * main.cpp
 *
 *  Created on: Jul 23, 2015
 *      Author: jacken
 */

#include <iostream>
#include "time_util.h"
#include "debug.h"
#include <time.h>
#include "socket_util.h"
#include <string.h>
#include <sys/un.h>
#include <stddef.h>

bool TestSocketIPCClient(const char* sIPCPath)
{
	int nSocketFD;
	bool bRet;

	bRet = SocketUtil::Socket(AF_UNIX, SOCK_DGRAM, 0, &nSocketFD);
	if (bRet == false) {
		ERR_PRINT("SocketUtil::Socket() error!\n");
		return false;
	}

	struct sockaddr_un sRemote;
	socklen_t RemoteLength = sizeof(sRemote);
	memset(&sRemote, 0, sizeof(sRemote));
	sRemote.sun_family = AF_UNIX;
	strncpy(sRemote.sun_path, sIPCPath, sizeof(sRemote.sun_path));
	bRet = SocketUtil::Connect(nSocketFD, (struct sockaddr *)&sRemote, RemoteLength);
	if (bRet == false) {
		ERR_PRINT("SocketUtil::Connect() error!\n");
		return false;
	}

	char sSendData[1024] = {0};
	strncpy(sSendData, "123456789", sizeof(sSendData));

	do {
//		int nRet = sendto(nSocketFD, sSendData, strlen(sSendData), 0, (struct sockaddr*)&sRemote, RemoteLength);
		int nRet = send(nSocketFD, sSendData, strlen(sSendData), 0);
		if(nRet < 0){
			nRet = errno;
			ERR_PRINT("%s\n", strerror(nRet));
			return false;
		}
	} while (true);
	bRet = SocketUtil::Close(nSocketFD);
	if (bRet == false) {
		ERR_PRINT("SocketUtil::Close() error!\n");
		return false;
	}
	return true;
}

int main(int argc, char *argv[])
{
	if (TestSocketIPCClient("/tmp/IPC.Socket") == false) {
		ERR_PRINT("TestSocketIPCClient(%s) error!\n", argv[1]);
		return -1;
	}

	return 0;
}
