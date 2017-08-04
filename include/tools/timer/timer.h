/*
 * =====================================================================================
 *
 *       Filename:  timer.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/04/17 19:48:42
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  linsheng.pan (), life_is_legend@163.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef INCLUDE_TOOLS_TIMER_H
#define INCLUDE_TOOLS_TIMER_H 

#include <sys/timerfd.h>
#include <stdbool.h>

typedef void *  (*timer_func_cb)(void *arg);

enum TaskStatus
{
    emRunning,
    emStop,
    emStart
};

struct TimerTask
{
    int tm_fd;            //定时器对象
    enum TaskStatus emTaskStatus;  //任务状态
    timer_func_cb func;   //定时执行函数
    void *arg;            //回调函数的数据
};


void Timer_InitTaskList(void);

void *Timer_MonitorTask(void *arg);

void Timer_RegisterTask(const struct itimerspec itimer, struct TimerTask *pstTimerTask);
void Timer_RemoveTask(const struct TimerTask *pstTimerTask);


#endif /* ifndef INCLUDE_TOOLS_TIMER_H */
