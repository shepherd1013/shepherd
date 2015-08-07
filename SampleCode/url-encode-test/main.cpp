/*
 * main.cpp
 *
 *  Created on: Aug 7, 2015
 *      Author: Jacken
 */
#include <string.h>
#include "url_util.h"
#include "debug.h"

bool TestURLEncode(const char *sURL, char *sEncodedURL, unsigned int *pEncodedURLLen)
{
	bool bRet;
	bRet = URLUtil::Encode(sURL, sEncodedURL, pEncodedURLLen);
	if (bRet == false) {
		ERR_PRINT("TestURLEncode() error!\n");
		return -1;
	}
	return true;
}

bool TestURLDecode(const char *sEncodedURL, char *sURL, unsigned int *pResultLen)
{
	bool bRet;
	bRet = URLUtil::Decode(sEncodedURL, sURL, pResultLen);
	if (bRet == false) {
		ERR_PRINT("TestURLDecode() error!\n");
		return -1;
	}
	return true;
}

int main(int argc, char *argv[])
{
	bool bRet;
//	bRet = TestURLEncode("http://10.0.0.199:80/PictureCatch.cgi?username=admin&password=admin&channel=1&data_type=0&steamNo=0");
//	if (bRet == false) {
//		ERR_PRINT("TestURLEncode() error!\n");
//		return -1;
//	}
//
//	bRet = TestURLEncode("rtsp://192.168.99.100/onvif-media/media.amp?profile=OmniV4C1Audio&sessiontimeout=60&streamtype=unicast");
//	if (bRet == false) {
//		ERR_PRINT("TestURLEncode() error!\n");
//		return -1;
//	}

	char sEncodedURL[256] = {0};
	unsigned int uEncodedURLLen = 0;

	const char *sURL = "http://192.168.99.100/onvif-cgi/jpg/image.cgi?resolution=800x600&compression=20";

	bRet = TestURLEncode(sURL, sEncodedURL, &uEncodedURLLen);
	if (bRet == false) {
		ERR_PRINT("TestURLEncode() error!\n");
		return -1;
	}

	char sResult[256] = {0};
	unsigned int uResultLen = 0;

	bRet = TestURLDecode(sEncodedURL, sResult, &uResultLen);
	if (bRet == false) {
		ERR_PRINT("TestURLDecode() error!\n");
		return -1;
	}

	int nRet = strncmp(sURL, sResult, sizeof(sURL));
	if (nRet != 0) {
		ERR_PRINT("Result: Fail!\n");
		return -1;
	}
	DBG_PRINT("Result: Pass!\n");

	return 0;
}
