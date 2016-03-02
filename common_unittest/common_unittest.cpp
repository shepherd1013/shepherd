/*
 * common_unittest.cpp
 *
 *  Created on: Sep 20, 2015
 *      Author: jacken
 */
#include "common_unittest.h"
#include "debug.h"
#include "file_unittest.h"

void Unittest::Run()
{
	DBG_PRINT("Run %s() ...\n", __FUNCTION__);
	FileUnittest Filetest;
	Filetest.Run();
}


