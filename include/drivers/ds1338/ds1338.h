/******************************************************************************

  Copyright (C), 2013,HuiZhou WELLAV TechnologyCo., Ltd.

 ******************************************************************************
  File Name     : DS1338.h
  Version       : Initial Draft
  Author        : dadi.zeng
  Created       : 2014/06/13
  Last Modified :
  Description   : DS1338.c header file
  Function List :
  History       :
  1.Date        : 2014/06/13
    Author      : dadi.zeng
    Modification: Created file

******************************************************************************/

#ifndef INCLUDE_DRIVERS_DS1338_H 
#define INCLUDE_DRIVERS_DS1338_H

#include "appGlobal.h"
#include "err/wv_err.h"

#define DS1338_DEV_ADDR 0x68

#define DS1338_YEAR_OFFSET  (2000)  /* оƬ��� 0 ~ 99����Ҫ������ʼ��� */
#define LINUX_YERA_OFFSE    (1900)  /* LINUX �趨ϵͳʱ�����ʼƫ����1900�� */
#define SECONDES_PER_HOUR   (3600)  /* ÿСʱ�����������ڵ���ʱ�� */

#define SECONDS_REG_ADDR    0x0
#define MINTES_REG_ADDR     0x1
#define HOURS_REG_ADDR      0x2
#define WEEKDAY_REG_ADDR    0x3
#define DATE_REG_ADDR       0x4
#define MONTH_REG_ADDR      0x5
#define YEAR_REG_ADDR       0x06

typedef enum
{
    HOURS_24,       /* 24Сʱ�� */
    HOURS_12,       /* 12Сʱ�� */
}PE_RTC_SYS;

typedef enum
{
    RTC_AM,         /* ���� */
    RTC_PM,         /* ���� */
}PE_RTC_DAY;


typedef struct PE_TIME
{
    PE_RTC_SYS enDaySys;        /* 12/24Сʱ��ʽ */
    PE_RTC_DAY enHalfDay;       /* ���� or ���� */
    U32 u32WeekDay;             /* �ܼ� */
    U32 u32Seconds;
    U32 u32Minites;
    U32 u32Hours;
    U32 u32Date;
    U32 u32Month;
    U32 u32Year;
}ST_PE_TIME;


wvErrCode   ds1338_OpenBus(void);
wvErrCode   ds1338_CloseBus(void);
wvErrCode   ds1338_GetTime(ST_PE_TIME *pstTime);
wvErrCode   ds1338_SetTime(ST_PE_TIME *pstTime);
wvErrCode   ds1338_SyncToSystem(void);
wvErrCode   ds1338_SyncToServer(void);
void ds1338_example(void);


#endif /* __DS1338_H__ */


