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

#include "fpga/FPGA.h"
#include "log/log.h"
#include "phy/phy.h"
#include "psconf/data_hex_array.h"
#include "psconf/psconf.h"
#include "timer/timer.h"

int main(int argc, char *argv[])
{
    printf("hello world\n");

    Log_Init("/tmp/log.txt", 4000);

    FPGA_Init();

    PHY_Init();

    //将FPGA配置CI芯片的bit文件，加载到CI芯片
    PSConf_FWConfigGeneralFPGA32Bit(data_array, sizeof(data_array) -1, XILINX_PLATFORM);    

    return 0;
}
