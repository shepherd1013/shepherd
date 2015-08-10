/*
 * url_util.h
 *
 *  Created on: Aug 7, 2015
 *      Author: Jacken
 */

#ifndef URL_UTIL_H_
#define URL_UTIL_H_

class URLUtil
{
public:
	/*
	 * Ref:
	 *		https://www.ietf.org/rfc/rfc3986.txt
	 *		http://meyerweb.com/eric/tools/dencoder/
	 *		http://www.w3schools.com/tags/ref_urlencode.asp
	 *
	 * Test pattern:
	 * 		1.http://10.0.0.199:80/PictureCatch.cgi?username=admin&password=admin&channel=1&data_type=0&steamNo=0
	 * 			=>http%3A%2F%2F10.0.0.199%3A80%2FPictureCatch.cgi%3Fusername%3Dadmin%26password%3Dadmin%26channel%3D1%26data_type%3D0%26steamNo%3D0
	 *
	 * 		2.rtsp://192.168.99.100/onvif-media/media.amp?profile=OmniV4C1Audio&sessiontimeout=60&streamtype=unicast
	 * 			=>rtsp%3A%2F%2F192.168.99.100%2Fonvif-media%2Fmedia.amp%3Fprofile%3DOmniV4C1Audio%26sessiontimeout%3D60%26streamtype%3Dunicast
	 *
	 * 		3.http://10.0.0.199:80/PictureCatch.cgi?username=admin&password=admin&channel=1&data_type=0&steamNo=0
	 * 			=>http%3A%2F%2F10.0.0.199%3A80%2FPictureCatch.cgi%3Fusername%3Dadmin%26password%3Dadmin%26channel%3D1%26data_type%3D0%26steamNo%3D0
	 */
	static bool Encode(const char *sURL, char *sEncodedURL, unsigned int *pEncodedURLLen);
	static bool Decode(const char *sEncodedURL, char *sURL, unsigned int *pURLLen);
};


#endif /* URL_UTIL_H_ */
