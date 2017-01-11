#ifndef PMU_Statistics_HPP
#define PMU_Statistics_HPP

#include "PMU_ci.h"

extern "C" 
{
#include "CLOCK_C1.h"
#include "pmu1_Interf_ce.h"
}

namespace PMU
{

	class PerformanceCPUJCP2011;
    class PerformanceGPUJCP2011GAD;

	class Statistics
	{


	public:
		//default constructor
		Statistics(void);

		explicit Statistics(uint8 &u8ErrorCnt,PMU_tenMeasurementType enMeasurementType);

	    virtual ~Statistics(void);

		virtual bool vInit(PMU_tenMeasurementType enMeasurementType);

		virtual void vDeInit(void);

	    static PMU_tenMeasurementType vGetMeasurememtType(HSTATISTICS hInstance);
		
	    static Statistics* vGetMeasurememtObject(HSTATISTICS hInstance);

		static void vStartTimerTracking(void);

		static void vEndTimerTracking(void);

		static void vExpiredTimeHandler(void);

		static inline void vSetHandleID(HSTATISTICS hInstance,uint8 hID)
		{
			hInstance->unused = hID;
		}

		static inline uint8 vGetHandleID(HSTATISTICS hInstance)
		{
			return hInstance->unused;
		}

		static volatile CLOCK_tstTimeDate stTimeData;
	
		static HSTATISTICS vReturnFromObject(void* pObject,PMU_tenMeasurementType);

		//static CLOCK_tstTimeDate* vGetExpiredTime(void);

	private:
		static Statistics* poStatistics[];

		uint16 m_u16RefreshCycles;

		HSTATISTICS m_HStatistics;

		PMU_tenMeasurementType m_enMeasurementType;
		


	};
	/** *  CLASS END */

}	// End of namespace


#endif //end of PMU_Statistics_HPP