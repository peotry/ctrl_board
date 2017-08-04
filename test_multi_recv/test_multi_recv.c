/*
 * =====================================================================================
 *
 *       Filename:  test_multi_send.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/25/17 09:25:28
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  linsheng.pan (), life_is_legend@163.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "net.h"
#include "log/log.h"
#include "socket/udp/udp.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(int argc, char *argv[])
{
    Log_Init("/home/mark/log.txt", 2000);

    struct MulticastInfo stMulticastInfo;
    int count = 0;
    char recv_buf[1024] = {0};

    stMulticastInfo.port = 4567;

    UDP_MulticastRecvSockfd(&stMulticastInfo);
    UDP_MultiAddGroup(&stMulticastInfo, "227.10.20.30");

    while(1)
    {
        if(WV_SUCCESS != UDP_Recvform(stMulticastInfo.sockfd, &(stMulticastInfo.ip_addr), recv_buf, sizeof(recv_buf)))
        {
            exit(1);
        }
        printf("Recv: %s\n", recv_buf);
        count++;
        if(count > 3)
        {
            UDP_MultiDropGroup(&stMulticastInfo.sockfd, "227.10.20.30");
        }
    }

    return 0;
}


