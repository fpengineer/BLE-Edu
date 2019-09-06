/*****************************************************************************************************

    HwAPI
    
    Defines

*******************************************************************************************************/


#ifndef _BLE_API_DEF_H_
#define _BLE_API_DEF_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>



/* Exported types ------------------------------------------------------------*/
typedef enum {
    BLE_API_OK       = 0x00U,
    BLE_API_ERROR    = 0x01U,
    BLE_API_BUSY     = 0x02U,
    BLE_API_TIMEOUT  = 0x03U
} bleAPI_Status_t;


/* Exported functions --------------------------------------------------------*/


#endif /* _BLE_API_DEF_H_ */
/* End of file */
