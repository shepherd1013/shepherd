/*
 * spd_uboot_env_parser.h
 *
 *  Created on: Mar 28, 2016
 *      Author: Jacken
 */

#ifndef SPD_UBOOT_ENV_PARSER_H_
#define SPD_UBOOT_ENV_PARSER_H_
#include "file_util.h"
#include <map>
#include <string>
#include <vector>

using namespace std;

#define BUF_SIZE 4096
#define CRC_SIZE 4

class UbootEnvParser: public File
{
public:
	UbootEnvParser(const char* sFile);
	bool LoadFile(const char* sFile);
	void ShowKeyValue();
	vector<char>* GetEnvData();
	unsigned int GetCRC();
	bool UpdateKeyValue(const char* sKey, const char* sVal = NULL);
	unsigned long int GetPartitionSize();
	map<string, string>* GetKeyValueTable();

private:
	map<string, string> m_KeyValMap;
	unsigned int		m_uCrc;
	unsigned int		m_uCrcSize;
	vector<char>		m_EnvData;
	unsigned long int m_ulPartiSize;
};


#endif /* SPD_UBOOT_ENV_PARSER_H_ */
