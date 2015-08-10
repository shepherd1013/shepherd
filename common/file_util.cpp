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

bool File::Open(FILE *fp, const char *sFilename, const char *sMode)
{
	int nRet;
	FILE *pFile;

	pFile = fopen(sFilename, sMode);
	if(pFile == NULL) {
		nRet = errno;
		ERR_PRINT("%s\n", strerror(nRet));
		return false;
	}
	fp = pFile;
	return true;
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


