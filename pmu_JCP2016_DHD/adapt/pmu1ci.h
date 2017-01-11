#ifndef PMU_CI_H
#define PMU_CI_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "cdef.h"
#include "osabs_c1.h"
#include "GOSW_c1.h"
#include "RSST_c1.h"  // C Export wrapper

#define __PMU__ (On) 
	
#define PMU_INLINE 

#define PMU_CFG_CONST  const

    /**
    * Active Rendering Pipelines:
    */
#define PMU_USE_JCP2011_PIPELINE   Off
#define PMU_USE_GDAD_PIPELINE      Off
#define PMU_USE_OVG_PIPELINE       Off
#define PMU_USE_PLCD_PIPELINE      Off
#define PMU_USE_SWR_PIPELINE       Off
#define PMU_USE_EMPTY_PIPELINE     Off
#define PMU_USE_OGLES20_PIPELINE   Off
#define PMU_USE_DHD_PIPELINE       On
#define PMU_USE_SWR16_PIPELINE     Off
#define PMU_USE_VSP_PIPELINE       Off
#ifdef JCP2016_CPU
    // Sub-pipelines  for JCP2016
#define PMU_USE_VDCE_SUBPIPELINE   On
#define PMU_USE_VSPDU_SUBPIPELINE  Off
#define PMU_USE_DHD_OVER_VDCE      On
#define PMU_USE_DHD_OVER_VSPDU     Off
#define PMU_USE_PLCD_OVER_VDCE     Off
#define PMU_USE_SWR16_OVER_VDCE    Off
#elif defined JCP2011_CPU
    // Sub-pipelines  for JCP2011
#define PMU_nUseCtrl_CapricornM          (No)
#define PMU_nUseCtrl_CapricornA          (No)
#define PMU_nUseCtrl_FjJade              (No)
#define PMU_nUseCtrl_JCP2011_HL          (Yes)
#define PMU_nUseCtrl_JCP2011_HL_L        (No)  /* HL-L (DK4-H) */
#define PMU_nUseCtrl_JCP2011_PL          (No)
#endif
#define PMU_STATISTICS_PIPELINE  PMU_USE_DHD_PIPELINE

#define MeasureCPU	(YES)
#define MeasureGPU	(YES)
#define SET_ZORDER_MANUALLY	(Off)
#define PMU_SET_BG_CA	(On)

//it should be xx(ms) 
#define PMU_BenchMarkTime 1
#define PMU_NoOfSurfaces  2//no video in layer

#define PMU_BMP_SIZE			4*PMU_BMP_SIZE_X*PMU_BMP_SIZE_Y      
#define PMU_BMP_SIZE_X			200
#define PMU_BMP_SIZE_Y			155//the rectanlge bargraph(100*50) is below the prompt window contains CPU in the left and GPU in the right
#define PMU_OFFSET_X			0
#define PMU_OFFSET_Y			40

#if(On == PMU_SET_BG_CA)
//transparent
#define PMU_SET_BG_CA_VALUE			0x80
//half transparent,note:the value can not be 0 due to GS reasons for blending 2D and TEXT into single layer
#define PMU_SET_BGTXT_CA_VALUE			0x80 
//define the bargraph alpha:opaque
#define PMU_SET_BGBAR_CA_VALUE			0xFF 
#endif
//surface config
#define PMU_APP_COLOR_FORMAT		RSST_nPFrmt_RGBA8888 //same with HMI setting

#ifdef JCP2016_CPU
#define	PMU_ObjCtrlLoc			RSST_nMemJCP2016_iRAM
#define PMU_ObjDataLoc			RSST_nMemJCP2016_DRAM
#define PMU_SurfaceLoc			RSST_nMemJCP2016_DRAM
#define PMU_TextLoc				RSST_nMemJCP2016_DRAM

#else
#define	PMU_DefaultSurface		RSST_nJCP2011MVO_L3

#define	PMU_ObjCtrlLoc			RSST_nMemJCP2011_iRAM
#define PMU_ObjDataLoc			RSST_nMemJCP2011_DRAM
#define PMU_SurfaceLoc			RSST_nMemJCP2011_DRAM
#define PMU_TextLoc				RSST_nMemJCP2011_DRAM
#endif

#define DECLARESURFACE //extern uint16 PMU_SurfacesMarco

#define PMU_DefaultContext 3

#define PMU_SurfaceFont			RSST_nFNT_EurostileNextLTComExtended11//RSST_nFNT_Eurostile11
//note this is sureface id created by system and PMU module if use ARTEMMIS HMI,it is buffer id
#if (SET_ZORDER_MANUALLY == On)
#define PMU_SurfacesMarco PMU_u16SurfaceID,0,0
#define	PMU_SurfaceAssembly		{PMU_SurfacesMarco}
#endif

#ifdef __cplusplus
}
#endif
#ifdef __cplusplus
namespace PMU
{
#endif
#define fCPUFactor 1.305f
#define fCPUBenchMarkValue_ML_E 22000000.0f
#ifdef MeasureCPU
#define NoOfCPUCores 1
#endif 

#ifdef MeasureCPU
#define NoOfGPUSPs 1
#endif 

#define NoOfMeasurementUnits  ((1*NoOfCPUCores)+(1*NoOfGPUSPs))
#ifdef __cplusplus
}	// End of namespace
#endif


#endif /* PMU1CI_H */

