/*
	ble led service
	UUID: 0x1410

*/
#include "sdk_common.h"
#include "ble_srv_common.h"
#include "ble_leds.h"
#include <string.h>
#include "nrf_gpio.h"
#include "boards.h"
#include "HwAPI.h"

#define NRF_LOG_MODULE_NAME ble_leds
#include "nrf_log.h"
NRF_LOG_MODULE_REGISTER();


// Declare private variables

// Declare private functions
static uint32_t custom_value_char_add(ble_leds_t * p_cus, const ble_leds_init_t * p_leds_init);
static void on_connect(ble_leds_t * p_leds, ble_evt_t const * p_ble_evt);
static void on_disconnect(ble_leds_t * p_cus, ble_evt_t const * p_ble_evt);
static void on_write(ble_leds_t * p_leds, ble_evt_t const * p_ble_evt);


uint32_t ble_leds_init(ble_leds_t * p_leds, const ble_leds_init_t * p_leds_init)
{
    if (p_leds == NULL || p_leds_init == NULL)
    {
        return NRF_ERROR_NULL;
    }

    uint32_t   err_code;
    ble_uuid_t ble_uuid;

    // Initialize service structure
    p_leds->conn_handle               = BLE_CONN_HANDLE_INVALID;
    
    // Add Custom Service UUID
    ble_uuid128_t base_uuid = {LED_SERVICE_UUID_BASE};
    err_code =  sd_ble_uuid_vs_add(&base_uuid, &p_leds->uuid_type);
    VERIFY_SUCCESS(err_code);

    ble_uuid.type = p_leds->uuid_type;
    ble_uuid.uuid = LED_SERVICE_UUID;
    
    // Add the Custom Service
    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_leds->service_handle);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Add Custom Value characteristic
    return custom_value_char_add(p_leds, p_leds_init);
}


/**@brief Function for adding the Custom Value characteristic.
 *
 * @param[in]   p_leds        Led Service structure.
 * @param[in]   p_leds_init   Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t custom_value_char_add(ble_leds_t * p_cus, const ble_leds_init_t * p_cus_init)
{
    uint32_t            err_code;
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read   = 0;
    char_md.char_props.write  = 1;
    char_md.char_props.notify = 0; 
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL; 
    char_md.p_sccd_md         = NULL;

    memset(&attr_md, 0, sizeof(attr_md));

    attr_md.read_perm  = p_cus_init->led_value_char_attr_md.read_perm;
    attr_md.write_perm = p_cus_init->led_value_char_attr_md.write_perm;
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 0;

    ble_uuid.type = p_cus->uuid_type;
    ble_uuid.uuid = LED1_VALUE_CHAR_UUID;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = sizeof(uint8_t);
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = sizeof(uint8_t);

    err_code = sd_ble_gatts_characteristic_add(p_cus->service_handle, &char_md,
                                               &attr_char_value,
                                               &p_cus->led1_value_handle);

    ble_uuid.uuid = LED2_VALUE_CHAR_UUID;

    err_code = sd_ble_gatts_characteristic_add(p_cus->service_handle, &char_md,
                                               &attr_char_value,
                                               &p_cus->led2_value_handle);

    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    return NRF_SUCCESS;
}


void ble_leds_on_ble_evt( ble_evt_t const * p_ble_evt, void * p_context)
{
    ble_leds_t * p_leds = (ble_leds_t *) p_context;
    
    if (p_leds == NULL || p_ble_evt == NULL)
    {
        return;
    }

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            on_connect(p_leds, p_ble_evt);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnect(p_leds, p_ble_evt);    
            break;

        case BLE_GATTS_EVT_WRITE:
            on_write(p_leds, p_ble_evt);
            break;
        
        default:
            // No implementation needed.
            break;
    }
}


/**@brief Function for handling the Connect event.
 *
 * @param[in]   p_leds      Led Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_connect(ble_leds_t * p_leds, ble_evt_t const * p_ble_evt)
{
    p_leds->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
}


/**@brief Function for handling the Disconnect event.
 *
 * @param[in]   p_leds      Led Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_disconnect(ble_leds_t * p_leds, ble_evt_t const * p_ble_evt)
{
    UNUSED_PARAMETER(p_ble_evt);
    p_leds->conn_handle = BLE_CONN_HANDLE_INVALID;
}


/**@brief Function for handling the Write event.
 *
 * @param[in]   p_cus       Custom Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_write(ble_leds_t * p_cus, ble_evt_t const * p_ble_evt)
{
    ble_gatts_evt_write_t const * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;
    
    // Check if the handle passed with the event matches the Custom Value Characteristic handle.
    if (p_evt_write->handle == p_cus->led1_value_handle.value_handle)
    {
        switch(p_evt_write->data[0])
        {
            case LED_ON:
                HwAPI_LED1_On();
                break;

            case LED_OFF:
                HwAPI_LED1_Off();
                break;

            case LED_FLASH:
                HwAPI_LED1_Flash(LED_FLASH_SLOW);
                break;

             default:
                NRF_LOG_INFO("invalid command!");
                break;
        }
    }
    else if (p_evt_write->handle == p_cus->led2_value_handle.value_handle)
    {
        switch(p_evt_write->data[0])
        {
            case LED_ON:
                HwAPI_LED2_On();
                break;

            case LED_OFF:
                HwAPI_LED2_Off();
                break;

            case LED_FLASH:
                HwAPI_LED2_Flash(LED_FLASH_SLOW);
                break;

            default:
                NRF_LOG_INFO("invalid command!");
                break;
        }
    }
}
/* End of file */

