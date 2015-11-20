/*
 * file_parser.cpp
 *
 *  Created on: Sep 20, 2015
 *      Author: jacken
 */
#include <string.h>
#include <assert.h>
#include "file_parser.h"
#include "debug.h"
#include "file_util.h"

FileParser::FileParser(const char *sFilename)
{
	DBG_PRINT("Run %s() ...\n", __FUNCTION__);
	fFd = FileUtil::Open(sFilename, "r");
	if (fFd == NULL) {
		ERR_PRINT("File::Open(%s) error!\n",sFilename);
		assert(0);
//		return;
	}
}

FileParser::~FileParser()
{
	DBG_PRINT("Run %s() ...\n", __FUNCTION__);
	bool bRet = FileUtil::Close(fFd);
	if (bRet == false) {
		ERR_PRINT("File::Close() error!\n");
	}
}

bool FileParser::ReadLine(char *sLine, unsigned int uLineSize)
{
	if (fgets(sLine, uLineSize, fFd) == NULL) {
//		ERR_PRINT("fgets() error!");
		return false;
	}
	sLine[strlen(sLine)-1] = 0;
	return true;
}

bool FileParser::ReadLineWithToken(char *sLine, unsigned int uLineSize, const char *sToken)
{
	int nChar = 0;
	unsigned int uTokenSize = strlen(sToken);
	unsigned int uIndex = 0;
	bool bIsTokenFound = false;
	unsigned int uLineIndex = 0;

	do {
		nChar = fgetc(fFd);
		if (nChar == EOF) {
			if (uLineIndex == 0) {
				return false;
			}
			sLine[uLineIndex] = 0;
			break;;
		}

		for (uIndex = 0; uIndex < uTokenSize; uIndex++) {
			if (nChar == sToken[uIndex]) {
				bIsTokenFound = true;
				sLine[uLineIndex] = 0;
				break;
			}
		}

		if (bIsTokenFound == false) {
			if (uLineIndex == uLineSize) {
				sLine[uLineIndex] = 0;
				ERR_PRINT("The sLine space is not enough!\n");
				return false;
			}
			sLine[uLineIndex] = (char)nChar;
			uLineIndex++;
		}
	} while(bIsTokenFound == false);
	return true;
}

IniFileParser::~IniFileParser()
{
	DBG_PRINT("Run %s() ...\n", __FUNCTION__);
}

bool IniFileParser::GetKeyValue(const char *sKey, char *sValue, unsigned int uValueSize)
{
	DBG_PRINT("Run %s() ...\n", __FUNCTION__);
	bool bRet = false;
	char sLine[256] = {0};
	char *pChar = NULL;

	if (sKey == NULL) {
		ERR_PRINT("sKey is NULL!\n");
		return false;
	}

	if (sValue == NULL) {
		ERR_PRINT("sValue is NULL!\n");
		return false;
	}

	while (true) {
		bRet = this->ReadLine(sLine, sizeof(sLine));
		if (bRet == false) {
			ERR_PRINT("this->ReadLine() error!\n");
			return false;
		}

		pChar = strtok(sLine, " =");
		if(pChar == NULL) {
			continue;
		}
		DBG_PRINT("pChar: %s\n", pChar);
		if (strncmp(pChar, sKey, strlen(pChar)) != 0) {
			continue;
		}
		pChar = strtok(NULL, " =");
		if(pChar == NULL) {
			continue;
		}
		DBG_PRINT("pChar: %s\n", pChar);
		strncpy(sValue, pChar, uValueSize);
		break;
	}
	return true;
}
