/*
	ble led service
	UUID: 424232e6-d542-4ed2-ab0a-08d826b06855

*/
#ifndef BLE_LEDS_H__
#define BLE_LEDS_H__

#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"

#define LED_SERVICE_UUID_BASE         {0x55, 0x68, 0xB0, 0x26, 0xD8, 0x08, 0x0A, 0xAB, \
                                       0x4E, 0xD2, 0xD5, 0x42, 0xE6, 0x32, 0x42, 0x42}

#define LED_SERVICE_UUID               0x1410
#define LED1_VALUE_CHAR_UUID           0x1411
#define LED2_VALUE_CHAR_UUID           0x1412

                                       
enum bleCommandLED {
    LED_ON = 0x01,
    LED_OFF,
    LED_FLASH
};
                                       

/**@brief   Macro for defining a ble_leds instance.
 *
 * @param   _name   Name of the instance.
 * @hideinitializer
 */
#define BLE_LEDS_DEF(_name)                                                                          \
static ble_leds_t _name;                                                                             \
NRF_SDH_BLE_OBSERVER(_name ## _obs,                                                                 \
                     BLE_HRS_BLE_OBSERVER_PRIO,                                                     \
                     ble_leds_on_ble_evt, &_name)

/**@brief Custom Service init structure. This contains all options and data needed for
 *        initialization of the service.*/
typedef struct
{
    uint8_t                       initial_custom_value;           /**< Initial custom value */
    ble_srv_cccd_security_mode_t  led_value_char_attr_md;         /**< Initial security level for Custom characteristics attribute */
} ble_leds_init_t;


/**@brief Custom Service structure. This contains various status information for the service. */
struct ble_leds_s
{
    uint16_t                      service_handle;                 /**< Handle of Custom Service (as provided by the BLE stack). */
    ble_gatts_char_handles_t      led1_value_handle;              /**< Handles related to the Custom Value characteristic. */
    ble_gatts_char_handles_t      led2_value_handle;              /**< Handles related to the Custom Value characteristic. */
    uint16_t                      conn_handle;                    /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection). */
    uint8_t                       uuid_type; 
};


// Forward declaration of the ble_cus_t type.
typedef struct ble_leds_s ble_leds_t;


/**@brief Function for initializing the Custom Service.
 *
 * @param[out]  p_cus       Custom Service structure. This structure will have to be supplied by
 *                          the application. It will be initialized by this function, and will later
 *                          be used to identify this particular service instance.
 * @param[in]   p_cus_init  Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on successful initialization of service, otherwise an error code.
 */
uint32_t ble_leds_init(ble_leds_t * p_leds, const ble_leds_init_t * p_leds_init);


/**@brief Function for handling the Application's BLE Stack events.
 *
 * @details Handles all events from the BLE stack of interest to the Battery Service.
 *
 * @note 
 *
 * @param[in]   p_ble_evt  Event received from the BLE stack.
 * @param[in]   p_context  Custom Service structure.
 */
void ble_leds_on_ble_evt( ble_evt_t const * p_ble_evt, void * p_context);


#endif /* BLE_LEDS_H__ */
