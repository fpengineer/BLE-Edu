/*****************************************************************************************************

    Project: 
    
    Date: 
    Author: Filipp Popov

    Overview: HwAPI Logging

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
HwAPI_Status_t HwAPI_Log_Info( char *message )
{
    extern QueueHandle_t xQueue_HwLog_Rx;
    HwLogQueueData_t hwLogQueueData;

    hwLogQueueData.stateHwLog = HW_LOG_SEND_INFO;
    hwLogQueueData.message = message;

    while ( uxQueueSpacesAvailable( xQueue_HwLog_Rx ) == 0 ){;}
    xQueueSend( xQueue_HwLog_Rx, &hwLogQueueData, NULL );

	vTaskDelay(10);
    return HW_API_OK;
}


//
HwAPI_Status_t HwAPI_Log_Warning( char *message )
{
    extern QueueHandle_t xQueue_HwLog_Rx;
    HwLogQueueData_t hwLogQueueData;

    hwLogQueueData.stateHwLog = HW_LOG_SEND_WARNING;
    hwLogQueueData.message = message;

    while ( uxQueueSpacesAvailable( xQueue_HwLog_Rx ) == 0 ){;}
    xQueueSend( xQueue_HwLog_Rx, &hwLogQueueData, NULL );

	return HW_API_OK;
}


//
HwAPI_Status_t HwAPI_Log_Error( char *message )
{
    extern QueueHandle_t xQueue_HwLog_Rx;
    HwLogQueueData_t hwLogQueueData;

    hwLogQueueData.stateHwLog = HW_LOG_SEND_ERROR;
    hwLogQueueData.message = message;

    while ( uxQueueSpacesAvailable( xQueue_HwLog_Rx ) == 0 ){;}
    xQueueSend( xQueue_HwLog_Rx, &hwLogQueueData, NULL );

	return HW_API_OK;
}


//
HwAPI_Status_t HwAPI_Log_Debug( char *message )
{
    extern QueueHandle_t xQueue_HwLog_Rx;
    HwLogQueueData_t hwLogQueueData;

    hwLogQueueData.stateHwLog = HW_LOG_SEND_DEBUG;
    hwLogQueueData.message = message;

    while ( uxQueueSpacesAvailable( xQueue_HwLog_Rx ) == 0 ){;}
    xQueueSend( xQueue_HwLog_Rx, &hwLogQueueData, NULL );

	return HW_API_OK;
}


//
HwAPI_BootStatus_t HwAPI_Log_Run( void )
{
    extern TaskHandle_t xTask_HwLog;
    extern QueueHandle_t xQueue_HwLog_Rx;
    extern volatile HwAPI_BootStatus_t bootStatus_HwLog;
    
    xQueue_HwLog_Rx = xQueueCreate( 10, sizeof( HwLogQueueData_t ) );

	if( pdTRUE != xTaskCreate( vTask_HwLog,
                               "Task - HwLog",
                               configMINIMAL_STACK_SIZE,
                               NULL,
                               tskIDLE_PRIORITY + 1,
                               &xTask_HwLog ) ) { /* some error action */ }	

    while ( bootStatus_HwLog == HW_TASK_BOOT_IDLE ){;}
    
    if ( bootStatus_HwLog == HW_TASK_BOOT_PENDING )
    {
        return HW_TASK_BOOT_RUN;
    }
    
    return HW_TASK_BOOT_ERROR;
}
/* End of file */
