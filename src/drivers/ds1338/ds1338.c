/******************************************************************************

  Copyright (C), 2014,HuiZhou WELLAV Technology Co., Ltd.

 ******************************************************************************
  File Name     : adt7410.c
  Version       : Initial Draft
  Author        : dadi.zeng
  Created       : 2014/06/13
  Last Modified :
  Description   : adt7410 driver'
  Function List :
                  ds1338_OpenBus
                  ds1338_read
                  ds1338_write
                  ds1338_GetTime
                  ds1338_VerifyTimeParameters
                  ds1338_SetTime
  History       :
  1.Date        : 2014/06/13
    Author      : dadi.zeng
    Modification: Created file

******************************************************************************/
#include "ds1338/ds1338.h"

#include "i2c/i2c.h"
#include "log/wv_log.h"

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

#ifdef RELEASE_VER
#define __func__ "RTC"
#endif

volatile int g_I2CFd = -1; //ds1338 i2c dev file descriptor

/*****************************************************************************
 Prototype    : ds1338_write
 Description  : write ds1338 register
 Input        : unsigned char offset  
                unsigned char value   
 Output       : None
 Return Value : static
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/06/13
    Author       : dadi.zeng
    Modification : Created function

*****************************************************************************/
static wvErrCode ds1338_write(unsigned char offset,unsigned char value)
{
    wvErrCode ret = WV_SUCCESS;

    ret = I2C_WriteReg(I2C_DEV_NAME_0, DS1338_DEV_ADDR, offset, value);

    return ret;
}


/*****************************************************************************
 Prototype    : ds1338_read
 Description  : read ds1338
 Input        : unsigned char offset  
                unsigned char *value  
 Output       : None
 Return Value : static
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/06/13
    Author       : dadi.zeng
    Modification : Created function

*****************************************************************************/
static wvErrCode ds1338_read(unsigned char offset,unsigned char *value)
{
    wvErrCode ret = WV_SUCCESS;

    ret = I2C_ReadReg(I2C_DEV_NAME_0, DS1338_DEV_ADDR, offset, value);

    return ret;
}

/*****************************************************************************
 Prototype    : ds1338_OpenBus
 Description  : init ds1338
 Input        : void  
 Output       : None
 Return Value : None
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/06/13
    Author       : dadi.zeng
    Modification : Created function

*****************************************************************************/
wvErrCode ds1338_OpenBus(void)
{
    wvErrCode ret = WV_SUCCESS;

    ret = I2C_Open(I2C_DEV_NAME_0); 

    return ret;
}


wvErrCode ds1338_CloseBus(void)
{    
    wvErrCode ret = WV_SUCCESS;
    
    ret = I2C_Close(I2C_DEV_NAME_0);

    return ret;
}



/*****************************************************************************
 Prototype    : ds1338_VerifyTimeParameters
 Description  : Verify the time parameters
 Input        : ST_PE_TIME * pstTime - time pointer  
 Output       : None
 Return Value : None
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/06/13
    Author       : dadi.zeng
    Modification : Created function

*****************************************************************************/
wvErrCode ds1338_VerifyTimeParameters(ST_PE_TIME * pstTime)
{
    bool isFalse = false;
    
    if(!pstTime)
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_DRIVERS, "Error: param = NULL");
        return WV_ERR_PARAMS;
    }

    if(pstTime->u32Year > (DS1338_YEAR_OFFSET + 99))
    {
        isFalse = true;
    }

    if((pstTime->u32Month > 12) || (pstTime->u32Month < 1))
    {
        isFalse = true;
    }

    if((pstTime->u32Date > 31) || (pstTime->u32Date < 1))
    {
        isFalse = true;
    }

    if(HOURS_24 != pstTime->enDaySys)
    {
        if((pstTime->u32Hours > 12) || (pstTime->u32Hours < 1))
        {
            isFalse = true;
        }
    }
    else
    {
        if(pstTime->u32Hours > 23)
        {
            isFalse = true;
        }    
    }

    if(pstTime->u32Minites > 59)
    {
        isFalse = true;
    }

    if(pstTime->u32Seconds > 59)
    {
        isFalse = true;
    }

    if(isFalse)
    {
        return WV_ERR_FAILURE;
    }
    else
    {
        return WV_SUCCESS;
    }

}

/*****************************************************************************
 Prototype    : ds1338_SetTime
 Description  : setting time for ds1338
 Input        : ST_PE_TIME * pstTime - time pointer  
 Output       : None
 Return Value : None
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/06/13
    Author       : dadi.zeng
    Modification : Created function

*****************************************************************************/
wvErrCode ds1338_SetTime(ST_PE_TIME * pstTime)
{
    wvErrCode enRet = WV_SUCCESS;
    U8 u8RegValue = 0;
    U8 u8TenValue = 0;
    U8 u8Value = 0;

    if(!pstTime)
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_DRIVERS, "Error: param = NULL");
        return WV_ERR_PARAMS;
    }

    //debug
    LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_DRIVERS, "\n ==== Set Time ====!\n", __func__, __LINE__);
    LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_DRIVERS, "Year:%d!", pstTime->u32Year);
    LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_DRIVERS, "Mon :%d!", pstTime->u32Month);
    LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_DRIVERS, "Date:%d!", pstTime->u32Date);
    LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_DRIVERS, "Hour:%d!", pstTime->u32Hours);
    LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_DRIVERS, "Min :%d!", pstTime->u32Minites);
    LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_DRIVERS, "Sec :%d!", pstTime->u32Seconds);

    /* verify parameters valid */
    enRet = ds1338_VerifyTimeParameters(pstTime);
    if(WV_SUCCESS != enRet)
    {
        return WV_ERR_FILE_CREAT;
    }

    /* seconds */
    u8RegValue = 0;
    //ds1338_read(SECONDS_REG_ADDR, &u8RegValue);
    //u8RegValue &= (~0x7f);
    u8TenValue = pstTime->u32Seconds / 10;
    u8Value    = pstTime->u32Seconds % 10;
    u8RegValue |= ((u8TenValue << 4) | u8Value); 
    ds1338_write(SECONDS_REG_ADDR, u8RegValue); 

    /* minutes */
    u8RegValue = 0;
    //ds1338_read(MINTES_REG_ADDR, &u8RegValue);
    //u8RegValue &= (~0x7f);
    u8TenValue = pstTime->u32Minites / 10;
    u8Value    = pstTime->u32Minites % 10;
    u8RegValue |= ((u8TenValue << 4) | u8Value);     
    ds1338_write(MINTES_REG_ADDR, u8RegValue);  

    /* hours */
    u8RegValue = 0;
    //ds1338_read(HOURS_REG_ADDR, &u8RegValue);
    //u8RegValue &= (~0x7f);
    u8TenValue = pstTime->u32Hours / 10;
    u8Value    = pstTime->u32Hours % 10;
    u8RegValue |= ((u8TenValue << 4) | u8Value);     
    if(HOURS_12 != pstTime->enDaySys)
    {
        u8RegValue &= ~(0x1 << 6);       
    }
    else
    {
        u8RegValue |= (0x1 << 6); 
        if(RTC_AM ==pstTime->enHalfDay)
        {
            u8RegValue &= ~(0x1 << 5);       
        }
        else
        {
            u8RegValue |= (0x1 << 5);          
        }
    }
    ds1338_write(HOURS_REG_ADDR, u8RegValue);   

    /* date */
    u8RegValue = 0;
    //ds1338_read(DATE_REG_ADDR, &u8RegValue);
    //u8RegValue &= (~0x3f);
    u8TenValue = pstTime->u32Date / 10;
    u8Value    = pstTime->u32Date % 10;
    u8RegValue |= ((u8TenValue << 4) | u8Value);     
    ds1338_write(DATE_REG_ADDR, u8RegValue);     

    /* month */
    u8RegValue = 0;
    //ds1338_read(MONTH_REG_ADDR, &u8RegValue);
    //u8RegValue &= (~0x1f);
    u8TenValue = pstTime->u32Month / 10;
    u8Value    = pstTime->u32Month % 10;
    u8RegValue |= ((u8TenValue << 4) | u8Value);     
    ds1338_write(MONTH_REG_ADDR, u8RegValue);     


    /* year */
    u8RegValue = 0;
    pstTime->u32Year -= DS1338_YEAR_OFFSET;
    //ds1338_read(YEAR_REG_ADDR, &u8RegValue);
    //u8RegValue &= 0;
    u8TenValue = pstTime->u32Year / 10;
    u8Value    = pstTime->u32Year % 10;
    u8RegValue |= ((u8TenValue << 4) | u8Value);     
    ds1338_write(YEAR_REG_ADDR, u8RegValue);   

    /* 设置完成后，同步到linux系统 */
    enRet = ds1338_SyncToSystem();

    return enRet;
}

/*****************************************************************************
 Prototype    : ds1338_GetTime
 Description  : get time form ds1338
 Input        : ST_PE_TIME * pstTime - time pointer  
 Output       : None
 Return Value : None
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/06/13
    Author       : dadi.zeng
    Modification : Created function

*****************************************************************************/
wvErrCode  ds1338_GetTime(ST_PE_TIME * pstTime)
{
    U8 u8RegValue = 0;
    U8 u8TenValue = 0;
    U8 u8Value = 0;

    if(!pstTime)
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_DRIVERS, "Error: param = NULL");
        return WV_ERR_PARAMS;
    }

    //seconds
    ds1338_read(SECONDS_REG_ADDR, &u8RegValue);
    u8TenValue = ((u8RegValue & 0x70) >> 4)*10;
    u8Value = u8RegValue & 0xf;
    pstTime->u32Seconds = u8TenValue  + u8Value; 

    //minutes
    ds1338_read(MINTES_REG_ADDR, &u8RegValue);
    u8TenValue = ((u8RegValue & 0x70) >> 4)*10;
    u8Value = u8RegValue & 0xf;
    pstTime->u32Minites = u8TenValue  + u8Value;  

    //hours
    ds1338_read(HOURS_REG_ADDR, &u8RegValue);
    if(0 != (u8RegValue & (0x1 << 6))) 
    {
        //printf("12 hours system!\n");
    
        u8TenValue = ((u8RegValue & 0x10) >> 4)*10;
        u8Value = u8RegValue & 0xf;
        pstTime->u32Hours = u8TenValue  + u8Value;  
        pstTime->enDaySys = HOURS_12;

        if(0 == (u8RegValue & (0x1 << 5)))
        {
            pstTime->enHalfDay = RTC_AM;
        }
        else
        {
            pstTime->enHalfDay = RTC_PM;   
        }
    }
    else
    {
        //printf("24 hours system!\n");
        u8TenValue = ((u8RegValue & 0x30) >> 4)*10;
        u8Value = u8RegValue & 0xf;
        pstTime->u32Hours = u8TenValue  + u8Value;  
        pstTime->enDaySys = HOURS_24;
    }

    //weekday
    ds1338_read(WEEKDAY_REG_ADDR, &u8RegValue);
    pstTime->u32WeekDay = u8TenValue & 0x7;  

    //date
    ds1338_read(DATE_REG_ADDR, &u8RegValue);
    u8TenValue = ((u8RegValue & 0x30) >> 4)*10;
    u8Value = u8RegValue & 0xf;
    pstTime->u32Date = u8TenValue  + u8Value;    

    //month
    ds1338_read(MONTH_REG_ADDR, &u8RegValue);
    u8TenValue = ((u8RegValue & 0x10) >> 4)*10;
    u8Value = u8RegValue & 0xf;
    pstTime->u32Month = u8TenValue  + u8Value; 

    //year
    ds1338_read(YEAR_REG_ADDR, &u8RegValue);
    u8TenValue = ((u8RegValue & 0xf0) >> 4)*10;
    u8Value = u8RegValue & 0xf;
    pstTime->u32Year = u8TenValue  + u8Value + DS1338_YEAR_OFFSET; 


    //debug
    LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_DRIVERS, "\n ==== Get Time ====!");
    LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_DRIVERS, "Year:%d!", pstTime->u32Year);
    LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_DRIVERS, "Mon :%d!", pstTime->u32Month);
    LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_DRIVERS, "Date:%d!", pstTime->u32Date);
    LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_DRIVERS, "Hour:%d!", pstTime->u32Hours);
    LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_DRIVERS, "Min :%d!", pstTime->u32Minites);
    LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_DRIVERS, " Sec :%d!", pstTime->u32Seconds);

    fsync(0);
    fsync(1);

    return WV_SUCCESS;
}



/*****************************************************************************
 Prototype    : ds1338_SyncToSystem
 Description  : Sync ds1338 time to linux system
 Input        : None 
 Output       : None
 Return Value : 
                PE_SUCCESS          - success
                PE_ERR_RTC_SYNC_ERR - fail
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/06/13
    Author       : dadi.zeng
    Modification : Created function

*****************************************************************************/
wvErrCode ds1338_SyncToSystem(void)
{
    U32 ret = 0;
    time_t stLinuxTime;
    ST_PE_TIME stRtcTime;
    struct tm stHTime;

    /* 得到RTC 时间 */
    ds1338_GetTime(&stRtcTime);

    /* 赋值给中间变量 */
    stHTime.tm_sec  = stRtcTime.u32Seconds;
    stHTime.tm_min  = stRtcTime.u32Minites;
    stHTime.tm_hour = stRtcTime.u32Hours;
    stHTime.tm_mday = stRtcTime.u32Date;
    stHTime.tm_mon  = stRtcTime.u32Month - 1;
    stHTime.tm_year = stRtcTime.u32Year - LINUX_YERA_OFFSE;

    /* 转换为系统时间格式 */
    stLinuxTime = mktime(&stHTime);

    /* 设置linux系统时间， 注意: 超级用户才有此权限  */
    ret = stime(&stLinuxTime);
    if(ret != 0)
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_DRIVERS, "Sync rtc to linux system error!");
        return WV_ERR_FAILURE;
    }

    return WV_SUCCESS;

}


/* example */
void ds1338_example(void)
{
    ST_PE_TIME stTime;
    ds1338_OpenBus();

    int i = 0;
    while(1)
    {
        //Get time
        ds1338_GetTime(&stTime);
        printf("%d Y: %d M: %d D:%d H: %d M:%d S\n", stTime.u32Year, stTime.u32Month, stTime.u32Date ,stTime.u32Hours ,stTime.u32Minites, stTime.u32Seconds);

        //Set time
        printf("++++++++++++++++++++++++\n");
        stTime.u32Year  = 2014;
        stTime.u32Month = 06;
        stTime.u32Date  = 26;
        stTime.u32Hours = 9;
        stTime.u32Minites   = 28;
        stTime.u32Seconds   = 8;
        stTime.enDaySys     = HOURS_24;
        ds1338_SetTime(&stTime);


        for(i = 0; i < 10; i++)
        {
            usleep(500000);
        }
    }
}

