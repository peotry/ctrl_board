#ifndef _CODE_CONVERT_H
#define _CODE_CONVERT_H

#include <iconv.h> 
#include <stddef.h>
#include <errno.h>
#include <string.h> 
#include <stdio.h> 
#include <stdlib.h>

//代码转换:从一种编码转为另一种编码      
int code_convert(char *from_charset, char *to_charset, char *inbuf, size_t inlen, char *outbuf, size_t outlen);
//UNICODE码转为GB2312码 	 
//int u2g(char *inbuf, size_t inlen, char *outbuf, size_t outlen);
//GB2312码转为UNICODE码 	 
//int g2u(char *inbuf, size_t inlen, char *outbuf, size_t outlen);
char* u2g(char *inbuf);
char* g2u(char *inbuf);



#endif
