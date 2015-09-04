/*
 * main.cpp
 *
 *  Created on: Aug 13, 2015
 *      Author: Jacken
 */
#include "url_util.h"
#include "debug.h"
#include <stdio.h>

extern char* program_invocation_short_name;

bool Usage()
{
	fprintf(stdout, "Usage: %s [HTTP Encoded URL]\n", program_invocation_short_name);
	return true;
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		Usage();
		return -1;
	}

	const char *sEncodedURL = argv[1];
	char sURL[256] = {0};
	unsigned int uURLLen;

	bool bRet = URLUtil::Decode(sEncodedURL, sURL, &uURLLen);
	if (bRet == false) {
		ERR_PRINT("URLUtil::Decode() error !\n");
		return -1;
	}
	printf("%s\n", sURL);
	return 0;
}
