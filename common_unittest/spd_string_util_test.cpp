/*
 * spd_string_util_test.cpp
 *
 *  Created on: Mar 18, 2016
 *      Author: Jacken
 */
#include "spd_string_util_test.h"
#include "string_util.h"
#include "debug.h"
#include <string.h>

void StrUtilTest::Run()
{
	this->TestReplace();
}

bool StrUtilTest::DoReplace(const char* sTestData, const char* sTestResult, const char* sOldKey, const char* sNewKey)
{
	char sBuf[256] = {0};
	strcpy(sBuf, sTestData);
	if (StringUtil::Replace(sBuf, sizeof(sBuf), sOldKey, sNewKey) == false) {
		ERR_PRINT("Replace string failed!\n");
		return false;
	}
	DBG_PRINT("Replace result: %s\n", sBuf);
	if (strcmp(sTestResult, sBuf) != 0) {
		return false;
	}
	return true;
}

void StrUtilTest::TestReplace()
{
	const char* sTestData = "123test456test";
	const char* sExpectedResult = "123abc456abc";
	const char* sOldKey = "test";
	const char* sNewKey = "abc";
	DBG_PRINT("Test Data: %s\n", sTestData);
	if (this->DoReplace(sTestData, sExpectedResult, sOldKey, sNewKey) == false) {
		ERR_PRINT("Test failed!\n");
		return;
	}
	DBG_PRINT("Test result: PASS\n");


	sTestData = "test456test789";
	sExpectedResult = "abc456abc789";
	DBG_PRINT("Test Data: %s\n", sTestData);
	if (this->DoReplace(sTestData, sExpectedResult, sOldKey, sNewKey) == false) {
		ERR_PRINT("Test failed!\n");
		return;
	}
	DBG_PRINT("Test result: PASS\n");

	sTestData = "test456test789";
	sExpectedResult = "abc456abc789";
	DBG_PRINT("Test Data: %s\n", sTestData);
	if (this->DoReplace(sTestData, sExpectedResult, sOldKey, sNewKey) == false) {
		ERR_PRINT("Test failed!\n");
		return;
	}
	DBG_PRINT("Test result: PASS\n");

	sTestData = "123testtest789";
	sExpectedResult = "123abcabc789";
	DBG_PRINT("Test Data: %s\n", sTestData);
	if (this->DoReplace(sTestData, sExpectedResult, sOldKey, sNewKey) == false) {
		ERR_PRINT("Test failed!\n");
		return;
	}
	DBG_PRINT("Test result: PASS\n");

	sTestData = "123testtest";
	sExpectedResult = "123abcabc";
	DBG_PRINT("Test Data: %s\n", sTestData);
	if (this->DoReplace(sTestData, sExpectedResult, sOldKey, sNewKey) == false) {
		ERR_PRINT("Test failed!\n");
		return;
	}
	DBG_PRINT("Test result: PASS\n");

	sTestData = "testtest";
	sExpectedResult = "abcabc";
	DBG_PRINT("Test Data: %s\n", sTestData);
	if (this->DoReplace(sTestData, sExpectedResult, sOldKey, sNewKey) == false) {
		ERR_PRINT("Test failed!\n");
		return;
	}
	DBG_PRINT("Test result: PASS\n");

	sTestData = "abctestabctestabc";
	sExpectedResult = "abcabcabcabcabc";
	DBG_PRINT("Test Data: %s\n", sTestData);
	if (this->DoReplace(sTestData, sExpectedResult, sOldKey, sNewKey) == false) {
		ERR_PRINT("Test failed!\n");
		return;
	}
	DBG_PRINT("Test result: PASS\n");

	sOldKey = "test";
	sNewKey = "abcde";
	sTestData = "abctestabctestabc";
	sExpectedResult = "abcabcdeabcabcdeabc";
	DBG_PRINT("Test Data: %s\n", sTestData);
	if (this->DoReplace(sTestData, sExpectedResult, sOldKey, sNewKey) == false) {
		ERR_PRINT("Test failed!\n");
		return;
	}
	DBG_PRINT("Test result: PASS\n");

	sOldKey = "test";
	sNewKey = "";
	sTestData = "abctestabctestabc";
	sExpectedResult = "abcabcabc";
	DBG_PRINT("Test Data: %s\n", sTestData);
	if (this->DoReplace(sTestData, sExpectedResult, sOldKey, sNewKey) == false) {
		ERR_PRINT("Test failed!\n");
		return;
	}
	DBG_PRINT("Test result: PASS\n");
}
