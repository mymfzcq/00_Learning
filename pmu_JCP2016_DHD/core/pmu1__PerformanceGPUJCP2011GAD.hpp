#ifndef PMU_PerformanceGPUJCP2011GAD_HPP
#define PMU_PerformanceGPUJCP2011GAD_HPP

#include "PMU_ci.h"
#include "pmu1__Performance.hpp"
#ifdef JCP2011_CPU
extern "C"
{
	/* Don't skip the GFX header and listed macros */
#define NEC__nDontHSkipJCP2011_GFX_APB
#if ((GAD_nUseCtrl_JCP2011_PL == Yes) || (GAD_nUseCtrl_JCP2011_HL == Yes))
#define NEC__nDontSkipMVO
#define NEC__nDontSkipGSRSDSCTRL
#endif
#if (GAD_nUseCtrl_JCP2011_HL_L == Yes)
#define NEC__nDontSkipPVO
#endif
#define NEC__nDontSkipDRW
#define NEC__nDontSkipVI
#define NEC__nDontSkipCLKD
#define NEC__nDontSkipGSVCTRL
#define NEC__nDontSkipTCON
#if ((GAD_nUseCtrl_JCP2011_HL == Yes) || (GAD_nUseCtrl_JCP2011_HL_L == Yes))
#define NEC__nDontSkipGSPRSCTRL
#endif

	/* Don't skip the PF3 header and listed macros */
#define NEC__nDontHSkipV850E2M_PF3_SPB
#if ((GAD_nUseCtrl_JCP2011_PL == Yes) || (GAD_nUseCtrl_JCP2011_HL == Yes))
#define NEC__nDontSkipMVOmInt
#endif
#if (GAD_nUseCtrl_JCP2011_HL_L == Yes)
#define NEC__nDontSkipPVOmInt // takes effect only in HL_L
#endif
#define NEC__nDontSkipDRWInt
#define NEC__nDontSkipICVI0SCL
#define NEC__nDontSkipICVI0FFO
#if (GAD_nUseCtrl_JCP2011_PL == Yes)
#define NEC__nDontSkipICVI1SCL
#define NEC__nDontSkipICVI1FFO
#endif

	/* Don't skip the CLK header and listed macros */
#define NEC__nDontHSkipJCP2011_CLK_APB
#define NEC__nDontSkipPROTCMD5
#define NEC__nDontSkipPROTS5

#if (GAD_nUseCtrl_JCP2011_PL == Yes)
#define NEC__nDontSkipGSMVO0BC
#define NEC__nDontSkipGSMVO1BC
#define NEC__nDontSkipGSMVOSMC
	/* Don't skip the CLK header and listed macros */
#define NEC__nDontHSkipJCP2011_CLK_APB
#define NEC__nDontSkipPROTCMD1
#define NEC__nDontSkipPROTCMD2
#define NEC__nDontSkipPROTCMD3
#define NEC__nDontSkipPROTCMD4
#define NEC__nDontSkipPROTS1
#define NEC__nDontSkipPROTS2
#define NEC__nDontSkipPROTS3
#define NEC__nDontSkipPROTS4
#define NEC__nDontSkipCSCSTAT_118
#define NEC__nDontSkipCSCSTAT_P05
#define NEC__nDontSkipCSCSTAT_P06
#define NEC__nDontSkipCSCSTAT_P07
#define NEC__nDontSkipCSCSTAT_P08
#define NEC__nDontSkipCSCSTAT_P09
#define NEC__nDontSkipCSCSTAT_P11
#define NEC__nDontSkipCKSC_A06
#define NEC__nDontSkipCKSC_P05
#define NEC__nDontSkipCKSC_P06
#define NEC__nDontSkipCKSC_P07
#define NEC__nDontSkipCKSC_P08
#define NEC__nDontSkipCKSC_P09
#define NEC__nDontSkipCKSC_P11
#define NEC__nDontSkipCMASKC
#define NEC__nDontSkipCMASKS
#define NEC__nDontSkipPLLC3
#define NEC__nDontSkipPLLE3
#define NEC__nDontSkipPLLS3
#define NEC__nDontSkipPLLST3
#define NEC__nDontSkipPLLC4
#define NEC__nDontSkipPLLE4
#define NEC__nDontSkipPLLS4
#define NEC__nDontSkipPLLST4
#define NEC__nDontSkipFOUTDIV
#define NEC__nDontSkipFOUTSTAT
#define NEC__nDontSkipSWBRESS
#define NEC__nDontSkipSWBRESC
#endif

#if (GAD_nUseCtrl_JCP2011_HL == Yes)
#define NEC__nDontSkipSVO0LCDTIMING0
#define NEC__nDontSkipSVO0LCDTIMING1
#define NEC__nDontSkipSVO0LCDTIMING2
#define NEC__nDontSkipSVO0LCDUPBASE
#define NEC__nDontSkipSVO0LCDCONTROL
#define NEC__nDontSkipSVO0LCDIMSC
#define NEC__nDontSkipSVO0LCDRIS
#define NEC__nDontSkipSVO0LCDMIS
#define NEC__nDontSkipSVO0LCDICR
#define NEC__nDontSkipSVO0LCDUPCURR
#define NEC__nDontSkipSVO0LCDPALETTE
#define NEC__nDontSkipICSVO0FUF
#define NEC__nDontSkipICSVO0NBA
#define NEC__nDontSkipICSVO0VCP
#endif
#include "gauxl_c1.h"
#include "gad_c1.h"
#include "pman_c1.h"
#include "rsst_c1.h"
#include "stdio.h"  // C Export wrapper
#include "grlc_c1.h"  // C Export wrapper
#include "RSST_c1.h"  // C Export wrapper
#include "HWHeader.h"  // C Export wrapper
	extern uint16 PMU_u16ContextID;
	extern uint16 PMU_u16SurfaceID;
	extern 	GBT_tstMText PMU_Text_GPU,PMU_Text_FPS;
	extern	uint16 *pPMU_Text_GPU,*pPMU_Text_FPS;
}

namespace PMU
{
#define DRW_TRIGGER1_DISABLETRIGGER				0x00000001|0x00000000
#define DRW_TRIGGER1_ACTIVE2DCYCLES				0x00000002
#define DRW_TRIGGER1_FBREADACCESS				0x00000004
#define DRW_TRIGGER1_FBWRITEACCESS				0x00000008
#define DRW_TRIGGER1_TXREADACCESS				0x00000010
#define DRW_TRIGGER1_INVISIABLEPX1				0x00000020
#define DRW_TRIGGER1_INVISIABLEPX2				0x00000040
#define DRW_TRIGGER1_DLACTIVECYCLES				0x00000080
#define DRW_TRIGGER1_FBREADHITS					0x00000100
#define DRW_TRIGGER1_FBREADMISSES				0x00000200
#define DRW_TRIGGER1_FBWRITEHITS				0x00000400
#define DRW_TRIGGER1_FBWRITEMISSES				0x00000800
#define DRW_TRIGGER1_TXREADHITS					0x00001000
#define DRW_TRIGGER1_TXREADMISSES				0x00002000
#define DRW_TRIGGER1_ACTIVECYCLES				0x00008000
#define DRW_TRIGGER2_DISABLETRIGGER				0x00010000|0x00000000
#define DRW_TRIGGER2_ACTIVE2DCYCLES				0x00020000
#define DRW_TRIGGER2_FBREADACCESS				0x00040000
#define DRW_TRIGGER2_FBWRITEACCESS				0x00080000
#define DRW_TRIGGER2_TXREADACCESS				0x00100000
#define DRW_TRIGGER2_INVISIABLEPX1				0x00200000
#define DRW_TRIGGER2_INVISIABLEPX2				0x00400000
#define DRW_TRIGGER2_DLACTIVECYCLES				0x00800000
#define DRW_TRIGGER2_FBREADHITS					0x01000000
#define DRW_TRIGGER2_FBREADMISSES				0x02000000
#define DRW_TRIGGER2_FBWRITEHITS				0x04000000
#define DRW_TRIGGER2_FBWRITEMISSES				0x08000000
#define DRW_TRIGGER2_TXREADHITS					0x10000000
#define DRW_TRIGGER2_TXREADMISSES				0x20000000
#define DRW_TRIGGER2_ACTIVECYCLES				0x80000000

	
	class PerformanceGPUJCP2011GAD:public Performance
	{
	public:
		friend class Statistics;


		PerformanceGPUJCP2011GAD(uint8 &u8ErrorCnt);
		~PerformanceGPUJCP2011GAD(void);
		static void vInit(void);
		static void vDeInit(void);
		//static void vCPUIdleHook(void);
		static inline void PerformanceGPUJCP2011GAD::vCPUIdleHook(void)
		{
			// this will increase the time of ++ operation so the factor is 1.305
			__DI();
			u32BenchMarkCounters++;
			__EI();
		}

		static uint32 vGetCPUBenchMarkCounter(void);

		virtual void vOutPut(void);

		void vOutPutGPUPerf(void);

		static volatile uint32 u32BenchMarkCounters;

		static PerformanceGPUJCP2011GAD *vReturnGPUObject(void);

		float PMU_fGPU_MAX;

		uint32 PMU_u32FPS_MAX;
	protected:
		uint16 PMU_u16CurrentCursor;
		uint16 PMU_au16BarGraph[PMU_BMP_SIZE_X/2];
		uint16 *PMU_pu16BarGraphBuffer;
	private:
		static PerformanceGPUJCP2011GAD *poPerformanceGPUJCP2011GAD;
		uint32 DRW_PFCOUNTER1;
		uint32 DRW_PFCOUNTER2;
		bool   DRW_boOverLoad;

	};
	/** *  CLASS END */

}	// End of namespace

#endif 
#endif //end of PMU_PerformanceGPUJCP2011GAD_HPP