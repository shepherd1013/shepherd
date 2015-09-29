/*
 * file_parser.h
 *
 *  Created on: Sep 20, 2015
 *      Author: jacken
 */

#ifndef FILE_PARSER_H_
#define FILE_PARSER_H_
#include <stdio.h>

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

class IniFileParser: public FileParser
{
public:
	IniFileParser(const char *sFilename) : FileParser(sFilename){};
	virtual ~IniFileParser();

	bool GetKeyValue(const char *sKey, char *sValue, unsigned int uValueSize);
};



#endif /* FILE_PARSER_H_ */
