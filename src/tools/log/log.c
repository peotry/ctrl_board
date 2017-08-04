#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <syslog.h>
#include <stdarg.h>
#include <string.h>
#include <stdarg.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/syslog.h>

#include "log/log.h"

static struct LogCtrl_ST s_stLogCtrl;
static bool s_isSyslogOpen = false;
static bool s_isLogInit    = false;

S8 s8arrLevelName[][LOG_LEVEL_NAME_MAX_LEN] =
{
	"DEBUG",
	"INFO",
	"NOTICE",
	"WARNING",
	"ERROR"
};

S8 s8arrModuleName[][LOG_MODULE_NAME_MAX_LEN] =
{
	"INIT",
    "TOOLS",
    "NET",
    "DRIVERS",
    "WEB",
    "XML",
    "COMMON"
};


/* 
 *function:	 Log_SetLevel
 *
 *description:	set the log level
 *
 *input:	@u32LogLevel: the log level
 *			@emLogMode: open the level or close the level
 *
 *output:	none
 *
 *return:   none
 *
 *author:   linsheng.pan
 * */
void Log_SetLevel(LogLevel_EM emLogLevel, LogMode_EM emLogMode)
{
	if(emLogOpenMode == emLogMode)
	{
		s_stLogCtrl.emLogLevel |= emLogLevel;
	}
	else
	{
		s_stLogCtrl.emLogLevel &= (~emLogLevel);
	}
}

 
/* 
 *function:	 Log_SetModule
 *
 *description:	set the log module
 *
 *input:	@u32LogModule: the log module
 *			@emLogMode: open the module or close the module
 *
 *output:	none
 *
 *return:   none
 *
 *author:   linsheng.pan
 * */
void Log_SetModule(LogModule_EM emLogModule, LogMode_EM emLogMode)
{
	if(emLogOpenMode == emLogMode)
	{
		s_stLogCtrl.emLogModule |= emLogModule;
	}
	else
	{
		s_stLogCtrl.emLogModule &= (~emLogModule);
	}
}


/* 
 *function:	 Log_SetOutputMode
 *
 *description:	set the log output module
 *
 *input:	@u32LogOutputMode: the log output module
 *			@emLogMode: open the output module or close the output module
 *
 *output:	none
 *
 *return:   none
 *
 *author:   linsheng.pan
 * */
void Log_SetOutputMode(LogOutput_EM emLogOutput, LogMode_EM emLogMode)
{
	if(emLogOpenMode == emLogMode)
	{
		s_stLogCtrl.emLogOutput |= emLogOutput;
	}
	else
	{
		s_stLogCtrl.emLogOutput &= (~emLogOutput);
	}
}


LogLevel_EM Log_GetLevel(void)
{
	LogLevel_EM emLogLevel = 0;
	emLogLevel = s_stLogCtrl.emLogLevel;
	
	return emLogLevel;
}


LogModule_EM Log_GetModule(void)
{
	LogModule_EM emLogModule = 0;
	emLogModule = s_stLogCtrl.emLogModule;

	return emLogModule;
}


LogOutput_EM Log_GetOutputMode(void)
{
	LogOutput_EM emLogOutput = 0;
	emLogOutput = s_stLogCtrl.emLogOutput;

	return emLogOutput;
}


static S32 Log_IsOpenLevel(LogLevel_EM emLogLevel)
{
	if(!(s_stLogCtrl.emLogLevel & emLogLevel))
	{
		return LOG_ERR;
	}

	return LOG_SUCCESS;
}


static S32 Log_IsOpenModule(LogModule_EM emLogModule)
{
	if(!(s_stLogCtrl.emLogModule & emLogModule))
	{
		return LOG_ERR;
	}

	return LOG_SUCCESS;
}


static S8 *Log_GetLevelString(LogLevel_EM emLogLevel)
{
	S8 *ptr = NULL;
	U32 index = 0;

	U32 u32ArraySize = ARRAY_SIZE(s8arrLevelName);

	//find the string in the array
	for(index = 0; index < u32ArraySize; ++index)
	{
		if((emLogLevel & (0x1 << index)))
		{
			break;
		}
	}

	if(index < u32ArraySize)
	{
		ptr = s8arrLevelName[index];
	}
	else
	{
		//not match, and the default is debug
		ptr = s8arrLevelName[0];
	}

	return ptr;
}


static S8 *Log_GetModuleString(LogModule_EM emLogModule)
{
	S8 * ptr = NULL;
	U32 u32ArraySize = ARRAY_SIZE(s8arrModuleName);
	U32 index = 0;

	//find the string in the array
	for(index = 0; index < u32ArraySize; ++index)
	{
		if((emLogModule & (0x1 << index)))
		{
			break;
		}
	}

	if(index < u32ArraySize)
	{
		ptr = s8arrModuleName[index];
	}
	else
	{
		//not match, the default is sys
		ptr = s8arrModuleName[0];
	}

	return ptr;
}

/* 
 *function:	Log_GetDirPath
 *
 *description:	get dir path from filepath
 *
 *input:	@filepath:	filepath
 *
 *output:	@dirpath:	dirpath
 *
 * author:	ruibing.zhang
 * */
static S32 Log_GetDirPath(S8 *dirpath, S8 *filepath)
{
	if((NULL == dirpath) || (NULL == filepath))
	{
		LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_INIT, "Params is NULL!");
		return LOG_ERR;
	}

	S8 * ptr = NULL;
	ptr = (S8 *)strrchr((char *)filepath, '/');
	if(ptr)
	{
		strncpy((char *)dirpath, (char *)filepath, (size_t)(ptr - filepath));
	}
	else
	{
		LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_INIT, "filepath isn't absolute path");
		return LOG_ERR;
	}

	return LOG_SUCCESS;
}


/* 
 * function:	Log_Init
 *
 * description:	Init log
 *
 * input:	@ps8LogSavePath: save the path to log
 *          @u32LogSize: the log size(KB)
 *
 * output:	
 *
 * author:	ruibing.zhang
 *
 * modify:  linsheng.pan
 * */
S32 Log_Init(S8 *ps8LogSavePath, U32 u32LogSize)
{
	if((NULL == ps8LogSavePath))
	{
		LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_TOOLS, "Params is NULL");
		return LOG_ERR;
	}

	if(0 == u32LogSize)
	{
		LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_TOOLS, "Log Size is zero");
		return LOG_ERR;
	}

	S8 s8Command[MAX_COMMAND_LEN] = {0};
	S8 s8LogSaveDir[MAX_FILENAME_LEN] = {0};
	DIR *pdir = NULL;

	Log_GetDirPath(s8LogSaveDir, ps8LogSavePath);

	pdir = opendir((char *)s8LogSaveDir);
	if(NULL == pdir)
	{
		LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_INIT, "%s dir is not existed", s8LogSaveDir);
		s_isSyslogOpen = false;
	}
	else
	{
		closedir(pdir);
		snprintf((char *)s8Command, sizeof(s8Command), "syslogd -O %s -s %u", (char *)ps8LogSavePath, u32LogSize);
		//system((char *)s8Command);

        //TODO
		//openlog(PROCESS_NAME, LOG_PID, LOG_USER);

		s_isSyslogOpen = true;
	}

	//open all prints
	Log_SetLevel(LOG_LEVEL_ALL, emLogOpenMode);
	Log_SetModule(LOG_MODULE_ALL, emLogOpenMode);
	Log_SetOutputMode(LOG_OUTPUT_ALL, emLogOpenMode);

	s_isLogInit = true;

	return LOG_SUCCESS;
}


static S32 Log_SendMainborad(LogLevel_EM emLogLevel, U8 *pu8PrintBuff)
{
	return LOG_SUCCESS;
}

/* 
 * function:	Log_Printf
 *
 * description:	print the information in the terminal and save it in the log file
 *
 * input:	@u32LogLevel: the log level
 *          @u32LogModule: the log module
 *
 * output:	
 *
 * author:	ruibing.zhang
 *
 * modify:  linsheng.pan
 * */
void Log_Printf(LogLevel_EM emLogLevel, LogModule_EM emLogModule, const char *format, ...)
{
	va_list 	Argument;
	static U8	u8arrTempBuff[LOG_LINE_MAX_LEN];
	U8 			u8arrStringBuff[LOG_LINE_MAX_LEN];
	U32 		u32LogOutputMode = 0;

	memset(u8arrTempBuff, 0, sizeof(u8arrTempBuff));

	//get the information to be printed
	va_start(Argument, format);
	vsprintf((char*)u8arrTempBuff, format, Argument);
	va_end(Argument);

	snprintf((char*)u8arrStringBuff, sizeof(u8arrStringBuff), "[%s][%s]%s", Log_GetLevelString(emLogLevel), Log_GetModuleString(emLogModule), u8arrTempBuff);

	//just for the log init
	if((LOG_MODULE_INIT == emLogModule) && (false == s_isLogInit))
	{
		printf("%s\n log module is not initialized\n", u8arrStringBuff);
	}

	//check whether the log level and log module are open
	if(!((LOG_SUCCESS == Log_IsOpenLevel(emLogLevel)) && (LOG_SUCCESS == Log_IsOpenModule(emLogModule))))
	{
		return ;
	}

	u32LogOutputMode = Log_GetOutputMode();
	if(u32LogOutputMode & LOG_OUTPUT_CONSOLE)
	{
		printf("%s", u8arrStringBuff);
	}

	if(u32LogOutputMode & LOG_OUTPUT_SYSLOG)
	{
		if(true == s_isSyslogOpen)
		{
            //TODO
			//syslog(LOG_INFO, "%s", u8arrStringBuff);
		}
		else
		{
			printf("[%s:%d] syslogd is not open\n", __func__, __LINE__);
		}
	}
}


void Log_Clear(S8 *ps8LogSavePath, U32 u32LogSize)
{
	if(NULL == ps8LogSavePath)
	{
		LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_TOOLS, "Param is NULL");
		return;
	}

	if(0 == u32LogSize)
	{
		LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_TOOLS, "Log Size is zero");
		return;
	}

	S8 s8Command[MAX_COMMAND_LEN] = {0};
	FILE *fp = NULL;

	//stop syslogd
	system("syslogd -s 0");

	fp = fopen((char *)ps8LogSavePath, "w+");
	if(NULL == fp)
	{
		LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_TOOLS, "can't open the log file");
		return;
	}

	fclose(fp);


	//restart syslogd
	snprintf((char *)s8Command, sizeof(s8Command), "syslogd -O %s -s %u", (char *)ps8LogSavePath, u32LogSize);
	system((char *)s8Command);

}


#if TEST_LOG
int main(int argc, char **argv)
{
	Log_Init("/home/root/log.txt", 1024);

	LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_PCM,"pcm..");
	LOG_PRINTF(LOG_LEVEL_INFO, LOG_MODULE_SYS,"sys..");
	LOG_PRINTF(LOG_LEVEL_NOTICE, LOG_MODULE_COMM,"comm..");
	LOG_PRINTF(LOG_LEVEL_WARNING, LOG_MODULE_FPGA, "fpga..");
	LOG_PRINTF(LOG_LEVEL_ERROR, LOG_MODULE_MUXER,"muxer..");
	LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_TABLE,"table..");
	LOG_PRINTF(LOG_LEVEL_INFO, LOG_MODULE_UPDATA, "module..");
	LOG_PRINTF(LOG_LEVEL_NOTICE, LOG_MODULE_ENCODER, "encoder..");

	printf("\n\n");
	Log_SetLevel(LOG_LEVEL_DEBUG, emLogCloseMode);
	LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_TABLE,"table..");
	Log_SetLevel(LOG_LEVEL_DEBUG, emLogOpenMode);
	LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_TABLE,"table..");


	printf("\n\n");
	Log_SetModule(LOG_MODULE_PCM, emLogCloseMode);
	LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_PCM,"pcm..");
	Log_SetModule(LOG_MODULE_PCM, emLogOpenMode);
	LOG_PRINTF(LOG_LEVEL_DEBUG, LOG_MODULE_PCM,"pcm..");

	sleep(20);

	Log_Clear("/home/root/log.txt",1024);

	return 0;
}
#endif



