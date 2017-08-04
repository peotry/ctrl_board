#include "code_convert.h"

//����ת��:��һ�ֱ���תΪ��һ�ֱ���      
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

//UNICODE��תΪGB2312�� 	 
//int u2g(char *inbuf,size_t inlen,char *outbuf,size_t outlen) 	
//{	   
//	return code_convert("utf-8","gb2312",inbuf,inlen,outbuf,outlen);
//}

////GB2312��תΪUNICODE�� 	 
//int g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen)	  
//{ 	 
//	return code_convert("gb2312","utf-8",inbuf,inlen,outbuf,outlen);	 
//}


//UNICODE��תΪGB2312��   
//�ɹ��򷵻�һ����̬�����char*��������Ҫ��ʹ����Ϻ��ֶ�free��ʧ�ܷ���NULL
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
//GB2312��תΪUNICODE��   
//�ɹ��򷵻�һ����̬�����char*��������Ҫ��ʹ����Ϻ��ֶ�free��ʧ�ܷ���NULL
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



