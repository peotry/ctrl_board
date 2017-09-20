#ifndef INCLUDE_TOOLS_LOG_H
#define INCLUDE_TOOLS_LOG_H

#include "common/appGlobal.h"

#define TEST_LOG    			 (0)

#define LOG_SUCCESS              (0)
#define LOG_ERROR                (-1)

#define LOG_FILE_PATH            ("/wellav_data/log/log.txt")
#define LOG_FILE_SIZE            (4 * 1024) //4 M

#define MAX_COMMAND_LEN          (256)
#define MAX_FILENAME_LEN         (256)

#define LOG_LINE_MAX_LEN		 (256)
#define LOG_LEVEL_NAME_MAX_LEN   (50)
#define LOG_MODULE_NAME_MAX_LEN  (50)


typedef enum
{
	LOG_LEVEL_ALL      =      (0xFF),
	LOG_LEVEL_DEBUG    =      (0x1 << 0),
	LOG_LEVEL_INFO     =      (0x1 << 1),
	LOG_LEVEL_NOTICE   =      (0x1 << 2),
	LOG_LEVEL_WARNING  =      (0x1 << 3),
	LOG_LEVEL_ERROR    =      (0x1 << 4),
}LogLevel_EM;

typedef enum
{
	LOG_OUTPUT_ALL       =      (0xFF),
	LOG_OUTPUT_CONSOLE   =      (0x1 << 0),
	LOG_OUTPUT_SYSLOG    =      (0x1 << 1),
	LOG_OUTPUT_MAINBOARD =      (0x1 << 2),
}LogOutput_EM;


typedef enum
{
	LOG_MODULE_ALL       =      (0xFFFF),
	LOG_MODULE_INIT      =      (0x1 << 0),
    LOG_MODULE_TOOLS     =      (0x1 << 1),
    LOG_MODULE_NET       =      (0x1 << 2),
    LOG_MODULE_DRIVERS   =      (0x1 << 3),
    LOG_MODULE_WEB       =      (0x1 << 4),
    LOG_MODULE_XML       =      (0x1 << 5),
    LOG_MODULE_NETON    =      (0x1 << 6)
}LogModule_EM;


typedef enum
{
	emLogOpenMode,
	emLogCloseMode,
}LogMode_EM;

struct LogCtrl_ST
{
	LogLevel_EM emLogLevel;
	LogModule_EM emLogModule;
	LogOutput_EM emLogOutput;
};


void Log_SetLevel(LogLevel_EM emLogLevel, LogMode_EM emLogMode);
void Log_SetModule(LogModule_EM emLogModule, LogMode_EM emLogMode);
void Log_SetOutputMode(LogOutput_EM emLogOutput, LogMode_EM emLogMode);

LogLevel_EM   Log_GetLevel(void);
LogModule_EM  Log_GetModule(void);
LogOutput_EM  Log_GetOutputMode(void);

S32  Log_Init(S8 *ps8LogSavePath, U32 u32LogSize);
void Log_Printf(LogLevel_EM emLogLevel, LogModule_EM emLogModule, const char *format, ...);
void Log_Clear(S8 *ps8LogSavePath, U32 u32LogSize);

#define LOG_PRINTF(emLogLevel, emLogModule, format, ...) Log_Printf(emLogLevel, emLogModule, "[%s:%d] " format "\r\n", __func__, __LINE__,##__VA_ARGS__)


#endif
