/**********************************************************************
* Copyright (c) 2017,HuiZhou WELLAV Technology Co.,Ltd.
* All rights reserved.
*
* FileName FPGA.h
* Description : set of other moudles refer to REG moudle
* Author    : ruibin.zhang
* Modified  :
* Reviewer  :
* Date      : 2017-02-20
* Record    :
*
**********************************************************************/
#ifndef INCLUDE_DRIVERS_FPGA_H
#define INCLUDE_DRIVERS_FPGA_H

#include "appGlobal.h"
#define FPGA_PARA_ERROR         1
//逻辑寄存器地址

//test moudle addr                  // R/W?     bits    default value
#define TEST_VER_NUM        0x000   // R        32      0x201
#define TEST_DATE           0x001   // R        32      0x17020916
#define TEST_REG1           0x002   // R/W      12      0x0
#define TEST_REG2           0x003   // R/W      32      0xa5a5a5a5

//slot id
#define REG_SLOT            0x10    // R        4

//DS2432 moudle addr                // R/W?     bits    default value
#define DS2432_WR_ADDR      0x030   // R/W      32      0x0
#define DS2432_WR_DATA      0x031   // R/W      8       0x0
#define DS2432_RD_DATA      0x032   // R        8
#define DS2432_RW_BUSY      0x033   // R        1

//FPGA Contrl led addr
#define FPGA_CONTROL_LED    0x12

//Define LED STATUS
typedef enum
{
    LED_ON,
    LED_OFF,
}LED_STATUS;

void FPGA_REG_Map();
void FPGA_REG_Write(U32 u32Address, U32 u32Data);
void FPGA_REG_Read(U32 u32Address, U32 *pu32Data);
void FPGA_Init(void);

#endif
