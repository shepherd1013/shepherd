/*
 * string_util.cpp
 *
 *  Created on: Sep 18, 2015
 *      Author: Jacken
 */
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "debug.h"
#include "string_util.h"

bool StringUtil::Snprintf(char* sBuf, size_t uBufSize, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    int nRet = vsnprintf(sBuf, uBufSize, format, args);
    va_end(args);
	if (nRet < 0) {
		ERR_PRINT("snprintf() error!\n");
		return false;
	}
	return true;
}

bool StringUtil::StrToULInt(const char* sInput, unsigned long int *pOutput, int nBase)
{
	char* pEnd;
	int nRet = 0;
	errno = 0;
	*pOutput = strtoul(sInput, &pEnd, nBase);
	nRet = errno;
	if ((nRet == ERANGE && (*pOutput == ULONG_MAX)) || (nRet != 0 && *pOutput == 0)) {
		ERR_PRINT("strtoul() error: %s!\n", strerror(nRet));
		return false;
	}
	if (*pEnd != '\0') {
		ERR_PRINT("Invalid digits (%s)!\n", pEnd);
		return false;
	}
	return true;
}

bool StringUtil::StrToLInt(const char* sInput, long int *pOutput, int nBase)
{
	char* pEnd;
	int nRet = 0;
	errno = 0;
	*pOutput = strtol(sInput, &pEnd, nBase);
	nRet = errno;
	if ((nRet == ERANGE && (*pOutput == LONG_MAX || *pOutput == LONG_MIN)) || (nRet != 0 && *pOutput == 0)) {
		ERR_PRINT("strtol() error: %s!\n", strerror(nRet));
		return false;
	}
	if (*pEnd != '\0') {
		ERR_PRINT("Invalid digits (%s)!\n", pEnd);
		return false;
	}
	return true;
}
