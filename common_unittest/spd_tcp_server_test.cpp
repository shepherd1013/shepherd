/*
 * spd_tcp_server_test.cpp
 *
 *  Created on: Mar 22, 2016
 *      Author: Jacken
 */

#include "spd_tcp_server_test.h"
#include "spd_tcp_server.h"
#include "spd_sig_util.h"
#include "debug.h"

TcpServerTest::TcpServerTest()
{
}

TcpServerTest::~TcpServerTest()
{
}

void TcpServerTest::Run()
{
	const char* sIP = NULL;
	unsigned int uPort = 8877;
	char sBuf[4096] = {0};
	unsigned int uBufSize = sizeof(sBuf);
	unsigned int uSigNo;
	TcpServer tcp(sIP, uPort);

	if (tcp.IsInitPass() == false) {
		ERR_PRINT("Initialize TCP server failed!\n");
		return;
	}

	while (true) {
		if (SigUtil::Wait(0, &uSigNo)) {
			if ((uSigNo == SIGINT) || (uSigNo == SIGTERM)) {
				ERR_PRINT("Signal %s received. Terminating.\n", SigUtil::GetSignalString(uSigNo));
				break;
			}
		}

		if (tcp.Wait(100)) {
			tcp.Accept();
		}

		if (tcp.WaitClient(0) == false) {
			continue;
		}
		if (tcp.Recv(sBuf, uBufSize) == false) {
			tcp.CloseClient();
			continue;
		}
		DBG_PRINT("Received data: %s\n", sBuf);
	}
}
