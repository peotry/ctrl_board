/*
 * =====================================================================================
 *
 *       Filename:  test_udp_server.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/06/17 11:07:58
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  linsheng.pan (), life_is_legend@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "socket/udp/udp.h"

#include "err/wv_err.h"

#include <stdio.h>
#include <unistd.h>

#define PORT 6000

int main(int argc, char *argv[])
{
    printf("udp client..\n"); 

    struct BroatcastInfo stBroatcastInfo;

    stBroatcastInfo.port = PORT;
    char arrSndMsg[] = "udp client msg!\n";

    UDP_BroatcastSockfd(&stBroatcastInfo);

    while(1)
    {
        UDP_Sendto(stBroatcastInfo.sockfd, &(stBroatcastInfo.addr), arrSndMsg, strlen(arrSndMsg));

        sleep(3);

    }

    return 0;
}
