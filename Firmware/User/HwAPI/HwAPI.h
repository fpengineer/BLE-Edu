/*****************************************************************************************************

    HwAPI



*******************************************************************************************************/


#ifndef _HWAPI_H_
#define _HWAPI_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>

#include "HwAPI Def.h"
#include "HwAPI Config.h"

#include "HwAPI_Boot.h"
#include "HwAPI_Log.h"
#include "HwAPI_LEDn.h"
#include "HwAPI_Measure.h"
//#include "HwAPI_FileManager.h"




/* Exported functions --------------------------------------------------------*/
void vTask_HwBoot( void *pvParameters );


#endif /* _HW_API_H_ */
/* End of file */
