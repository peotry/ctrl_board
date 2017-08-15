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

#include "ADT7410/ADT7410.h"
#include "ds1338/ds1338.h"
#include "fpga/FPGA.h"
#include "log/log.h"
#include "i2c/i2c.h"
#include "phy/phy.h"
#include "psconf/data_hex_array.h"
#include "psconf/psconf.h"
#include "timer/timer.h"
#include "thread/thread.h"

int main(int argc, char *argv[])
{
    printf("hello world\n");

    Log_Init("/tmp/log.txt", 4000);

    //FPGA_Init();

    Phy_Init(ETH_TYPE_0);
    Phy_Init(ETH_TYPE_1);
    //I2C_InitAll();

    //I2C_WriteRegWithCheck(I2C_DEV_NAME_0, 0x4B, 0x4, 0x28);

    //ADT7410_StartTask();
    //THREAD_NEW_DETACH(ADT7410_Example, NULL, "ADT7410_Example");
    
    //ds1338_example();  
   
   int count = 5;

   while(--count > 0)
   {
       sleep(1);
   }

    return 0;
}
