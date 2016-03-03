/*
 * spd_msgq_utest.cpp
 *
 *  Created on: Mar 2, 2016
 *      Author: Jacken
 */
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>
#include "debug.h"
#include "spd_msgq_util.h"
#include "spd_msgq_utest.h"
#include "time_util.h"
#include "spd_sig_util.h"

void MsgQUTest::Run()
{
	if (this->TestRecv() == false) {
		ERR_PRINT("this->TestRecv() error!\n");
		return;
	}

//	if (this->TestSend() == false) {
//		ERR_PRINT("this->TestSend() error!\n");
//		return;
//	}
}

bool MsgQUTest::TestRecv()
{
	MsgQ msgq(0x103, true);
	char sRecvBuf[MAX_MSGQ_TEXT] = {0};
	unsigned int uRecvDataSize;
	unsigned int uSigNo;
	do {
		uRecvDataSize = 0;
		if (msgq.Recv(sRecvBuf, sizeof(sRecvBuf), &uRecvDataSize) == false) {
			return false;
		}
		if (uRecvDataSize > 0) {
			DBG_PRINT("Received data: %s\n", sRecvBuf);
		}
		if (SigUtil::Wait(0, &uSigNo)) {
			if ((uSigNo == SIGINT) || (uSigNo == SIGTERM)) {
				ERR_PRINT("Signal %s received. Terminating.\n", SigUtil::GetSignalString(uSigNo));
				break;
			}
		}
		usleep(100 * 1000);
	} while (true);
	return true;
}

bool MsgQUTest::TestSend()
{
	MsgQ msgq(0x103, false);
//	const char* sData = "event=system_reboot&note=gg";
	const char* sData = "1234567890";
//	char sData[2048] = {0};
	if(msgq.Send(sData, strlen(sData), 500) == false) {
		return false;
	}
	return true;
}
