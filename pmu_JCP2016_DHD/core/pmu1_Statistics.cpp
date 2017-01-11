#include "pmu1_Statistics.hpp"
#if (PMU_STATISTICS_PIPELINE == PMU_USE_JCP2011_PIPELINE)
#include "pmu1__PerformanceCPUJCP2011.hpp"
#include "pmu1__PerformanceGPUJCP2011GAD.hpp"
#elif  (PMU_STATISTICS_PIPELINE == PMU_USE_DHD_PIPELINE)
#include "pmu1__PerformanceCPUJCP2016.hpp"
#include "pmu1__PerformanceGPUJCP2016DHD.hpp"
#endif

extern "C"
{
#if (CLOCK__nDateUsed_Cfg == Yes) 
CLOCK_tstTimeDate PMU_stAddSecond = 
	{
		{1,0,0,0}, 
		/**
		* Date Information
		*/
		{8,4,2016}
	};
#else
CLOCK_tstTimeDate PMU_stAddSecond = 
	{
		{1,0,0,0}	 
	};
#endif /*(CLOCK__nDateUsed_Cfg == Yes) */
extern volatile uint32 PMU_u32StatisticsCounters;
extern PMU_tenTaskMode PMU_enTaskFlag;
}


namespace PMU
{


#if (CLOCK__nDateUsed_Cfg == Yes) 
volatile CLOCK_tstTimeDate Statistics::stTimeData =
	{
		{0,0,0,0}, 
		/**
		* Date Information
		*/
		{0,0,0}
	};
#else
volatile CLOCK_tstTimeDate Statistics::stTimeData =
	{
		{0,0,0,0}	 
	};
#endif /*(CLOCK__nDateUsed_Cfg == Yes) */
	Statistics* Statistics::poStatistics[NoOfMeasurementUnits] = {NULL,NULL};

	//functions

	Statistics::Statistics(uint8& u8ErrorCnt,PMU_tenMeasurementType enMeasurementType) \
		:m_u16RefreshCycles(RefreshCycleInvalid)
	{
		if(!vInit(enMeasurementType))
		{	
			u8ErrorCnt++;
		}
		if(enMeasurementType == Type_CPU)
		{
            HSTATISTICS hCPU = new HSTATISTICS__();
			vSetHandleID(hCPU,(uint8)Type_CPU);
			m_HStatistics = hCPU;
		}
		else if(enMeasurementType == Type_GPU)
		{
            HSTATISTICS hGPU = new HSTATISTICS__();
			vSetHandleID(hGPU,(uint8)Type_GPU);
			m_HStatistics = hGPU;
		}
		else
		{
			u8ErrorCnt++;
		}
	}

	Statistics::~Statistics(void)
	{
		//uint8 hID = vGetHandleID(m_HStatistics)
			//poStatistics[hID]=NULL;		
		vDeInit();
	}

	bool Statistics::vInit(PMU_tenMeasurementType enMeasurementType)
	{
		bool boRet = false;
		if(enMeasurementType>NoOfMeasurementTypes)
		{
			vGoFailure();
			return boRet;
		}
		else
		{
			//vSetHandleID(m_HStatistics,static_cast<uint8>(enMeasurementType));
		}
		if (enMeasurementType==Type_CPU)
		{
			//we can use this pointer but should be carefully
			m_enMeasurementType = Type_CPU;
			if (poStatistics[static_cast<uint8>(enMeasurementType)+1] == NULL)
			{
				poStatistics[static_cast<uint8>(enMeasurementType)] = this;
			}
			boRet=true;
		}
		else if (enMeasurementType == Type_GPU)
		{
			m_enMeasurementType = Type_GPU;
			if (poStatistics[static_cast<uint8>(enMeasurementType)-1])
			{
				poStatistics[static_cast<uint8>(enMeasurementType)] = this;
			}
			boRet=true;
		}
		return boRet;
	}

	void Statistics::vDeInit(void)
	{
		//nothing 
        delete m_HStatistics;
	}

    PMU_tenMeasurementType Statistics::vGetMeasurememtType(HSTATISTICS hInstance)
	{
		uint8 hID = vGetHandleID(hInstance);
		return 	poStatistics[hID]->m_enMeasurementType;
	}

    Statistics* Statistics::vGetMeasurememtObject(HSTATISTICS hInstance)
	{
		uint8 hID = vGetHandleID(hInstance);
		return 	poStatistics[hID];
	}

	void Statistics::vStartTimerTracking(void)
	{
		//perform the statistics
		//if (Type_CPU == vGetMeasurememtType(hCPU))
		//{
		//
		//}
		//else if (Type_GPU == vGetMeasurememtType(hGPU))
		//{
		//
		//}
		//else
		//{
		//	// no handler
		//}
		//output time info		
		//this is for GPU performance synchronous mode
#ifdef JCP2011_CPU 
        HSTATISTICS HGPU = vReturnFromObject(static_cast<void*>(PMU::PerformanceGPUJCP2011GAD::vReturnGPUObject()),Type_GPU);
#elif (defined JCP2016_CPU && PMU_STATISTICS_PIPELINE == PMU_USE_DHD_PIPELINE)
        HSTATISTICS HGPU = vReturnFromObject(static_cast<void*>(PMU::PerformanceGPUJCP2016DHD::vReturnGPUObject()), Type_GPU);
#endif
		if (::PMU_enTaskFlag == enCPUOutputMode)
		{
#ifdef JCP2011_CPU 
            PMU::PerformanceCPUJCP2011::vReturnCPUObject()->vOutPutCPUPerf();
#elif defined JCP2016_CPU
            PMU::PerformanceCPUJCP2016::vReturnCPUObject()->vOutPutCPUPerf();
#endif
		}
		else if (::PMU_enTaskFlag == enGPUOutputMode)
		{
#if (PMU_STATISTICS_PIPELINE == PMU_USE_JCP2011_PIPELINE)
            PMU::PerformanceGPUJCP2011GAD::vReturnGPUObject()->vOutPutGPUPerf();
#elif (PMU_STATISTICS_PIPELINE == PMU_USE_DHD_PIPELINE)
            PMU::PerformanceGPUJCP2016DHD::vReturnGPUObject()->vOutPutGPUPerf();
#endif
		}
	}

	void Statistics::vEndTimerTracking(void)
	{
		
	}

	void Statistics::vExpiredTimeHandler(void)
	{
#ifdef JCP2011_CPU 
        PMU::PerformanceCPUJCP2011::vReturnCPUObject()->vOutPut();
#elif defined JCP2016_CPU
        PMU::PerformanceCPUJCP2016::vReturnCPUObject()->vOutPut();
#endif
		//update the current time/date
		CLOCK_boInterfaceAddTimeDate(const_cast<CLOCK_tstTimeDate*>(&stTimeData),&PMU_stAddSecond);
		PMU_u32StatisticsCounters++;
	}

	HSTATISTICS Statistics::vReturnFromObject(void* pObject,PMU_tenMeasurementType enType)
	{
		HSTATISTICS hRET;
		switch (enType)
		{
		case Type_CPU:
			{
				//no cpu special handler
				break;
			}
		case Type_GPU:
			{
#ifdef JCP2011_CPU 
            hRET = static_cast<PerformanceGPUJCP2011GAD*>(pObject)->m_poStatistics->m_HStatistics;
#elif (defined JCP2016_CPU && PMU_STATISTICS_PIPELINE == PMU_USE_DHD_PIPELINE)
            hRET = static_cast<PerformanceGPUJCP2016DHD*>(pObject)->m_poStatistics->m_HStatistics;
#endif
				break;
			}
		default:
			vGoFailure();
			break;
		}
		return hRET;
	}
	// end of functions
}