/******************************************************************************
* Copyright (c) 2017, WELLAV Technology Co.,Ltd.
* All rights reserved.
*
* FileName xilinx_i2c.c
* Description :xlinx i2c controller driver
* Author    : guanghui.chen
* Modified  : ruibin.zhang
* Reviewer  :
* Date      : 2017-02-26
* Record    :
*
******************************************************************************/

#include "i2c/i2c.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <semaphore.h>

volatile int g_I2C0Fd = -1;
volatile int g_I2C1Fd = -1;

static sem_t  g_stI2CSem;

/*****************************************************************************
  Function:     i2c_LockInit
  Description:  none
  Input:        none
  Output:       none
  Return:       none
  Author:       ruibin.zhang
*****************************************************************************/
static void i2c_LockInit(void)
{
    sem_init(&g_stI2CSem, 0, 1);
}

/*****************************************************************************
  Function:     i2c_Lock
  Description:  none
  Input:        none
  Output:       none
  Return:       none
  Author:       ruibin.zhang
*****************************************************************************/
static void i2c_Lock(void)
{
    sem_wait(&g_stI2CSem);
}

/*****************************************************************************
  Function:     i2c_UnLock
  Description:  none
  Input:        none
  Output:       none
  Return:       none
  Author:       ruibin.zhang
*****************************************************************************/
static void i2c_UnLock(void)
{
    sem_post(&g_stI2CSem);
}

/*****************************************************************************
  Function:     i2c_set_slave_addr
  Description:  set device's slave addrs
  Input:        int file
                int address
                int force
  Output:       none
  Return:       SUCCESS 0;FAILURE -1
  Author:       guanghui.chen
*****************************************************************************/
int i2c_set_slave_addr(int file, int address, int force)
{
    int ret = I2C_FAILURE;

    /* With force, let the user read from/write to the registers
       even when a driver is also running */
    ret = ioctl(file, force ? I2C_SLAVE_FORCE : I2C_SLAVE, address);
    if (I2C_SUCCESS > ret)
    {
        printf("Could not set address to 0x%02x,ret = %d.\n",address,ret);
        return I2C_FAILURE;
    }

    return I2C_SUCCESS;
}

/*****************************************************************************
  Function:     i2c_smbus_access
  Description:  i2c access func
  Input:        int file
                char read_write
                unsigned char command
                int size
                union i2c_smbus_data *data
  Output:       none
  Return:       SUCCESS 0;FAILURE -1
  Author:       guanghui.chen
*****************************************************************************/
static int i2c_smbus_access(int file, char read_write, unsigned char command,
                                     int size, union i2c_smbus_data *data)
{
    struct i2c_smbus_ioctl_data args;

    args.read_write = read_write;
    args.command = command;
    args.size = size;
    args.data = data;
    return ioctl(file,I2C_SMBUS,&args);
}

/*****************************************************************************
  Function:     i2c_smbus_access
  Description:  i2c access func
  Input:        int file
                char read_write
                unsigned char command
                int size
                union i2c_smbus_data *data
  Output:       none
  Return:       SUCCESS 0;FAILURE -1
  Author:       huada.huang
*****************************************************************************/
static int i2c_rdwr_access(int file, unsigned char address, unsigned short read_write,
                                    int size, unsigned char *data)
{
    int ret = I2C_FAILURE;
    struct i2c_rdwr_ioctl_data args;

    if (size > 0)
    {
        struct i2c_msg msgs =
        {
             .addr = address,
             .flags = read_write, //read or write
             .len = size,
             .buf = data
        };

        args.nmsgs = 1;
        args.msgs = &msgs;

        ret = ioctl(file, I2C_RDWR,&args);
    }
    return ret;
}

/*****************************************************************************
  Function:     i2c_smbus_write_byte_data
  Description:  i2c write by one byte
  Input:        int file
                unsigned char command
                unsigned char value
  Output:       none
  Return:       SUCCESS 0;FAILURE -1
  Author:       guanghui.chen
*****************************************************************************/
static int i2c_smbus_write_byte_data(int file, unsigned char command, unsigned char value)
{
    union i2c_smbus_data data;

    data.byte = value;
    return i2c_smbus_access(file,I2C_SMBUS_WRITE,command,
                            I2C_SMBUS_BYTE_DATA, &data);
}

/*****************************************************************************
  Function:     i2c_smbus_read_byte_data
  Description:  i2c smbus read by one byte
  Input:        int file
                unsigned char command
                unsigned char *value
  Output:       none
  Return:       SUCCESS 0;FAILURE -1
  Author:       guanghui.chen
*****************************************************************************/
static int i2c_smbus_read_byte_data(int file, unsigned char command, unsigned char *value)
{
    int ret = I2C_FAILURE;
    union i2c_smbus_data data;

    ret = i2c_smbus_access(file,I2C_SMBUS_READ,command,I2C_SMBUS_BYTE_DATA,&data);
    if (I2C_SUCCESS == ret)
    {
        *value = data.byte;
        return I2C_SUCCESS;
    }
    else
    {
        return ret;
    }
}

/*****************************************************************************
  Function:     i2c_receive_msg
  Description:  i2c receive msg
  Input:        int file
                unsigned char address
                unsigned char *value
                unsigned char len
  Output:       none
  Return:       SUCCESS 0;FAILURE -1
  Author:       huada.huang
*****************************************************************************/
int i2c_receive_msg(int file, unsigned char address, unsigned char *value, unsigned char len)
{
    int ret = I2C_FAILURE;
    i2c_Lock();

    ret = i2c_rdwr_access(file, address, I2C_M_RD, len, value);
    if(I2C_SUCCESS > ret)
    {
        printf("i2c_receive_msg:  error,ret = %x.\n",ret);
        return I2C_FAILURE;
    }
    i2c_UnLock();
    return I2C_SUCCESS;
}

/*****************************************************************************
  Function:     i2c_write_msg
  Description:  i2c write msg
  Input:        int file
                unsigned char address
                unsigned char *value
                unsigned char len
  Output:       none
  Return:       SUCCESS 0;FAILURE -1
  Author:       huada.huang
*****************************************************************************/
int i2c_write_msg(int file, unsigned char address, unsigned char *value, unsigned char len)
{
    int ret = I2C_FAILURE;
    i2c_Lock();

    ret = i2c_rdwr_access(file, address, 0, len, value);
    if(I2C_SUCCESS > ret)
    {
        printf("i2c_write_msg:  error,ret = %x.\n",ret);
        return I2C_FAILURE;
    }
    i2c_UnLock();
    return I2C_SUCCESS;
}

/*****************************************************************************
  Function:     i2c_dev_open
  Description:  i2c dev open
  Input:        const char *dev_name       device name,such as "/dev/i2c-0"
                unsigned char device_addr  slave device address,such as 0x4B(ADT7410)
  Output:       none
  Return:       SUCCESS 0;FAILURE -1
  Author:       guanghui.chen
*****************************************************************************/
int i2c_dev_open(const char *dev_name,unsigned char device_addr)
{
    int fd = -1;
    int ret = I2C_FAILURE;

    i2c_LockInit();

    fd = open(dev_name, O_RDWR);
    if(0 > fd)
    {
        printf("i2c_dev_open:open error.\n");
        return I2C_FAILURE;
    }
    ret = i2c_set_slave_addr(fd,device_addr,0);
    if(I2C_SUCCESS > ret)
    {
        close(fd);
        printf("i2c_dev_open:set_slave_addr error.\n");
        return I2C_FAILURE;
    }

    return fd;

}

/*****************************************************************************
  Function:     i2c_dev_close
  Description:  close i2c device
  Input:        int file  the open device file descriptor
  Output:       none
  Return:       SUCCESS 0;FAILURE -1
  Author:       guanghui.chen
*****************************************************************************/
int i2c_dev_close(int file)
{
    int ret = I2C_FAILURE;

    ret = close(file);

    return ret;
}

/*****************************************************************************
  Function:     i2c_write
  Description:  i2c write by one byte
  Input:        int file              the open device file descriptor
                unsigned char offset  address of the register
                unsigned char value   the value you want to set
  Output:       none
  Return:       SUCCESS 0;FAILURE -1
  Author:       guanghui.chen
*****************************************************************************/
int i2c_write(int file,unsigned char offset,unsigned char value)
{
    int ret = I2C_FAILURE;
    i2c_Lock();

    ret = i2c_smbus_write_byte_data(file, offset, value);
    if(I2C_SUCCESS > ret)
    {
        printf("i2c_write:write offset %d error,ret = %x.\n",offset,ret);
        return I2C_FAILURE;
    }

    i2c_UnLock();
    return I2C_SUCCESS;
}

/*****************************************************************************
  Function:     i2c_read
  Description:  i2c read by one byte
  Input:        int file              the open device file descriptor
                unsigned char offset  address of the register
                unsigned char *value  the buf you want to save data
  Output:       none
  Return:       SUCCESS 0;FAILURE -1
  Author:       guanghui.chen
*****************************************************************************/
int i2c_read(int file,unsigned char offset,unsigned char *value)
{
    unsigned char data = 0;
    int ret = I2C_FAILURE;
    i2c_Lock();

    ret = i2c_smbus_read_byte_data(file, offset, &data);
    if(I2C_SUCCESS > ret)
    {
        printf("i2c_read:read offset %d error,ret = %x.\n",offset,ret);
        return I2C_FAILURE;
    }
    *value = data;

    i2c_UnLock();
    return I2C_SUCCESS;
}

