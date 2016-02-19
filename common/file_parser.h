/*
 * file_parser.h
 *
 *  Created on: Sep 20, 2015
 *      Author: jacken
 */

#ifndef FILE_PARSER_H_
#define FILE_PARSER_H_

#include <stdio.h>
#include <string>
#include <map>
#include "file_util.h"

using namespace std;

class FileParser
{
public:
	FileParser(const char *sFilename);
	virtual ~FileParser();

	bool ReadLine(char *sLine, unsigned int uLineSize);
	bool ReadLineWithToken(char *sLine, unsigned int uLineSize, const char *sToken = "\n");

protected:
	FILE *fFd;
};

class IniFileParser: public File
{
public:
	IniFileParser();
	IniFileParser(const char *sIniFile);
	virtual ~IniFileParser();

	bool LoadFile(const char* sIniFile);
	bool SaveFile();
	const char* GetKeyValue(const char* sKey);
	bool SetKeyValue(const char* sKey, const char* sVal);
	void ShowKeyValue();
private:
	char sBuf[4096];
	map<string, string> KeyValMap;
};



#endif /* FILE_PARSER_H_ */
