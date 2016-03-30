/*
 * spd_uboot_env_parser.cpp
 *
 *  Created on: Mar 28, 2016
 *      Author: Jacken
 */
#include "debug.h"
#include "spd_uboot_env_parser.h"
#include "string_util.h"
#include <string.h>

UbootEnvParser::UbootEnvParser(const char* sFile): File(sFile)
{
	m_uCrc = 0;
	m_uCrcSize = sizeof(m_uCrc);
	if (this->LoadFile(sFile) == false) {
		ERR_PRINT("Load file (%s) failed!\n", sFile);
		return;
	}
}

bool UbootEnvParser::LoadFile(const char* sFile)
{
	unsigned int uReadSize = 0;
	if (this->Read(&m_uCrc, 1, m_uCrcSize, &uReadSize) == false) {
		ERR_PRINT("Read checksum failed!\n");
		return false;
	}

	long unsigned int ulPartiSize = 0;
	if (FileUtil::GetPartitionSize(sFile, &ulPartiSize) == false) {
		ERR_PRINT("Get partition (%s) size failed!\n", sFile);
		return false;
	}

	long unsigned int ulEnvDataSize = ulPartiSize - m_uCrcSize;
	char sBuf[BUF_SIZE];
	const char* sKey = NULL;
	const char* sVal = NULL;
	char cDelim = '\0';
	char *pToken = NULL;
	long unsigned int ulReadTotal = 0;
	do {
		bzero(sBuf, BUF_SIZE);
		if (this->ReadLine(sBuf, BUF_SIZE, &uReadSize, cDelim) == false) {
			ERR_PRINT("Read line with failed!\n");
			return false;
		}

		ulReadTotal += uReadSize;
		if (uReadSize == 0) {
			if (ulReadTotal >= ulEnvDataSize) {
				break;
			} else {
				continue;
			}
		}

		m_EnvData.insert(m_EnvData.end(), &sBuf[0], &sBuf[uReadSize]);

		sKey = sBuf;
		pToken = strstr(sBuf, "=");
		if(pToken == NULL) {
			continue;
		}
		sVal = pToken + 1;
		*pToken = '\0';

		m_KeyValMap.insert(pair<string, string>(sKey, sVal));

	} while (true);
	return true;
}

void UbootEnvParser::ShowKeyValue()
{
	if (m_KeyValMap.empty()) {
		return;
	}
	for (map<string, string>::iterator it = m_KeyValMap.begin(); it != m_KeyValMap.end(); it++) {
		printf("%s=%s\n", it->first.c_str(), it->second.c_str());
	}
}

vector<char>* UbootEnvParser::GetEnvData()
{
	return &m_EnvData;
}

unsigned int UbootEnvParser::GetCRC()
{
	return m_uCrc;
}
