/*****************************************************************************************************

    Project: 
    
    Date: 
    Author: Filipp Popov

    Overview: Task - Hw LED2

*******************************************************************************************************/
#include <stdio.h>
#include <string.h>

#include "stdint.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "HwAPI.h"


TaskHandle_t xTask_HwLED2;
QueueHandle_t xQueue_HwLED2_Rx;
volatile HwAPI_BootStatus_t bootStatus_HwLED2 = HW_TASK_BOOT_IDLE;

// Declare private functions
static void Init_LED_Hardware( void );

// Declare private variables


void vTask_HwLED2( void *pvParameters )
{
    HwLEDQueueData_t hwLEDQueueData;
    TickType_t timeout = 0;
    
    hwLEDQueueData.stateHwLED = HW_LED_INIT;            
    xQueueSend( xQueue_HwLED2_Rx, &hwLEDQueueData, NULL ); 
	while ( 1 )
	{
        xQueueReceive( xQueue_HwLED2_Rx, &hwLEDQueueData, timeout );
        switch ( hwLEDQueueData.stateHwLED )
        {
            case HW_LED_INIT:
            {
				Init_LED_Hardware();
				LED2_Off();
				
                bootStatus_HwLED2 = HW_TASK_BOOT_PENDING;
                break;
            }

            case HW_LED_ON:
            {
                LED2_On();
                timeout = portMAX_DELAY;
                break;
            }
            
            case HW_LED_OFF:
            {
				LED2_Off();
                timeout = portMAX_DELAY;
                break;
            }
            
            case HW_LED_FLASH:
            {
				LED2_Toggle();
                timeout = hwLEDQueueData.delay_ms;
                break;
            }                
            case HW_LED_IDLE:
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
static void Init_LED_Hardware( void )
{

}

/* End of file */
