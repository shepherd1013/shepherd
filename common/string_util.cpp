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
#include <math.h>
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

bool StringUtil::Replace(const char* OldStr, char* sNewStr, unsigned int uNewStrBufSize, const char* sOldKey, const char* sNewKey)
{
	if (OldStr == NULL || sNewStr == NULL || uNewStrBufSize == 0 || sOldKey == NULL || sNewKey == NULL) {
		ERR_PRINT("Invalid argument!\n");
		return false;
	}
	bzero(sNewStr, uNewStrBufSize);
	const char *pStart = OldStr;
	const char *pFound = NULL;
	unsigned int uStrLen = 0;
	unsigned int uTotalLen = 0;
	unsigned int uOldKeyLen = strlen(sOldKey);
	unsigned int uNewKeyLen = strlen(sNewKey);
	do {
		pFound = strstr(pStart, sOldKey);
		if (pFound == NULL) {
			if (*pStart != '\0') {
				uTotalLen += strlen(pStart);
				if (uTotalLen > uNewStrBufSize) {
					ERR_PRINT("The buffer size (%u) isn't enough!\n", uNewStrBufSize);
					return false;
				}
				strcat(sNewStr, pStart);
			}
			break;
		}
		uStrLen = (unsigned int)(pFound - pStart);
		if(uStrLen != 0) {
			uTotalLen += uStrLen;
			if (uTotalLen > uNewStrBufSize) {
				ERR_PRINT("The buffer size (%u) isn't enough!\n", uNewStrBufSize);
				return false;
			}
			strncat(sNewStr, pStart, uStrLen);
		}
		if (uNewKeyLen != 0) {
			uTotalLen += uNewKeyLen;
			if (uTotalLen > uNewStrBufSize) {
				ERR_PRINT("The buffer size (%u) isn't enough!\n", uNewStrBufSize);
				return false;
			}
			strcat(sNewStr, sNewKey);
		}
		pStart = pFound + uOldKeyLen;
	} while (true);
	return true;
}

bool StringUtil::StrToDouble(const char* sIn, double* dOut)
{
	char* pEnd;
	errno = 0;
	*dOut = strtod(sIn, &pEnd);
	int nRet = errno;
	if ((nRet == ERANGE && *dOut == HUGE_VAL) || (nRet != 0 && *dOut == 0)) {
		ERR_PRINT("strtod() error: %s!\n", strerror(nRet));
		return false;
	}
	if (*pEnd != '\0') {
		ERR_PRINT("Invalid digits (%s)!\n", pEnd);
		return false;
	}
	return true;
}
