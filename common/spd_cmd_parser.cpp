/*
 * spd_cmd_parser.cpp
 *
 *  Created on: Feb 17, 2016
 *      Author: Jacken
 */
#include <string.h>
#include "debug.h"
#include "spd_cmd_parser.h"

CommandParser::CommandParser()
{
}

CommandParser::CommandParser(const char* sData)
{
	this->LoadData(sData);
}

bool CommandParser::LoadData(const char* sData)
{
	if (sData == NULL) {
		ERR_PRINT("The data is NULL!\n");
		return false;
	}
	CmdMap.clear();
	strncpy(sBuf, sData, sizeof(sBuf));
	char *pSave1;
	char *pSave2;
	const char* sKey;
	const char* sVal;
	char *pStr1 = strtok_r(sBuf, "&", &pSave1);
	char *pStr2;
	while (pStr1 != NULL) {
		pStr2 = strtok_r(pStr1, "=", &pSave2);
		sKey = pStr2;
		if (sKey != NULL) {
			pStr2 = strtok_r(NULL, "=", &pSave2);
			sVal = pStr2;
			if (sVal != NULL) {
				CmdMap.insert(pair<string, string>(sKey, sVal));
			}
		}
		pStr1 = strtok_r(NULL, "&", &pSave1);
	}
	return true;
}

const char* CommandParser::GetKeyValue(const char *sKey)
{
	if (sKey == NULL) {
		ERR_PRINT("The key is NULL!\n");
		return NULL;
	}
	map<string, string>::iterator it = CmdMap.find(sKey);
	if (it == CmdMap.end()) {
		return NULL;
	}
	return it->second.c_str();
}
