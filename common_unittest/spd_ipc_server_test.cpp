/*
 * spd_ipc_server_test.cpp
 *
 *  Created on: Mar 7, 2016
 *      Author: Jacken
 */
#include "debug.h"
#include "spd_ipc_server_test.h"
#include "socket_util.h"
#include "spd_cmd_server.h"
#include "spd_sig_util.h"

void IPCServerTest::Run()
{
	const char* sLocalPath = "/tmp/SI_CONFIG_BACKUP";
	CommandHandler CmdHdlr(sLocalPath);
	char sRecvBuf[1024] = {0};
	unsigned int uSigNo;
	do {
		if (SigUtil::Wait(0, &uSigNo)) {
			if ((uSigNo == SIGINT) || (uSigNo == SIGTERM)) {
				ERR_PRINT("Signal %s received. Terminating.\n", SigUtil::GetSignalString(uSigNo));
				break;
			}
		}

		if (CmdHdlr.Wait(100) == false) {
			continue;
		}

		if (CmdHdlr.Recv(sRecvBuf, 1024) == false) {
			continue;
		}
		DBG_PRINT("Recv command: %s\n", sRecvBuf);
		CmdHdlr.Reply(CommandHandler::REP_SUCCESS);
	} while(true);
}
