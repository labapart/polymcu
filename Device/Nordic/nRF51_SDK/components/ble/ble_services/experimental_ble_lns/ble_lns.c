/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
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

#include "ble_lns.h"
#include <string.h>
#include "nordic_common.h"
#include "ble_l2cap.h"
#include "ble_srv_common.h"
#include "app_util.h"
#include "ble_gatt.h"

// Location and Speed flag bits
#define LOC_SPEED_FLAG_INSTANT_SPEED_PRESENT             (0x01 << 0)         /**< Instantaneous Speed Present bit. */
#define LOC_SPEED_FLAG_TOTAL_DISTANCE_PRESENT            (0x01 << 1)         /**< Total Distance Present bit. */
#define LOC_SPEED_FLAG_LOCATION_PRESENT                  (0x01 << 2)         /**< Location Present bit. */
#define LOC_SPEED_FLAG_ELEVATION_PRESENT                 (0x01 << 3)         /**< Elevation Present bit. */
#define LOC_SPEED_FLAG_HEADING_PRESENT                   (0x01 << 4)         /**< Heading Present bit. */
#define LOC_SPEED_FLAG_ROLLING_TIME_PRESENT              (0x01 << 5)         /**< Rolling Time Present bit. */
#define LOC_SPEED_FLAG_UTC_TIME_PRESENT                  (0x01 << 6)         /**< UTC Time Present bit. */
#define LOC_SPEED_FLAG_POSITION_STATUS                   (0x03 << 7)         /**< Position Status bits(2). */
#define LOC_SPEED_FLAG_SPEED_AND_DIST_FORMAT             (0x01 << 9)         /**< Speed and Distance Format. */
#define LOC_SPEED_FLAG_ELEVATION_SOURCE                  (0x03 << 10)        /**< Elevation Source bits(2). */
#define LOC_SPEED_FLAG_HEADING_SOURCE                    (0x01 << 12)        /**< Heading Source. */
                                                         
// Position Quality flag bits                            
#define POS_QUAL_FLAG_NUM_SATS_IN_SOLUTION_PRESENT       (0x01 << 0)         /**< Number of Satellites in Solution Present bit. */
#define POS_QUAL_FLAG_NUM_SATS_IN_VIEW_PRESENT           (0x01 << 1)         /**< Number of Satellites in View Present bit. */
#define POS_QUAL_FLAG_TIME_TO_FIRST_FIX_PRESESNT         (0x01 << 2)         /**< Time to First Fix Present bit. */
#define POS_QUAL_FLAG_EHPE_PRESENT                       (0x01 << 3)         /**< EHPE Present bit. */
#define POS_QUAL_FLAG_EVPE_PRESENT                       (0x01 << 4)         /**< EVPE Present bit. */
#define POS_QUAL_FLAG_HDOP_PRESENT                       (0x01 << 5)         /**< HDOP Present bit. */
#define POS_QUAL_FLAG_VDOP_PRESENT                       (0x01 << 6)         /**< VDOP Present bit. */
#define POS_QUAL_FLAG_POSITION_STATUS                    (0x03 << 7)         /**< Position Status bits(2). */
                                                         
// Navigation flag bits                                  
#define NAV_FLAG_REMAINING_DIST_PRESENT                  (0x01 << 0)         /**< Remaining Distance Present bit. */
#define NAV_FLAG_REAMINGING_VERT_DIST_PRESESNT           (0x01 << 1)         /**< Remaining Vertical Distance Present bit . */
#define NAV_FLAG_ETA_PRESENT                             (0x01 << 2)         /**< Estimated Time of Arrival Present bit. */
#define NAV_FLAG_POSITION_STATUS                         (0x03 << 3)         /**< Position Status bits(2). */
#define NAV_FLAG_HEADING_SOURCE                          (0x01 << 5)         /**< Heading Source bit. */
#define NAV_FLAG_NAVIGATION_INDICATOR_TYPE               (0x01 << 6)         /**< Navigation Indicator Type bit. */
#define NAV_FLAG_WAYPOINT_REACHED                        (0x01 << 7)         /**< Waypoint Reached bit. */
#define NAV_FLAG_DESTINATION_REACHED                     (0x01 << 8)         /**< Destination Reached bit. */
                                                         
// Feature Mask bits                                     
#define FEATURE_MASK_INSTANTANEOUS_SPEED                 (0x01 << 0)         /**< Instantaneous Speed mask bit. */
#define FEATURE_MASK_TOTAL_DISTANCE                      (0x01 << 1)         /**< Total Distance mask bit. */
#define FEATURE_MASK_LOCATION                            (0x01 << 2)         /**< Location mask bit. */
#define FEATURE_MASK_ELEVATION                           (0x01 << 3)         /**< Elevation mask bit. */
#define FEATURE_MASK_HEADING                             (0x01 << 4)         /**< Heading mask bit. */
#define FEATURE_MASK_ROLLING_TIME                        (0x01 << 5)         /**< Rolling Time mask bit. */
#define FEATURE_MASK_UTC_TIME                            (0x01 << 6)         /**< UTC Time mask bit. */
                                                         
// Data Control point parameter type lengths.
#define INT8_LEN   1
#define INT16_LEN  2
#define INT24_LEN  3
#define INT32_LEN  4

#define OPCODE_LENGTH                                   1  /**< Length of opcode inside Location and Navigation Measurement packet. */
#define HANDLE_LENGTH                                   2  /**< Length of handle inside Location and Navigation Measurement packet. */
#define BLE_LNS_NAV_MAX_LEN                             19 /**< The length of a navigation notification when all features are enabled. See @ref ble_lns_navigation_t to see what this represents, or check https://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.navigation.xml. */ 


/**@brief Connect event handler.
 *
 * @param[in]   p_lns       Location and Navigation Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_connect(ble_lns_t * p_lns, ble_evt_t * p_ble_evt)
{
    // Clear previous mask. No features are masked away.
    memset(&p_lns->mask, 0, sizeof (p_lns->mask));
    p_lns->control_point_procedure_status = BLE_LNS_CTRLPT_STATE_NO_PROC_IN_PROGRESS;
    p_lns->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
}

/**@brief Disconnect event handler.
 *
 * @param[in]   p_lns       Location and Navigation Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_disconnect(ble_lns_t * p_lns, ble_evt_t * p_ble_evt)
{
    UNUSED_PARAMETER(p_ble_evt);
    p_lns->conn_handle = BLE_CONN_HANDLE_INVALID;
    p_lns->control_point_procedure_status = BLE_LNS_CTRLPT_STATE_NO_PROC_IN_PROGRESS;
}

/**@brief Handle write events to the Location and Speed cccd.
 *
 * @param[in]   p_lns       Location and Navigation Service structure.
 * @param[in]   p_evt_write Write event received from the BLE stack.
 */
static void on_loc_speed_cccd_write(ble_lns_t * p_lns, ble_gatts_evt_write_t * p_evt_write)
{
    if (p_evt_write->len == BLE_CCCD_VALUE_LEN)
    {
        // CCCD written, update notification state
        memcpy(p_lns->loc_speed_cccd_handle_status, p_evt_write->data, INT16_LEN); //lint !e420 "Apparent access beyond array"
        if (p_lns->evt_handler != NULL)
        {
            ble_lns_evt_t evt;
            
            if (ble_srv_is_notification_enabled(p_evt_write->data))
            {
                evt.evt_type = BLE_LNS_LOC_SPEED_EVT_NOTIFICATION_ENABLED;
            }
            else
            {
                evt.evt_type = BLE_LNS_LOC_SPEED_EVT_NOTIFICATION_DISABLED;
            }
            
            p_lns->evt_handler(p_lns, &evt);
        }
    }
}

/**@brief Handle write events to the control point cccd.
 *
 * @param[in]   p_lns       Location and Navigation Service structure.
 * @param[in]   p_evt_write Write event received from the BLE stack.
 */
static void on_control_point_cccd_write(ble_lns_t * p_lns, ble_gatts_evt_write_t * p_evt_write)
{
    if (p_evt_write->len == BLE_CCCD_VALUE_LEN)
    {
        // CCCD written, update indications state
        memcpy(p_lns->ctrl_pnt_cccd_handle_status, p_evt_write->data, INT16_LEN); //lint !e420 "Apparent access beyond array"
        if (p_lns->evt_handler != NULL)
        {
            ble_lns_evt_t evt;
            
            if (ble_srv_is_indication_enabled(p_evt_write->data))
            {
                evt.evt_type = BLE_LNS_CONTROL_POINT_EVT_INDICATION_ENABLED;
            }
            else
            {
                evt.evt_type = BLE_LNS_CONTROL_POINT_EVT_INDICATION_DISABLED;
            }
            
            p_lns->evt_handler(p_lns, &evt);
        }
    }
}

/**@brief Handle write events to the navigation cccd.
 *
 * @param[in]   p_lns       Location and Navigation Service structure.
 * @param[in]   p_evt_write Write event received from the BLE stack.
 */
static void on_navigation_cccd_write(ble_lns_t * p_lns, ble_gatts_evt_write_t * p_evt_write)
{
    if (p_evt_write->len == BLE_CCCD_VALUE_LEN)
    {
        // CCCD written, update notification state
        memcpy(p_lns->navigation_cccd_handle_status, p_evt_write->data, INT16_LEN); //lint !e420 "Apparent access beyond array"
        if (p_lns->evt_handler != NULL)
        {
            ble_lns_evt_t evt;
            
            if (ble_srv_is_notification_enabled(p_evt_write->data))
            {
                evt.evt_type = BLE_LNS_NAVIGATION_EVT_NOTIFICATION_ENABLED;
            }
            else
            {
                evt.evt_type = BLE_LNS_NAVIGATION_EVT_NOTIFICATION_DISABLED;
            }
            
            p_lns->evt_handler(p_lns, &evt);
        }
    }
}

static void ctrl_point_response_send(ble_lns_t * p_lns)
{
    // Send indication
    uint16_t               hvx_len;
    uint8_t                hvx_data[MAX_CTRL_POINT_RESP_PARAM_LEN];
    ble_gatts_hvx_params_t hvx_params;
    uint32_t               err_code;
    
    memset(&hvx_params, 0, sizeof(hvx_params));

    hvx_len = 3 + p_lns->pending_ctrlpt_response.response_param_len;
    hvx_data[0] = OP_CODE_RESPONSE_CODE;
    hvx_data[1] = p_lns->pending_ctrlpt_response.op_code;
    hvx_data[2] = p_lns->pending_ctrlpt_response.response_code;

    memcpy(&hvx_data[3], &p_lns->pending_ctrlpt_response.response_param[0], p_lns->pending_ctrlpt_response.response_param_len);
    
    hvx_params.handle   = p_lns->ctrl_point_handles.value_handle;
    hvx_params.type     = BLE_GATT_HVX_INDICATION;
    hvx_params.offset   = 0;
    hvx_params.p_len    = &hvx_len;
    hvx_params.p_data   = hvx_data;

    err_code = sd_ble_gatts_hvx(p_lns->conn_handle, &hvx_params);
    
    // Error handling
    if ((err_code == NRF_SUCCESS) && (hvx_len != p_lns->pending_ctrlpt_response.response_param_len + 3))
    {
        err_code = NRF_ERROR_DATA_SIZE;
    }

    switch (err_code)
    {
        case NRF_SUCCESS:
            p_lns->control_point_procedure_status = BLE_LNS_CTRLPT_STATE_IND_CONFIRM_PENDING;
            // Wait for HVC event
            break;

        case BLE_ERROR_NO_TX_BUFFERS:
            // Wait for TX_COMPLETE event to retry transmission
            p_lns->control_point_procedure_status = BLE_LNS_CTRLPT_STATE_INDICATION_PENDING;
            break;

        default:
            p_lns->control_point_procedure_status = BLE_LNS_CTRLPT_STATE_INDICATION_PENDING;
            // error
            if (p_lns->error_handler != NULL)
            {
                p_lns->error_handler(err_code);                
            }
            break;
    }
}

/**@brief Event handler for control point write.
 *
 * @param[in]   p_lns     Location and Navigation Service structure.
 * @param[in]   p_ble_evt Event received from the BLE stack.
 */
static void on_set_cumulative_value(ble_lns_t * p_lns, ble_gatts_evt_write_t * p_evt_write)
{
    //lint -save -e415 -e416 "Likely access of out-of-bounds pointer"
    ble_lns_evt_t lns_evt;
    
    if ( !(p_lns->available_features & BLE_LNS_FEATURE_TOTAL_DISTANCE_SUPPORTED) )
    {
        p_lns->pending_ctrlpt_response.response_code = CONTROL_POINT_RESP_OP_CODE_NOT_SUPPORTED;
        return;
    }
    
    if (p_evt_write->len != OPCODE_LENGTH + INT24_LEN)
    {
        p_lns->pending_ctrlpt_response.response_code = CONTROL_POINT_RESP_INVALID_PARAMETER;
        return;
    }
    
    p_lns->pending_ctrlpt_response.response_code = CONTROL_POINT_RESP_SUCCESS;
    
    p_lns->p_location_speed->total_distance =   (p_evt_write->data[1]<<0) 
                                              | (p_evt_write->data[2]<<8) 
                                              | (p_evt_write->data[3]<<16); 
    if (p_lns->evt_handler != NULL)
    {
        lns_evt.evt_type = BLE_LNS_LOC_SPEED_EVT_TOTAL_DISTANCE_SET;
        lns_evt.params.total_distance = p_lns->p_location_speed->total_distance;
        p_lns->evt_handler(p_lns, &lns_evt);
    }
    //lint -restore
}

/**@brief Event handler for control point write.
 *
 * @param[in]   p_lns     Location and Navigation Service structure.
 * @param[in]   p_ble_evt Event received from the BLE stack.
 */
static void on_mask_loc_speed_content(ble_lns_t * p_lns, ble_gatts_evt_write_t * p_evt_write)
{
    //lint -save -e415 -e416 "Likely access of out-of-bounds pointer"
    ble_lns_evt_t lns_evt;
    
    if ( !(p_lns->available_features & BLE_LNS_FEATURE_LOC_AND_SPEED_CONTENT_MASKING_SUPPORTED) )
    {
        p_lns->pending_ctrlpt_response.response_code = CONTROL_POINT_RESP_OP_CODE_NOT_SUPPORTED;
        return;
    }
    if (p_evt_write->len != OPCODE_LENGTH + INT16_LEN)
    {
        p_lns->pending_ctrlpt_response.response_code = CONTROL_POINT_RESP_INVALID_PARAMETER;
        return;
    }
    
    p_lns->pending_ctrlpt_response.response_code = CONTROL_POINT_RESP_SUCCESS;
    uint16_t rcvd_mask = uint16_decode(&p_evt_write->data[1]);
    if(rcvd_mask > 0x7f)
    {
        p_lns->pending_ctrlpt_response.response_code = CONTROL_POINT_RESP_INVALID_PARAMETER;
        return;
    }
    
    p_lns->mask.instantaneous_speed = !(!(rcvd_mask & FEATURE_MASK_INSTANTANEOUS_SPEED));
    p_lns->mask.total_distance      = !(!(rcvd_mask & FEATURE_MASK_TOTAL_DISTANCE     ));
    p_lns->mask.location            = !(!(rcvd_mask & FEATURE_MASK_LOCATION           ));
    p_lns->mask.elevation           = !(!(rcvd_mask & FEATURE_MASK_ELEVATION          ));
    p_lns->mask.heading             = !(!(rcvd_mask & FEATURE_MASK_HEADING            ));
    p_lns->mask.rolling_time        = !(!(rcvd_mask & FEATURE_MASK_ROLLING_TIME       ));
    p_lns->mask.utc_time            = !(!(rcvd_mask & FEATURE_MASK_UTC_TIME           ));

    
    if (p_lns->evt_handler != NULL)
    {
        lns_evt.evt_type = BLE_LNS_LOC_SPEED_EVT_MASK_SET;   
        lns_evt.params.mask = p_lns->mask;
        p_lns->evt_handler(p_lns, &lns_evt);
    }
    //lint -restore
}

/**@brief Event handler for control point write.
 *
 * @param[in]   p_lns     Location and Navigation Service structure.
 * @param[in]   p_ble_evt Event received from the BLE stack.
 */
static void on_nav_control(ble_lns_t * p_lns, ble_gatts_evt_write_t * p_evt_write)
{
    //lint -save -e415 -e416 "Likely access of out-of-bounds pointer"
    ble_lns_evt_t lns_evt;

    if ( !(p_lns->is_navigation_present) )
    {   
        p_lns->pending_ctrlpt_response.response_code = CONTROL_POINT_RESP_OP_CODE_NOT_SUPPORTED;
        return;
    }

    if(p_evt_write->len != OPCODE_LENGTH + INT8_LEN)
    {
        p_lns->pending_ctrlpt_response.response_code = CONTROL_POINT_RESP_INVALID_PARAMETER;
        return;
    }
    
    if (p_evt_write->data[1] > 0x05)
    {
        p_lns->pending_ctrlpt_response.response_code = CONTROL_POINT_RESP_INVALID_PARAMETER;
        return;
    }
    
    p_lns->pending_ctrlpt_response.response_code = CONTROL_POINT_RESP_SUCCESS;
    if (   p_evt_write->data[1] == BLE_LNS_NAVIGATION_START
        || p_evt_write->data[1] == BLE_LNS_NAVIGATION_CONTINUE
        || p_evt_write->data[1] == BLE_LNS_NAVIGATION_NEAREST
       )
    {
        p_lns->is_navigation_running = true;
    }
    else if (   p_evt_write->data[1] == BLE_LNS_NAVIGATION_STOP
             || p_evt_write->data[1] == BLE_LNS_NAVIGATION_PAUSE
            )
    {
        p_lns->is_navigation_running = false;
    }
    
    if (p_lns->evt_handler != NULL)
    {
        lns_evt.evt_type = BLE_LNS_NAVIGATION_EVT_COMMAND;   
        lns_evt.params.navigation_command = (ble_lns_navigation_command_t)p_evt_write->data[1];
        p_lns->evt_handler(p_lns, &lns_evt);
    }
    //lint -restore
}

/**@brief Event handler for control point write.
 *
 * @param[in]   p_lns     Location and Navigation Service structure.
 * @param[in]   p_ble_evt Event received from the BLE stack.
 */
static void on_req_num_routes(ble_lns_t * p_lns, ble_gatts_evt_write_t * p_evt_write)
{
    //lint -save -e415 -e416 "Likely access of out-of-bounds pointer"
    if ( !(p_lns->is_navigation_present) )
    {
        p_lns->pending_ctrlpt_response.response_code = CONTROL_POINT_RESP_OP_CODE_NOT_SUPPORTED;
        return;
    }
    if(p_evt_write->len == OPCODE_LENGTH)
    {
        p_lns->pending_ctrlpt_response.response_param_len = 
            uint16_encode(p_lns->number_of_routes, &p_lns->pending_ctrlpt_response.response_param[0]);
        p_lns->pending_ctrlpt_response.response_code = CONTROL_POINT_RESP_SUCCESS;
    }
    else
    {
        p_lns->pending_ctrlpt_response.response_code = CONTROL_POINT_RESP_INVALID_PARAMETER;
    }
    //lint -restore    
}

/**@brief Event handler for control point write.
 *
 * @param[in]   p_lns     Location and Navigation Service structure.
 * @param[in]   p_ble_evt Event received from the BLE stack.
 */
static void on_req_name_of_route(ble_lns_t * p_lns, ble_gatts_evt_write_t * p_evt_write)
{
    //lint -save -e415 -e416 "Likely access of out-of-bounds pointer"
    if ( !(p_lns->is_navigation_present) )
    {
        p_lns->pending_ctrlpt_response.response_code = CONTROL_POINT_RESP_OP_CODE_NOT_SUPPORTED;
        return;
    }
    
    if(p_evt_write->len != OPCODE_LENGTH + INT16_LEN)
    {
        p_lns->pending_ctrlpt_response.response_code = CONTROL_POINT_RESP_INVALID_PARAMETER;
        return;
    }
    
    uint16_t route_num = uint16_decode(&p_evt_write->data[1]);
    if (route_num < p_lns->number_of_routes)
    {
        memcpy(&p_lns->pending_ctrlpt_response.response_param[0], p_lns->routes[route_num].route_name, BLE_LNS_MAX_ROUTE_NAME_LEN);
        p_lns->pending_ctrlpt_response.response_param_len = BLE_LNS_MAX_ROUTE_NAME_LEN;
        p_lns->pending_ctrlpt_response.response_code = CONTROL_POINT_RESP_SUCCESS;
    }
    else
    {
        p_lns->pending_ctrlpt_response.response_code = CONTROL_POINT_RESP_INVALID_PARAMETER;
    }
    //lint -restore
}

/**@brief Event handler for control point write.
 *
 * @param[in]   p_lns     Location and Navigation Service structure.
 * @param[in]   p_ble_evt Event received from the BLE stack.
 */
static void on_select_route(ble_lns_t * p_lns, ble_gatts_evt_write_t * p_evt_write)
{
    //lint -save -e415 -e416 "Likely access of out-of-bounds pointer"
    ble_lns_evt_t lns_evt;
    
    if ( !(p_lns->is_navigation_present))
    {
        p_lns->pending_ctrlpt_response.response_code = CONTROL_POINT_RESP_OP_CODE_NOT_SUPPORTED;
        return;
    }
    
    if(p_evt_write->len != OPCODE_LENGTH + INT16_LEN)
    {
        p_lns->pending_ctrlpt_response.response_code = CONTROL_POINT_RESP_INVALID_PARAMETER;
        return;
    }
    
    uint16_t route_num = uint16_decode(&p_evt_write->data[1]);
    if (route_num < p_lns->number_of_routes)
    {
        p_lns->selected_route = route_num;
        p_lns->pending_ctrlpt_response.response_code = CONTROL_POINT_RESP_SUCCESS;
        if (p_lns->evt_handler != NULL)
        {
            lns_evt.evt_type = BLE_LNS_NAVIGATION_EVT_ROUTE_SELECTED;
            lns_evt.params.selected_route = p_lns->selected_route;
            p_lns->evt_handler(p_lns, &lns_evt);
        }                                
    }
    else
    {
        p_lns->pending_ctrlpt_response.response_code = CONTROL_POINT_RESP_INVALID_PARAMETER;
    }  
    //lint -restore    
}

/**@brief Event handler for control point write.
 *
 * @param[in]   p_lns     Location and Navigation Service structure.
 * @param[in]   p_ble_evt Event received from the BLE stack.
 */
static void on_set_fix_rate(ble_lns_t * p_lns, ble_gatts_evt_write_t * p_evt_write)
{
    //lint -save -e415 -e416 "Likely access of out-of-bounds pointer"
    ble_lns_evt_t lns_evt;
    
    if ( !(p_lns->available_features & BLE_LNS_FEATURE_FIX_RATE_SETTING_SUPPORTED) )
    {
        p_lns->pending_ctrlpt_response.response_code = CONTROL_POINT_RESP_OP_CODE_NOT_SUPPORTED;
        return;
    }

    if (p_evt_write->len != OPCODE_LENGTH + INT8_LEN)
    {
        p_lns->pending_ctrlpt_response.response_code = CONTROL_POINT_RESP_INVALID_PARAMETER;
        return;
    }

    p_lns->pending_ctrlpt_response.response_code = CONTROL_POINT_RESP_SUCCESS;
    p_lns->fix_rate = p_evt_write->data[1];
    if (p_lns->evt_handler != NULL)
    {
        lns_evt.evt_type = BLE_LNS_POS_QUAL_EVT_FIX_RATE_SET;
        lns_evt.params.fix_rate = p_lns->fix_rate;
        p_lns->evt_handler(p_lns, &lns_evt);
    }
    //lint -restore    
}

/**@brief Event handler for control point write.
 *
 * @param[in]   p_lns     Location and Navigation Service structure.
 * @param[in]   p_ble_evt Event received from the BLE stack.
 */
static void on_set_elevation(ble_lns_t * p_lns, ble_gatts_evt_write_t * p_evt_write)
{
    //lint -save -e415 -e416 "Likely access of out-of-bounds pointer"
    ble_lns_evt_t lns_evt;
    
    if ( !(p_lns->available_features & BLE_LNS_FEATURE_ELEVATION_SETTING_SUPPORTED) )
    {
        p_lns->pending_ctrlpt_response.response_code = CONTROL_POINT_RESP_OP_CODE_NOT_SUPPORTED;
        return;
    }
    
    if (p_evt_write->len != OPCODE_LENGTH + INT24_LEN)
    {
        p_lns->pending_ctrlpt_response.response_code = CONTROL_POINT_RESP_INVALID_PARAMETER;
        return;
    }
    
    p_lns->pending_ctrlpt_response.response_code = CONTROL_POINT_RESP_SUCCESS;
    
    p_lns->p_location_speed->elevation =   (p_evt_write->data[1]<<0) 
                                         | (p_evt_write->data[2]<<8) 
                                         | (p_evt_write->data[3]<<16); 

    if (p_lns->evt_handler != NULL)
    {
        lns_evt.evt_type = BLE_LNS_LOC_SPEED_EVT_ELEVATION_SET;
        lns_evt.params.elevation = p_lns->p_location_speed->elevation;
        p_lns->evt_handler(p_lns, &lns_evt);
    }
    //lint -restore
}


/**@brief Handle write events to the Location and Navigation Service Control Point characteristic.
 *
 * @param[in]   p_lns         Location and Navigation Service structure.
 * @param[in]   p_evt_write   Write event received from the BLE stack.
 */
static void on_ctrl_point_write(ble_lns_t * p_lns, ble_gatts_evt_write_t * p_evt_write)
{
    uint32_t      err_code;
              
    p_lns->pending_ctrlpt_response.response_param_len = 0;

    ble_gatts_rw_authorize_reply_params_t write_authorize_reply;
    write_authorize_reply.type = BLE_GATTS_AUTHORIZE_TYPE_WRITE;

    if (ble_srv_is_indication_enabled(p_lns->ctrl_pnt_cccd_handle_status))
    {
        if (p_lns->control_point_procedure_status == BLE_LNS_CTRLPT_STATE_NO_PROC_IN_PROGRESS)
        {
            write_authorize_reply.params.write.gatt_status = BLE_GATT_STATUS_SUCCESS;
            
            // if the op code is navigation control, its cccd must be checked
            if (p_evt_write->len > 0 && p_lns->is_navigation_present)
            {
                if (   p_evt_write->data[0] == OP_CODE_NAV_CONTROL
                    || p_evt_write->data[0] == OP_CODE_REQ_NAME_OF_ROUTE
                    || p_evt_write->data[0] == OP_CODE_REQ_NUM_ROUTES)
                {
                    if (ble_srv_is_notification_enabled(p_lns->navigation_cccd_handle_status) == false)
                    {
                        write_authorize_reply.params.write.gatt_status = CONTROL_POINT_RESP_CCCD_CONFIG_IMPROPER;           
                    }
                }
            } 
        }
        else 
        {
            write_authorize_reply.params.write.gatt_status = CONTROL_POINT_RESP_PROC_ALR_IN_PROG;         
        }      
    }
    else 
    {
        write_authorize_reply.params.write.gatt_status =
            CONTROL_POINT_RESP_CCCD_CONFIG_IMPROPER;  
    }

    // reply to the write authorization
    do {
        err_code = sd_ble_gatts_rw_authorize_reply(p_lns->conn_handle, &write_authorize_reply); 
        if (err_code != NRF_SUCCESS && err_code != NRF_ERROR_BUSY)
        {
            if (p_lns->error_handler != NULL)
            {
                p_lns->error_handler(err_code);
            }
        }
    } while (err_code == NRF_ERROR_BUSY);

    
    if (write_authorize_reply.params.write.gatt_status != BLE_GATT_STATUS_SUCCESS)
    {
        return;
    }
    
    // Start executing the control point write action
    p_lns->control_point_procedure_status = BLE_LNS_CTRLPT_STATE_INDICATION_PENDING;
    if (p_evt_write->len > 0)
    {
        p_lns->pending_ctrlpt_response.op_code = p_evt_write->data[0];
        switch (p_lns->pending_ctrlpt_response.op_code)
        {
            case OP_CODE_SET_CUMULATIVE_VALUE:
                on_set_cumulative_value(p_lns, p_evt_write);
                break;

            case OP_CODE_MASK_LOC_SPEED_CONTENT:
                on_mask_loc_speed_content(p_lns, p_evt_write);                           
                break;
            
            case OP_CODE_NAV_CONTROL:
                on_nav_control(p_lns, p_evt_write);
                break;

            case OP_CODE_REQ_NUM_ROUTES:
                on_req_num_routes(p_lns, p_evt_write);
                break;

            case OP_CODE_REQ_NAME_OF_ROUTE:
                on_req_name_of_route(p_lns, p_evt_write);
                break;

            case OP_CODE_SELECT_ROUTE:
                on_select_route(p_lns, p_evt_write);
                break;

            case OP_CODE_SET_FIX_RATE:
                on_set_fix_rate(p_lns, p_evt_write);           
                break;

            case OP_CODE_SET_ELEVATION:
                on_set_elevation(p_lns, p_evt_write);
                break;

            // Unrecognized Op Code
            default:
                p_lns->pending_ctrlpt_response.response_code = CONTROL_POINT_RESP_OP_CODE_NOT_SUPPORTED;
                break;
        }
        
        ctrl_point_response_send(p_lns);
    } 
    else 
    {
        p_lns->control_point_procedure_status = BLE_LNS_CTRLPT_STATE_NO_PROC_IN_PROGRESS;
    }
}

/**@brief Write event handler.
 *
 * @param[in]   p_lns     Location and Navigation Service structure.
 * @param[in]   p_ble_evt Event received from the BLE stack.
 */
static void on_write(ble_lns_t * p_lns, ble_evt_t * p_ble_evt)
{
    ble_gatts_evt_write_t * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;
    
    if (p_evt_write->handle == p_lns->loc_speed_handles.cccd_handle)
    {
        on_loc_speed_cccd_write(p_lns, p_evt_write);
    }
    else if (p_evt_write->handle == p_lns->ctrl_point_handles.cccd_handle)
    {
        on_control_point_cccd_write(p_lns, p_evt_write);
    }
    else if (p_evt_write->handle == p_lns->navigation_handles.cccd_handle)
    {
        on_navigation_cccd_write(p_lns, p_evt_write);
    }

}

/**@brief Write authorization request event handler.
 *
 * @details The write authorization request event handler is only called when writing to the control point.
 * 
 * @param[in]   p_lns     Location and Navigation Service structure.
 * @param[in]   p_ble_evt Event received from the BLE stack.
 */
static void on_rw_authorize_req(ble_lns_t * p_lns, ble_evt_t * p_ble_evt)
{
    ble_gatts_evt_rw_authorize_request_t * p_authorize_request;

    p_authorize_request = &(p_ble_evt->evt.gatts_evt.params.authorize_request);

    if (
        (p_authorize_request->type == BLE_GATTS_AUTHORIZE_TYPE_WRITE)
        &&
        (p_authorize_request->request.write.handle == p_lns->ctrl_point_handles.value_handle)
        && 
        (p_ble_evt->evt.gatts_evt.params.authorize_request.request.write.op != BLE_GATTS_OP_PREP_WRITE_REQ)
        &&
        (p_ble_evt->evt.gatts_evt.params.authorize_request.request.write.op != BLE_GATTS_OP_EXEC_WRITE_REQ_NOW)
        &&
        (p_ble_evt->evt.gatts_evt.params.authorize_request.request.write.op != BLE_GATTS_OP_EXEC_WRITE_REQ_CANCEL)
       )
    {
        on_ctrl_point_write(p_lns, &(p_authorize_request->request.write));
    }
    
}

/**@brief Tx Complete event handler. This is used to retry sending a packet.
 *
 * @details Tx Complete event handler.
 *          Handles WRITE events from the BLE stack and if an indication was pending try sending it
 *          again.
 *
 * @param[in]   p_lns  Location navigation structure.
 */
static void on_tx_complete(ble_lns_t * p_lns)
{
    if (p_lns->control_point_procedure_status == BLE_LNS_CTRLPT_STATE_INDICATION_PENDING)
    {
        ctrl_point_response_send(p_lns);
    }
}

/**@brief Function for handling the BLE_GATTS_EVT_HVC event. A control point procedure is finished when its indication is acknowledged.
 *
 * @param[in]   p_lns       LNS structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_hvc_confirm(ble_lns_t * p_lns, ble_evt_t * p_ble_evt)
{
    if (p_ble_evt->evt.gatts_evt.params.hvc.handle == p_lns->ctrl_point_handles.value_handle)
    {
        if (p_lns->control_point_procedure_status == BLE_LNS_CTRLPT_STATE_IND_CONFIRM_PENDING)
        {
            p_lns->control_point_procedure_status = BLE_LNS_CTRLPT_STATE_NO_PROC_IN_PROGRESS;
        }
        else
        {
            if (p_lns->error_handler != NULL)
            {
                p_lns->error_handler(NRF_ERROR_INVALID_STATE);
            }
        }
    }
}

void ble_lns_on_ble_evt(ble_lns_t * p_lns, ble_evt_t * p_ble_evt)
{
    if (p_lns == NULL || p_ble_evt == NULL)
    {
        return;       
    }
    
    
    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            on_connect(p_lns, p_ble_evt);
            break;
            
        case BLE_GAP_EVT_DISCONNECTED:
            if (p_ble_evt->evt.gap_evt.conn_handle == p_lns->conn_handle)
            {
                on_disconnect(p_lns, p_ble_evt);
            }
            break;
            
        case BLE_GATTS_EVT_WRITE:
            if (p_ble_evt->evt.gatts_evt.conn_handle == p_lns->conn_handle)
            {
                on_write(p_lns, p_ble_evt);
            }
            break;
            
        case BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST:
            if (p_ble_evt->evt.gatts_evt.conn_handle == p_lns->conn_handle)
            {
                on_rw_authorize_req(p_lns, p_ble_evt);
            }
            break;
        
        case BLE_GATTS_EVT_HVC:
            if (p_ble_evt->evt.gatts_evt.conn_handle == p_lns->conn_handle)
            {
                on_hvc_confirm(p_lns, p_ble_evt);
            }
            break;

        case BLE_EVT_TX_COMPLETE:
            on_tx_complete(p_lns);
            break;
        
        default:
            // no implementation
            break;
    }
}

/**@brief Encode position quality.
 *
 * @param[in]   p_lns              Location and Navigation Service structure.
 * @param[in]   p_pos_qual         Position quality data to be encoded.
 * @param[out]  p_encoded_buffer   Buffer where the encoded data will be written.
 *
 * @return      Size of encoded data.
 */
static uint8_t pos_qual_encode(ble_lns_t * p_lns, ble_lns_pos_quality_t const * p_pos_qual, uint8_t * p_encoded_buffer)
{
    uint16_t flags = 0;
    uint8_t  len = 2;  // flags are added at last
       
    flags |= ((uint16_t)p_pos_qual->position_status << 7) & POS_QUAL_FLAG_POSITION_STATUS;
   
    if (p_pos_qual->number_of_satellites_in_solution_present)
    {
        flags |= POS_QUAL_FLAG_NUM_SATS_IN_SOLUTION_PRESENT;
        p_encoded_buffer[len++] = p_pos_qual->number_of_satellites_in_solution;
    }

    if (p_pos_qual->number_of_satellites_in_view_present)
    {
        flags |= POS_QUAL_FLAG_NUM_SATS_IN_VIEW_PRESENT;
        p_encoded_buffer[len++] = p_pos_qual->number_of_satellites_in_view;
    }

    if (p_pos_qual->time_to_first_fix_present)
    {
        flags |= POS_QUAL_FLAG_TIME_TO_FIRST_FIX_PRESESNT;
        len += uint16_encode(p_pos_qual->time_to_first_fix, &p_encoded_buffer[len]);
    }

    if (p_pos_qual->ehpe_present)
    {
        flags |= POS_QUAL_FLAG_EHPE_PRESENT;
        len += uint32_encode(p_pos_qual->ehpe, &p_encoded_buffer[len]);
    }
                    
    if (p_pos_qual->evpe_present)
    {
        flags |= POS_QUAL_FLAG_EVPE_PRESENT;
        len += uint32_encode(p_pos_qual->evpe, &p_encoded_buffer[len]);
    }

    if (p_pos_qual->hdop_present)
    {
        flags |= POS_QUAL_FLAG_HDOP_PRESENT;
        p_encoded_buffer[len++] = p_pos_qual->hdop;
    }

    if (p_pos_qual->vdop_present)
    {
        flags |= POS_QUAL_FLAG_VDOP_PRESENT;
        p_encoded_buffer[len++] = p_pos_qual->vdop;
    }
    
    // Flags field
    uint16_encode(flags, &p_encoded_buffer[0]); //lint !e534 "Ignoring return value of function"

    return len;
}


/**@brief Encode Location and Speed data.
 * 
 * @details The Location and Speed data will be encoded into two packets. Each packet has its own flags.
 *
 * @param[in]   p_lns              Location and Navigation Service structure.
 * @param[in]   p_loc_speed        Location and Speed data to be encoded.
 * @param[out]  p_encoded_buffer1  First buffer where encoded data will be written.
 * @param[out]  p_encoded_buffer2  Second buffer where encoded data will be written.
 * @param[out]  p_lengths          The lengths of the resulting buffers.
 *
 */
static void loc_speed_encode(ble_lns_t * p_lns, ble_lns_loc_speed_t const * p_loc_speed, 
                             uint8_t * p_encoded_buffer1, uint8_t * p_encoded_buffer2, uint8_t * p_lengths)
{
    uint16_t flags;

    // Encode packet 1
    flags = 0;
    p_lengths[0] = 2;
        
    // Instantaneous Speed
    if (p_lns->available_features & BLE_LNS_FEATURE_INSTANT_SPEED_SUPPORTED)
    {
        if ((p_loc_speed->instant_speed_present)&&(!p_lns->mask.instantaneous_speed))
        {
            flags |= LOC_SPEED_FLAG_INSTANT_SPEED_PRESENT;
            flags |= ((uint16_t)p_loc_speed->data_format<<9) & LOC_SPEED_FLAG_SPEED_AND_DIST_FORMAT;
            p_lengths[0] += uint16_encode(p_loc_speed->instant_speed, &p_encoded_buffer1[p_lengths[0]]);
        }
    }

    // Total Distance 
    if (p_lns->available_features & BLE_LNS_FEATURE_TOTAL_DISTANCE_SUPPORTED)
    {
        if ((p_loc_speed->total_distance_present)&&(!p_lns->mask.total_distance))
        {
            flags |= LOC_SPEED_FLAG_TOTAL_DISTANCE_PRESENT;
            p_encoded_buffer1[p_lengths[0]++] = ((p_loc_speed->total_distance >>  0) & 0xFF);
            p_encoded_buffer1[p_lengths[0]++] = ((p_loc_speed->total_distance >>  8) & 0xFF);
            p_encoded_buffer1[p_lengths[0]++] = ((p_loc_speed->total_distance >> 16) & 0xFF);
        }
    }
    
    // Location
    if (p_lns->available_features & BLE_LNS_FEATURE_LOCATION_SUPPORTED)
    {
        if ((p_loc_speed->location_present)&&(!p_lns->mask.location))
        {
            flags |= LOC_SPEED_FLAG_LOCATION_PRESENT;
            flags |= ((uint16_t)p_loc_speed->position_status <<7) & LOC_SPEED_FLAG_POSITION_STATUS;
            p_lengths[0] += uint32_encode(p_loc_speed->latitude,  &p_encoded_buffer1[p_lengths[0]]);
            p_lengths[0] += uint32_encode(p_loc_speed->longitude, &p_encoded_buffer1[p_lengths[0]]);
        }
    }
      
    // Flags field
    uint16_encode(flags, &p_encoded_buffer1[0]); //lint !e534 "Ignoring return value of function"

    // Encode packet 2
    flags = 0;
    p_lengths[1] = 2;
    
    // Elevation
    if (p_lns->available_features & BLE_LNS_FEATURE_ELEVATION_SUPPORTED)
    {
        if ((p_loc_speed->elevation_present)&&(!p_lns->mask.elevation))
        {
            flags |= LOC_SPEED_FLAG_ELEVATION_PRESENT;
            flags |= ((uint16_t)p_loc_speed->elevation_source<<10)& LOC_SPEED_FLAG_ELEVATION_SOURCE;
            p_encoded_buffer2[p_lengths[1]++] = ((p_loc_speed->elevation >>  0) & 0xFF);
            p_encoded_buffer2[p_lengths[1]++] = ((p_loc_speed->elevation >>  8) & 0xFF);
            p_encoded_buffer2[p_lengths[1]++] = ((p_loc_speed->elevation >> 16) & 0xFF);
        }
    }
    
    // Heading
    if (p_lns->available_features & BLE_LNS_FEATURE_HEADING_SUPPORTED)
    {
        if ((p_loc_speed->heading_present)&&(!p_lns->mask.heading))
        {
            flags |= LOC_SPEED_FLAG_HEADING_PRESENT;
            flags |= ((uint16_t)p_loc_speed->heading_source<<12) & LOC_SPEED_FLAG_HEADING_SOURCE;
            p_lengths[1] += uint16_encode(p_loc_speed->heading, &p_encoded_buffer2[p_lengths[1]]);
        }
    }
    
    // Rolling Time
    if (p_lns->available_features & BLE_LNS_FEATURE_ROLLING_TIME_SUPPORTED)
    {
        if ((p_loc_speed->rolling_time_present)&&(!p_lns->mask.rolling_time))
        {
            flags |= LOC_SPEED_FLAG_ROLLING_TIME_PRESENT;
            p_encoded_buffer2[p_lengths[1]++] = p_loc_speed->rolling_time;
        }
    }

    // UTC Time
    if (p_lns->available_features & BLE_LNS_FEATURE_UTC_TIME_SUPPORTED)
    {
        if ((p_loc_speed->utc_time_time_present)&&(!p_lns->mask.utc_time))
        {
            flags |= LOC_SPEED_FLAG_UTC_TIME_PRESENT;
            p_lengths[1] += ble_date_time_encode(&p_loc_speed->utc_time, &p_encoded_buffer2[p_lengths[1]]);
        }
    }      
    // Flags field
    uint16_encode(flags, &p_encoded_buffer2[0]); //lint !e534 "Ignoring return value of function"
}


/**@brief Encode Navigation data.
 *
 * @param[in]   p_lns              Location and Navigation Service structure.
 * @param[in]   p_navigation       Navigation data to be encoded.
 * @param[out]  p_encoded_buffer   Buffer where the encoded data will be written.
 *
 * @return      Size of encoded data.
 */
static uint8_t navigation_encode(ble_lns_t * p_lns, ble_lns_navigation_t const * p_navigation, uint8_t * p_encoded_buffer)
{
    uint16_t flags = 0;
    uint8_t  len   = 2;

    // Bearing
    len += uint16_encode(p_navigation->bearing, &p_encoded_buffer[len]);
    
    // Heading
    len += uint16_encode(p_navigation->heading, &p_encoded_buffer[len]);
    
    // Remaining Distance
    if (p_lns->available_features & BLE_LNS_FEATURE_REMAINING_DISTANCE_SUPPORTED)
    {
        if (p_navigation->remaining_dist_present)
        {
            flags |= NAV_FLAG_REMAINING_DIST_PRESENT;
            p_encoded_buffer[len++] = ((p_navigation->remaining_distance >>  0) & 0xFF);
            p_encoded_buffer[len++] = ((p_navigation->remaining_distance >>  8) & 0xFF);
            p_encoded_buffer[len++] = ((p_navigation->remaining_distance >> 16) & 0xFF);
        }
    }
    
    // Remaining Vertical Distance
    if (p_lns->available_features & BLE_LNS_FEATURE_REMAINING_VERT_DISTANCE_SUPPORTED)
    {
        if (p_navigation->remaining_vert_dist_present)
        {
            flags |= NAV_FLAG_REAMINGING_VERT_DIST_PRESESNT;
            p_encoded_buffer[len++] = ((p_navigation->remaining_vert_distance >>  0) & 0xFF);
            p_encoded_buffer[len++] = ((p_navigation->remaining_vert_distance >>  8) & 0xFF);
            p_encoded_buffer[len++] = ((p_navigation->remaining_vert_distance >> 16) & 0xFF);
        }
    }
    
    // Estimated Time of Arrival
    if (p_lns->available_features & BLE_LNS_FEATURE_EST_TIME_OF_ARRIVAL_SUPPORTED)
    {
        if (p_navigation->eta_present)
        {
            flags |= NAV_FLAG_ETA_PRESENT;
            len   += ble_date_time_encode(&p_navigation->eta, &p_encoded_buffer[len]);
        }
    }
    
    flags |= ((uint16_t)p_navigation->position_status          <<3) & NAV_FLAG_POSITION_STATUS;
    flags |= ((uint16_t)p_navigation->heading_source           <<5) & NAV_FLAG_HEADING_SOURCE;
    flags |= ((uint16_t)p_navigation->navigation_indicator_type<<6)& NAV_FLAG_NAVIGATION_INDICATOR_TYPE;
    flags |= ((uint16_t)p_navigation->waypoint_reached         <<7)& NAV_FLAG_WAYPOINT_REACHED;
    flags |= ((uint16_t)p_navigation->destination_reached      <<8)& NAV_FLAG_DESTINATION_REACHED;
    
    // Flags field
    uint16_encode(flags, &p_encoded_buffer[0]); //lint !e534 "Ignoring return value of function"

    return len;
}


/**@brief Add Location and Navigation Feature characteristic.
 *
 * @param[in]   p_lns        Location and Navigation Service structure.
 * @param[in]   p_lns_init   Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t loc_and_nav_feature_char_add(ble_lns_t * p_lns, const ble_lns_init_t * p_lns_init)
{
    uint8_t               init_value_encoded[sizeof(uint32_t)];
    uint8_t               len;
    ble_add_char_params_t add_char_params;
    
    len = uint32_encode(p_lns_init->available_features, init_value_encoded);
    
    memset(&add_char_params, 0, sizeof(add_char_params));
    
    add_char_params.uuid              = BLE_UUID_LN_FEATURE_CHAR;
    add_char_params.max_len           = len;
    add_char_params.init_len          = len;
    add_char_params.p_init_value      = &init_value_encoded[0];
    add_char_params.char_props.read   = true;
    add_char_params.read_access       = p_lns_init->loc_nav_feature_security_req_read_perm; 
    
    return characteristic_add(p_lns->service_handle, 
                              &add_char_params,
                              &p_lns->feature_handles);
}

/**@brief Add Location and Speed characteristic.
 *
 * @param[in]   p_lns        Location and Navigation Service structure.
 * @param[in]   p_lns_init   Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t loc_speed_char_add(ble_lns_t *            p_lns,
                                   const ble_lns_init_t * p_lns_init)
{
    uint8_t               encoded_initial_loc_speed1[BLE_L2CAP_MTU_DEF ];
    uint8_t               encoded_initial_loc_speed2[BLE_L2CAP_MTU_DEF ];
    uint8_t               lengths[2];
    ble_add_char_params_t add_char_params;
    
    loc_speed_encode(p_lns, p_lns_init->p_location_speed, &encoded_initial_loc_speed1[0], &encoded_initial_loc_speed2[0], &lengths[0]);
    
    memset(&add_char_params, 0, sizeof(add_char_params));
    
    add_char_params.uuid              = BLE_UUID_LN_LOCATION_AND_SPEED_CHAR;
    add_char_params.max_len           = BLE_L2CAP_MTU_DEF ;
    add_char_params.init_len          = lengths[0];
    add_char_params.p_init_value      = &encoded_initial_loc_speed1[0];
    add_char_params.is_var_len        = true;
    add_char_params.char_props.notify = true; 
    add_char_params.cccd_write_access = p_lns_init->loc_speed_security_req_cccd_write_perm; 
    
    return characteristic_add(p_lns->service_handle, 
                              &add_char_params,
                              &p_lns->loc_speed_handles);
}

/**@brief Add Location and Navigation position quality characteristic.
 *
 * @param[in]   p_lns        Location and Navigation Service structure.
 * @param[in]   p_lns_init   Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t pos_quality_char_add(ble_lns_t * p_lns, const ble_lns_init_t * p_lns_init)
{
    uint8_t               len;
    uint8_t               init_value_encoded[BLE_L2CAP_MTU_DEF];
    ble_add_char_params_t add_char_params;
        
    len = pos_qual_encode(p_lns, p_lns_init->p_position_quality, init_value_encoded);
    
    memset(&add_char_params, 0, sizeof(add_char_params));
    
    add_char_params.uuid              = BLE_UUID_LN_POSITION_QUALITY_CHAR;
    add_char_params.max_len           = BLE_L2CAP_MTU_DEF ;
    add_char_params.init_len          = len;
    add_char_params.p_init_value      = init_value_encoded;
    add_char_params.char_props.read   = true;
    add_char_params.read_access       = p_lns_init->position_quality_security_req_read_perm; 
    
    return characteristic_add(p_lns->service_handle, 
                              &add_char_params,
                              &p_lns->pos_qual_handles);
}

/**@brief Add Location and Navigation Control Point characteristic.
 *
 * @param[in]   p_lns        Location and Navigation Service structure.
 *
 * @return      NRF_SUCCESS if characteristic was successfully added, otherwise an error code.
 */
static uint32_t loc_and_nav_control_point_char_add(ble_lns_t * p_lns, const ble_lns_init_t * p_lns_init)
{
    ble_add_char_params_t add_char_params;
    
    memset(&add_char_params, 0, sizeof(add_char_params));
    
    add_char_params.uuid                      = BLE_UUID_LN_CONTROL_POINT_CHAR;
    add_char_params.max_len                   = 0;
    add_char_params.char_props.indicate       = true;
    add_char_params.char_props.write          = true;
    add_char_params.is_defered_write          = true;
    add_char_params.is_var_len                = true;
    add_char_params.max_len                   = BLE_L2CAP_MTU_DEF;
    add_char_params.write_access              = p_lns_init->ctrl_point_security_req_write_perm;
    add_char_params.cccd_write_access         = p_lns_init->ctrl_point_security_req_cccd_write_perm;
   
    return characteristic_add(p_lns->service_handle, 
                              &add_char_params,
                              &p_lns->ctrl_point_handles);
}

/**@brief Add Navigation characteristic.
 *
 * @param[in]   p_lns        Location and Navigation Service structure.
 * @param[in]   p_lns_init   Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t navigation_char_add(ble_lns_t *            p_lns,
                                    const ble_lns_init_t * p_lns_init)
{
    ble_add_char_params_t add_char_params;  
    
    memset(&add_char_params, 0, sizeof(add_char_params));
    
    add_char_params.uuid              = BLE_UUID_LN_NAVIGATION_CHAR;
    add_char_params.max_len           = BLE_LNS_NAV_MAX_LEN;
    add_char_params.init_len          = 0;
    add_char_params.p_init_value      = NULL;
    add_char_params.char_props.notify = true;
    add_char_params.cccd_write_access = p_lns_init->navigation_security_req_cccd_write_perm;
    
    return characteristic_add(p_lns->service_handle, 
                              &add_char_params,
                              &p_lns->navigation_handles);
}

/** @brief Check if there is a mismatch in initialization parameters.
 *
 *  @details It is possible to give an input which has an internal mismatch. Such a mismatch can arise in two different ways.
 *           One possibility is a mismatch between the characteristic present indicators and the available features specified. 
 *           The other mismatch arises when no pointer to the characteristic data structure is specified.
 *
 *  @param[in] p_lns_init The init structure which will be checked
 * 
 *  @return    false if there is no mismatch. true if there is a mismatch
 */
static bool init_param_mismatch_present(const ble_lns_init_t * p_lns_init)
{
    if (p_lns_init->is_position_quality_present == false)
    {
        if (p_lns_init->available_features &
             (BLE_LNS_FEATURE_NUM_SATS_IN_SOLUTION_SUPPORTED       |
              BLE_LNS_FEATURE_NUM_SATS_IN_VIEW_SUPPORTED           |
              BLE_LNS_FEATURE_TIME_TO_FIRST_FIX_SUPPORTED          |
              BLE_LNS_FEATURE_EST_HORZ_POS_ERROR_SUPPORTED         |
              BLE_LNS_FEATURE_EST_VERT_POS_ERROR_SUPPORTED         |
              BLE_LNS_FEATURE_HORZ_DILUTION_OF_PRECISION_SUPPORTED |
              BLE_LNS_FEATURE_VERT_DILUTION_OF_PRECISION_SUPPORTED)
           )
        {
            return true;
        }
        if (p_lns_init->p_position_quality != NULL)
        {
            return true;
        }
    } 
    else if (p_lns_init->is_position_quality_present == true)
    {
        if (p_lns_init->p_position_quality == NULL)
        {
            return true;
        }
    }
    
    if (p_lns_init->is_control_point_present == false)
    {
        if (p_lns_init->available_features &
              (BLE_LNS_FEATURE_LOC_AND_SPEED_CONTENT_MASKING_SUPPORTED |
               BLE_LNS_FEATURE_FIX_RATE_SETTING_SUPPORTED              |
               BLE_LNS_FEATURE_ELEVATION_SETTING_SUPPORTED)
            )
        {
            return true;
        }
    }
    
    if (p_lns_init->is_navigation_present == false)
    {
        if (p_lns_init->available_features & 
              (BLE_LNS_FEATURE_REMAINING_DISTANCE_SUPPORTED      | 
               BLE_LNS_FEATURE_REMAINING_VERT_DISTANCE_SUPPORTED |
               BLE_LNS_FEATURE_EST_TIME_OF_ARRIVAL_SUPPORTED)   
            )
        {
            return true;
        }
        if (p_lns_init->p_navigation != NULL)
        {
            return true;
        }            
    }
    else if (p_lns_init->is_navigation_present == true)
    {
        if (p_lns_init->p_navigation == NULL)
        {
            return true;
        }
    }

    // location and speed must always be specified
    if (p_lns_init->p_location_speed == NULL)
    {
        return true;
    }
    
    return false;
}

uint32_t ble_lns_init(ble_lns_t * p_lns, const ble_lns_init_t * p_lns_init)
{
    if (p_lns == NULL || p_lns_init == NULL)
    {
        return NRF_ERROR_NULL;
    }
    
    if (init_param_mismatch_present(p_lns_init) == true)
    {
        return NRF_ERROR_INVALID_PARAM;        
    }
    
      
    uint32_t            err_code;
    ble_uuid_t          service_uuid;
    
    // Initialize service structure
    p_lns->evt_handler           = p_lns_init->evt_handler;
    p_lns->error_handler         = p_lns_init->error_handler;
    p_lns->conn_handle           = BLE_CONN_HANDLE_INVALID;
    p_lns->available_features    = p_lns_init->available_features;
    p_lns->is_navigation_present = p_lns_init->is_navigation_present;
    
    memset(&p_lns->mask, 0, sizeof (p_lns->mask));
    
    p_lns->p_location_speed   = p_lns_init->p_location_speed;
    p_lns->p_position_quality = p_lns_init->p_position_quality;
    p_lns->p_navigation       = p_lns_init->p_navigation;
    
    p_lns->control_point_procedure_status = BLE_LNS_CTRLPT_STATE_NO_PROC_IN_PROGRESS;
    
    memset(p_lns->ctrl_pnt_cccd_handle_status, 0, INT16_LEN);
    memset(p_lns->navigation_cccd_handle_status, 0, INT16_LEN);
    memset(p_lns->loc_speed_cccd_handle_status, 0, INT16_LEN);
    
    p_lns->is_navigation_running = false;
    p_lns->number_of_routes = 0;
    p_lns->selected_route = BLE_LNS_INVALID_ROUTE;
    p_lns->fix_rate       = BLE_LNS_NO_FIX;
    
    for (int i = 0; i < BLE_LNS_MAX_NUM_ROUTES; i++)
    {
        p_lns->routes[i].route_id = BLE_LNS_INVALID_ROUTE;
    }
    
    // Add service
    BLE_UUID_BLE_ASSIGN(service_uuid, BLE_UUID_LOCATION_AND_NAVIGATION_SERVICE);
   
    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &service_uuid, &p_lns->service_handle);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
    
    // Add location and navigation feature characteristic
    err_code = loc_and_nav_feature_char_add(p_lns, p_lns_init);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Add location and speed characteristic
    err_code = loc_speed_char_add(p_lns, p_lns_init);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
    
    if (p_lns_init->is_position_quality_present)
    {
        // Add Position quality characteristic
        err_code = pos_quality_char_add(p_lns, p_lns_init);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }
    else 
    {
        p_lns->pos_qual_handles.cccd_handle      = BLE_GATT_HANDLE_INVALID;
        p_lns->pos_qual_handles.sccd_handle      = BLE_GATT_HANDLE_INVALID;
        p_lns->pos_qual_handles.user_desc_handle = BLE_GATT_HANDLE_INVALID;
        p_lns->pos_qual_handles.value_handle     = BLE_GATT_HANDLE_INVALID;
    }

    if (p_lns_init->is_control_point_present)
    {
        // Add control pointer characteristic
        err_code = loc_and_nav_control_point_char_add(p_lns, p_lns_init);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }
    else
    {
        p_lns->ctrl_point_handles.cccd_handle      = BLE_GATT_HANDLE_INVALID;
        p_lns->ctrl_point_handles.sccd_handle      = BLE_GATT_HANDLE_INVALID;
        p_lns->ctrl_point_handles.user_desc_handle = BLE_GATT_HANDLE_INVALID;
        p_lns->ctrl_point_handles.value_handle     = BLE_GATT_HANDLE_INVALID; 
    }
    
    if (p_lns_init->is_navigation_present)
    {
        // Add navigation characteristic
        err_code = navigation_char_add(p_lns, p_lns_init);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }
    else 
    {
        p_lns->navigation_handles.cccd_handle      = BLE_GATT_HANDLE_INVALID;
        p_lns->navigation_handles.sccd_handle      = BLE_GATT_HANDLE_INVALID;
        p_lns->navigation_handles.user_desc_handle = BLE_GATT_HANDLE_INVALID;
        p_lns->navigation_handles.value_handle     = BLE_GATT_HANDLE_INVALID;        
    }
    
    return NRF_SUCCESS;
}


uint32_t ble_lns_loc_speed_send(ble_lns_t * p_lns)
{
    if (p_lns == NULL)
    {
        return NRF_ERROR_NULL;
    }
    
    if (p_lns->conn_handle == BLE_CONN_HANDLE_INVALID)
    {
        return NRF_ERROR_INVALID_STATE;
    }
    
    uint32_t err_code;
    

    // Location speed data is always sent as two packets.
    uint8_t                encoded_loc_speed1[BLE_L2CAP_MTU_DEF];
    uint8_t                encoded_loc_speed2[BLE_L2CAP_MTU_DEF];
    uint8_t                lengths[2];
    uint16_t               hvx_len;
    ble_gatts_hvx_params_t hvx_params;
    
    loc_speed_encode(p_lns, p_lns->p_location_speed, &encoded_loc_speed1[0], &encoded_loc_speed2[0], &lengths[0]);

    memset(&hvx_params, 0, sizeof(hvx_params));

    hvx_params.handle   = p_lns->loc_speed_handles.value_handle;
    hvx_params.type     = BLE_GATT_HVX_NOTIFICATION;
    hvx_params.offset   = 0;
    hvx_params.p_len    = &hvx_len;
    
    // send packet 1
    
    hvx_len           = lengths[0];
    hvx_params.p_data = &encoded_loc_speed1[0];

    err_code = sd_ble_gatts_hvx(p_lns->conn_handle, &hvx_params);
    
    if (err_code == NRF_ERROR_INVALID_STATE)
    {
        return err_code;
    }
    
    if ((err_code == NRF_SUCCESS) && (hvx_len != lengths[0]))
    {
        return NRF_ERROR_DATA_SIZE;
    }
    
    
    // send packet 2
    hvx_len           = lengths[1];
    hvx_params.p_data = &encoded_loc_speed2[0];

    err_code = sd_ble_gatts_hvx(p_lns->conn_handle, &hvx_params);
    
    if ((err_code == NRF_SUCCESS) && (hvx_len != lengths[1]))
    {
        return NRF_ERROR_DATA_SIZE;
    }
        


    return err_code;
}

uint32_t ble_lns_navigation_send(ble_lns_t * p_lns)
{
    if (p_lns == NULL)
    {
        return NRF_ERROR_NULL;
    }
    
    if (p_lns->conn_handle == BLE_CONN_HANDLE_INVALID)
    {
        return NRF_ERROR_INVALID_STATE;
    }
    
    uint32_t err_code;
    
    // Send value if connected and notifying
    if (p_lns->is_navigation_present)
    {
        if (p_lns->is_navigation_running)
        {
            uint8_t                encoded_navigation[BLE_L2CAP_MTU_DEF];
            uint16_t               len;
            uint16_t               hvx_len;
            ble_gatts_hvx_params_t hvx_params;
            
            len     = navigation_encode(p_lns, p_lns->p_navigation, &encoded_navigation[0]);
            hvx_len = len;
          
            memset(&hvx_params, 0, sizeof(hvx_params));
        
            hvx_params.handle   = p_lns->navigation_handles.value_handle;
            hvx_params.type     = BLE_GATT_HVX_NOTIFICATION;
            hvx_params.offset   = 0;
            hvx_params.p_len    = &hvx_len;
            hvx_params.p_data   = &encoded_navigation[0];
        
            err_code = sd_ble_gatts_hvx(p_lns->conn_handle, &hvx_params);
            if ((err_code == NRF_SUCCESS) && (hvx_len != len))
            {
                err_code = NRF_ERROR_DATA_SIZE;
            }
        }
        else
        {
            err_code = NRF_ERROR_INVALID_STATE;
        }

    }
    else
    {
        err_code = NRF_ERROR_NOT_SUPPORTED;
    }

    return err_code;
}


uint32_t ble_lns_add_route(ble_lns_t * p_lns, ble_lns_route_t * p_route)
{
    if (p_lns == NULL || p_route == NULL)
    {
        return NRF_ERROR_NULL;
    }
    
    if (p_lns->is_navigation_present == false)
    {
        return NRF_ERROR_NOT_SUPPORTED;
    }
    
    if (p_lns->number_of_routes >= BLE_LNS_MAX_NUM_ROUTES)
    {
        return NRF_ERROR_NO_MEM;
    }
    
    // find an empty slot
    for (int i = 0; i < BLE_LNS_MAX_NUM_ROUTES; i++)
    {
        if (p_lns->routes[i].route_id == BLE_LNS_INVALID_ROUTE)
        {
            memcpy(p_lns->routes[i].route_name, p_route->route_name, BLE_LNS_MAX_ROUTE_NAME_LEN);
            p_lns->routes[i].route_id = i;
            p_route->route_id = i;
            
            p_lns->number_of_routes++;
            return NRF_SUCCESS;  
        }
    }
    // an inconsistency in the route the table 
    return NRF_ERROR_INTERNAL;
}

uint32_t ble_lns_remove_route(ble_lns_t * p_lns, uint16_t route_id)
{
    if (p_lns == NULL)
    {
        return NRF_ERROR_NULL;
    }
    
    if (p_lns->is_navigation_present == false)
    {
        return NRF_ERROR_NOT_SUPPORTED;
    }
    
    if (route_id >= BLE_LNS_MAX_NUM_ROUTES)
    {
        return NRF_ERROR_INVALID_PARAM;
    }
    
    if (p_lns->routes[route_id].route_id != BLE_LNS_INVALID_ROUTE)
    {
        p_lns->routes[route_id].route_id = BLE_LNS_INVALID_ROUTE;
        p_lns->number_of_routes--;
        return NRF_SUCCESS;
    }
    else
    {
        return NRF_ERROR_INVALID_PARAM;
    }
}

