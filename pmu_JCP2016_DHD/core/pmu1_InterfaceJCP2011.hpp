#ifndef PMU_Interface_JCP2011_HPP
#define PMU_Interface_JCP2011_HPP

//#include "cdef.h"
#include "pmu_ci.h"
#include "pmu_ct.h"

#include "clock_c1.h"  // C Export wrapper

#ifdef JCP2011_CPU
extern "C" void PMU_vInitJCP2011(void);
extern "C" void PMU_vPreparePromptSurfaceJCP2011(void);
extern "C" void PMU_vInitPFC_JCP2011(void);

#ifdef __cplusplus //the interface is invoked from C style
//namespace PMU
//{
//	class PMUInterface
//	{
//	public:
#endif //end of __cplusplus

extern GBT_tstMText PMU_Text_CPU_MAX,PMU_Text_GPU_MAX,PMU_Text_FPS_MAX;
extern char *pPMU_Text_CPU_MAX,*pPMU_Text_GPU_MAX,*pPMU_Text_FPS_MAX;

namespace PMU
{
#if (PMU_STATISTICS_PIPELINE == PMU_USE_JCP2011_PIPELINE)
	namespace JCP2011GAD 
	{
		extern void PMU_vRecalcFPS(uint32&,uint16);
		extern void PMU_vPreRecalcPrompt(void);
		extern void PMU_vPostRecalcPrompt(void);
		void PMU_vMaxCPULoadPrompt(void);
		void PMU_vMaxGPULoadPrompt(void);
		void PMU_vMaxFPSLoadPrompt(void);
		void PMU_vMaxStringPrompt(void);
		void PMU_vBandwidthNotSupport(void);

	}
#else

#endif

}
#ifdef __cplusplus

	//};
	///** CLASS END */
//}
#endif
#endif //end of JCP2011_CPU
#endif //end of PMU_Interface_JCP2011_HPP