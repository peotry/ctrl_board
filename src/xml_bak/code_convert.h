#ifndef _CODE_CONVERT_H
#define _CODE_CONVERT_H

#include <iconv.h> 
#include <stddef.h>
#include <errno.h>
#include <string.h> 
#include <stdio.h> 
#include <stdlib.h>

//����ת��:��һ�ֱ���תΪ��һ�ֱ���      
int code_convert(char *from_charset, char *to_charset, char *inbuf, size_t inlen, char *outbuf, size_t outlen);
//UNICODE��תΪGB2312�� 	 
//int u2g(char *inbuf, size_t inlen, char *outbuf, size_t outlen);
//GB2312��תΪUNICODE�� 	 
//int g2u(char *inbuf, size_t inlen, char *outbuf, size_t outlen);
char* u2g(char *inbuf);
char* g2u(char *inbuf);



#endif
