/*
 * spd_msgq_utest.h
 *
 *  Created on: Mar 2, 2016
 *      Author: Jacken
 */

#ifndef SPD_MSGQ_UTEST_H_
#define SPD_MSGQ_UTEST_H_

class MsgQUTest
{
public:
	void Run();
private:
	bool TestRecv();
	bool TestSend();
};


#endif /* SPD_MSGQ_UTEST_H_ */
