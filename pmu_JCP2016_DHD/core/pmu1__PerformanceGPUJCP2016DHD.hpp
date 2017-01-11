#ifndef PMU_PerformanceGPUJCP2016DHD_HPP
#define PMU_PerformanceGPUJCP2016DHD_HPP

#include "PMU_ci.h"
#include "pmu1__Performance.hpp"
#ifdef JCP2016_CPU
extern "C"
{
    //    E_DHD_PC_STR_FETCH_START = 0,        /* Stream reader start of fetch event*/
    //    E_DHD_PC_STR_BURST_READ,             /* Stream reader did a burst read on MBI*/
    //    E_DHD_PC_STR_WORD_READ,              /* Stream reader received a word from MBI (1 event for every word in a burst)*/
    //    E_DHD_PC_STW_BURST_WRITE,            /* Stream writer did a burst write on MBI*/
    //    E_DHD_PC_STW_WORD_WRITE,             /* Stream writer did a word write on MBI (1 event for every word in a burst)*/
    //    E_DHD_PC_CLR_BURST_WRITE,            /* Clear unit did a burst write on MBI*/
    //    E_DHD_PC_CLR_WORD_WRITE,             /* Clear unit did a word write on MBI (1 event for every word in a burst)*/
    //    E_DHD_PC_PSU_PRIMITIVE,              /* New primitive enumerated*/
    //    E_DHD_PC_PSU_STRIPE,                 /* New stripe enumerated*/
    //    E_DHD_PC_PSU_VISIBLE_PIXEL,          /* Visible pixel enumerated*/
    //    E_DHD_PC_PSU_INVISIBLE_PIXEL,        /* Invisible pixel enumerated*/
    //    E_DHD_PC_ZSS_SPAN_LENGTH_LIMIT,      /* ZSA buffer span split due to span length limit reached*/
    //    E_DHD_PC_ZSC_BURST_READ,             /* ZSA buffer cache did a burst read on MBI*/
    //    E_DHD_PC_ZSC_BURST_WRITE,            /* ZSA buffer cache did a burst write on MBI*/
    //    E_DHD_PC_ZSC_WORD_READ,              /* ZSA buffer cache received a word from MBI (1 event for every word in a burst)*/
    //    E_DHD_PC_ZSC_WORD_WRITE,             /* ZSA buffer cache did a word write on MBI (1 event for every word in a burst)*/
    //    E_DHD_PC_ZSC_PIXEL_READ,             /* ZSA buffer cache processed a pixel read*/
    //    E_DHD_PC_ZSC_PIXEL_READ_HIT,         /* ZSA buffer cache processed a read without any wait cycle*/
    //    E_DHD_PC_ZSC_PIXEL_READ_WAIT,        /* ZSA buffer cache wait cycles on read side (>= 1 event per miss)*/
    //    E_DHD_PC_ZSC_PIXEL_WRITE,            /* ZSA buffer cache processed a pixel write*/
    //    E_DHD_PC_ZSC_SPAN_CONFLICT,          /* ZSA buffer span conflict occured between new job requiring a read and a line already in the ZSA cache: Read range of new job overlaps write range of open line.*/
    //    E_DHD_PC_ZSC_SPAN_CONFLICT_WAIT,     /* Wait cycles at ZSA buffer prefetch queue interface due to span conflict.*/
    //    E_DHD_PC_ZSC_CACHE_FULL_WAIT,        /* Wait cycles at ZSA buffer prefetch queue interface because line could not be allocated as there is no unused line available.*/
    //    E_DHD_PC_ZSU_DEPTH_TEST_FAILED,      /* Pixel failed depth test*/
    //    E_DHD_PC_ZSU_STENCIL_TEST_FAILED,    /* Pixel failed stencil test*/
    //    E_DHD_PC_FBD_ALPHA_TEST_FAILED,      /* Pixel dropped through alpha test*/
    //    E_DHD_PC_FBS_SPAN_LENGTH_LIMIT,      /* Span split due to span length limit reached*/
    //    E_DHD_PC_FBS_READ_GAP_LIMIT,         /* Span split due to read gap limit reached*/
    //    E_DHD_PC_FBC_BURST_READ,             /* Framebuffer cache did a burst read on MBI*/
    //    E_DHD_PC_FBC_BURST_WRITE,            /* Framebuffer cache did a burst write on MBI*/
    //    E_DHD_PC_FBC_WORD_READ,              /* Framebuffer cache received a word from MBI (1 event for every word in a burst)*/
    //    E_DHD_PC_FBC_WORD_WRITE,             /* Framebuffer cache did a word write on MBI (1 event for every word in a burst)*/
    //    E_DHD_PC_FBC_PIXEL_READ,             /* Framebuffer cache processed a pixel read*/
    //    E_DHD_PC_FBC_PIXEL_READ_HIT,         /* Framebuffer cache processed a read without any wait cycle*/
    //    E_DHD_PC_FBC_PIXEL_READ_WAIT,        /* Framebuffer cache wait cycles on read side (>= 1 event per miss)*/
    //    E_DHD_PC_FBC_PIXEL_WRITE,            /* Framebuffer cache processed a pixel write*/
    //    E_DHD_PC_FBC_SPAN_CONFLICT,          /* Span conflict occured between new job requiring a read and a line already in the framebuffer cache: Read range of new job overlaps write range of open line.*/
    //    E_DHD_PC_FBC_SPAN_CONFLICT_WAIT,     /* Wait cycles at prefetch queue interface due to span conflict.*/
    //    E_DHD_PC_FBC_CACHE_FULL_WAIT,        /* Wait cycles at prefetch queue interface because line could not be allocated as there is no unused line available.*/
    //    E_DHD_PC_TXS_PREFETCH_MISS,          /* Cache miss in the in the prefetching texture cache scheduler*/
    //    E_DHD_PC_TXS_REFRESH,                /* Line refresh job sent from texel schedule to texture cache*/
    //    E_DHD_PC_TXC_BURST_READ,             /* Texture cache did a burst read on MBI*/
    //    E_DHD_PC_TXC_WORD_READ,              /* Texture cache received a word from MBI (1 event for every word in a burst)*/
    //    E_DHD_PC_TXC_PIXEL_READ,             /* Texture cache processed a pixel read (i.e. potentially multiple texel reads, still only a single event)*/
    //    E_DHD_PC_TXC_PIXEL_READ_HIT,         /* Texture cache processed a pixel read without the need to wait for a fetch.*/
    //    E_DHD_PC_TXC_PIXEL_READ_FETCH_WAIT,  /* Wait cycles at pipeline read interface because of wait for line to be fetched.*/
    //    E_DHD_PC_TXC_PIXEL_READ_RAM_WAIT,    /* Wait cycles inserted in pipeline read path because read RAM needed to be accessed more than once and prefetch was not successful.*/
    //    E_DHD_PC_TXC_JOB_LINE_READY,         /* A job was taken from the prefetch queue without the need to wait for the access count to reach the replace count.*/
    //    E_DHD_PC_TXC_JOB_LINE_WAIT,          /* Wait cycles at prefetch queue interface because job could not be fetched as the access count had not yet reached the replace count.*/
    //    E_DHD_PC_RLD_BURST_READ,             /* RLD did a burst read on MBI*/
    //    E_DHD_PC_RLD_WORD_READ,              /* RLD did a word read on MBI (1 event for every word in a burst)*/
    //    E_DHD_PC_RLD_REWIND,                 /* RLD rewinds to start of texture code*/
    //    E_DHD_PC_TXP_CLUT_ACCESS_WAIT,       /* Wait cycles when accessing CLUT: Count every additional cycle of CLUT access which exceeds the optimal throughput of 1 cycle/pixel.*/
    //    E_DHD_PC_STC_COMMAND,                /* Finished execution of a stream command*/
    //    E_DHD_PC_STC_STR_WAIT,               /* Wait cycles when STC waits for data from STR*/
    //    E_DHD_PC_STC_REG_WAIT,               /* Wait cycles when STC waits for register write acknowledge from register file*/
    //    E_DHD_PC_STC_COMMAND_WAIT,           /* Wait cycles when STC waits because of wait command*/
    //    E_DHD_PC_STC_ACTIVE,                 /* Active cycles of the STC while not in pause-reached mode*/
    //    E_DHD_PC_STC_CORE_ACTIVE,            /* Active cycles of the STC-Core only when the ACG would be enabled*/
    //    E_DHD_PC_STC_BUSY,                   /* Busy cycles of STC*/
    //    E_DHD_PC_STR_BUSY,                   /* Busy cycles of STR*/
    //    E_DHD_PC_STW_BUSY,                   /* Busy cycles of STW*/
    //    E_DHD_PC_REG_BUSY,                   /* Busy cycles of REG*/
    //    E_DHD_PC_PSU_BUSY,                   /* Busy cycles of PSU*/
    //    E_DHD_PC_TXI_BUSY,                   /* Busy cycles of modules after PSU, up to ZSS*/
    //    E_DHD_PC_TEX_BUSY,                   /* Busy cycles of modules after ZSS, up to TXS*/
    //    E_DHD_PC_COL_BUSY,                   /* Busy cycles of modules after TXS, up to FBS*/
    //    E_DHD_PC_BLEND_BUSY,                 /* Busy cycles of modules after FBS, up to FBC*/
    //    E_DHD_PC_CLEAR_BUSY,                 /* Busy cycles of clear unit*/
    //    E_DHD_PC_CLOCK                       /* Every clock cycle               */

#include "gauxl_c1.h"
#include "grgl_c1.h"
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
    typedef struct
    {
        dhd_device_t stDHDDeviceContext;
        dhd_handle_t pvDHDHandle;
#if ( GRLC__nUseDHDNativeHandle == On )
        dhd_device_t stDHDDeviceContextNative;
        dhd_handle_t pvDHDHandleNative;  // used for native access to DHD, bypassing GRLC
#endif
        uint32 u32DHDKernelHorizontal;
        uint32 u32DHDKernelVertical;
        uint32 u32DHDKernel2D;
        uint32 u32DHDCLUT;
    }
    PMU___tstDHDContextInfo;
    extern PMU___tstDHDContextInfo GRLC__astDHDContextInfo[RSST_nLastCtxtDesc];
    extern void __DI(void);
    extern void __EI(void);
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
#define PMU__nDHDNumPerfCounters          (4u)
	
	class PerformanceGPUJCP2016DHD:public Performance
	{
	public:
		friend class Statistics;


		PerformanceGPUJCP2016DHD(uint8 &u8ErrorCnt);
		~PerformanceGPUJCP2016DHD(void);
		static void vInit(void);
		static void vDeInit(void);
		//static void vCPUIdleHook(void);
		static inline void PerformanceGPUJCP2016DHD::vCPUIdleHook(void)
		{
			// this will increase the time of ++ operation so the factor is 1.305
			__DI();
			u32BenchMarkCounters++;
			__EI();
		}
        dhd_handle_t dhd_device;
        dhd_performance_counter_t PMU_stPFC[PMU__nDHDNumPerfCounters];

		static uint32 vGetCPUBenchMarkCounter(void);

		virtual void vOutPut(void);

        void vPerformanceGetResult(void);
        void vPFCResetAndStart();
		void vOutPutGPUPerf(void);

		static volatile uint32 u32BenchMarkCounters;

		static PerformanceGPUJCP2016DHD *vReturnGPUObject(void);

		float PMU_fGPU_MAX;

		uint32 PMU_u32FPS_MAX;
	protected:
		uint16 PMU_u16CurrentCursor;
		uint16 PMU_au16BarGraph[PMU_BMP_SIZE_X/2];
		uint16 *PMU_pu16BarGraphBuffer;
	private:
		static PerformanceGPUJCP2016DHD *poPerformanceGPUJCP2016DHD;
		bool   DRW_boOverLoad;

	};
	/** *  CLASS END */

}	// End of namespace

#endif
#endif //end of PMU_PerformanceGPUJCP2016DHD_HPP