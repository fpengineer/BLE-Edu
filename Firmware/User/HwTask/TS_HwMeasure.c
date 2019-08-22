/*****************************************************************************************************

    Project: 
    
    Date: 
    Author: Filipp Popov

    Overview: Task - Hw Measure

*******************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "nrf_gpio.h"
#include "nrf_gpiote.h"
#include "nrf_drv_gpiote.h"
#include "nrf_drv_saadc.h"
#include "nrf_drv_timer.h"
#include "app_timer.h"
#include "nrf_ppi.h"
#include "nrf_drv_ppi.h"

#define NRF_LOG_MODULE_NAME TS_HwMeasure
#include "nrf_log.h"
NRF_LOG_MODULE_REGISTER();

#include "HwAPI.h"

TaskHandle_t xTask_HwMeasure;
QueueHandle_t xQueue_HwMeasure_Rx;
volatile HwAPI_BootStatus_t bootStatus_HwMeasure = HW_TASK_BOOT_IDLE;

// Declare private functions
static void InitMeasureHardware( void );

// Declare private variables
#define SAMPLES_IN_BUFFER 16
static nrf_saadc_value_t     m_buffer_pool[2][SAMPLES_IN_BUFFER];
static volatile int16_t bufferADC[SAMPLES_IN_BUFFER];
static volatile uint16_t valuePWM1 = 0;
static volatile uint16_t valuePWM2 = 0;
static volatile uint8_t statusADC = 0;
static volatile uint8_t statusPWM1 = 0;
static volatile uint8_t statusPWM2 = 0;
static volatile uint8_t statusRPM = 0;

static const nrf_drv_timer_t m_timer_PWM1 = NRF_DRV_TIMER_INSTANCE(1);
static const nrf_drv_timer_t m_timer_PWM2 = NRF_DRV_TIMER_INSTANCE(2);
static const nrf_drv_timer_t m_timer_RPM = NRF_DRV_TIMER_INSTANCE(3);

void vTask_HwMeasure( void *pvParameters )
{
    HwMeasureQueueData_t hwMeasureQueueData;
    
    NRF_LOG_INFO("HwMeasure: thread started");

    for (int32_t i = 0; i < SAMPLES_IN_BUFFER; i++)
    {
        bufferADC[i] = 0;
    }
    
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

                hwMeasureQueueData.stateHwMeasure = HW_MEASURE_TACT;            
                xQueueSend( xQueue_HwMeasure_Rx, &hwMeasureQueueData, NULL ); 
                vTaskDelay( MEASURE_TACT_MS );
                break;
            }

            case HW_MEASURE_TACT:
            {
                //NRF_LOG_INFO("HW_MEASURE_TACT");

                // Get temperature and system voltage values
                int32_t i = 0;
                for (i = 0; i < (SAMPLES_IN_BUFFER / 2); i++)
                {
                    nrf_drv_saadc_sample();
                    vTaskDelay(1);
                }

                if (statusADC == 1)
                {
                    int16_t valueTemperature_ADC = 0;
                    int16_t valueSystemVoltage_ADC = 0;
                    for (int32_t i = 0; i < (SAMPLES_IN_BUFFER / 2); i++)
                    {
                        valueSystemVoltage_ADC += bufferADC[i * 2];
                        valueTemperature_ADC += bufferADC[(i * 2) + 1];
                    }
                    valueTemperature_ADC /= (SAMPLES_IN_BUFFER / 2);
                    valueSystemVoltage_ADC /= (SAMPLES_IN_BUFFER / 2);

                    float temperature_F = 0.0f;
                    float temperature_C = 0.0f;
                    float systemVoltage = 0.0f;
                    
                    temperature_C = ((valueTemperature_ADC * ((0.6f * 6.0f) / 4096.0f)) - 1.24f) / 0.005f;
                    temperature_F = temperature_C * 1.8f + 32.0f;
                    systemVoltage = valueSystemVoltage_ADC * ((0.6f * 6.0f) / 4096.0f);
                    
                    NRF_LOG_INFO("temperature = "NRF_LOG_FLOAT_MARKER" C", NRF_LOG_FLOAT(temperature_C));
                    NRF_LOG_INFO("temperature = "NRF_LOG_FLOAT_MARKER" F", NRF_LOG_FLOAT(temperature_F));
                    NRF_LOG_INFO("system voltage = "NRF_LOG_FLOAT_MARKER" V", NRF_LOG_FLOAT(systemVoltage));
                    statusADC = 0;
                }
                else
                {
                    NRF_LOG_INFO("temperature = NaN");
                    NRF_LOG_INFO("system voltage = NaN");
                }
                
                // Get PWM1 value
                if (statusPWM1 == 1)
                {
                    NRF_LOG_INFO("pwm1 = "NRF_LOG_FLOAT_MARKER" us", NRF_LOG_FLOAT((float)valuePWM1 / 15.72f));
                    statusPWM1 = 0;
                }
                else
                {
                    NRF_LOG_INFO("pwm1 = NaN");
                }
                
                // Get PWM2 value
                if (statusPWM2 == 1)
                {
                    NRF_LOG_INFO("pwm2 = "NRF_LOG_FLOAT_MARKER" us", NRF_LOG_FLOAT((float)valuePWM2 / 15.72f));
                    statusPWM2 = 0;
                }
                else
                {
                    NRF_LOG_INFO("pwm2 = NaN");
                }
                
                // Get RPM value
                if (statusRPM == 1)
                {
                    uint16_t valueRPM = 0;
                    valueRPM = nrf_drv_timer_capture_get(&m_timer_RPM, NRF_TIMER_CC_CHANNEL0);
                    NRF_LOG_INFO("engine = "NRF_LOG_FLOAT_MARKER" rpm", NRF_LOG_FLOAT((15720000.0f / (float)valueRPM) * 2.0f));
                    statusRPM = 0;
                }
                else
                {
                    NRF_LOG_INFO("engine = NaN rpm");
                }
                
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
        
        for (int32_t i = 0; i < SAMPLES_IN_BUFFER; i++)
        {
            bufferADC[i] = p_event->data.done.p_buffer[i];
        }

        statusADC = 1;
    }
}

void timer_handler(nrf_timer_event_t event_type, void * p_context)
{
    //NRF_LOG_INFO("Timer event handler.");
}

void gpiote_evt_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    
    switch ( pin )
    {
        case PWM1_PIN:
        {
            if (!nrf_gpio_pin_read(PWM1_PIN))
            {    
                valuePWM1 = nrf_drv_timer_capture_get(&m_timer_PWM1, NRF_TIMER_CC_CHANNEL0);
                statusPWM1 = 1;
            }
            break;
        }
        
        case PWM2_PIN:
        {
            if (!nrf_gpio_pin_read(PWM2_PIN))
            {    
                valuePWM2 = nrf_drv_timer_capture_get(&m_timer_PWM2, NRF_TIMER_CC_CHANNEL0);
                statusPWM2 = 1;
            }
            break;
        }
        
        case RPM_PIN:
        {
            statusRPM = 1;
            break;
        }
        
        default:
            break;
        
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

    err_code = nrf_drv_saadc_channel_init(2, &channel_temperature_config);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_channel_init(6, &channel_voltage_config);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_buffer_convert(m_buffer_pool[0], SAMPLES_IN_BUFFER);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_buffer_convert(m_buffer_pool[1], SAMPLES_IN_BUFFER);
    APP_ERROR_CHECK(err_code);

    
    // Init measure PWM1
    err_code = nrf_drv_ppi_init();
    APP_ERROR_CHECK(err_code);
    
    err_code = nrf_drv_gpiote_init();
    APP_ERROR_CHECK(err_code);
    
    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
    timer_cfg.mode = NRF_TIMER_MODE_TIMER;
    timer_cfg.bit_width = NRF_TIMER_BIT_WIDTH_16;
    timer_cfg.frequency = NRF_TIMER_FREQ_16MHz;
    err_code = nrf_drv_timer_init(&m_timer_PWM1, &timer_cfg, timer_handler);
    APP_ERROR_CHECK(err_code);
    
    uint32_t pwm1_gpiote_evt_addr;
    uint32_t pwm1_capture_task_addr;
    uint32_t pwm1_clear_task_addr;
    nrf_ppi_channel_t pwm1_capture_ppi_channel;
    nrf_ppi_channel_t pwm1_clear_ppi_channel;
    nrf_drv_gpiote_in_config_t pwm1_config;
    
    pwm1_config.sense = NRF_GPIOTE_POLARITY_TOGGLE;
    pwm1_config.pull = NRF_GPIO_PIN_NOPULL;
    pwm1_config.is_watcher = false;
    pwm1_config.hi_accuracy = true;
    
    nrf_drv_gpiote_in_init(PWM1_PIN, &pwm1_config, gpiote_evt_handler);
    APP_ERROR_CHECK(err_code);
    
    err_code = nrf_drv_ppi_channel_alloc(&pwm1_capture_ppi_channel); // get free ppi channel
    APP_ERROR_CHECK(err_code);
    err_code = nrf_drv_ppi_channel_alloc(&pwm1_clear_ppi_channel); // get free ppi channel
    APP_ERROR_CHECK(err_code);
    
    pwm1_capture_task_addr = nrf_drv_timer_task_address_get(&m_timer_PWM1, NRF_TIMER_TASK_CAPTURE0);
    pwm1_clear_task_addr = nrf_drv_timer_task_address_get(&m_timer_PWM1, NRF_TIMER_TASK_CLEAR);
    pwm1_gpiote_evt_addr = nrf_drv_gpiote_in_event_addr_get(PWM1_PIN);
    
    err_code = nrf_drv_ppi_channel_assign(pwm1_capture_ppi_channel, pwm1_gpiote_evt_addr, pwm1_capture_task_addr);
    APP_ERROR_CHECK(err_code);
    err_code = nrf_drv_ppi_channel_assign(pwm1_clear_ppi_channel, pwm1_gpiote_evt_addr, pwm1_clear_task_addr);
    APP_ERROR_CHECK(err_code);
    
    err_code = nrf_drv_ppi_channel_enable(pwm1_capture_ppi_channel);
    APP_ERROR_CHECK(err_code);
    err_code = nrf_drv_ppi_channel_enable(pwm1_clear_ppi_channel);
    APP_ERROR_CHECK(err_code);
    
    nrf_drv_gpiote_in_event_enable(PWM1_PIN, true);
    
    nrf_drv_timer_enable(&m_timer_PWM1);


    // Init measure PWM2
    timer_cfg.mode = NRF_TIMER_MODE_TIMER;
    timer_cfg.bit_width = NRF_TIMER_BIT_WIDTH_16;
    timer_cfg.frequency = NRF_TIMER_FREQ_16MHz;
    err_code = nrf_drv_timer_init(&m_timer_PWM2, &timer_cfg, timer_handler);
    APP_ERROR_CHECK(err_code);
    
    uint32_t pwm2_gpiote_evt_addr;
    uint32_t pwm2_capture_task_addr;
    uint32_t pwm2_clear_task_addr;
    nrf_ppi_channel_t pwm2_capture_ppi_channel;
    nrf_ppi_channel_t pwm2_clear_ppi_channel;
    nrf_drv_gpiote_in_config_t pwm2_config;
    
    pwm2_config.sense = NRF_GPIOTE_POLARITY_TOGGLE;
    pwm2_config.pull = NRF_GPIO_PIN_NOPULL;
    pwm2_config.is_watcher = false;
    pwm2_config.hi_accuracy = true;
    
    nrf_drv_gpiote_in_init(PWM2_PIN, &pwm2_config, gpiote_evt_handler);
    APP_ERROR_CHECK(err_code);
    
    err_code = nrf_drv_ppi_channel_alloc(&pwm2_capture_ppi_channel); // get free ppi channel
    APP_ERROR_CHECK(err_code);
    err_code = nrf_drv_ppi_channel_alloc(&pwm2_clear_ppi_channel); // get free ppi channel
    APP_ERROR_CHECK(err_code);
    
    pwm2_capture_task_addr = nrf_drv_timer_task_address_get(&m_timer_PWM2, NRF_TIMER_TASK_CAPTURE0);
    pwm2_clear_task_addr = nrf_drv_timer_task_address_get(&m_timer_PWM2, NRF_TIMER_TASK_CLEAR);
    pwm2_gpiote_evt_addr = nrf_drv_gpiote_in_event_addr_get(PWM2_PIN);
    
    err_code = nrf_drv_ppi_channel_assign(pwm2_capture_ppi_channel, pwm2_gpiote_evt_addr, pwm2_capture_task_addr);
    APP_ERROR_CHECK(err_code);
    err_code = nrf_drv_ppi_channel_assign(pwm2_clear_ppi_channel, pwm2_gpiote_evt_addr, pwm2_clear_task_addr);
    APP_ERROR_CHECK(err_code);
    
    err_code = nrf_drv_ppi_channel_enable(pwm2_capture_ppi_channel);
    APP_ERROR_CHECK(err_code);
    err_code = nrf_drv_ppi_channel_enable(pwm2_clear_ppi_channel);
    APP_ERROR_CHECK(err_code);
    
    nrf_drv_gpiote_in_event_enable(PWM2_PIN, true);
    
    nrf_drv_timer_enable(&m_timer_PWM2);


    // Init measure RPM
    timer_cfg.mode = NRF_TIMER_MODE_TIMER;
    timer_cfg.bit_width = NRF_TIMER_BIT_WIDTH_24;
    timer_cfg.frequency = NRF_TIMER_FREQ_16MHz;
    err_code = nrf_drv_timer_init(&m_timer_RPM, &timer_cfg, timer_handler);
    APP_ERROR_CHECK(err_code);
    
    uint32_t rpm_gpiote_evt_addr;
    uint32_t rpm_capture_task_addr;
    uint32_t rpm_clear_task_addr;
    nrf_ppi_channel_t rpm_capture_ppi_channel;
    nrf_ppi_channel_t rpm_clear_ppi_channel;
    nrf_drv_gpiote_in_config_t rpm_config;
  
    rpm_config.sense = NRF_GPIOTE_POLARITY_HITOLO;
    rpm_config.pull = NRF_GPIO_PIN_NOPULL;
    rpm_config.is_watcher = false;
    rpm_config.hi_accuracy = true;
  
    nrf_drv_gpiote_in_init(RPM_PIN, &rpm_config, gpiote_evt_handler);
    APP_ERROR_CHECK(err_code);
  
    err_code = nrf_drv_ppi_channel_alloc(&rpm_capture_ppi_channel); // get free ppi channel
    APP_ERROR_CHECK(err_code);
    err_code = nrf_drv_ppi_channel_alloc(&rpm_clear_ppi_channel); // get free ppi channel
    APP_ERROR_CHECK(err_code);
  
    rpm_capture_task_addr = nrf_drv_timer_task_address_get(&m_timer_RPM, NRF_TIMER_TASK_CAPTURE0);
    rpm_clear_task_addr = nrf_drv_timer_task_address_get(&m_timer_RPM, NRF_TIMER_TASK_CLEAR);
    rpm_gpiote_evt_addr = nrf_drv_gpiote_in_event_addr_get(RPM_PIN);
  
    err_code = nrf_drv_ppi_channel_assign(rpm_capture_ppi_channel, rpm_gpiote_evt_addr, rpm_capture_task_addr);
    APP_ERROR_CHECK(err_code);
    err_code = nrf_drv_ppi_channel_assign(rpm_clear_ppi_channel, rpm_gpiote_evt_addr, rpm_clear_task_addr);
    APP_ERROR_CHECK(err_code);
  
    err_code = nrf_drv_ppi_channel_enable(rpm_capture_ppi_channel);
    APP_ERROR_CHECK(err_code);
    err_code = nrf_drv_ppi_channel_enable(rpm_clear_ppi_channel);
    APP_ERROR_CHECK(err_code);
  
    nrf_drv_gpiote_in_event_enable(RPM_PIN, true);
  
    nrf_drv_timer_enable(&m_timer_RPM);
}
/* End of file */
