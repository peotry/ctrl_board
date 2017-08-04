#include "code_convert.h"

//代码转换:从一种编码转为另一种编码      
int code_convert(char *from_charset,char *to_charset,char *inbuf,size_t inlen,char *outbuf,size_t outlen)     
{     
	iconv_t cd;     
    char **pin = &inbuf;     
	char **pout = &outbuf;

	cd = iconv_open(to_charset,from_charset);     
	if (cd==0) return -1;     
	memset(outbuf,0,outlen);      
	if (iconv(cd,pin,&inlen,pout,&outlen)==-1) 
    {
       printf("errno = %d\n",errno);
       return -1; 
    }    
	iconv_close(cd);     
	return 0;     
}

//UNICODE码转为GB2312码 	 
//int u2g(char *inbuf,size_t inlen,char *outbuf,size_t outlen) 	
//{	   
//	return code_convert("utf-8","gb2312",inbuf,inlen,outbuf,outlen);
//}

////GB2312码转为UNICODE码 	 
//int g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen)	  
//{ 	 
//	return code_convert("gb2312","utf-8",inbuf,inlen,outbuf,outlen);	 
//}


//UNICODE码转为GB2312码   
//成功则返回一个动态分配的char*变量，需要在使用完毕后手动free，失败返回NULL
char* u2g(char *inbuf)   
{
    int nOutLen = 2 * strlen(inbuf) - 1;
    char* szOut = (char*)malloc(nOutLen);
    if (-1 == code_convert("utf-8","gb2312",inbuf,strlen(inbuf),szOut,nOutLen))
    {
       free(szOut);
       szOut = NULL;
    }
    return szOut;
}   
//GB2312码转为UNICODE码   
//成功则返回一个动态分配的char*变量，需要在使用完毕后手动free，失败返回NULL
char* g2u(char *inbuf)   
{
    int nOutLen = 2 * strlen(inbuf) - 1;
    char* szOut = (char*)malloc(nOutLen);
    if (-1 == code_convert("gb2312","utf-8",inbuf,strlen(inbuf),szOut,nOutLen))
    {
       free(szOut);
       szOut = NULL;
    }
    return szOut;
}



