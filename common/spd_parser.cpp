/*
 * spd_parser.cpp
 *
 *  Created on: Feb 2, 2016
 *      Author: Jacken
 */
#include "debug.h"
#include "spd_parser.h"
#include <string.h>

Parser::Parser(const char* sData, const char* sToken)
{
	if (sData == NULL) {
		return;
	}

	strncpy(sBuf, sData, sizeof(sBuf));
	char *pSave;
	char *pSubstr = strtok_r(sBuf, sToken, &pSave);
	while (pSubstr != NULL) {
		Substring.push_back(pSubstr);
		pSubstr = strtok_r(NULL, sToken, &pSave);
	}
}

Parser::~Parser()
{

}

bool Parser::ShowSubstring()
{
	for (vector<string>::iterator it = Substring.begin(); it != Substring.end(); it++) {
		DBG_PRINT("%s\n", it->c_str());
	}
	return true;
}
