/**********************************************************************
* Copyright(c), 2017 WELLAV Technology Co.,Ltd.
* All rights reserved.
*
* FileName  :appGlobal.h
* Description : macro define about data type and version;
* Author    : wei.li
* Modified  :
* Reviewer  :
* Date      : 2017-02-17
* Record    :
*
**********************************************************************/
#ifndef INCLUDE_COMMON_APPGLOBAL_H
#define INCLUDE_COMMON_APPGLOBAL_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SOFT_VERSION            0x00000006
#define BB_DESCRIPTOR           "BackBoard"

#define DEV4U_BB_APP            1

#define LIC_FUNC                1
//方便测试人员测试 关掉MAC授权
#if LIC_FUNC
#define MAC_LIC_FUNC            1
#endif

#define WEB_MONI                0
//
#define MAX_INTERFACE_NUM       16          //子板个数

typedef enum{
    SLOT0               =           0X0,
    SLOT1               =           0X1,
    SLOT2               =           0X2,
    SLOT3               =           0X3,
    SLOT4               =           0X4,
    SLOT5               =           0X5,
    SLOT6               =           0X6,
    SLOT7               =           0X7,
    SLOT8               =           0X8,
    SLOT9               =           0X9,
    SLOT10              =           0Xa,
    SLOT11              =           0Xb,
    SLOT12              =           0Xc,
    SLOT13              =           0Xd,
    SLOT14              =           0Xe,
    SLOT15              =           0Xf,
    SLOT_BB             =           0X10,
    MAX_BOARD_NUM       =           0X11
}SLOT_ID;

// 0:CMP-000;1:CMP-001;2:CMP-011;3:CMP-011;
typedef enum{
    CMP_000             =           0X0,
    CMP_001             =           0X1,
    CMP_011             =           0X2,
    CMP_111             =           0X3,
}CMP_MODEL;

#define WELLAV_APP              "/wellav_app/"
#define WELLAV_DATA             "/wellav_data/"
#define PROCESS_NAME            ("control_board")

#define ETH0_IP_BASE_STR        ("192.168.1.100")
#define ETH1_IP_BASE_STR        ("192.168.3.100")
#define ETH1_GAY_WAY            ("192.168.3.1")

#define ETH0_NAME               ("eth0")
#define ETH1_NAME               ("eth1")

/**********************************************************************
 * Data type macro
 *********************************************************************/
typedef unsigned char                   U8;
typedef unsigned short                  U16;
typedef unsigned int                    U32;
typedef unsigned long                   U64;

typedef signed char                     S8;
typedef signed short                    S16;
typedef signed int                      S32;

typedef unsigned char volatile          VU8;
typedef unsigned short volatile         VU16;
typedef unsigned int volatile           VU32;
typedef unsigned long volatile          VU64;

typedef unsigned char                   BYTE;
typedef short                           SHORT;
typedef unsigned short                  WORD;
typedef int                             LONG;
typedef unsigned int                    DWORD;
typedef char                            BOOL;

#define FALSE                           0
#define TRUE                            1

#define MAX_FILENAME_LENGTH     512     //最大文件名长度
#define MAX_COMMAND_LENGTH      1000     //最大命令行长度 比如 "rm xx.ts"
#define MAX_LINE_LENGTH         200      //一行最大长度

#define WEB_LOG(format, ...)    //printf("[%s:%d] " format " \r\n", __FUNCTION__, __LINE__, ## __VA_ARGS__)
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define CLOSE(fd)  {\
    if(fd > 0) \
    {\
        close(fd);\
        fd = -1;\
    }\
}

#endif/*APPGLOBAL_H_*/

