/*
 * file_util.cpp
 *
 *  Created on: Jun 23, 2015
 *      Author: Jacken
 */
#include "file_util.h"
#include <errno.h>
#include <string.h>
#include "debug.h"

FILE* File::Open(const char *sFilename, const char *sMode)
{
	int nRet;
	FILE *pFile;

	pFile = fopen(sFilename, sMode);
	if(pFile == NULL) {
		nRet = errno;
		ERR_PRINT("%s\n", strerror(nRet));
		return NULL;
	}
	return pFile;
}

bool File::Close(FILE *fp)
{
	if (fp != NULL) {
		int nRet = fclose(fp);
		if (nRet != 0) {
			nRet = errno;
			ERR_PRINT("%s\n", strerror(nRet));
			return false;
		}
	}
	return true;
}

bool File::Write(const void *pData, size_t uDataSize, size_t uNumElement, FILE *pFile)
{
	size_t nRet = fwrite(pData, uDataSize, uNumElement, pFile);
	DBG_PRINT("nRet: %u\n", nRet);
	if ( (nRet == 0) || (nRet != (uDataSize * uNumElement)) ) {
		ERR_PRINT("fwrite() error!\n");
		return false;
	}
	return true;
}
