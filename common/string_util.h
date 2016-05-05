/*
 * string_util.h
 *
 *  Created on: Sep 18, 2015
 *      Author: Jacken
 */

#ifndef STRING_UTIL_H_
#define STRING_UTIL_H_
#include <sys/types.h>

class StringUtil
{
public:
	static bool Snprintf(char* sBuf, size_t uBufSize, const char* format, ...);
	static bool StrToULInt(const char* sInput, unsigned long int *pOutput, int nBase = 0);
	static bool StrToLInt(const char* sInput, long int *pOutput, int nBase = 0);
	static bool Replace(const char* OldStr, char* sNewStr, unsigned int uBufSize, const char* sOldKey, const char* sNewKey);
	static bool StrToDouble(const char* sIn, double* dOut);
};


#endif /* STRING_UTIL_H_ */
