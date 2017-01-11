#ifndef PMU_Interface_JCP2016_HPP
#define PMU_Interface_JCP2016_HPP

//#include "cdef.h"
#include "pmu_ci.h"
#include "pmu_ct.h"

#include "clock_c1.h"  // C Export wrapper

/* TAUB Register */
#define REE__nDontHSkipTIMER
#define REE__nDontSkipTAUB0TT
#define REE__nDontSkipTAUB0TS
#define REE__nDontSkipTAUB0RDE
#define REE__nDontSkipTAUB0RDT
#define REE__nDontSkipTAUB0TOC
#define REE__nDontSkipTAUB0TOE
#define REE__nDontSkipTAUB0TOL
#define REE__nDontSkipTAUB0TOM
#define REE__nDontSkipTAUB0CDR6
#define REE__nDontSkipTAUB0CMOR6
#define REE__nDontSkipTAUB0CMUR6

/* Interrupt Register */
#define REE__nDontSkipINTC1EIC23
#define REE__nDontSkipINTC1EIC23_LH

#ifdef JCP2016_CPU
extern "C" void PMU_vInitJCP2016(void);
extern "C" void PMU_vPreparePromptSurfaceJCP2016(void);
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
#if (PMU_STATISTICS_PIPELINE == PMU_USE_DHD_PIPELINE)
	namespace JCP2016DHD 
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

#endif //end of JCP2016_CPU
#endif //end of PMU_Interface_JCP2016_HPP