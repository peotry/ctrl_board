/*
 * =====================================================================================
 *
 *       Filename:  crc.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/13/17 11:28:50
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  linsheng.pan (), life_is_legend@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef INCLUDE_TOOLS_CRC_H
#define INCLUDE_TOOLS_CRC_H 

#include "appGlobal.h"

#define CRC32_POLYNOMIAL    (0x04C11DB7)

U32 CRC_Calculate(U8 *data, U32 length, U32 u32CRCValue);

#endif /* ifndef INCLUDE_TOOLS_CRC_H */





