/*
 * gpio_util.cpp
 *
 *  Created on: Oct 27, 2015
 *      Author: Jacken
 */
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include "gpio_util.h"
#include "driver_util.h"
#include "debug.h"

GpioUtil::GpioUtil(unsigned int uGroupNum, unsigned int uPinNum)
{
	m_uGroupNum = uGroupNum;
	m_uPinNum = uPinNum;
}

GpioUtil::~GpioUtil()
{
}

bool GpioUtil::SetPinDirectionToOutput()
{
	const char *sDeviceName = "/dev/mem";
	int nFD = -1;
	const unsigned int REGISTER_SIZE = 0x1000;
	unsigned int uPhysicalMemAddr = 0;
	const unsigned int uGPIODirectionOffset = 0x400;

	if(this->GetGPIOGroupPhysicalAddr(&uPhysicalMemAddr) == false) {
		ERR_PRINT("this->GetGPIOGroupPhysicalAddr() error!\n");
		return false;
	}

	if(Driver::Open(sDeviceName, O_RDWR, &nFD) == false) {
		ERR_PRINT("Driver::Open(%s) error!\n", sDeviceName);
		return false;
	}

	void *pVirtualMemAddr = NULL;
	pVirtualMemAddr = (unsigned char *) mmap((void *)0, REGISTER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, nFD, uPhysicalMemAddr);

	if (pVirtualMemAddr == (void*)-1) {
		ERR_PRINT("Invalid virtual memory address (%p)!\n", pVirtualMemAddr);
		if(Driver::Close(nFD) == false) {
			ERR_PRINT("Driver::Close() error!\n");
		}
		return false;
	}

	(*(volatile unsigned int *)(pVirtualMemAddr + uGPIODirectionOffset)) |= (1 << m_uPinNum);

	int nRet = munmap(pVirtualMemAddr, REGISTER_SIZE);
	if (nRet < 0) {
		nRet = errno;
		fprintf(stderr, "munmap(): %s\n", strerror(nRet));
		if(Driver::Close(nFD) == false) {
			ERR_PRINT("Driver::Close() error!\n");
		}
		return false;
	}

	if(Driver::Close(nFD) == false) {
		ERR_PRINT("Driver::Close() error!\n");
		return false;
	}

	return true;
}

bool GpioUtil::SetPinDirectionToInput()
{
	const char *sDeviceName = "/dev/mem";
	int nFD = -1;
	const unsigned int REGISTER_SIZE = 0x1000;
	unsigned int uPhysicalMemAddr = 0;
	const unsigned int uGPIODirectionOffset = 0x400;

	if(this->GetGPIOGroupPhysicalAddr(&uPhysicalMemAddr) == false) {
		ERR_PRINT("this->GetGPIOGroupPhysicalAddr() error!\n");
		return false;
	}

	if(Driver::Open(sDeviceName, O_RDWR, &nFD) == false) {
		ERR_PRINT("Driver::Open(%s) error!\n", sDeviceName);
		return false;
	}

	void *pVirtualMemAddr = NULL;
	pVirtualMemAddr = (unsigned char *) mmap((void *)0, REGISTER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, nFD, uPhysicalMemAddr);

	if (pVirtualMemAddr == (void*)-1) {
		ERR_PRINT("Invalid virtual memory address (%p)!\n", pVirtualMemAddr);
		if(Driver::Close(nFD) == false) {
			ERR_PRINT("Driver::Close() error!\n");
		}
		return false;
	}

	(*(volatile unsigned int *)(pVirtualMemAddr + uGPIODirectionOffset)) &= (~(1 << m_uPinNum));

	int nRet = munmap(pVirtualMemAddr, REGISTER_SIZE);
	if (nRet < 0) {
		nRet = errno;
		fprintf(stderr, "munmap(): %s\n", strerror(nRet));
		if(Driver::Close(nFD) == false) {
			ERR_PRINT("Driver::Close() error!\n");
		}
		return false;
	}

	if(Driver::Close(nFD) == false) {
		ERR_PRINT("Driver::Close() error!\n");
		return false;
	}

	return true;
}

bool GpioUtil::ReadPinValue(unsigned int *pValue)
{
	const char *sDeviceName = "/dev/mem";
	int nFD = -1;
	const unsigned int REGISTER_SIZE = 0x1000;
	unsigned int uPhysicalMemAddr = 0;
	unsigned int uValue = 0;

	if(this->GetGPIOGroupPhysicalAddr(&uPhysicalMemAddr) == false) {
		ERR_PRINT("this->GetGPIOGroupPhysicalAddr() error!\n");
		return false;
	}

	if(Driver::Open(sDeviceName, O_RDWR, &nFD) == false) {
		ERR_PRINT("Driver::Open(%s) error!\n", sDeviceName);
		return false;
	}

	void *pVirtualMemAddr = NULL;
	pVirtualMemAddr = (unsigned char *) mmap((void *)0, REGISTER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, nFD, uPhysicalMemAddr);

	if (pVirtualMemAddr == (void*)-1) {
		ERR_PRINT("Invalid virtual memory address (%p)!\n", pVirtualMemAddr);
		if(Driver::Close(nFD) == false) {
			ERR_PRINT("Driver::Close() error!\n");
		}
		return false;
	}

//	DBG_PRINT("uPhysicalMemAddr: 0x%x \n", uPhysicalMemAddr);
//	DBG_PRINT("uPhysicalMemAddr+ (1 << (m_nPin + 2)): 0x%x \n", uPhysicalMemAddr + (1 << (m_nPin + 2)));
	uValue = *((volatile unsigned int *)(pVirtualMemAddr + (1 << (m_uPinNum + 2))));
	uValue = uValue >> (m_uPinNum);

	int nRet = munmap(pVirtualMemAddr, REGISTER_SIZE);
	if (nRet < 0) {
		nRet = errno;
		fprintf(stderr, "munmap(): %s\n", strerror(nRet));
		if(Driver::Close(nFD) == false) {
			ERR_PRINT("Driver::Close() error!\n");
		}
		return false;
	}

	if(Driver::Close(nFD) == false) {
		ERR_PRINT("Driver::Close() error!\n");
		return false;
	}

	return true;
}

bool GpioUtil::SetPinValueToOne()
{
	const char *sDeviceName = "/dev/mem";
	int nFD = -1;
	const unsigned int REGISTER_SIZE = 0x1000;
	unsigned int uPhysicalMemAddr = 0;

	if(this->GetGPIOGroupPhysicalAddr(&uPhysicalMemAddr) == false) {
		ERR_PRINT("this->GetGPIOGroupPhysicalAddr() error!\n");
		return false;
	}

	if(Driver::Open(sDeviceName, O_RDWR, &nFD) == false) {
		ERR_PRINT("Driver::Open(%s) error!\n", sDeviceName);
		return false;
	}

	void *pVirtualMemAddr = NULL;
	pVirtualMemAddr = (unsigned char *) mmap((void *)0, REGISTER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, nFD, uPhysicalMemAddr);

	if (pVirtualMemAddr == (void*)-1) {
		ERR_PRINT("Invalid virtual memory address (%p)!\n", pVirtualMemAddr);
		if(Driver::Close(nFD) == false) {
			ERR_PRINT("Driver::Close() error!\n");
		}
		return false;
	}

	*((volatile unsigned int *)(pVirtualMemAddr + (1 << (m_uPinNum + 2)))) |= (1 << m_uPinNum);

	int nRet = munmap(pVirtualMemAddr, REGISTER_SIZE);
	if (nRet < 0) {
		nRet = errno;
		fprintf(stderr, "munmap(): %s\n", strerror(nRet));
		if(Driver::Close(nFD) == false) {
			ERR_PRINT("Driver::Close() error!\n");
		}
		return false;
	}

	if(Driver::Close(nFD) == false) {
		ERR_PRINT("Driver::Close() error!\n");
		return false;
	}
	return true;
}

bool GpioUtil::SetPinValueToZero()
{
	const char *sDeviceName = "/dev/mem";
	int nFD = -1;
	const unsigned int REGISTER_SIZE = 0x1000;
	unsigned int uPhysicalMemAddr = 0;

	if(this->GetGPIOGroupPhysicalAddr(&uPhysicalMemAddr) == false) {
		ERR_PRINT("this->GetGPIOGroupPhysicalAddr() error!\n");
		return false;
	}

	if(Driver::Open(sDeviceName, O_RDWR, &nFD) == false) {
		ERR_PRINT("Driver::Open(%s) error!\n", sDeviceName);
		return false;
	}

	void *pVirtualMemAddr = NULL;
	pVirtualMemAddr = (unsigned char *) mmap((void *)0, REGISTER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, nFD, uPhysicalMemAddr);

	if (pVirtualMemAddr == (void*)-1) {
		ERR_PRINT("Invalid virtual memory address (%p)!\n", pVirtualMemAddr);
		if(Driver::Close(nFD) == false) {
			ERR_PRINT("Driver::Close() error!\n");
		}
		return false;
	}

	*((volatile unsigned int *)(pVirtualMemAddr + (1 << (m_uPinNum + 2)))) &= (~(1 << m_uPinNum));

	int nRet = munmap(pVirtualMemAddr, REGISTER_SIZE);
	if (nRet < 0) {
		nRet = errno;
		fprintf(stderr, "munmap(): %s\n", strerror(nRet));
		if(Driver::Close(nFD) == false) {
			ERR_PRINT("Driver::Close() error!\n");
		}
		return false;
	}

	if(Driver::Close(nFD) == false) {
		ERR_PRINT("Driver::Close() error!\n");
		return false;
	}
	return true;
}

bool GpioUtil::GetGPIOGroupPhysicalAddr(unsigned int *pPhysicalAddr)
{
	switch (m_uGroupNum) {
	case 0:
		*pPhysicalAddr = 0xF8B20000;
		break;
	case 1:
		*pPhysicalAddr = 0xF8B21000;
		break;
	case 2:
		*pPhysicalAddr = 0xF8B22000;
		break;
	case 3:
		*pPhysicalAddr = 0xF8B23000;
		break;
	case 4:
		*pPhysicalAddr = 0xF8B24000;
		break;
	case 5:
		*pPhysicalAddr = 0xF8004000;
		break;
	case 6:
		*pPhysicalAddr = 0xF8B26000;
		break;
	default:
		ERR_PRINT("Unknown GPIO group (%u)!\n", m_uGroupNum);
		return false;
	}
	return true;
}
