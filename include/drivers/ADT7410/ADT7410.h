/******************************************************************************

  Copyright (C), 2013,HuiZhou WELLAV TechnologyCo., Ltd.

 ******************************************************************************
  File Name     : adt7410.h
  Version       : Initial Draft
  Author        : guanghui.chen
  Created       : 2013/12/12
  Last Modified :
  Description   : adt7410.c header file
  Function List :
  History       :
  1.Date        : 2013/12/12
    Author      : guanghui.chen
    Modification: Created file

******************************************************************************/

#ifndef INCLUDE_DRIVERS_ADT7410_H
#define INCLUDE_DRIVERS_ADT7410_H

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "err/wv_err.h"

//TODO 根据挂在哪个I2C组件定义
#define ADT7410_DEV_NAME I2C_DEV_NAME_0
#define ADT7410_DEV_ADDR 0x4B

#define ADT7410_MANUFACTURE_ID 0xC8
#define ADT7410_MANUFACTURE_ID_MASK 0xF8 //[7:3]  11001  R  Manufacture ID  Contains the manufacturer identification numbe

//TODO 以下 数据需重新评测
#define AD7410_LOWEST_TEMP_VALUE   0x1C90 //-55 degree
#define AD7410_WARNING_TEMP_VALUE  0x4E0  //78 degree  
#define AD7410_POWEROFF_TEMP_VALUE 0x520  //82 degree 

#define AD7410_NORMAL_RECONGNIZE_LIMIT 0x960  //根据芯片手册，最大工作温度为150度

#define POWEROFF_THRESHOLD  3

wvErrCode ADT7410_OpenBus(void);
wvErrCode ADT7410_CloseBus(void);
wvErrCode ADT7410_Init(void);
wvErrCode ADT7410_Detect(void);
wvErrCode ADT7410_GetTemperature(float *pfTemp);
void *ADT7410_Example(void *arg);
wvErrCode ADT7410_StartTask(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __ADT7410_H__ */


