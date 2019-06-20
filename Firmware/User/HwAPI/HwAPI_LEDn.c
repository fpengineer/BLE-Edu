/*****************************************************************************************************

    Project: 
    
    Date: 
    Author: Filipp Popov

    Overview: HwAPI for Status LED

*******************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "HwAPI.h"


//
HwAPI_Status_t HwAPI_LED1_On( void )
{
    extern QueueHandle_t xQueue_HwLED1_Rx;
    HwLEDQueueData_t hwLEDQueueData;

    hwLEDQueueData.stateHwLED = HW_LED_ON;

    xQueueSend( xQueue_HwLED1_Rx, &hwLEDQueueData, NULL );

	return HW_API_OK;
}


//
HwAPI_Status_t HwAPI_LED1_Off( void )
{
    extern QueueHandle_t xQueue_HwLED1_Rx;
    HwLEDQueueData_t hwLEDQueueData;

    hwLEDQueueData.stateHwLED = HW_LED_OFF;

    xQueueSend( xQueue_HwLED1_Rx, &hwLEDQueueData, NULL );

	return HW_API_OK;
}


//
HwAPI_Status_t HwAPI_LED1_Flash( uint16_t delay_ms )
{
    extern QueueHandle_t xQueue_HwLED1_Rx;
    HwLEDQueueData_t hwLEDQueueData;

    hwLEDQueueData.stateHwLED = HW_LED_FLASH;
    hwLEDQueueData.delay_ms = delay_ms;
    
    xQueueSend( xQueue_HwLED1_Rx, &hwLEDQueueData, NULL );

	return HW_API_OK;
}


//
HwAPI_Status_t HwAPI_LED2_On( void )
{
    extern QueueHandle_t xQueue_HwLED2_Rx;
    HwLEDQueueData_t hwLEDQueueData;

    hwLEDQueueData.stateHwLED = HW_LED_ON;

    xQueueSend( xQueue_HwLED2_Rx, &hwLEDQueueData, NULL );

	return HW_API_OK;
}


//
HwAPI_Status_t HwAPI_LED2_Off( void )
{
    extern QueueHandle_t xQueue_HwLED2_Rx;
    HwLEDQueueData_t hwLEDQueueData;

    hwLEDQueueData.stateHwLED = HW_LED_OFF;

    xQueueSend( xQueue_HwLED2_Rx, &hwLEDQueueData, NULL );

	return HW_API_OK;
}


//
HwAPI_Status_t HwAPI_LED2_Flash( uint16_t delay_ms )
{
    extern QueueHandle_t xQueue_HwLED2_Rx;
    HwLEDQueueData_t hwLEDQueueData;

    hwLEDQueueData.stateHwLED = HW_LED_FLASH;
    hwLEDQueueData.delay_ms = delay_ms;
    
    xQueueSend( xQueue_HwLED2_Rx, &hwLEDQueueData, NULL );

	return HW_API_OK;
}


//
HwAPI_BootStatus_t HwAPI_LED1_Run( void )
{
    extern TaskHandle_t xTask_HwLED1;
    extern QueueHandle_t xQueue_HwLED1_Rx;
    extern volatile HwAPI_BootStatus_t bootStatus_HwLED1;
    
    xQueue_HwLED1_Rx = xQueueCreate( 2, sizeof( HwLEDQueueData_t ) );

	if( pdTRUE != xTaskCreate(  vTask_HwLED1,
                                "Task - HwLED1",
                                configMINIMAL_STACK_SIZE,
                                NULL,
                                tskIDLE_PRIORITY + 1,
                                &xTask_HwLED1 ) ) { /* some error action */ }	

    while ( bootStatus_HwLED1 == HW_TASK_BOOT_IDLE ){;}
    
    if ( bootStatus_HwLED1 == HW_TASK_BOOT_PENDING )
    {
        return HW_TASK_BOOT_RUN;
    }
    
    return HW_TASK_BOOT_ERROR;
}


//
HwAPI_BootStatus_t HwAPI_LED2_Run( void )
{
    extern TaskHandle_t xTask_HwLED2;
    extern QueueHandle_t xQueue_HwLED2_Rx;
    extern volatile HwAPI_BootStatus_t bootStatus_HwLED2;
    
    xQueue_HwLED2_Rx = xQueueCreate( 2, sizeof( HwLEDQueueData_t ) );

	if( pdTRUE != xTaskCreate(  vTask_HwLED2,
                                "Task - HwLED2",
                                configMINIMAL_STACK_SIZE,
                                NULL,
                                tskIDLE_PRIORITY + 1,
                                &xTask_HwLED2 ) ) { /* some error action */ }

    while ( bootStatus_HwLED2 == HW_TASK_BOOT_IDLE ){;}
    
    if ( bootStatus_HwLED2 == HW_TASK_BOOT_PENDING )
    {
        return HW_TASK_BOOT_RUN;
    }
    
    return HW_TASK_BOOT_ERROR;
}

/* End of file */
