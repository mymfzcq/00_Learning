#include "pmu_ci.h"
#include "pmu_ct.h"
//#include "pmu_InterfaceJCP2011.h"
#include "pmu1_Statistics.hpp"
#include "pmu1__Performance.hpp"
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

#if (PMU_STATISTICS_PIPELINE == PMU_USE_JCP2011_PIPELINE)
#endif
}

GBT_tstMText PMU_Text_CPU_MAX,PMU_Text_GPU_MAX,PMU_Text_FPS_MAX,PMU_Const_String_MAX;
char *pPMU_Text_CPU_MAX,*pPMU_Text_GPU_MAX,*pPMU_Text_FPS_MAX;

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
	sprintf(tempCPU_MAX,"%0.2f%%",PerformanceCPUJCP2011::vReturnCPUObject()->PMU_fCPU_MAX);
	pPMU_Text_CPU_MAX = tempCPU_MAX;
	PMU_Text_CPU_MAX.stText.pvTextData = pPMU_Text_CPU_MAX;
	PMU_Text_CPU_MAX.stText.u16TextByteLen = strlen(tempCPU_MAX);
	GRLC_vDrawMTextAt(PMU_u16ContextID,&PMU_Text_CPU_MAX,PMU_BMP_SIZE_X*2/3,20);
	////GRLC_vDrawTextIDAt(PMU_u16ContextID,RSST_nTXT_Sample_All_Content,0,0);
	GRLC_vReleaseMText(PMU_u16ContextID,&PMU_Text_CPU_MAX);

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
	sprintf(tempGPU_MAX,"%0.2f%%",PerformanceGPUJCP2011GAD::vReturnGPUObject()->PMU_fGPU_MAX);
	pPMU_Text_GPU_MAX = tempGPU_MAX;
	PMU_Text_GPU_MAX.stText.pvTextData = pPMU_Text_GPU_MAX;
	PMU_Text_GPU_MAX.stText.u16TextByteLen = strlen(tempGPU_MAX);
	GRLC_vDrawMTextAt(PMU_u16ContextID,&PMU_Text_GPU_MAX,PMU_BMP_SIZE_X*2/3,40);
	////GRLC_vDrawTextIDAt(PMU_u16ContextID,RSST_nTXT_Sample_All_Content,0,0);
	GRLC_vReleaseMText(PMU_u16ContextID,&PMU_Text_GPU_MAX);
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
	GRLC_vDrawMTextAt(PMU_u16ContextID,&PMU_Const_String_MAX,PMU_BMP_SIZE_X*3/4+3,0);
	////GRLC_vDrawTextIDAt(PMU_u16ContextID,RSST_nTXT_Sample_All_Content,0,0);
	GRLC_vReleaseMText(PMU_u16ContextID,&PMU_Const_String_MAX);

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
	sprintf(tempFPS_MAX,"%d",PerformanceGPUJCP2011GAD::vReturnGPUObject()->PMU_u32FPS_MAX);
	pPMU_Text_FPS_MAX = tempFPS_MAX;
	PMU_Text_FPS_MAX.stText.pvTextData = pPMU_Text_FPS_MAX;
	PMU_Text_FPS_MAX.stText.u16TextByteLen = strlen(tempFPS_MAX);
	GRLC_vDrawMTextAt(PMU_u16ContextID,&PMU_Text_FPS_MAX,PMU_BMP_SIZE_X*2/3+20,60);
	////GRLC_vDrawTextIDAt(PMU_u16ContextID,RSST_nTXT_Sample_All_Content,0,0);
	GRLC_vReleaseMText(PMU_u16ContextID,&PMU_Text_FPS_MAX);
}

void PMU::JCP2011GAD::PMU_vBandwidthNotSupport(void)
{
	GBT_tstMText PMU_Text_BandWidth;
	memset(&PMU_Text_BandWidth,0,sizeof PMU_Text_BandWidth);
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
	GRLC_vDrawMTextAt(PMU_u16ContextID,&PMU_Text_BandWidth,0,80);
	////GRLC_vDrawTextIDAt(PMU_u16ContextID,RSST_nTXT_Sample_All_Content,0,0);
	GRLC_vReleaseMText(PMU_u16ContextID,&PMU_Text_BandWidth);
}


