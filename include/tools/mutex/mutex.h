/*
 * =====================================================================================
 *
 *       Filename:  mutex.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/11/17 09:10:20
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef INCLUDE_TOOLS_MUTEX_H
#define INCLUDE_TOOLS_MUTEX_H

#include "appGlobal.h"
#include "err/wv_err.h"

wvErrCode Mutex_Init(void);
wvErrCode Mutex_Lock(void);
wvErrCode Mutex_Unlock(void);
wvErrCode Mutex_Destroy(void);

#endif
