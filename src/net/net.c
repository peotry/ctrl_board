/*
 * =====================================================================================
 *
 *       Filename:  net.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/22/17 16:32:49
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  linsheng.pan (), life_is_legend@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "net.h"

#include "appGlobal.h"
#include "log/log.h"
#include "mutex/mutex.h"

#include <arpa/inet.h>
#include <sys/ioctl.h>

#define MAX_NETWORK_NUM         (16)

static struct ifconf s_stIfconf;
static struct ifreq s_st_arrIfreq[MAX_NETWORK_NUM];


static int Net_FindInterface(const char *eth_name)
{
    if(!eth_name)
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_NET, "Error: param = NULL");
        return -1;
    }

    int index = -1;
    int interface_num = 0;
    interface_num = s_stIfconf.ifc_len / sizeof(struct ifreq);

    while(interface_num-- > 0)
    {
        if(!strcmp(s_st_arrIfreq[interface_num].ifr_name, eth_name))
        {
            index = interface_num;
            break;
        }
    }

    return index;
}

/*
* function: Net_GetInterfaceInfo
*
* description: 获取所有工作网卡的信息(网)
*
* input:  @
*
* output: @
*
* return: 
*
* author: linsheng.pan
*/
static wvErrCode Net_GetInterfaceInfo(void)
{
    char err_buf[ERR_BUF_LEN] = {0};
    int sockfd = -1;
    int interface_num = 0;

    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        ERR_STRING(err_buf);
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_NET, "Error: %s", err_buf);

        return WV_ERR_SOCKET_OPEN;
    }

    s_stIfconf.ifc_len = sizeof(s_st_arrIfreq);
    s_stIfconf.ifc_buf = (caddr_t)(s_st_arrIfreq);

    if(!ioctl(sockfd, SIOCGIFCONF, &s_stIfconf))
    {
        interface_num = s_stIfconf.ifc_len / sizeof(struct ifreq);
        LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_NET, "Network interface : %d", interface_num);

        while(interface_num-- > 0)
        {
            //get mac ip broatcast subnet mask
            if(!ioctl(sockfd, SIOCGIFFLAGS | SIOCGIFHWADDR | SIOCGIFADDR | SIOCGIFBRDADDR | SIOCGIFNETMASK, &s_st_arrIfreq[interface_num]))
            {
                LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_NET, "Get Network[%d] Info Success!", interface_num);
            }
            else
            {
                ERR_STRING(err_buf);
                LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_NET, "Error: %s", err_buf);

                CLOSE(sockfd);
                return WV_ERR_SOCKET_SETOPT;
            }
        }
    }
    else
    {
        ERR_STRING(err_buf);
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_NET, "Error: %s", err_buf);

        CLOSE(sockfd);
        return WV_ERR_SOCKET_SETOPT;

    }

    return WV_SUCCESS;
}

/*
* function: 获取本机指定网卡的IP
*
* description: Net_GetIpAddr
*
* input:  @eth_name: 网卡的名字
*         @ip_buf: ip的字符串
*         @buf_size: 字符串的长度
*
* output: @
*
* return: 
*
* author: linsheng.pan
*/
wvErrCode Net_GetIpAddr(const char *eth_name, char *ip_buf, int buf_size)
{
    if((!eth_name) || (!ip_buf))
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_NET, "Error: param = NULL");
        return WV_ERR_PARAMS;
    }
    
    int index = -1;

    //锁住资源
    Mutex_Lock();
    //获取本地网卡信息
    Net_GetInterfaceInfo();
    //判断是否有指定网口的ip
    if((index = Net_FindInterface(eth_name)) >= 0)
    {
        //将ip的网络序转换为点十制
        inet_ntop(AF_INET, &(((struct sockaddr_in *)&(s_st_arrIfreq[index].ifr_addr))->sin_addr.s_addr), ip_buf, buf_size);
        LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_NET, "%s : %s", eth_name, ip_buf);
    }
    else
    {
        //解锁资源并推出
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_NET, "Error: Can't find %s", eth_name);
        Mutex_Unlock();
        return WV_ERR_FAILURE;
    }
    //解锁资源
    Mutex_Unlock();

    return WV_SUCCESS;
}


/*
* function: Net_GetMacAddr
*
* description: 获取本机指定网卡的MAC地址
*
* input:  @eth_name: 网卡名字
*         @mac_buf: 存储mac地址
*         @buf_size: mac地址的长度
*
* output: @
*
* return: 
*
* author: linsheng.pan
*/
wvErrCode Net_GetMacAddr(const char *eth_name, unsigned char *mac_buf, int buf_size)
{
    if((!eth_name) || (!mac_buf))
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_NET, "Error: param = NULL");
        return WV_ERR_PARAMS;
    }

    int index = -1;

    //锁住资源
    Mutex_Lock();
    //获取本地网卡信息
    Net_GetInterfaceInfo();
    //判断是否有指定网卡
    if((index = Net_FindInterface(eth_name)) >= 0)
    {
        memcpy(mac_buf, s_st_arrIfreq[index].ifr_hwaddr.sa_data, buf_size);
    }
    else
    {
        //解锁资源并推出
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_NET, "Error: Can't find %s", eth_name);
        Mutex_Unlock();
        return WV_ERR_FAILURE;
    }
    //解锁资源
    Mutex_Unlock();

    return WV_SUCCESS;

}


/*
* function: Net_SetIpAddr
*
* description: 设置网卡的IP
*
* input:  @eth_name:网卡名
*         @ip_buf:ip地址("192.168.1.1")
*         @buf_size: ip地址的长度
*
* output: @
*
* return: 
*
* author: linsheng.pan
*/
wvErrCode Net_SetIpAddr(const char *eth_name, const char *ip_buf, int buf_size)
{
    if((!eth_name) || (!ip_buf))
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_NET, "Error: param = NULL");
        return WV_ERR_PARAMS;
    }

    char cmd_str[MAX_LINE_LENGTH] = {0};

    snprintf(cmd_str, sizeof(cmd_str), "ifconfig %s down", eth_name);
    system(cmd_str);
    snprintf(cmd_str, sizeof(cmd_str), "ifconfig %s %s", eth_name, ip_buf);
    system(cmd_str);
    snprintf(cmd_str, sizeof(cmd_str), "ifconfig %s up", eth_name);
    system(cmd_str);

    return WV_SUCCESS;
}


/*
* function: Net_SetMacAddr
*
* description:给指定网卡设置mac地址
*
* input:  @eth_name:网卡名
*         @mac_buf:mac地址
*         @buf_size:mac地址长度
*
* output: @
*
* return: 
*
* author: linsheng.pan
*/
wvErrCode Net_SetMacAddr(const char *eth_name, const unsigned char *mac_buf, int buf_size)
{
    if((!eth_name) || (!mac_buf))
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_NET, "Error: param = NULL");
        return WV_ERR_FAILURE;
    }

    char cmd_str[MAX_LINE_LENGTH] = {0};

    snprintf(cmd_str, sizeof(cmd_str), "ifconfig %s hw ether %02x:%02x:%02x:%02x:%02x:%02x", eth_name, mac_buf[0], mac_buf[1], mac_buf[2], mac_buf[3], mac_buf[4], mac_buf[5]);

    system(cmd_str);
    
    return WV_SUCCESS;
}


/*
* function: Net_SetIpAddr2FPGA
*
* description: 将IP地址告诉FPGA
*
* input:  @eth_name:网卡名
*         @ip_buf:ip地址("192.168.1.1")
*         @buf_size:地址长度
*
* output: @
*
* return: 
*
* author: linsheng.pan
*/
wvErrCode Net_SetIpAddr2FPGA(const char *eth_name, const char *ip_buf, int buf_size);


/*
* function: Net_SetMacAddr2FPGA
*
* description: 将Mac地址告诉FPGA
*
* input:  @eth_name:网卡名
*         @mac_buf:mac地址
*         @buf_size:mac地址的长度
*
* output: @
*
* return: 
*
* author: linsheng.pan
*/
wvErrCode Net_SetMacAddr2FPGA(const char *eth_name, const unsigned char *mac_buf, int buf_size);


/*
* function: Net_BindInterface
*
* description: 将套接字绑定网卡
*
* input:  @sockfd: 一个已经打开的套接字
*         @eth_name: 网卡名字
*
* output: @
*
* return: 
*
* author: linsheng.pan
*/
wvErrCode Net_BindInterface(int sockfd, const char *eth_name)
{
    if(!eth_name)
    {
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_NET, "Error: param = NULL");
        return WV_ERR_PARAMS;
    }

    int ret = 0;
    char err_buf[MAX_ERR_INFO_LEN] = {0};
    struct ifreq netdev;
    memset(&netdev, 0, sizeof(netdev));
    strncpy(netdev.ifr_name, eth_name, sizeof(netdev.ifr_name) - 1);

    ret = setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, &netdev, sizeof(netdev));

    if(ret)
    {
        ERR_STRING(err_buf);
        LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_NET, "Error: call setsockopt:%s", err_buf);

        return WV_ERR_SOCKET_SETOPT;
    }

    return WV_SUCCESS;
}

