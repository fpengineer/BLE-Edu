/*****************************************************************************************************

    Project: 
    
    Date: 
    Author: Filipp Popov

    Overview: Task - Hw LED2

*******************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "nrf_gpio.h"

#define NRF_LOG_MODULE_NAME TS_HwLED2
#include "nrf_log.h"
NRF_LOG_MODULE_REGISTER();

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
    TickType_t timeout = portMAX_DELAY;

#ifdef LED_LOG_ENABLE
    NRF_LOG_INFO("thread started");
#endif
    
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
                //nrf_gpio_pin_clear( LED2_PIN );
                bootStatus_HwLED2 = HW_TASK_BOOT_PENDING;
                break;
            }

            case HW_LED_ON:
            {
                //nrf_gpio_pin_set( LED2_PIN );
                timeout = portMAX_DELAY;
#ifdef LED_LOG_ENABLE
                NRF_LOG_INFO("HW_LED_ON");
#endif
                break;
            }
            
            case HW_LED_OFF:
            {
                //nrf_gpio_pin_clear( LED2_PIN );
                timeout = portMAX_DELAY;
#ifdef LED_LOG_ENABLE
                NRF_LOG_INFO("HW_LED_OFF");
#endif
                break;
            }
            
            case HW_LED_FLASH:
            {
                //nrf_gpio_pin_toggle( LED2_PIN );
                timeout = hwLEDQueueData.delay_ms;
#ifdef LED_LOG_ENABLE
                NRF_LOG_INFO("HW_LED_FLASH: %dms", hwLEDQueueData.delay_ms);
#endif
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
    nrf_gpio_cfg_output( LED2_PIN );
}


/* End of file */
