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
#include <list>
#include <sys/un.h>

using namespace std;

class SocketUtil
{
public:
	static bool Socket(int domain, int type, int protocol, int *sFD);
	/*
	 * domain:
	 * 	AF_UNIX 	- Local communication
	 * 	AF_INET		- IPv4 Internet protocols
	 * 	AF_INET6	- IPv6 Internet protocols
	 *
	 * type:
	 * 	SOCK_STREAM	- Provides sequenced, reliable, two-way, connection-based byte streams. An out-of-band data transmission mechanism may be supported.
	 * 	SOCK_DGRAM	- Supports datagrams (connectionless, unreliable messages of a fixed maximum length).
	 * 	SOCK_RAW	- Provides raw network protocol access.
	 */
	static bool Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	static bool BindInterface(int nSocketFD, const char *sInterfaceName);
	static bool BindPortOnly(int sockfd, unsigned int port);
	static bool BindPortOnly(int sockfd, unsigned int port, const char* sIP);
	static bool Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	static bool Close(int sockfd);
	static bool SetSockOpt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
	static bool GetSocketOpt(int sockfd, int level, int optname, void *optval, socklen_t *optlen);
	static bool Wait(time_t tMS, vector<int> sRegisterFD, int* sEventFD); // Millisecond
	static bool Wait(time_t tMS, list<int> sRegisterFD, int* sEventFD); // Millisecond
	static bool RecvFrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *RemoteAddr, socklen_t *RemoteAddrLen, unsigned int *uRecvDataLen);
	/*
	 * RecvFrom() Prototype:
	 * 	ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);
	 */
	static bool SendTo(int sockfd, const void *buf, size_t len, int flags, struct sockaddr *RemoteAddr, socklen_t RemoteAddrLen);
	/*
	 * SendTo() Prototype:
	 * 	ssize_t sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);
	 */
	static bool DuplicateFD(int nOldFD, int *nNewFD);
	static bool Listen(int sockfd, int nPendingNum);
	static bool Accept(int sockfd, struct sockaddr *RemoteAddr, socklen_t *RemoteAddrLen, int *nAcceptedFD);
	static bool Send(int sockfd, const void *buf, size_t len, int flags);
};

class Socket
{
public:
	Socket();
	~Socket();
	int GetFD();
protected:
	int m_sFD;
};

class SocketIPC: public Socket
{
public:
	SocketIPC();
	~SocketIPC();

protected:
};

class SocketIPCServer: public SocketIPC
{
public:
	SocketIPCServer();
	SocketIPCServer(const char *sLocalPath);
	~SocketIPCServer();

	bool Wait(time_t tMS);
	bool Recv();
	bool Recv(char* sBuf, unsigned int uBufSize);
	bool Send(const char *SendData, unsigned int uDataSize);
	bool Connect(const char *sLocalPath);

protected:
	char			m_sBuf[4096];
	sockaddr_un		m_unRemoteAddr;
	socklen_t		m_uRemoteAddrLen;
};

class SocketIPCClient: public SocketIPC
{
public:
	SocketIPCClient(const char *sLocalPath, const char *sRemotePath);
	~SocketIPCClient();

	bool Send(const char *SendData, unsigned int uDataSize);
	bool Recv(char* sBuf, unsigned int uBufSize);
	bool Connect(const char *sLocalPath, const char *sRemotePath);
protected:
	sockaddr_un		m_unRemoteAddr;
	socklen_t		m_uRemoteAddrLen;
};


#endif /* SOCKET_UTIL_H_ */
