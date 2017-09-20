/*
 * =====================================================================================
 *
 *       Filename:  crc.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/04/17 12:00:22
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  linsheng.pan (), life_is_legend@163.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "CRC/crc.h"

#include "log/wv_log.h"

/*
* function: Common_CRCCalculate
*
* description: Calculate CRC Value
*
* input:  @data: 需要校验的数据句柄
*         @length: 数据长度
*         @CRCValue: 生成多项式子 目前0x04C11DB7 校验效果最好, 效率最高
*
* output: @
*
* return:  success -  crc value
*          fail    -  0
*
* author: linsheng.pan
*/
U32 Common_CRCCalculate(U8 * data, U32 length, U32 CRCValue)
{
    if(NULL == data)
    {
        LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_TOOLS, "Param is NULL!");
        return 0;
    }

    U32 temp = 0;
    U32 byte_count=0;
    U32 bit_count=0;
    U32 crc = 0;
    U8  tempcrc,tempdata;
    U32 crc_ploy=0x04c11db7;

    if(CRCValue==0)
        crc=0xffffffff;
    else
        crc=CRCValue;
    while(byte_count<length)
    {
        tempcrc=(unsigned char) (crc>>31);
        temp=(7-bit_count);
        bit_count++;
        tempdata=(*data>>temp)&0x1;
        if ((tempdata^tempcrc)!=0)
            crc=(crc<<1^crc_ploy);
        else
            crc=crc<<1;
        if (bit_count>7)
        {
            bit_count=0;
            byte_count++;
            data++;
        }
    }

    return crc;
}

