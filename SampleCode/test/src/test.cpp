//============================================================================
// Name        : test.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "time_util.h"
#include "debug.h"
#include <time.h>
#include "socket_util.h"
#include <string.h>
#include <sys/un.h>
#include <stddef.h>

using namespace std;

bool TestTime()
{
	struct timespec tTime;
	bool bRet;
	bRet = TimeUtil::GetUptime(&tTime);

	if (!bRet) {
		ERR_PRINT("TimeUtil::GetUptime() error!\n");
		return false;
	}

	DBG_PRINT("tTime.tv_sec:%lu\n", tTime.tv_sec);
	DBG_PRINT("tTime.tv_nsec:%ld\n", tTime.tv_nsec);
	DBG_PRINT("Uptime: %lu minute\n", tTime.tv_sec/60);
	return true;
}

bool TestSocketIPCServer(const char* sIPCPath)
{
	int nSocketFD;
	bool bRet;

	bRet = SocketUtil::Socket(AF_UNIX, SOCK_DGRAM, 0, &nSocketFD);
	if (bRet == false) {
		ERR_PRINT("SocketUtil::Socket() error!\n");
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
	int nRet = recvfrom(nSocketFD, sRecvData, sizeof(sRecvData), 0, (struct sockaddr*)&sRemote, &RemoteLength);
	if(nRet < 0){
		nRet = errno;
		ERR_PRINT("%s\n", strerror(nRet));
		return false;
	}

	DBG_PRINT("sRecvData:%s\n", sRecvData);

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
	int nRet = sendto(nSocketFD, sSendData, strlen(sSendData), 0, (struct sockaddr*)&sRemote, RemoteLength);
	if(nRet < 0){
		nRet = errno;
		ERR_PRINT("%s\n", strerror(nRet));
		return false;
	}


	bRet = SocketUtil::Close(nSocketFD);
	if (bRet == false) {
		ERR_PRINT("SocketUtil::Close() error!\n");
		return false;
	}
	return true;
}

int main(int argc, char *argv[])
{
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	if (TestTime() == false){
		ERR_PRINT("TestTime() error!\n");
		return -1;
	}

	argv[1] = "IPC.Socket";

//	if (TestSocketIPCServer(argv[1]) == false){
//		ERR_PRINT("TestSocketIPCServer(%s) error!\n", argv[1]);
//		return -1;
//	}

	if (TestSocketIPCClient(argv[1]) == false){
		ERR_PRINT("TestSocketIPCClient(%s) error!\n", argv[1]);
		return -1;
	}

	return 0;
}
