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

#define PMU_USE_EMPTY_PIPELINE     Off
#define PMU_USE_PLCD_PIPELINE      Off
#define PMU_USE_JADE_PIPELINE      Off
#define PMU_USE_CAPM_PIPELINE      Off
#define PMU_USE_JCP2011_PIPELINE   On
#define PMU_USE_GDAD_PIPELINE      Off
#define PMU_USE_OVG_PIPELINE       Off
#define PMU_USE_DFB_PIPELINE       Off
#define PMU_USE_SWR_PIPELINE       Off
#define PMU_USE_OGLES20_PIPELINE   Off
#define PMU_USE_JCP2016_PIPELINE   Off


#define PMU_STATISTICS_PIPELINE  PMU_USE_JCP2011_PIPELINE

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

#define	PMU_DefaultSurface		RSST_nJCP2011MVO_L3
#define	PMU_ObjCtrlLoc			RSST_nMemJCP2011_iRAM
#define PMU_ObjDataLoc			RSST_nMemJCP2011_DRAM
#define PMU_SurfaceLoc			RSST_nMemJCP2011_DRAM
#define PMU_TextLoc				RSST_nMemJCP2011_DRAM

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
#ifdef MeasureCPU
#define NoOfCPUCores 1
#endif 

#ifdef MeasureCPU
#define NoOfGPUSPs 1
#endif 

const uint8 NoOfMeasurementUnits = (1*NoOfCPUCores)+(1*NoOfGPUSPs);
#ifdef __cplusplus
}	// End of namespace
#endif


#endif /* PMU1CI_H */

