/*
 * socket_util.h
 *
 *  Created on: Jul 19, 2015
 *      Author: jacken
 */

#ifndef SOCKET_UTIL_H_
#define SOCKET_UTIL_H_
#include <unistd.h>
#include <time.h>
#include <vector>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>

using namespace std;

class SocketUtil
{
public:
	static bool Socket(int domain, int type, int protocol, int *sFD);
	static bool Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	static bool Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	static bool Close(int sockfd);
	static bool Setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
	static bool Wait(time_t tMS, vector<int> sRegisterFD, vector<int> &sEventFD); // Millisecond
};

class Socket
{
public:
	Socket();
	~Socket();

	virtual bool Start() = 0;

protected:
	int m_sFD;

	int GetFD();
	bool Wait(time_t tMS); // Millisecond
};

class SocketIPC: public Socket
{
public:
	SocketIPC();
	~SocketIPC();

	virtual bool Start();
protected:
	string sIPCPath;
};

class SocketIPCServer: public Socket
{
public:
	SocketIPCServer();
	~SocketIPCServer();

	virtual bool Start();
protected:
	string sIPCPath;
};

class SocketIPCClient: public SocketIPC
{
public:
	SocketIPCClient();
	~SocketIPCClient();

	virtual bool Start();
protected:
};


#endif /* SOCKET_UTIL_H_ */
