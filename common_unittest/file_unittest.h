/*
 * file_unittest.h
 *
 *  Created on: Nov 20, 2015
 *      Author: jacken
 */

#ifndef FILE_UNITTEST_H_
#define FILE_UNITTEST_H_

class FileUnittest
{
public:
	void Run();

private:
	bool TestFileParser();
	bool TestIniFileParser();
	bool TestFileWrite();
	bool TestFileRead();
	bool TestGetFileSize();
	bool TestGetFileAccessTime();
	bool TestGetFileModificationTime();
};



#endif /* FILE_UNITTEST_H_ */
