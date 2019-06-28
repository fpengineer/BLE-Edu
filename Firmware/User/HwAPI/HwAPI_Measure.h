/*****************************************************************************************************

    HwAPI Measure



*******************************************************************************************************/


#ifndef _HWAPI_MEASURE_H_
#define _HWAPI_MEASURE_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>

#include "HwAPI.h"

#define TEMPERATURE_ANALOG_IN       NRF_SAADC_INPUT_AIN0
#define VOLTAGE_ANALOG_IN           NRF_SAADC_INPUT_AIN1

#define TEMPERATURE_PIN             0
#define VOLTAGE_PIN                 1

#define MEASURE_TACT_MS     250

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

#endif /* _HWAPI_DAQ_ADC_H_*/
/* End of file */
