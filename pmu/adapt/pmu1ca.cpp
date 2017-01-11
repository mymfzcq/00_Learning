#include "pmu_ci.h"
#include "pmu_ct.h"
//#include "pmu_InterfaceJCP2011.h"
#include "pmu1_Statistics.hpp"
#include "pmu1__Performance.hpp"
#include "pmu1__PerformanceCPUJCP2011.hpp"
#include "pmu1__PerformanceGPUJCP2011GAD.hpp"
#include "pmu1_InterfaceJCP2011.hpp"

extern "C"
{
#include "gauxl_c1.h"
#include "gad_c1.h"  // C Export wrapper
#include "stdio.h"  // C Export wrapper
#include "pmu1_Interf_ce.h"  // C Export wrapper
#include "grlc_c1.h"  // C Export wrapper
#include "RSST_c1.h"  // C Export wrapper

#if (PMU_STATISTICS_PIPELINE == PMU_USE_JCP2011_PIPELINE)
	extern GRLC__tstContext* GRLC__pstGetContext( uint16 u16ContextID );
	extern uint16 PMU_u16ContextID,PMU_u16SurfaceID;
#endif
}


#if (PMU_STATISTICS_PIPELINE == PMU_USE_JCP2011_PIPELINE)
// this is all adapted for special project settings
void PMU::JCP2011GAD::PMU_vRecalcFPS(uint32& fps,uint16 contextID = 0)
{
	GRLC__tstContext* appContext = GRLC__pstGetContext (contextID);

	if( appContext->pstMSrfc->stSrfDesc.stBmp.u8Compression == GBT_nComp_No)
	{
		if(appContext->pstMSrfc->stSrfDesc.u8VideoOut == 0 )
		{   // MVO
			switch( appContext->pstMSrfc->stSrfDesc.stBmp.u8PixFormatIdx )
			{
			case RSST_nPFrmt_ARGB8888:
			case RSST_nPFrmt_ARGB0888:
			case RSST_nPFrmt_RGBA8888:
				fps /= 4;
				break;
			case RSST_nPFrmt_RGB565:
			case RSST_nPFrmt_RGBA4444:
				fps /= 2;
				break;
			default:  /*@CTC RBN*/	
				__asm("nop");
				break;
			}
		}
		else // SVO is enabled
		{ 
			switch( appContext->pstMSrfc->stSrfDesc.stBmp.u8PixFormatIdx )
			{
			case RSST_nPFrmt_ARGB0888:
				fps /= 3;
				break;
			case RSST_nPFrmt_RGB565:
				fps /= 2;
				break;
			default:  /*@CTC RBN*/
				__asm("nop");
				break;
			}
		}
	}
	else
	{
		//RLE layer is not allowed,only texture RLE is allowed
		vGoFailure();
	}
}
//using namespace PMU::JCP2011GAD;
void PMU::JCP2011GAD::PMU_vPreRecalcPrompt(void)
{
#if(PMU_SET_BG_CA == On)
	//GRLC_vSurfaceSetConstAlpha(PMU_u16SurfaceID,PMU_SET_CA_VALUE);
	GRLC_vPrimSetCompCAlphaAttr(PMU_u16ContextID,True);
	GRLC_vPrimSetCompCAlpha(PMU_u16ContextID,PMU_SET_BG_CA_VALUE);
#endif
	//clear surface
	GRLC_vPrimSetBgColor(PMU_u16ContextID, GBT_nGRAY_RGBA8888, PMU_APP_COLOR_FORMAT);
	GRLC_vClear(PMU_u16ContextID);
	//text
	GRLC_vTextSetFontID(PMU_u16ContextID,PMU_SurfaceFont);
	GRLC_vTextSetFontSize(PMU_u16ContextID, 8, GRLC_nFontSize_Point);
	GRLC_vTextSetDrawColor(PMU_u16ContextID,GBT_nGREEN_RGBA8888,PMU_APP_COLOR_FORMAT);
	//old interface
	//GRLC_vTextSetComposition(PMU_u16ContextID, GBT_nCompose_BlendSrcOver, GBT_nROP2_Src, GBT_nMapOpt_Auto, 0xff, False);
	//new interface
    GRLC_vTextSetBlendEquation(PMU_u16ContextID, GBT_nBlendEquation_Add);
    GRLC_vTextSetBlendFactors(PMU_u16ContextID, GBT_nBF_SrcAlpha, GBT_nBF_OneMinusSrcAlpha, GBT_nBF_One, GBT_nBF_OneMinusSrcAlpha);
#if(On == PMU_SET_BG_CA)
	GRLC_vTextSetBgColor(PMU_u16ContextID,GBT_nGRAY_RGBA8888&(0xffffff00|PMU_SET_BGTXT_CA_VALUE),PMU_APP_COLOR_FORMAT);
#else
	GRLC_vTextSetBgColor(PMU_u16ContextID,GBT_nGRAY_RGBA8888,PMU_APP_COLOR_FORMAT);
#endif

#if(PMU_SET_BG_CA == On)
	GRLC_vPrimSetCompCAlphaAttr(PMU_u16ContextID,False);
#endif

}
void PMU::JCP2011GAD::PMU_vPostRecalcPrompt(void)
{
	GRLC_vPrimSetCompCAlphaAttr(PMU_u16ContextID,True);
	GRLC_vPrimSetCompCAlpha(PMU_u16ContextID,0x00);
	//set patten
	GRLC_vPrimSetDrawColor(PMU_u16ContextID, GBT_nBLUE_RGBA8888, PMU_APP_COLOR_FORMAT);
	//GRLC_vPrimSetComposition(PMU_u16ContextID, GBT_nCompose_Overwrite, GBT_nROP2_Src,GBT_nMapOpt_Auto, NULL, False);
	GRLC_vDrawLine(PMU_u16ContextID,PMU_BMP_SIZE_X*3/4,		0,	0/*x distance*/,	18/*y distance*/);
	GRLC_vDrawLine(PMU_u16ContextID,PMU_BMP_SIZE_X*2/3-5,	18,	0/*x distance*/,	62/*y distance*/);
	GRLC_vDrawLine(PMU_u16ContextID,0,						18,	200/*x distance*/,	0/*y distance*/);
	GRLC_vDrawLine(PMU_u16ContextID,0,						80,	200/*x distance*/,	0/*y distance*/);
	GRLC_vPrimSetCompCAlphaAttr(PMU_u16ContextID,False);
	//before prompt maximum value,we shoud display 'MAX'
	PMU_vMaxStringPrompt();

	PMU_vMaxCPULoadPrompt();
	PMU_vMaxGPULoadPrompt();
	PMU_vMaxFPSLoadPrompt();
	PMU_vBandwidthNotSupport();

}
#else

#endif

