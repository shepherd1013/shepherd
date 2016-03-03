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

struct timespec TimeUtil::TimeDiff(struct timespec tTimeA, struct timespec tTimeB)
{
	struct timespec tTime;
	long int nNanoSecDiff;
	if (tTimeA.tv_sec >= tTimeB.tv_sec) {
		tTime.tv_sec = tTimeA.tv_sec - tTimeB.tv_sec;
		nNanoSecDiff = tTimeA.tv_nsec - tTimeB.tv_nsec;
	} else {
		tTime.tv_sec = tTimeB.tv_sec - tTimeA.tv_sec;
		nNanoSecDiff = tTimeB.tv_nsec - tTimeA.tv_nsec;
	}
	if (nNanoSecDiff < 0) {
		tTime.tv_sec--;
		tTime.tv_nsec = 1000000000 + nNanoSecDiff;
	} else {
		tTime.tv_nsec = nNanoSecDiff;
	}
//	DBG_PRINT("Time diff: %ld.%ld\n", tTime.tv_sec, tTime.tv_nsec);
	return tTime;
}
