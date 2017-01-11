#include "pmu1__PerformanceGPUJCP2016DHD.hpp"

#ifdef JCP2016_CPU
namespace PMU
{
	float fTempGPU;
	float fTempBDWrite;
    float fTempBDRead;
    uint32 u32TempFPS;
	volatile uint32 PerformanceGPUJCP2016DHD::u32BenchMarkCounters = 0;
	PerformanceGPUJCP2016DHD *PerformanceGPUJCP2016DHD::poPerformanceGPUJCP2016DHD = NULL;

	void PerformanceGPUJCP2016DHD::vInit(void)
	{
		uint8 u8ErrorCnt = 0;
		poPerformanceGPUJCP2016DHD = new PerformanceGPUJCP2016DHD(u8ErrorCnt);
		if (poPerformanceGPUJCP2016DHD == NULL || u8ErrorCnt> 0)
		{
			vGoFailure();
		}
	}

	void PerformanceGPUJCP2016DHD::vDeInit(void)
	{
		//
	}

    PerformanceGPUJCP2016DHD::PerformanceGPUJCP2016DHD(uint8 &u8ErrorCnt) :dhd_device(GRLC__astDHDContextInfo[0].pvDHDHandle), PMU_stPFC(), DRW_boOverLoad(false), \
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

	PerformanceGPUJCP2016DHD::~PerformanceGPUJCP2016DHD(void)
	{
        delete m_poStatistics;
	}

	PerformanceGPUJCP2016DHD *PerformanceGPUJCP2016DHD::vReturnGPUObject(void)
	{
		return poPerformanceGPUJCP2016DHD;
	}

	uint32 PerformanceGPUJCP2016DHD::vGetCPUBenchMarkCounter(void)
	{
		return u32BenchMarkCounters;
	}

	void PerformanceGPUJCP2016DHD::vOutPut(void)
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

    void PerformanceGPUJCP2016DHD::vPerformanceGetResult(void)
    {
        uint32 err = E_DHD_OK;
        /* Prepare counter for the next measurement */

        err |= dhd_read_pc(dhd_device, &PMU_stPFC[0], E_DHD_WAIT_WRITEBACK);


        err |= dhd_read_pc(dhd_device, &PMU_stPFC[1], E_DHD_WAIT_WRITEBACK);


        err |= dhd_read_pc(dhd_device, &PMU_stPFC[2], E_DHD_WAIT_WRITEBACK);


        err |= dhd_read_pc(dhd_device, &PMU_stPFC[3], E_DHD_WAIT_WRITEBACK);


        /* Calling <dhd_get_pc_value> stalls the CPU until <dhd_read_pc> is finished on GPU side */
        /* err |= dhd_finish(dhd_handle); */


        dhd_reset_pc(dhd_device, &PMU_stPFC[0]);
        dhd_get_pc_value(dhd_device, &PMU_stPFC[0]);

        dhd_reset_pc(dhd_device, &PMU_stPFC[1]);
        dhd_get_pc_value(dhd_device, &PMU_stPFC[1]);

        dhd_reset_pc(dhd_device, &PMU_stPFC[2]);
        dhd_get_pc_value(dhd_device, &PMU_stPFC[2]);

        dhd_reset_pc(dhd_device, &PMU_stPFC[3]);
        dhd_get_pc_value(dhd_device, &PMU_stPFC[3]);

        if (err != E_DHD_OK) 
        {
            vGoFailure();
        }

    }
    
    void PerformanceGPUJCP2016DHD::vPFCResetAndStart(void)
    {
        uint32 err = E_DHD_OK;

        /* Prepare counter for the next measurement */
        PMU_stPFC[0].m_value = 0;
        PMU_stPFC[1].m_value = 0;
        PMU_stPFC[2].m_value = 0;
        PMU_stPFC[3].m_value = 0;

        err |= dhd_reset_pc(dhd_device, &PMU_stPFC[0]);
        err |= dhd_reset_pc(dhd_device, &PMU_stPFC[1]);
        err |= dhd_reset_pc(dhd_device, &PMU_stPFC[2]);
        err |= dhd_reset_pc(dhd_device, &PMU_stPFC[3]);

        err |= dhd_set_pc_enable(dhd_device, &PMU_stPFC[0], 1);
        err |= dhd_set_pc_enable(dhd_device, &PMU_stPFC[1], 1);
        err |= dhd_set_pc_enable(dhd_device, &PMU_stPFC[2], 1);
        err |= dhd_set_pc_enable(dhd_device, &PMU_stPFC[3], 1);

        if (err != E_DHD_OK) 
        {
            vGoFailure();
        }
    }
    void PerformanceGPUJCP2016DHD::vOutPutGPUPerf(void)
    {
        dhd_uint32_t gpuPixWrite, gpuClks, gpuPixRead;
        //step1: gpu load
        __DI();
        vPerformanceGetResult();
        __EI();
        gpuClks = PMU_stPFC[2].m_value;
        gpuPixWrite = PMU_stPFC[0].m_value + PMU_stPFC[1].m_value;
        gpuPixRead = PMU_stPFC[3].m_value;
        uint32 u32ErrorCode;
        char CharGPU[30] = "GPU: ";
        char ResultCharGPU[30] = "";
        uint16 ResultChar[30] = { 0 };
        strcat(ResultCharGPU, CharGPU);
        __DI();
        if (gpuClks == 0)
        {
            fTempGPU = 99.99f;
            DRW_boOverLoad = true;
        }
        else
        {
            fTempGPU = (float)gpuClks / (float)(PMAN__FreqPLL_PMAN_nXCCLK);
            fTempGPU *= 100;
            if (fTempGPU > 1.001f)
            {
                fTempGPU -= 1.0f;// offset of the gpu load
            }
            DRW_boOverLoad = false;
        }
        __EI();
        sprintf(CharGPU, "%0.2f", fTempGPU);
        strcat(ResultCharGPU, CharGPU);
        strcat(ResultCharGPU, "%");
        printf("%s------", ResultCharGPU);

        //step2: bandwidth of pixel writing
        memset(ResultChar, 0, sizeof ResultChar);
        char CharBDWrite[30] = "BDWrite(mpixel/s): ";
        char ResultCharBDWrite[30] = "";
        strcat(ResultCharBDWrite, CharBDWrite);
        __DI();
        fTempBDWrite = (gpuPixWrite * 240.0) / (float)gpuClks;
        __EI();
        sprintf(CharBDWrite, "%f", fTempBDWrite);
        strcat(ResultCharBDWrite, CharBDWrite);
        if (DRW_boOverLoad)
        {
            strcat(ResultCharBDWrite, "   ERROR!");
        }
        printf("%s------", ResultCharBDWrite);

        //step3: bandwidth of texture reading
        memset(ResultChar, 0, sizeof ResultChar);
        char CharBDRead[30] = "BDRead(mpixel/s): ";
        char ResultCharBDRead[30] = "";
        strcat(ResultCharBDRead, CharBDRead);
        __DI();
        fTempBDRead = (gpuPixRead * 240.0) / (float)gpuClks;
        __EI();

        sprintf(CharBDRead, "%f", fTempBDRead);
        strcat(ResultCharBDRead, CharBDRead);
        printf("%s------", ResultCharBDRead);

        //step4: FPS
        memset(ResultChar, 0, sizeof ResultChar);
        char CharFPS[30] = "FPS: ";
        char ResultCharFPS[30] = "";
        strcat(ResultCharFPS, CharFPS);
        __DI();
        u32TempFPS = (uint32)(fTempBDWrite*1000000.0f / ((uint32)RSST__stGSConfig.pastDispAttr[RSST_nGE_FE_7Inch_Display].u16Width*(uint32)RSST__stGSConfig.pastDispAttr[RSST_nGE_FE_7Inch_Display].u16Height));
        sprintf(CharFPS, "%d", u32TempFPS);
        strcat(ResultCharFPS, CharFPS);
        printf("%s\n", ResultCharFPS);
        printf("------\n------\n");


        asm("nop");
        __DI();
        vPFCResetAndStart();
        __EI();
    }


	// end of functions
}
#endif