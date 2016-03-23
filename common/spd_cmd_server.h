/*
 * spd_cmd_server.h
 *
 *  Created on: Feb 16, 2016
 *      Author: Jacken
 */

#ifndef SPD_CMD_SERVER_H_
#define SPD_CMD_SERVER_H_

#include "socket_util.h"

class CommandHandler: public SocketIPC
{
public:
	CommandHandler();
	CommandHandler(const char *sLocalPath);
	enum REPLY {
		REP_SUCCESS		= 0,
		REP_ERROR		= 1,
		REP_UNSUPPORT	= 2,
		REP_TIMEOUT		= 3,
		REP_BUSY		= 4,
		REP_HEARTBEAT	= 5,
		REP_MAX
	};
	bool Reply(enum REPLY eReply);
	bool Reply(enum REPLY eReply, const char* sFormat, ...);
protected:
	char m_sBuf[4096];
};

#endif /* SPD_CMD_SERVER_H_ */
