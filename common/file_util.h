/*
 * file_util.h
 *
 *  Created on: Jun 23, 2015
 *      Author: Jacken
 */

#ifndef FILE_UTIL_H_
#define FILE_UTIL_H_
#include <stdio.h>
#define FILE_PATH_MAX 256

class File
{
public:
	File();
	File(const char *sFilePath);
	~File();
	bool Load(const char *sFilePath);
	bool Read(void *pData, size_t uDataSize, size_t uNumElement, unsigned int *uReadSize);
	bool Write(const void *pData, size_t uDataSize, size_t uNumElement);
	bool AppendFile(const char* sFile);
	bool IsFileExisting();
	bool ReadLine(char *sLine, unsigned int uLineSize, unsigned int *pReadSize);
	bool IsEOF();
	FILE* GetFD();
	bool Close();

protected:
	char	m_sFilePath[FILE_PATH_MAX];
	FILE*	m_fp;
};

class FileUtil
{
public:
	static FILE*Open(const char *filename, const char *sMode);
	static bool Close(FILE *fp);
	static bool Write(const void *pData, size_t uDataSize, size_t uNumElement, FILE *pFile);
	static bool Read(void *pData, size_t uDataSize, size_t uNumElement, FILE *pFile, unsigned int *uReadSize);
	static bool GetFileSize(const char *filename, long unsigned int *pSize);
	static bool GetFileAccessTime(const char *filename, struct timespec *pTime);
	static bool GetFileModTime(const char *filename, struct timespec *pTime);
	static bool CopyFile(const char* sSrc, const char* sDest);
	static bool CopyFile(FILE *fpSrc, FILE *fpDst);
};


#endif /* FILE_UTIL_H_ */
