/*
 * =====================================================================================
 *
 *       Filename:  mutex.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/11/17 09:13:08
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <pthread.h>

#include "mutex/mutex.h"
#include "log/log.h"

static pthread_mutex_t s_mutex;

wvErrCode Mutex_Init(void)
{
	int ret = 0;
    char err_buf[MAX_ERR_INFO_LEN] = {0};

	ret = pthread_mutex_init(&s_mutex, NULL);

	if(0 != ret)
	{
        ERR_STRING(err_buf);
		LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_TOOLS, "Error: %s", err_buf);

		return WV_ERR_FAILURE;
	}

	return WV_SUCCESS;
}


wvErrCode Mutex_Lock(void)
{
	int ret = 0;
    char err_buf[MAX_ERR_INFO_LEN] = {0};

	ret = pthread_mutex_lock(&s_mutex);

	if(0 != ret)
	{
        ERR_STRING(err_buf);
		LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_TOOLS, "Error: %s", err_buf);

		return WV_ERR_FAILURE;
	}

	return WV_SUCCESS;
}


wvErrCode Mutex_Unlock(void)
{
	int ret = 0;
    char err_buf[MAX_ERR_INFO_LEN] = {0};

	ret = pthread_mutex_unlock(&s_mutex);

	if(0 != ret)
	{
        ERR_STRING(err_buf);
		LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_TOOLS, "Error: %s", err_buf); 

		return WV_ERR_FAILURE;
	}

	return WV_SUCCESS;
}


wvErrCode Mutex_Destroy(void)
{
	int ret = 0;
    char err_buf[MAX_ERR_INFO_LEN] = {0};

	ret = pthread_mutex_destroy(&s_mutex);

    if(0 != ret)
    {
        ERR_STRING(err_buf);
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_TOOLS, "Error: %s", err_buf); 

        return WV_ERR_FAILURE;
    }

	return WV_SUCCESS;

}
