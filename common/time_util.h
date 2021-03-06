/*
 * time_util.h
 *
 *  Created on: Jul 7, 2015
 *      Author: Jacken
 */

#ifndef TIME_UTIL_H_
#define TIME_UTIL_H_
#include <time.h>

class TimeUtil
{
public:
	static bool GetUptime(struct timespec *tTime);
/*
	struct timespec {
		time_t   tv_sec;        //	seconds
		long     tv_nsec;       //	nanoseconds
	};
*/
	static struct timespec		DiffTime(struct timespec tTimeA, struct timespec tTimeB);
	static unsigned long int	SpecTimeToMS(struct timespec tTime);
};


#endif /* TIME_UTIL_H_ */
