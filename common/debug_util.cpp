/*
 * debug_util.cpp
 *
 *  Created on: Aug 12, 2015
 *      Author: jacken
 */

#include "debug.h"

enum DebugLevel eDebugLevel = DBG_LEVEL_NONE;

void SetDebugLevel(enum DebugLevel eLevel)
{
	eDebugLevel = eLevel;
}

enum DebugLevel GetDebugLevel()
{
	return eDebugLevel;
}
