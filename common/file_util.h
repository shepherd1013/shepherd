/*
 * file_util.h
 *
 *  Created on: Jun 23, 2015
 *      Author: Jacken
 */

#ifndef FILE_UTIL_H_
#define FILE_UTIL_H_
#include <stdio.h>

class File
{
public:
	File(const char *sFilePath);
	~File();
	bool Read(void *pData, size_t uDataSize, size_t uNumElement);
	bool Write(const void *pData, size_t uDataSize, size_t uNumElement);

protected:
	FILE *fpFile;
};

class FileUtil
{
public:
	static FILE*	Open(const char *filename, const char *sMode);
	static bool	Close(FILE *fp);
	static bool	Write(const void *pData, size_t uDataSize, size_t uNumElement, FILE *pFile);
	static bool	Read(void *pData, size_t uDataSize, size_t uNumElement, FILE *pFile);
};


#endif /* FILE_UTIL_H_ */
