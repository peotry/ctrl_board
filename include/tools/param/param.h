/*
 * =====================================================================================
 *
 *       Filename:  param.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/13/17 09:43:16
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  linsheng.pan (), life_is_legend@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef INCLUDE_TOOLS_PARAM_H
#define INCLUDE_TOOLS_PARAM_H 

#include "err/wv_err.h"

wvErrCode Param_ReadFile(const char *filename, char *buf, const int buf_len);
wvErrCode Param_WriteFile(const char *filename, const char *buf, const int buf_len);

#endif /* ifndef INCLUDE_TOOLS_PARAM_H */
