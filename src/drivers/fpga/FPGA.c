/**********************************************************************
* Copyright (c) 2017, WELLAV Technology Co.,Ltd.
* All rights reserved.
*
* FileName FPGA.c
* Description : set of other moudles refer to REG moudle
* Author    : ruibin.zhang
* Modified  :
* Reviewer  :
* Date      : 2017-02-20
* Record    :
*
**********************************************************************/
#include "FPGA.h"
#include "REG.h"
#include "log/wv_log.h"
#include "phy/phy.h"

/*****************************************************************************
  Function:     FPGA_Reg_Map
  Description:  map address
  Input:        none
  Output:       none
  Return:       none
  Author:       jie.zhan
  Modify:
*****************************************************************************/
void FPGA_REG_Map()
{
    REG_Map();
}

/*****************************************************************************
  Function:     FPGA_REG_Write
  Description:  FPGA REG Write
  Input:        u32Address      --- address of register
                u32Data         --- value of register
  Output:       none
  Return:       none
  Author:       jie.zhan
  Modify:
*****************************************************************************/
void FPGA_REG_Write(U32 u32Address, U32 u32Data)
{
    REG_Write(u32Address, u32Data);
}

/*****************************************************************************
  Function:     FPGA_REG_Read
  Description:  FPGA REG Read
  Input:        u32Address     --- address of register
  Output:       pu32Data       --- value of register
  Return:       none
  Author:       jie.zhan
  Modify:       
*****************************************************************************/
void FPGA_REG_Read(U32 u32Address, U32 *pu32Data)
{
    REG_Read(u32Address,pu32Data);
}

/*****************************************************************************
  Function:     FPGA_GetSlotID
  Description:  get slot id
  Input:        none
  Output:       none
  Return:       none
  Author:       jie.zhan
  Modify:
*****************************************************************************/
U32 FPGA_GetSlotID(void)
{
    U32 u32Data = 0;
    FPGA_REG_Read(REG_SLOT, &u32Data);

    return u32Data;
}

/*****************************************************************************
  Function:     FPGA_GetLOGVerSion
  Description:  get logic version num
  Input:        none
  Output:       pu32Data       --- value of register
  Return:       none
  Author:       jie.zhan
*****************************************************************************/
void FPGA_GetLOGVerSion(U32 * pu32value)
{
    if(NULL == pu32value)
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_DRIVERS, "Error: Param is NULL");
        return ;
    }

    REG_Read(TEST_VER_NUM, pu32value);
}

/*****************************************************************************
  Function:     FPGA_SetMacAddr
  Description:  FPGA Set Mac Addr
  Input:        u8MacAddr     --- Mac Addr
  				ETHType       --- ETHTYPE
  Output:       none
  Return:       none
  Author:       jie.zhan
  Modify:
*****************************************************************************/
void FPGA_SetMacAddr(U8 *u8MacAddr, U8 ETHType)
{
	if(NULL == u8MacAddr)
	{
		LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_DRIVERS, "Input Para Error...\n");
		return ;
	}
	
    U32 u32MacAddrH = 0;
    U32 u32MacAddrL = 0;

    u32MacAddrH = (u8MacAddr[0] << 16)|(u8MacAddr[1] << 8)|(u8MacAddr[2]);
    u32MacAddrL = (u8MacAddr[3] << 16)|(u8MacAddr[4] << 8)|(u8MacAddr[5]);
	
	LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_DRIVERS, "ETHY = %d u32MacAddrH = 0x%x u32MacAddrL = 0x%x\n", ETHType, u32MacAddrH, u32MacAddrL);

	if(ETH_TYPE_0 == ETHType)
	{
//		FPGA_REG_Write(MAC0REG_HIGHT_ADDR, u32MacAddrH);
		usleep(20000);
//		FPGA_REG_Write(MAC0REG_LOWER_ADDR, u32MacAddrL);
	}
	else
	{
	//	FPGA_REG_Write(MAC1REG_HIGHT_ADDR, u32MacAddrH);
		usleep(20000);
//		FPGA_REG_Write(MAC1REG_LOWER_ADDR, u32MacAddrL);
	}

}

/*****************************************************************************
  Function:     FPGA_Init
  Description:  init FPGA module
  Input:        none
  Output:       none
  Return:       none
  Author:       jie.zhan
  Modify:
*****************************************************************************/
void FPGA_Init(void)
{
    FPGA_REG_Map();
}


