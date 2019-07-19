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

#define NRF_LOG_MODULE_NAME TS_HwBoot
#include "nrf_log.h"
NRF_LOG_MODULE_REGISTER();

#include "HwAPI.h"

TaskHandle_t xTask_HwBoot;
HwBootData_t hwBootData = { HW_BOOT_IDLE, "" }; 


// Declare private functions

// Declare private variables
void vTask_HwBoot( void *pvParameters )
{
    HwBootStatus_t hwBootStatus_LED1 = HW_BOOT_IDLE;
    HwBootStatus_t hwBootStatus_LED2 = HW_BOOT_IDLE;
    HwBootStatus_t hwBootStatus_Measure = HW_BOOT_IDLE;

    hwBootData.hwBootStatus = HW_BOOT_IN_PROGRESS;
    
    NRF_LOG_INFO("HwBoot thread started");

// Run HwLED1 task
#if 1   
    if ( HwAPI_LED1_Run() == HW_TASK_BOOT_RUN )
    {
        hwBootStatus_LED1 = HW_BOOT_SUCCESS;
#ifdef HWAPI_BOOT_DEBUG_INFO
        NRF_LOG_INFO("HwLED1 thread created; free mem: %d B", xPortGetFreeHeapSize());
#endif
    }
    else
    {
        hwBootStatus_LED1 = HW_BOOT_ERROR;
#ifdef HWAPI_BOOT_DEBUG_INFO
        NRF_LOG_INFO("HwLED1 thread boot error");
#endif
    }
#endif

// Run HwLED2 task
#if 1   
    if ( HwAPI_LED2_Run() == HW_TASK_BOOT_RUN )
    {
        hwBootStatus_LED2 = HW_BOOT_SUCCESS;
#ifdef HWAPI_BOOT_DEBUG_INFO
        NRF_LOG_INFO("HwLED2 thread created; free mem: %d B", xPortGetFreeHeapSize());
#endif
    }
    else
    {
        hwBootStatus_LED2 = HW_BOOT_ERROR;
#ifdef HWAPI_BOOT_DEBUG_INFO
        NRF_LOG_INFO("HwLED2 thread boot error");
#endif
    }
#endif

// Run HwMeasure task
#if 1   
    if ( HwAPI_Measure_Run() == HW_TASK_BOOT_RUN )
    {
        hwBootStatus_Measure = HW_BOOT_SUCCESS;
#ifdef HWAPI_BOOT_DEBUG_INFO
        NRF_LOG_INFO("HwMeasure thread created; free mem: %d B", xPortGetFreeHeapSize());
#endif
    }
    else
    {
        hwBootStatus_Measure = HW_BOOT_ERROR;
#ifdef HWAPI_BOOT_DEBUG_INFO
        NRF_LOG_INFO("HwLog thread boot error");
#endif
    }
#endif
     
#if 1    
// Check boot result
    if ( hwBootStatus_LED1 == HW_BOOT_SUCCESS && 
         hwBootStatus_LED2 == HW_BOOT_SUCCESS && 
         hwBootStatus_Measure == HW_BOOT_SUCCESS )
    {
        hwBootData.hwBootStatus = HW_BOOT_SUCCESS;
//        HwAPI_LED1_On();
        HwAPI_LED1_Flash( STATUS_LED_FLASH_FAST );
#ifdef HWAPI_BOOT_DEBUG_INFO
        NRF_LOG_INFO("HwBoot thread complete");
#endif
    }
    else if ( hwBootStatus_LED1 == HW_BOOT_ERROR ||
              hwBootStatus_LED2 == HW_BOOT_ERROR ||
              hwBootStatus_Measure == HW_BOOT_ERROR )
    {
        hwBootData.hwBootStatus = HW_BOOT_ERROR;
        HwAPI_LED1_Flash( STATUS_LED_FLASH_SLOW );
#ifdef HWAPI_BOOT_DEBUG_INFO
        NRF_LOG_INFO("HwBoot thread result: HW_BOOT_ERROR");
#endif
    }
    else
    {
        hwBootData.hwBootStatus = HW_BOOT_IDLE;
        HwAPI_LED1_Flash( STATUS_LED_FLASH_FAST );
#ifdef HWAPI_BOOT_DEBUG_INFO
        NRF_LOG_INFO("HwBoot thread result: HW_BOOT_IDLE");
#endif
    }
#endif

    while ( 1 )
    {
        vTaskSuspend(xTask_HwBoot);
    }
}
/* End of file */
