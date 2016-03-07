/*
 * string_util.cpp
 *
 *  Created on: Sep 18, 2015
 *      Author: Jacken
 */
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "debug.h"
#include "string_util.h"

bool StringUtil::Snprintf(char *s, size_t n, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int nRet = snprintf(s, n, format, args);
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
	*pOutput = strtoul(sInput, &pEnd, nBase);
	int nRet = errno;
	if (nRet != 0) {
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
	*pOutput = strtol(sInput, &pEnd, nBase);
	int nRet = errno;
	if (nRet != 0) {
		ERR_PRINT("strtoul() error: %s!\n", strerror(nRet));
		return false;
	}
	if (*pEnd != '\0') {
		ERR_PRINT("Invalid digits (%s)!\n", pEnd);
		return false;
	}
	return true;
}
