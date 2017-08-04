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

#define PORT 6000

int main(int argc, char *argv[])
{
    printf("udp server..\n"); 

    struct BroatcastInfo stBroatcastInfo;

    stBroatcastInfo.port = PORT;
    char arrRecvMsg[128] = {0};

    UDP_BroatcastSockfd(&stBroatcastInfo);

    while(1)
    {
        UDP_Recvform(stBroatcastInfo.sockfd, &(stBroatcastInfo.addr), arrRecvMsg, sizeof(arrRecvMsg));

        printf("Client: %s\n", arrRecvMsg);

        //if(WV_SUCCESS == UDP_BroatcastSend(&stBroatcastInfo, arrSndMsg, strlen(arrSndMsg)))
        //{
        //    printf("udp server send ok!\n");
        //}
    }

    return 0;
}
