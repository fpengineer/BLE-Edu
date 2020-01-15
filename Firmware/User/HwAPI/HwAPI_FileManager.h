/*****************************************************************************************************

    HwAPI File Manager



*******************************************************************************************************/
#ifndef _HWAPI_FILE_MANAGER_H_
#define _HWAPI_FILE_MANAGER_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>

#include "HwAPI.h"

// Uncomment next line to enable module logging
#define FILE_MANAGER_LOG_ENABLE

/* Exported types ------------------------------------------------------------*/
enum stateHwFileManager {
    HW_FILE_MANAGER_INIT,
    HW_FILE_MANAGER_CREATE_FILE,
    HW_FILE_MANAGER_WRITE_FILE,
    HW_FILE_MANAGER_READ_FILE,
    HW_FILE_MANAGER_RUN_RECORD,
    HW_FILE_MANAGER_STOP_RECORD,
    HW_FILE_MANAGER_IDLE
};

typedef struct {
    enum stateHwFileManager stateHwFileManager;
} HwFileManagerQueueData_t;

/* Exported functions --------------------------------------------------------*/
HwAPI_Status_t HwAPI_FileManager_CreateFile( void );
HwAPI_Status_t HwAPI_FileManager_WriteFile( void );
HwAPI_Status_t HwAPI_FileManager_ReadFile( void );
HwAPI_Status_t HwAPI_FileManager_RunRecord( void );
HwAPI_Status_t HwAPI_FileManager_StopRecord( void );

void vTask_HwFileManager( void *pvParameters );
HwAPI_BootStatus_t HwAPI_FileManager_Run( void );

#endif /* _HWAPI_FILE_MANAGER_H_*/
/* End of file */
