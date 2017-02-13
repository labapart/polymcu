/* Copyright (c) 2016 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

#include "nrf_ble_escs.h"
#include <string.h>
#include "es_app_config.h"

#ifdef BLE_HANDLER_DEBUG
    #include "SEGGER_RTT.h"
    #define DEBUG_PRINTF SEGGER_RTT_printf
#else
    #define DEBUG_PRINTF(...)
#endif

// ECS UUIDs
#define BLE_UUID_ESCS_BROADCAST_CAP_CHAR      0x7501
#define BLE_UUID_ESCS_ACTIVE_SLOT_CHAR        0x7502
#define BLE_UUID_ESCS_ADV_INTERVAL_CHAR       0x7503
#define BLE_UUID_ESCS_RADIO_TX_PWR_CHAR       0x7504
#define BLE_UUID_ESCS_ADV_TX_PWR_CHAR         0x7505
#define BLE_UUID_ESCS_LOCK_STATE_CHAR         0x7506
#define BLE_UUID_ESCS_UNLOCK_CHAR             0x7507
#define BLE_UUID_ESCS_PUBLIC_ECDH_KEY_CHAR    0x7508
#define BLE_UUID_ESCS_EID_ID_KEY_CHAR         0x7509
#define BLE_UUID_ESCS_RW_ADV_SLOT_CHAR        0x750A
#define BLE_UUID_ESCS_FACTORY_RESET_CHAR      0x750B
#define BLE_UUID_ESCS_REMAIN_CONNECTABLE_CHAR 0x750C

#define ESCS_BASE_UUID                                                                          \
    {{0x95, 0xE2, 0xED, 0xEB, 0x1B, 0xA0, 0x39, 0x8A, 0xDF, 0x4B, 0xD3, 0x8E, 0x00, 0x00, 0xC8, \
      0xA3}}
// A3C8XXXX-8ED3-4BDF-8A39-A01BEBEDE295

#define NRF_BLE_ESCS_BROADCAST_CAP_LEN (ESCS_NUM_OF_SUPPORTED_TX_POWER + 6) // According to the eddystone spec, there are 6 bytes of data in addition to the supported_radio_tx_power array

// Memory block for ECDH key exchange long writes
#define EID_BUFF_SIZE       64
static uint8_t              m_eid_mem[EID_BUFF_SIZE] = {0};
static ble_user_mem_block_t m_eid_mem_block =
{
    .p_mem = m_eid_mem,
    .len = EID_BUFF_SIZE
};
static uint8_t              m_data_buf[ESCS_ADV_SLOT_CHAR_LENGTH_MAX];


/**@brief Function to reset the active slot to 0
 * @details This function will write to the active slot characteristic
 */
static void reset_active_slot(nrf_ble_escs_t * p_escs)
{
    ret_code_t        err_code;
    uint8_t           reset_slot = 0;
    ble_gatts_value_t value      = {.len = 1, .offset = 0, .p_value = &reset_slot};


    err_code = sd_ble_gatts_value_set(p_escs->conn_handle,
                                      p_escs->active_slot_handles.value_handle,
                                      &value);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for handling the @ref BLE_GAP_EVT_CONNECTED event from the SoftDevice.
 *
 * @param[in] p_escs     Eddystone Configuration Service structure.
 * @param[in] p_ble_evt Pointer to the event received from BLE stack.
 */
static void on_connect(nrf_ble_escs_t * p_escs, ble_evt_t * p_ble_evt)
{
    p_escs->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
    // Reset active slot to 0 on a new connection
    reset_active_slot(p_escs);
}


/**@brief Function for handling the @ref BLE_GAP_EVT_DISCONNECTED event from the SoftDevice.
 *
 * @param[in] p_escs    Eddystone Configuration Service structure.
 * @param[in] p_ble_evt Pointer to the event received from BLE stack.
 */
static void on_disconnect(nrf_ble_escs_t * p_escs, ble_evt_t * p_ble_evt)
{
    UNUSED_PARAMETER(p_ble_evt);
    p_escs->conn_handle = BLE_CONN_HANDLE_INVALID;
}


/**@brief Function for handling the @ref BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST: BLE_GATTS_AUTHORIZE_TYPE_WRITE event from the SoftDevice.
 *
 * @param[in] p_escs     Eddystone Configuration Service structure.
 * @param[in] p_ble_evt Pointer to the event received from BLE stack.
 */
static void on_write(nrf_ble_escs_t * p_escs, ble_evt_t * p_ble_evt)
{
    // ble_gatts_evt_rw_authorize_request_t * p_evt_rw_auth = &p_ble_evt->evt.gatts_evt.params.authorize_request;
    ble_gatts_evt_write_t * p_evt_write =
        &p_ble_evt->evt.gatts_evt.params.authorize_request.request.write;

    if (p_escs->write_evt_handler == NULL)
    {
        return;
    }
    
    if (p_evt_write->handle == p_escs->active_slot_handles.value_handle)
    {
        p_escs->write_evt_handler(p_escs,
                                  NRF_BLE_ESCS_EVT_ACTIVE_SLOT,
                                  p_evt_write->handle,
                                  p_evt_write->data,
                                  p_evt_write->len);
    }
    else if (p_evt_write->handle == p_escs->adv_interval_handles.value_handle)
    {
        p_escs->write_evt_handler(p_escs,
                                  NRF_BLE_ESCS_EVT_ADV_INTERVAL,
                                  p_evt_write->handle,
                                  p_evt_write->data,
                                  p_evt_write->len);
    }
    else if (p_evt_write->handle == p_escs->radio_tx_pwr_handles.value_handle)
    {
        p_escs->write_evt_handler(p_escs,
                                  NRF_BLE_ESCS_EVT_RADIO_TX_PWR,
                                  p_evt_write->handle,
                                  p_evt_write->data,
                                  p_evt_write->len);
    }
    else if (p_evt_write->handle == p_escs->adv_tx_pwr_handles.value_handle)
    {
        p_escs->write_evt_handler(p_escs,
                                  NRF_BLE_ESCS_EVT_ADV_TX_PWR,
                                  p_evt_write->handle,
                                  p_evt_write->data,
                                  p_evt_write->len);
    }
    else if (p_evt_write->handle == p_escs->lock_state_handles.value_handle)
    {
        p_escs->write_evt_handler(p_escs,
                                  NRF_BLE_ESCS_EVT_LOCK_STATE,
                                  p_evt_write->handle,
                                  p_evt_write->data,
                                  p_evt_write->len);
    }
    else if (p_evt_write->handle == p_escs->unlock_handles.value_handle)
    {
        p_escs->write_evt_handler(p_escs,
                                  NRF_BLE_ESCS_EVT_UNLOCK,
                                  p_evt_write->handle,
                                  p_evt_write->data,
                                  p_evt_write->len);
    }
    // BLE_GATTS_OP_PREP_WRITE_REQ & BLE_GATTS_OP_EXEC_WRITE_REQ_NOW are for long writes to the RW ADV slot characteristic
    else if (p_evt_write->op == BLE_GATTS_OP_PREP_WRITE_REQ)
    {
        p_escs->write_evt_handler(p_escs,
                                  NRF_BLE_ESCS_EVT_RW_ADV_SLOT_PREP,
                                  p_evt_write->handle,
                                  p_evt_write->data,
                                  p_evt_write->len);
    }
    else if (p_evt_write->op == BLE_GATTS_OP_EXEC_WRITE_REQ_NOW)
    {
        p_escs->write_evt_handler(p_escs,
                                  NRF_BLE_ESCS_EVT_RW_ADV_SLOT_EXEC,
                                  p_evt_write->handle,
                                  p_evt_write->data,
                                  p_evt_write->len);
    }
    else if (p_evt_write->handle == p_escs->rw_adv_slot_handles.value_handle)
    {
        p_escs->write_evt_handler(p_escs,
                                  NRF_BLE_ESCS_EVT_RW_ADV_SLOT,
                                  p_evt_write->handle,
                                  p_evt_write->data,
                                  p_evt_write->len);
    }
    else if (p_evt_write->handle == p_escs->factory_reset_handles.value_handle)
    {
        p_escs->write_evt_handler(p_escs,
                                  NRF_BLE_ESCS_EVT_FACTORY_RESET,
                                  p_evt_write->handle,
                                  p_evt_write->data,
                                  p_evt_write->len);
    }
    else if (p_evt_write->handle == p_escs->remain_connectable_handles.value_handle)
    {
        p_escs->write_evt_handler(p_escs,
                                  NRF_BLE_ESCS_EVT_REMAIN_CONNECTABLE,
                                  p_evt_write->handle,
                                  p_evt_write->data,
                                  p_evt_write->len);
    }
    else
    {
        // Do Nothing. This event is not relevant for this service.
    }
}


/**@brief Function for handling the @ref BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST: BLE_GATTS_AUTHORIZE_TYPE_READ event from the SoftDevice.
 *
 * @param[in] p_escs     Eddystone Configuration Service structure.
 * @param[in] p_ble_evt Pointer to the event received from BLE stack.
 */
static void on_read(nrf_ble_escs_t * p_escs, ble_evt_t * p_ble_evt)
{
    ble_gatts_evt_read_t * p_evt_read =
        &p_ble_evt->evt.gatts_evt.params.authorize_request.request.read;

    if (p_evt_read->handle == p_escs->broadcast_cap_handles.value_handle)
    {
        p_escs->read_evt_handler(p_escs, NRF_BLE_ESCS_EVT_BROADCAST_CAP, p_evt_read->handle);
    }
    else if (p_evt_read->handle == p_escs->active_slot_handles.value_handle)
    {
        p_escs->read_evt_handler(p_escs, NRF_BLE_ESCS_EVT_ACTIVE_SLOT, p_evt_read->handle);
    }
    else if (p_evt_read->handle == p_escs->adv_interval_handles.value_handle)
    {
        p_escs->read_evt_handler(p_escs, NRF_BLE_ESCS_EVT_ADV_INTERVAL, p_evt_read->handle);
    }
    else if (p_evt_read->handle == p_escs->radio_tx_pwr_handles.value_handle)
    {
        p_escs->read_evt_handler(p_escs, NRF_BLE_ESCS_EVT_RADIO_TX_PWR, p_evt_read->handle);
    }
    else if (p_evt_read->handle == p_escs->adv_tx_pwr_handles.value_handle)
    {
        p_escs->read_evt_handler(p_escs, NRF_BLE_ESCS_EVT_ADV_TX_PWR, p_evt_read->handle);
    }
    else if (p_evt_read->handle == p_escs->unlock_handles.value_handle)
    {
        p_escs->read_evt_handler(p_escs, NRF_BLE_ESCS_EVT_UNLOCK, p_evt_read->handle);
    }
    else if (p_evt_read->handle == p_escs->pub_ecdh_key_handles.value_handle)
    {
        p_escs->read_evt_handler(p_escs, NRF_BLE_ESCS_EVT_PUBLIC_ECDH_KEY, p_evt_read->handle);
    }
    else if (p_evt_read->handle == p_escs->eid_id_key_handles.value_handle)
    {
        p_escs->read_evt_handler(p_escs, NRF_BLE_ESCS_EVT_EID_ID_KEY, p_evt_read->handle);
    }
    else if (p_evt_read->handle == p_escs->rw_adv_slot_handles.value_handle)
    {
        p_escs->read_evt_handler(p_escs, NRF_BLE_ESCS_EVT_RW_ADV_SLOT, p_evt_read->handle);
    }
    else if (p_evt_read->handle == p_escs->remain_connectable_handles.value_handle)
    {
        p_escs->read_evt_handler(p_escs, NRF_BLE_ESCS_EVT_REMAIN_CONNECTABLE, p_evt_read->handle);
    }
    else
    {
        // Do Nothing. This event is not relevant for this service.
    }
}


/**@brief Function for adding braodcast capability characteristic.
 *
 * @param[in] p_escs       Eddystone Configuration Service structure.
 * @param[in] p_escs_init  Information needed to initialize the service.
 *
 * @return NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t broadcast_cap_char_add(nrf_ble_escs_t            * p_escs,
                                    const nrf_ble_escs_init_t * p_escs_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read  = 1;
    char_md.p_char_user_desc = NULL;
    char_md.p_char_pf        = NULL;
    char_md.p_user_desc_md   = NULL;
    char_md.p_cccd_md        = NULL;
    char_md.p_sccd_md        = NULL;

    ble_uuid.type = p_escs->uuid_type;
    ble_uuid.uuid = BLE_UUID_ESCS_BROADCAST_CAP_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 1;
    attr_md.wr_auth = 0;
    attr_md.vlen    = 1;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    // Eddystone spec requires big endian
    nrf_ble_escs_broadcast_cap_t temp = p_escs_init->p_init_vals->broadcast_cap;
    temp.supp_frame_types = BYTES_SWAP_16BIT(temp.supp_frame_types);

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = NRF_BLE_ESCS_BROADCAST_CAP_LEN;
    attr_char_value.init_offs = 0;
    attr_char_value.p_value   = (uint8_t *)(&temp);
    attr_char_value.max_len   = NRF_BLE_ESCS_BROADCAST_CAP_LEN;

    return sd_ble_gatts_characteristic_add(p_escs->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_escs->broadcast_cap_handles);
}


/**@brief Function for adding active slot characteristic.
 *
 * @param[in] p_escs       Eddystone Configuration Service structure.
 * @param[in] p_escs_init  Information needed to initialize the service.
 *
 * @return NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t active_slot_char_add(nrf_ble_escs_t            * p_escs,
                                     const nrf_ble_escs_init_t * p_escs_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read  = 1;
    char_md.char_props.write = 1;
    char_md.p_char_user_desc = NULL;
    char_md.p_char_pf        = NULL;
    char_md.p_user_desc_md   = NULL;
    char_md.p_cccd_md        = NULL;
    char_md.p_sccd_md        = NULL;

    ble_uuid.type = p_escs->uuid_type;
    ble_uuid.uuid = BLE_UUID_ESCS_ACTIVE_SLOT_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 1;
    attr_md.wr_auth = 1;
    attr_md.vlen    = 0;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = sizeof(nrf_ble_escs_active_slot_t);
    attr_char_value.init_offs = 0;
    attr_char_value.p_value   = (uint8_t *)(&(p_escs_init->p_init_vals->active_slot));
    attr_char_value.max_len   = sizeof(nrf_ble_escs_active_slot_t);

    return sd_ble_gatts_characteristic_add(p_escs->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_escs->active_slot_handles);
}


/**@brief Function for adding advertising interval characteristic.
 *
 * @param[in] p_escs       Eddystone Configuration Service structure.
 * @param[in] p_escs_init  Information needed to initialize the service.
 *
 * @return NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t adv_interval_char_add(nrf_ble_escs_t            * p_escs,
                                      const nrf_ble_escs_init_t * p_escs_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read  = 1;
    char_md.char_props.write = 1;
    char_md.p_char_user_desc = NULL;
    char_md.p_char_pf        = NULL;
    char_md.p_user_desc_md   = NULL;
    char_md.p_cccd_md        = NULL;
    char_md.p_sccd_md        = NULL;

    ble_uuid.type = p_escs->uuid_type;
    ble_uuid.uuid = BLE_UUID_ESCS_ADV_INTERVAL_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 1;
    attr_md.wr_auth = 1;
    attr_md.vlen    = 0;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    nrf_ble_escs_adv_interval_t temp = p_escs_init->p_init_vals->adv_interval;
    temp = BYTES_SWAP_16BIT(temp);

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = sizeof(nrf_ble_escs_adv_interval_t);
    attr_char_value.init_offs = 0;
    attr_char_value.p_value   = (uint8_t *)(&temp);
    attr_char_value.max_len   = sizeof(nrf_ble_escs_adv_interval_t);

    return sd_ble_gatts_characteristic_add(p_escs->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_escs->adv_interval_handles);
}


/**@brief Function for adding radio tx power characteristic.
 *
 * @param[in] p_escs       Eddystone Configuration Service structure.
 * @param[in] p_escs_init  Information needed to initialize the service.
 *
 * @return NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t radio_tx_pwr_char_add(nrf_ble_escs_t            * p_escs,
                                      const nrf_ble_escs_init_t * p_escs_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read  = 1;
    char_md.char_props.write = 1;
    char_md.p_char_user_desc = NULL;
    char_md.p_char_pf        = NULL;
    char_md.p_user_desc_md   = NULL;
    char_md.p_cccd_md        = NULL;
    char_md.p_sccd_md        = NULL;

    ble_uuid.type = p_escs->uuid_type;
    ble_uuid.uuid = BLE_UUID_ESCS_RADIO_TX_PWR_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 1;
    attr_md.wr_auth = 1;
    attr_md.vlen    = 0;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = sizeof(nrf_ble_escs_radio_tx_pwr_t);
    attr_char_value.init_offs = 0;
    attr_char_value.p_value   = (uint8_t *)(&(p_escs_init->p_init_vals->radio_tx_pwr));
    attr_char_value.max_len   = sizeof(nrf_ble_escs_radio_tx_pwr_t);

    return sd_ble_gatts_characteristic_add(p_escs->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_escs->radio_tx_pwr_handles);
}


/**@brief Function for adding radio tx power characteristic.
 *
 * @param[in] p_escs       Eddystone Configuration Service structure.
 * @param[in] p_escs_init  Information needed to initialize the service.
 *
 * @return NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t adv_tx_pwr_char_add(nrf_ble_escs_t            * p_escs,
                                    const nrf_ble_escs_init_t * p_escs_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read  = 1;
    char_md.char_props.write = 1;
    char_md.p_char_user_desc = NULL;
    char_md.p_char_pf        = NULL;
    char_md.p_user_desc_md   = NULL;
    char_md.p_cccd_md        = NULL;
    char_md.p_sccd_md        = NULL;

    ble_uuid.type = p_escs->uuid_type;
    ble_uuid.uuid = BLE_UUID_ESCS_ADV_TX_PWR_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 1;
    attr_md.wr_auth = 1;
    attr_md.vlen    = 0;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = sizeof(nrf_ble_escs_adv_tx_pwr_t);
    attr_char_value.init_offs = 0;
    attr_char_value.p_value   = (uint8_t *)(&(p_escs_init->p_init_vals->adv_tx_pwr));
    attr_char_value.max_len   = sizeof(nrf_ble_escs_adv_tx_pwr_t);

    return sd_ble_gatts_characteristic_add(p_escs->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_escs->adv_tx_pwr_handles);
}


/**@brief Function for adding lock state characteristic.
 *
 * @param[in] p_escs       Eddystone Configuration Service structure.
 * @param[in] p_escs_init  Information needed to initialize the service.
 *
 * @return NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t lock_state_char_add(nrf_ble_escs_t            * p_escs,
                                    const nrf_ble_escs_init_t * p_escs_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read  = 1;
    char_md.char_props.write = 1;
    char_md.p_char_user_desc = NULL;
    char_md.p_char_pf        = NULL;
    char_md.p_user_desc_md   = NULL;
    char_md.p_cccd_md        = NULL;
    char_md.p_sccd_md        = NULL;

    ble_uuid.type = p_escs->uuid_type;
    ble_uuid.uuid = BLE_UUID_ESCS_LOCK_STATE_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 1;
    attr_md.vlen    = 1;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = 1;
    attr_char_value.init_offs = 0;
    attr_char_value.p_value   = (uint8_t *)(&(p_escs_init->p_init_vals->lock_state.read));
    attr_char_value.max_len   = 17;

    return sd_ble_gatts_characteristic_add(p_escs->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_escs->lock_state_handles);
}


/**@brief Function for adding unlock characteristic.
 *
 * @param[in] p_escs       Eddystone Configuration Service structure.
 * @param[in] p_escs_init  Information needed to initialize the service.
 *
 * @return NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t unlock_char_add(nrf_ble_escs_t * p_escs, const nrf_ble_escs_init_t * p_escs_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read  = 1;
    char_md.char_props.write = 1;
    char_md.p_char_user_desc = NULL;
    char_md.p_char_pf        = NULL;
    char_md.p_user_desc_md   = NULL;
    char_md.p_cccd_md        = NULL;
    char_md.p_sccd_md        = NULL;

    ble_uuid.type = p_escs->uuid_type;
    ble_uuid.uuid = BLE_UUID_ESCS_UNLOCK_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 1;
    attr_md.wr_auth = 1;
    attr_md.vlen    = 1;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    uint8_t init_val = 0;

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = 1;
    attr_char_value.init_offs = 0;
    attr_char_value.p_value   = &init_val;
    attr_char_value.max_len   = ESCS_AES_KEY_SIZE;

    return sd_ble_gatts_characteristic_add(p_escs->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_escs->unlock_handles);
}


/**@brief Function for adding public ECDH key characteristic.
 *
 * @param[in] p_escs       Eddystone Configuration Service structure.
 * @param[in] p_escs_init  Information needed to initialize the service.
 *
 * @return NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t pub_ecdh_key_char_add(nrf_ble_escs_t            * p_escs,
                                      const nrf_ble_escs_init_t * p_escs_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read  = 1;
    char_md.p_char_user_desc = NULL;
    char_md.p_char_pf        = NULL;
    char_md.p_user_desc_md   = NULL;
    char_md.p_cccd_md        = NULL;
    char_md.p_sccd_md        = NULL;

    ble_uuid.type = p_escs->uuid_type;
    ble_uuid.uuid = BLE_UUID_ESCS_PUBLIC_ECDH_KEY_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 1;
    attr_md.wr_auth = 0;
    attr_md.vlen    = 1;

    memset(&attr_char_value, 0, sizeof(attr_char_value));
    uint8_t init_val = 0;
    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = 1;
    attr_char_value.init_offs = 0;
    attr_char_value.p_value   = &init_val;
    attr_char_value.max_len   = ESCS_ECDH_KEY_SIZE;

    return sd_ble_gatts_characteristic_add(p_escs->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_escs->pub_ecdh_key_handles);
}


/**@brief Function for adding EID ID key characteristic.
 *
 * @param[in] p_escs       Eddystone Configuration Service structure.
 * @param[in] p_escs_init  Information needed to initialize the service.
 *
 * @return NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t eid_id_key_char_add(nrf_ble_escs_t            * p_escs,
                                    const nrf_ble_escs_init_t * p_escs_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read  = 1;
    char_md.p_char_user_desc = NULL;
    char_md.p_char_pf        = NULL;
    char_md.p_user_desc_md   = NULL;
    char_md.p_cccd_md        = NULL;
    char_md.p_sccd_md        = NULL;

    ble_uuid.type = p_escs->uuid_type;
    ble_uuid.uuid = BLE_UUID_ESCS_EID_ID_KEY_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 1;
    attr_md.wr_auth = 0;
    attr_md.vlen    = 1;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    uint8_t init_val = 0;

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = 1;
    attr_char_value.init_offs = 0;
    attr_char_value.p_value   = &init_val;
    attr_char_value.max_len   = ESCS_AES_KEY_SIZE;

    return sd_ble_gatts_characteristic_add(p_escs->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_escs->eid_id_key_handles);
}


/**@brief Function for adding readwrite ADV slot characteristic.
 *
 * @param[in] p_escs       Eddystone Configuration Service structure.
 * @param[in] p_escs_init  Information needed to initialize the service.
 *
 * @return NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t rw_adv_slot_char_add(nrf_ble_escs_t            * p_escs,
                                     const nrf_ble_escs_init_t * p_escs_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read  = 1;
    char_md.char_props.write = 1;
    char_md.p_char_user_desc = NULL;
    char_md.p_char_pf        = NULL;
    char_md.p_user_desc_md   = NULL;
    char_md.p_cccd_md        = NULL;
    char_md.p_sccd_md        = NULL;

    ble_uuid.type = p_escs->uuid_type;
    ble_uuid.uuid = BLE_UUID_ESCS_RW_ADV_SLOT_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_USER;
    attr_md.rd_auth = 1;
    attr_md.wr_auth = 1;
    attr_md.vlen    = 1;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = 0;
    attr_char_value.init_offs = 0;
    attr_char_value.p_value   = m_data_buf;
    attr_char_value.max_len   = ESCS_ADV_SLOT_CHAR_LENGTH_MAX;

    return sd_ble_gatts_characteristic_add(p_escs->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_escs->rw_adv_slot_handles);
}


/**@brief Function for adding factory reset characteristic.
 *
 * @param[in] p_escs       Eddystone Configuration Service structure.
 * @param[in] p_escs_init  Information needed to initialize the service.
 *
 * @return NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t factory_reset_char_add(nrf_ble_escs_t            * p_escs,
                                       const nrf_ble_escs_init_t * p_escs_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.write = 1;
    char_md.p_char_user_desc = NULL;
    char_md.p_char_pf        = NULL;
    char_md.p_user_desc_md   = NULL;
    char_md.p_cccd_md        = NULL;
    char_md.p_sccd_md        = NULL;

    ble_uuid.type = p_escs->uuid_type;
    ble_uuid.uuid = BLE_UUID_ESCS_FACTORY_RESET_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 1;
    attr_md.vlen    = 0;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    nrf_ble_escs_factory_reset_t temp = p_escs_init->p_init_vals->factory_reset;

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = sizeof(temp);
    attr_char_value.init_offs = 0;
    attr_char_value.p_value   = (uint8_t *)(&(temp));
    attr_char_value.max_len   = sizeof(temp);

    return sd_ble_gatts_characteristic_add(p_escs->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_escs->factory_reset_handles);
}


/**@brief Function for adding remain connectable characteristic.
 *
 * @param[in] p_escs       Eddystone Configuration Service structure.
 * @param[in] p_escs_init  Information needed to initialize the service.
 *
 * @return NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t remain_connectable_char_add(nrf_ble_escs_t            * p_escs,
                                       const nrf_ble_escs_init_t * p_escs_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.write = 1;
    char_md.char_props.read  = 1;
    char_md.p_char_user_desc = NULL;
    char_md.p_char_pf        = NULL;
    char_md.p_user_desc_md   = NULL;
    char_md.p_cccd_md        = NULL;
    char_md.p_sccd_md        = NULL;

    ble_uuid.type = p_escs->uuid_type;
    ble_uuid.uuid = BLE_UUID_ESCS_REMAIN_CONNECTABLE_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 1;
    attr_md.wr_auth = 1;
    attr_md.vlen    = 0;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    uint8_t temp = p_escs_init->p_init_vals->remain_connectable.r_is_non_connectable_supported;

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = sizeof(temp);
    attr_char_value.init_offs = 0;
    attr_char_value.p_value   = (uint8_t *)(&(temp));
    attr_char_value.max_len   = sizeof(temp);

    return sd_ble_gatts_characteristic_add(p_escs->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_escs->remain_connectable_handles);
}


static ret_code_t on_rw_authorize_req(nrf_ble_escs_t * p_escs, ble_evt_t * p_ble_evt)
{
    if (p_ble_evt->evt.gatts_evt.params.authorize_request.type ==
        BLE_GATTS_AUTHORIZE_TYPE_READ)
    {
        on_read(p_escs, p_ble_evt);
    }
    else if (p_ble_evt->evt.gatts_evt.params.authorize_request.type ==
             BLE_GATTS_AUTHORIZE_TYPE_WRITE)
    {
        on_write(p_escs, p_ble_evt);
    }
    else
    {
        return NRF_ERROR_INVALID_STATE;
    }
    
    return NRF_SUCCESS;
}



ret_code_t nrf_ble_escs_on_ble_evt(nrf_ble_escs_t * p_escs, ble_evt_t * p_ble_evt)
{
    ret_code_t err_code;

    VERIFY_PARAM_NOT_NULL(p_escs);
    VERIFY_PARAM_NOT_NULL(p_ble_evt);

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            on_connect(p_escs, p_ble_evt);

            break;

        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnect(p_escs, p_ble_evt);
            break;

        case BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST:
            err_code = on_rw_authorize_req(p_escs, p_ble_evt);
            VERIFY_SUCCESS(err_code);
            break;

        // BLE_EVT_USER_MEM_REQUEST & BLE_EVT_USER_MEM_RELEASE are for long writes to the RW ADV slot characteristic
        case BLE_EVT_USER_MEM_REQUEST:
            err_code = sd_ble_user_mem_reply(p_escs->conn_handle, &m_eid_mem_block);
            VERIFY_SUCCESS(err_code);
            DEBUG_PRINTF(0, "USER_MEM_REQUEST: error: %d \r\n", err_code);
            break;

        case BLE_EVT_USER_MEM_RELEASE:
            DEBUG_PRINTF(0, "USER_MEM_RELEASE\r\n", 0);
            break;

        default:
            // No implementation needed.
            break;
    }
    
    return NRF_SUCCESS;
}


ret_code_t nrf_ble_escs_init(nrf_ble_escs_t * p_escs, const nrf_ble_escs_init_t * p_escs_init)
{
    uint32_t      err_code;
    ble_uuid_t    ble_uuid;
    ble_uuid128_t ecs_base_uuid = ESCS_BASE_UUID;

    VERIFY_PARAM_NOT_NULL(p_escs);
    VERIFY_PARAM_NOT_NULL(p_escs_init);

    // Initialize the service structure.
    p_escs->conn_handle       = BLE_CONN_HANDLE_INVALID;
    p_escs->write_evt_handler = p_escs_init->write_evt_handler;
    p_escs->read_evt_handler  = p_escs_init->read_evt_handler;

    // Add a custom base UUID.
    err_code = sd_ble_uuid_vs_add(&ecs_base_uuid, &p_escs->uuid_type);
    VERIFY_SUCCESS(err_code);

    ble_uuid.type = p_escs->uuid_type;
    ble_uuid.uuid = BLE_UUID_ESCS_SERVICE;

    // Add the service.
    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                        &ble_uuid,
                                        &p_escs->service_handle);
    VERIFY_SUCCESS(err_code);

    // Adding chracteristics
    err_code = broadcast_cap_char_add(p_escs, p_escs_init);
    VERIFY_SUCCESS(err_code);

    err_code = active_slot_char_add(p_escs, p_escs_init);
    VERIFY_SUCCESS(err_code);

    err_code = adv_interval_char_add(p_escs, p_escs_init);
    VERIFY_SUCCESS(err_code);

    err_code = radio_tx_pwr_char_add(p_escs, p_escs_init);
    VERIFY_SUCCESS(err_code);

    err_code = adv_tx_pwr_char_add(p_escs, p_escs_init);
    VERIFY_SUCCESS(err_code);

    err_code = lock_state_char_add(p_escs, p_escs_init);
    VERIFY_SUCCESS(err_code);

    err_code = unlock_char_add(p_escs, p_escs_init);
    VERIFY_SUCCESS(err_code);

    err_code = pub_ecdh_key_char_add(p_escs, p_escs_init);
    VERIFY_SUCCESS(err_code);

    err_code = eid_id_key_char_add(p_escs, p_escs_init);
    VERIFY_SUCCESS(err_code);

    err_code = rw_adv_slot_char_add(p_escs, p_escs_init);
    VERIFY_SUCCESS(err_code);

    err_code = factory_reset_char_add(p_escs, p_escs_init);
    VERIFY_SUCCESS(err_code);

    err_code = remain_connectable_char_add(p_escs, p_escs_init);
    VERIFY_SUCCESS(err_code);

    return NRF_SUCCESS;
}


ret_code_t nrf_ble_escs_get_lock_state(const nrf_ble_escs_t * p_escs, nrf_ble_escs_lock_state_read_t * p_lock_state)
{
    ret_code_t        err_code;
    uint8_t           lock_state;
    ble_gatts_value_t value = {.len = sizeof(lock_state), .offset = 0, .p_value = &lock_state};

    VERIFY_PARAM_NOT_NULL(p_escs);
    
    err_code = sd_ble_gatts_value_get(p_escs->conn_handle,
                                      p_escs->lock_state_handles.value_handle,
                                      &value);
    VERIFY_SUCCESS(err_code);

    *p_lock_state = (nrf_ble_escs_lock_state_read_t)lock_state;
    
    return NRF_SUCCESS;
}


ret_code_t nrf_ble_escs_active_slot_get(const nrf_ble_escs_t * p_escs, uint8_t * p_active_slot)
{
    ret_code_t        err_code;
    uint8_t           active_slot;
    ble_gatts_value_t value = {.len = sizeof(uint8_t), .offset = 0, .p_value = &active_slot};

    VERIFY_PARAM_NOT_NULL(p_escs);
    
    err_code = sd_ble_gatts_value_get(p_escs->conn_handle,
                                      p_escs->active_slot_handles.value_handle,
                                      &value);
    VERIFY_SUCCESS(err_code);
    // boundary checking
    if (active_slot > APP_MAX_ADV_SLOTS)
    {
        active_slot = APP_MAX_ADV_SLOTS;
    }
    
    *p_active_slot = active_slot;
    
    return NRF_SUCCESS;
}


