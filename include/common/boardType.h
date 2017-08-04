/**********************************************************************
* Copyright(c), 2017 WELLAV Technology Co.,Ltd.
* All rights reserved.
*
* FileName ��boardType.h
* Description ��macro define about board type
* Author    : huada.huang
* Modified  :
* Reviewer  :
* Date      : 2017-03-9
* Record    :
*
**********************************************************************/
#ifndef INCLUDE_COMMON_BOARDTYPE_H
#define INCLUDE_COMMON_BOARDTYPE_H

#define CMP_MACHINE_TYPE    0x1a

typedef enum{
    CMP_INVALID             =        0x0,
    CMP_BACKBOARD           =       0x01,

    CMP_RECEIVER_C_S2       =       0x11,

    CMP_QAM_16              =       0x21,

    CMP_ENCODER_SD_CVBS_6   =       0x31,
    CMP_ENCODER_HD_HDMI_4   =       0X32,
}BOARD_TYPE;

typedef enum{
    CMP_FUNCTIONAL          =       0x01,
    CMP_INPUT               =       0x02,
    CMP_OUTPUT              =       0X03
}ICON_BOARD_TYPE;

#endif
