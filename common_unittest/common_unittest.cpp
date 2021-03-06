/*
 * common_unittest.cpp
 *
 *  Created on: Sep 20, 2015
 *      Author: jacken
 */
#include "common_unittest.h"
#include "debug.h"
#include "file_unittest.h"
#include "spd_msgq_utest.h"
#include "spd_ipc_server_test.h"
#include "spd_ipc_client_test.h"
#include "spd_string_util_test.h"
#include "spd_tcp_server_test.h"
#include "spd_tcp_client_test.h"

void Unittest::Run()
{
//	FileUnittest Filetest;
//	Filetest.Run();

//	MsgQUTest MQtest;
//	MQtest.Run();

//	IPCServerTest IPCSvrTest;
//	IPCSvrTest.Run();

//	IPCClientTest IPCCltTest;
//	IPCCltTest.Run();

//	StrUtilTest StrTest;
//	StrTest.Run();

//	TcpServerTest TcpSvr;
//	TcpSvr.Run();

	TcpClientTest TcpClt;
	TcpClt.Run();
}



