/*****************************************************************************************************

    Project: 
    
    Date: 
    Author: Filipp Popov

    Overview: Task - Hw LED1

*******************************************************************************************************/
#include <stdio.h>
#include <string.h>

#include "stdint.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "HwAPI.h"


TaskHandle_t xTask_HwLED1;
QueueHandle_t xQueue_HwLED1_Rx;
volatile HwAPI_BootStatus_t bootStatus_HwLED1 = HW_TASK_BOOT_IDLE;

// Declare private functions
static void Init_LED_Hardware( void );

// Declare private variables


void vTask_HwLED1( void *pvParameters )
{
    HwLEDQueueData_t hwLEDQueueData;
    TickType_t timeout = 0;
    
    hwLEDQueueData.stateHwLED = HW_LED_INIT;            
    xQueueSend( xQueue_HwLED1_Rx, &hwLEDQueueData, NULL ); 
	while ( 1 )
	{
        xQueueReceive( xQueue_HwLED1_Rx, &hwLEDQueueData, timeout );
        switch ( hwLEDQueueData.stateHwLED )
        {
            case HW_LED_INIT:
            {
				Init_LED_Hardware();
				LED1_Off();
				
                bootStatus_HwLED1 = HW_TASK_BOOT_PENDING;
                break;
            }

            case HW_LED_ON:
            {
                LED1_On();
                timeout = portMAX_DELAY;
                break;
            }
            
            case HW_LED_OFF:
            {
				LED1_Off();
                timeout = portMAX_DELAY;
                break;
            }
            
            case HW_LED_FLASH:
            {
				LED1_Toggle();
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
