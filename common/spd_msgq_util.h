/*
 * msgq_util.h
 *
 *  Created on: Feb 22, 2016
 *      Author: Jacken
 */

#ifndef MSGQ_UTIL_H_
#define MSGQ_UTIL_H_

#define MAX_MSGQ_TEXT 4096 //Maximum size is 8192 bytes
class MsgQ
{
public:
	MsgQ();
	MsgQ(unsigned int uKey, bool bCreation);
	~MsgQ();

	bool LoadKey(unsigned int uKey);
	bool Connect();
	bool Send(const char* sSndData, unsigned int uDataSize, unsigned int uTimeoutMS);
	bool Recv(char* sRecvBuf, unsigned int uBufSize, unsigned int* uRecvSize);

protected:
	int		m_nKey;
	int		m_nID;
	bool	m_bCreation;
};


#endif /* MSGQ_UTIL_H_ */
