/*
 * main.cpp
 *
 *  Created on: Sep 20, 2015
 *      Author: jacken
 */
#include "common_unittest.h"
#include "debug.h"
#include "spd_sig_util.h"

int main(int argc, char *argv[])
{
	if(SigUtil::Block(SIGINT) == false) {
		return -1;
	}
	if(SigUtil::Block(SIGTERM) == false) {
		return -1;
	}
	SetDebugLevel(DBG_LEVEL_DEBUG);
	Unittest test;
	test.Run();
	return 0;
}


