/*
 * msgq_util.cpp
 *
 *  Created on: Feb 22, 2016
 *      Author: Jacken
 */
#include <sys/msg.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "debug.h"
#include "spd_msgq_util.h"
#include "time_util.h"

MsgQ::MsgQ()
:m_nKey(-1)
{
	DBG_PRINT("Run %s()...\n", __FUNCTION__);
}

MsgQ::~MsgQ()
{
	DBG_PRINT("Run %s()...\n", __FUNCTION__);
	if (m_bCreation) {
		int nRet = msgctl(m_nID, IPC_RMID, NULL);
		if (nRet < 0) {
			nRet = errno;
			ERR_PRINT("msgctl() error: %s!\n", strerror(nRet));
			return;
		}
	}
}

MsgQ::MsgQ(unsigned int uKey, bool bCreation)
{
	DBG_PRINT("Run %s()...\n", __FUNCTION__);
	m_bCreation = bCreation;
	m_nKey = uKey;
	if (bCreation) {
		if (this->LoadKey(uKey) == false) {
			ERR_PRINT("Load key (0x%x) failed!\n", uKey);
			return;
		}
	} else {
		if (this->Connect() == false) {
			ERR_PRINT("Connect key (0x%x) failed!\n", m_nKey);
			return;
		}
	}
}

bool MsgQ::LoadKey(unsigned int uKey)
{
	m_nID = msgget(uKey, IPC_CREAT | IPC_EXCL | 0666);
	if (m_nID < 0) {
		m_nID = errno;
		ERR_PRINT("msgget() error: %s!\n", strerror(m_nID));
		return false;
	}
	return true;
}

bool MsgQ::Connect()
{
	m_nID = msgget(m_nKey, 0);
	if (m_nID < 0) {
		m_nID = errno;
		ERR_PRINT("msgget() error: %s!\n", strerror(m_nID));
		return false;
	}
	return true;
}

bool MsgQ::Send(const char* sSndData, unsigned int uDataSize, unsigned int uTimeoutMSec)
{
	int nRet = 0;
	struct timespec tStartTime;
	struct timespec tCurrentTime;
	struct timespec tDiffTime;

	struct msgbuf {
		long mtype;					/* message type, must be > 0 */
		char mtext[MAX_MSGQ_TEXT];	/* message data */
	} SndData;
	SndData.mtype = 1;
//	strncpy(SndData.mtext, sSndData, MAX_MSGQ_TEXT);
	if (uDataSize > MAX_MSGQ_TEXT) {
		memcpy(SndData.mtext, sSndData, MAX_MSGQ_TEXT);
	} else {
		memcpy(SndData.mtext, sSndData, uDataSize);
	}

	if (TimeUtil::GetUptime(&tStartTime) == false) {
		ERR_PRINT("Get uptime failed!\n");
		return false;
	}

	do {
		nRet = msgsnd(m_nID, &SndData, uDataSize, IPC_NOWAIT);
		if (nRet < 0) {
			nRet = errno;
			if (nRet != EAGAIN) {
				ERR_PRINT("msgsnd() error: %s!\n", strerror(nRet));
				return false;
			}
			if (uTimeoutMSec != 0) {
				usleep(50000);
			}
			if (TimeUtil::GetUptime(&tCurrentTime) == false) {
				ERR_PRINT("Get uptime failed!\n");
				return false;
			}
			tDiffTime = TimeUtil::TimeDiff(tCurrentTime, tStartTime);
			unsigned long int uTimeDiffMSec = tDiffTime.tv_sec * 1000 + tDiffTime.tv_nsec / 1000000;
			if ((unsigned int)uTimeDiffMSec >= uTimeoutMSec) {
				ERR_PRINT("Send message timeout!\n");
				return false;
			}
		}
	} while (nRet);
	return true;
}

bool MsgQ::Recv(char* sRecvBuf, unsigned int uBufSize, unsigned int* uRecvSize)
{
	/*
	 * The argument msgtyp specifies the type of message requested as follows:
	 * 1.If msgtyp is 0, then the first message in the queue is read.
	 * 2.If msgtyp is greater than 0, then the first message in the queue of type msgtyp is read,
	 * unless MSG_EXCEPT was specified in msgflg, in which case the first message in the queue of type not equal to msgtyp will be read.
	 * 3.If msgtyp is less than 0, then the first message in the queue with the lowest type less than or equal to the absolute value of msgtyp will be read.
	 */
	struct msgbuf {
		long mtype;					/* message type, must be > 0 */
		char mtext[MAX_MSGQ_TEXT];	/* message data */
	} RecvData;
	int nRet = msgrcv(m_nID, &RecvData, MAX_MSGQ_TEXT, 0, IPC_NOWAIT | MSG_NOERROR);
	if (nRet < 0) {
		nRet = errno;
		if (nRet != ENOMSG) {
			ERR_PRINT("msgrcv() error: %s!, errno: %d\n", strerror(nRet), nRet);
			return false;
		}
		*uRecvSize = 0;
	} else {
		DBG_PRINT("msgrcv() result: %d\n", nRet);
		*uRecvSize = nRet;
		if (*uRecvSize >= uBufSize) {
			memcpy(sRecvBuf, RecvData.mtext, uBufSize);
		} else {
			memcpy(sRecvBuf, RecvData.mtext, *uRecvSize);
		}
//		strncpy(sRecvBuf, RecvData.mtext, MAX_MSGQ_TEXT);
	}
	return true;
}
