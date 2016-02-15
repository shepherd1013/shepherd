/*
 * spd_parser.h
 *
 *  Created on: Feb 2, 2016
 *      Author: Jacken
 */

#ifndef SPD_PARSER_H_
#define SPD_PARSER_H_

#include <string>
#include <vector>

using namespace std;

class Parser
{
public:
	Parser(const char* sData, const char* sToken);
	virtual ~Parser();
	bool ShowSubstring();

private:
	char sBuf[1024];
	vector<string> Substring;
};

#endif /* SPD_PARSER_H_ */
