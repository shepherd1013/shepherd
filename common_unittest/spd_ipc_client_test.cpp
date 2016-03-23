/*
 * spd_ipc_client_test.cpp
 *
 *  Created on: Mar 11, 2016
 *      Author: Jacken
 */
#include "spd_ipc_client_test.h"
#include "socket_util.h"
#include "debug.h"


void IPCClientTest::Run()
{
	const char* sLocal = "/tmp/SI_RESTORER_REPLY_TEST";
	const char* sRemote = "/tmp/SI_RESTORER";
	const char* sCmd = "action=load_default";
	SocketIPC client(sLocal, sRemote);
	if (client.Send(sCmd, strlen(sCmd)) == false){
		return;
	}
	char sBuf[512] = {0};
	do {
		if(client.Wait(100) == false) {
			continue;
		}
		if (client.Recv(sBuf, sizeof(sBuf)) == false) {
			continue;
		}
		DBG_PRINT("sBuf: %s\n", sBuf);
		break;
	} while (true);
}


