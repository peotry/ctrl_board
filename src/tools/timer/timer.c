/*
 * =====================================================================================
 *
 *       Filename:  timer.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/04/17 21:17:14
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  linsheng.pan (), life_is_legend@163.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "timer/timer.h"

#include "event/event.h"
#include "log/log.h"
#include "thread/thread.h"

#include <errno.h>
#include <poll.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#define ERR_BUF_LEN           (80)
#define MAX_TASK              (100)
#define OPEN_MAX_FD           (500)
#define NANO_OF_ONE_SECOND    (1000000000)

static int s_EventFd = 0;
static struct TimerTask s_stTimerTask[MAX_TASK];


/*
* function: Timer_InitTaskList
*
* description: 初始化任务列表
*
* input:  @
*
* output: @
*
* return: 
*
* author: linsheng.pan
*/
void Timer_InitTaskList(void)
{
    int i = 0;

    for(i = 0; i < MAX_TASK; ++i)
    {
        s_stTimerTask[i].tm_fd = -1;
        s_stTimerTask[i].emTaskStatus = emStop;
    }
}


/*
* function: Timer_CreateEvent
*
* description: 创建事件通知句柄
*
* input:  @
*
* output: @
*
* return: 
*
* author: linsheng.pan
*/
static void Timer_CreateEvent(void)
{
    s_EventFd = Event_CreateFd();
}


/*
* function: Timer_Notify
*
* description: 事件通知
*
* input:  @
*
* output: @
*
* return: 
*
* author: linsheng.pan
*/
static void Timer_Notify(void)
{
    Event_Write(s_EventFd); 
}


/*
* function: Timer_RegisterTask
*
* description: 注册定时执行任务
*
* input:  @
*
* output: @
*
* return: 
*
* author: linsheng.pan
*/
void Timer_RegisterTask(const struct itimerspec itimer, struct TimerTask *pstTimerTask)
{
    if(!pstTimerTask)
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_TOOLS, "Error: params = NULL");
        return ;
    }

    LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_TOOLS, "Timer_RegisterTask ...");

    int i = 0;
    int tm_fd = 0;
    char err_buf[ERR_BUF_LEN] = {0};
    
    //生成一个定时器对象
    if(-1 == (tm_fd = timerfd_create(CLOCK_MONOTONIC, 0)))
    {
        strerror_r(errno, err_buf, sizeof(err_buf));
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_TOOLS, "error: call timerfd_create: %s", err_buf);
        return ;
    }


#if 0
    //获取当前时间
    if(-1 == clock_gettime(CLOCK_REALTIME, &now))
    {
        strerror_r(errno, err_buf, sizeof(err_buf));
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_TOOLS, "error: call tclock_gettime: %s", err_buf);
        return ;
    }

    //第一次触发定时器是绝对时间
    nanosecond = now.tv_nsec + itimer.it_value.tv_nsec;
    if(nanosecond >= NANO_OF_ONE_SECOND)
    {
        new_itimer.it_value.tv_nsec = nanosecond - NANO_OF_ONE_SECOND;
        new_itimer.it_value.tv_sec = itimer.it_value.tv_nsec + now.tv_sec  + 1;
    }
    else
    {
        new_itimer.it_value.tv_nsec = nanosecond;
        new_itimer.it_value.tv_sec = itimer.it_value.tv_sec + now.tv_sec;
    }
#endif

    if( -1 == timerfd_settime(tm_fd, 0, &itimer, NULL))
    {
        strerror_r(errno, err_buf, sizeof(err_buf));
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_TOOLS, "Error: call timerfd_settime: %s", err_buf);
        return ;
    }

    //将任务推入任务列表
    for(i = 0; i < MAX_TASK; ++i)
    {
        if(-1 == s_stTimerTask[i].tm_fd)
        {
            pstTimerTask->tm_fd = tm_fd;
            pstTimerTask->emTaskStatus = emStart;
            s_stTimerTask[i].tm_fd = tm_fd;
            s_stTimerTask[i].emTaskStatus = emStart;
            s_stTimerTask[i].func = pstTimerTask->func;
            s_stTimerTask[i].arg = pstTimerTask->arg;
            break;
        }
    }

    //任务列表已经满
    if(i == MAX_TASK)
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_TOOLS, "Error: task list is full！");
        close(tm_fd);
    }
    else
    {
        //通知事件监听线程，监听新的对象
        Timer_Notify();
    }
   
}


/*
* function: Timer_ExecuteTask
*
* description: 执行注册的定时任务
*
* input:  @
*
* output: @
*
* return: 
*
* author: linsheng.pan
*/
static void Timer_ExecuteTask(int tm_fd)
{
    int i = 0;

    for(i = 0; i < MAX_TASK; ++i)
    {
        if(tm_fd == s_stTimerTask[i].tm_fd)
        {
            THREAD_NEW_DETACH(s_stTimerTask[i].func, s_stTimerTask[i].arg, "Timer_ExecuteTask");
            break;
        }
    }

}


/*
* function: Timer_HandleTask
*
* description: 处理注册的定时任务
*
* input:  @
*
* output: @
*
* return: 
*
* author: linsheng.pan
*/
static void Timer_HandleTask(struct pollfd * timerfds, int maxfd)
{
   int i = 0; 
   int size = 0;
   uint64_t exp = 0;

   for(i = 1; i <= maxfd; ++i)
   {
        if(timerfds[i].revents & POLLIN)
        {
            size = read(timerfds[i].fd, &exp, sizeof(exp));
            if(size != sizeof(exp))
            {
                LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_DRIVERS, "Error: read faild!");
                continue;
            }

            Timer_ExecuteTask(timerfds[i].fd);
        }
   }
}


/*
* function: Timer_MonitorTask
*
* description: 监测所有定时任务的IO，定时执行注册的任务
*
* input:  @
*
* output: @
*
* return: 
*
* author: linsheng.pan
*/
void *Timer_MonitorTask(void *arg)
{

    LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_TOOLS, "Timer_MonitorTask ...");

    int i = 0;
    int j = 0;
    int maxi = 0;
    int nready = 0;
    uint64_t num = 0;
    struct pollfd timerfds[OPEN_MAX_FD];

    Timer_CreateEvent();

    //添加监听描述符
    timerfds[0].fd = s_EventFd;
    timerfds[0].events = POLLIN;

    //初始化事件监听
    for(i = 1; i < OPEN_MAX_FD; ++i)
    {
        timerfds[i].fd = -1;
    }
    
    maxi = 0;

    while(1)
    {
        nready = poll(timerfds, maxi + 1, -1);
        if((-1 == nready) && (EINTR == errno))
        {
            continue;
        }

        //执行定时任务
        Timer_HandleTask(timerfds, maxi);

        //有新任务注册或撤销任务
        if(timerfds[0].revents & POLLIN)
        {
             read(timerfds[0].fd, &num, sizeof(num));
             //初始化事件监听
             //Timer_InitTaskList();
             for(i = 1; i < OPEN_MAX_FD; ++i)
             {
                 timerfds[i].fd = -1;
             }
             maxi = 0;

             //更新监听任务列表
             for(i = 1, j = 0; j < MAX_TASK; ++j)
             {
                 if(-1 == s_stTimerTask[j].tm_fd)
                 {
                     continue;
                 }
                 timerfds[i].fd = s_stTimerTask[j].tm_fd;
                 timerfds[i].events = POLLIN;
                 i++;
             }

             maxi = i - 1;
             printf("maxi = %d\n", maxi);
        }
    }

}



/*
* function: Timer_RemoveTask
*
* description: 注销任务
*
* input:  @
*
* output: @
*
* return: 
*
* author: linsheng.pan
*/
void Timer_RemoveTask(const struct TimerTask *pstTimerTask)
{
    if(!pstTimerTask)
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_DRIVERS, "Error: param = NULL");
        return;
    }

    int i = 0;
    
    for(i = 0; i < MAX_TASK; ++i)
    {
        if(pstTimerTask->tm_fd == s_stTimerTask[i].tm_fd)
        {
            s_stTimerTask[i].emTaskStatus = emStop;
            close(s_stTimerTask[i].tm_fd);
            s_stTimerTask[i].tm_fd = -1;

            //通知监听线程，取消监听
            Timer_Notify();
            break;
        }
    }

    if(i == MAX_TASK)
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_TOOLS, "Error: can't find task!!");
    }

}

