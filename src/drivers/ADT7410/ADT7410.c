/******************************************************************************

  Copyright (C), 2013, WELLAV Technology Co., Ltd.

 ******************************************************************************
  File Name     : adt7410.c
  Version       : Initial Draft
  Author        : guanghui.chen
  Created       : 2013/12/12
  Last Modified :
  Description   : adt7410 driver'
  Function List :
              adt7410_get_temperature
              adt7410_init
              ADT7410_read
              ADT7410_write
  History       :
  1.Date        : 2013/12/12
    Author      : guanghui.chen
    Modification: Created file

******************************************************************************/
#include "ADT7410/ADT7410.h"

#include "log/log.h"
#include "i2c/i2c.h"
#include "thread/thread.h"

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>
#include <error.h>

/*****************************************************************************
  Function:     ADT7410_write
  Description:  write adt7410 register
  Input:        U8 offset
                U8 value
  Output:       none
  Return:       success return 0,and fail return -1
  Author:       guanghui.chen
*****************************************************************************/
static wvErrCode ADT7410_write(U8 offset,U8 value)
{
    wvErrCode ret = WV_SUCCESS;

    ret = I2C_WriteReg(ADT7410_DEV_NAME, ADT7410_DEV_ADDR, offset, value);
    
    return ret;
}

/*****************************************************************************
  Function:     ADT7410_read
  Description:  read adt7410
  Input:        U8 offset
                U8 *value
  Output:       none
  Return:       success return 0,and fail return -1
  Author:       guanghui.chen
*****************************************************************************/
static wvErrCode ADT7410_read(U8 offset, U8 *value)
{
    wvErrCode ret = WV_SUCCESS;

    ret = I2C_ReadReg(ADT7410_DEV_NAME, ADT7410_DEV_ADDR, offset, value);

    return ret;
}

/*****************************************************************************
  Function:     ADT7410_OpenBus
  Description:  init ADT7410
  Input:        None
  Output:       None
  Return:       success return 0,and fail return -1
  Author:       dadi.zeng
*****************************************************************************/
wvErrCode ADT7410_OpenBus(void)
{
    wvErrCode ret = WV_SUCCESS;

    ret = I2C_Open(ADT7410_DEV_NAME);

    return ret;
}

/*****************************************************************************
  Function:     ds1338_CloseBus
  Description:  close ADT7410
  Input:        None
  Output:       None
  Return:       success return 0
  Author:       dadi.zeng
*****************************************************************************/
wvErrCode ADT7410_CloseBus(void)
{
    wvErrCode ret = WV_SUCCESS;

    ret = I2C_Close(ADT7410_DEV_NAME);

    return ret;
}

/*****************************************************************************
  Function:     adt740_Detect
  Description:  detect 7410 chip
  Input:        void
  Output:       none
  Return:       Exist return 1,
                fail return 0
  Author:       guanghui.chen
*****************************************************************************/
wvErrCode ADT7410_Detect(void)
{
    wvErrCode ret = WV_SUCCESS;
    U8 id = 0;

    ret = ADT7410_read(0x0B, &id);
    
    //check chip id
    if(ADT7410_MANUFACTURE_ID != (id & ADT7410_MANUFACTURE_ID_MASK))
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_DRIVERS, "I2C device  no exist!");
        return WV_ERR_FAILURE;
    }
    else
    {
        LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_DRIVERS, "I2C device exist!");
        ADT7410_Init();
        return WV_SUCCESS;
    }
}

/*****************************************************************************
  Function:     ADT7410_Init
  Description:  init adt7410
  Input:        void
  Output:       none
  Return:       WV_TEMP_SUCCESS
  Author:       guanghui.chen
*****************************************************************************/
wvErrCode ADT7410_Init(void)
{
    //80 degree //0x28
    ADT7410_write(0x04,0x28);     //high temperature most  significant byte
    ADT7410_write(0x05,0x00);     //high temperature least significant byte

    //-10 degree
    ADT7410_write(0x06,0x85);     //low  temperature most  significant byte
    ADT7410_write(0x07,0x00);     //low  temperature least significant byte
    
    return WV_SUCCESS;
}

/*****************************************************************************
  Function:     ADT7410_GetTemperature
  Description:  get the temperature of adt7410
  Input:        none
  Output:       the temperature,conver method:
                1.Positive Temperature= ADC Code (dec)/16

                2.Negative Temperature= (ADC Code (dec) - 8192)/16
                  where ADC Codeuses the first 13 MSBs of the data byte,
                  including the sign bit.

                3.Negative Temperature= (ADC Code (dec) ?C 4096)/16
                  where Bit 15 (sign bit) is removed from the ADC cod
  Return:       WV_TEMP_SUCCESS
  Author:       guanghui.chen
*****************************************************************************/
wvErrCode ADT7410_GetTemperature(float * pfTemp)
{
    if(!pfTemp)
    {
        LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_DRIVERS, "param = NULL");
        return WV_ERR_PARAMS;
    }

    U8 value1 = 0;
    U8 value2 = 0;
    U32 u32Temp = 0;

    ADT7410_read(0x00, &value1);
    ADT7410_read(0x01, &value2);

    //normal read
    u32Temp = (value1 << 5) | (value2 >> 3);   // the highest bit is Sign,13bit mode

    if(u32Temp & 0x1000)//Negative Temperature
    {
        *pfTemp= (float)(u32Temp -8192)/16;
    }
    else
    {
        *pfTemp = (float)u32Temp/16;
    }

    return WV_SUCCESS;
}

/* example */
void *ADT7410_Example(void *arg)
{
    wvErrCode ret = WV_SUCCESS;
    float tmp   =   0.0;
    ADT7410_OpenBus();

    ret = ADT7410_Detect();
    if(WV_SUCCESS != ret)
    {
        return NULL;
    }

    while(1)
    {
        sleep(3);

        ADT7410_GetTemperature(&tmp);
        LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_DRIVERS, "Temperature is %f.", tmp);

    }
}

/*****************************************************************************
  Function:     ADT7410Task_Start
  Description:  启动获取温控线程
  Input:        none
  Output:       none
  Return:       WV_ERR_COMM_PTHREAD_CREAT       ---  失败
                WV_SUCCESS                      ---  成功
  Author:       jie.zhan
*****************************************************************************/
wvErrCode ADT7410_StartTask(void)
{
    THREAD_NEW_DETACH(ADT7410_Example, NULL, "ADT7410_Example");

    return WV_SUCCESS;
}

