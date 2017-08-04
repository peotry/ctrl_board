/*
 * =====================================================================================
 *
 *       Filename:  crc.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/04/17 11:57:45
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  linsheng.pan (), life_is_legend@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef INCLUDE_COMMON_CRC_H
#define INCLUDE_COMMON_CRC_H 

#include "common/appGlobal.h"

U32 Common_CRCCalculate(U8* data, U32 length, U32 CRCValue);

#endif /* ifndef INCLUDE_COMMON_CRC_H */
