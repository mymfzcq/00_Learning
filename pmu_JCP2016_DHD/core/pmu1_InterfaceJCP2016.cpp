#include "pmu_ci.h"
#include "pmu_ct.h"
#include "pmu1_Statistics.hpp"
#include "pmu1__Performance.hpp"
#ifdef JCP2016_CPU
//#include "pmu_InterfaceJCP2011.h"
#include "pmu1__PerformanceCPUJCP2016.hpp"
#include "pmu1__PerformanceGPUJCP2016DHD.hpp"

extern "C"
{
#include <string.h>
#include "gauxl_c1.h"
#include "grlc__pipej16dhd.h"
#include "grgl_c1.h"  // C Export wrapper
#include "davehd_registertypes.h"
#include "grlc_c1.h"  // C Export wrapper
#include "stdio.h"  // C Export wrapper
#include "pmu1_Interf_ce.h"  // C Export wrapper
#include "grlc_c1.h"  // C Export wrapper
#include "RSST_c1.h"  // C Export wrapper

    extern volatile uint32 PMU_u32StatisticsCounters;
    extern GBT_tstBmp PMU_stBmp;
    extern uint16 PMU_u16ContextID;
    extern uint16 PMU_u16SurfaceID;
    extern PMU_tenTaskMode PMU_enTaskFlag;
    extern volatile bool boGPUMatchCPUFlag;
    extern 	GBT_tstMSrfcDesc PMU_stMSrfcDesc;
#if (PMU_STATISTICS_PIPELINE == PMU_USE_DHD_PIPELINE)
    void PMU_vInitJCP2016(void)
    {
        PMU_u32StatisticsCounters = 0;
        PMU_u16ContextID = 0;
        PMU_u16SurfaceID = 0;
        PMU_enTaskFlag = enInitialMode;
        boGPUMatchCPUFlag = true;
        PMU::PerformanceCPUJCP2016::vInit();
        PMU::PerformanceGPUJCP2016DHD::vInit();
    }
    void PMU_vPreparePromptSurfaceJCP2016(void)
    {
    }

#endif
    void PMU_vInitPFC_JCP2016(void)
    {

        PMU::PerformanceGPUJCP2016DHD* GPUHandle = PMU::PerformanceGPUJCP2016DHD::vReturnGPUObject();


        static uint32_t err = E_DHD_OK;

        /* Initialise and allocate performance counter in GPU memory space */
        err |= dhd_alloc_pc(GPUHandle->dhd_device, &(GPUHandle->PMU_stPFC)[0]);
        err |= dhd_alloc_pc(GPUHandle->dhd_device, &(GPUHandle->PMU_stPFC)[1]);
        err |= dhd_alloc_pc(GPUHandle->dhd_device, &(GPUHandle->PMU_stPFC)[2]);
        err |= dhd_alloc_pc(GPUHandle->dhd_device, &(GPUHandle->PMU_stPFC)[3]);

        /* Register performance counter for your event
        * Short description of event types is in 'davehd_registertypes.h' -> 'dhd_reg_pfc_event_t'
        */
        err |= dhd_set_pc_event(GPUHandle->dhd_device, &(GPUHandle->PMU_stPFC)[0], (dhd_pcevents_t)E_PFC_EVENT_FBC_PIXEL_WRITE);
        err |= dhd_set_pc_event(GPUHandle->dhd_device, &(GPUHandle->PMU_stPFC)[1], (dhd_pcevents_t)E_PFC_EVENT_CLR_WORD_WRITE);
        err |= dhd_set_pc_event(GPUHandle->dhd_device, &(GPUHandle->PMU_stPFC)[2], (dhd_pcevents_t)E_PFC_EVENT_STC_CORE_ACTIVE);
        err |= dhd_set_pc_event(GPUHandle->dhd_device, &(GPUHandle->PMU_stPFC)[3], (dhd_pcevents_t)E_PFC_EVENT_TXC_WORD_READ);

        if (err != E_DHD_OK)
        {
            vGoFailure();
        }

        GPUHandle->vPFCResetAndStart();
    }
}

GBT_tstMText PMU_Text_CPU_MAX, PMU_Text_GPU_MAX, PMU_Text_FPS_MAX, PMU_Const_String_MAX;
char *pPMU_Text_CPU_MAX, *pPMU_Text_GPU_MAX, *pPMU_Text_FPS_MAX;

char chPMU_Const_String_MAX[] = "MAX";

using namespace PMU;


void PMU::JCP2016DHD::PMU_vMaxCPULoadPrompt(void)
{
    PMU_Text_CPU_MAX.stText.u16TextByteLen = 0xFFFF;
    PMU_Text_CPU_MAX.stText.u8Attributes = 0x00; // 8-bit
    PMU_Text_CPU_MAX.stText.u8Compression = GBT_nComp_No;
    PMU_Text_CPU_MAX.stMgInfo.u16ResID = 0xffff;
    PMU_Text_CPU_MAX.stMgInfo.u8Attr = 0;
    PMU_Text_CPU_MAX.stMgInfo.u8CPolicy = GBT_nCPolicy_No;
    PMU_Text_CPU_MAX.stMgInfo.u8ObjCtrlLoc = PMU_TextLoc;
    PMU_Text_CPU_MAX.stMgInfo.u8ObjDataLoc = PMU_TextLoc;
    char tempCPU_MAX[10] = "";
    sprintf(tempCPU_MAX, "%0.2f%%", PerformanceCPUJCP2016::vReturnCPUObject()->PMU_fCPU_MAX);
    pPMU_Text_CPU_MAX = tempCPU_MAX;
    PMU_Text_CPU_MAX.stText.pvTextData = pPMU_Text_CPU_MAX;
    PMU_Text_CPU_MAX.stText.u16TextByteLen = strlen(tempCPU_MAX);
    GRLC_vDrawMTextAt(PMU_u16ContextID, &PMU_Text_CPU_MAX, PMU_BMP_SIZE_X * 2 / 3, 20);
    GRLC_vReleaseMText(PMU_u16ContextID, &PMU_Text_CPU_MAX);

}

void PMU::JCP2016DHD::PMU_vMaxGPULoadPrompt(void)
{
    //PMU_Text_GPU_MAX.stText.u16TextByteLen = 0xFFFF;
    //PMU_Text_GPU_MAX.stText.u8Attributes = 0x00; // 8-bit
    //PMU_Text_GPU_MAX.stText.u8Compression = GBT_nComp_No;
    //PMU_Text_GPU_MAX.stMgInfo.u16ResID = 0xffff;
    //PMU_Text_GPU_MAX.stMgInfo.u8Attr = 0;
    //PMU_Text_GPU_MAX.stMgInfo.u8CPolicy = GBT_nCPolicy_No;
    //PMU_Text_GPU_MAX.stMgInfo.u8ObjCtrlLoc = PMU_TextLoc;
    //PMU_Text_GPU_MAX.stMgInfo.u8ObjDataLoc = PMU_TextLoc;
    //char tempGPU_MAX[10] = "";
    //sprintf(tempGPU_MAX, "%0.2f%%", PerformanceGPUJCP2016DHD::vReturnGPUObject()->PMU_fGPU_MAX);
    //pPMU_Text_GPU_MAX = tempGPU_MAX;
    //PMU_Text_GPU_MAX.stText.pvTextData = pPMU_Text_GPU_MAX;
    //PMU_Text_GPU_MAX.stText.u16TextByteLen = strlen(tempGPU_MAX);
    //GRLC_vDrawMTextAt(PMU_u16ContextID, &PMU_Text_GPU_MAX, PMU_BMP_SIZE_X * 2 / 3, 40);
    //////GRLC_vDrawTextIDAt(PMU_u16ContextID,RSST_nTXT_Sample_All_Content,0,0);
    //GRLC_vReleaseMText(PMU_u16ContextID, &PMU_Text_GPU_MAX);
}

void PMU::JCP2016DHD::PMU_vMaxStringPrompt(void)
{
    //PMU_Const_String_MAX.stText.u16TextByteLen = 0xFFFF;
    //PMU_Const_String_MAX.stText.u8Attributes = 0x00; // 8-bit
    //PMU_Const_String_MAX.stText.u8Compression = GBT_nComp_No;
    //PMU_Const_String_MAX.stMgInfo.u16ResID = 0xffff;
    //PMU_Const_String_MAX.stMgInfo.u8Attr = 0;
    //PMU_Const_String_MAX.stMgInfo.u8CPolicy = GBT_nCPolicy_No;
    //PMU_Const_String_MAX.stMgInfo.u8ObjCtrlLoc = PMU_TextLoc;
    //PMU_Const_String_MAX.stMgInfo.u8ObjDataLoc = PMU_TextLoc;
    //PMU_Const_String_MAX.stText.pvTextData = static_cast<void*>(::chPMU_Const_String_MAX);
    //PMU_Const_String_MAX.stText.u16TextByteLen = sizeof ::chPMU_Const_String_MAX;
    //GRLC_vDrawMTextAt(PMU_u16ContextID, &PMU_Const_String_MAX, PMU_BMP_SIZE_X * 3 / 4 + 3, 0);
    //////GRLC_vDrawTextIDAt(PMU_u16ContextID,RSST_nTXT_Sample_All_Content,0,0);
    //GRLC_vReleaseMText(PMU_u16ContextID, &PMU_Const_String_MAX);

}

void PMU::JCP2016DHD::PMU_vMaxFPSLoadPrompt(void)
{
    //PMU_Text_FPS_MAX.stText.u16TextByteLen = 0xFFFF;
    //PMU_Text_FPS_MAX.stText.u8Attributes = 0x00; // 8-bit
    //PMU_Text_FPS_MAX.stText.u8Compression = GBT_nComp_No;
    //PMU_Text_FPS_MAX.stMgInfo.u16ResID = 0xffff;
    //PMU_Text_FPS_MAX.stMgInfo.u8Attr = 0;
    //PMU_Text_FPS_MAX.stMgInfo.u8CPolicy = GBT_nCPolicy_No;
    //PMU_Text_FPS_MAX.stMgInfo.u8ObjCtrlLoc = PMU_TextLoc;
    //PMU_Text_FPS_MAX.stMgInfo.u8ObjDataLoc = PMU_TextLoc;
    //char tempFPS_MAX[10] = "";
    //sprintf(tempFPS_MAX, "%d", PerformanceGPUJCP2016DHD::vReturnGPUObject()->PMU_u32FPS_MAX);
    //pPMU_Text_FPS_MAX = tempFPS_MAX;
    //PMU_Text_FPS_MAX.stText.pvTextData = pPMU_Text_FPS_MAX;
    //PMU_Text_FPS_MAX.stText.u16TextByteLen = strlen(tempFPS_MAX);
    //GRLC_vDrawMTextAt(PMU_u16ContextID, &PMU_Text_FPS_MAX, PMU_BMP_SIZE_X * 2 / 3 + 20, 60);
    //////GRLC_vDrawTextIDAt(PMU_u16ContextID,RSST_nTXT_Sample_All_Content,0,0);
    //GRLC_vReleaseMText(PMU_u16ContextID, &PMU_Text_FPS_MAX);
}

void PMU::JCP2016DHD::PMU_vBandwidthNotSupport(void)
{
    //GBT_tstMText PMU_Text_BandWidth;
    //memset(&PMU_Text_BandWidth, 0, sizeof PMU_Text_BandWidth);
    //PMU_Text_BandWidth.stText.u16TextByteLen = 0xFFFF;
    //PMU_Text_BandWidth.stText.u8Attributes = 0x00; // 8-bit
    //PMU_Text_BandWidth.stText.u8Compression = GBT_nComp_No;
    //PMU_Text_BandWidth.stMgInfo.u16ResID = 0xffff;
    //PMU_Text_BandWidth.stMgInfo.u8Attr = 0;
    //PMU_Text_BandWidth.stMgInfo.u8CPolicy = GBT_nCPolicy_No;
    //PMU_Text_BandWidth.stMgInfo.u8ObjCtrlLoc = PMU_TextLoc;
    //PMU_Text_BandWidth.stMgInfo.u8ObjDataLoc = PMU_TextLoc;
    //PMU_Text_BandWidth.stText.pvTextData = static_cast<char*>("No Bandwidth support");
    //PMU_Text_BandWidth.stText.u16TextByteLen = strlen("No Bandwidth support");
    //GRLC_vDrawMTextAt(PMU_u16ContextID, &PMU_Text_BandWidth, 0, 80);
    //////GRLC_vDrawTextIDAt(PMU_u16ContextID,RSST_nTXT_Sample_All_Content,0,0);
    //GRLC_vReleaseMText(PMU_u16ContextID, &PMU_Text_BandWidth);
}

#endif 
