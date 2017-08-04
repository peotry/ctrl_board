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
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 7000
#define IPADDR ("0.0.0.0")

int main_2(int argc, char *argv[])
{
    printf("udp server..\n"); 

    struct UnicastInfo stUnicastInfo;
    char arrSndMsg[128] = "Welcome To Server...";
    char arrRecvMsg[128] = {0};
    char arrIp[16] = {0};
    short port = 0;
    struct sockaddr_in client;

    memcpy(stUnicastInfo.arrIp, IPADDR, 16);
    stUnicastInfo.port = PORT;

    UDP_UnicastServerSockfd(&stUnicastInfo);

    while(1)
    {
        UDP_Recvform(stUnicastInfo.sockfd, &(client), arrRecvMsg, sizeof(arrRecvMsg));
        inet_ntop(AF_INET, &(client.sin_addr), arrIp, sizeof(arrIp)); 
        port = ntohs(client.sin_port);
        
        printf("Client[ip: %s port: %u]: %s\n", arrIp, port, arrRecvMsg);

        UDP_Sendto(stUnicastInfo.sockfd, &(client), arrSndMsg, sizeof(arrSndMsg));

    }

    return 0;
}


int main(int argc, char *argv[])
{
    int sockfd = -1;
    struct sockaddr_in servaddr, cliaddr;
    char RecvMsg[1024] = {0};
    char SndMsg[1024] = "Welcome To Server1";
    int len = 0;
    int n = 0;
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    bind(sockfd, (struct sockaddr *)(&servaddr), sizeof(struct sockaddr));

    while(1)
    {
        n = recvfrom(sockfd, RecvMsg, sizeof(RecvMsg), 0, (struct sockaddr *)(&cliaddr), (void *)(&len));
        printf("Server: %s\n", RecvMsg);
        sendto(sockfd, RecvMsg, n, 0, (struct sockaddr *)(&cliaddr), len);
    }


    return 0;
}


