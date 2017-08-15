/*
 * =====================================================================================
 *
 *       Filename:  phy_reg.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/14/17 17:48:07
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  linsheng.pan (), life_is_legend@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>

#include "appGlobal.h"
#include "log/log.h"
#include "phy/phy.h"

void usage(void)
{
    printf("Usage: \n");

    printf("read: phy_reg r eth_num page addr\n");
    printf("write: phy_reg w eth_num page addr val\n");

}

int main(int argc, char *argv[])
{
    if(argc < 5)
    {
        usage();
        return -1;
    }

    Log_Init("/tmp/log.txt", 2000);

    U32 eth_num = strtoul(argv[2], NULL, 16);
    U32 page = strtoul(argv[3], NULL, 16);
    U32 addr = strtoul(argv[4], NULL, 16);
    U32 val = 0;

    Phy_MapReg(ETH_TYPE_0);
    Phy_MapReg(ETH_TYPE_1);
    Phy_MapReg(ETH_TYPE_2);

    if(!strcmp(argv[1], "r"))
    {
        Phy_ReadReg(eth_num, page, addr, &val);
        printf("[read] ===> page: %u, addr: %u, val: 0x%x\n", page, addr, val);
    }
    else if(!strcmp(argv[1], "w"))
    {
        val = strtoul(argv[5], NULL, 16);
        Phy_WriteReg(eth_num, page, addr, val);
        printf("[write] ===> page: %u, addr: %u, val: 0x%x\n", page, addr, val);

        Phy_ReadReg(eth_num, page, addr, &val);
        printf("[read] ===> page: %u, addr: %u, val: 0x%x\n", page, addr, val);

    }
    else
    {
        usage();
    }

    return 0;
}
