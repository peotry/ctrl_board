/*****************************************************************************
* Copyright (c) 2017, WELLAV Technology Co.,Ltd.
* All rights reserved.
*
* FileName WV_err.c
* Description : 返回错误string 模块
* Author    : ruibin.zhang
* Modified  :
* Reviewer  :
* Date      : 2017-02-28
* Record    :
*
*****************************************************************************/
#include "err/wv_err.h"

#include "log/log.h"

#include <stdarg.h>

wvLanguage g_enLanguage = EN_LANGUAGE_CN;


ST_PE_ERR g_stErrArray[] = 
{
    // code                             English                         Chinese
    // sys
    {  WV_SUCCESS,                      "Success!",                     "成功", },
    {  WV_ERR_FAILURE,                  "Failure!",                     "失败", },
    {  WV_ERR_CONFIG_TIMEOUOT,          "Config timeout!",              "配置超时", },
    {  WV_ERR_VER,                      "Version error!",               "版本错误", },

    // add new err code in here
    { WV_ERR_USER_TOKEN_ERR,   "",  "请登录",  },
    { WV_ERR_USER_TOKEN_TIMEOUT, "",    "您当前登录状态已失效，请重新登录",  },
    { WV_ERR_WEB_SUBBOADR_LOADING, "", "当前子板未全部加载成功，请等待子板载完成或拔出未加载子板后，设置网络配置"},
    // the last one
    {  WV_ERR_INVALID_ERR_CODE,       "",                   "", }
};

/*****************************************************************************
  Function:     err_SetLanguage
  Description:  设置错误码对应的语言
  Input:        wvLanguage  enLanguage        (i)   语种
  Output:       none
  Return:       -1           --  设置失败
                0            --  设置成功
  Author:       dadi.zeng
*****************************************************************************/
int err_SetLanguage(wvLanguage enLanguage)
{
    if(enLanguage >= EN_LANGUAGE_MAX)
    {
        return -1; 
    }
    
    g_enLanguage = enLanguage;

    return 0;
}

/*****************************************************************************
  Function:     err_GetErrCodeString
  Description:  通过错误码得到相应的字符串
  Input:        wvErrCode  enErrCode        (i)   错误代码
  Output:       none
  Return:       错误码对应的字符串
  Author:       dadi.zeng
*****************************************************************************/
char * err_GetErrCodeString(wvErrCode enErrCode)
{
    int u32Index = 0;
    char * pu8Ret = NULL;

    /* 找到节点 */
    while(1)
    {
        if(enErrCode == g_stErrArray[u32Index].enErrCode)
        {   
            break;
        }

        if((u32Index >= MAX_ERR_CODE_NUM) || (WV_ERR_INVALID_ERR_CODE == g_stErrArray[u32Index].enErrCode))
        {
            //LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_COMMON, "[%s]%d: Can't find errcode [%d] information!\r\n", __func__, __LINE__, enErrCode);
            return NULL;    
        }
        
        u32Index++;
    }

    /* 找到所对应的语言 */
    if(g_enLanguage >= EN_LANGUAGE_MAX)
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_COMMON, "[%s]%d: Can't support this lanuage [%d]!\r\n", __func__, __LINE__, g_enLanguage);
        return NULL;        
    }

    switch(g_enLanguage)
    {
        case EN_LANGUAGE_EN :
        {
            pu8Ret = g_stErrArray[u32Index].u8ErrEnglishString;
            break;
        }
        case EN_LANGUAGE_CN :
        {
            pu8Ret = g_stErrArray[u32Index].u8ErrChineseString;
            break;
        }
        default:
        {
            break;
        }
    }
    
    return pu8Ret;
}

