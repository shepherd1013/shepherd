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

#define DEBUG_OUTPUT stderr

enum DebugLevel {
	DBG_LEVEL_NONE = 0,
	DBG_LEVEL_ERROR,
	DBG_LEVEL_WARN,
	DBG_LEVEL_INFO,
	DBG_LEVEL_DEBUG,
};

extern enum DebugLevel eDebugLevel;

#define DBG_PRINT(fmt, args ...) { \
	if(eDebugLevel >= DBG_LEVEL_DEBUG) {\
		fprintf(DEBUG_OUTPUT, "[DBG] [%s:%s:%d] ", __FILE__, __FUNCTION__, __LINE__);\
		fprintf(DEBUG_OUTPUT, fmt, ## args);\
	}\
}

#define INF_PRINT(fmt, args ...) { \
	if(eDebugLevel >= DBG_LEVEL_INFO) {\
		fprintf(DEBUG_OUTPUT, "[INF] [%s:%s:%d] ", __FILE__, __FUNCTION__, __LINE__);\
		fprintf(DEBUG_OUTPUT, fmt, ## args);\
	}\
}

#define WARN_PRINT(fmt, args ...) { \
	if(eDebugLevel >= DBG_LEVEL_WARN) {\
		fprintf(DEBUG_OUTPUT, "[WARN] [%s:%s:%d] ", __FILE__, __FUNCTION__, __LINE__);\
		fprintf(DEBUG_OUTPUT, fmt, ## args);\
	}\
}

#define ERR_PRINT(fmt, args ...) { \
	if(eDebugLevel >= DBG_LEVEL_ERROR) {\
		fprintf(DEBUG_OUTPUT, "[ERR] [%s:%s:%d] ", __FILE__, __FUNCTION__, __LINE__);\
		fprintf(DEBUG_OUTPUT, fmt, ## args);\
	}\
}

void SetDebugLevel(enum DebugLevel eLevel);
enum DebugLevel GetDebugLevel();

#endif /* DEBUG_H_ */
