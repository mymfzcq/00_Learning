#include "pmu1__PerformanceGPUJCP2011GAD.hpp"

#ifdef JCP2011_CPU
namespace PMU
{
	float fTempGPU;
	uint32 u32TempFPS;
	volatile uint32 PerformanceGPUJCP2011GAD::u32BenchMarkCounters = 0;
	PerformanceGPUJCP2011GAD *PerformanceGPUJCP2011GAD::poPerformanceGPUJCP2011GAD = NULL;

	void PerformanceGPUJCP2011GAD::vInit(void)
	{
		uint8 u8ErrorCnt = 0;
		poPerformanceGPUJCP2011GAD = new PerformanceGPUJCP2011GAD(u8ErrorCnt);
		if (poPerformanceGPUJCP2011GAD == NULL || u8ErrorCnt> 0)
		{
			vGoFailure();
		}
	}

	void PerformanceGPUJCP2011GAD::vDeInit(void)
	{
		//
	}

	PerformanceGPUJCP2011GAD::PerformanceGPUJCP2011GAD(uint8 &u8ErrorCnt):DRW_PFCOUNTER1(0),DRW_PFCOUNTER2(0),DRW_boOverLoad(false),\
		PMU_fGPU_MAX(0.0f),PMU_u32FPS_MAX(0u)
	{
		PMU_u16CurrentCursor = 0;
		memset(PMU_au16BarGraph,0,sizeof PMU_au16BarGraph);
		PMU_pu16BarGraphBuffer = new uint16[PMU_BMP_SIZE_X/2];
		memset(PMU_pu16BarGraphBuffer,0,(sizeof (uint16))*PMU_BMP_SIZE_X/2);
		m_poStatistics = new Statistics(u8ErrorCnt,Type_GPU);
		if(m_poStatistics == NULL)
		{
			vGoFailure();
		}
	}

	PerformanceGPUJCP2011GAD::~PerformanceGPUJCP2011GAD(void)
	{
        delete m_poStatistics;
	}

	PerformanceGPUJCP2011GAD *PerformanceGPUJCP2011GAD::vReturnGPUObject(void)
	{
		return poPerformanceGPUJCP2011GAD;
	}

	uint32 PerformanceGPUJCP2011GAD::vGetCPUBenchMarkCounter(void)
	{
		return u32BenchMarkCounters;
	}

	void PerformanceGPUJCP2011GAD::vOutPut(void)
	{

		//#pragma asm-
		//	di
		//#pragma endasm
		//	ActivateTask(PMU_MainTask);
		//
		//#pragma asm
		//	ei
		//#pragma endasm
		//	// can dot schedule inside a interrupt
		//	Schedule();
		//// it seems to be a OS limitation of error code osdErrSHOnInterruptLevel and osdErrATMultipleActivation
		////so we add the GS code here to make it in advance compared with activate a task without calling schedule()
	}

	void PerformanceGPUJCP2011GAD::vOutPutGPUPerf(void)
	{
		//GAD_vJCP2011PerfCntSetMode( hSrfcDst32, 31, 1 ); // Cnt1 CPU cycles, Cnt2 2D active cycles
		//	GAD_vJCP2011PerfCntReset( hSrfcDst32, 1 );
		//	GAD_vJCP2011PerfCntReset( hSrfcDst32, 2 );
		//	GAD_vJCP2011DrawMesh( hSrfcDst32, hSrfc0, &stMesh, &stMeshOffset );
		//	GAD_vJCP2011PerfCntSetMode( hSrfcDst32, 0, 0 ); // Stop both counters

		//	SetPort( 0 );
		//	GAD_vFinish( hSrfcDst32 );
		//	SetPort( 1 );
		//	u32Cnt0++;
		//	u32Cnt1 += GAD_u32JCP2011PerfCntGet( hSrfcDst32, 1 ); 
		//step1: gpu load
		__DI();
		this->DRW_PFCOUNTER1 = NEC_u32GetDRWPERFCOUNT1();//GAD_u32JCP2011PerfCntGet(0,1);
		this->DRW_PFCOUNTER2 = NEC_u32GetDRWPERFCOUNT2();//GAD_u32JCP2011PerfCntGet(0,2);
		__EI();
		uint32 u32ErrorCode;
		char CharGPU[30] = "GPU: ";
		char ResultCharGPU[30] = "";
		uint16 ResultChar[30] = {0};
		strcat(ResultCharGPU,CharGPU);
		//(DRW_TRIGGER1_DLACTIVECYCLES|DRW_TRIGGER2_ACTIVECYCLES,0);
		__DI();
		if(DRW_PFCOUNTER1 == 0)
		{
			fTempGPU = 99.99f;
			DRW_boOverLoad = true;
		}
		else
		{
			fTempGPU = (float)DRW_PFCOUNTER1/(float)(PMAN__FreqPLL_PMAN_nDRW);
			fTempGPU *=100;
			if(fTempGPU > 1.001f)
			{
				fTempGPU-=1.0f;// offset of the gpu load
			}
			DRW_boOverLoad = false;
		}
		__EI();
		sprintf(CharGPU,"%0.2f",fTempGPU);
		strcat(ResultCharGPU,CharGPU);
		strcat(ResultCharGPU,"%");

		GAUXL_vCharToWChar(ResultChar,(const uint8*)ResultCharGPU,30,&u32ErrorCode);
		pPMU_Text_GPU = ResultChar;
		PMU_Text_GPU.stText.pvTextData = pPMU_Text_GPU;
		PMU_Text_GPU.stText.u16TextByteLen = 2*GAUXL_u16WStrlen(static_cast<const uint16*>(PMU_Text_GPU.stText.pvTextData),&u32ErrorCode);
		GRLC_vDrawMTextAt(PMU_u16ContextID,&PMU_Text_GPU,0,40);
		////GRLC_vDrawTextIDAt(PMU_u16ContextID,RSST_nTXT_Sample_All_Content,0,0);
		GRLC_vReleaseMText(PMU_u16ContextID,&PMU_Text_GPU);

		//step2: FPS
		memset(ResultChar,0,sizeof ResultChar);
		char CharFPS[30] = "FPS: ";
		char ResultCharFPS[30] = "";
		strcat(ResultCharFPS,CharFPS);
		//(DRW_TRIGGER1_DLACTIVECYCLES|DRW_TRIGGER2_ACTIVECYCLES,0);
		__DI();
		u32TempFPS = DRW_PFCOUNTER2/((uint32)RSST__stGSConfig.pastDispAttr[RSST_nDisplayID0].u16Width*(uint32)RSST__stGSConfig.pastDispAttr[RSST_nDisplayID0].u16Height);
		PMU::JCP2011GAD::PMU_vRecalcFPS(u32TempFPS,PMU_DefaultContext);	
		sprintf(CharFPS,"%d",u32TempFPS);
		strcat(ResultCharFPS,CharFPS);
		if(DRW_boOverLoad)
		{
			strcat(ResultCharFPS,"   ERROR!");
			GRLC_vTextSetDrawColor(PMU_u16ContextID,GBT_nRED_RGBA8888,PMU_APP_COLOR_FORMAT);
		}
		else
		{
			GRLC_vTextSetDrawColor(PMU_u16ContextID,GBT_nGREEN_RGBA8888,PMU_APP_COLOR_FORMAT);
		}
		__EI();
		GAUXL_vCharToWChar(ResultChar,(const uint8*)ResultCharFPS,30,&u32ErrorCode);
		pPMU_Text_FPS = ResultChar;
		PMU_Text_FPS.stText.pvTextData = pPMU_Text_FPS;
		PMU_Text_FPS.stText.u16TextByteLen = 2*GAUXL_u16WStrlen(static_cast<const uint16*>(PMU_Text_FPS.stText.pvTextData),&u32ErrorCode);
		GRLC_vDrawMTextAt(PMU_u16ContextID,&PMU_Text_FPS,0,60);
		////GRLC_vDrawTextIDAt(PMU_u16ContextID,RSST_nTXT_Sample_All_Content,0,0);
		GRLC_vReleaseMText(PMU_u16ContextID,&PMU_Text_FPS);
		//GAD_vJCP2011PerfCntSetMode(0,0,0);
		__DI();
		NEC_vSetDRWPERFTRIGGER(0x00030001,0);
		NEC_vSetDRWPERFCOUNT1(0);
		NEC_vSetDRWPERFCOUNT2(0);
		//GAD_vJCP2011PerfCntSetMode(0,1,10);
		//GAD_vJCP2011PerfCntReset(0,1);
		//GAD_vJCP2011PerfCntReset(0,2);
		__EI();
		PMU::JCP2011GAD::PMU_vPostRecalcPrompt();

		//step3:calculate maximum gpu load value
		if((fTempGPU-PMU_fGPU_MAX) > PMU_FLT_EPSILON)
		{
			PMU_fGPU_MAX = fTempGPU;
		}

		if((u32TempFPS>PMU_u32FPS_MAX))
		{
			PMU_u32FPS_MAX = u32TempFPS;
		}
		//step4:draw the GPU load bargraph,after drawing the texts,now we need to change the 2D style to draw the bargraph
		GRLC_vPrimSetCompCAlphaAttr(PMU_u16ContextID,True);
		GRLC_vPrimSetCompCAlpha(PMU_u16ContextID,PMU_SET_BGBAR_CA_VALUE);

		GRLC_vPrimSetDrawColor(PMU_u16ContextID, 0xffc60eff/*yellow*/, PMU_APP_COLOR_FORMAT);

		if(PMU_u16CurrentCursor < PMU_BMP_SIZE_X/2)
		{
			//operate on the bargraph array elements
			
		}
		else
		{
			PMU_u16CurrentCursor=0;
		}
		PMU_au16BarGraph[PMU_u16CurrentCursor] = (uint16)((fTempGPU/100.0f)*(PMU_BMP_SIZE_Y-105)+0.5f);
		if(PMU_au16BarGraph[PMU_u16CurrentCursor] == 0)
			PMU_au16BarGraph[PMU_u16CurrentCursor] = 1;

		//for(uint16 u16temp1=0;u16temp1<PMU_BMP_SIZE_X/2;u16temp1++)
		//{
		//	//make the array like a stack style
		//	PMU_au16BarGraph[u16temp1] = PMU_au16BarGraph[u16temp1+1];
		//
		//}//it is much more inefficient
		memcpy(PMU_pu16BarGraphBuffer,PMU_au16BarGraph+PMU_u16CurrentCursor+1,(sizeof (uint16))*(PMU_BMP_SIZE_X/2-PMU_u16CurrentCursor-1));
		memcpy(PMU_pu16BarGraphBuffer+PMU_BMP_SIZE_X/2-PMU_u16CurrentCursor-1,PMU_au16BarGraph,(sizeof (uint16))*(PMU_u16CurrentCursor+1));
		//then globally draw it in the loop
		for(uint16 u16temp2=0;u16temp2<PMU_BMP_SIZE_X/2;u16temp2++)	
		{
			//if(PMU_au16BarGraph[u16temp2] == 0)
			//{
			//	break;
			//}
			//else
			{
				GRLC_vDrawLine(PMU_u16ContextID,PMU_BMP_SIZE_X-(1+u16temp2),		PMU_BMP_SIZE_Y-1,	0/*x distance*/,	-*(PMU_pu16BarGraphBuffer+u16temp2)/*y distance*/);
			}
		}
		PMU_u16CurrentCursor++;
		GRLC_vPrimSetCompCAlphaAttr(PMU_u16ContextID,False);
	}


	// end of functions
}
#endif