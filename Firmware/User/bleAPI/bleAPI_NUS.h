/*****************************************************************************************************

    bleAPI Nordic UART Service 



*******************************************************************************************************/
#ifndef _BLE_API_NUS_H_
#define _BLE_API_NUS_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

#include "bleAPI.h"



/* Exported types ------------------------------------------------------------*/


/* Exported functions --------------------------------------------------------*/
void SendMessageNUS(const char *message, ...);

#endif /* _BLE_API_NUS_H_*/
/* End of file */
