/*
 * socket_util_wrapper.cpp
 *
 *  Created on: Sep 2, 2015
 *      Author: Jacken
 */
#include "debug.h"
#include "socket_util.h"

#ifndef BOOL
#define BOOL	int
#define TRUE	1
#define FALSE	0
#endif

BOOL SocketUtil_DuplicateFD(int nOldFD, int *nNewFD)
{
	bool bRet = SocketUtil::DuplicateFD(nOldFD, nNewFD);
	if (bRet == false) {
		ERR_PRINT("SocketUtil::DuplicateFD() error!\n");
		return FALSE;
	}
	return TRUE;
}


