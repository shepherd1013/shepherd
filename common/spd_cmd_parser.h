/*
 * spd_cmd_parser.h
 *
 *  Created on: Feb 17, 2016
 *      Author: Jacken
 */

#ifndef SPD_CMD_PARSER_H_
#define SPD_CMD_PARSER_H_

#include <string>
#include <map>

using namespace std;

class CommandParser
{
public:
	CommandParser();
	CommandParser(const char* sData);
	const char* GetKeyValue(const char *sKey);
	bool GetKeyValue(const char* sKey, int *pVal);
	bool GetKeyValue(const char* sKey, unsigned int *pVal);
	bool GetKeyValue(const char* sKey, bool *pVal);
	bool LoadData(const char* sData);

private:
	char sBuf[4096];
	map<string, string> CmdMap;
};


#endif /* SPD_CMD_PARSER_H_ */
