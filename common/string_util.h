/*
 * string_util.h
 *
 *  Created on: Sep 18, 2015
 *      Author: Jacken
 */

#ifndef STRING_UTIL_H_
#define STRING_UTIL_H_

class StringUtil
{
public:
	static bool Snprintf(char *s, size_t n, const char *format, ...);//TODO: To be verified
	static bool StrToULInt(const char* sInput, unsigned long int *pOutput, int nBase = 0);
	static bool StrToLInt(const char* sInput, long int *pOutput, int nBase = 0);
};


#endif /* STRING_UTIL_H_ */
