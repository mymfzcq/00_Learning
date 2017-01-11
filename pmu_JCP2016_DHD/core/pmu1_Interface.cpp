#include "pmu_ci.h"
#include "pmu_ct.h"
#include "pmu1_Statistics.hpp"
#include "pmu1__Performance.hpp"
#ifdef JCP2011_CPU
#include "pmu1_InterfaceJCP2011.hpp"
#include "pmu1__PerformanceCPUJCP2011.hpp"
#include "pmu1__PerformanceGPUJCP2011GAD.hpp"
#elif defined JCP2016_CPU
#include "pmu1_InterfaceJCP2016.hpp"
#include "pmu1__PerformanceCPUJCP2016.hpp"
#include "pmu1__PerformanceGPUJCP2016DHD.hpp"
#endif


extern "C"
{
#include <string.h>
#include "gauxl_c1.h"
#if (PMU_STATISTICS_PIPELINE == PMU_USE_DHD_PIPELINE)
#include "grgl_c1.h"
#elif (PMU_STATISTICS_PIPELINE == PMU_USE_JCP2011_PIPELINE)
#include "gad_c1.h"  // C Export wrapper
#endif
#include "stdio.h"  // C Export wrapper
#include "frt_c1.h"  // C Export wrapper
#include "timd_c1.h"  // C Export wrapper
#include "pmu1_Interf_ce.h"  // C Export wrapper
#include "grlc_c1.h"  // C Export wrapper
#include "RSST_c1.h"  // C Export wrapper
#include "ASRL_C1.h"



    static bool PMU__boSWInit = false;
    volatile uint32 PMU_u32StatisticsCounters;

    volatile uint32 PMU_u32BenchMarkValue;

    uint16 PMU_u16ContextID;
    uint16 PMU_u16SurfaceID;

    GBT_tstMSrfcDesc PMU_stMSrfcDesc;

    GBT_tstBmp PMU_stBmp;

    PMU_tenTaskMode PMU_enTaskFlag;
    volatile bool boGPUMatchCPUFlag;
    volatile uint32 u32TempCPUCnt;
    volatile float fTempCPU;
    /*GBT_tstMBmp PMU_stMBmp =
    {
    {  // GBT_tstBmp
    NULL, //void *pvPixData;
    {NULL,0,0,0,0},         //GBT_tstAlphaMap stAlphaMap;
    {NULL,0,0},             //GBT_tstCLUT stCLUT;
    PMU_BMP_SIZE,           //uint32 u32PixDataByteLen;
    0,                      //uint32 u32ColorKey;
    PMU_BMP_SIZE_X,         //uint16 u16Width;
    PMU_BMP_SIZE_Y,         //uint16 u16Height;
    0,                      //uint8 u8LinePaddingBytes;
    0,                      //uint8 u8Compression;
    0x01, // direct color   //uint8 u8Attribs;
    RSST_nPFrmt_RGBA8888
    },						  //uint8 u8PixFormatIdx;
    {						  // GBT_tstMgdObj
    0xFFFF,                 //uint16 u16ResID;
    RSST_nMemJCP2011_iRAM,  //uint8 u8ObjCtrlLoc;
    RSST_nMemJCP2011_DRAM,  //uint8 u8ObjDataLoc;
    GBT_nCPolicy_No,        //uint8 u8CPolicy;
    0,
    },						  //uint8 u8Attr;
    RSST_nOffscreenSurface //1 //u16SrfcID
    };
    */
    GBT_tstMText PMU_Text_Time, PMU_Text_CPU, PMU_Text_GPU, PMU_Text_FPS;
    uint16 *pPMU_Text_Time, *pPMU_Text_CPU, *pPMU_Text_GPU, *pPMU_Text_FPS;

    DeclareAlarm(PMU_1000msAlarm);
    DeclareTask(PMU_1000ms);
    DeclareTask(PMU_MainTask);
    DECLARESURFACE;
    //extern GRLC__tstContext* GRLC__pstGetContext( uint16 u16ContextID );
    //extern volatile uint8 GRLC__astContext[RSST_nLastCtxtDesc];
    ISR(PMU_TAUB0106_ISR)
    {

        TIMD_vDisIntAndStop0106();
    }

    TASK(PMU_1000ms)
    {
        if (PMU__boSWInit == false)
        {
            PMU__boSWInit = true;
        }
        else
        {
            //do not change init flag
        }
        //timer tick handler
        vSecondAccumulationHandler();
        //PMU_enTaskFlag = enGPUOutputMode;
        TerminateTask();
    }
    TASK(PMU_MainTask)
    {
        switch (PMU_enTaskFlag)
        {
        case enBenchmarkMode:
        {
            uint32 u32result1, u32result2, u32result3;
            //phase 1
            TIMD_vResetReloadInt0106(TIMD_xUSecToValue0106(100));
            u32result1 = 0;
            __DI();
            while (False == TIMD_boTimerElapsed0106())
            {
                u32result1++;
            }
            __EI();
            //phase2
            TIMD_vResetReloadInt0106(TIMD_xUSecToValue0106(1000));
            u32result2 = 0;
            __DI();
            while (False == TIMD_boTimerElapsed0106())
            {
                u32result2++;
            }
            __EI();
            //phase3
            TIMD_vResetReloadInt0106(TIMD_xUSecToValue0106(10000));
            u32result3 = 0;
            __DI();
            while (False == TIMD_boTimerElapsed0106())
            {
                u32result3++;
            }
            __EI();
            // PMU_u32BenchMarkValue indicates the 1s cpu add values;
            PMU_u32BenchMarkValue = (u32result1 * 100 + u32result2 * 10 + u32result3) / 3 * 100;
            PMU_enTaskFlag = enGraphicPrepareMode;
            // this is default funciton
            vSetGPURefreshCycle(1000);

            //TerminateTask();
            break;
        }
        case enGraphicPrepareMode:
        {
            vPreparePromptSurface();
            PMU_enTaskFlag = enCPUOutputMode;
            //now we can calc the first gpu load
#if defined JCP2011_CPU
            PMU_vInitPFC_JCP2011();
#elif (defined JCP2016_CPU && PMU_STATISTICS_PIPELINE == PMU_USE_DHD_PIPELINE)
            PMU_vInitPFC_JCP2016();
#endif
            //TerminateTask();
            break;
        }
        case enCPUOutputMode:
        {

            PMU::Statistics::vStartTimerTracking();
            //stage 3: gpu load
            vClearIDLEHook();
            PMU_enTaskFlag = enGPUOutputMode;
        }
        case enGPUOutputMode:
        {
            if (boGPUMatchCPUFlag == true)
            {
                // gpu match cpu,synchronous method
                PMU::Statistics::vStartTimerTracking();
                PMU_enTaskFlag = enCPUOutputMode;
            }
            else
            {

            }
            //TerminateTask();
        }
            break;
        default:
            break;
    }

        TerminateTask();
}

}


void PMU_vInit(void)
{
#if (PMU_STATISTICS_PIPELINE == PMU_USE_JCP2011_PIPELINE)
    PMU_vInitJCP2011();
#elif (PMU_STATISTICS_PIPELINE == PMU_USE_DHD_PIPELINE)
    PMU_vInitJCP2016();
#endif
}

void PMU_vCPUIdleTaskHook(void)
{
    //enhance the credibility so we use the minus operation
    //if(PMU__boSWInit)
    //{
#if defined JCP2011_CPU
    PMU::PerformanceCPUJCP2011::vCPUIdleHook();
#elif (defined JCP2016_CPU && PMU_STATISTICS_PIPELINE == PMU_USE_DHD_PIPELINE)
    PMU::PerformanceCPUJCP2016::vCPUIdleHook();
#endif
    //}
}

void PMU_vCPUGetPerformanceBenchMark(PMU_tenCPUType enCPUType, uint32 PMU_u32BenchMarkTime)
{
    __DI();
    //start pmu task
    PMU_enTaskFlag = enBenchmarkMode;
    ActivateTask(PMU_MainTask);
    __EI();
    Schedule();
}

void vSetGPURefreshCycle(uint16 u16MicroSeconds)
{
    if (u16MicroSeconds == 1000)
    {
        boGPUMatchCPUFlag = true;
    }
    else
    {
        boGPUMatchCPUFlag = false;
        // asynchronous method

    }
}

uint16 vGetGPURefreshCycle(void)
{

}

void vStartMeasurement(void)
{
    // lock 
    vClearIDLEHook();
    //reload cpu counter to 0
    PMU_u32StatisticsCounters = 0;
    //vPreparePromptSurface();
    SetRelAlarm(PMU_1000msAlarm, MSEC(35), MSEC(1000));

    // unlock
}

void vStopMeasurement(void)
{
    CancelAlarm(PMU_1000msAlarm);
    //PMU_enTaskFlag = enCPUOutputMode;
}

void PMU_1000msHandler(void)
{

}
void vSecondAccumulationHandler(void)
{
    //PMU_enTaskFlag = enGraphicPrepareMode;
    PMU::Statistics::vExpiredTimeHandler();
}

void vPreparePromptSurface(void)
{
#if (PMU_STATISTICS_PIPELINE == PMU_USE_JCP2011_PIPELINE)
    PMU_vPreparePromptSurfaceJCP2011();
#elif (PMU_STATISTICS_PIPELINE == PMU_USE_DHD_PIPELINE)
    PMU_vPreparePromptSurfaceJCP2016();
#endif
}

void vGetExpiredTime(CLOCK_tstTimeDate* pstTemp)
{
    pstTemp->unTime.stTime.u8Hour = PMU::Statistics::stTimeData.unTime.stTime.u8Hour;
    pstTemp->unTime.stTime.u8Minute = PMU::Statistics::stTimeData.unTime.stTime.u8Minute;
    pstTemp->unTime.stTime.u8Second = PMU::Statistics::stTimeData.unTime.stTime.u8Second;
}

void vClearIDLEHook(void)
{
    __DI();
    //clear the time value
#if defined JCP2011_CPU
    PMU::PerformanceCPUJCP2011::u32BenchMarkCounters = 0;
#elif (defined JCP2016_CPU && PMU_STATISTICS_PIPELINE == PMU_USE_DHD_PIPELINE)
    PMU::PerformanceCPUJCP2016::u32BenchMarkCounters = 0;
#endif
    __EI();
}

void vGoFailure(void)
{
    ASRL_vAssert(false);
}