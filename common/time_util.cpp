/*
 * time_util.cpp
 *
 *  Created on: Jul 7, 2015
 *      Author: Jacken
 */
#include <errno.h>
#include <string.h>
#include "time_util.h"
#include "debug.h"

bool TimeUtil::GetUptime(struct timespec *tTime)
{
	int nRet = clock_gettime(CLOCK_MONOTONIC, tTime);
	if (nRet < 0) {
		nRet = errno;
		ERR_PRINT("clock_gettime() error: %s\n", strerror(nRet));
		return false;
	}
	return true;
}
