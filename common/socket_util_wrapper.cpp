/*
 * socket_util_wrapper.cpp
 *
 *  Created on: Sep 2, 2015
 *      Author: Jacken
 */
#include "debug.h"
#include "socket_util.h"
#include "common_define.h"

BOOL SocketUtil_DuplicateFD(int nOldFD, int *nNewFD)
{
	bool bRet = SocketUtil::DuplicateFD(nOldFD, nNewFD);
	if (bRet == false) {
		ERR_PRINT("SocketUtil::DuplicateFD() error!\n");
		return FALSE;
	}
	return TRUE;
}

BOOL SocketUtil_Socket(int domain, int type, int protocol, int *sFD)
{
	bool bRet = SocketUtil::Socket(domain, type, protocol, sFD);
	if (bRet == false) {
		ERR_PRINT("SocketUtil::DuplicateFD() error!\n");
		return FALSE;
	}
	return TRUE;
}


