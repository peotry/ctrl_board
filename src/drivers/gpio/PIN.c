/******************************************************************************
* Copyright (c) 2017, WELLAV Technology Co.,Ltd.
* All rights reserved.
*
* FileName PIN.c
* Description : control pin
* Author    : ruibin.zhang
* Modified  :
* Reviewer  :
* Date      : 2017-02-23
* Record    :
*
******************************************************************************/
#include "GPIO.h"
#include "gpio/PIN.h"

#include "log/log.h"

#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <stdbool.h>

BOOL g_LEDfresh_flag = 0;

/*  led module  */
/*****************************************************************************
 * led module 背板状态指示灯
*****************************************************************************/

/*****************************************************************************
  Function:     PIN_LEDGreenLight
  Description:  Set the Green led light
  Input:        none
  Output:       none
  Return:       none
  Author:       ruibin.zhang
  Modify:
*****************************************************************************/
void* PIN_LEDGreenLight(void *arg)
{
    GPIO_SetGpio(LED1, 0);
    GPIO_SetGpio(LED2, 0);

    LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_TOOLS, "Set Status LED Green");
    //Status_SetLEDStatus(EN_GREEN);
}

/*****************************************************************************
 Function:     PIN_LEDRedLight
 Description:  Set the Red led light
 Input:        none
 Output:       none
 Return:       none
 Author:       ruibin.zhang
 Modify:
*****************************************************************************/
void* PIN_LEDRedLight(void *arg)
{
   GPIO_SetGpio(LED1, 0);
   GPIO_SetGpio(LED2, 1);

   LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_TOOLS, "Set Status LED Red");
   //Status_SetLEDStatus(EN_RED);
}

/*****************************************************************************
 Function:     PIN_LEDOut
 Description:  Set the led light out
 Input:        none
 Output:       none
 Return:       none
 Author:       ruibin.zhang
 Modify:
*****************************************************************************/
void PIN_LEDOut()
{
   GPIO_SetGpio(LED1, 1);
   GPIO_SetGpio(LED2, 1);

   //Status_SetLEDStatus(EN_OUT);
}

/*****************************************************************************
 Function:     PIN_LEDGreenfresh
 Description:  Green LED fresh
 Input:        none
 Output:       none
 Return:       none
 Author:       ruibin.zhang
 Modify:
*****************************************************************************/
void * PIN_LEDGreenfresh(void *arg)
{
    while(1)
    {
        PIN_LEDGreenLight(arg);
        usleep(LED_FRESH_FREQ);
        PIN_LEDOut();
        usleep(LED_FRESH_FREQ);

        if(0 == g_LEDfresh_flag)
        {
            LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_DRIVERS, "Stop LED Fresh");
            break;
        }
    }
}

/*****************************************************************************
  Function:     PIN_LEDGreenfresh_Start
  Description:  启动绿灯闪烁
  Input:        none
  Output:       none
  Return:       WV_ERR_FAILURE         --  失败
                WV_SUCCESS             --  成功
  Author:       ruibin.zhang
*****************************************************************************/
wvErrCode PIN_LEDGreenfresh_Start(void)
{
    int res = 0;
    pthread_t LEDGreenfresh_thread;

    g_LEDfresh_flag = 1;

    res = pthread_create(&LEDGreenfresh_thread, NULL, PIN_LEDGreenfresh, NULL);
    if(-1 == res)
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_DRIVERS, "LEDGreenfresh thread create error :%s(errno: %d)",strerror(errno),errno);
        return WV_ERR_PIN_PTHREAD_CREAT;
    }
    else
    {
        LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_DRIVERS, "LEDGreenfresh create successfully!");
    }

    res = pthread_detach(LEDGreenfresh_thread);
    if(-1 == res)
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_DRIVERS, "LEDGreenfresh thread detach error :%s(errno: %d)",strerror(errno),errno);
        return WV_ERR_PIN_PTHREAD_CREAT;
    }
    else
    {
        LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_DRIVERS, "LEDGreenfresh thread detach successfully!");
    }

    return WV_SUCCESS;
}

/*****************************************************************************
 Function:     PIN_LEDRedfresh
 Description:  Red LED fresh
 Input:        none
 Output:       none
 Return:       none
 Author:       ruibin.zhang
 Modify:
*****************************************************************************/
void* PIN_LEDRedfresh(void *arg)
{
    while(1)
    {
        PIN_LEDRedLight(arg);
        usleep(LED_FRESH_FREQ);
        PIN_LEDOut();
        usleep(LED_FRESH_FREQ);

        if(0 == g_LEDfresh_flag)
        {
            LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_DRIVERS, "Stop LED Fresh");
            break;
        }
    }
}

/*****************************************************************************
  Function:     PIN_LEDRedfresh_Start
  Description:  启动红灯闪烁
  Input:        none
  Output:       none
  Return:       WV_ERR_FAILURE         --  失败
                WV_SUCCESS             --  成功
  Author:       ruibin.zhang
*****************************************************************************/
wvErrCode PIN_LEDRedfresh_Start(void)
{
    int res = 0;
    pthread_t LEDRedfresh_thread;

    g_LEDfresh_flag = 1;

    res = pthread_create(&LEDRedfresh_thread, NULL, PIN_LEDRedfresh, NULL);
    if(-1 == res)
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_DRIVERS, "LEDRedfresh thread create error :%s(errno: %d)",strerror(errno),errno);
        return WV_ERR_PIN_PTHREAD_CREAT;
    }
    else
    {
        LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_DRIVERS, "LEDRedfresh create successfully!");
    }

    res = pthread_detach(LEDRedfresh_thread);
    if(-1 == res)
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_DRIVERS, "LEDRedfresh thread detach error :%s(errno: %d)",strerror(errno),errno);
        return WV_ERR_PIN_PTHREAD_CREAT;
    }
    else
    {
        LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_DRIVERS, "LEDRedfresh thread detach successfully!");
    }

    return WV_SUCCESS;
}

/*****************************************************************************
  Function:     PIN_LEDfresh_Stop
  Description:  闪烁停止
  Input:        none
  Output:       none
  Return:       WV_ERR_FAILURE         --  失败
                WV_SUCCESS             --  成功
  Author:       ruibin.zhang
*****************************************************************************/
void PIN_LEDfresh_Stop(void)
{
    g_LEDfresh_flag = 0;
    usleep(LED_FRESH_FREQ*2);
    PIN_LEDOut();
}

/*  INTERFACE module  */
/*****************************************************************************
 * interfaces module 背板插槽
*****************************************************************************/

// 槽号 MIO数组，方便循环
U32 Interfaces[MAX_INTERFACE_NUM] = {INTERFACE0, INTERFACE1, INTERFACE2, INTERFACE3, INTERFACE4,
        INTERFACE5, INTERFACE6, INTERFACE7, INTERFACE8, INTERFACE9 , INTERFACE10,
        INTERFACE11, INTERFACE12, INTERFACE13, INTERFACE14, INTERFACE15};

/*****************************************************************************
 Function:     PIN_CheckPlug
 Description:  Set the led light out
 Input:        none
 Output:       none
 Return:       none
 Author:       ruibin.zhang
 Modify:
*****************************************************************************/
//TODO U16定义大些  方便拓展
void PIN_CheckPlug(VU16 * pvu16SubBoardStatus)
{
    U32 u32value   = 1;
    U32 i       = 0;

    *pvu16SubBoardStatus = 0x0;
    for(i = 0; i < MAX_INTERFACE_NUM; i++)
    {
        GPIO_GetGpio(Interfaces[i], &u32value);
        usleep(INTERFACE_CHECK_FREQ);
        if(0 == u32value)
        {
            //LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_DRIVERS, "The interfaces %d Plugged!", i);
            u32value = 1;
            *pvu16SubBoardStatus |= (0x1 << i);
        }
    }

}

/*  HW Version module  */
/*****************************************************************************
 * HW Version 硬件版本
*****************************************************************************/

/*****************************************************************************
 Function:     PIN_GetHWVersion
 Description:  Get Hw Version
 Input:        none
 Output:       none
 Return:       U32 u32HWVer     -       HW Version
 Author:       ruibin.zhang
 Modify:
*****************************************************************************/
U32 PIN_GetHWVersion(void)
{
    U32 u32value   = 0;
    U32 u32HWVer   = 0;

    GPIO_GetGpio(HWVER1, &u32value);
    if(1 == u32value)
    {
        u32HWVer = 1;
    }

    GPIO_GetGpio(HWVER2, &u32value);
    if(1 == u32value)
    {
        u32HWVer |= (0x1 << 1);
    }

    return u32HWVer;
}
