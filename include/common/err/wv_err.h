/*****************************************************************************
* Copyright (c) 2017, WELLAV Technology Co.,Ltd.
* All rights reserved.
*
* FileName WV_err.h
* Description : 返回错误string 模块
* Author    : ruibin.zhang
* Modified  :
* Reviewer  :
* Date      : 2017-02-28
* Record    :
*
*****************************************************************************/
#ifndef INCLUDE_COMMON_WV_ERR_H
#define INCLUDE_COMMON_WV_ERR_H


#include <errno.h>
#include <string.h>

#define ERR_BUF_LEN         (256)
#define MAX_ERR_INFO_LEN    (150)
#define MAX_LANGUAGE_NUM    (5)
#define MAX_ERR_CODE_NUM    (1024)

#define ERR_STRING(err_buf)    strerror_r(errno, err_buf, sizeof(err_buf))

/* language menu */
typedef enum{
    EN_LANGUAGE_EN,     /* 英文 */
    EN_LANGUAGE_CN,     /* 中文 */
    EN_LANGUAGE_MAX     /* 标记 */
}wvLanguage;


/*  error code */
typedef enum{
    WV_SUCCESS                      = 0,
    WV_ERR_FAILURE                  = 0x1,
    WV_ERR_CONFIG_TIMEOUOT          = 0x2,
    WV_ERR_VER                      = 0x3,
    WV_ERR_PARAMS                   = 0x4,
    WV_ERR_CRC                      = 0x5,
    WV_ERR_FILE_NOT_EXISTING        = 0x6,      // 文件不存在
    WV_ERR_FILE_CAN_NOT_READE       = 0x7,      // 文件不可读
    WV_ERR_MALLOC_FALIURE           = 0x8,
    WV_ERR_UPGRADE_CHECK_HEADER     = 0x9,      // 升级文件头部校验失败
    WV_ERR_UPGRADE_CHECK_NET_HEADER = 0xa,      // 校验网络同步失败
    WV_ERR_NO_DNS                   = 0xb,      // 没有DNS信息
    WV_ERR_NO_NETCARD_INFO          = 0xc,      // 没有网卡信息
    WV_ERR_PARAMS_BY_MODIFY         = 0xd,      // 参数已经被修改过
    WV_ERR_REALTIME_STREAM_BANORMAL = 0xe,      // 实时码流异常
    WV_ERR_PARAMS_MIGRATION         = 0xf,      // 数据需要迁移
    WV_ERR_HARDWAREVER              = 0x10,     // 硬件版本不匹配
    WV_ERR_TEMPERATURE              = 0x11,     // 设备温度过高
    WV_ERR_TAR_UPGRADE_FILE_ERROR   = 0x12,     // 解压升级文件失败
    WV_ERR_LIC_EXPIRED              = 0x13,     // 授权过期
    WV_ERR_CMD_POPEN                = 0x14,     // 管道命令打开失败
    WV_ERR_PONIT_OUT_RANGE          = 0x15,     // 指针超过范围
    WV_ERR_FILE_CREAT               = 0x16,     // 文件创建失败
    WV_ERR_HWDES_NOT_FOUND          = 0x17,     // 找不到硬件版本信息的描述
    WV_ERR_PARAMS_FILE_WRITE        = 0x18,     // 参数文件写入错误
    WV_ERR_PARAMS_FILE_TAR          = 0x19,     // 参数文件解压错误

    //comm
    WV_ERR_COMM_PTHREAD_CREAT       = 0x202,    //线程创建失败
    WV_ERR_COMM_UART_RECV_MISS      = 0x203,    //串口接收数据过少
    WV_ERR_COMM_UART_RECV_HEADER    = 0x204,    //串口接收数据头错误
    WV_ERR_COMM_UART_RECV_SLOT      = 0x205,    //串口接收数据槽号错误
    WV_ERR_COMM_IP_RECV_MISS        = 0x206,    //IP接收数据过少
    WV_ERR_COMM_IP_TEMP_FULL        = 0x207,    //IP临时数据满了
    WV_ERR_COMM_IP_TIME_OUT         = 0x208,    //IP通信尝试次数超过
    WV_ERR_COMM_SOCKETC_OPEN        = 0x209,
    WV_ERR_COMM_SOCKETC_BIND        = 0x20a,
    WV_ERR_COMM_SOCKETC_SEND        = 0x20b,
    WV_ERR_COMM_NTP_OPEN            = 0x20c,    //socket open error
    WV_ERR_COMM_NTP_BIND            = 0x20d,    //socket bind error
    WV_ERR_COMM_NTP_SEND            = 0x20e,    //Send to NTP 请求失败
    WV_ERR_COMM_NTP_RECV            = 0x20f,    //Receive from NTP 失败
    WV_ERR_COMM_NTP_CONN            = 0x210,    //Receive from NTP 失败

    //web   
    WV_ERR_WEB_NOTEXIST             = 0x302,    //不存在
    WV_ERR_WEB_MALLOC               = 0x303,    //开辟内存失败
    WV_ERR_WEB_GETPE                = 0x304,    //获取PE参数失败
    WV_ERR_WEB_SETPE                = 0x305,    //设置PE参数失败
    WV_ERR_WEB_POST_PARAM           = 0x306,    //post的参数有误
    WV_ERR_WEB_BAD_PARAM            = 0x307,    //入参错误
    WV_ERR_WEB_TARFAIL              = 0x308,    //打包失败
    WV_ERR_WEB_DIRCREATFAIL         = 0x309,    //创建文件夹失败
    WV_ERR_WEB_SUBBOADR_LOADING     = 0x30a,    //正在加载子板，无法设置网络参数


    //user
    WV_ERR_USER_LEN_ERROR           = 0x402,    //用户名密码，长度错误
    WV_ERR_USER_EXIST               = 0x403,    //用户存在
    WV_ERR_USER_NOT_EXIST           = 0x404,    //用户不存在
    WV_ERR_USER_FULL                = 0x405,    //用户满了
    WV_ERR_USER_OPEN_FILE_ERROR     = 0x406,    //打开文件错误
    WV_ERR_USER_PWD_ERR             = 0x407,    //用户密码错误
    WV_ERR_USER_TOKEN_ERR           = 0x408,    //Token错误
    WV_ERR_USER_TOKEN_TIMEOUT       = 0x409,    //Token已失效
    WV_ERR_USER_WAS_LOGIN           = 0x40a,    //用户已登录

    //license

    //resource
    WV_ERR_RESOURCE_DATA_LEN        = 0x501,    //资源信息过短
    WV_ERR_RESOURCE_DATA_CHECK      = 0x502,    //资源信息错误

    //GPIO
    WV_ERR_PIN_PTHREAD_CREAT        = 0x601,    //线程创建失败

    //net
    WV_ERR_SOCKET_OPEN             = 0x701,
    WV_ERR_SOCKET_BIND             = 0x702,
    WV_ERR_SOCKET_SEND             = 0x703,
    WV_ERR_SOCKET_RECV             = 0x704,
    WV_ERR_SOCKET_SETOPT           = 0x705,
    WV_ERR_SOCKET_IPADDR           = 0x706,

    // to add
    //
    
    //invalid error code
    WV_ERR_INVALID_ERR_CODE        = 0xFFFFFFFF,
}wvErrCode;


typedef struct
{
    wvErrCode   enErrCode;
    char        u8ErrEnglishString[MAX_ERR_INFO_LEN];
    char        u8ErrChineseString[MAX_ERR_INFO_LEN];
} ST_PE_ERR;

int err_SetLanguage(wvLanguage enLanguage);
char * err_GetErrCodeString(wvErrCode enErrCode);

#endif






