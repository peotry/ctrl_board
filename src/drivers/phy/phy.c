/**********************************************************************
* Copyright(c), 2017 WELLAV Technology Co.,Ltd.
* All rights reserved.
*
* FileName:     IPconfig.c
* Description:  IP manage and configure
* Author    :   ruibin.zhang
* Modified  :
* Reviewer  :
* Date      :   2017-03-04
* Record    :
*
**********************************************************************/

#include "phy/phy.h"

#include "log/log.h"

#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/msg.h>

struct EthInfo
{
    EthType emEthType;
    void *p_PhyAddr;
    void *p_MapAddr;
};

static struct EthInfo s_stEthInfo[ETH_NUM];


/*
* function: Phy_InitPhyAddr
*
* description: 初始化Phy 物理地址
*
* input:  @
*
* output: @
*
* return@ 
* success: 
*    fail: 
*
* author: linsheng.pan
*/
static void Phy_InitPhyAddr(void)
{
    s_stEthInfo[0].emEthType = ETH_TYPE_0;
    s_stEthInfo[0].p_PhyAddr = ETH0_BASE_ADDR;

    s_stEthInfo[1].emEthType = ETH_TYPE_1;
    s_stEthInfo[1].p_PhyAddr = ETH1_BASE_ADDR;

    s_stEthInfo[2].emEthType = ETH_TYPE_2;
    s_stEthInfo[2].p_PhyAddr = ETH2_BASE_ADDR;

    s_stEthInfo[3].emEthType = ETH_TYPE_3;
    s_stEthInfo[3].p_PhyAddr = ETH3_BASE_ADDR;
}


/*
* function: Phy_MapReg
*
* description:  寄存器映射
*
* input:  @
*
* output: @
*
* return@ 
* success: 
*    fail: 
*
* author: linsheng.pan
*/
static wvErrCode Phy_MapReg(EthType emEthType)
{
    int mem_fd = 0;
    int mem_len = 0x1fff;
    int i = 0;
    int j = 0;

    Phy_InitPhyAddr();

    mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
    if(-1 == mem_fd)
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_DRIVERS, "Error: Can't open /dev/mem");

        return WV_ERR_FAILURE;
    }

    s_stEthInfo[emEthType].p_MapAddr = mmap(0, mem_len, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, s_stEthInfo[i].p_PhyAddr);

    if((void *)(-1) == s_stEthInfo[i].p_MapAddr)
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_DRIVERS, "Error: Can't open /dev/mem.");
        CLOSE(mem_fd);

        return WV_ERR_FAILURE;
    }

    return WV_SUCCESS;

}



/*
* function: Phy_MapRegAll
*
* description:  寄存器映射
*
* input:  @
*
* output: @
*
* return@ 
* success: 
*    fail: 
*
* author: linsheng.pan
*/
static void Phy_MapRegAll(void)
{
    int mem_len = 0x1fff;
    int i = 0;
    int j = 0;

    Phy_InitPhyAddr();

    for(i = 0; i < ETH_NUM; ++i)
    {
        if(WV_SUCCESS != Phy_MapReg(s_stEthInfo[i].emEthType))
        {
            for(j = 0; j < i; ++j)
            {
                munmap(s_stEthInfo[j].p_MapAddr, mem_len);
            }
            return ;
        }
    }

}


/*
* function: Phy_WriteReg
*
* description: 写寄存器,调用此函数之前，确保Phy_MapReg已经被调用
*
* input:  @emEtnType: 网口
*         @page: 页
*         @addr: 页内寄存器地址
*         @val: 值
*
* output: @
*
* return@ 
* success: 
*    fail: 
*
* author: linsheng.pan
*/
wvErrCode Phy_WriteReg(EthType emEthType, U32 page, U32 addr, U32 val)  
{  
    void *p_MapAddr = s_stEthInfo[emEthType].p_MapAddr;
    U16 value  = 0; 
    U32 u32Cnt = 2048;
    value = (((page << 11) & (0x0000F800)) |  
            ((addr << 6) & (0x000007C0))) |  
            MII_WRITE;  
    value |= MII_BUSY; 
    
    while(u32Cnt > 0)
    {
        if(((READ(p_MapAddr + MDIO_ADDR)) & MII_BUSY) == 1)
        {
            usleep(2000);
        }
        else
        {
            break;
        }

        u32Cnt--;
    }   

    /* Set the MII address register to write */  
    WRITE(p_MapAddr + MDIO_DATA, val);  
    WRITE(p_MapAddr + MDIO_ADDR, value);  
  
    /* Wait until any existing MII operation is complete */  
    u32Cnt = 2048;
    while(u32Cnt > 0)
    {
        if(((READ(p_MapAddr + MDIO_ADDR)) & MII_BUSY) == 1)
        {
            usleep(2000);
        }
        else
        {
            break;
        }

        u32Cnt--;
    }

    return WV_SUCCESS;  
}  


/*
* function: Phy_ReadReg
*
* description: 读取寄存器
*
* input:  @emEthType: 网口
*         @page: 页
*         @addr: 页内寄存器地址
*         @val: 寄存器值
*
* output: @
*
* return@ 
* success: 
*    fail: 
*
* author: linsheng.pan
*/
wvErrCode Phy_ReadReg(EthType emEthType, U32 page, U32 addr, U32 *val)  
{     
    S32 data   = -1;  
    U16 value  = 0;  
    U32 u32Cnt = 2048;

    void * p_MapAddr = s_stEthInfo[emEthType].p_MapAddr;
      
    value = (((page << 11) & (0x0000F800)) |  
            ((addr << 6) & (0x000007C0)));  
    value |= MII_BUSY;  
    
    while(u32Cnt > 0)
    {
        if(((READ(p_MapAddr + MDIO_ADDR)) & MII_BUSY) == 1)
        {
            usleep(2000);
        }
        else
        {
            break;
        }

        u32Cnt--;
    }  
    
    /* Set the MII address register to write */  
    WRITE(p_MapAddr + MDIO_ADDR, value); 
    
    /* Wait until any existing MII operation is complete */  
    u32Cnt = 2048;
    while(u32Cnt > 0)
    {
        if(((READ(p_MapAddr + MDIO_ADDR)) & MII_BUSY) == 1)
        {
            usleep(2000);
        }
        else
        {
            break;
        }

        u32Cnt--;
    }
      
    *val = READ(p_MapAddr + MDIO_DATA);  

    return WV_ERR_FAILURE;
}  



/*
* function: Phy_SetEthReg
*
* description: 初始化phy
*
* input:  @
*
* output: @
*
* return@ 
* success: 
*    fail: 
*
* author: linsheng.pan
*/
static void Phy_SetEthReg(EthType emEthType)
{
    U32 u32PhyReg0 = 0;
    U32 u32PhyReg2 = 0;
    U32 u32PhyReg3 = 0;
    U32 u32PhyReg16 = 0;
    U32 u32PhyReg17 = 0;
    U32 u32PhyReg20 = 0;
    U32 u32ChipId = 0;

    Phy_ReadReg(emEthType, 0, 2, &u32PhyReg2);
    Phy_ReadReg(emEthType, 0, 3, &u32PhyReg3);

    u32ChipId = u32PhyReg2;
    u32ChipId = u32ChipId << 6;
    u32ChipId = u32ChipId | (u32PhyReg3 >> 10);

    LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_DRIVERS, "Chip Id: 0x%x", u32ChipId);

    Phy_ReadReg(emEthType, 0, 20, &u32PhyReg20);
    LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_DRIVERS, "Read: [Page: 0] [Reg20: 0x%x]", u32PhyReg20);

    u32PhyReg20 |= 0xfa;
    Phy_WriteReg(emEthType, 0, 20, u32PhyReg20);
    LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_DRIVERS, "Write: [Page: 0] [Reg20: 0x%x]", u32PhyReg20);


    Phy_ReadReg(emEthType, 0, 20, &u32PhyReg20);
    LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_DRIVERS, "Read: [Page: 0] [Reg20: 0x%x]", u32PhyReg20);


    Phy_ReadReg(emEthType, 0, 0, &u32PhyReg0);
    u32PhyReg0 |= 0x1000;

    Phy_WriteReg(emEthType, 0, 0, u32PhyReg0);
    LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_DRIVERS, "Write: [Page: 0] [Reg0: 0x%x]", u32PhyReg0);
    

    Phy_ReadReg(emEthType, 0, 0, &u32PhyReg0);
    LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_DRIVERS, "Read: [Page: 0] [Reg0: 0x%x]", u32PhyReg0);
    
    u32PhyReg0 |= 0x8000;
    Phy_WriteReg(emEthType, 0, 0, u32PhyReg0);

    /*  
    bit 15:14
    11 = Reserved
    10 = 1000Mbps
    01 = 100Mbps
    00 = 10Mbps
    */
    Phy_ReadReg(emEthType, 0, 17, &u32PhyReg17);
    LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_DRIVERS, "Read: [Page: 0] [Reg17: 0x%x]", u32PhyReg17);
    
    Phy_ReadReg(emEthType, 8, 16, &u32PhyReg16);

    if((u32PhyReg17 & 0xC000) == 0x8000)
    {
        //1000M
        LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_DRIVERS , "***** 1000M *****");
        u32PhyReg16 |= 0x40;
        u32PhyReg16 &= ~0x2000;
    }
    else if((u32PhyReg17 & 0xC000) == 0x4000)
    {
        //100M
        LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_DRIVERS, "***** 100M *****");
        u32PhyReg16 |= 0x2000;
        u32PhyReg16 &= ~0x40;
    }
    else
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_DRIVERS, "***** speed mode error *****");
        return ;
    }

    Phy_WriteReg(emEthType, 8, 16, u32PhyReg16);

    Phy_ReadReg(emEthType, 0, 0, &u32PhyReg0);
    LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_DRIVERS, "Read: [Page: 0] [Reg0: 0x%x]", u32PhyReg0);
    
    u32PhyReg0 |= 0x8000;
    Phy_WriteReg(emEthType, 0, 0, u32PhyReg0);

    Phy_ReadReg(emEthType, 0, 2, &u32PhyReg2);
    Phy_ReadReg(emEthType, 0, 3, &u32PhyReg3);

    u32ChipId = u32PhyReg2;
    u32ChipId = u32ChipId << 6;
    u32ChipId = u32ChipId | (u32PhyReg3 >> 10);

    LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_DRIVERS, "Chip Id: 0x%x", u32ChipId);

    return ;
}


/*
* function: Phy_Init
*
* description: 初始化PHY
*
* input:  @
*
* output: @
*
* return@ 
* success: 
*    fail: 
*
* author: linsheng.pan
*/
void PHY_Init(EthType emEthType)
{
    static int count[ETH_NUM] = {0};

    if(0 == count[emEthType]++)
    {
        Phy_MapReg(emEthType);
        
        Phy_SetEthReg(emEthType);
    }
}


/*
* function: Phy_InitAll
*
* description: 初始化phy
*
* input:  @
*
* output: @
*
* return@ 
* success: 
*    fail: 
*
* author: linsheng.pan
*/
void Phy_InitAll(void)
{
    Phy_Init(ETH_TYPE_0);
    Phy_Init(ETH_TYPE_1);
    Phy_Init(ETH_TYPE_2);
    Phy_Init(ETH_TYPE_3);
}
