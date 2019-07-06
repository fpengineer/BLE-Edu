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
#include "nrf_drv_timer.h"

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
#define SAMPLES_IN_BUFFER 16
static nrf_saadc_value_t     m_buffer_pool[2][SAMPLES_IN_BUFFER];
static uint16_t valueTemperature_ADC = 0;
static uint16_t valueVoltage_ADC = 0;
static uint8_t statusADC = 0;
static uint8_t statusPWM1 = 0;
static uint8_t statusPWM2 = 0;
static uint8_t statusRPM = 0;

//static const nrf_drv_timer_t m_timer_PWM1 = NRF_DRV_TIMER_INSTANCE(0);
//static const nrf_drv_timer_t m_timer_PWM2 = NRF_DRV_TIMER_INSTANCE(1);
//static const nrf_drv_timer_t m_timer_RPM = NRF_DRV_TIMER_INSTANCE(2);


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
				//InitMeasureHardware();
                bootStatus_HwMeasure = HW_TASK_BOOT_PENDING;

                //hwMeasureQueueData.stateHwMeasure = HW_MEASURE_TACT;            
                //xQueueSend( xQueue_HwMeasure_Rx, &hwMeasureQueueData, NULL ); 
                break;
            }

            case HW_MEASURE_TACT:
            {
                //HwAPI_Log_Info( "HW_MEASURE_TACT start" );
                // Perform measure
                statusADC = 0;
                statusPWM1 = 0;
                statusPWM2 = 0;
                statusRPM = 0;

                nrf_drv_saadc_sample();
                //nrf_drv_timer_enable(&m_timer_PWM1);
                //nrf_drv_timer_enable(&m_timer_PWM1);
                //nrf_drv_timer_enable(&m_timer_RPM);
                
                
                // Wait for measure complete
                while( !statusADC || !statusPWM1 || !statusPWM2 || !statusRPM ){;}
                
                hwMeasureQueueData.stateHwMeasure = HW_MEASURE_TACT;            
                xQueueSend( xQueue_HwMeasure_Rx, &hwMeasureQueueData, NULL ); 
                //HwAPI_Log_Info( "HW_MEASURE_START complete" );
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
        
        valueTemperature_ADC = 0;
        valueVoltage_ADC = 0;
        for (int32_t i = 0; i < (SAMPLES_IN_BUFFER / 2); i++)
        {
            valueTemperature_ADC += p_event->data.done.p_buffer[i * 2];
            valueVoltage_ADC += p_event->data.done.p_buffer[(i * 2) + 1];
        }
        valueTemperature_ADC /= (SAMPLES_IN_BUFFER / 2);
        valueVoltage_ADC /= (SAMPLES_IN_BUFFER / 2);
    
        statusADC = 1;
        
        NRF_LOG_INFO("SAADC handler.\n");
    }
}


void timer_handler(nrf_timer_event_t event_type, void * p_context)
{
    //NRF_LOG_INFO("Timer handler.\n");
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
    // Init ADC to measure voltage and temperature
    ret_code_t err_code;
    nrf_saadc_channel_config_t channel_temperature_config 
        = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(TEMPERATURE_ANALOG_IN);

    nrf_saadc_channel_config_t channel_voltage_config 
        = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(VOLTAGE_ANALOG_IN);

    err_code = nrf_drv_saadc_init(NULL, saadc_callback);
    APP_ERROR_CHECK(err_code);

    err_code = nrfx_saadc_calibrate_offset();
    APP_ERROR_CHECK(err_code);
    
    while(nrfx_saadc_is_busy()) // Wait for calibration to complete
    {
        __WFE();    //
        __SEV();    //
        __WFE();    // This sequence puts the system to sleep (SystemON) while waiting
    }

    err_code = nrf_drv_saadc_channel_init(TEMPERATURE_PIN, &channel_temperature_config);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_channel_init(VOLTAGE_PIN, &channel_voltage_config);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_buffer_convert(m_buffer_pool[0], SAMPLES_IN_BUFFER);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_buffer_convert(m_buffer_pool[1], SAMPLES_IN_BUFFER);
    APP_ERROR_CHECK(err_code);

/*    
    // Init timer to measure PWM1
    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
    timer_cfg.bit_width = NRF_TIMER_BIT_WIDTH_32;
    err_code = nrf_drv_timer_init(&m_timer, &timer_cfg, timer_handler);
    APP_ERROR_CHECK(err_code);

    uint32_t time_ticks = nrf_drv_timer_ms_to_ticks(&m_timer, 500);

    nrf_drv_timer_extended_compare(
         &m_timer, NRF_TIMER_CC_CHANNEL0, time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);

//    nrf_drv_timer_enable(&m_timer);


    // Init timer to measure PWM2
*/

    // Init timer to measure RPM pulses
}





/* End of file */
