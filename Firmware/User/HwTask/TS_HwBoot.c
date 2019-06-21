/*****************************************************************************************************

    Project: 
    
    Date: 
    Author: Filipp Popov

    Overview: Task - Hw to boot for all enabled hardware modules

*******************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "HwAPI.h"

#define	ERROR_ACTION( CODE,POS )		do{}while( 0 )

TaskHandle_t xTask_HwBoot;

HwBootData_t hwBootData = { HW_BOOT_IDLE, "" }; 


// Declare private functions

// Declare private variables
#ifdef HWAPI_BOOT_DEBUG_INFO
static char tempString[ 100 ] = {""};
#endif

void vTask_HwBoot( void *pvParameters )
{
    HwBootStatus_t hwBootStatus_LED1 = HW_BOOT_IDLE;
    HwBootStatus_t hwBootStatus_LED2 = HW_BOOT_IDLE;

    hwBootData.hwBootStatus = HW_BOOT_IN_PROGRESS;
    
#if 1   // HwLED1
//        vTaskDelay(500);
        if ( HwAPI_LED1_Run() == HW_TASK_BOOT_RUN )
        {
            hwBootStatus_LED1 = HW_BOOT_SUCCESS;
#ifdef HWAPI_BOOT_DEBUG_INFO
            //snprintf( tempString, sizeof( tempString ), "HwLED1 task run - free mem: %dB\n", xPortGetFreeHeapSize() );
            //HwAPI_Terminal_SendMessage( tempString );
#endif
        }
        else
        {
            hwBootStatus_LED1 = HW_BOOT_ERROR;
#ifdef HWAPI_BOOT_DEBUG_INFO
            //HwAPI_Terminal_SendMessage( "HwLED1 task boot error\n" );
#endif
        }
#endif
#if 1   // HwLED2
//        vTaskDelay(500);
        if ( HwAPI_LED2_Run() == HW_TASK_BOOT_RUN )
        {
            hwBootStatus_LED2 = HW_BOOT_SUCCESS;
#ifdef HWAPI_BOOT_DEBUG_INFO
            //snprintf( tempString, sizeof( tempString ), "HwLED2 task run - free mem: %dB\n", xPortGetFreeHeapSize() );
            //HwAPI_Terminal_SendMessage( tempString );
#endif
        }
        else
        {
            hwBootStatus_LED2 = HW_BOOT_ERROR;
#ifdef HWAPI_BOOT_DEBUG_INFO
            //HwAPI_Terminal_SendMessage( "HwLED2 task boot error\n" );
#endif
        }
#endif
    

    if ( hwBootStatus_LED1 == HW_BOOT_SUCCESS && hwBootStatus_LED2 == HW_BOOT_SUCCESS )
    {
        hwBootData.hwBootStatus = HW_BOOT_SUCCESS;
        HwAPI_LED1_On();
#ifdef HWAPI_BOOT_DEBUG_INFO
        //HwAPI_Terminal_SendMessage( "HwBoot task complete\n" );
#endif
    }
    else if ( hwBootStatus_LED1 == HW_BOOT_ERROR || hwBootStatus_LED2 == HW_BOOT_ERROR )
    {
        hwBootData.hwBootStatus = HW_BOOT_ERROR;
        HwAPI_LED1_Flash( STATUS_LED_FLASH_FAST );
#ifdef HWAPI_BOOT_DEBUG_INFO
        //HwAPI_Terminal_SendMessage( "HwBoot task result: HW_BOOT_ERROR\n" );
#endif
    }
    else
    {
        hwBootData.hwBootStatus = HW_BOOT_IDLE;
#ifdef HWAPI_BOOT_DEBUG_INFO
        //HwAPI_Terminal_SendMessage( "HwBoot task result: HW_BOOT_IDLE \n" );
#endif
    }

    while ( 1 )
    {

    }
}
/* End of file */
