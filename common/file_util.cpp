/*
 * file_util.cpp
 *
 *  Created on: Jun 23, 2015
 *      Author: Jacken
 */
#include "file_util.h"
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
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
			ERR_PRINT("fclose() error: %s!\n", strerror(nRet));
			return false;
		}
	}
	return true;
}

bool FileUtil::Write(const void *pData, size_t uDataSize, size_t uNumElement, FILE *pFile)
{
	size_t uRet = fwrite(pData, uDataSize, uNumElement, pFile);
//	DBG_PRINT("Write size: %u\n", uRet);
	if (uRet != (uDataSize * uNumElement)) {
		ERR_PRINT("fwrite() error!\n");
		return false;
	}
	return true;
}

bool FileUtil::Read(void *pData, size_t uDataSize, size_t uNumElement, FILE *pFile, unsigned int *uReadSize)
{
	size_t uRet = fread(pData, uDataSize, uNumElement, pFile);
//	DBG_PRINT("Read size: %u\n", uRet);
	*uReadSize = uRet;
	if (uRet == 0) {
		int nRet = feof(pFile);
//		DBG_PRINT("feof() result: %d\n", nRet);
		if (nRet < 0) {
			nRet = errno;
			ERR_PRINT("fread() error: %s!\n", strerror(nRet));
			return false;
		}
	}
	return true;
}

bool FileUtil::GetFileSize(const char *filename, long unsigned int *pSize)
{
	struct stat stFileStat;
	int nRet = stat(filename, &stFileStat);
	if (nRet < 0) {
		nRet = errno;
		ERR_PRINT("%s\n", strerror(nRet));
		return false;
	}
	*pSize = stFileStat.st_size;
	return true;
}

bool FileUtil::GetFileAccessTime(const char *filename, struct timespec *pTime)
{
	struct stat stFileStat;
	int nRet = stat(filename, &stFileStat);
	if (nRet < 0) {
		nRet = errno;
		ERR_PRINT("%s\n", strerror(nRet));
		return false;
	}
	pTime->tv_sec = stFileStat.st_atim.tv_sec;
	pTime->tv_nsec = stFileStat.st_atim.tv_nsec;
	return true;
}

bool FileUtil::GetFileModTime(const char *filename, struct timespec *pTime)
{
	struct stat stFileStat;
	int nRet = stat(filename, &stFileStat);
	if (nRet < 0) {
		nRet = errno;
		ERR_PRINT("%s\n", strerror(nRet));
		return false;
	}
	pTime->tv_sec = stFileStat.st_mtim.tv_sec;
	pTime->tv_nsec = stFileStat.st_mtim.tv_nsec;
	return true;
}

bool FileUtil::CopyFile(const char* sSrc, const char* sDest)
{
	File Src(sSrc);
	File Dest(sDest);
	const unsigned int uBufSize = 4096;
	char sBuf[uBufSize] = {0};
	unsigned int uReadSize = 0;
	do {
		if (Src.Read(sBuf, 1, uBufSize, &uReadSize) == false) {
			return false;
		}
		if (uReadSize) {
			if (Dest.Write(&sBuf, 1, uReadSize) == false) {
				return false;
			}
		}
	} while(uReadSize);
	return true;
}

bool FileUtil::CopyFile(FILE *fpSrc, FILE *fpDst)
{
	const unsigned int uBufSize = 4096;
	char sBuf[uBufSize] = {0};
	unsigned int uReadSize = 0;
	do {
		if (FileUtil::Read(sBuf, 1, uBufSize, fpSrc, &uReadSize) == false) {
			return false;
		}
		if (uReadSize) {
			if (FileUtil::Write(&sBuf, 1, uReadSize, fpDst) == false) {
				return false;
			}
		}
	} while(uReadSize);
	return true;
}

File::File()
:m_fp(NULL)
{
//	DBG_PRINT("Run %s() ...\n", __FUNCTION__);
}

File::File(const char *sFilePath)
:m_fp(NULL)
{
//	DBG_PRINT("Run %s() ...\n", __FUNCTION__);
	this->Load(sFilePath);
}

File::~File()
{
//	DBG_PRINT("Run %s() ...\n", __FUNCTION__);
	if (FileUtil::Close(m_fp) == false) {
		ERR_PRINT("FileUtil::Close() error!\n");
		return;
	}
}

bool File::Load(const char *sFilePath)
{
	m_sFilePath = sFilePath;
	if (FileUtil::Close(m_fp) == false) {
		ERR_PRINT("FileUtil::Close() error!\n");
		return false;
	}
	return true;
}

bool File::Read(void *pData, size_t uDataSize, size_t uNumElement, unsigned int *uReadSize)
{
	if (m_fp == NULL) {
		m_fp = FileUtil::Open(m_sFilePath, "r+");
		if (m_fp == NULL) {
			ERR_PRINT("FileUtil::Open(%s) error!\n", m_sFilePath);
			return false;
		}
	}
	if (FileUtil::Read(pData, uDataSize, uNumElement, m_fp, uReadSize) == false) {
		ERR_PRINT("FileUtil::Read() error!\n");
		return false;
	}
	return true;
}

bool File::Write(const void *pData, size_t uDataSize, size_t uNumElement)
{
	if (m_fp == NULL) {
		m_fp = FileUtil::Open(m_sFilePath, "w+");
		if (m_fp == NULL) {
			ERR_PRINT("FileUtil::Open() error!\n");
			return false;
		}
	}
	if (FileUtil::Write(pData, uDataSize, uNumElement, m_fp) == false ) {
		ERR_PRINT("FileUtil::Write() error!\n");
		return false;
	}
	return true;
}

bool File::AppendFile(const char* sFile)
{
	File file(sFile);
	const unsigned int uBufSize = 4096;
	char sBuf[uBufSize] = {0};
	unsigned int uReadSize = 0;
	do {
		if (file.Read(sBuf, 1, uBufSize, &uReadSize) == false) {
			return false;
		}
		if (uReadSize) {
			if (this->Write(&sBuf, 1, uReadSize) == false) {
				return false;
			}
		}
	} while(uReadSize);
	return true;
}

bool File::IsFileExisting()
{
	if (fopen(m_sFilePath, "r") == NULL) {
		return false;
	}
	return true;
}

bool File::ReadLine(char *sLine, unsigned int uLineSize)
{
	if (m_fp == NULL) {
		m_fp = FileUtil::Open(m_sFilePath, "r+");
		if (m_fp == NULL) {
			ERR_PRINT("FileUtil::Open() error!\n");
			return false;
		}
	}
	if (fgets(sLine, uLineSize, m_fp) == NULL) {
		int nRet = errno;
		if (this->IsEOF()) {
			return true;
		}
		if (nRet != 0) {
			ERR_PRINT("fgets() error: %s!\n", strerror(nRet));
			return false;
		}
	}
	unsigned int uLineLen = strlen(sLine);
	if (uLineLen > 0) {
		sLine[uLineLen - 1] = 0;
	}

	return true;
}

bool File::IsEOF()
{
//	DBG_PRINT("Run %s() ...\n", __FUNCTION__);
	if (m_fp == NULL) {
		m_fp = FileUtil::Open(m_sFilePath, "r+");
		if (m_fp == NULL) {
			ERR_PRINT("FileUtil::Open() error!\n");
			return false;
		}
	}

	if (m_fp == NULL) {
		DBG_PRINT("m_fpFile is NULL!\n");
	}
	//Hint: 0:Not EOF, 1:EOF, <0:Error
	int nRet = feof(m_fp);
	if (nRet == 0) {
		return false;
	}
	else if (nRet < 0) {
		nRet = errno;
		ERR_PRINT("feof() error: %s!\n", strerror(nRet));
		return false;
	}
	return true;
}

FILE* File::GetFD()
{
	return m_fp;
}

bool File::Close()
{
//	DBG_PRINT("Run %s() ...\n", __FUNCTION__);
	if (FileUtil::Close(m_fp) == false) {
		ERR_PRINT("FileUtil::Close() error!\n");
		return false;
	}
	m_fp = NULL;
	return true;
}
