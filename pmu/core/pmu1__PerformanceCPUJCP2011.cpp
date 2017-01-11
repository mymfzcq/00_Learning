#include "pmu1__PerformanceCPUJCP2011.hpp"

namespace PMU
{

	volatile uint32 PerformanceCPUJCP2011::u32BenchMarkCounters = 0;
	PerformanceCPUJCP2011 *PerformanceCPUJCP2011::poPerformanceCPUJCP2011 = NULL;

	void PerformanceCPUJCP2011::vInit(void)
	{
		uint8 u8ErrorCnt = 0;
		poPerformanceCPUJCP2011 = new PerformanceCPUJCP2011(u8ErrorCnt);
		if (poPerformanceCPUJCP2011 == NULL || u8ErrorCnt> 0)
		{
			vGoFailure();
		}
	}

	void PerformanceCPUJCP2011::vDeInit(void)
	{

	}

	PerformanceCPUJCP2011::PerformanceCPUJCP2011(uint8 &u8ErrorCnt):PMU_fCPU_MAX(0.0)
	{
		PMU_u16CurrentCursor = 0;
		memset(PMU_au16BarGraph,0,sizeof PMU_au16BarGraph);
		PMU_pu16BarGraphBuffer = new uint16[PMU_BMP_SIZE_X/2];
		memset(PMU_pu16BarGraphBuffer,0,(sizeof (uint16))*PMU_BMP_SIZE_X/2);
		m_poStatistics = new Statistics(u8ErrorCnt,Type_CPU);
		if(m_poStatistics == NULL)
		{
			vGoFailure();
		}
	}

	PerformanceCPUJCP2011::~PerformanceCPUJCP2011(void)
	{

	}

	PerformanceCPUJCP2011 *PerformanceCPUJCP2011::vReturnCPUObject(void)
	{
		return poPerformanceCPUJCP2011;
	}

	uint32 PerformanceCPUJCP2011::vGetCPUBenchMarkCounter(void)
	{
		return u32BenchMarkCounters;
	}

	void PerformanceCPUJCP2011::vOutPut(void)
	{

#pragma asm-
		di
#pragma endasm
			ActivateTask(PMU_MainTask);

#pragma asm
		ei
#pragma endasm
			// can not schedule inside a interrupt
			Schedule();
	}

	void PerformanceCPUJCP2011::vOutPutCPUPerf(void)
	{
		//GAD_vJCP2011PerfCntSetMode(0,0,0);
		//GAD_vJCP2011PerfCntReset(0,1);
		//GAD_vJCP2011PerfCntReset(0,2);
		//FRT_vWaitnMicroSec(20000);
		PMU::JCP2011GAD::PMU_vPreRecalcPrompt();
		uint32 u32ErrorCode;
//stage1:time spend
		CLOCK_tstTimeDate* pstTempTime = NULL;
		CLOCK_tstTimeDate stTempTime;
		char TempChar[30] = "Time: ";
		char TempCharHour[3] = "";
		char TempCharMinute[3] = "";
		char TempCharSecond[3] = "";
		memset(&stTempTime,0,sizeof(CLOCK_tstTimeDate));
		pstTempTime = &stTempTime;
		vGetExpiredTime(pstTempTime);
		sprintf(TempCharHour,"%02d",pstTempTime->unTime.stTime.u8Hour);
		sprintf(TempCharMinute,"%02d",pstTempTime->unTime.stTime.u8Minute);
		sprintf(TempCharSecond,"%02d",pstTempTime->unTime.stTime.u8Second);
		strcat(TempChar,TempCharHour);
		strcat(TempChar,":");
		strcat(TempChar,TempCharMinute);
		strcat(TempChar,":");
		strcat(TempChar,TempCharSecond);
		TempChar[14] = 0;
		uint16 ResultChar[30] = {0};
		GAUXL_vCharToWChar(ResultChar,(const uint8*)TempChar,30,&u32ErrorCode);
		pPMU_Text_Time = ResultChar;
		PMU_Text_Time.stText.pvTextData = pPMU_Text_Time;
		PMU_Text_Time.stText.u16TextByteLen = 2*GAUXL_u16WStrlen(static_cast<const uint16*>(PMU_Text_Time.stText.pvTextData),&u32ErrorCode);
		//prompt expired time and cpu load	
		GRLC_vDrawMTextAt(PMU_u16ContextID,&PMU_Text_Time,0,0);
		//GRLC_vDrawTextIDAt(PMU_u16ContextID,RSST_nTXT_Sample_All_Content,0,0);
		GRLC_vReleaseMText(PMU_u16ContextID,&PMU_Text_Time);

//stage2:cpu usage
		__DI();
		u32TempCPUCnt = PMU::PerformanceCPUJCP2011::vGetCPUBenchMarkCounter();
		fTempCPU = (float)u32TempCPUCnt/(float)PMU_u32BenchMarkValue;
		fTempCPU *= fCPUFactor;//1.305f;
		fTempCPU = 1.0f-fTempCPU;
		fTempCPU *= 100;
		__EI();
		char TempCharCPU[10] = "";
		memset(TempChar,0,sizeof TempChar);
		memset(ResultChar,0,sizeof ResultChar);
		strcpy(TempChar,"CPU: ");
		sprintf(TempCharCPU,"%0.2f",fTempCPU);
		strcat(TempChar,TempCharCPU);
		strcat(TempChar,"%");
		GAUXL_vCharToWChar(ResultChar,(const uint8*)TempChar,30,&u32ErrorCode);
		pPMU_Text_CPU = ResultChar;
		PMU_Text_CPU.stText.pvTextData = pPMU_Text_CPU;
		PMU_Text_CPU.stText.u16TextByteLen = 2*GAUXL_u16WStrlen(static_cast<const uint16*>(PMU_Text_CPU.stText.pvTextData),&u32ErrorCode);
		GRLC_vDrawMTextAt(PMU_u16ContextID,&PMU_Text_CPU,0,20);
		////GRLC_vDrawTextIDAt(PMU_u16ContextID,RSST_nTXT_Sample_All_Content,0,0);
		GRLC_vReleaseMText(PMU_u16ContextID,&PMU_Text_CPU);

//stage3:calculate maximum load value
		if((fTempCPU-PMU_fCPU_MAX) > PMU_FLT_EPSILON)
		{
			PMU_fCPU_MAX = fTempCPU;
		}
//step4:draw the CPU load bargraph,after drawing the texts,now we need to change the 2D style to draw the bargraph
		GRLC_vPrimSetCompCAlphaAttr(PMU_u16ContextID,True);
		GRLC_vPrimSetCompCAlpha(PMU_u16ContextID,PMU_SET_BGBAR_CA_VALUE);

		GRLC_vPrimSetDrawColor(PMU_u16ContextID, GBT_nWHITE_RGBA8888, PMU_APP_COLOR_FORMAT);

		if(PMU_u16CurrentCursor < PMU_BMP_SIZE_X/2)
		{
			//operate on the bargraph array elements
			
		}
		else
		{
			PMU_u16CurrentCursor=0;
		}
		PMU_au16BarGraph[PMU_u16CurrentCursor] = (uint16)((fTempCPU/100.0f)*(PMU_BMP_SIZE_Y-105)+0.5f);
		if(PMU_au16BarGraph[PMU_u16CurrentCursor] == 0)
			PMU_au16BarGraph[PMU_u16CurrentCursor] = 1;

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
				GRLC_vDrawLine(PMU_u16ContextID,PMU_BMP_SIZE_X/2-(1+u16temp2),		PMU_BMP_SIZE_Y-1,	0/*x distance*/,	-*(PMU_pu16BarGraphBuffer+u16temp2)/*y distance*/);
			}
		}
		PMU_u16CurrentCursor++;
		GRLC_vPrimSetCompCAlphaAttr(PMU_u16ContextID,False);

	}


	// end of functions
}
