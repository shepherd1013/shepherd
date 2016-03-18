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

bool StringUtil::Replace(char* sData, unsigned int uBufSize, const char* sOldKey, const char* sNewKey)
{
	if (sData == NULL || uBufSize == 0 || sOldKey == NULL || sNewKey == NULL) {
		ERR_PRINT("Invalid argument!\n");
		return false;
	}
	char sTmp[1024] = {0};
	char *pStart = sData;
	char *pFound = NULL;
	unsigned int uStrLen = 0;
	unsigned int uOldKeyLen = strlen(sOldKey);
	unsigned int uNewKeyLen = strlen(sNewKey);
	do {
		pFound = strstr(pStart, sOldKey);
		if (pFound == NULL) {
			if (*pStart != '\0') {
				strcat(sTmp, pStart);
			}
			break;
		}
		uStrLen = (unsigned int)(pFound - pStart);
		if(uStrLen != 0) {
			strncat(sTmp, pStart, uStrLen);
		}
		if (uNewKeyLen != 0) {
			strcat(sTmp, sNewKey);
		}
		pStart = pFound + uOldKeyLen;
	} while (true);

	if (strlen(sTmp) > uBufSize) {
		ERR_PRINT("The buffer size isn't enough!\n");
		return false;
	}
	bzero(sData, uBufSize);
	strncpy(sData, sTmp, uBufSize);
	return true;
}
