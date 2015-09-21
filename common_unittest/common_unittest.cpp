/*
 * common_unittest.cpp
 *
 *  Created on: Sep 20, 2015
 *      Author: jacken
 */
#include "common_unittest.h"
#include "debug.h"
#include "file_parser.h"

void Unittest::Run()
{
	DBG_PRINT("Run %s() ...\n", __FUNCTION__);
	bool bRet = false;
	bRet = this->TestFileParser();
	if (bRet == false) {
		ERR_PRINT("this->TestFileParser() error!\n");
		return;
	}
}

bool Unittest::TestFileParser()
{
	DBG_PRINT("Run %s() ...\n", __FUNCTION__);
	FileParser parser("test.ini");
	char sLine[512] = {0};
	bool bRet = false;

	do {
		bRet = parser.ReadLineWithToken(sLine, sizeof(sLine), "\n");
		if(bRet){
			DBG_PRINT("sLine: %s\n", sLine);
		}
	} while(bRet);

	return true;
}
