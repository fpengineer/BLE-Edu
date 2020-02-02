/*****************************************************************************************************

    HwAPI Status LED



*******************************************************************************************************/
#ifndef _HWAPI_LEDN_H_
#define _HWAPI_LEDN_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>


#include "HwAPI.h"

// Uncomment next line to enable module logging
#define LED_LOG_ENABLE


// LED port/pin defines
#define LED1_PIN    14      // P0.14
#define LED2_PIN    15      // P0.15

#define STATUS_LED_FLASH_SLOW		500 // delay in milliseconds
#define STATUS_LED_FLASH_FAST		200 // delay in milliseconds

#define LED_FLASH_SLOW		        STATUS_LED_FLASH_SLOW
#define LED_FLASH_FAST		        STATUS_LED_FLASH_FAST


/* Exported types ------------------------------------------------------------*/
enum stateHwLED {
    HW_LED_INIT,
    HW_LED_ON,
    HW_LED_OFF,
    HW_LED_FLASH,
    HW_LED_IDLE
};

typedef struct {
    enum stateHwLED stateHwLED;
    uint16_t delay_ms;
} HwLEDQueueData_t;


/* Exported functions --------------------------------------------------------*/
HwAPI_Status_t HwAPI_LED1_On( void );
HwAPI_Status_t HwAPI_LED1_Off( void );
HwAPI_Status_t HwAPI_LED1_Flash( uint16_t delay_ms );
HwAPI_Status_t HwAPI_LED2_On( void );
HwAPI_Status_t HwAPI_LED2_Off( void );
HwAPI_Status_t HwAPI_LED2_Flash( uint16_t delay_ms );

void vTask_HwLED1( void *pvParameters );
void vTask_HwLED2( void *pvParameters );
HwAPI_BootStatus_t HwAPI_LED1_Run( void );
HwAPI_BootStatus_t HwAPI_LED2_Run( void );

#endif /* _HWAPI_LEDN_H_*/
/* End of file */
