#ifndef PMU_Interface_H
#define PMU_Interface_H

//#include "cdef.h"
#ifdef __cplusplus
#include "pmu1_InterfaceJCP2011.hpp"
#endif
#include "pmu_ci.h"
#include "pmu_ct.h"

#include "clock_c1.h"  // C Export wrapper

#ifdef __cplusplus //the interface is invoked from C style
//namespace PMU
//{
//	class PMUInterface
//	{
//	public:
#endif //end of __cplusplus
	extern uint16 PMU_u16ContextID;
	extern uint16 PMU_u16SurfaceID;

		void PMU_vInit(void);

		void PMU_vCPUIdleTaskHook(void);

		void PMU_vCPUGetPerformanceBenchMark(PMU_tenCPUType enCPUType,uint32 PMU_u32BenchMarkTime);

		void PMU_vCreateWindow(void);

		void PMU_vShowResult(void);

		void vGoFailure(void);

		void vSetGPURefreshCycle(uint16);

		uint16 vGetGPURefreshCycle(void);

		void vStartMeasurement(void);

		void vStopMeasurement(void);

		void PMU_1000msHandler(void);

		void vSecondAccumulationHandler(void);

		void vPreparePromptSurface(void);

		void vGetExpiredTime(CLOCK_tstTimeDate* pstTemp);

		void vClearIDLEHook(void);

#ifdef __cplusplus

	//};
	///** CLASS END */
//}
#endif


#endif //end of PMU_Interface_H