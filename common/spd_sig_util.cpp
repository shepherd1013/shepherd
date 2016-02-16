/*
 * spd_sig_util.cpp
 *
 *  Created on: Feb 15, 2016
 *      Author: Jacken
 */
#include <string.h>
#include <sys/signalfd.h>
#include <unistd.h>
#include "spd_sig_util.h"
#include "debug.h"

bool SigUtil::Block(int nSigNo)
{
	sigset_t mask;
	int nRet;
	if (sigaddset(&mask, nSigNo) < 0) {
		nRet = errno;
		ERR_PRINT("sigaddset() error:%s!\n", strerror(nRet));
		return false;
	}
	if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0) {
		int nRet = errno;
		ERR_PRINT("sigprocmask() error:%s!\n", strerror(nRet));
		return false;
	}
	return true;
}

const char* SigUtil::GetSignalString(int nSigNo)
{
	const char *sRet = strsignal(nSigNo);
	if (sRet == NULL) {
		ERR_PRINT("Invalid signal number (%d)!\n", nSigNo);
		return "";
	}
	return sRet;
}

bool SigUtil::Wait(time_t tMS, unsigned int *uSigNo)
{
	sigset_t mask;
	int nRet;
	int nFD;

	if (sigfillset(&mask) < 0) {
		nRet = errno;
		ERR_PRINT("sigfillset() error:%s!\n", strerror(nRet));
		return false;
	}

	nFD = signalfd(-1, &mask, 0);
	if (nFD == -1) {
		nRet = errno;
		ERR_PRINT("signalfd() error:%s!\n", strerror(nRet));
		return false;
	}

	struct timeval tv;
	tv.tv_sec = tMS / 1000;
	tv.tv_usec = (tMS % 1000) * 1000;

	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(nFD, &readfds);

	nRet = select(nFD + 1, &readfds, NULL, NULL, &tv);
	if (nRet == 0) { // Timeout
		return false;
	}
	if (nRet < 0) {
		nRet = errno;
		ERR_PRINT("select() error:%s!\n", strerror(nRet));
		return false;
	}

	struct signalfd_siginfo SigInfo;
	unsigned int uSigInfoSize = sizeof(struct signalfd_siginfo);
	ssize_t nReadSize = read(nFD, &SigInfo, uSigInfoSize);
	DBG_PRINT("Read size: %d\n", nReadSize);
	if (nReadSize != (int)uSigInfoSize) {
		ERR_PRINT("Invalid size (%d) of reading signal info!\n", nReadSize);
		return false;
	}
	DBG_PRINT("Signal no:%u\n", SigInfo.ssi_signo);
	*uSigNo = SigInfo.ssi_signo;
	return true;
}
