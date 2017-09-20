/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/04/17 10:11:35
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "ADT7410/ADT7410.h"
#include "ds1338/ds1338.h"
#include "fpga/FPGA.h"
#include "log/wv_log.h"
#include "i2c/i2c.h"
#include "phy/phy.h"
#include "psconf/data_hex_array.h"
#include "psconf/psconf.h"
#include "timer/timer.h"
#include "thread/thread.h"
#include "net/net.h"
#include "gpio/PIN.h"

void Main_SetIp(const char * eth_name)
{
    if(!eth_name)
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_INIT, "Error: param = NULL");
        return ;
    }

    U32 u32IPAddr = 0;
    char *ip_str_base = NULL;
    char ip_str_new[16] = {0};

    if(!strcmp(ETH0_NAME, eth_name))
    {
        ip_str_base = ETH0_IP_BASE_STR;
    }
    else
    {
        ip_str_base = ETH1_IP_BASE_STR;
    }

    Net_SetIpAddr(eth_name, ip_str_base, sizeof(ip_str_base));
    Net_GetIpAddr(eth_name, &u32IPAddr);

    u32IPAddr += PIN_GetSlotID();
    u32IPAddr = htonl(u32IPAddr);
    inet_ntop(AF_INET, &u32IPAddr, ip_str_new, sizeof(ip_str_new));
    Net_SetIpAddr(eth_name, ip_str_new, sizeof(ip_str_new));

}

void IP_Set(void)
{
    char ip_str[16] = {0};
    char cmd_str[MAX_COMMAND_LEN] = {0};

    Net_GetIpAddrWithDHCP(ETH1_NAME);

    if(WV_SUCCESS != Net_GetIpAddrStr(ETH1_NAME, ip_str, sizeof(ip_str)))
    {
        Main_SetIp(ETH1_NAME);
        snprintf(cmd_str, sizeof(cmd_str), "route add default gw %s", ETH1_GAY_WAY);
        system(cmd_str);
    }

    Main_SetIp(ETH0_NAME);
}

int main(int argc, char *argv[])
{
    printf("hello world\n");

    Log_Init("/wellav_data/log.txt", 4000);

    Phy_Init(ETH_TYPE_0);

    PIN_GetSlotID();

    IP_Set();

    return 0;
}
