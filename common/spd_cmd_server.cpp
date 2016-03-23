/*
 * spd_cmd_server.cpp
 *
 *  Created on: Feb 16, 2016
 *      Author: Jacken
 */
#include "debug.h"
#include "spd_cmd_server.h"
#include <stdarg.h>

static char sReply[CommandHandler::REP_MAX][64] = {
		"success",
		"error",
		"unsupport",
		"timeout",
		"busy",
		"heartbeat",
};

CommandHandler::CommandHandler()
{
	DBG_PRINT("Run %s() ...\n", __FUNCTION__);
}

CommandHandler::CommandHandler(const char *sLocalPath): SocketIPC(sLocalPath, NULL)
{
	DBG_PRINT("Run %s() ...\n", __FUNCTION__);
}

bool CommandHandler::Reply(enum REPLY eReply)
{
	unsigned int uBufSize = sizeof(m_sBuf);
	int nRet = snprintf(m_sBuf, uBufSize, "reply=%s", sReply[eReply]);
	if (nRet < 0) {
		ERR_PRINT("snprintf() error!\n");
		return false;
	}
	if (this->Send(m_sBuf, nRet) == false) {
		return false;
	}
	return true;
}

bool CommandHandler::Reply(enum REPLY eReply, const char* sFormat, ...)
{
	unsigned int uBufSize = sizeof(m_sBuf);
    va_list args;

    char sBuf[256] = {0};
    va_start(args,sFormat);
    int nRet = vsnprintf(sBuf, sizeof(sBuf), sFormat, args);
    va_end(args);
	if (nRet < 0) {
		ERR_PRINT("snprintf() error!\n");
		return false;
	}

	nRet = snprintf(m_sBuf, uBufSize, "reply=%s%s", sReply[eReply], sBuf);
	if (nRet < 0) {
		ERR_PRINT("snprintf() error!\n");
		return false;
	}
	if (this->Send(m_sBuf, nRet) == false) {
		return false;
	}
	return true;
}
