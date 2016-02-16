/*
 * spd_cmd_server.cpp
 *
 *  Created on: Feb 16, 2016
 *      Author: Jacken
 */
#include "debug.h"
#include "spd_cmd_server.h"

static char sReply[CommandHandler::REP_MAX][64] = {
		"success",
		"error",
		"unsupport",
		"timeout",
		"busy",
		"heartbeat",
};

CommandHandler::CommandHandler(const char *sLocalPath): SocketIPCServer(sLocalPath)
{

}

bool CommandHandler::Reply(REPLY eReply)
{
	unsigned int uBufSize = sizeof(sBuf);
	int nRet = snprintf(sBuf, uBufSize, "reply=%s", sReply[eReply]);
	if (nRet < 0) {
		ERR_PRINT("snprintf() error!\n");
		return false;
	}
	if (this->Send(sBuf, nRet) == false) {
		return false;
	}
	return true;
}
