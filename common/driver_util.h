/*
 * driver_util.h
 *
 *  Created on: Aug 28, 2015
 *      Author: Jacken
 */

#ifndef DRIVER_UTIL_H_
#define DRIVER_UTIL_H_

class Driver
{
public:
	static bool Open(const char *filename, int flags, int *nFD);
	/*
	 * flags: O_RDONLY, O_WRONLY, or O_RDWR
	 */
	static bool Close(int nFD);
	static bool Ioctl(int nFD, unsigned long int uReq, void *pData);
};


#endif /* DRIVER_UTIL_H_ */
