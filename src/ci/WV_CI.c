/******************************************************************************
 * WV_CI.c - WV_CI
 * 
 * Copyright 2008-2013 HuiZhou WELLAV Technology Co.,Ltd.
 * 
 * DESCRIPTION: - 
 *      WV_CI
 * modification history
 * --------------------
 * 10 Oct 2008, WELLAV-TT written
 * --------------------
 ******************************************************************************/

/****************************************************************************
 * Include files
 ****************************************************************************/
// Standar
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

// CI
#include "all.h"
#include "PCMCIA.h"
#include "CI.h"

// GUI
#include "WVCI_GUI.h"

#include "WV_CI.h"
#include "TSConstruct.h"
#include "FPGA.h"

#define WVCI_InfoPrint(x)       printf x
#define WVCI_ErrorPrint(x)      printf x


extern const BYTE g_pDefaultStreamTypeList[];
extern int R2_CI_SetTSPATH(U8 u8Slot, U8 u8Mode);
extern volatile U32 s_CAMStatusCapture;


int WVCI_GetEvent(U8 u8Slot, int nCAMStatus);

/****************************************************************************
 * Macro
 ****************************************************************************/
CIMAX_PCMCIA_HANDLESTRUCT_t g_CIMax_CIHandles[2];
    
    
/****************************************************************************
 * vars
 ****************************************************************************/
// gobal 
WVCI_Status_t       g_WVCIStatusInst;
WVCI_Status_t*      g_ptWVCIStatusInst = &g_WVCIStatusInst;

static int          s_nMAXTRYREAD[2] ;
U32                 g_Status_CI_ResetTimes[2] = {0};                     // CI通信异常次数统计


static int          n_ts_through_cam0       = 0x00;
static int          n_ts_through_cam1       = 0x00;
static int          n_LastProgramSetupState = 0x00;

static WVAlarm_CI_Status_t      CAMCommStatus[WVCI_MAX_SLOT_NUM];

//增加任务互斥锁 ==>全局变量
U8  s_CITaskMuxFlag = 0;

static int  s_nExceptionRebootFlag[2] = {0};





#ifdef ENABLE_CI_PES_CHECKING
typedef struct 
{
    U16     u16PIDList[CI_PES_MAX_PID_NUM];
    U8      u8Ready;
    U8      u8PIDOffset;
    U8      u8CheckState;       // 
    U8      u8StateCnt;
    U8      u8StatePIDOffset;   // 
    U8      u8BitrateEnable;
    U8      u8Reserved[2];
} PES_CheckInfo_t;

PES_CheckInfo_t     g_PESCHECK[WVCI_MAX_SLOT_NUM];
U8                  g_PES_SLOT = 0;
U8                  g_PES_ENABLE[WVCI_MAX_SLOT_NUM] = {0};
extern TParameter_MainBoard gDMPParam;  
#endif



U16 s_u16LastDescrProgIndex[WVCI_MAX_SLOT_NUM] = {0};

DWORD     g_dwNumPCMCIASlots = WVCI_MAX_SLOT_NUM;     // 最大支持通道数目，协议栈要求定义



typedef struct
{
    BYTE   ucIsNeedSend;    // 1: need to send, 0: no need
    BYTE   ucModuleIndex;   // 0: slot 0, 1: slot 1
    BYTE   ucReserved[2];
} CI_AutoSendPMT_t;

static CI_AutoSendPMT_t        AutoSendPMT[2];


int  WVCI_SetAutoSendPMTFlag(BYTE ucSlotIndex)
{
    if (ucSlotIndex == 0x00 ||
        ucSlotIndex == 0x01)
    {
        AutoSendPMT[ucSlotIndex].ucIsNeedSend = 0x01;
        AutoSendPMT[ucSlotIndex].ucModuleIndex = ucSlotIndex;    
    }
    return WVCI_SUCCESS;
}


void WVCI_PMTCheck(void)
{
    // VARS   
    int          ii = 0;
    
    for (ii = 0; ii < 2; ii++)
    {
        if (AutoSendPMT[ii].ucIsNeedSend == 0x01)
        {
            WVCI_AutoUpdateModule(AutoSendPMT[ii].ucModuleIndex);
            AutoSendPMT[ii].ucIsNeedSend = 0x00;
        }
    }
    // 以后要增加pmt版本更新操作
}

#if 1
U8                      g_u8MMI_CurrentMenuLevel[2] = {WAV_CI_MMI_ROOT_LEVEL};
U8                      g_u8MMI_LastAccessSlot      = 0;
U8                      g_u8MMI_PINCode[MAX_PIN_NUM_LEN + 1]    = {0};
U8                      g_u8MMI_AnswerTextLen       = 4;
WAVCI_MMI_Info_t        g_stMMIInfo;

/******************************************************************************
 * WVCI_MMI_UpdateInfo() - 
 * DESCRIPTION: - Check module is inited or not
 * Input  : ucSlotIndex             slot index
 *          nTimeoutMS              Max try time
 * Output : 
 * Returns: WVCI_SUCCESS            get data ok
 *          WVCI_ERROR_PARAMS       bad parameters
 *          WVCI_ERROR_NO_INSERT    cam no inserted
 *          WVCI_FAILURE            cam no init
 *          WVCI_ERROR_TIMEOUT      get data timeout
 *          WVCI_ERROR_EXIT_MMI     not in mmi
 * 
 ******************************************************************************/
int WVCI_MMI_UpdateInfo(U8 ucSlotIndex, int nTimeoutMS)
{
    // vars
    int         nRet = WVCI_SUCCESS;

    // check params
    if (ucSlotIndex >= WVCI_MAX_SLOT_NUM)
    {
        return WVCI_ERROR_PARAMS;
    }
    
    // init the data
    memset(&g_stMMIInfo, 0x00, sizeof(WAVCI_MMI_Info_t));

    // no inserted
    if (!WVCI_IsModuleInserted(ucSlotIndex))
    {
        g_stMMIInfo.operate_ret = WVCI_ERROR_NO_INSERT;
        return WVCI_ERROR_NO_INSERT;        
    }  

    // no inited
    if (!WVCI_IsModuleInited(ucSlotIndex))
    {
        g_stMMIInfo.operate_ret = WVCI_ERROR_NO_INITED;
        return WVCI_ERROR_NO_INITED;        
    }

    // no in mmi
    if (g_u8MMI_CurrentMenuLevel[ucSlotIndex] == WAV_CI_MMI_ROOT_LEVEL)
    {
        g_stMMIInfo.operate_ret = WVCI_ERROR_EXIT_MMI; // no in mmi
        return WVCI_ERROR_EXIT_MMI;
    }    
    
    // Loop to get the mmi info
    while ((nTimeoutMS > 0) && (nRet == WVCI_SUCCESS))
    {
        //OSTimeDlyHMSM(0, 0, 0, 100);
        usleep(100*1000);
        nTimeoutMS -= 100;
        
        if (0x01 == g_stMMIInfo.mmi_text_valid) 
        {
            nRet = WVCI_SUCCESS;
            break;
        }
    }

    if ((WVCI_ERROR_EXIT_MMI != nRet) && (nTimeoutMS <= 0))
    {
        nRet = WVCI_ERROR_TIMEOUT;
    }    

    //
    if (0x01 == g_stMMIInfo.mmi_text_valid)
    {
        g_stMMIInfo.operate_ret = 0;             // success;             
        return WVCI_SUCCESS;
    }
    else
    {
        g_stMMIInfo.operate_ret = nRet;          // timeout;
        return nRet;
    }
}

extern  U8       bIsInputPINOperate;
int WVCI_MMI_CommandProcess(BYTE u8Slot, BYTE u8MMICmd, BYTE *pucData, int nDataLen, int nTimoutMS)
{
    // vars
    int         nRet = WVCI_SUCCESS;
    
    // check params
    if (u8Slot >= WVCI_MAX_SLOT_NUM)
    {
        CI_LOG_ERR("[MMI] WVCI_ERROR_PARAMS slot bad params(%d)!\r\n", u8Slot);
        return WVCI_ERROR_PARAMS;
    }

    // check cam module is plugged or not
    if (!WVCI_SYSIsModuleStillPlugged(u8Slot))
    {
        nRet = WVCI_MMI_UpdateInfo(u8Slot, nTimoutMS); 
        CI_LOG_ERR("[MMI] WVCI_ERROR_NO_INSERT slot%d no inserted!\r\n", u8Slot);
        return WVCI_ERROR_NO_INSERT;
    }

    if (bIsInputPINOperate)
    {
        u8MMICmd = WVCI_MMI_SEND_PIN_CODE;
    }
    CI_LOG_INF("[MMI] =========  slot:%d, cmm: %d =========\r\n", u8Slot, u8MMICmd);
    memcpy(g_u8MMI_PINCode, pucData, g_u8MMI_AnswerTextLen);
    switch(u8MMICmd)
    {
        case WVCI_MMI_CLOSE:
        {
            if (g_u8MMI_CurrentMenuLevel[u8Slot] != WAV_CI_MMI_ROOT_LEVEL)
            {
                CI_LOG_INF("[MMI] WVCI_MMI_CLOSE \r\n");
                g_u8MMI_CurrentMenuLevel[u8Slot] = WAV_CI_MMI_ROOT_LEVEL;
                CI_MMI_Close(u8Slot);
            }
            break;
        }
        
        case WVCI_MMI_CTRL_ENTER_MENU: 
        {
            if (g_u8MMI_CurrentMenuLevel[u8Slot] == WAV_CI_MMI_ROOT_LEVEL)
            {
                CI_LOG_INF("[MMI] WVCI_MMI_CTRL_ENTER_MENU Enter Menu.\r\n");
                CI_MMI_EnterMenu(u8Slot);
                g_u8MMI_CurrentMenuLevel[u8Slot]        = 2;
            }
            else
            {
                CI_LOG_INF("[MMI] WVCI_MMI_CTRL_ENTER_MENU Exit Menu. \r\n");
                CI_MMI_Close(u8Slot);
                g_u8MMI_CurrentMenuLevel[u8Slot]        = WAV_CI_MMI_ROOT_LEVEL;
            }
            
            g_u8MMI_LastAccessSlot                      = u8Slot;                
            break;
        }

        case WVCI_MMI_CTRL_ESCAPE: 
        {
            CI_LOG_INF("[MMI] WVCI_MMI_CTRL_ESCAPE \r\n");
            if (g_u8MMI_CurrentMenuLevel[u8Slot] != WAV_CI_MMI_ROOT_LEVEL)
            {
                CI_MMI_AnswerMenu(u8Slot, 0);
            }
            break;
        }
        case WVCI_MMI_CTRL_ARROW_LEFT: 
        case WVCI_MMI_CTRL_ARROW_RIGHT: 
        case WVCI_MMI_CTRL_ARROW_UP:    
        case WVCI_MMI_CTRL_ARROW_DOWN:  
        {
            CI_LOG_INF("[MMI] WVCI_MMI_CTRL_ARROW_LEFT \r\n");
            break;
        }
        
        case WVCI_MMI_CTRL_CANCEL_ACTION: 
        {
            CI_LOG_INF("[MMI] WVCI_MMI_CTRL_CANCEL_ACTION \r\n");
            if (g_u8MMI_CurrentMenuLevel[u8Slot] != WAV_CI_MMI_ROOT_LEVEL)
            {
                CI_MMI_AnswerMenu(u8Slot, 0);
            }
            break;
        }
        case WVCI_MMI_CTRL_DEFAULT_ACTION: 
        {
            CI_LOG_INF("[MMI] WVCI_MMI_CTRL_DEFAULT_ACTION \r\n");
            break;
        }
        case WVCI_MMI_CTRL_0_CANCELED_PREV_MENU: 
        {
            CI_LOG_INF("[MMI] WVCI_MMI_CTRL_0_CANCELED_PREV_MENU \r\n");
            if (g_u8MMI_CurrentMenuLevel[u8Slot] != WAV_CI_MMI_ROOT_LEVEL)
            {                
                CI_MMI_AnswerMenu(u8Slot, 0);
            }
            break;
        }

        case WVCI_MMI_SEND_PIN_CODE:
        {
            CI_LOG_INF("[MMI] WVCI_MMI_SEND_PIN_CODE %s, answer len:%d\r\n", g_u8MMI_PINCode, g_u8MMI_AnswerTextLen);            
            bIsInputPINOperate = FALSE;
            CI_MMI_AnswerInput(u8Slot, g_u8MMI_PINCode, g_u8MMI_AnswerTextLen);            
            break;
        }
        //
        // as answer command
        default: 
        {
            if ((g_u8MMI_CurrentMenuLevel[u8Slot] != WAV_CI_MMI_ROOT_LEVEL)         &&
                (u8MMICmd > WVCI_MMI_CTRL_0_CANCELED_PREV_MENU)   &&
                (u8MMICmd < WVCI_MMI_SEND_PIN_CODE))
            {         
                CI_LOG_INF("[MMI] Anser Menu %d \r\n", (u8MMICmd-2));
                CI_MMI_AnswerMenu(u8Slot, (u8MMICmd-2));
            }
            break;            
        }
    }     
    
    nRet = WVCI_MMI_UpdateInfo(u8Slot, nTimoutMS);   
    return WVCI_SUCCESS;
}


WAVCI_MMI_Info_t *WVCI_MMI_GetMenuInfo(void)
{
    CI_LOG_INF("[MMI] WVCI_MMI_GetMenuInfo() \r\n");
    return &g_stMMIInfo;
}


#endif



void WVCI_TaskMonitoring(void *pvParams);

/****************************************************************************
 *                                                                          *
 *                          -- WV_CI API Functions --                       *
 *                                                                          *
 ****************************************************************************/
#if defined(ENABLE_CI_SEMPHORE)
//static OS_EVENT *hSemphoreCIData = NULL;
pthread_mutex_t         g_ci_mutex;


int WVCI_InitSemphore(void)
{

    pthread_mutex_init(&g_ci_mutex, NULL);

    return WVCI_SUCCESS;
}

int WVCI_GetSemphore(U32 u32TickCnt)
{
    if (u32TickCnt == 0){
        u32TickCnt = 1; //wait for 1 tick, "0" means that wait for ever.
    }
    
    pthread_mutex_lock(&g_ci_mutex);

    return WVCI_SUCCESS;
}

int WVCI_ReleaseSemphore(void)
{
     pthread_mutex_unlock(&g_ci_mutex);
    return WVCI_SUCCESS;
}
#endif

/******************************************************************************
 * WVCI_Setup() - ...
 * DESCRIPTION: - 
 *     配置WVCI模块 
 * Input  : 
 * Output : 
 * Returns: 
 * 
 * modification history
 * --------------------
 * 15 Dec 2009
 * --------------------
 ******************************************************************************/
int WVCI_Setup()
{
    int i = 0;
    
    // Init PARAMS
    memset(g_ptWVCIStatusInst, 0, sizeof(WVCI_Status_t)); 
    memcpy(g_ptWVCIStatusInst->m_ucSlot0Name, "EMPTY", 5);
    memcpy(g_ptWVCIStatusInst->m_ucSlot1Name, "EMPTY", 5);

#if defined(ENABLE_CI_SEMPHORE)
    WVCI_InitSemphore();
   // WVCI_ReleaseSemphore();
#endif
    
    for (i = 0; i < 2; i++)
    {
        memset(&CAMCommStatus[i],0x00,sizeof(WVAlarm_CI_Status_t));
    }
    
    // 初始化实例
    for (i = 0; i < WVCI_MAX_SLOT_NUM; i++)
    {
        // init static vars
        g_CIMax_CIHandles[i].ucSlotReg      = 0;
        g_CIMax_CIHandles[i].pucSlotMemory  = 0;
        g_CIMax_CIHandles[i].emSlotState    = CIMAX_EMPTY;
        g_CIMax_CIHandles[i].ucSlotIndex    = i;
        g_CIMax_CIHandles[i].bLastAccessWasToAttrMem = 0;
    }

    // 3 CI-Lib init
    CI_Init();

    (void)TSMUX_CI_SetMaxBitrate(CI_TS_BITRATE_108Mbps_8);

    
    // 正常启动时，先关闭CAM供电，因为目前带监控的逻辑是默认供电
     WVCI_SYSPowerOffModule(0);
     WVCI_SYSPowerOffModule(1);

    return WVCI_SUCCESS; 
}


static void CIMax_CheckSlot(CIHANDLE hCI,BYTE ucSlotNum)
{
    // vars    
    PCMCIA_INFO     *pInfo;
    int             nRet = 0;
    BOOL            bIsReturn = FALSE;
    int             nMaxRetryTimes = 5;
    
    switch(hCI->emSlotState)
    {
         
        case CIMAX_INSERTED:
        {            
            do
            {
                CI_OSSleep(2000);    // wait for the cam ready
                PCMCIA_ReadAttribMem(hCI->ucSlotIndex, (BYTE *)hCI->ucAttribMem);            
                nRet = PCMCIA_ParseAttribMem(hCI->ucAttribMem, sizeof(hCI->ucAttribMem), &hCI->tPCMCIAInfo);
                if (nRet != 0x00)
                {
                    nMaxRetryTimes--;
                    if (nMaxRetryTimes < 0)
                    {
                        // read mem failure, reset the cam
                        STATUS_ResetCAM(ucSlotNum,CI_INIT_ERROR);
                        return;
                    }
                }
                else
                {
                    break;
                }
                
            } while (1);
            
            // set the satae
            hCI->emSlotState = CIMAX_CISREAD;

            // show info
            pInfo = &hCI->tPCMCIAInfo;          

#ifdef ENABLE_CI

            // 在这里加循环CI_CheckConfig
            do
            {
                bIsReturn = CI_CheckConfig(hCI, pInfo, ucSlotNum);
                GURU_DelayMS(30);
                if(bIsReturn)
                {
                    hCI->emSlotState = CIMAX_CISTART;
                    break;
                }
                else
                {                        
                    if (nMaxRetryTimes <= 0)
                    {
                        hCI->emSlotState = CIMAX_CISREAD; 
                    }
                    CI_LOG_ERR("[CIMax_CheckSlot] CHECK CONFIG FAILURE!!!! \r\n");
                }                
                nMaxRetryTimes--;
                CI_OSSleep(20);
                
            } while (!bIsReturn && nMaxRetryTimes > 0);
#endif

            break;
        }

        
#ifdef ENABLE_CI
        case CIMAX_CISTART:
        {            
            CI_CheckSlot(hCI,ucSlotNum);
            
            break;
        }
#endif

        ////TT-Add
        case CIMAX_CISREAD:
        {        
           printf(" *** CIMAX_CISREAD\r\n");
            s_nMAXTRYREAD[ucSlotNum]++;
            
            CI_LOG_DEBG("[CIMax_CheckSlot] CIMAX_CISREAD (retry times%d)\r\n", s_nMAXTRYREAD[ucSlotNum]);
            if (s_nMAXTRYREAD[ucSlotNum] >= 3)
            {
                s_nMAXTRYREAD[ucSlotNum] = 0;                
                // read mem failure, reset the cam
                hCI->emSlotState = CIMAX_EMPTY;
                //WVCI_SYSResetModule(ucSlotNum);  
                STATUS_ResetCAM(ucSlotNum,CI_INIT_ERROR);         
            }
            break;
        }
        
        default:
        {
            CI_LOG_ERR("[CIMax_CheckSlot] never into here!! Or error!!!!\r\n");
        }

    }
}
void WVCI_TaskMonitoring(void *pvParams)
{
    // VARS
    int         nEventOfCAMStatus  = 0;
    int         nCAMStatusRegister = 0x00;
    BYTE        u8Slot = 0;
    int         nCAMEvent = 0;

    while (1)
    {
        usleep(500000);
            
        nEventOfCAMStatus = WVCI_GetINTEvent();
        if (nEventOfCAMStatus != 0x00) 
        {
            WVCI_ClearINTEvent();
            nCAMStatusRegister = WVCI_SYSGetCAMRegister();

            for (u8Slot = 0; u8Slot < WVCI_MAX_SLOT_NUM; u8Slot++)
            {
                nCAMEvent = WVCI_GetEvent(u8Slot, nCAMStatusRegister);
                if (CAM_EVENT_INSERTED == nCAMEvent)
                {   
                    WVCI_InsertCAM(u8Slot);                    
                }
                else if (CAM_EVENT_REMOVED == nCAMEvent)
                {
                    WVCI_RemoveCAM(u8Slot);
                }
                usleep(100*1000);
            }
        }

        for (u8Slot = 0; u8Slot < WVCI_MAX_SLOT_NUM; u8Slot++)
        {     
            if((g_CIMax_CIHandles[u8Slot].emSlotState != CIMAX_EMPTY))
            {      
                CIMax_CheckSlot(&g_CIMax_CIHandles[u8Slot], u8Slot);               
            }
        }
    }
}



void WVCI_TaskProcess(void *pvParams)
{
    // vars
    int     nIsCam0Inserted = 0;
    int     nIsCam1Inserted = 0;
    int     nTSThroughCAM0Flag = 0;
    int     nTSThroughCAM1Flag = 0;

    int     nLastCAM0InsertState = 0x00;
    int     nLastCAM1InsertState = 0x00;
    
    
    while (1)
    {
        usleep(500*1000);
        WVCI_PMTCheck();
        
        // check the cam insert or not
        nIsCam0Inserted = WVCI_IsModuleInserted(0);
        nIsCam1Inserted = WVCI_IsModuleInserted(1);

        if (n_LastProgramSetupState == 0x01)
        {
            n_LastProgramSetupState = 0x00;
            printf ("program setup status is changed! \r\n");
            // if cam is not inserted and ts setting through this one, bypass
            if (n_ts_through_cam0 == 0x01 && nIsCam0Inserted == 0x01)
            {
                nTSThroughCAM0Flag = 0x01;
            }
            else
            {
                nTSThroughCAM0Flag = 0x00; // ts bypass cam0, if cam not inserted
            }

            if (n_ts_through_cam1 == 0x01 && nIsCam1Inserted == 0x01)
            {
                nTSThroughCAM1Flag = 0x01;
            }
            else
            {
                nTSThroughCAM1Flag = 0x00; // ts bypass cam1, if cam not inserted
            }
            
            //TSMUX_CI_SetTSPATH(nTSThroughCAM0Flag, nTSThroughCAM1Flag);
        }

        if (nIsCam0Inserted != nLastCAM0InsertState     ||
            nIsCam1Inserted != nLastCAM1InsertState)
        {
            printf ("CAM inserted status is changed, cam0: %d-->%d, cam1: %d-->%d \r\n", nLastCAM0InsertState, nIsCam0Inserted, nLastCAM1InsertState, nIsCam1Inserted);
            
            nLastCAM0InsertState = nIsCam0Inserted;
            nLastCAM1InsertState = nIsCam1Inserted;
            // if cam is not inserted and ts setting through this one, bypass
            if (n_ts_through_cam0 == 0x01 && nIsCam0Inserted == 0x01)
            {
                nTSThroughCAM0Flag = 0x01;
            }
            else
            {
                nTSThroughCAM0Flag = 0x00; // ts bypass cam0, if cam not inserted
            }

            if (n_ts_through_cam1 == 0x01 && nIsCam1Inserted == 0x01)
            {
                nTSThroughCAM1Flag = 0x01;
            }
            else
            {
                nTSThroughCAM1Flag = 0x00; // ts bypass cam1, if cam not inserted
            }
            
            //TSMUX_CI_SetTSPATH(nTSThroughCAM0Flag, nTSThroughCAM1Flag);
        }
    }
}


void CIMAX_PCMCIAModuleRemoved(CIHANDLE hCI)
{
    switch(hCI->emSlotState)
    {
        case CIMAX_CISTART:
        {
            CI_ModuleRemoved(hCI,hCI->ucSlotIndex);
            break;
        }   
        default: break;
    }

    hCI->emSlotState = CIMAX_EMPTY;
}

int CIMAX_PCMCIAModuleInserted(CIHANDLE hCI)
{
    int nRet = 0;
    
    hCI->emSlotState = CIMAX_INSERTED;
    
    hCI->bLastAccessWasToAttrMem = TRUE;

    return nRet;
}


int WVCI_GetEvent(U8 u8Slot, int nCAMStatus)
{
    int     nCheckFlag = 0;
    int     nEvent = CAM_EVENT_NONE;

    if (u8Slot >= WVCI_MAX_SLOT_NUM)
    {
        CI_LOG_ERR("[WVCI_GetEvent] BAD PARAMS!!\r\n");
        return nEvent;
    }    

    nCheckFlag = (nCAMStatus >> u8Slot) & 0x01;

    // 
    if (g_CIMax_CIHandles[u8Slot].emSlotState == CIMAX_EMPTY)
    {
        if (0x01 == nCheckFlag)
        {
            nEvent = CAM_EVENT_INSERTED;
        }
    }
    else
    {
        if (0x00 == nCheckFlag)
        {
            nEvent = CAM_EVENT_REMOVED;
        }
    }
    //printf ("[WVCI_GetEvent] CAM%d nCAMStatus:%d, EVENT: %d \r\n", u8Slot, nCAMStatus, nEvent);
    return nEvent;
}



void WVCI_InsertCAM(U8 u8Slot)
{
    if (u8Slot >= WVCI_MAX_SLOT_NUM)
    {
        CI_LOG_ERR("[WVCI_InsertCAM] BAD PARAMS!!\r\n");
        return;
    }
    CI_LOG_INF("[WVCI_InsertCAM] CAM%d inserted!!\r\n",u8Slot);
    
    if (0 == u8Slot)
    {
        // 增加状态显示
        memset(&g_ptWVCIStatusInst->m_ucSlot0Name, 0x00, WVCI_SLOT_NAME_SIZE);                        
        memcpy(&g_ptWVCIStatusInst->m_ucSlot0Name, "Inserted...", WVCI_SLOT_NAME_SIZE);
        //g_ptWVCIStatusInst->m_ucModuleValid  |= 0x01; 
    }
    else
    {
        // 增加状态显示
        memset(&g_ptWVCIStatusInst->m_ucSlot1Name, 0x00, WVCI_SLOT_NAME_SIZE);                        
        memcpy(&g_ptWVCIStatusInst->m_ucSlot1Name, "Inserted...", WVCI_SLOT_NAME_SIZE);
        //g_ptWVCIStatusInst->m_ucModuleValid  |= 0x02;  
    }
    g_ptWVCIStatusInst->m_u8SlotInserted[u8Slot] = 0x01;
    CIMAX_PCMCIAModuleInserted(&g_CIMax_CIHandles[u8Slot]);
    g_CIMax_CIHandles[u8Slot].emSlotState = CIMAX_INSERTED;
    CAMCommStatus[u8Slot].CISlotState  = CIMAX_INSERTED;
    CAMCommStatus[u8Slot].CAMCommStatusError = FALSE;
    // power off 
    WVCI_SYSPowerOffModule(u8Slot); 
   // OSTimeDlyHMSM(0, 0, 0, 500);
    usleep(500*1000);
    // power on
    WVCI_SYSPowerOnModule(u8Slot);
    usleep(500*1000);
    PCMCIA_ResetModule2(u8Slot);

    g_u8MMI_CurrentMenuLevel[u8Slot] = WAV_CI_MMI_ROOT_LEVEL;
    return;
}



void WVCI_SetExceptionFlag(U8 u8Slot)
{
    printf ("[WVCI_GetEvent] u8Slot%d Set exception flag!!\r\n", u8Slot);
    s_nExceptionRebootFlag[u8Slot] = 1;
}

void WVCI_ClearExceptionFlag(U8 u8Slot)
{
    printf ("[WVCI_GetEvent] u8Slot%d Clear exception flag!!\r\n", u8Slot);
    s_nExceptionRebootFlag[u8Slot] = 0;
}

int WVCI_GetExceptionFlag(U8 u8Slot)
{
    return s_nExceptionRebootFlag[u8Slot];
}

void WVCI_RemoveCAM(U8 u8Slot)
{
    if (u8Slot >= WVCI_MAX_SLOT_NUM)
    {
        CI_LOG_ERR("[WVCI_RemoveCAM] BAD PARAMS!!\r\n");
        return;
    }
    CI_LOG_INF("[WVCI_RemoveCAM] CAM%d removed!!\r\n",u8Slot);
    
    // wav_status
    CAMCommStatus[u8Slot].CISlotState = CIMAX_EMPTY;
    CAMCommStatus[u8Slot].CAMCommStatusError = FALSE;
    CIMAX_PCMCIAModuleRemoved(&g_CIMax_CIHandles[u8Slot]);    
    g_ptWVCIStatusInst->m_u8SlotInserted[u8Slot] = 0x00;
    WVCI_SYSPowerOffModule(u8Slot);  
    #ifdef ENABLE_CI_PES_CHECKING
    WVCI_PES_InitSlot(u8Slot);
    #endif 

    g_u8MMI_CurrentMenuLevel[u8Slot] = WAV_CI_MMI_ROOT_LEVEL;
}


/******************************************************************************
 * WVCI_AutoUpdateModule() - 
 * DESCRIPTION: - 
 *     当模块初始化晒时,自动重新发送PMT到模块
 *     注意,不能与CIMODULE底层通信同一个任务, 因为部分CAM在发送PMT过快时存在PMT丢失问题...
 * Input  : 
 * Output : 
 * Returns: 
 * 
 * modification history
 * --------------------
 * 16 Dec 2009
 * --------------------
 ******************************************************************************/
int WVCI_AutoUpdateModule(BYTE ucSlotIndex)
{    
    if (ucSlotIndex == 0)
    {        
        WVCI_UpdateSetting(ONLY_SLOT_0);
    }
    else
    {
        WVCI_UpdateSetting(ONLY_SLOT_1);
    }
    
    return WVCI_SUCCESS;
}


/******************************************************************************
 * WVCI_MessageCallback() - 底层回调使用
 * DESCRIPTION: - 
 *     .. 
 * Input  : 
 * Output : 
 * Returns: 
 * 
 * modification history
 * --------------------
 * 15 Dec 2009
 * --------------------
 ******************************************************************************/
int WVCI_MessageCallback(int nSlotIndex, BYTE *pData, int nDataLen)
{
    // VARS
    int     nRet = WVCI_SUCCESS;

    // CHECK PARAMS
    if (pData == NULL           ||
        nDataLen <= 0)
    {
        return WVCI_FAILURE;
    }
    
    nRet = WVCI_GUIParseMessage(nSlotIndex, pData, nDataLen);
    return nRet;
}


int WVCI_IsModuleInserted(BYTE ucSlotIndex)
{

    // CHECK PARAMS
    if (ucSlotIndex >= WVCI_MAX_SLOT_NUM)
    {
        return 0;
    }

    if (g_CIMax_CIHandles[ucSlotIndex].emSlotState != CIMAX_EMPTY)
    {
        return 1;
    }
    
    return 0;
}

int WVCI_IsModuleInited(BYTE ucSlotIndex)
{
    // VARS

    // CHECK PARAMS
    if (ucSlotIndex >= WVCI_MAX_SLOT_NUM)
    {
        return 0;
    }
    
    if (g_CIMax_CIHandles[ucSlotIndex].emSlotState == CIMAX_CISTART)
    {
        return 1;
    }
    
    return 0;
}



// auto reset cam operation
void STATUS_ResetCAM(U8 u8SlotIndex,U8 u8ErrorCode)
{  
    static U16 u16PesErrorCount[WVCI_MAX_SLOT_NUM] = {0};
    static U16 u16InitErrorCount[WVCI_MAX_SLOT_NUM] = {0};
    static U16 u16CommunicationErrorCount[WVCI_MAX_SLOT_NUM] = {0};

    if (u8SlotIndex > 1)
    {
        ////LogPrint(LOG_TYPE_ERR, "[STATUS_ResetCAM] AUTO RESET CAM%d... \r\n", u8SlotIndex);
        return;
    }
    switch (u8ErrorCode)
    {
        case CI_PES_ERROR:
            u16PesErrorCount[u8SlotIndex]++;
            break;
        case CI_INIT_ERROR:
            u16InitErrorCount[u8SlotIndex]++;
            break;
        case CI_COMMUNICATION_ERROR:
            u16CommunicationErrorCount[u8SlotIndex]++;
            break;
        default:
            break;
    }
    CI_LOG_ERR("[STATUS_ResetCAM] slot%d reset for %d,Pes count:%d,Init count:%d,communication count:%d\r\n",
                                        u8SlotIndex,
                                        u8ErrorCode,
                                        u16PesErrorCount[u8SlotIndex],
                                        u16InitErrorCount[u8SlotIndex],
                                        u16CommunicationErrorCount[u8SlotIndex]);
    g_Status_CI_ResetTimes[u8SlotIndex]++;
    g_CIMax_CIHandles[u8SlotIndex].emSlotState = CIMAX_EMPTY;
    s_CAMStatusCapture = 0x01; // cam status change
}

void WVCI_GetCAM_Comm_status(WVAlarm_CI_Status_t * CommStatus,int SlotIndex)
{
    *CommStatus  = CAMCommStatus[SlotIndex];
}


#ifdef ENABLE_CI_PES_CHECKING

#define STREAM_TYPE_VIDEO2      0x01        //ISO/IEC 13818 视频
#define STREAM_TYPE_VIDEO1      0x01        //ISO/IEC 11172     视频
#define STREAM_TYPE_VIDEO       0x02        //ISO/IEC 13818-2   视频
#define STREAM_TYPE_VIDEO_ATSC  0x80        //ATSC 视频
#define STREAM_TYPE_VIDEO_H264  0x1B        //H264 视频
#define STREAM_TYPE_VIDEO_MPEG4 0x10        //MPEG4 视频
#define STREAM_TYPE_VIDEO_AVS   0x42        //AVS 视频
/******************************************************************************
 * WVCI_IsVideoStreamType() - 
 * DESCRIPTION: - 
 *     判断流类型
 * Input  : 
 * Output : 
 * Returns: 为视频则返回TRUE,否则返回FALSE
 * 
 ******************************************************************************/
BOOL WVCI_IsVideoStreamType(U8 u8StreamType)
{
    BOOL    bRet = FALSE;
    
    switch (u8StreamType)
    {
        case STREAM_TYPE_VIDEO:
        case STREAM_TYPE_VIDEO2:
        case STREAM_TYPE_VIDEO_H264:
        case STREAM_TYPE_VIDEO_MPEG4:
        case STREAM_TYPE_VIDEO_ATSC:
        case STREAM_TYPE_VIDEO_AVS:
        {
            bRet = TRUE;
            break;
        }
        
        default: 
        {
            break;
        }
    }

    return bRet;
}

#define STATE_CHECK_PES_INIT        (0)
#define STATE_CHECK_PES_READY       (1)  
#define STATE_CHECK_PES_STOP        (2)   
#define STATE_CHECK_PES_RUNNING     (3)
#define STATE_CHECK_PES_ERROR       (4)


int WVCI_PES_SetEnable(U8 u8Slot, U8 u8Enable)
{
    if (u8Slot >= WVCI_MAX_SLOT_NUM)
    {
        return WVCI_ERROR_PARAMS;
    } 

    g_PES_ENABLE[u8Slot] = u8Enable;

    return WVCI_SUCCESS;
}


/******************************************************************************
 * WVCI_PES_TSBitrateSetEnable() - 
 * DESCRIPTION: - 
 *          1. 根据过CAM的码率确定PES监控是否开启
 *          
 * Input  : u8Slot:                 模块通道
            u8Enable                0: 关闭    1: 开启
 * Output : None    
 * Returns: WVCI_SUCCESS                        操作成功
            WVCI_ERROR_PARAMS                   参数错误
 * 
 ******************************************************************************/
int WVCI_PES_TSBitrateSetEnable(U8 u8Slot, U8 u8Enable)
{
    if (u8Slot >= WVCI_MAX_SLOT_NUM)
    {
        return WVCI_ERROR_PARAMS;
    } 

    g_PESCHECK[u8Slot].u8BitrateEnable = u8Enable;

    return WVCI_SUCCESS;
}

/******************************************************************************
 * WVCI_PES_TSBitrateGetEnable() - 
 * DESCRIPTION: - 
 *          1. 返回当前开关状态
 *          
 * Input  : u8Slot:                 模块通道
 * Output : None    
 * Returns: 0: 关闭
 *          1: 开启
 * 
 ******************************************************************************/
U8 WVCI_PES_TSBitrateGetEnable(U8 u8Slot)
{
    if (u8Slot >= WVCI_MAX_SLOT_NUM)
    {
        return 0;
    } 

    return g_PESCHECK[u8Slot].u8BitrateEnable;
}

/******************************************************************************
 * WVCI_PES_ClearTSMonitor() - 
 * DESCRIPTION: - 如果当前搜索的节目流是处于PES监控状态，则把该TS流的监控关闭，
 *                以防止异常复位CAM，并由此带来的流截止
 *                (CAM初始化的时候会中断过CAM的流)
 *     在每次重新搜索节目时调用
 * Input  : U16 u16TSSource
 * Output : 
 * Returns: 操作成功
 * 
 ******************************************************************************/
void WVCI_PES_ClearTSMonitor(U16 u16TSSource)
{  
    // vars
    U8                      u8Slot = 0;
    
    // IP PROJECT ONLY 1 PES CHECKING MODULE   
    u8Slot = 0;
    WVCI_PES_SetEnable(u8Slot, 0x00);    
}


/******************************************************************************
 * WVCI_PES_SetSlotIndex() - 
 * DESCRIPTION: - 
 *     ... 
 * Input  : 
 * Output : 
 * Returns: 
 * 
 ******************************************************************************/
int WVCI_PES_SetSlotIndex(int nSlot)
{
    g_PES_SLOT = nSlot;
    return WVCI_SUCCESS;
}

/******************************************************************************
 * WVCI_PES_GetSlotIndex() - 
 * DESCRIPTION: - 
 *     ... 
 * Input  : 
 * Output : 
 * Returns: 
 * 
 ******************************************************************************/
int WVCI_PES_GetSlotIndex(void)
{    
    return g_PES_SLOT;
}


/******************************************************************************
 * WVCI_PES_GetSlotState() - 
 * DESCRIPTION: - 
 *     ... 
 * Input  : 
 * Output : 
 * Returns: 
 * 
 ******************************************************************************/
U8 WVCI_PES_GetSlotState(U8 u8Slot)
{  
    if (u8Slot >= WVCI_MAX_SLOT_NUM)
    {
        return 0;
    } 
    
    return g_PESCHECK[u8Slot].u8CheckState;
}

/******************************************************************************
 * WVCI_PES_InitSlot() - 
 * DESCRIPTION: - 
 *     ... 
 * Input  : 
 * Output : 
 * Returns: 
 * 
 ******************************************************************************/
int WVCI_PES_InitSlot(U8 u8Slot)
{
    // 
    if (u8Slot >= WVCI_MAX_SLOT_NUM)
    {
        return WVCI_ERROR_PARAMS;
    }    
    memset(&g_PESCHECK[u8Slot], 0x00, sizeof(PES_CheckInfo_t));
    
    g_PESCHECK[u8Slot].u16PIDList[0] = 0x1FFF;
    g_PESCHECK[u8Slot].u16PIDList[1] = 0x1FFF;
    g_PESCHECK[u8Slot].u16PIDList[2] = 0x1FFF;
    g_PESCHECK[u8Slot].u16PIDList[3] = 0x1FFF;
    g_PESCHECK[u8Slot].u8CheckState  = STATE_CHECK_PES_INIT;

    CI_LOG_DEBG("[WVCI_PES_InitSlot] slot%d, pid: %04X, %04X, %04X, %04X \r\n", u8Slot,
            g_PESCHECK[u8Slot].u16PIDList[0], 
            g_PESCHECK[u8Slot].u16PIDList[1], 
            g_PESCHECK[u8Slot].u16PIDList[2], 
            g_PESCHECK[u8Slot].u16PIDList[3]);

    if (u8Slot == 0)
    {
        g_PES_ENABLE[u8Slot] = gDMPParam.system.CISlot0PESSetting;
    }
    else
    {
        g_PES_ENABLE[u8Slot] = gDMPParam.system.CISlot1PESSetting;
    }    
    return WVCI_SUCCESS;
}

/******************************************************************************
 * WVCI_PES_AddCheckPID2Slot() - 
 * DESCRIPTION: - 
 *     ... 
 * Input  : 
 * Output : 
 * Returns: 
 * 
 ******************************************************************************/
int WVCI_PES_AddCheckPID2Slot(U8 u8Slot, U16 u16PID)
{
    U8     u8Offset = 0;
    
    if (u8Slot >= WVCI_MAX_SLOT_NUM)
    {
        return WVCI_ERROR_PARAMS;
    }  
    
    // IP PROJECT ONLY 1 PES CHECKING MODULE, CAM0 & CAM1 WITH THE SAME PES CHECKING MODULE
    u8Slot = 0; 

    u8Offset = g_PESCHECK[u8Slot].u8PIDOffset;
    // overflow
    if (u8Offset >= CI_PES_MAX_PID_NUM)
    {
        //u8Offset = 0;
        //g_PESCHECK[u8Slot].u8PIDOffset = 0;
        return WVCI_ERROR_OVERFLOW;
    }    
    g_PESCHECK[u8Slot].u16PIDList[u8Offset] = u16PID;
    g_PESCHECK[u8Slot].u8PIDOffset++;

    CI_LOG_DEBG("[WVCI_PES_AddCheckPID2Slot] slot%d, offset:%d, pid:%X \r\n", u8Slot, u8Offset, g_PESCHECK[u8Slot].u16PIDList[u8Offset]);
    
    return WVCI_SUCCESS;
}

/******************************************************************************
 * WVCI_PES_SetPID() - 
 * DESCRIPTION: - 
 *     ... 
 * Input  : 
 * Output : 
 * Returns: 
 * 
 ******************************************************************************/
int WVCI_PES_SetPID(U8 u8Slot)
{
    int     nRet = WVCI_SUCCESS;

    CI_LOG_DEBG("[WVCI_PES_SetPID] slot%d, pid: %04X, %04X, %04X, %04X \r\n", u8Slot,
            g_PESCHECK[u8Slot].u16PIDList[0], 
            g_PESCHECK[u8Slot].u16PIDList[1], 
            g_PESCHECK[u8Slot].u16PIDList[2], 
            g_PESCHECK[u8Slot].u16PIDList[3]);
    if (u8Slot == 0)
    {
        REG_Write(CI_PES_SLOT0_PID0, g_PESCHECK[u8Slot].u16PIDList[0]);
        REG_Write(CI_PES_SLOT0_PID1, g_PESCHECK[u8Slot].u16PIDList[1]);
        REG_Write(CI_PES_SLOT0_PID2, g_PESCHECK[u8Slot].u16PIDList[2]);
        REG_Write(CI_PES_SLOT0_PID3, g_PESCHECK[u8Slot].u16PIDList[3]);
        REG_Write(CI_PES_SLOT0_CLR_STATUS, 1);        
        REG_Write(CI_PES_SLOT0_CLR_STATUS, 0);
    }
    else if (u8Slot == 1)
    {
        REG_Write(CI_PES_SLOT1_PID0, g_PESCHECK[u8Slot].u16PIDList[0]);
        REG_Write(CI_PES_SLOT1_PID1, g_PESCHECK[u8Slot].u16PIDList[1]);
        REG_Write(CI_PES_SLOT1_PID2, g_PESCHECK[u8Slot].u16PIDList[2]);
        REG_Write(CI_PES_SLOT1_PID3, g_PESCHECK[u8Slot].u16PIDList[3]);
        REG_Write(CI_PES_SLOT1_CLR_STATUS, 1);        
        REG_Write(CI_PES_SLOT1_CLR_STATUS, 0);
    }
    else
    {
        nRet = WVCI_ERROR_PARAMS;
    }   
    
    return nRet;
}


/******************************************************************************
 * WVCI_PES_SetSinglePID() - 
 * DESCRIPTION: - 
 *     ... 
 * Input  : 
 * Output : 
 * Returns: 
 * 
 ******************************************************************************/
int WVCI_PES_SetSinglePID(U8 u8Slot, U16 u16PID)
{
    int     nRet = WVCI_SUCCESS;

    if (u8Slot >= WVCI_MAX_SLOT_NUM)
    {
        return WVCI_ERROR_PARAMS;
    }
    
    //printf ("[WVCI_PES_SetSinglePID] slot%d, pid: %04X \r\n", u8Slot, u16PID);
    if (u8Slot == 0)
    {
        REG_Write(CI_PES_SLOT0_PID0, u16PID);
        REG_Write(CI_PES_SLOT0_PID1, 0x1FFF);
        REG_Write(CI_PES_SLOT0_PID2, 0x1FFF);
        REG_Write(CI_PES_SLOT0_PID3, 0x1FFF);
        REG_Write(CI_PES_SLOT0_CLR_STATUS, 1);        
        REG_Write(CI_PES_SLOT0_CLR_STATUS, 0);
    }
    else if (u8Slot == 1)
    {
        REG_Write(CI_PES_SLOT1_PID0, u16PID);
        REG_Write(CI_PES_SLOT1_PID1, 0x1FFF);
        REG_Write(CI_PES_SLOT1_PID2, 0x1FFF);
        REG_Write(CI_PES_SLOT1_PID3, 0x1FFF);
        REG_Write(CI_PES_SLOT1_CLR_STATUS, 1);        
        REG_Write(CI_PES_SLOT1_CLR_STATUS, 0);
    }    
    return nRet;
}

/******************************************************************************
 * WVCI_PES_ClrStatus() - 
 * DESCRIPTION: - 
 *     ... 
 * Input  : 
 * Output : 
 * Returns: 
 * 
 ******************************************************************************/
int WVCI_PES_ClrStatus(U8 u8Slot)
{
    if (u8Slot == 0)
    {
        REG_Write(CI_PES_SLOT0_CLR_STATUS, 1);        
        REG_Write(CI_PES_SLOT0_CLR_STATUS, 0);
    }
    else
    {
        REG_Write(CI_PES_SLOT1_CLR_STATUS, 1);        
        REG_Write(CI_PES_SLOT1_CLR_STATUS, 0);
    }
    return WVCI_SUCCESS;
}


/******************************************************************************
 * WVCI_PES_GetStatus() - 
 * DESCRIPTION: - 
 *     ... 
 * Input  : 
 * Output : 
 * Returns: 
 * 
 ******************************************************************************/
int WVCI_PES_GetStatus(U8 u8Slot)
{
    U32 u32Status = 0;   

    if (u8Slot == 0)
    {        
        REG_Read(CI_PES_SLOT0_STATUS, &u32Status);
    }
    else
    {
        REG_Read(CI_PES_SLOT1_STATUS, &u32Status);
    }

    return (u32Status&0x01);
}

/******************************************************************************
 * WVCI_PES_Checking() - 
 * DESCRIPTION: - 
 *     ... 
 * Input  : 
 * Output : 
 * Returns: 
 * 
 ******************************************************************************/
int WVCI_PES_Checking(void)
{
    int     nRet = WVCI_SUCCESS;
    U8      u8Slot = 0;
    int     nStatus = 0;
    U8      u8StatePIDOffset = 0;

    // IP PROJECT ONLY 1 PES CHECKING MODULE
    //for (u8Slot = 0; u8Slot < WVCI_MAX_SLOT_NUM; u8Slot++)    
    u8Slot = 0;
    {
        // no checking if disable
        if (g_PES_ENABLE[u8Slot] != 0x01                    ||      // 软件控制开关
            g_PESCHECK[u8Slot].u8BitrateEnable != 0x01)             // 码率控制开关: 只有两个开关均打开时,才进行PES检测
        {
            g_PESCHECK[u8Slot].u8CheckState = STATE_CHECK_PES_INIT;
            //continue;
            return WVCI_SUCCESS;
        }
        
        // checking if ready
        if (g_PESCHECK[u8Slot].u8Ready == 0x01)
        {
            switch(g_PESCHECK[u8Slot].u8CheckState)
            {
                case STATE_CHECK_PES_INIT:
                {  
                    WVCI_PES_SetSinglePID(u8Slot, 0x1FFF);                    
                    nStatus = WVCI_PES_GetStatus(u8Slot);
                    nRet    = WVCI_PES_ClrStatus(u8Slot); 
                    g_PESCHECK[u8Slot].u8CheckState = STATE_CHECK_PES_READY;
                    g_PESCHECK[u8Slot].u8StatePIDOffset = 0;
                    CI_LOG_INF("PES STATE%d: STATE_CHECK_PES_INIT  ==>READY \r\n", u8Slot);
                    break;
                }
                case STATE_CHECK_PES_READY:
                {  
                    //
                    nStatus = WVCI_PES_GetStatus(u8Slot);
                    if (nStatus == 0)
                    {
                        g_PESCHECK[u8Slot].u8StateCnt++;
                        
                        u8StatePIDOffset = g_PESCHECK[u8Slot].u8StatePIDOffset;
                        if (u8StatePIDOffset >= CI_PES_MAX_PID_NUM)
                        {
                            // set all pid and will come into checking
                            CI_LOG_INF("PES STATE%d: STATE_CHECK_PES_READY  ==>STOP \r\n", u8Slot);
                            nRet = WVCI_PES_SetPID(u8Slot);         
                            nRet = WVCI_PES_ClrStatus(u8Slot); 
                            g_PESCHECK[u8Slot].u8CheckState = STATE_CHECK_PES_STOP;
                        }
                        else if (g_PESCHECK[u8Slot].u16PIDList[u8StatePIDOffset] != 0x1FFF)
                        {
                            // set "state_offset" pid to check ok or not
                            nRet = WVCI_PES_SetSinglePID(u8Slot, g_PESCHECK[u8Slot].u16PIDList[u8StatePIDOffset]);                        
                            nRet = WVCI_PES_ClrStatus(u8Slot); 
                            OSTimeDlyHMSM(0, 0, 0, 100);
                        }
                        if (g_PESCHECK[u8Slot].u8StateCnt > 3)
                        {
                            g_PESCHECK[u8Slot].u8StatePIDOffset++;
                        }
                    }
                    else
                    {    
                        g_PESCHECK[u8Slot].u8StateCnt = 0;
                        nRet    = WVCI_PES_ClrStatus(u8Slot);
                        CI_LOG_DEBG("PES STATE%d: STATE_CHECK_PES_READY  ==>stay(%d)\r\n", u8Slot, g_PESCHECK[u8Slot].u8StatePIDOffset);
                    }
                    
                    break;
                }
                case STATE_CHECK_PES_STOP:
                {
                    nStatus = WVCI_PES_GetStatus(u8Slot);
                    if (nStatus == 0)
                    {
                        g_PESCHECK[u8Slot].u8CheckState = STATE_CHECK_PES_RUNNING;
                        CI_LOG_INF("PES STATE%d: STATE_CHECK_PES_STOP  ==>RUNNING \r\n", u8Slot);
                    }
                    else
                    {    
                        g_PESCHECK[u8Slot].u8StateCnt = 0;
                        nRet    = WVCI_PES_ClrStatus(u8Slot);
                        CI_LOG_DEBG("PES STATE%d: STATE_CHECK_PES_STOP  ==>stay \r\n", u8Slot);
                    }

                    break;
                }
                
                case STATE_CHECK_PES_RUNNING:
                {
                    nStatus = WVCI_PES_GetStatus(u8Slot);
                    if (nStatus == 0)
                    {
                        g_PESCHECK[u8Slot].u8StateCnt = 0;
                        //printf ("STATE%d: STATE_CHECK_PES_RUNNING  ==>stay \r\n", u8Slot);                   
                    }
                    else
                    { 
                        nRet    = WVCI_PES_ClrStatus(u8Slot);                   
                        g_PESCHECK[u8Slot].u8StateCnt++;                      
                        // 10:是个经验值:过小会导致无信号输入时,误判为异常. 过大会导致触发复位过久或根本不解决. 目前自测情况
                        // 测试惠州有线实时信号处理正常
                        // CONAX 测试流(CD1 ts1.trp)正常
                        // CTI测试流(T01_regstbscno48keysepg.ts)触发不成功
                        if (g_PESCHECK[u8Slot].u8StateCnt > 10)     
                        {                            
                            g_PESCHECK[u8Slot].u8CheckState = STATE_CHECK_PES_ERROR;  
                            CI_LOG_INF("PES STATE%d: STATE_CHECK_PES_RUNNING  ==>ERROR \r\n", u8Slot);
                        }
                    }
                    break;
                }
                case STATE_CHECK_PES_ERROR:
                {
                    #ifdef ENABLE_CI_PES_CHECKING
                    WVCI_PES_InitSlot(u8Slot);
                    #endif                    
                    g_PESCHECK[u8Slot].u8StateCnt = 0;
                    g_PESCHECK[u8Slot].u8CheckState = STATE_CHECK_PES_INIT; 
                    CI_LOG_INF("PES STATE%d: STATE_CHECK_PES_ERROR  ==>INIT \r\n", u8Slot);
                    STATUS_ResetCAM(u8Slot,CI_PES_ERROR);                   
                    break;
                }                
                default: break;
            }
        }
    }
    return WVCI_SUCCESS;
}

#endif


BOOL WVCI_IsCASysIdInList(U8 u8Slot, WORD wCASysId)
{
    int i;
    if(u8Slot > 1)
    {
        return FALSE;
    }
//  The spec does not mention any requirement
//  to filter the PMT based on the reported
//  ca system ids, but really a lot of modules
//  require this to function properly.
//  if(!hCI->fModuleRequiresCAIDFiltering)
//      return TRUE;
    for(i=0;i<CiSlot[u8Slot].wNumCASystems;i++)
    {
        if(CiSlot[u8Slot].wCASystemIDs[i]==wCASysId)
        {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL WVCI_IsStreamTypeInList(U8 u8StreamType)
{
    int k;
    
    for(k=0; g_pDefaultStreamTypeList[k] != 0x00; k++)
    {
        if(u8StreamType == g_pDefaultStreamTypeList[k])
        {
            return TRUE;
        }
    }

    return FALSE;
}

#ifdef ENABLE_CHECK_DECODER_ESLIST_2DESCRAMBLE                      // 香港客户，旧CAM，单个节目只支持10个ES
#define WVCI_SERVICE_MAX_ES_NUM                     (32)            // 单个节目最大支持ES个数
typedef struct 
{
    U16         u16ServiceID;                                       // playing service id
    U16         u16ServiceOffset;                                   // input programlist offset    
    U16         u16Reserved;                                        //    
    U16         u16PlayingESNum;                                    // playing es number
    U16         u16PlayingESPID[WVCI_SERVICE_MAX_ES_NUM];           // playing es pid list
} DecoderPlayingESInfo_t;


static S16                          s_s16DescrambleESNum    = 0;    // Current service descramble es number
static S16                          s_s16HadAddPlayingESNum = 0;    // Current service descramble playing es number
static DecoderPlayingESInfo_t       s_DecoderESInfo;                // Current playing service info

static U8 WVCI_GetCAMMaxSupportedESNum(U8 u8Slot)
{
    // vars
    U8          u8CAMMaxSupportedESNum  = 0;

    // check params
    if (u8Slot < WVCI_MAX_SLOT_NUM)
    {
        u8CAMMaxSupportedESNum = ((gDMPParam.system.u16CAMMaxESNumber >> (u8Slot * 8)) & 0xFF);        
    }
    
    if (0 == u8CAMMaxSupportedESNum)
    {
        // 如果参数中没有指定CAM单个节目最大支持ES数目，默认32
        u8CAMMaxSupportedESNum = WVCI_SERVICE_MAX_ES_NUM;       
    }
    
    return u8CAMMaxSupportedESNum;
}


/******************************************************************************
 * WVCI_GetCurrPlayingServiceOffset() - 
 * DESCRIPTION: - Get current playing service offset of ProgramList.
 *     ... 
 * Input  : void
 * Output : void
 * Returns: Offset
 * 
 ******************************************************************************/
static U16 WVCI_GetCurrPlayingServiceOffset(void)
{
    // vars
    U16                         u16DecoProgIndex    = 0;
    U16                         ProgramListOffset   = 0;
    U8                          u8CurTSIdx          = 0;
    
#if defined(ENABLE_PROGRAM_BACKUP)
    u8CurTSIdx = R2_GetInputTS_Index(R2_GetInputPort());
#endif        

    ProgramListOffset =  gDMPParam.ts.InputTS[u8CurTSIdx].ProgramListOffset;
    u16DecoProgIndex = 0;
    if((ProgramListOffset+gDMPParam.Decoder.ProgramConfig[0].PlayingService.wServiceIndex)<gDMPParam.ts.InputProgOffset)
    {
        u16DecoProgIndex = ProgramListOffset+gDMPParam.Decoder.ProgramConfig[0].PlayingService.wServiceIndex;
    }

    return u16DecoProgIndex;
}


/******************************************************************************
 * WVCI_GetPlayingServiceInfo() - 
 * DESCRIPTION: - Get current playing service info. Including video, audio,
 *                sutitle and teletext.
 * Input  : void
 * Output : void
 * Returns: WVCI_SUCCESS                success
 *          WVCI_ERROR_PARAMS           bad parameters
 * 
 ******************************************************************************/
static U32  WVCI_GetPlayingServiceInfo(void)
{
    // vars  
    DECO_Main_Params_t     *hDecoMainParams     = NULL; 

    // get decoder paras handle
    hDecoMainParams = MBOARD_GetDecoderParamsHandle();
    if(hDecoMainParams == NULL)
    {
        CI_LOG_ERR("[WVCI_GetPlayingServiceInfo] GET DECODE MAIN PARAMS FAILURE !!!!\r\n");
        return WVCI_ERROR_PARAMS;
    }
    
    // 
    memset(&s_DecoderESInfo, 0x00, sizeof(DecoderPlayingESInfo_t));

    // get service offset    
    s_DecoderESInfo.u16ServiceOffset = WVCI_GetCurrPlayingServiceOffset();    
    // get decoder service id
    s_DecoderESInfo.u16ServiceID     = hDecoMainParams->ProgramConfig[0].PlayingService.wServiceId;
    // get decoder playing es list
    if ((hDecoMainParams->ProgramConfig[0].PlayingService.wVideoPID != 0x0000)  &&
        (hDecoMainParams->ProgramConfig[0].PlayingService.wVideoPID != 0x1FFF))
    {
        s_DecoderESInfo.u16PlayingESPID[s_DecoderESInfo.u16PlayingESNum] 
                        = hDecoMainParams->ProgramConfig[0].PlayingService.wVideoPID;
        s_DecoderESInfo.u16PlayingESNum++;
    }

    if ((hDecoMainParams->ProgramConfig[0].PlayingService.wAudioPID != 0x0000)  &&
        (hDecoMainParams->ProgramConfig[0].PlayingService.wAudioPID != 0x1FFF))
    {
        s_DecoderESInfo.u16PlayingESPID[s_DecoderESInfo.u16PlayingESNum] 
                        = hDecoMainParams->ProgramConfig[0].PlayingService.wAudioPID;
        s_DecoderESInfo.u16PlayingESNum++;
    }

    if ((hDecoMainParams->ProgramConfig[0].PlayingService.wAudio1PID != 0x0000)  &&
        (hDecoMainParams->ProgramConfig[0].PlayingService.wAudio1PID != 0x1FFF))
    {
        s_DecoderESInfo.u16PlayingESPID[s_DecoderESInfo.u16PlayingESNum] 
                        = hDecoMainParams->ProgramConfig[0].PlayingService.wAudio1PID;
        s_DecoderESInfo.u16PlayingESNum++;
    }    

    if ((hDecoMainParams->ProgramConfig[0].PlayingService.SubtitlePID != 0x0000)  &&
        (hDecoMainParams->ProgramConfig[0].PlayingService.SubtitlePID != 0x1FFF))
    {
        s_DecoderESInfo.u16PlayingESPID[s_DecoderESInfo.u16PlayingESNum] 
                        = hDecoMainParams->ProgramConfig[0].PlayingService.SubtitlePID;
        s_DecoderESInfo.u16PlayingESNum++;
    }
    
    if ((hDecoMainParams->ProgramConfig[0].PlayingService.TtxPID != 0x0000)  &&
        (hDecoMainParams->ProgramConfig[0].PlayingService.TtxPID != 0x1FFF))
    {
        s_DecoderESInfo.u16PlayingESPID[s_DecoderESInfo.u16PlayingESNum] 
                        = hDecoMainParams->ProgramConfig[0].PlayingService.TtxPID;
        s_DecoderESInfo.u16PlayingESNum++;
    }

    #if 0
    for (ii = 0; ii < s_DecoderESInfo.u16PlayingESNum; ii++)
    {
        CI_LOG_ERR("[WVCI_GetPlayingServiceInfo] ES%d: 0x%04X \r\n", 
                ii, s_DecoderESInfo.u16PlayingESPID[ii]);
    }
    #endif

    return WVCI_SUCCESS;
}


/******************************************************************************
 * WVCI_IsServicePlaying() - 
 * DESCRIPTION: - Check service is in playing or not
 * Input  : u16ServiceOffset            offset of service of program list
 * Output : void
 * Returns: TRUE  or FALSE
 * 
 ******************************************************************************/
static BOOL WVCI_IsServicePlaying(U16 u16ServiceOffset)
{
    if (u16ServiceOffset == s_DecoderESInfo.u16ServiceOffset)
    {    
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/******************************************************************************
 * WVCI_IsESinPlaying() - 
 * DESCRIPTION: - Check es is in playing or not
 * Input  : u16ESPID                    es pid 
 * Output : void
 * Returns: TRUE  or FALSE
 * 
 ******************************************************************************/
static BOOL WVCI_IsESinPlaying(U16 u16ESPID)
{
    // vars
    int     ii = 0;    

    //
    for (ii = 0; ii < s_DecoderESInfo.u16PlayingESNum; ii++)
    {
        if (u16ESPID == (s_DecoderESInfo.u16PlayingESPID[ii] & 0x1FFF))
        {
            return TRUE;
        }       
    }
    
    return FALSE;
}



/******************************************************************************
 * WVCI_IsPositionEnough() - 
 * DESCRIPTION: - Check there is enough position for other ES to be descramble, 
                  for which is not in playing. case priority playing es.
 * Input  : u8Slot                    slot id of cam. 
 * Output : void
 * Returns: TRUE  or FALSE
 * 
 ******************************************************************************/
static BOOL WVCI_IsPositionEnough(U8 u8Slot)
{
    // vars 
    int         nLeftPosition           = 0;
    U8          u8CAMMaxSupportedESNum  = 0;
    U16         u16DecoderPlayingESNum  = 0;
    int         nInallUsedPosition      = 0;
    int         nNeedReservedPosition   = 0;

    //
    u8CAMMaxSupportedESNum      = WVCI_GetCAMMaxSupportedESNum(u8Slot);
    nInallUsedPosition          = s_s16DescrambleESNum;
    u16DecoderPlayingESNum      = s_DecoderESInfo.u16PlayingESNum;
    nNeedReservedPosition       = (u16DecoderPlayingESNum-s_s16HadAddPlayingESNum);
    CI_LOG_DEBG("[WVCI_IsPositionEnough] (max-supported:%d, inall-used:%d, needReserved:%d ) \r\n", 
                    u8CAMMaxSupportedESNum,
                    nInallUsedPosition,
                    nNeedReservedPosition); 
        
    // Left = MaxSupported - InallUsed - NeedReserved
    nLeftPosition = u8CAMMaxSupportedESNum - nInallUsedPosition - nNeedReservedPosition;    
    if (nLeftPosition > 0)
    {
        return TRUE;
    }
    else 
    {
        return FALSE;
    }
}




/******************************************************************************
 * WVCI_IsESOverflow() - 
 * DESCRIPTION: - Check es is overflow or not. 
 *                More than cam's capacity is as overflow.
 * Input  : u8Slot                  slot id of cam. 
 * Output : void
 * Returns: TRUE  or FALSE
 * 
 ******************************************************************************/
static BOOL WVCI_IsESOverflow(U8 u8Slot)
{
    // vars 
    U8          u8CAMMaxSupportedESNum = 0;

    // check params
    if (u8Slot >= WVCI_MAX_SLOT_NUM)
    {
        return TRUE; // bad parameters
    }

    // u32CAMMaxESNumber: cam capacity is set by user on debug page. 
    u8CAMMaxSupportedESNum = WVCI_GetCAMMaxSupportedESNum(u8Slot);
    
    if (s_s16DescrambleESNum < u8CAMMaxSupportedESNum)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}


/******************************************************************************
 * WVCI_IsESNeed2Descramble() - 
 * DESCRIPTION: - Check es is should be add into descramble.
 *              1) check current service's es-list is overflow or not
 *              2) check current service is be playing in decoder or not
 *              3) check current es is be playing or not
 *              4) check there is enough space to add other es which is no playing.
 *               
 * Input  : u8Slot                  slot id of cam. 
 *          u16ServiceOffset        service offset in program list
 *          u16ESPID                pid of es be checked
 * Output : void
 * Returns: TRUE  or FALSE
 * 
 ******************************************************************************/
static BOOL WVCI_IsESNeed2Descramble(U8 u8Slot, U16 u16ServiceOffset, U16 u16ESPID)
{ 
    // vars

    // check params
    if (u8Slot >= WVCI_MAX_SLOT_NUM)
    {
        return FALSE; // bad parameters
    }
    
    // check service's es-list is overflow or not
    if (WVCI_IsESOverflow(u8Slot))
    {
        // when es number is overflow, skip this es, no add into CAPMT, 
        // or some cam system maybe halt on. 
        // when service's es-list is more than cam's captiliy, will skip the end party of es-list.         
        // Eg. Hongkong Customer-SMIT-CAM(only supported 10 es)
        CI_LOG_ERR("[WVCI_IsESNeed2Descramble] skip this es, no add into CAPMT. u16ESPID:0x%X. --overflow \r\n", u16ESPID); 
        return FALSE;       
    }

    // check service is playing or not
    if (!WVCI_IsServicePlaying(u16ServiceOffset))
    {
        s_s16DescrambleESNum++;
        // service is not in playing, so add the es in order.
        CI_LOG_ERR("[WVCI_IsESNeed2Descramble] descramble u16ESPID:0x%X. --in order \r\n", u16ESPID); 
        return TRUE;
    }

    // service is in playing, descram the playing es first
    // if is playing es, give priority to add 
    if (WVCI_IsESinPlaying(u16ESPID))
    {
        s_s16DescrambleESNum++;
        s_s16HadAddPlayingESNum++;
        CI_LOG_ERR("[WVCI_IsESNeed2Descramble] descramble u16ESPID:0x%X. --playing \r\n", u16ESPID); 
        return TRUE;
    }
    // if left position is enough, can be add
    else if (WVCI_IsPositionEnough(u8Slot))
    {
        s_s16DescrambleESNum++;
        CI_LOG_ERR("[WVCI_IsESNeed2Descramble] descramble u16ESPID:0x%X. --left enough\r\n", u16ESPID);         
        return TRUE;
    }
    // not in playing and no enough space, skip.
    else 
    {
        CI_LOG_ERR("[WVCI_IsESNeed2Descramble] skip u16ESPID:0x%X. --skip\r\n", u16ESPID); 
        return FALSE; // skip this es pid to descramble
    }        
}



/******************************************************************************
 * WVCI_IsPlayingServiceInDescrambled() - 
 * DESCRIPTION: - check playing service is descramble by specified slot
 * Input  : u8Slot                  slot id of cam. 
 * Output : void
 * Returns: TRUE  or FALSE
 * 
 ******************************************************************************/
BOOL WVCI_IsPlayingServiceInDescrambled(U8 u8Slot)
{
    // vars
    U16         u16ServiceOffset = 0;

    // check params
    if (u8Slot >= WVCI_MAX_SLOT_NUM)
    {
        return FALSE; // bad parameters
    }
    
    //
    u16ServiceOffset = WVCI_GetCurrPlayingServiceOffset();
    if (0x00 == u8Slot)
    {
        if ((gDMPParam.ts.InputProgList[u16ServiceOffset].DescrambleFlag & 0x01) == 0x01)
        {            
            return TRUE;
        }
    }
    else
    {
        if ((gDMPParam.ts.InputProgList[u16ServiceOffset].DescrambleFlag & 0x02) == 0x02)
        {
            return TRUE;
        }
    }

    return FALSE;
}

#endif  // #ifdef ENABLE_CHECK_DECODER_ESLIST_2DESCRAMBLE


/******************************************************************************
 * WVCI_GenerateCAPMT() - 按标准生成节目的CAPMT
 * DESCRIPTION: - 
 *          1. 处理节目描述信息
 *          2. 处理节目下所有与解扰有关的ES描述信息
 *          
 * Input  : u8Slot:                 模块通道
            u16InProgIndex          节目索引
            pstParamTS              系统流结构指针
            eMode                   CAPMT模式(CAPMTLISTMANAGEMENT类型)
 * Output : None    
 * Returns: WVCI_SUCCESS                        操作成功
            WVCI_FAILURE                        操作失败
 * 
 ******************************************************************************/
int WVCI_GenerateCAPMT(U8 u8Slot, U16 u16OutProgIndex, TSInfo *pstParamTS, U8 eMode)
{
    int  nRet = 0;
    U8   CAPMTBuf[MAX_LEN_CA_PMT];
    WORD wSizePtr = 0;
    int nLoopSizePtr = 0;
    int nTotalLen = 0;
    int nLoopLen = 0;
    BYTE bMatchedDescriptors = 0;
    
    int i = 0;
    int j = 0;
    int nRootDescriptorCount = 0;
    U8 *pBuf = NULL;
    int nProgInfoLen = 0;
    U16 u16CASystemID = 0; 

    U16 u16ESPID = 0;
    U8  u8ESStreamType = 0;

    U16 u16ESDescLen = 0;
    U8 *pu8ESDesc = NULL;

    Output_TSProgram *pOutputProg = NULL;
    Output_TSPID *pOutputPID = NULL;
    IndexInfoList stIndexList;

    if ((!pstParamTS) || (pstParamTS->u32OutputProgramNumber <= u16OutProgIndex))
    {
        CI_LOG_ERR("[WVCI_GenerateCAPMT]Input Error,pstParamTS[%p],u16OutProgIndex[%u]\r\n", pstParamTS, u16OutProgIndex);
        return WVCI_ERROR_PARAMS;
    }

    #if 0
    // 检查CAM是否初始化成功
    if (0 == WVCI_IsModuleInited(u8Slot))
    {
        CI_LOG_ERR("[WVCI_GenerateCAPMT] Slot%d no inited, skip generate pmts...\r\n", u8Slot);
        return WVCI_SUCCESS;
    }
    #endif

    pOutputProg = &pstParamTS->pOutputProgram[u16OutProgIndex];

    // 初始化
    memset(CAPMTBuf, 0x00, sizeof(CAPMTBuf));

    CAPMTBuf[0] = 0x9F;
    CAPMTBuf[1] = 0x80;
    CAPMTBuf[2] = 0x32;
    wSizePtr = 3;
    
    // 1 byte size
    nTotalLen = 4;      // 5 for long size
    CAPMTBuf[3] = 0;    // size comes here
    if (CA_PMT_LIST_MANAGEMENT_WELLAV_DESCRAMBLE != eMode)
    {
        CAPMTBuf[nTotalLen++] = (BYTE)(eMode);
    }
    // stop descramble
    else
    {
        // 指定为仅仅解扰一个节目，同时后面的ES ECM描述为一个与节目无关的PID
        // 否则如果使用CA_PMT_CMD_ID_NOT_SELECTED参数，部分CAM还是不能满足停止解扰
        CAPMTBuf[nTotalLen++] = CA_PMT_LIST_MANAGEMENT_ONLY;
    }

    // program number
    CAPMTBuf[nTotalLen++] = (pOutputProg->u16ServiceID >> 8) & 0xFF;
    CAPMTBuf[nTotalLen++] = pOutputProg->u16ServiceID & 0xFF;

    // 使用搜索节目时记录的PMT Version
    CAPMTBuf[nTotalLen++] = pOutputProg->u8PMTVersionNumber;
    nLoopSizePtr = nTotalLen;
    
    CAPMTBuf[nTotalLen++] = 0;  // size comes here
    CAPMTBuf[nTotalLen++] = 0;  // size comes here

    pBuf = pOutputProg->aru8ProgramInfo;
    nProgInfoLen = pOutputProg->u16ProgramInfoLength;
    nLoopLen = 0;
    i = 0;
    
    while (CI_ProInfo_GetDescriptor(pBuf, nProgInfoLen, 0x09, i))
    {
        BYTE bSize;
        const BYTE *pDesc = CI_ProInfo_GetDescriptor(pBuf, nProgInfoLen, 0x09, i);
        
        nRootDescriptorCount++;
        u16CASystemID = ((pDesc[2] << 8) | pDesc[3]); 
        if (WVCI_IsCASysIdInList(u8Slot, u16CASystemID))
        {
            if (bMatchedDescriptors == 0) 
            {
                CAPMTBuf[nTotalLen] = CA_PMT_CMD_ID_OK_DESCRAMBLING;   //CA_PMT_CMD_ID_OK_DESCRAMBLING     = 0x01, //!< just descramble
                nTotalLen++;
                nLoopLen++;
            }
            
            bSize = (BYTE)(pDesc[1] + 2);
            if ((nTotalLen+bSize) >= MAX_LEN_CA_PMT)
            {
                CI_LOG_ERR("[WVCI_GenerateCAPMT] line:%d Data Overflow(%d)...\r\n", __LINE__, nTotalLen+bSize);            
                return WVCI_ERROR_OVERFLOW;
            }
            
            memcpy(&CAPMTBuf[nTotalLen], pDesc, bSize);
            nTotalLen += bSize;
            nLoopLen += bSize;
            bMatchedDescriptors++;
        }
        
        i++;
    }

    CAPMTBuf[nLoopSizePtr] = (BYTE)(0xF0 | (nLoopLen >> 8)); // reserved bits + upper part of the loop size
    CAPMTBuf[nLoopSizePtr + 1] = (BYTE)(nLoopLen & 0xFF);
    nLoopLen = 0;

    stIndexList.u16IndexListNum = 0;
    (void)TSP_GetOutProgESPIDIndexList(u16OutProgIndex, pstParamTS, &stIndexList);

    for (i = 0; i < stIndexList.u16IndexListNum; i++)
    {
        pOutputPID = &pstParamTS->pOutputPID[stIndexList.aru16IndexList[i]];
        u16ESPID = pOutputPID->u16PID;
        
        u8ESStreamType = pOutputPID->u8StreamType;
        u16ESDescLen = pOutputPID->u16ESInfoLength;
        pu8ESDesc = pOutputPID->aru8ESInfo;

        if (!WVCI_IsStreamTypeInList(u8ESStreamType))
        {
            continue;
        }

        if (nTotalLen >= (MAX_LEN_CA_PMT - 5))
        {
            CI_LOG_ERR("[WVCI_GenerateCAPMT] line:%d Data Overflow(%d)...\r\n", __LINE__, nTotalLen);            
            return WVCI_ERROR_OVERFLOW;
        }
        
        CAPMTBuf[nTotalLen] = u8ESStreamType;
        if (CA_PMT_LIST_MANAGEMENT_WELLAV_DESCRAMBLE != eMode)
        {                
            CAPMTBuf[nTotalLen+1] = (BYTE)(0xE0 | (u16ESPID>>8)); // reserved bits + upper part of the pid
            CAPMTBuf[nTotalLen+2] = (BYTE)(u16ESPID&0xFF);

        }
        // 停止解扰
        else
        {
            u16ESPID = 0x1FF5; //8181 ::　if the last ES's ECM pid is 0x1FF5, it can not stop ES descramble... 
            CAPMTBuf[nTotalLen+1] = (BYTE)(0xE0 | (u16ESPID>>8)); // reserved bits + upper part of the pid
            CAPMTBuf[nTotalLen+2] = (BYTE)(u16ESPID&0xFF);
        }
        nLoopSizePtr = nTotalLen+3;
        CAPMTBuf[nTotalLen+3] = 0; // looplen
        CAPMTBuf[nTotalLen+4] = 0;
        nTotalLen+=5;
        nLoopLen=0;
        bMatchedDescriptors = 0;

        j = 0;
        while(CI_ESInfo_GetESDescriptor(pu8ESDesc,u16ESDescLen,0x09,j))
        {
            BYTE bSize;
            const BYTE *pDesc = CI_ESInfo_GetESDescriptor(pu8ESDesc,u16ESDescLen,0x09,j);
            u16CASystemID = ((pDesc[2]<<8)| pDesc[3]);
            if (WVCI_IsCASysIdInList(u8Slot, u16CASystemID))
            {
                if(bMatchedDescriptors==0) 
                {
                    CAPMTBuf[nTotalLen] = CA_PMT_CMD_ID_OK_DESCRAMBLING;  //CA_PMT_CMD_ID_OK_DESCRAMBLING     = 0x01, //!< just descramble
                    nTotalLen++;
                    nLoopLen++;
                }
                
                //WVCI_InfoPrint(("[WVCI_GenerateCAPMT]  Descriptor %02X Len %02X (%02X%02X %02X%02X)\r\n",pDesc[0],pDesc[1],pDesc[2],pDesc[3],pDesc[4],pDesc[5]));
                bSize=(BYTE)(pDesc[1]+2);
                if ((nTotalLen+bSize) >= MAX_LEN_CA_PMT)
                {
                    CI_LOG_ERR("[WVCI_GenerateCAPMT]line:%d Data Overflow(%d)...\r\n", __LINE__, nTotalLen+bSize);            
                    return WVCI_ERROR_OVERFLOW;
                }
                memcpy(&CAPMTBuf[nTotalLen],pDesc,bSize);
                nTotalLen+=bSize;
                nLoopLen+=bSize;
                bMatchedDescriptors++;
            } 
            //else 
            //{
            //    WVCI_InfoPrint(("[WVCI_GenerateCAPMT] Discarding CA Descriptor due to CASysId Filtering (ES CA SYSTEM ID: 0x%04X)\r\n", (pDesc[2]<<8)|pDesc[3]));
            //}
            
            j++;
        }   
        CAPMTBuf[nLoopSizePtr] = (BYTE)(0xF0 | (nLoopLen>>8)); // reserved bits + upper part of the loop size
        CAPMTBuf[nLoopSizePtr+1] = (BYTE)(nLoopLen&0xFF);
    }

    // fix the complete size
    CAPMTBuf[wSizePtr] = (BYTE)(nTotalLen-4);

    // Large PMT handling
    //length_field
    if (((nTotalLen-4) > 127) && ((nTotalLen-4) < 255))
    {
        int i;
        for (i=nTotalLen;i>3;i--)
            CAPMTBuf[i+1] = CAPMTBuf[i];
        CAPMTBuf[3] = 0x81;             
        CAPMTBuf[4] = (BYTE)(nTotalLen-4);
        nTotalLen++;
    }   
    else if ((nTotalLen-4) >= 255)
    {
        int i;
        for (i=nTotalLen;i>3;i--)
            CAPMTBuf[i+2] = CAPMTBuf[i];
        CAPMTBuf[3] = 0x82;             
        CAPMTBuf[4] = (BYTE)((nTotalLen-4)>>8);
        CAPMTBuf[5] = (BYTE)((nTotalLen-4)&0xFF);
        nTotalLen+=2;
    }   

    // CAPMT长度不太可能大于1020
    if (nTotalLen < (MAX_LEN_CA_PMT - 4))
    {
        CAPMTBuf[nTotalLen+0] = 0x47;
        CAPMTBuf[nTotalLen+1] = 0x47;
        CAPMTBuf[nTotalLen+2] = 0x47;
        CAPMTBuf[nTotalLen+3] = 0x47;        
        nTotalLen += 4;
    }

    nRet = CI_SendCAPMT2CAM(u8Slot, CAPMTBuf, nTotalLen);
    
    // recoder the last capmt offset
    s_u16LastDescrProgIndex[u8Slot] = u16OutProgIndex;
    CI_LOG_INF("[WVCI_GenerateCAPMT] Slot%d,u16OutProgIndex:%02d,eMode:%02d setting...(%d)\r\n", 
                                            u8Slot, 
                                            u16OutProgIndex, 
                                            eMode, 
                                            nRet);
    
    return WVCI_SUCCESS;
}

int WVCI_SetTSBypassass2CAM(U8 u8Slot, U32 u32StreamID)
{
    if (CI_SLOT0 == u8Slot)
    {
        FPGA_setCI0Sid(u32StreamID);
    }
    else if (CI_SLOT1 == u8Slot)
    {
        FPGA_setCI1Sid(u32StreamID);
    }

    return WVCI_SUCCESS;
}

int WVCI_SetTSBypass2CAMFlag(U8 u8Slot)
{
    if (CI_SLOT0 == u8Slot)
    {
        FPGA_setCAM0bypassOn();
    }
    else if (CI_SLOT1 == u8Slot)
    {
        FPGA_setCAM1bypassOn();
    }

    return WVCI_SUCCESS;
}

int WVCI_ClearTSBypass2CAMFlag(U8 u8Slot)
{
    if (CI_SLOT0 == u8Slot)
    {
        FPGA_setCAM0bypassOff();
    }
    else if (CI_SLOT1 == u8Slot)
    {
        FPGA_setCAM1bypassOff();
    }

    return WVCI_SUCCESS;
}

/******************************************************************************
 * WVCI_UpdateSetting() - 根据TS配置更新CI模块的应用
 * DESCRIPTION: - 
 *          1. 遍历所有节目，判断是否设置解扰
 *          2. 根据设置，生成对应的CAPMT，并加入到协议栈的发送队列
 *          
 * Input  : emSlotMode:         配置模式，可单独某个通道或两个通道一起配置
 * Output : None    
 * Returns: WVCI_SUCCESS        操作成功
            WVCI_FAILURE        操作失败
 * 
 ******************************************************************************/
int WVCI_UpdateSetting(WVCI_SLOTMODE_t emSlotMode)
{
    wvErrCode enErrCode = WV_SUCCESS; 
    int i = 0;
    U8 u8Slot = 0;
    TSInfo *pstParamTS = NULL;
    IndexInfoList arstDescrambledProgIndexList[WVCI_MAX_SLOT_NUM];
    U16 u16OutTSIndex = INDEX_INVALID_VALUE;
    U16 u16ProgIndex = INDEX_INVALID_VALUE;
    U8 u8Mode = 0;

    pstParamTS = TSP_GetTSParamHandle();

    s_CITaskMuxFlag = 0x01;     // disable check slot 

    for (u8Slot = 0; u8Slot < WVCI_MAX_SLOT_NUM; u8Slot++)
    {
        arstDescrambledProgIndexList[u8Slot].u16IndexListNum = 0;
        if (WV_SUCCESS != TSP_GetOutputTSIndex(u8Slot, pstParamTS, &u16OutTSIndex))
        {
            continue;
        }

        enErrCode = TSP_GetOutTSProgIndexList(u16OutTSIndex, pstParamTS, &arstDescrambledProgIndexList[u8Slot]);
        if (WV_SUCCESS != enErrCode)
        {
            log_printf(LOG_LEVEL_ERROR, LOG_MODULE_TSP,
               "[%s:%d]TSP_GetOutTSProgIndexList Error:enErrCode[%X],u8Slot[%u],u16OutTSIndex[%u]\r\n",
               __FUNCTION__, __LINE__, enErrCode, u8Slot, u16OutTSIndex);
            return (int)enErrCode;
        }
    }
    
    for (u8Slot = 0; u8Slot < WVCI_MAX_SLOT_NUM; u8Slot++)
    {
        if (0 == arstDescrambledProgIndexList[u8Slot].u16IndexListNum)
        {
            WVCI_ClearTSBypass2CAMFlag(u8Slot);
        }
        else
        {
            WVCI_SetTSBypassass2CAM(u8Slot, TSP_CICamIndex2StreamID(u8Slot));
            WVCI_SetTSBypass2CAMFlag(u8Slot);
        }
    }

    for (u8Slot = 0; u8Slot < WVCI_MAX_SLOT_NUM; u8Slot++)
    {
        if (((CI_SLOT0 == u8Slot) && (emSlotMode == ONLY_SLOT_1))
            || ((CI_SLOT1 == u8Slot) && (emSlotMode == ONLY_SLOT_0)))
        {
            continue;
        }

        if (0 == arstDescrambledProgIndexList[u8Slot].u16IndexListNum)// 停止解扰
        {
            WVCI_GenerateCAPMT(u8Slot,
                                s_u16LastDescrProgIndex[u8Slot],
                                pstParamTS,
                                CA_PMT_LIST_MANAGEMENT_WELLAV_DESCRAMBLE); //WELLAV增加测试用，不符合en50221标准
            continue;
        }
        else if (1 == arstDescrambledProgIndexList[u8Slot].u16IndexListNum)// ONLY one CAPMT
        {
            WVCI_GenerateCAPMT(u8Slot,
                                arstDescrambledProgIndexList[u8Slot].aru16IndexList[0],
                                pstParamTS,
                                CA_PMT_LIST_MANAGEMENT_ONLY);
            continue;
        }

        for (i = 0; i < arstDescrambledProgIndexList[u8Slot].u16IndexListNum; i++)
        {
            u16ProgIndex = arstDescrambledProgIndexList[u8Slot].aru16IndexList[i];

            if (0 == i)
            {
                u8Mode = CA_PMT_LIST_MANAGEMENT_FIRST;
            }
            else if ((i + 1) == arstDescrambledProgIndexList[u8Slot].u16IndexListNum)
            {
                u8Mode = CA_PMT_LIST_MANAGEMENT_LAST;
            }
            else
            {
                u8Mode = CA_PMT_LIST_MANAGEMENT_MORE;
            }

            WVCI_GenerateCAPMT(u8Slot, u16ProgIndex, pstParamTS, u8Mode);
        }
    }
    
    s_CITaskMuxFlag = 0x00;     // enable check slot    
    
    return WVCI_SUCCESS;
}

/******************************************************************************
 * TSMUX_CI_SetMaxBitrate() - 设置CI模块支持的最大码率
 * DESCRIPTION:
 * Input  : u32Bitrate:         支持的最大码率
 * Output : None    
 * Returns: WVCI_SUCCESS        操作成功
            WVCI_FAILURE        操作失败
 * 
 ******************************************************************************/
int TSMUX_CI_SetMaxBitrate(U32 u32Bitrate)
{
    U8 RegValue = 12;

    switch(u32Bitrate)
    {
        case CI_TS_BITRATE_48Mbps_18:
        {
            RegValue = 23;              // in fact: supported max: 45Mbps
            break;
        }
        
        case CI_TS_BITRATE_56Mbps_16:
        {
            RegValue = 20;              // in fact: supported max: 57Mbps
            break;
        }
        
        case CI_TS_BITRATE_64Mbps_13:
        {
            RegValue = 16;              // in fact: supported max: 61Mbps
            break;
        }
        
        case CI_TS_BITRATE_72Mbps_12:
        {
            RegValue = 15;              // in fact: supported max: 70Mbps
            break;
        }
        
        case CI_TS_BITRATE_80Mbps_10:
        {
            RegValue = 12;              // in fact: supported max: 78Mbps
            break;
        }
        
        case CI_TS_BITRATE_100Mbps_9:
        {
            RegValue = 11;               // in fact: supported max: 99Mbps
            break;
        }
        
        default:
        {
            RegValue = 10;     //
            break;
        }
    }
    
    FPGA_setCiClk(RegValue);

    return WV_SUCCESS;
}

/********************************************************************
Function:     WVCI_GetCIStatus
Description:  获取CI解扰状态信息
Input:        u8Slot:CI的CAM槽号
Output:       pstCIStatus:CI解扰状态信息
Return:       
Author:       Momouwei 2017.05.11
********************************************************************/
wvErrCode WVCI_GetCIStatus(U8 u8Slot, WAVCI_Status_4_web_t *pstCIStatus, TSInfo *pstParamTS)
{
    U8 *pu8CAMName = NULL;
    U8 *pu8Manufacturer = NULL;
    
    if ((WVCI_MAX_SLOT_NUM <= u8Slot) || (!pstCIStatus) || (!pstParamTS))
    {
        log_printf(LOG_LEVEL_ERROR, LOG_MODULE_CI,
            "[%s]:Input error,u8Slot[%u],pstCIStatus[%p],pstParamTS[%p]\r\n",
            __FUNCTION__, u8Slot, pstCIStatus, pstParamTS);

        return WV_ERR_PARAMS;
    }

    memset(pstCIStatus, 0, sizeof(WAVCI_Status_4_web_t));

    pstCIStatus->u8CAMInsertFlag = g_WVCIStatusInst.m_u8SlotInserted[u8Slot];
    pstCIStatus->u8CAMInitFlag = g_WVCIStatusInst.m_ucSlotInited[u8Slot];

    if (0 == u8Slot)
    {
        pu8CAMName = g_WVCIStatusInst.m_ucSlot0Name;
        pu8Manufacturer = g_WVCIStatusInst.m_ucSlot0Manufacturer;
    }
    else
    {
        pu8CAMName = g_WVCIStatusInst.m_ucSlot1Name;
        pu8Manufacturer = g_WVCIStatusInst.m_ucSlot1Manufacturer;
    }
    memcpy(pstCIStatus->aru8CAMName, pu8CAMName, WVCI_SLOT_NAME_SIZE);
    memcpy(pstCIStatus->aru8Manufacturer, pu8Manufacturer, WVCI_SLOT_NAME_SIZE);

    pstCIStatus->u8CASystemNum = g_WVCIStatusInst.m_u16SystemIDNum[u8Slot];
    memcpy(pstCIStatus->aru16CASystemIDList,
            g_WVCIStatusInst.m_u16SystemIDList[u8Slot],
            MAX_CASYSTEM_ID_NUM * sizeof(U16));

    return TSP_GetCIPIDInfo(u8Slot, pstCIStatus->arstPIDInfoList, &pstCIStatus->u8PIDNum, pstParamTS);
}

