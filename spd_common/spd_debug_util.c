/*
 * spd_debug_util.c
 *
 *  Created on: Sep 14, 2015
 *      Author: Jacken
 */

#include "spd_debug_util.h"

enum DebugLevel eDebugLevel = DBG_LEVEL_DEBUG;

void SetDebugLevel(enum DebugLevel eLevel)
{
	eDebugLevel = eLevel;
}

enum DebugLevel GetDebugLevel()
{
	return eDebugLevel;
}


