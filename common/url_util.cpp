/*
 * url_util.cpp
 *
 *  Created on: Aug 7, 2015
 *      Author: Jacken
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "url_util.h"
#include "debug.h"

bool URLUtil::Encode(const char *sURL, char *sEncodedURL, unsigned int *pEncodedURLLen)
{
	unsigned int uURLLen = strlen(sURL);
	char sEncodedResult[4] = {0};

	if (uURLLen <= 0) {
		sEncodedURL[0] = 0;
		*pEncodedURLLen = 0;
		return false;
	}

	unsigned int uEncodedURLIndex = 0;
	for (unsigned int uURLIndex = 0; uURLIndex < uURLLen; uURLIndex++) {
		if ((sURL[uURLIndex] >= 48) && (sURL[uURLIndex] <= 57)) { //ASCII: 0~9
			sEncodedURL[uEncodedURLIndex] = sURL[uURLIndex];
			uEncodedURLIndex++;
			continue;
		}

		if ((sURL[uURLIndex] >= 65) && (sURL[uURLIndex] <= 90)) { //ASCII: A~Z
			sEncodedURL[uEncodedURLIndex] = sURL[uURLIndex];
			uEncodedURLIndex++;
			continue;
		}

		if ((sURL[uURLIndex] >= 97) && (sURL[uURLIndex] <= 122)) { //ASCII: a~z
			sEncodedURL[uEncodedURLIndex] = sURL[uURLIndex];
			uEncodedURLIndex++;
			continue;
		}

		if ((sURL[uURLIndex] == 46) || (sURL[uURLIndex] == 95) || (sURL[uURLIndex] == 45)) { //ASCII: . _ -
			sEncodedURL[uEncodedURLIndex] = sURL[uURLIndex];
			uEncodedURLIndex++;
			continue;
		}

		snprintf(sEncodedResult, sizeof(sEncodedResult), "%%%02X", sURL[uURLIndex]);
		strncat(sEncodedURL, sEncodedResult, sizeof(sEncodedResult));
		uEncodedURLIndex = uEncodedURLIndex + 3;
	}

	DBG_PRINT("sEncodedURL:%s\n", sEncodedURL);
	*pEncodedURLLen = uEncodedURLIndex;
	return true;
}

bool URLUtil::Decode(const char *sEncodedURL, char *sURL, unsigned int *pURLLen)
{
	unsigned int uEncodedURLLen = strlen(sEncodedURL);
	char sEncodedStr[3] = {0};
	long int lDecodedResult = 0;
	int nRet;

	if (uEncodedURLLen <= 0) {
		sURL[0] = 0;
		*pURLLen = 0;
		return false;
	}

	unsigned int uEncodedURLIdx = 0;
	unsigned int uURLIndex = 0;
	for (uURLIndex = 0; uEncodedURLIdx < uEncodedURLLen; uURLIndex++) {

		if (sEncodedURL[uEncodedURLIdx] == '%') {
			sEncodedStr[0] = sEncodedURL[uEncodedURLIdx + 1];
			sEncodedStr[1] = sEncodedURL[uEncodedURLIdx + 2];
			lDecodedResult = strtol(sEncodedStr, NULL, 16);
			if( (lDecodedResult == LONG_MAX) || (lDecodedResult == LONG_MIN) ) {
				nRet = errno;
				ERR_PRINT("strtol() error! Reason:%s\n", strerror(nRet));
				return false;
			}
			sURL[uURLIndex] = (char)lDecodedResult;
			uEncodedURLIdx = uEncodedURLIdx + 3;
			continue;
		}

		sURL[uURLIndex] = sEncodedURL[uEncodedURLIdx];
		uEncodedURLIdx++;
	}
	DBG_PRINT("sURL:%s\n", sURL);
	*pURLLen = uURLIndex;
	return true;
}
