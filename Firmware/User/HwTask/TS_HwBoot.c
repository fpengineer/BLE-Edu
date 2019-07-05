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

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "HwAPI.h"

TaskHandle_t xTask_HwBoot;
HwBootData_t hwBootData = { HW_BOOT_IDLE, "" }; 


// Declare private functions

// Declare private variables
#ifdef HWAPI_BOOT_DEBUG_INFO
static char tempString[ 100 ] = {""};
#endif

void vTask_HwBoot( void *pvParameters )
{
    HwBootStatus_t hwBootStatus_Log = HW_BOOT_IDLE;
    HwBootStatus_t hwBootStatus_LED1 = HW_BOOT_IDLE;
    HwBootStatus_t hwBootStatus_LED2 = HW_BOOT_IDLE;
    HwBootStatus_t hwBootStatus_Measure = HW_BOOT_IDLE;

    hwBootData.hwBootStatus = HW_BOOT_IN_PROGRESS;
    
// Run HwLog task
#if 1
    if ( HwAPI_Log_Run() == HW_TASK_BOOT_RUN )
    {
        hwBootStatus_Log = HW_BOOT_SUCCESS;
#ifdef HWAPI_BOOT_DEBUG_INFO
        sprintf( tempString, "HwLog thread created; free mem: %d B", xPortGetFreeHeapSize() );
        HwAPI_Log_Info( tempString );
#endif

// Run HwLED1 task
#if 1   
        if ( HwAPI_LED1_Run() == HW_TASK_BOOT_RUN )
        {
            hwBootStatus_LED1 = HW_BOOT_SUCCESS;
#ifdef HWAPI_BOOT_DEBUG_INFO
            sprintf( tempString, "HwLED1 thread created; free mem: %d B", xPortGetFreeHeapSize() );
            HwAPI_Log_Info( tempString );
#endif
        }
        else
        {
            hwBootStatus_LED1 = HW_BOOT_ERROR;
#ifdef HWAPI_BOOT_DEBUG_INFO
            HwAPI_Log_Info( "HwLED1 thread boot error" );
#endif
        }
#endif

// Run HwLED2 task
#if 1   
        if ( HwAPI_LED2_Run() == HW_TASK_BOOT_RUN )
        {
            hwBootStatus_LED2 = HW_BOOT_SUCCESS;
#ifdef HWAPI_BOOT_DEBUG_INFO
            sprintf( tempString, "HwLED2 thread created; free mem: %d B", xPortGetFreeHeapSize() );
            HwAPI_Log_Info( tempString );
#endif
        }
        else
        {
            hwBootStatus_LED2 = HW_BOOT_ERROR;
#ifdef HWAPI_BOOT_DEBUG_INFO
            HwAPI_Log_Info( "HwLED2 thread boot error" );
#endif
        }
#endif

// Run HwMeasure task
#if 1   
        if ( HwAPI_Measure_Run() == HW_TASK_BOOT_RUN )
        {
            hwBootStatus_Measure = HW_BOOT_SUCCESS;
#ifdef HWAPI_BOOT_DEBUG_INFO
            sprintf( tempString, "HwMeasure thread created; free mem: %d B", xPortGetFreeHeapSize() );
            HwAPI_Log_Info( tempString );
#endif
        }
        else
        {
            hwBootStatus_Measure = HW_BOOT_ERROR;
#ifdef HWAPI_BOOT_DEBUG_INFO
            HwAPI_Log_Info( "HwLog thread boot error" );
#endif
        }
#endif
    }
#endif       
        
// Check boot result
    if ( hwBootStatus_Log == HW_BOOT_SUCCESS && 
         hwBootStatus_LED1 == HW_BOOT_SUCCESS && 
         hwBootStatus_LED2 == HW_BOOT_SUCCESS && 
         hwBootStatus_Measure == HW_BOOT_SUCCESS )
    {
        hwBootData.hwBootStatus = HW_BOOT_SUCCESS;
        HwAPI_LED1_On();
#ifdef HWAPI_BOOT_DEBUG_INFO
        HwAPI_Log_Info( "HwBoot thread complete" );
#endif
    }
    else if ( hwBootStatus_Log == HW_BOOT_ERROR || 
              hwBootStatus_LED1 == HW_BOOT_ERROR ||
              hwBootStatus_LED2 == HW_BOOT_ERROR ||
              hwBootStatus_Measure == HW_BOOT_ERROR )
    {
        hwBootData.hwBootStatus = HW_BOOT_ERROR;
        HwAPI_LED1_Flash( STATUS_LED_FLASH_FAST );
#ifdef HWAPI_BOOT_DEBUG_INFO
        HwAPI_Log_Info( "HwBoot thread result: HW_BOOT_ERROR" );
#endif
    }
    else
    {
        hwBootData.hwBootStatus = HW_BOOT_IDLE;
#ifdef HWAPI_BOOT_DEBUG_INFO
        HwAPI_Log_Info( "HwBoot thread result: HW_BOOT_IDLE" );
#endif
    }

    while ( 1 )
    {

    }
}
/* End of file */
