/**********************************************************************
* Copyright (c) 2017, WELLAV Technology Co.,Ltd.
* All rights reserved.
*
* FileName  I2C.h
* Description : The Incude of I2C Interface
* Author    : jie.zhan
* Modified  :
* Reviewer  :
* Date      : 2017-04-19
* Record    :
*
**********************************************************************/
#ifndef INCLUDE_DRIVERS_I2C_H
#define INCLUDE_DRIVERS_I2C_H

#include "appGlobal.h"
#include "err/wv_err.h"

#define I2C_BUS_COUNT	3
#define I2C_BUS_0_NAME  "/dev/i2c-0"  // PORT0 & PORT1	
#define I2C_BUS_1_NAME  "/dev/i2c-1"  // PORT2 & PORT3
#define I2C_BUS_2_NAME  "/dev/i2c-2"  // PORT4 & PORT5	
#define I2C_BUS_3_NAME  "/dev/i2c-3"  // reserved

#define I2C_DEV_MAX     (2)
#define I2C_DEV_NAME_0  "/dev/i2c-0"
#define I2C_DEV_NAME_1  "/dev/i2c-1"

struct I2cInfo
{
    char *dev_name;  //i2c 设备名字
    int  fd;         //ioctl 操作的描述符
};

wvErrCode I2C_InitAll(void);
wvErrCode I2C_Open(const char *dev_name);
wvErrCode I2C_WriteReg(const char *dev_name, const unsigned short chip_addr, const unsigned char reg_addr, const unsigned char value);
wvErrCode I2C_ReadReg(const char *dev_name, const unsigned short chip_addr, const unsigned char reg_addr, unsigned char *value);
wvErrCode I2C_WriteRegWithCheck(const char *dev_name, const unsigned short chip_addr, const unsigned char reg_addr, const unsigned char value);

#endif /* _WV_I2C_H_ */

