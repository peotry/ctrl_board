/******************************************************************************
* Copyright (c) 2017, WELLAV Technology Co.,Ltd.
* All rights reserved.
*
* FileName PIN.h
* Description : control pin
* Author    : ruibin.zhang
* Modified  :
* Reviewer  :
* Date      : 2017-02-23
* Record    :
*
******************************************************************************/
#ifndef INCLUDE_DRIVERS_PIN_H
#define INCLUDE_DRIVERS_PIN_H

#include "appGlobal.h"
#include "err/wv_err.h"

/*****************************************************************************
 * led module 背板状态指示灯
*****************************************************************************/
#define LED_FRESH_FREQ          1000000        //灯闪烁频率（us）

typedef enum{
    LED1    =   40,
    LED2    =   33
}LED_PIN;

void* PIN_LEDGreenLight(void *arg);
void* PIN_LEDRedLight(void *arg);
void PIN_LEDOut();
wvErrCode PIN_LEDGreenfresh_Start(void);
wvErrCode PIN_LEDRedfresh_Start(void);
void PIN_LEDfresh_Stop(void);

/*****************************************************************************
 * interfaces module 背板插槽
*****************************************************************************/
#define INTERFACE_CHECK_FREQ    1000        //循环检测子板是否插入 时间间隔（us）

// MIO  1   无插入
//      0   有插入
typedef enum{
    INTERFACE0      =   16,
    INTERFACE1      =   18,
    INTERFACE2      =   22,
    INTERFACE3      =   24,
    INTERFACE4      =   45,
    INTERFACE5      =   26,
    INTERFACE6      =   32,
    INTERFACE7      =   50,
    INTERFACE8      =   34,
    INTERFACE9      =   36,
    INTERFACE10     =   37,
    INTERFACE11     =   51,
    INTERFACE12     =   43,
    INTERFACE13     =   39,
    INTERFACE14     =   41,
    INTERFACE15     =   28,
}Interface_PIN;

void PIN_CheckPlug(VU16 * SubBoardStatus);

/*****************************************************************************
 * HW Version 硬件版本
*****************************************************************************/
typedef enum{
    HWVER1      =   30,
    HWVER2      =   29
}HWVER_PIN;

U32 PIN_GetHWVersion(void);

#endif
