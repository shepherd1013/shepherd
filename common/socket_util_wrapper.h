/*
 * socket_util_wrapper.h
 *
 *  Created on: Sep 2, 2015
 *      Author: root
 */

#ifndef SOCKET_UTIL_WRAPPER_H_
#define SOCKET_UTIL_WRAPPER_H_

#ifndef BOOL
#define BOOL	int
#define TRUE	1
#define FALSE	0
#endif

#ifdef __cplusplus
extern "C" {
#endif


BOOL SocketUtil_DuplicateFD(int nOldFD, int *nNewFD);


#ifdef __cplusplus
}
#endif

#endif /* SOCKET_UTIL_WRAPPER_H_ */
