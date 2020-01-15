/*****************************************************************************************************

    Project: 
    
    Date: 
    Author: Filipp Popov

    Overview: Task - Hw File Manager

*******************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "ff.h"
#include "diskio_blkdev.h"
#include "nrf_block_dev_sdc.h"

#include "nrf_gpio.h"
#include "nrf_gpiote.h"
#include "nrf_drv_gpiote.h"
#include "nrf_drv_saadc.h"
#include "nrf_drv_timer.h"
#include "app_timer.h"
#include "nrf_ppi.h"
#include "nrf_drv_ppi.h"
#include "nrf_drv_clock.h"


#define NRF_LOG_MODULE_NAME TS_HwFileManager
#include "nrf_log.h"
NRF_LOG_MODULE_REGISTER();

#include "HwAPI.h"
#include "bleAPI.h"

TaskHandle_t xTask_HwFileManager;
QueueHandle_t xQueue_HwFileManager_Rx;
volatile HwAPI_BootStatus_t bootStatus_HwFileManager = HW_TASK_BOOT_IDLE;

// Declare private functions
static void InitFileManagerHardware( void );

// Declare private variables



//static char message[200] = {""};

void vTask_HwFileManager( void *pvParameters )
{
    HwFileManagerQueueData_t hwFileManagerQueueData;
    
#ifdef FILE_MANAGER_LOG_ENABLE
    NRF_LOG_INFO("HwFileManager: thread started");
#endif    
	
    hwFileManagerQueueData.stateHwFileManager = HW_FILE_MANAGER_INIT;
    xQueueSend( xQueue_HwFileManager_Rx, &hwFileManagerQueueData, NULL );
	while ( 1 )
	{
        xQueueReceive( xQueue_HwFileManager_Rx, &hwFileManagerQueueData, portMAX_DELAY );
        switch ( hwFileManagerQueueData.stateHwFileManager )
        {
            case HW_FILE_MANAGER_INIT:
            {
				InitFileManagerHardware();
                bootStatus_HwFileManager = HW_TASK_BOOT_PENDING;
                break;
            }

            case HW_FILE_MANAGER_CREATE_FILE:
            {
                break;
            }

            case HW_FILE_MANAGER_WRITE_FILE:
            {
                break;
            }

            case HW_FILE_MANAGER_READ_FILE:
            {
                break;
            }

            case HW_FILE_MANAGER_RUN_RECORD:
            {
                break;
            }
            
            case HW_FILE_MANAGER_STOP_RECORD:
            {
                break;
            }
            
            case HW_FILE_MANAGER_IDLE:
            {
                break;
            }                

            default:
                break;
        }
    }
}


//*************************************************
//
// Private function
//
// Initialize hardware
//
//*************************************************
static void InitFileManagerHardware( void )
{
    // Init FatFs

}


/* End of file */
