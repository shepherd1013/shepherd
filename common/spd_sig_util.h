/*
 * spd_sig_util.h
 *
 *  Created on: Feb 15, 2016
 *      Author: Jacken
 */

#ifndef SPD_SIG_UTIL_H_
#define SPD_SIG_UTIL_H_
#include <signal.h>
#include <time.h>

class SigUtil
{
public:
	static bool Block(int nSigNo);
	static const char* GetSignalString(int nSigNo);
	static bool Wait(time_t tMS, unsigned int *uSigNo);
};

#endif /* SPD_SIG_UTIL_H_ */
