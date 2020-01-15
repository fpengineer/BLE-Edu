/*****************************************************************************************************

    Project: 
    
    Date: 
    Author: Filipp Popov

    Overview: HwAPI for File Manager

*******************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "HwAPI.h"


//
HwAPI_Status_t HwAPI_FileManager_CreateFile( void )
{

	return HW_API_OK;
}


//
HwAPI_Status_t HwAPI_FileManager_WriteFile( void )
{

	return HW_API_OK;
}


//
HwAPI_Status_t HwAPI_FileManager_ReadFile( void )
{

	return HW_API_OK;
}


//
HwAPI_Status_t HwAPI_FileManager_RunRecord( void )
{

	return HW_API_OK;
}


//
HwAPI_Status_t HwAPI_FileManager_StopRecord( void )
{

	return HW_API_OK;
}


//
HwAPI_BootStatus_t HwAPI_FileManager_Run( void )
{
    extern TaskHandle_t xTask_HwFileManager;
    extern QueueHandle_t xQueue_HwFileManager_Rx;
    extern volatile HwAPI_BootStatus_t bootStatus_HwFileManager;
    
    xQueue_HwFileManager_Rx = xQueueCreate( 2, sizeof( HwFileManagerQueueData_t ) );

	if( pdTRUE != xTaskCreate(  vTask_HwFileManager,
                                "Task - HwFileManager",
                                configMINIMAL_STACK_SIZE + 500,
                                NULL,
                                tskIDLE_PRIORITY + 1,
                                &xTask_HwFileManager ) ) { /* some error action */ }	

    while ( bootStatus_HwFileManager == HW_TASK_BOOT_IDLE ){;}
    
    if ( bootStatus_HwFileManager == HW_TASK_BOOT_PENDING )
    {
        bootStatus_HwFileManager = HW_TASK_BOOT_RUN;
    }
    
    return bootStatus_HwFileManager;
}

/* End of file */
