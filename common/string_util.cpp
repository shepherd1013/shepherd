/*
 * string_util.cpp
 *
 *  Created on: Sep 18, 2015
 *      Author: Jacken
 */
#include <stdarg.h>
#include <stdio.h>
#include "debug.h"
#include "string_util.h"

bool StringUtil::Snprintf(char *s, size_t n, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int nRet = snprintf(s, n, format, args);
    va_end(args);
	if (nRet < 0) {
		ERR_PRINT("snprintf() error!\n");
		return false;
	}
	return true;
}
