/*****************************************************************************************************

    HwAPI Logging



*******************************************************************************************************/
#ifndef _HWAPI_LOG_H_
#define _HWAPI_LOG_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>


#include "HwAPI.h"


// defines


/* Exported types ------------------------------------------------------------*/
enum stateHwLog {
    HW_LOG_INIT,
    HW_LOG_SEND_INFO,
    HW_LOG_SEND_WARNING,
    HW_LOG_SEND_ERROR,
    HW_LOG_SEND_DEBUG,
    HW_LOG_IDLE
};

typedef struct {
    enum stateHwLog stateHwLog;
    char *message;
} HwLogQueueData_t;


/* Exported functions --------------------------------------------------------*/
HwAPI_Status_t HwAPI_Log_Info( char *message );
HwAPI_Status_t HwAPI_Log_Warning( char *message );
HwAPI_Status_t HwAPI_Log_Error( char *message );
HwAPI_Status_t HwAPI_Log_Debug( char *message );

void vTask_HwLog( void *pvParameters );
HwAPI_BootStatus_t HwAPI_Log_Run( void );

#endif /* _HWAPI_LEDN_H_*/
/* End of file */
