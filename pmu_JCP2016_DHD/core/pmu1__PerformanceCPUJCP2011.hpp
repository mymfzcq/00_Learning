#ifndef PMU_PerformanceCPUJCP2011_HPP
#define PMU_PerformanceCPUJCP2011_HPP

#include "PMU_ci.h"
#include "pmu1__Performance.hpp"

#ifdef JCP2011_CPU
extern "C"
{

#include "gauxl_c1.h"
#include "gad_c1.h"
#include "pman_c1.h"
#include "rsst_c1.h"
#include "stdio.h"  // C Export wrapper
#include "grlc_c1.h"  // C Export wrapper
#include "RSST_c1.h"  // C Export wrapper
#include "CLOCK_C1.h"  // C Export wrapper
	extern uint16 PMU_u16ContextID;
	extern uint16 PMU_u16SurfaceID;
	extern 	GBT_tstMText PMU_Text_Time,PMU_Text_CPU;
	extern	uint16 *pPMU_Text_Time,*pPMU_Text_CPU;

	extern volatile uint32 PMU_u32BenchMarkValue;
	extern volatile uint32 u32TempCPUCnt;
	extern volatile float fTempCPU;
}
namespace PMU
{

	class PerformanceCPUJCP2011:public Performance
	{
	public:
		friend class Statistics;


		PerformanceCPUJCP2011(uint8 &u8ErrorCnt);
		~PerformanceCPUJCP2011(void);
		static void vInit(void);
		static void vDeInit(void);
		//static void vCPUIdleHook(void);
		static inline void PerformanceCPUJCP2011::vCPUIdleHook(void)
		{
			// this will increase the time of ++ operation so the factor is 1.305
			__DI();
			u32BenchMarkCounters++;
			__EI();
		}

		static uint32 vGetCPUBenchMarkCounter(void);

		void vOutPutCPUPerf(void);

		virtual void vOutPut(void);

		static volatile uint32 u32BenchMarkCounters;

		static PerformanceCPUJCP2011 *vReturnCPUObject(void);

		float PMU_fCPU_MAX;
	protected:
		uint16 PMU_u16CurrentCursor;
		uint16 PMU_au16BarGraph[PMU_BMP_SIZE_X/2];
		uint16 *PMU_pu16BarGraphBuffer;
	private:
		static PerformanceCPUJCP2011 *poPerformanceCPUJCP2011;

	};
	/** *  CLASS END */

}	// End of namespace

#endif //end of JCP2011_CPU
#endif //end of PMU_PerformanceCPUJCP2011_HPP