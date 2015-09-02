/*
 * driver_util.cpp
 *
 *  Created on: Aug 28, 2015
 *      Author: root
 */
#include "driver_util.h"
#include "debug.h"
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

bool Driver::Open(const char *filename, int flags, int *nFD)
{
	*nFD = open(filename, flags);
	if (*nFD < 0) {
		*nFD = errno;
		ERR_PRINT("%s\n", strerror(*nFD));
		return false;
	}
	return true;
}

bool Driver::Close(int nFD)
{
	int nRet = close(nFD);
	if (nRet < 0) {
		nRet = errno;
		ERR_PRINT("%s\n", strerror(nRet));
		return false;
	}
	return true;
}

bool Driver::Ioctl(int nFD, unsigned long int uReq, void *pData)
{
	int nRet = ioctl(nFD, uReq, pData);
	if (nRet < 0) {
		nRet = errno;
		ERR_PRINT("%s\n", strerror(nRet));
		return false;
	}
	return true;
}
