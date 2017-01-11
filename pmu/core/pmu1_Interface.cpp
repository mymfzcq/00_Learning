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
	GBT_tstMText PMU_Text_Time,PMU_Text_CPU,PMU_Text_GPU,PMU_Text_FPS;
	uint16 *pPMU_Text_Time,*pPMU_Text_CPU,*pPMU_Text_GPU,*pPMU_Text_FPS;

	DeclareAlarm(PMU_1000msAlarm);
	DeclareTask(PMU_1000ms);
	DeclareTask(PMU_MainTask);
	DECLARESURFACE;
	//extern GRLC__tstContext* GRLC__pstGetContext( uint16 u16ContextID );
	//extern volatile uint8 GRLC__astContext[RSST_nLastCtxtDesc];
	ISR(PMU_TAUA0106_ISR)
	{

		TIMD_vDisIntAndStop0106();
	}

	TASK(PMU_1000ms)
	{
		if(	PMU__boSWInit == false)
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
				uint32 u32result1,u32result2,u32result3;
				//phase 1
				TIMD_vResetReloadInt0106(TIMD_xUSecToValue0106(100,NULL));
				u32result1 = 0;
#pragma asm
				di
#pragma endasm
					while (False == TIMD_boTimerElapsed0106())
					{
						u32result1++;
					}
#pragma asm
					ei
#pragma endasm
						//phase2
						TIMD_vResetReloadInt0106(TIMD_xUSecToValue0106(1000,NULL));
					u32result2 = 0;
#pragma asm
					di
#pragma endasm
						while (False == TIMD_boTimerElapsed0106())
						{
							u32result2++;
						}
#pragma asm
						ei
#pragma endasm
							//phase3
							TIMD_vResetReloadInt0106(TIMD_xUSecToValue0106(10000,NULL));
						u32result3 = 0;
#pragma asm
						di
#pragma endasm
							while (False == TIMD_boTimerElapsed0106())
							{
								u32result3++;
							}
#pragma asm
							ei
#pragma endasm
							// PMU_u32BenchMarkValue indicates the 1s cpu add values;
							PMU_u32BenchMarkValue = (u32result1*100+u32result2*10+u32result3)/3*100;
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
				//GAD_vJCP2011PerfCntSetMode(0,31,1);
				NEC_vSetDRWPERFTRIGGER(0x00030001,0);
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
				if(boGPUMatchCPUFlag == true)
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
	PMU_u32StatisticsCounters = 0;
	PMU_u16ContextID = 0;
	PMU_u16SurfaceID = 0;
	PMU_enTaskFlag = enInitialMode;
	boGPUMatchCPUFlag = true;
	PMU::PerformanceCPUJCP2011::vInit();
#if (PMU_STATISTICS_PIPELINE == PMU_USE_JCP2011_PIPELINE)
	GBT_tstAlphaMap stIniAlphaMap = {NULL,0,0,0,0};
	GBT_tstCLUT stInistCLUT = {NULL,0,0};
	//memset(&stTimeData,0,sizeof stTimeData);
	memset(&PMU_stBmp,0,sizeof PMU_stBmp);
	memset(&PMU_stMSrfcDesc,0,sizeof PMU_stMSrfcDesc);

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
#endif
}

void PMU_vCPUIdleTaskHook(void)
{
	//enhance the credibility so we use the minus operation
	//if(PMU__boSWInit)
	//{
	PMU::PerformanceCPUJCP2011::vCPUIdleHook();
	//}
}

void PMU_vCPUGetPerformanceBenchMark(PMU_tenCPUType enCPUType,uint32 PMU_u32BenchMarkTime)
{
#pragma asm
	di
#pragma endasm
		//start pmu task
	PMU_enTaskFlag = enBenchmarkMode;
	ActivateTask(PMU_MainTask);
#pragma asm
	ei
#pragma endasm
	Schedule();
}

void vSetGPURefreshCycle(uint16 u16MicroSeconds)
{
	if(u16MicroSeconds == 1000)
	{
		boGPUMatchCPUFlag = true;
	}
	else
	{
		boGPUMatchCPUFlag= false;
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
	SetRelAlarm(PMU_1000msAlarm,MSEC(35),MSEC(1000));

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
	PMU_u16ContextID = GRLC_u16GetRenderContext ( static_cast<RSST_tenCtxtDescID>(RSST_nLastCtxtDesc-1));
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
	GRLC_vSetSurfaceID(PMU_u16ContextID,PMU_u16SurfaceID);
	GRLC_vSurfaceSetPosition(PMU_u16SurfaceID,PMU_OFFSET_X,PMU_OFFSET_Y);
    GRLC_vPrimSetComposition(PMU_u16ContextID, GBT_nCompose_Overwrite, GBT_nROP2_Src, GBT_nMapOpt_Auto, NULL, False);
	PMU::JCP2011GAD::PMU_vPreRecalcPrompt();
	//GRLC_vSurfaceFlip(PMU_u16SurfaceID); 

	GRLC_vSurfaceSetVisibility(PMU_u16SurfaceID, True);
#if (SET_ZORDER_MANUALLY == On)
	//note: set layer order
	uint16 au16SrfcIDList[PMU_NoOfSurfaces] =PMU_SurfaceAssembly;
	for(u8ContextID = 0;u8ContextID<static_cast<RSST_tenCtxtDescID>(RSST_nLastCtxtDesc-1);u8ContextID++)
	{
		au16SrfcIDList[PMU_NoOfSurfaces-++u8ContextID]=GRLC_u16GetSurfaceID(u8ContextID);
	}
	GBT_tstSrfList stSrfList;
	stSrfList.pau16SrfcIDList = au16SrfcIDList;
	stSrfList.u16NumberSurfaces = PMU_NoOfSurfaces;

	GRLC_vVideoOutSetLayerZOrder(RSST_nDisplayID0,&stSrfList);
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
	PMU::PerformanceCPUJCP2011::u32BenchMarkCounters = 0;
	__EI();
}

void vGoFailure(void)
{
	ASRL_vAssert(false);
}