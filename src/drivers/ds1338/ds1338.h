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

#define DS1338_YEAR_OFFSET  (2000)  /* 芯片年份 0 ~ 99，需要加上起始年份 */
#define LINUX_YERA_OFFSE    (1900)  /* LINUX 设定系统时间的起始偏移是1900年 */
#define SECONDES_PER_HOUR   (3600)  /* 每小时的秒数，用于调整时区 */

#define SECONDS_REG_ADDR    0x0
#define MINTES_REG_ADDR     0x1
#define HOURS_REG_ADDR      0x2
#define WEEKDAY_REG_ADDR    0x3
#define DATE_REG_ADDR       0x4
#define MONTH_REG_ADDR      0x5
#define YEAR_REG_ADDR       0x06

typedef enum
{
    HOURS_24,       /* 24小时制 */
    HOURS_12,       /* 12小时制 */
}PE_RTC_SYS;

typedef enum
{
    RTC_AM,         /* 上午 */
    RTC_PM,         /* 下午 */
}PE_RTC_DAY;


typedef struct PE_TIME
{
    PE_RTC_SYS enDaySys;        /* 12/24小时制式 */
    PE_RTC_DAY enHalfDay;       /* 上午 or 下午 */
    U32 u32WeekDay;             /* 周几 */
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


