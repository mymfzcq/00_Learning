#ifndef PMU_CT_H
#define PMU_CT_H

enum PMU_enCPUType{
	CPUType_V850E2M_LL    =0,
	CPUType_V850E2M_HL_L  =1,
	CPUType_V850E2M_HL    =2,
	CPUType_V850E2M_HL_E  =3,
	CPUType_V850E2M_PL    =4,
	NoOfCPUTypes		  =5

};
typedef enum PMU_enCPUType PMU_tenCPUType;

enum PMU_enGPUDriver{
	CPUType_V850E2M_GAD    =0,
	CPUType_V850E2M_PLCD   =1,
	CPUType_V850E2M_SLCD   =2,
	NoOfGPUTypes		   =3

};
typedef enum PMU_enGPUDriver PMU_tenGPUDriver;

enum PMU_enMeasurementType{
	Type_CPU		     = 0,
	Type_GPU			 = 1,
	NoOfMeasurementTypes = 2

};
typedef enum PMU_enMeasurementType PMU_tenMeasurementType;

enum enTaskMode
{
	enInitialMode	   	  = 0,
	enBenchmarkMode		  = 1,
	enGraphicPrepareMode  = 2,
	enCPUOutputMode       = 3,
	enGPUOutputMode       = 4,
	enLastMode            = 5
};

typedef enum enTaskMode PMU_tenTaskMode;


#ifdef __cplusplus
extern "C" {
#endif	//  __cplusplus

#define RefreshCycleInvalid 0
#define HInstanceInvalidID  255
#define PMU_UnInitialized  255

#define DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name;

	//declare handle of object
	DECLARE_HANDLE(HSTATISTICS);
	typedef void *HANDLE;

	typedef HANDLE *PHANDLE;
#ifdef __cplusplus
}
#endif	//  __cplusplus


#ifdef __cplusplus
namespace PMU
{


}	// End of namespace
#endif	//  __cplusplus


#endif //end of PMU_CT_H