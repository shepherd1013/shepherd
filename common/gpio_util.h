/*
 * gpio_util.h
 *
 *  Created on: Oct 27, 2015
 *      Author: Jacken
 */

#ifndef GPIO_UTIL_H_
#define GPIO_UTIL_H_

enum GpioDir {
	GPIO_DIR_IN = 0,
	GPIO_DIR_OUT
};

class GpioUtil
{
public:
	GpioUtil(unsigned int uGroupNum, unsigned int uPinNum);
	~GpioUtil();
	virtual bool SetPinDirectionToOutput();
	virtual bool SetPinDirectionToInput();
	virtual bool ReadPinValue(unsigned int *pValue);
	virtual bool SetPinValueToOne();
	virtual bool SetPinValueToZero();

//	bool SetPinValue(unsigned int uGruop, unsigned int pin, unsigned int pValue);
//	bool SetPinDirection(unsigned int uGruop, unsigned int pin);
//	bool GetGroupPhysicalAddr(unsigned int uGruop, unsigned int *pPhysicalAddr);
//	bool ReadPinValue(unsigned int uGruop, unsigned int pin, unsigned int *pValue);

protected:
	unsigned int m_uGroupNum;
	unsigned int m_uPinNum;

	virtual bool GetGPIOGroupPhysicalAddr(unsigned int *pPhysicalAddr);
};


#endif /* GPIO_UTIL_H_ */
