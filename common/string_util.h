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
	static bool Snprintf(char *s, size_t n, const char *format, ...);
};


#endif /* STRING_UTIL_H_ */
