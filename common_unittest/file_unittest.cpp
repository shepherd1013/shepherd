/*
 * file_unittest.cpp
 *
 *  Created on: Nov 20, 2015
 *      Author: jacken
 */
#include "file_unittest.h"
#include "file_parser.h"
#include "file_util.h"
#include "debug.h"

void FileUnittest::Run()
{
	DBG_PRINT("Run %s() ...\n", __FUNCTION__);
	bool bRet = false;
	bRet = this->TestFileParser();
	if (bRet == false) {
		ERR_PRINT("this->TestFileParser() error!\n");
		return;
	}

	bRet = this->TestIniFileParser();
	if (bRet == false) {
		ERR_PRINT("this->TestIniFileParser() error!\n");
		return;
	}

	if (this->TestFileWrite() == false) {
		ERR_PRINT("this->TestFileWrite() error!\n");
		return;
	}

	if (this->TestFileRead() == false) {
		ERR_PRINT("this->TestFileRead() error!\n");
		return;
	}
}

bool FileUnittest::TestFileParser()
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

bool FileUnittest::TestIniFileParser()
{
	DBG_PRINT("Run %s() ...\n", __FUNCTION__);
	IniFileParser parser("test.ini");
	char sValue[128] = {0};
	const char *sKey = "IPC.Local.Path";
	if(parser.GetKeyValue(sKey, sValue, sizeof(sValue)) == false) {
		ERR_PRINT("parser.GetKeyValue(%s) error!\n", sKey);
		return false;
	}
	DBG_PRINT("sValue: %s\n", sValue);
	return true;
}

bool FileUnittest::TestFileWrite()
{
	DBG_PRINT("Run %s() ...\n", __FUNCTION__);
	const char *sFileName = "../test.txt";
	File file(sFileName);
	char sData[64] = {"0123456789\0"};

	if(file.Write(sData, 1, sizeof(sData)) == false) {
		ERR_PRINT("file.Write() error!\n");
		return false;
	}
	return true;
}

bool FileUnittest::TestFileRead()
{
	DBG_PRINT("Run %s() ...\n", __FUNCTION__);
	const char *sFileName = "../test.txt";
	File file(sFileName);
	char sData[64] = {0};

	if(file.Read(sData, 1, sizeof(sData)) == false) {
		ERR_PRINT("file.Read error!\n");
		return false;
	}
	DBG_PRINT("sData: %s\n", sData);
	return true;
}

