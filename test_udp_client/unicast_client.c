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
#include "log/log.h"

#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 7000
#define IPADDR ("127.0.0.1")

int main_0(int argc, char *argv[])
{
    printf("udp server..\n"); 

    Log_Init("/home/mark/log.txt", 40);

    struct UnicastInfo stUnicastInfo;
    char arrSndMsg[128] = "I am client...";
    char arrRecvMsg[128] = {0};
    struct sockaddr_in peer;

    memcpy(stUnicastInfo.arrIp, IPADDR, 16);
    stUnicastInfo.port = PORT;

    UDP_UnicastClientSockfd(&stUnicastInfo);

    while(1)
    {
        sleep(3);
        UDP_Sendto(stUnicastInfo.sockfd, &(stUnicastInfo.addr), arrSndMsg, sizeof(arrSndMsg));
        printf("Send to Server: %s\n", arrSndMsg);
        UDP_Recvform(stUnicastInfo.sockfd, &(peer), arrRecvMsg, sizeof(arrRecvMsg));

        printf("Server Echo: %s\n", arrRecvMsg);

    }

    return 0;
}

int main(int argc, char *argv[])
{
    struct sockaddr_in servaddr, client;
    int sockfd = -1;
    int len = 0;
    int n = 0;
    char SndMsg[1024] = "I am client!";
    char RecvMsg[1024] = {0};

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    inet_pton(AF_INET, IPADDR, &servaddr.sin_addr.s_addr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    while(1)
    {
        printf("Client: %s\n", SndMsg);
        sendto(sockfd, SndMsg, strlen(SndMsg), 0 , (struct sockaddr *)(&servaddr), sizeof(servaddr));
        recvfrom(sockfd, RecvMsg, sizeof(RecvMsg), 0, (struct sockaddr *)(&client), &len);
        printf("Server: %s\n", RecvMsg);
        sleep(2);

    }

    return 0;
}
