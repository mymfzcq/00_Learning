#include "pmu_ci.h"
#include "pmu_ct.h"
#include "pmu1_Statistics.hpp"
#include "pmu1__Performance.hpp"
#ifdef JCP2011_CPU
//#include "pmu_InterfaceJCP2011.h"
#include "pmu1__PerformanceCPUJCP2011.hpp"
#include "pmu1__PerformanceGPUJCP2011GAD.hpp"

extern "C"
{
#include "gauxl_c1.h"
#include "gad_c1.h"  // C Export wrapper
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
#if (PMU_STATISTICS_PIPELINE == PMU_USE_JCP2011_PIPELINE)
    void PMU_vInitJCP2011(void)
    {
        PMU_u32StatisticsCounters = 0;
        PMU_u16ContextID = 0;
        PMU_u16SurfaceID = 0;
        PMU_enTaskFlag = enInitialMode;
        boGPUMatchCPUFlag = true;
        PMU::PerformanceCPUJCP2011::vInit();
        GBT_tstAlphaMap stIniAlphaMap = { NULL, 0, 0, 0, 0 };
        GBT_tstCLUT stInistCLUT = { NULL, 0, 0 };
        //memset(&stTimeData,0,sizeof stTimeData);
        memset(&PMU_stBmp, 0, sizeof PMU_stBmp);
        memset(&PMU_stMSrfcDesc, 0, sizeof PMU_stMSrfcDesc);

        //init managed text structure
        memset(&PMU_Text_Time, 0, sizeof(GBT_tstMText));
        memset(&PMU_Text_CPU, 0, sizeof(GBT_tstMText));
        memset(&PMU_Text_GPU, 0, sizeof(GBT_tstMText));
        memset(&PMU_Text_FPS, 0, sizeof(GBT_tstMText));
        pPMU_Text_Time = NULL;
        pPMU_Text_CPU = NULL;
        pPMU_Text_GPU = NULL;
        pPMU_Text_FPS = NULL;

        memset(&::PMU_Text_CPU_MAX, 0, sizeof(GBT_tstMText));
        memset(&::PMU_Text_GPU_MAX, 0, sizeof(GBT_tstMText));
        memset(&::PMU_Text_FPS_MAX, 0, sizeof(GBT_tstMText));
        ::pPMU_Text_CPU = NULL;
        ::pPMU_Text_GPU = NULL;
        ::pPMU_Text_FPS = NULL;


        //PMU_Text_Time
        //managed_text16.stText.pvTextData = text16;
        PMU_Text_Time.stText.u16TextByteLen = 0xFFFF;
        PMU_Text_Time.stText.u8Attributes = 0x01; // 16-bit
        PMU_Text_Time.stText.u8Compression = GBT_nComp_No;
        PMU_Text_Time.stMgInfo.u16ResID = 0xffff;
        PMU_Text_Time.stMgInfo.u8Attr = 0;
        PMU_Text_Time.stMgInfo.u8CPolicy = GBT_nCPolicy_No;
        PMU_Text_Time.stMgInfo.u8ObjCtrlLoc = PMU_TextLoc;
        PMU_Text_Time.stMgInfo.u8ObjDataLoc = PMU_TextLoc;
        //PMU_Text_CPU
        //managed_text16.stText.pvTextData = text16;
        PMU_Text_CPU.stText.u16TextByteLen = 0xFFFF;
        PMU_Text_CPU.stText.u8Attributes = 0x01; // 16-bit
        PMU_Text_CPU.stText.u8Compression = GBT_nComp_No;
        PMU_Text_CPU.stMgInfo.u16ResID = 0xffff;
        PMU_Text_CPU.stMgInfo.u8Attr = 0;
        PMU_Text_CPU.stMgInfo.u8CPolicy = GBT_nCPolicy_No;
        PMU_Text_CPU.stMgInfo.u8ObjCtrlLoc = PMU_TextLoc;
        PMU_Text_CPU.stMgInfo.u8ObjDataLoc = PMU_TextLoc;
        //PMU_Text_GPU
        //PMU_Text_GPU.stText.pvTextData = text16;
        PMU_Text_GPU.stText.u16TextByteLen = 0xFFFF;
        PMU_Text_GPU.stText.u8Attributes = 0x01; // 16-bit
        PMU_Text_GPU.stText.u8Compression = GBT_nComp_No;
        PMU_Text_GPU.stMgInfo.u16ResID = 0xffff;
        PMU_Text_GPU.stMgInfo.u8Attr = 0;
        PMU_Text_GPU.stMgInfo.u8CPolicy = GBT_nCPolicy_No;
        PMU_Text_GPU.stMgInfo.u8ObjCtrlLoc = PMU_TextLoc;
        PMU_Text_GPU.stMgInfo.u8ObjDataLoc = PMU_TextLoc;
        //PMU_Text_FPS
        //PMU_Text_FPS.stText.pvTextData = text16;
        PMU_Text_FPS.stText.u16TextByteLen = 0xFFFF;
        PMU_Text_FPS.stText.u8Attributes = 0x01; // 16-bit
        PMU_Text_FPS.stText.u8Compression = GBT_nComp_No;
        PMU_Text_FPS.stMgInfo.u16ResID = 0xffff;
        PMU_Text_FPS.stMgInfo.u8Attr = 0;
        PMU_Text_FPS.stMgInfo.u8CPolicy = GBT_nCPolicy_No;
        PMU_Text_FPS.stMgInfo.u8ObjCtrlLoc = PMU_TextLoc;
        PMU_Text_FPS.stMgInfo.u8ObjDataLoc = PMU_TextLoc;

        PMU_stBmp.pvPixData = NULL;
        PMU_stBmp.stAlphaMap = stIniAlphaMap;
        PMU_stBmp.stCLUT = stInistCLUT;
        PMU_stBmp.u32ColorKey = 0;
        PMU_stBmp.u16Width = PMU_BMP_SIZE_X;
        PMU_stBmp.u16Height = PMU_BMP_SIZE_Y;
        PMU_stBmp.u32PixDataByteLen = PMU_BMP_SIZE;
        PMU_stBmp.u8LinePaddingBytes = 0;
        PMU_stBmp.u8Compression = 0;
        PMU_stBmp.u8Attribs = 0x01;
        PMU_stBmp.u8PixFormatIdx = PMU_APP_COLOR_FORMAT;
        PMU::PerformanceGPUJCP2011GAD::vInit();
    }
    void PMU_vPreparePromptSurfaceJCP2011(void)
    {
        //create temporary surface for statistics
        //uint8 GRLC_contextID;
        //for (GRLC_contextID = 0;GRLC_contextID<RSST_nLastCtxtDesc;GRLC_contextID++)
        //{
        //	//the internal function is needed
        //	if (GRLC__astContext[GRLC_contextID] == PMU_UnInitialized)
        //	{
        //		PMU_u16ContextID = GRLC_u16GetRenderContext ( static_cast<RSST_tenCtxtDescID>(GRLC_contextID));
        //		break;
        //	}
        //}
        //note 
        uint8 u8ContextID;
        PMU_u16ContextID = GRLC_u16GetRenderContext(static_cast<RSST_tenCtxtDescID>(RSST_nLastCtxtDesc - 1));
        PMU_stMSrfcDesc.stSrfcDesc.pvAlphaMemList = NULL;
        PMU_stMSrfcDesc.stSrfcDesc.pvPixelMemList = NULL;
        PMU_stMSrfcDesc.stSrfcDesc.stBmp = PMU_stBmp;
        PMU_stMSrfcDesc.stSrfcDesc.u16ConfigSet = 0;
        PMU_stMSrfcDesc.stSrfcDesc.u16Pool = 0;
        PMU_stMSrfcDesc.stSrfcDesc.u8Ctrl = RSST_nCtrl_JCP2011;
        PMU_stMSrfcDesc.stSrfcDesc.u8Layer = PMU_DefaultSurface;//RSST_nJCP2011MVO_L3;
        PMU_stMSrfcDesc.stSrfcDesc.u8MemLoc = PMU_SurfaceLoc;
        PMU_stMSrfcDesc.stSrfcDesc.u8NBufferCnt = 1;
        PMU_stMSrfcDesc.stSrfcDesc.u8VideoOut = RSST_nCtrl_JCP2011MVO_0;
        //no need to initialize
        //PMU_stMSrfcDesc.stMgInfo.au8DepList[GBT__nNumberDep] = {0,0,0,0,0};
        PMU_stMSrfcDesc.stMgInfo.u16ResID = 0xFFFF;
        PMU_stMSrfcDesc.stMgInfo.u8Attr = 0;
        PMU_stMSrfcDesc.stMgInfo.u8CPolicy = GBT_nCPolicy_No;
        PMU_stMSrfcDesc.stMgInfo.u8ObjCtrlLoc = PMU_ObjCtrlLoc;
        PMU_stMSrfcDesc.stMgInfo.u8ObjDataLoc = PMU_ObjDataLoc;
        PMU_stMSrfcDesc.stMgInfo.u8ObjSrc = GBT_nObjSrc_RSST;
        PMU_u16SurfaceID = GRLC_u16SurfaceCreateDyn(&PMU_stMSrfcDesc);
        GRLC_vSetSurfaceID(PMU_u16ContextID, PMU_u16SurfaceID);
        GRLC_vSurfaceSetPosition(PMU_u16SurfaceID, PMU_OFFSET_X, PMU_OFFSET_Y);
        GRLC_vPrimSetComposition(PMU_u16ContextID, GBT_nCompose_Overwrite, GBT_nROP2_Src, GBT_nMapOpt_Auto, NULL, False);
        PMU::JCP2011GAD::PMU_vPreRecalcPrompt();
        //GRLC_vSurfaceFlip(PMU_u16SurfaceID); 

        GRLC_vSurfaceSetVisibility(PMU_u16SurfaceID, True);
#if (SET_ZORDER_MANUALLY == On)
        //note: set layer order
        uint16 au16SrfcIDList[PMU_NoOfSurfaces] = PMU_SurfaceAssembly;
        for (u8ContextID = 0; u8ContextID<static_cast<RSST_tenCtxtDescID>(RSST_nLastCtxtDesc - 1); u8ContextID++)
        {
            au16SrfcIDList[PMU_NoOfSurfaces - ++u8ContextID] = GRLC_u16GetSurfaceID(u8ContextID);
        }
        GBT_tstSrfList stSrfList;
        stSrfList.pau16SrfcIDList = au16SrfcIDList;
        stSrfList.u16NumberSurfaces = PMU_NoOfSurfaces;

        GRLC_vVideoOutSetLayerZOrder(RSST_nDisplayID0, &stSrfList);
#endif

    }

#endif
    void PMU_vInitPFC_JCP2011(void)
    {
        NEC_vSetDRWPERFTRIGGER(0x00030001, 0);
    }
}

GBT_tstMText PMU_Text_CPU_MAX, PMU_Text_GPU_MAX, PMU_Text_FPS_MAX, PMU_Const_String_MAX;
char *pPMU_Text_CPU_MAX, *pPMU_Text_GPU_MAX, *pPMU_Text_FPS_MAX;

char chPMU_Const_String_MAX[] = "MAX";

using namespace PMU;


void PMU::JCP2011GAD::PMU_vMaxCPULoadPrompt(void)
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
    sprintf(tempCPU_MAX, "%0.2f%%", PerformanceCPUJCP2011::vReturnCPUObject()->PMU_fCPU_MAX);
    pPMU_Text_CPU_MAX = tempCPU_MAX;
    PMU_Text_CPU_MAX.stText.pvTextData = pPMU_Text_CPU_MAX;
    PMU_Text_CPU_MAX.stText.u16TextByteLen = strlen(tempCPU_MAX);
    GRLC_vDrawMTextAt(PMU_u16ContextID, &PMU_Text_CPU_MAX, PMU_BMP_SIZE_X * 2 / 3, 20);
    ////GRLC_vDrawTextIDAt(PMU_u16ContextID,RSST_nTXT_Sample_All_Content,0,0);
    GRLC_vReleaseMText(PMU_u16ContextID, &PMU_Text_CPU_MAX);

}

void PMU::JCP2011GAD::PMU_vMaxGPULoadPrompt(void)
{
    PMU_Text_GPU_MAX.stText.u16TextByteLen = 0xFFFF;
    PMU_Text_GPU_MAX.stText.u8Attributes = 0x00; // 8-bit
    PMU_Text_GPU_MAX.stText.u8Compression = GBT_nComp_No;
    PMU_Text_GPU_MAX.stMgInfo.u16ResID = 0xffff;
    PMU_Text_GPU_MAX.stMgInfo.u8Attr = 0;
    PMU_Text_GPU_MAX.stMgInfo.u8CPolicy = GBT_nCPolicy_No;
    PMU_Text_GPU_MAX.stMgInfo.u8ObjCtrlLoc = PMU_TextLoc;
    PMU_Text_GPU_MAX.stMgInfo.u8ObjDataLoc = PMU_TextLoc;
    char tempGPU_MAX[10] = "";
    sprintf(tempGPU_MAX, "%0.2f%%", PerformanceGPUJCP2011GAD::vReturnGPUObject()->PMU_fGPU_MAX);
    pPMU_Text_GPU_MAX = tempGPU_MAX;
    PMU_Text_GPU_MAX.stText.pvTextData = pPMU_Text_GPU_MAX;
    PMU_Text_GPU_MAX.stText.u16TextByteLen = strlen(tempGPU_MAX);
    GRLC_vDrawMTextAt(PMU_u16ContextID, &PMU_Text_GPU_MAX, PMU_BMP_SIZE_X * 2 / 3, 40);
    ////GRLC_vDrawTextIDAt(PMU_u16ContextID,RSST_nTXT_Sample_All_Content,0,0);
    GRLC_vReleaseMText(PMU_u16ContextID, &PMU_Text_GPU_MAX);
}

void PMU::JCP2011GAD::PMU_vMaxStringPrompt(void)
{
    PMU_Const_String_MAX.stText.u16TextByteLen = 0xFFFF;
    PMU_Const_String_MAX.stText.u8Attributes = 0x00; // 8-bit
    PMU_Const_String_MAX.stText.u8Compression = GBT_nComp_No;
    PMU_Const_String_MAX.stMgInfo.u16ResID = 0xffff;
    PMU_Const_String_MAX.stMgInfo.u8Attr = 0;
    PMU_Const_String_MAX.stMgInfo.u8CPolicy = GBT_nCPolicy_No;
    PMU_Const_String_MAX.stMgInfo.u8ObjCtrlLoc = PMU_TextLoc;
    PMU_Const_String_MAX.stMgInfo.u8ObjDataLoc = PMU_TextLoc;
    PMU_Const_String_MAX.stText.pvTextData = static_cast<void*>(::chPMU_Const_String_MAX);
    PMU_Const_String_MAX.stText.u16TextByteLen = sizeof ::chPMU_Const_String_MAX;
    GRLC_vDrawMTextAt(PMU_u16ContextID, &PMU_Const_String_MAX, PMU_BMP_SIZE_X * 3 / 4 + 3, 0);
    ////GRLC_vDrawTextIDAt(PMU_u16ContextID,RSST_nTXT_Sample_All_Content,0,0);
    GRLC_vReleaseMText(PMU_u16ContextID, &PMU_Const_String_MAX);

}

void PMU::JCP2011GAD::PMU_vMaxFPSLoadPrompt(void)
{
    PMU_Text_FPS_MAX.stText.u16TextByteLen = 0xFFFF;
    PMU_Text_FPS_MAX.stText.u8Attributes = 0x00; // 8-bit
    PMU_Text_FPS_MAX.stText.u8Compression = GBT_nComp_No;
    PMU_Text_FPS_MAX.stMgInfo.u16ResID = 0xffff;
    PMU_Text_FPS_MAX.stMgInfo.u8Attr = 0;
    PMU_Text_FPS_MAX.stMgInfo.u8CPolicy = GBT_nCPolicy_No;
    PMU_Text_FPS_MAX.stMgInfo.u8ObjCtrlLoc = PMU_TextLoc;
    PMU_Text_FPS_MAX.stMgInfo.u8ObjDataLoc = PMU_TextLoc;
    char tempFPS_MAX[10] = "";
    sprintf(tempFPS_MAX, "%d", PerformanceGPUJCP2011GAD::vReturnGPUObject()->PMU_u32FPS_MAX);
    pPMU_Text_FPS_MAX = tempFPS_MAX;
    PMU_Text_FPS_MAX.stText.pvTextData = pPMU_Text_FPS_MAX;
    PMU_Text_FPS_MAX.stText.u16TextByteLen = strlen(tempFPS_MAX);
    GRLC_vDrawMTextAt(PMU_u16ContextID, &PMU_Text_FPS_MAX, PMU_BMP_SIZE_X * 2 / 3 + 20, 60);
    ////GRLC_vDrawTextIDAt(PMU_u16ContextID,RSST_nTXT_Sample_All_Content,0,0);
    GRLC_vReleaseMText(PMU_u16ContextID, &PMU_Text_FPS_MAX);
}

void PMU::JCP2011GAD::PMU_vBandwidthNotSupport(void)
{
    GBT_tstMText PMU_Text_BandWidth;
    memset(&PMU_Text_BandWidth, 0, sizeof PMU_Text_BandWidth);
    PMU_Text_BandWidth.stText.u16TextByteLen = 0xFFFF;
    PMU_Text_BandWidth.stText.u8Attributes = 0x00; // 8-bit
    PMU_Text_BandWidth.stText.u8Compression = GBT_nComp_No;
    PMU_Text_BandWidth.stMgInfo.u16ResID = 0xffff;
    PMU_Text_BandWidth.stMgInfo.u8Attr = 0;
    PMU_Text_BandWidth.stMgInfo.u8CPolicy = GBT_nCPolicy_No;
    PMU_Text_BandWidth.stMgInfo.u8ObjCtrlLoc = PMU_TextLoc;
    PMU_Text_BandWidth.stMgInfo.u8ObjDataLoc = PMU_TextLoc;
    PMU_Text_BandWidth.stText.pvTextData = "No Bandwidth support";
    PMU_Text_BandWidth.stText.u16TextByteLen = strlen("No Bandwidth support");
    GRLC_vDrawMTextAt(PMU_u16ContextID, &PMU_Text_BandWidth, 0, 80);
    ////GRLC_vDrawTextIDAt(PMU_u16ContextID,RSST_nTXT_Sample_All_Content,0,0);
    GRLC_vReleaseMText(PMU_u16ContextID, &PMU_Text_BandWidth);
}

#endif 
