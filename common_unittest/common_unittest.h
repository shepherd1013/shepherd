/*
 * common_unittest.h
 *
 *  Created on: Sep 20, 2015
 *      Author: jacken
 */

#ifndef COMMON_UNITTEST_H_
#define COMMON_UNITTEST_H_

class Unittest
{
public:
	void Run();
private:
	bool TestFileParser();
	bool TestIniFileParser();
};



#endif /* COMMON_UNITTEST_H_ */
