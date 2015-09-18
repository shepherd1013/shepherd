/*
 * socket_util_wrapper.h
 *
 *  Created on: Sep 2, 2015
 *      Author: root
 */

#ifndef SOCKET_UTIL_WRAPPER_H_
#define SOCKET_UTIL_WRAPPER_H_

#include "common_define.h"

//#ifdef __cplusplus
//extern "C" {
//#endif


BOOL SocketUtil_DuplicateFD(int nOldFD, int *nNewFD);
BOOL SocketUtil_Socket(int domain, int type, int protocol, int *sFD);


//#ifdef __cplusplus
//}
//#endif

#endif /* SOCKET_UTIL_WRAPPER_H_ */
