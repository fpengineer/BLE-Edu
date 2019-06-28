/*****************************************************************************************************

    Project: 
    
    Date: 
    Author: Filipp Popov

    Overview: HwAPI for Voltage Source

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
HwAPI_Status_t HwAPI_Measure_Start( void )
{
    extern QueueHandle_t xQueue_HwMeasure_Rx;
    HwMeasureQueueData_t hwMeasureQueueData;

    hwMeasureQueueData.stateHwMeasure = HW_MEASURE_START;

    xQueueSend( xQueue_HwMeasure_Rx, &hwMeasureQueueData, NULL );

	return HW_API_OK;
}


//
HwAPI_Status_t HwAPI_Measure_Stop( void )
{
    extern QueueHandle_t xQueue_HwMeasure_Rx;
    HwMeasureQueueData_t hwMeasureQueueData;

    hwMeasureQueueData.stateHwMeasure = HW_MEASURE_STOP;

    xQueueSendToFront( xQueue_HwMeasure_Rx, &hwMeasureQueueData, NULL );

	return HW_API_OK;
}


//
HwAPI_BootStatus_t HwAPI_Measure_Run( void )
{
    extern TaskHandle_t xTask_HwMeasure;
    extern QueueHandle_t xQueue_HwMeasure_Rx;
    extern volatile HwAPI_BootStatus_t bootStatus_HwMeasure;
    
    xQueue_HwMeasure_Rx = xQueueCreate( 2, sizeof( HwMeasureQueueData_t ) );

	if( pdTRUE != xTaskCreate(  vTask_HwMeasure,
                                "Task - HwMeasure",
                                configMINIMAL_STACK_SIZE,
                                NULL,
                                tskIDLE_PRIORITY + 1,
                                &xTask_HwMeasure ) ) { /* some error action */ }	

    while ( bootStatus_HwMeasure == HW_TASK_BOOT_IDLE ){;}
    
    if ( bootStatus_HwMeasure == HW_TASK_BOOT_PENDING )
    {
        bootStatus_HwMeasure = HW_TASK_BOOT_RUN;
    }
    
    return bootStatus_HwMeasure;
}

/* End of file */
