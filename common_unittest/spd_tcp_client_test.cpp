/*
 * spd_tcp_client_test.cpp
 *
 *  Created on: Mar 22, 2016
 *      Author: Jacken
 */

#include "spd_tcp_client_test.h"
#include "spd_tcp_client.h"
#include "spd_sig_util.h"
#include "debug.h"
#include "string_util.h"

TcpClientTest::TcpClientTest()
{
}

TcpClientTest::~TcpClientTest()
{
}

void TcpClientTest::Run()
{
	const char* sIP = "192.168.6.131";
	unsigned int uPort = 8877;
	char sBuf[4096] = {0};
	unsigned int uBufSize = sizeof(sBuf);
	unsigned int uSigNo;
	TcpClient tcp(sIP, uPort, 5566);

	int nSendBufSize = 1024;
	socklen_t optlen = sizeof(nSendBufSize);

	if (SocketUtil::SetSockOpt(tcp.GetFD(), SOL_SOCKET, SO_SNDBUF, &nSendBufSize, optlen) == false) {
		ERR_PRINT("Get socket option error!\n");
		return;
	}

	if (SocketUtil::GetSocketOpt(tcp.GetFD(), SOL_SOCKET, SO_SNDBUF, &nSendBufSize, &optlen) == false) {
		ERR_PRINT("Get socket option error!\n");
		return;
	}
	DBG_PRINT("nSendBufSize: %d\n", nSendBufSize);

	if (StringUtil::Snprintf(sBuf, uBufSize, "01234567890123456789012345678901234567890123456789") == false) {
		ERR_PRINT("Snprintf() error!\n");
		return;
	}
	unsigned int uDataLen = strlen(sBuf);
	do {
		if (tcp.Send(sBuf, uDataLen) == false) {
			ERR_PRINT("TCP send error!\n");
			break;
		}
		DBG_PRINT("Send data: %s\n", sBuf);
		if (SigUtil::Wait(0, &uSigNo)) {
			if ((uSigNo == SIGINT) || (uSigNo == SIGTERM)) {
				ERR_PRINT("Signal %s received. Terminating.\n", SigUtil::GetSignalString(uSigNo));
				break;
			}
		}
		usleep(1000 * 1000);
	} while (true);
}
