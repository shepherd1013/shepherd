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
	~FileParser();

	bool ReadLine(char *sLine, unsigned int uLineSize);
	bool ReadLineWithToken(char *sLine, unsigned int uLineSize, const char *sToken = "\n");

private:
	FILE *fFd;
};

#endif /* FILE_PARSER_H_ */
