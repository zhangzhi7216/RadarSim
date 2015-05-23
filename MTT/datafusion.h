#ifdef DATAFUSION_API
#else
#define DATAFUSION_API extern "C" _declspec(dllimport)
#endif

#include "globalParameter.h"
#include "structDefinition.h"


DATAFUSION_API void data_fusion(ECHO *Echo_TongLei,ECHO *Echo_RadarAIS,int *Echo_EM,int *Echo_DL,int *iSpt, TRACKINFO *pTrk,ECHO_PARAMETER *Echo_Param,int *TrkNum,int *FusionMethod);
