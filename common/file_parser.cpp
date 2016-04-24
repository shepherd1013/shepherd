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
#include "string_util.h"

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
	unsigned int uReadSize = 0;
	m_KeyValMap.clear();
	do {
		uReadSize = 0;
		if (this->ReadLine(m_sBuf, BUF_MAX, &uReadSize) == false) {
			return false;
		}
		if (this->IsEOF()) {
			if (uReadSize == 0) {
				break;
			}
		}
		if (uReadSize == 0 || m_sBuf[0] == '#') {
			continue;
		}
		sKey = strtok_r(m_sBuf, " =\t", &pSave);
		if (sKey == NULL) {
			continue;
		}
		sVal = strtok_r(NULL, " =\t", &pSave);
		if (sVal == NULL) {
			continue;
		}
		m_KeyValMap.insert(pair<string, string>(sKey, sVal));
	} while (true);

	return true;
}

const char* IniFileParser::GetKeyValue(const char *sKey)
{
	if (sKey == NULL) {
		ERR_PRINT("The key is NULL!\n");
		return NULL;
	}
	map<string, string>::iterator it = m_KeyValMap.find(sKey);
	if (it == m_KeyValMap.end()) {
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
	for (map<string, string>::iterator it = m_KeyValMap.begin(); it != m_KeyValMap.end(); it++) {
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
	map<string, string>::iterator it = m_KeyValMap.find(sKey);
	if (it != m_KeyValMap.end()) { //The find matched
		m_KeyValMap[sKey] = sVal;
	} else {
		m_KeyValMap.insert(pair<string, string>(sKey, sVal));
	}
	return true;
}

bool IniFileParser::SetKeyValue(const char* sKey, bool bVal)
{
	char sVal[8] = {0};
	if (StringUtil::Snprintf(sVal, sizeof(sVal), "%d", bVal) == false) {
		ERR_PRINT("Compose key value failed!\n");
		return false;
	}
	if (this->SetKeyValue(sKey, sVal) == false) {
		return false;
	}
	return true;
}

bool IniFileParser::SetKeyValue(const char* sKey, int nVal)
{
	char sVal[16] = {0};
	if (StringUtil::Snprintf(sVal, sizeof(sVal), "%d", nVal) == false) {
		ERR_PRINT("Compose key value failed!\n");
		return false;
	}
	if (this->SetKeyValue(sKey, sVal) == false) {
		return false;
	}
	return true;
}



void IniFileParser::ShowKeyValue()
{
	if (m_KeyValMap.empty()) {
		return;
	}
	DBG_PRINT("Show key and value table:\n");
	for (map<string, string>::iterator it = m_KeyValMap.begin(); it != m_KeyValMap.end(); it++) {
		printf("Key:%s, Value:%s\n", it->first.c_str(), it->second.c_str());
	}
}

//bool IniFileParser::GetKeyIntValue(const char* sKey, int *pVal)
//{
//	const char *sVal = this->GetKeyValue(sKey);
//	if (sVal == NULL) {
//		ERR_PRINT("Get key (%s) failed!\n", sKey);
//		return false;
//	}
//
//	long int lOutput;
//	if (StringUtil::StrToLInt(sVal, &lOutput) == false) {
//		ERR_PRINT("Convert string (%s) to long integer failed!\n", sVal);
//		return false;
//	}
//	*pVal = (int)lOutput;
//	return true;
//}
//
//bool IniFileParser::GetKeyUIntValue(const char* sKey, unsigned int *pVal)
//{
//	const char *sVal = this->GetKeyValue(sKey);
//	if (sVal == NULL) {
//		ERR_PRINT("Get key (%s) failed!\n", sKey);
//		return false;
//	}
//
//	unsigned long int lOutput;
//	if (StringUtil::StrToULInt(sVal, &lOutput) == false) {
//		ERR_PRINT("Convert string (%s) to unsigned long integer failed!\n", sVal);
//		return false;
//	}
//	*pVal = (unsigned int)lOutput;
//	return true;
//}
//
//bool IniFileParser::GetKeyBoolValue(const char* sKey, bool *pVal)
//{
//	const char *sVal = this->GetKeyValue(sKey);
//	if (sVal == NULL) {
//		ERR_PRINT("Get key (%s) failed!\n", sKey);
//		return false;
//	}
//
//	unsigned long int lOutput;
//	if (StringUtil::StrToULInt(sVal, &lOutput) == false) {
//		ERR_PRINT("Convert string (%s) to unsigned long integer failed!\n", sVal);
//		return false;
//	}
//	*pVal = (bool)lOutput;
//	return true;
//}

bool IniFileParser::GetKeyValue(const char* sKey, int *pVal)
{
	const char *sVal = this->GetKeyValue(sKey);
	if (sVal == NULL) {
		ERR_PRINT("Get key (%s) failed!\n", sKey);
		return false;
	}

	long int lOutput;
	if (StringUtil::StrToLInt(sVal, &lOutput) == false) {
		ERR_PRINT("Convert string (%s) to long integer failed!\n", sVal);
		return false;
	}
	*pVal = (int)lOutput;
	return true;
}

bool IniFileParser::GetKeyValue(const char* sKey, unsigned int *pVal)
{
	const char *sVal = this->GetKeyValue(sKey);
	if (sVal == NULL) {
		ERR_PRINT("Get key (%s) failed!\n", sKey);
		return false;
	}

	unsigned long int lOutput;
	if (StringUtil::StrToULInt(sVal, &lOutput) == false) {
		ERR_PRINT("Convert string (%s) to unsigned long integer failed!\n", sVal);
		return false;
	}
	*pVal = (unsigned int)lOutput;
	return true;
}

bool IniFileParser::GetKeyValue(const char* sKey, bool *pVal)
{
	const char *sVal = this->GetKeyValue(sKey);
	if (sVal == NULL) {
		ERR_PRINT("Get key (%s) failed!\n", sKey);
		return false;
	}

	unsigned long int lOutput;
	if (StringUtil::StrToULInt(sVal, &lOutput) == false) {
		ERR_PRINT("Convert string (%s) to unsigned long integer failed!\n", sVal);
		return false;
	}
	*pVal = (bool)lOutput;
	return true;
}
