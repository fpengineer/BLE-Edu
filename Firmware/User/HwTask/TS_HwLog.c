/*****************************************************************************************************

    Project: 
    
    Date: 
    Author: Filipp Popov

    Overview: Task - Hw Log

*******************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "HwAPI.h"


TaskHandle_t xTask_HwLog;
QueueHandle_t xQueue_HwLog_Rx;
volatile HwAPI_BootStatus_t bootStatus_HwLog = HW_TASK_BOOT_IDLE;

// Declare private functions
static void Init_Log( void );

// Declare private variables


void vTask_HwLog( void *pvParameters )
{
    HwLogQueueData_t hwLogQueueData;

    hwLogQueueData.stateHwLog = HW_LOG_INIT;            
    xQueueSend( xQueue_HwLog_Rx, &hwLogQueueData, NULL ); 
	while ( 1 )
	{
        xQueueReceive( xQueue_HwLog_Rx, &hwLogQueueData, portMAX_DELAY );
        switch ( hwLogQueueData.stateHwLog )
        {
            case HW_LOG_INIT:
            {
				Init_Log();
                bootStatus_HwLog = HW_TASK_BOOT_PENDING;
                break;
            }

            case HW_LOG_SEND_INFO:
            {
                NRF_LOG_INFO( "%s", hwLogQueueData.message );
                break;
            }
            
            case HW_LOG_SEND_WARNING:
            {
                NRF_LOG_WARNING( "%s", hwLogQueueData.message );
                break;
            }
            
            case HW_LOG_SEND_ERROR:
            {
                NRF_LOG_ERROR( "%s", hwLogQueueData.message );
                break;
            }
            
            case HW_LOG_SEND_DEBUG:
            {
                NRF_LOG_DEBUG( "%s", hwLogQueueData.message );
                break;
            }
            
            case HW_LOG_IDLE:
            {
                break;
            }                

            default:
                break;
        }
    }
}


//*************************************************
//
// Private function
//
// Initialize hardware
//
//*************************************************
static void Init_Log( void )
{

}

/* End of file */
