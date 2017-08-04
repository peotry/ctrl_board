/*
 * =====================================================================================
 *
 *       Filename:  timer_test.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/05/17 13:34:20
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  linsheng.pan (), life_is_legend@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>

#include "log/log.h"
#include "timer/timer.h"
#include "thread/thread.h"

void * timer_task1(void *arg)
{
    printf("[Task]timer task1 ...\n");

    return NULL;
}


void * timer_task2(void *arg)
{
    printf("[Task]timer task2 ...\n");

    return NULL;
}


int main(int argc, char *argv[])
{
    printf("timer test ...\n");

    Log_Init("/home/mark/log.txt", 4);

    Timer_InitTaskList();

    THREAD_NEW_DETACH(Timer_MonitorTask, NULL, "Timer_MonitorTask");

    struct itimerspec itimer;
    struct TimerTask stTimerTask;
    struct TimerTask stTimerTask2;

    itimer.it_value.tv_sec = 1;
    itimer.it_value.tv_nsec = 0;
    itimer.it_interval.tv_sec = 2;
    itimer.it_interval.tv_nsec = 0;

    stTimerTask.func = timer_task1;
    stTimerTask.arg = NULL;
    stTimerTask2.func = timer_task2;
    stTimerTask2.arg = NULL;

    sleep(2);

    Timer_RegisterTask(itimer, &stTimerTask);
    Timer_RegisterTask(itimer, &stTimerTask2);

    sleep(5);

    Timer_RemoveTask(&stTimerTask);

    sleep(5);

    Timer_RegisterTask(itimer, &stTimerTask);
    
    while(1)
    {
        sleep(5);
    }

    return 0;
}
