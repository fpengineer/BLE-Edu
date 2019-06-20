/*****************************************************************************************************

    Project: 
    
    Date: 
    Author: Filipp Popov

    Overview: HwAPI boot functions

*******************************************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "HwAPI.h"



//
HwBootData_t HwAPI_Boot_GetStatus( void )
{
    extern HwBootData_t hwBootData;

    return hwBootData;
}
/* End of file */
