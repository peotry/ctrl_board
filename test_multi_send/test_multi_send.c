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
    int sockfd = -1;
    char group_ip[16] = "227.10.20.30";
    short port = 4567;
    struct sockaddr_in addr;
    char send_buf[] = "Hello world!"; 
    
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, group_ip, &addr.sin_addr.s_addr);
    addr.sin_port = htons(port);

    UDP_CreateSendSockfd(&sockfd);

    while(1)
    {
        UDP_Sendto(sockfd, &addr,send_buf, sizeof(send_buf));
        sleep(1);
    }

    return 0;
}


