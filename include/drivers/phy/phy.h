/**********************************************************************
* Copyright(c), 2017 WELLAV Technology Co.,Ltd.
* All rights reserved.
*
* FileName:     IPconfig.h
* Description:  IP manage and configure
* Author    :   ruibin.zhang
* Modified  :
* Reviewer  :
* Date      :   2017-03-04
* Record    :
*
**********************************************************************/

#ifndef INCLUDE_DRIVERS_PHY_H
#define INCLUDE_DRIVERS_PHY_H

#include "appGlobal.h"
#include "err/wv_err.h"

#define ETH0_BASE_ADDR             (0xff700000)
#define ETH1_BASE_ADDR             (0xff702000)
#define ETH2_BASE_ADDR             (0xff200000)
#define ETH3_BASE_ADDR             (0xff200800)

#define ETH_NUM                     (4)

#define MDIO_BASE 0x11020000  
#define MDIO_DATA 0X0014  
#define MDIO_ADDR 0X0010  
#define MII_BUSY  0x00000001  
#define MII_WRITE 0x00000002  
#define MII_PHY   0x04  

#define READ(_a)     ( *((U32*)(_a)) )  
#define WRITE(_a, _v)   (*(volatile U32 *)(_a) = (_v))  

enum _EthType
{
    ETH_TYPE_0,
    ETH_TYPE_1,
    ETH_TYPE_2,
    ETH_TYPE_3
};

typedef enum _EthType EthType;

void Phy_MapRegAll(void);
wvErrCode Phy_MapReg(EthType emEthType);
wvErrCode Phy_WriteReg(EthType emEthType, U32 page, U32 addr, U32 value);
wvErrCode Phy_ReadReg(EthType emEthType, U32 page, U32 addr, U32 *value);
void Phy_InitAll(void);
void Phy_Init(EthType emEthType);

#endif /* IP_CONFIG_H */
