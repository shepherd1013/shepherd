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
		ERR_PRINT("FileUtil::Open(%s) error!\n",sFilename);
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

IniFileParser::IniFileParser()
{
//	DBG_PRINT("Run %s() ...\n", __FUNCTION__);
}

IniFileParser::IniFileParser(const char* sIniFile): File(sIniFile)
{
//	DBG_PRINT("Run %s() ...\n", __FUNCTION__);
	this->LoadFile(sIniFile);
}

IniFileParser::~IniFileParser()
{
//	DBG_PRINT("Run %s() ...\n", __FUNCTION__);
}

bool IniFileParser::LoadFile(const char* sIniFile)
{
	const char* sKey = NULL;
	const char* sVal = NULL;
	char* pSave;
	KeyValMap.clear();
	do {
		if (this->ReadLine(sBuf, sizeof(sBuf)) == false) {
			return false;
		}
		if (this->IsEOF()) {
			break;
		}
		if (strlen(sBuf) == 0) {
			continue;
		}
		sKey = strtok_r(sBuf, " =", &pSave);
		if (sKey == NULL) {
			continue;
		}
		sVal = strtok_r(NULL, " =", &pSave);
		if (sVal == NULL) {
			continue;
		}
		KeyValMap.insert(pair<string, string>(sKey, sVal));
	} while (true);

	return true;
}

const char* IniFileParser::GetKeyValue(const char *sKey)
{
	if (sKey == NULL) {
		ERR_PRINT("The key is NULL!\n");
		return NULL;
	}
	map<string, string>::iterator it = KeyValMap.find(sKey);
	if (it == KeyValMap.end()) {
		return NULL;
	}
	return it->second.c_str();
}

bool IniFileParser::SaveFile()
{
#if 1
	bool bRet = FileUtil::Close(m_fp);
	if (bRet == false) {
		ERR_PRINT("FileUtil::Close() error!\n");
		return false;
	}

	m_fp = FileUtil::Open(m_sFilePath, "w+");
	if (m_fp == NULL) {
		ERR_PRINT("FileUtil::Open() error!\n");
		return false;
	}

#else
	int nRet = fseek(m_fp, 0, SEEK_SET);
	if (nRet < 0) {
		nRet = errno;
		ERR_PRINT("fseek() error: %s!\n", strerror(nRet));
		return false;
	}
	nRet = fflush(m_fp);
	if (nRet == EOF) {
		nRet = errno;
		ERR_PRINT("fflush() error: %s!\n", strerror(nRet));
		return false;
	}
	__fpurge(m_fp);
#endif
	for (map<string, string>::iterator it = KeyValMap.begin(); it != KeyValMap.end(); it++) {
		fprintf(m_fp, "%s = %s\n", it->first.c_str(), it->second.c_str());
	}
	return true;
}

bool IniFileParser::SetKeyValue(const char* sKey, const char* sVal)
{
	if (sKey == NULL || sVal == NULL) {
		ERR_PRINT("The key or value is NULL!\n");
		return false;
	}
	map<string, string>::iterator it = KeyValMap.find(sKey);
	if (it != KeyValMap.end()) { //The find matched
		KeyValMap[sKey] = sVal;
	} else {
		KeyValMap.insert(pair<string, string>(sKey, sVal));
	}
	return true;
}

void IniFileParser::ShowKeyValue()
{
	if (KeyValMap.empty()) {
		return;
	}
	DBG_PRINT("Show key and value table:\n");
	for (map<string, string>::iterator it = KeyValMap.begin(); it != KeyValMap.end(); it++) {
		printf("Key:%s, Value:%s\n", it->first.c_str(), it->second.c_str());
	}
}
