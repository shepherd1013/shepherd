/*
 * spd_file_util.h
 *
 *  Created on: Sep 10, 2015
 *      Author: Jacken
 */

#ifndef SPD_FILE_UTIL_H_
#define SPD_FILE_UTIL_H_

#include <stdio.h>

#ifndef BOOL
#define BOOL int
#define TRUE 1
#define FALSE 0
#endif

FILE* FileUtil_Open(const char *sFilename, const char *sMode);
BOOL FileUtil_Close(FILE *pFp);
BOOL FileUtil_Write(const void *pData, size_t uDataSize, size_t uNumElement, FILE *pFile);

#endif /* SPD_FILE_UTIL_H_ */
