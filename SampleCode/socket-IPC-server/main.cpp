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

bool TestSocketIPCServer(const char* sIPCPath)
{
	int nSocketFD;
	bool bRet;

	bRet = SocketUtil::Socket(AF_UNIX, SOCK_DGRAM, 0, &nSocketFD);
	if (bRet == false) {
		ERR_PRINT("SocketUtil::Socket() error!\n");
		return false;
	}

	int nRet = unlink(sIPCPath);
	if ( (nRet < 0) && (errno != ENOENT) ) {
		nRet = errno;
		ERR_PRINT("nRet: %d\n", nRet);
		ERR_PRINT("%s\n", strerror(nRet));
		return false;
	}

	int size;
	struct sockaddr_un un;
	memset(&un, 0, sizeof(un));
	strncpy(un.sun_path, sIPCPath, sizeof(un.sun_path));
	un.sun_family = AF_UNIX;
	size = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);
	bRet = SocketUtil::Bind(nSocketFD, (struct sockaddr *)&un, size);
	if (bRet == false) {
		ERR_PRINT("SocketUtil::Bind() error!\n");
		return false;
	}

	char sRecvData[1024] = {0};
	struct sockaddr_un sRemote;
	socklen_t RemoteLength;
	do {
//		nRet = recvfrom(nSocketFD, sRecvData, sizeof(sRecvData), 0, (struct sockaddr*)&sRemote, &RemoteLength);
		nRet = recv(nSocketFD, sRecvData, sizeof(sRecvData), 0);
		if(nRet < 0){
			nRet = errno;
			ERR_PRINT("%s\n", strerror(nRet));
			return false;
		}
		DBG_PRINT("sRecvData:%s\n", sRecvData);
	} while(true);

	bRet = SocketUtil::Close(nSocketFD);
	if (bRet == false) {
		ERR_PRINT("SocketUtil::Close() error!\n");
		return false;
	}

	nRet = unlink(sIPCPath);
	if (nRet < 0) {
		nRet = errno;
		ERR_PRINT("%s\n", strerror(nRet));
		return false;
	}
	return true;
}

int main(int argc, char *argv[])
{
	if (TestSocketIPCServer("/tmp/IPC.Socket") == false) {
		ERR_PRINT("TestSocketIPCServer(%s) error!\n", argv[1]);
		return -1;
	}

	return 0;
}
