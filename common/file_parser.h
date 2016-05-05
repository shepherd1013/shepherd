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

#define BUF_MAX 4096

using namespace std;

class FileParser
{
public:
	FileParser(const char *sFilename);
	virtual ~FileParser();

	bool ReadLine(char *sLine, unsigned int uLineSize);
	bool ReadLineWithToken(char *sLine, unsigned int uLineSize, const char *sToken = "\n");

protected:
	FILE* fFd;
};

class IniFileParser: public File
{
public:
	IniFileParser();
	IniFileParser(const char *sIniFile);
	virtual ~IniFileParser();

	virtual bool Load(const char* sIniFile);
	bool SaveFile();
	const char* GetKeyValue(const char* sKey);
	bool SetKeyValue(const char* sKey, const char* sVal);
	bool SetKeyValue(const char* sKey, bool bVal);
	bool SetKeyValue(const char* sKey, int nVal);
	void ShowKeyValue();
	bool GetKeyValue(const char* sKey, int *pVal);
	bool GetKeyValue(const char* sKey, unsigned int *pVal);
	bool GetKeyValue(const char* sKey, bool *pVal);

private:
	char m_sBuf[BUF_MAX];
	map<string, string> m_KeyValMap;
};



#endif /* FILE_PARSER_H_ */
