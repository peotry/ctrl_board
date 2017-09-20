/**********************************************************************
* Copyright (c) 2017, WELLAV Technology Co.,Ltd.
* All rights reserved.
*
* FileName  I2C.c
* Description : The Driver of I2C Interface
* Author    : jie.zhan
* Modified  :
* Reviewer  :
* Date      : 2017-04-19
* Record    :
*
**********************************************************************/

#include "i2c/i2c.h"

#include "log/wv_log.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>

static struct I2cInfo s_stI2cInfo[I2C_DEV_MAX] = {
            {I2C_DEV_NAME_0, -1},
            {I2C_DEV_NAME_1, -1},
        };


/*
* function: I2C_FindInfoIndex
*
* description: 找到i2c结构体的索引
*
* input:  @dev_name: 设备名字
*
* output: @
*
* return@ 
* success: index 
*    fail: -1
*
* author: linsheng.pan
*/
static int I2C_FindInfoIndex(const char *dev_name)
{
    if(!dev_name)
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_DRIVERS, "Error: param = NULL");
        return -1;
    }

    int i = 0;

    for(i = 0; i < I2C_DEV_MAX; ++i)
    {
        if(!strcmp(s_stI2cInfo[i].dev_name, dev_name))
        {
            break;
        }
    }

    if(I2C_DEV_MAX == i)
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_DRIVERS, "Error: can't find i2c dev:%s", dev_name);
        return -1;
    }

    return i;
}



/*
* function: I2C_InitAll
*
* description: 初始化所有的I2C
*
* input:  @
*
* output: @
*
* return@ 
* success: 
*    fail: 
*
* author: linsheng.pan
*/
wvErrCode I2C_InitAll(void)
{
    int i = 0;
    for(i = 0; i < I2C_DEV_MAX; ++i)
    {
        I2C_Open(s_stI2cInfo[i].dev_name);
    }

    return WV_SUCCESS;
}


/*
* function: I2C_Open
*
* description: 打开i2c设备
*
* input:  @
*
* output: @
*
* return@ 
* success: 
*    fail: 
*
* author: linsheng.pan
*/
wvErrCode I2C_Open(const char *dev_name)
{
    if(!dev_name)
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_DRIVERS, "Error: param = NULL");
        return WV_ERR_PARAMS;
    }

    //if dev existed
    char err_str[ERR_BUF_LEN] = {0};
    int index = I2C_FindInfoIndex(dev_name);
    if(index < 0)
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_DRIVERS, "Error: can't open i2c dev: %s", dev_name);
        return WV_ERR_FAILURE;
    }

    //open
    if(s_stI2cInfo[index].fd > 0)
    {
        LOG_PRINTF(LOG_LEVEL_WARNING, LOG_MODULE_DRIVERS, "%s is already opened, fd: %d", dev_name, s_stI2cInfo[index].fd);
        return WV_SUCCESS;
    }

    s_stI2cInfo[index].fd = open(dev_name, O_RDWR); 
    if(s_stI2cInfo[index].fd > 0)
    {
        LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_DRIVERS, "Open i2c dev:%s successful, fd: %d.", dev_name, s_stI2cInfo[index].fd);
        return WV_SUCCESS;
    }
    else
    {
        ERR_STRING(err_str);
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_DRIVERS, "Error: open i2c dev:%s , %s", err_str);
        return WV_ERR_FAILURE;
    }
}


/*
* function: I2C_Close
*
* description: 关闭i2c设备
*
* input:  @dev_name:
*
* output: @
*
* return@ 
* success: 
*    fail: 
*
* author: linsheng.pan
*/
wvErrCode I2C_Close(const char *dev_name)
{
    if(!dev_name)
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_DRIVERS, "Error: param = NULL");
        return WV_ERR_PARAMS;
    }

    //if dev existed
    char err_str[ERR_BUF_LEN] = {0};
    int index = I2C_FindInfoIndex(dev_name);
    if(index < 0)
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_DRIVERS, "Error: can't find i2c dev: %s", dev_name);
        return WV_ERR_FAILURE;
    }

    CLOSE(s_stI2cInfo[index].fd);

    return WV_SUCCESS;
}

/*
* function: I2C_WriteReg
*
* description: 向i2c总线上地址为 chip_addr 的芯片里面的寄存器写入一个值
*
* input:  @dev_name:i2c设备名字
*         @chip_addr:芯片地址
*         @reg_addr:寄存器地址
*         @value:值
*
* output: @
*
* return@ 
* success: WV_SUCCESS
*    fail: WV_ERR_FAILURE
*
* author: linsheng.pan
*/
wvErrCode I2C_WriteReg(const char *dev_name, const unsigned short chip_addr, const unsigned char reg_addr, const unsigned char value)
{
    if(!dev_name)
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_DRIVERS, "Error: param = NULL");
        return WV_ERR_PARAMS;
    }

	struct i2c_rdwr_ioctl_data  I2C_Data;
    char err_buf[ERR_BUF_LEN] = {0};
    int index = 0;
    wvErrCode ret = WV_SUCCESS;	

    if((index = I2C_FindInfoIndex(dev_name)) < 0)
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_DRIVERS, "Error: can't find i2c dev:%s", dev_name);
        return WV_ERR_FAILURE;
    }

	I2C_Data.nmsgs = 1;
	I2C_Data.msgs  = (struct i2c_msg *)malloc(I2C_Data.nmsgs * sizeof(struct i2c_msg));
	I2C_Data.msgs[0].len    = 2;
	I2C_Data.msgs[0].addr   = (chip_addr);
	I2C_Data.msgs[0].flags  = 0;
	I2C_Data.msgs[0].buf    = (unsigned char*)malloc(I2C_Data.msgs[0].len);
	I2C_Data.msgs[0].buf[0] = reg_addr;
	I2C_Data.msgs[0].buf[1] = value;

    if(ioctl(s_stI2cInfo[index].fd, I2C_RDWR, (unsigned long)&I2C_Data) < 0)
    {
        ERR_STRING(err_buf);
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_DRIVERS, "Error: fd: %d, call ioctl:%s", s_stI2cInfo[index].fd, err_buf);
        ret = WV_ERR_FAILURE;
    }
   
	free(I2C_Data.msgs[0].buf);
	free(I2C_Data.msgs);

	return ret;
}

/*
* function: I2C_ReadReg
*
* description: 向i2c总线上地址为 chip_addr 的芯片里面的寄存器读取一个值
*
* input:  @dev_name:i2c设备名字
*         @chip_addr:芯片地址
*         @reg_addr:寄存器地址
*         @value:值
*
* output: @
*
* return@ 
* success: WV_SUCCESS
*    fail: WV_ERR_FAILURE
*
* author: linsheng.pan
*/
wvErrCode I2C_ReadReg(const char *dev_name, const unsigned short chip_addr, const unsigned char reg_addr, unsigned char *value)
{
    if((!dev_name) || (!value))
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_DRIVERS, "Error: param = NULL");
        return WV_ERR_PARAMS;
    }

	struct i2c_rdwr_ioctl_data  I2C_Data;
    char err_buf[ERR_BUF_LEN] = {0};
    int index = 0;
    wvErrCode ret = WV_SUCCESS;	

    if((index = I2C_FindInfoIndex(dev_name)) < 0)
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_DRIVERS, "Error: can't find i2c dev:%s", dev_name);
        return WV_ERR_FAILURE;
    }


    I2C_Data.nmsgs  = 2;
    I2C_Data.msgs   = (struct i2c_msg *)malloc(I2C_Data.nmsgs * sizeof(struct i2c_msg));
    I2C_Data.msgs[0].len    = 1;
    I2C_Data.msgs[0].addr   = (chip_addr );
    I2C_Data.msgs[0].flags  = 0;
    I2C_Data.msgs[0].buf    = (unsigned char*)malloc(1);
    I2C_Data.msgs[0].buf[0] = reg_addr;
    
	I2C_Data.msgs[1].len    = 1;
	I2C_Data.msgs[1].addr   = (chip_addr);
    I2C_Data.msgs[1].flags  = I2C_M_RD;
    I2C_Data.msgs[1].buf    = (unsigned char*)malloc(1);
    I2C_Data.msgs[1].buf[0] = 0;	

    if(ioctl(s_stI2cInfo[index].fd, I2C_RDWR, (unsigned long)&I2C_Data) < 0)
    {
        ERR_STRING(err_buf);
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_DRIVERS, "Error: fd: %d, call ioctl:%s", s_stI2cInfo[index].fd, err_buf);
        ret = WV_ERR_FAILURE;
    }
    else
    {
        *value = I2C_Data.msgs[1].buf[0];
    }
   
    free(I2C_Data.msgs[0].buf);
    free(I2C_Data.msgs[1].buf);
    free(I2C_Data.msgs);

	return ret;
}

/*
* function: I2C_WriteRegWithCheck
*
* description: 向i2c总线上地址为 chip_addr 的芯片里面的寄存器写入一个值
*
* input:  @dev_name:i2c设备名字
*         @chip_addr:芯片地址
*         @reg_addr:寄存器地址
*         @value:值
*
* output: @
*
* return@ 
* success: WV_SUCCESS
*    fail: WV_ERR_FAILURE
*
* author: linsheng.pan
*/
wvErrCode I2C_WriteRegWithCheck(const char *dev_name, const unsigned short chip_addr, const unsigned char reg_addr, const unsigned char value)
{

    wvErrCode ret = WV_SUCCESS;
    unsigned char read_val = 0;

    if(WV_SUCCESS == I2C_WriteReg(dev_name, chip_addr, reg_addr, value))
    {
        if(WV_SUCCESS == I2C_ReadReg(dev_name, chip_addr, reg_addr, &read_val))
        {
            if(value != read_val) 
            {
                printf("Error:ChipAddr:0x%x, Reg:0x%x, WriteVal = %d, ReadVal = %d\n",chip_addr, reg_addr, value, read_val); 
            }
            else
            {
                printf("Error:ChipAddr:0x%x, Reg:0x%x, WriteVal = %d\n",chip_addr, reg_addr, value); 
            }
        }

    }

    return ret;
}

