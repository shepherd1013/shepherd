/*
 * spd_file_util.c
 *
 *  Created on: Sep 10, 2015
 *      Author: Jacken
 */
#include <errno.h>
#include <string.h>
#include "spd_file_util.h"
#include "spd_debug_util.h"

FILE* FileUtil_Open(const char *sFilename, const char *sMode)
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

BOOL FileUtil_Close(FILE *pFile)
{
	if (pFile != NULL) {
		int nRet = fclose(pFile);
		if (nRet != 0) {
			nRet = errno;
			ERR_PRINT("%s\n", strerror(nRet));
			return FALSE;
		}
	}
	return TRUE;
}

BOOL FileUtil_Write(const void *pData, size_t uDataSize, size_t uNumElement, FILE *pFile)
{
	size_t nRet = fwrite(pData, uDataSize, uNumElement, pFile);
//	DBG_PRINT("nRet: %u\n", nRet);
	if ( (nRet == 0) || (nRet != (uDataSize * uNumElement)) ) {
		ERR_PRINT("fwrite() error!\n");
		return FALSE;
	}
	return TRUE;
}
