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

FILE* FileUtil::Open(const char *sFilename, const char *sMode)
{
	int nRet;
	FILE *pFile;

	pFile = fopen(sFilename, sMode);
	if(pFile == NULL) {
		nRet = errno;
		ERR_PRINT("fopen(%s): %s\n", sFilename, strerror(nRet));
		return NULL;
	}
	return pFile;
}

bool FileUtil::Close(FILE *fp)
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

bool FileUtil::Write(const void *pData, size_t uDataSize, size_t uNumElement, FILE *pFile)
{
	size_t nRet = fwrite(pData, uDataSize, uNumElement, pFile);
	DBG_PRINT("nRet: %u\n", nRet);
	if ( (nRet == 0) || (nRet != (uDataSize * uNumElement)) ) {
		ERR_PRINT("fwrite() error!\n");
		return false;
	}
	return true;
}

bool FileUtil::Read(void *pData, size_t uDataSize, size_t uNumElement, FILE *pFile)
{
	size_t nRet = fread(pData, uDataSize, uNumElement, pFile);
	DBG_PRINT("nRet: %u\n", nRet);
	if ( (nRet == 0) || (nRet != (uDataSize * uNumElement)) ) {
		ERR_PRINT("fwrite() error!\n");
		return false;
	}
	return true;
}

File::File(const char *sFilePath)
{
	fpFile = FileUtil::Open(sFilePath, "r+");
	if (fpFile == NULL) {
		ERR_PRINT("FileUtil::Open() error!\n");
		return;
	}
}

File::~File()
{
	if(FileUtil::Close(fpFile) == false) {
		ERR_PRINT("FileUtil::Close() error!\n");
		return;
	}
}

bool File::Read(void *pData, size_t uDataSize, size_t uNumElement)
{
	if (FileUtil::Read(pData, uDataSize, uNumElement, fpFile) == false ) {
		ERR_PRINT("FileUtil::Read() error!\n");
		return false;
	}
	return true;
}

bool File::Write(const void *pData, size_t uDataSize, size_t uNumElement)
{
	if (FileUtil::Write(pData, uDataSize, uNumElement, fpFile) == false ) {
		ERR_PRINT("FileUtil::Write() error!\n");
		return false;
	}
	return true;
}
