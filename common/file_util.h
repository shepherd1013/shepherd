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
	static FILE*	Open(const char *filename, const char *sMode);
	static bool	Close(FILE *fp);
	static bool	Write(const void *ptr, size_t size, size_t nmemb, FILE *stream);
};


#endif /* FILE_UTIL_H_ */
