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

bool File::Open(FILE *fp, const char *sFilename)
{
	int nRet;

	fp = fopen(sFilename, "r");
	if(fp == NULL) {
		nRet = errno;
		ERR_PRINT("%s\n", strerror(nRet));
		return false;
	}
	return true;
}

bool File::Close(FILE *fp)
{
	int nRet = fclose(fp);

	if (nRet != 0) {
		nRet = errno;
		ERR_PRINT("%s\n", strerror(nRet));
		return false;
	}
	return true;
}


