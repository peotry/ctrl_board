#include "event/event.h"

#include "log/wv_log.h"

#include <errno.h>
#include <unistd.h>
#include <sys/eventfd.h>

S32 Event_CreateFd(void)
{
	S32 fd = eventfd(0, 0);

	return fd;
}


S32 Event_Read(S32 fd)
{
	S32 ret = 0;
	S32 num = 0;
    S32 timeout = 5;
	U64 u64Val = 0;

again:
	num = read(fd, &u64Val, sizeof(u64Val));
    if((num < 0) && (EINTR == errno))
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_TOOLS, "Error: read fail by EINTR");
        if(--timeout < 0)
        {
            ret = EVENT_ERR;
        }
        else
        {
            goto again;
        }
    }
    else if((num <= 0) && ( EINTR != errno))
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_TOOLS, "Error: read faild!");
        ret = EVENT_ERR;
    }
    else
    {
        ret = EVENT_SUCCESS;
    }

	return ret;
}


S32 Event_Write(S32 fd)
{
	S32 ret = 0;
	S32 num = 0;
	U64 u64Val = 1;

	num = write(fd, &u64Val, sizeof(u64Val));
	if(num != sizeof(u64Val))
	{
		LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_TOOLS, "Error: call write failed!");
		ret = EVENT_ERR;
	}

	return ret;
}



