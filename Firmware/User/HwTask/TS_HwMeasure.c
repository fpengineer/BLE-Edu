/*****************************************************************************************************

    Project: 
    
    Date: 
    Author: Filipp Popov

    Overview: Task - Hw Measure

*******************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "nrf_gpio.h"
#include "nrf_drv_saadc.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "HwAPI.h"


TaskHandle_t xTask_HwMeasure;
QueueHandle_t xQueue_HwMeasure_Rx;
volatile HwAPI_BootStatus_t bootStatus_HwMeasure = HW_TASK_BOOT_IDLE;

// Declare private functions
static void InitMeasureHardware( void );

// Declare private variables
#define SAMPLES_IN_BUFFER 1
static nrf_saadc_value_t m_buffer[SAMPLES_IN_BUFFER];


void vTask_HwMeasure( void *pvParameters )
{
    HwMeasureQueueData_t hwMeasureQueueData;
    
    hwMeasureQueueData.stateHwMeasure = HW_MEASURE_INIT;            
    xQueueSend( xQueue_HwMeasure_Rx, &hwMeasureQueueData, NULL ); 
	while ( 1 )
	{
        xQueueReceive( xQueue_HwMeasure_Rx, &hwMeasureQueueData, portMAX_DELAY );
        switch ( hwMeasureQueueData.stateHwMeasure )
        {
            case HW_MEASURE_INIT:
            {
				InitMeasureHardware();
                bootStatus_HwMeasure = HW_TASK_BOOT_PENDING;
                break;
            }

            case HW_MEASURE_TACT:
            {
                // Perform measure
                nrfx_saadc_sample();
                
                hwMeasureQueueData.stateHwMeasure = HW_MEASURE_TACT;            
                xQueueSend( xQueue_HwMeasure_Rx, &hwMeasureQueueData, NULL ); 
                vTaskDelay( MEASURE_TACT_MS );
                break;
            }
            
            case HW_MEASURE_START:
            {
                hwMeasureQueueData.stateHwMeasure = HW_MEASURE_TACT;            
                xQueueSend( xQueue_HwMeasure_Rx, &hwMeasureQueueData, NULL ); 
                break;
            }
            
            case HW_MEASURE_STOP:
            {
                xQueueReset( xQueue_HwMeasure_Rx ); 
                break;
            }
            
            case HW_MEASURE_IDLE:
            {
                break;
            }                

            default:
                break;
        }
    }
}




void saadc_callback(nrf_drv_saadc_evt_t const * p_event)
{
    if (p_event->type == NRF_DRV_SAADC_EVT_DONE)
    {
        ret_code_t err_code;

        err_code = nrf_drv_saadc_buffer_convert(p_event->data.done.p_buffer, SAMPLES_IN_BUFFER);
        APP_ERROR_CHECK(err_code);
        
        NRF_LOG_INFO("%d\r\n", p_event->data.done.p_buffer[0]);
    }
}



//*************************************************
//
// Private function
//
// Initialize hardware
//
//*************************************************
static void InitMeasureHardware( void )
{
    // Init ADC to measure voltage
    ret_code_t err_code;
    nrf_saadc_channel_config_t channel_config 
        = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN0);

    err_code = nrf_drv_saadc_init(NULL, saadc_callback);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_channel_init(0, &channel_config);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_buffer_convert(m_buffer, SAMPLES_IN_BUFFER);
    APP_ERROR_CHECK(err_code);

    
    
    
    // Init ADC to measure Temperature
    // Init timer to measure PWM1
    // Init timer to measure PWM2
    // Init timer to measure RPM pulses
}





/* End of file */
