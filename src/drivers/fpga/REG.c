/*
* Copyright (c) 2017,HuiZhou WELLAV Technology Co.,Ltd.
* All rights reserved.
*
* FileName REG.c
* Description : logic_CPU_reg moudle
* Author    : ruibin.zhang
* Modified  :
* Reviewer  :
* Date      : 2017-02-20
* Record    :
*
 */

#include "REG.h"
#include "log/wv_log.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/mman.h>
#include <fcntl.h>

static void             *s_pLogicAddress;
pthread_mutex_t         g_reg_mutex;
bool                    g_reg_mapped = false;

/*****************************************************************************
  Function:     Xil_In32
  Description:  Performs an input operation for a 32-bit memory location by
                reading from the specified address and returning the Value read
                from that address.
  Input:        Addr contains the address to perform the input operation
  Output:       none
  Return:       The Value read from the specified input address.
*****************************************************************************/
U32 Xil_In32(U32 Addr)
{
    return *(volatile U32 *) Addr;
}

/*****************************************************************************
  Function:     Xil_Out32
  Description:  Performs an output operation for a 32-bit memory location by
                writing the specified Value to the the specified address.
  Input:        OutAddress contains the address to perform the output operation
                Value contains the Value to be output at the specified address.
  Output:       none
  Return:       none
*****************************************************************************/
void Xil_Out32(U32 OutAddress, U32 Value)
{
    *(volatile U32 *) OutAddress = Value;
}

/*****************************************************************************
  Function:     REG_Lock_Init
  Description:  reg Lock init
  Input:        none
  Output:       none
  Return:       none
  Author:       ruibin.zhang
*****************************************************************************/
void REG_Lock_Init()
{
    pthread_mutex_init(&g_reg_mutex, NULL);
}

/*****************************************************************************
  Function:     REG_Lock
  Description:  Lock wr reg
  Input:        none
  Output:       none
  Return:       none
  Author:       ruibin.zhang
*****************************************************************************/
void REG_Lock()
{
    pthread_mutex_lock(&g_reg_mutex);
}

/*****************************************************************************
  Function:     REG_UnLock
  Description:  unLock wr reg
  Input:        none
  Output:       none
  Return:       none
  Author:       ruibin.zhang
*****************************************************************************/
void REG_UnLock()
{
    pthread_mutex_unlock(&g_reg_mutex);
}

/*****************************************************************************
  Function:     Reg_Map
  Description:  map address
  Input:        none
  Output:       none
  Return:       none
  Author:       liwei
  Modify:       ruibin.zhang
*****************************************************************************/
void REG_Map()
{
    S32 memfd = 0;

    REG_Lock_Init();

    memfd = open("/dev/mem", O_RDWR | O_SYNC);
    if(memfd == -1)
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_DRIVERS, "Error: Can't open /dev/mem.\n");
        return;
    }

    s_pLogicAddress = mmap(0, 0x100000, PROT_READ | PROT_WRITE, MAP_SHARED, memfd, (__off_t)AXI_CPU_LOGIC_INFO);
    if(s_pLogicAddress == (void *) -1)
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_DRIVERS, "Error: Can't map the memory to user space.\n");
        return;
    }
    else
    {
        g_reg_mapped = true;
        return;
    }

}

/*****************************************************************************
  Function:     Reg_Read
  Description:  read register
  Input:        u32Address     --- address of register
  Output:       pu32Data       --- value of register
  Return:       none
  Author:       liwei
  Modify:       ruibin.zhang
*****************************************************************************/
void REG_Read(U32 u32Address, U32 *pu32Data)
{
    if(false == g_reg_mapped)
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_DRIVERS, "Reg Must Map Success\n");
        return;
    }

    REG_Lock();
    *pu32Data = AXI_CPU_LOGIC_INF_mReadReg((U32)s_pLogicAddress, u32Address<<4);
    //逻辑寄存器读写需有一定延时，5us目前测试正常
    //usleep(5);
    REG_UnLock();
}

/*****************************************************************************
  Function:     Reg_Write
  Description:  write register
  Input:        u32Address      --- address of register
                u32Data         --- value of register
  Output:       none
  Return:       none
  Author:       liwei
*****************************************************************************/
void REG_Write(U32 u32Address, U32 u32Data)
{
    if(false == g_reg_mapped)
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_DRIVERS, "Reg Must Map Success\n");
        return;
    }

    REG_Lock();
    AXI_CPU_LOGIC_INF_mWriteReg((U32)s_pLogicAddress, u32Address<<4, u32Data);//???16??????
    //逻辑寄存器读写需有一定延时，5us目前测试正常
    //usleep(5);
    REG_UnLock();
}
