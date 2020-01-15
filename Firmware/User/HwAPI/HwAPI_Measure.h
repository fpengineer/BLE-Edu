/*****************************************************************************************************

    HwAPI Measure



*******************************************************************************************************/


#ifndef _HWAPI_MEASURE_H_
#define _HWAPI_MEASURE_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>

#include "HwAPI.h"

// Uncomment next line to enable module logging
#define MEASURE_LOG_ENABLE

#define TEMPERATURE_ANALOG_IN       NRF_SAADC_INPUT_AIN1
#define VOLTAGE_ANALOG_IN           NRF_SAADC_INPUT_AIN2

//#define TEMPERATURE_PIN             3       // P0.03 - AIN1
//#define VOLTAGE_PIN                 4       // P0.04 - AIN2
#define PWM1_PIN                    11      // P0.11
#define PWM2_PIN                    7       // P0.07
#define RPM_PIN                     16      // P0.16

#define MEASURE_TACT_MS     5000

#define ADC_COUNT     4096.0f
#define ADC_GAIN      6.0f
#define ADC_VREF      0.6f
#define AD8495_VREF   1.24f
#define AD8495_RATIO  0.005f

#define TIMER_CLOCK     15.72f // Why? Should be 16.0 MHz

/* Exported types ------------------------------------------------------------*/
enum stateHwMeasure {
    HW_MEASURE_INIT,
    HW_MEASURE_TACT,
    HW_MEASURE_START,
    HW_MEASURE_STOP,
    HW_MEASURE_IDLE
};

#ifndef CALIBRATION_DATA_TYPEDEF
#define CALIBRATION_DATA_TYPEDEF
typedef struct {
    float slope;
    float offset;
} CalibrationData_t;
#endif

typedef struct {
    enum stateHwMeasure stateHwMeasure;
} HwMeasureQueueData_t;

/* Exported functions --------------------------------------------------------*/
HwAPI_Status_t HwAPI_Measure_Start( void );
HwAPI_Status_t HwAPI_Measure_Stop( void );

void vTask_HwMeasure( void *pvParameters );
HwAPI_BootStatus_t HwAPI_Measure_Run( void );

#endif /* _HWAPI_MEASURE_H_*/
/* End of file */
