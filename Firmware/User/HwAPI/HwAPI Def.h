/*****************************************************************************************************

    HwAPI



*******************************************************************************************************/


#ifndef _HWAPI_DEF_H_
#define _HWAPI_DEF_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>



/* Exported types ------------------------------------------------------------*/
typedef enum {
    HW_API_OK       = 0x00U,
    HW_API_ERROR    = 0x01U,
    HW_API_BUSY     = 0x02U,
    HW_API_TIMEOUT  = 0x03U
} HwAPI_Status_t;

typedef enum {
    HW_TASK_BOOT_PENDING,
    HW_TASK_BOOT_RUN,
    HW_TASK_BOOT_IDLE,
    HW_TASK_BOOT_ERROR
} HwAPI_BootStatus_t;


/* Exported functions --------------------------------------------------------*/


#endif /* _HW_API_DEF_H_ */
/* End of file */
