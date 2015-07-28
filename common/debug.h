/*
 * debug.h
 *
 *  Created on: May 28, 2015
 *      Author: Jacken
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define IS_DEBUG_ENABLED 0

#if IS_DEBUG_ENABLED
#define DEBUG_OUTPUT stderr

#define DBG_PRINT(fmt, args ...) {\
	fprintf(DEBUG_OUTPUT, "[DBG] [%s:%s:%d] ", __FILE__, __FUNCTION__, __LINE__);\
	fprintf(DEBUG_OUTPUT, fmt, ## args);}
#else
#define DBG_PRINT(message, arsg ...)
#endif

#define INF_PRINT(fmt, args ...) {\
	fprintf(stderr, "[INF] [%s:%s:%d] ", __FILE__, __FUNCTION__, __LINE__);\
	fprintf(stderr, fmt, ## args);}

#define ERR_PRINT(fmt, args ...) {\
	fprintf(stderr, "[ERR] [%s:%s:%d] ", __FILE__, __FUNCTION__, __LINE__);\
	fprintf(stderr, fmt, ## args);}

#endif /* DEBUG_H_ */
