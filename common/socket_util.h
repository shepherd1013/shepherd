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
	static bool Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	static bool Close(int sockfd);
	static bool SetSockOpt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
	static bool GetSocketOpt(int sockfd, int level, int optname, void *optval, socklen_t *optlen);
	static bool Wait(time_t tMS, vector<int> sRegisterFD, int* sEventFD); // Millisecond
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

};

class Socket
{
public:
	Socket();
	~Socket();

protected:
	int m_sFD;

	int GetFD();
};

class SocketIPC: public Socket
{
public:
	SocketIPC();
	~SocketIPC();

protected:
	string sIPCPath;
};

class SocketIPCServer: public SocketIPC
{
public:
	SocketIPCServer();
	~SocketIPCServer();

protected:
	string sIPCPath;
};

class SocketIPCClient: public SocketIPC
{
public:
	SocketIPCClient();
	~SocketIPCClient();

protected:
};


#endif /* SOCKET_UTIL_H_ */
