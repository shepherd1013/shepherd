/*
 * spd_string_util_test.h
 *
 *  Created on: Mar 18, 2016
 *      Author: Jacken
 */

#ifndef SPD_STRING_UTIL_TEST_H_
#define SPD_STRING_UTIL_TEST_H_

class StrUtilTest
{
public:
	void Run();
private:
	void TestReplace();
	bool DoReplace(const char* sTestData, const char* sTestResult, const char* sOldKey, const char* sNewKey);
};


#endif /* SPD_STRING_UTIL_TEST_H_ */
