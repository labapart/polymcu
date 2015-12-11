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

/** @file
 *
 * @defgroup ble_sdk_srv_lns Location and Navigation Service
 * @{
 * @ingroup ble_sdk_srv
 * @brief Location and Navigation Service module.
 *
 * @details This module implements the Location and Navigation Service with the Location and Speed, Position 
 *          Quality, Feature, Control Point, and Navigation characteristics.
 *
 *          If an event handler is supplied by the application, the Location and Navigation Service will
 *          generate Location and Navigation Service events to the application.
 *
 * @note The application must propagate BLE stack events to the Location and Navigation Service module by calling
 *       @ref ble_lns_on_ble_evt() from the from the ble_stack_handler callback.
 */

#ifndef BLE_LNS_H__
#define BLE_LNS_H__

#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"
#include "ble_date_time.h"

// Location and Navigation Service feature bits
#define BLE_LNS_FEATURE_INSTANT_SPEED_SUPPORTED                     (0x01 << 0)           /**< Instaneous Speed Supported bit. */
#define BLE_LNS_FEATURE_TOTAL_DISTANCE_SUPPORTED                    (0x01 << 1)           /**< Total Distance Supported bit. */
#define BLE_LNS_FEATURE_LOCATION_SUPPORTED                          (0x01 << 2)           /**< Location Supported bit. */
#define BLE_LNS_FEATURE_ELEVATION_SUPPORTED                         (0x01 << 3)           /**< Elevation Supported bit. */
#define BLE_LNS_FEATURE_HEADING_SUPPORTED                           (0x01 << 4)           /**< Heading Supported bit. */
#define BLE_LNS_FEATURE_ROLLING_TIME_SUPPORTED                      (0x01 << 5)           /**< Rolling Time Supported bit. */
#define BLE_LNS_FEATURE_UTC_TIME_SUPPORTED                          (0x01 << 6)           /**< UTC Time Supported bit. */
#define BLE_LNS_FEATURE_REMAINING_DISTANCE_SUPPORTED                (0x01 << 7)           /**< Remaining Distance Supported bit. */
#define BLE_LNS_FEATURE_REMAINING_VERT_DISTANCE_SUPPORTED           (0x01 << 8)           /**< Remaining Vertical Distance Supported bit. */
#define BLE_LNS_FEATURE_EST_TIME_OF_ARRIVAL_SUPPORTED               (0x01 << 9)           /**< Estimated Time of Arrival Supported bit. */
#define BLE_LNS_FEATURE_NUM_SATS_IN_SOLUTION_SUPPORTED              (0x01 << 10)          /**< Number of Satellites in Solution Supported bit. */
#define BLE_LNS_FEATURE_NUM_SATS_IN_VIEW_SUPPORTED                  (0x01 << 11)          /**< Number of Satellites in View Supported bit. */
#define BLE_LNS_FEATURE_TIME_TO_FIRST_FIX_SUPPORTED                 (0x01 << 12)          /**< Time to First Fix Supported bit. */
#define BLE_LNS_FEATURE_EST_HORZ_POS_ERROR_SUPPORTED                (0x01 << 13)          /**< Estimated Horizontal Position Error Supported bit. */
#define BLE_LNS_FEATURE_EST_VERT_POS_ERROR_SUPPORTED                (0x01 << 14)          /**< Estimated Vertical Position Error Supported bit. */
#define BLE_LNS_FEATURE_HORZ_DILUTION_OF_PRECISION_SUPPORTED        (0x01 << 15)          /**< Horizontal Dilution of Precision Supported bit. */
#define BLE_LNS_FEATURE_VERT_DILUTION_OF_PRECISION_SUPPORTED        (0x01 << 16)          /**< Vertical Dilution of Precision Supported bit. */
#define BLE_LNS_FEATURE_LOC_AND_SPEED_CONTENT_MASKING_SUPPORTED     (0x01 << 17)          /**< Location and Speed Characteristic Content Masking Supported bit. */
#define BLE_LNS_FEATURE_FIX_RATE_SETTING_SUPPORTED                  (0x01 << 18)          /**< Fix Rate Setting Supported bit. */
#define BLE_LNS_FEATURE_ELEVATION_SETTING_SUPPORTED                 (0x01 << 19)          /**< Elevation Setting Supported bit. */
#define BLE_LNS_FEATURE_POSITION_STATUS_SUPPORTED                   (0x01 << 20)          /**< Position Status Supported bit. */

#define BLE_LNS_MAX_ROUTE_NAME_LEN                                  (BLE_L2CAP_MTU_DEF-5) /**< The maximum length of length of a route name. */
#define BLE_LNS_MAX_NUM_ROUTES                                      3                     /**< The maximum number of routes. This affects memory usage only. */
#define BLE_LNS_INVALID_ROUTE                                       0xFFFF
#define BLE_LNS_NO_FIX                                              0xFF

#define MAX_CTRL_POINT_RESP_PARAM_LEN                               BLE_LNS_MAX_ROUTE_NAME_LEN + 3

// Location and Navigation Control Point Op Code values
typedef enum {
    OP_CODE_RESERVED                         = 0x00, /**< Reserved for future use. */
    OP_CODE_SET_CUMULATIVE_VALUE             = 0x01, /**< Set Cumulative Value. */
    OP_CODE_MASK_LOC_SPEED_CONTENT           = 0x02, /**< Mask Location and Speed Characteristic Content. */
    OP_CODE_NAV_CONTROL                      = 0x03, /**< Navigation Control. */
    OP_CODE_REQ_NUM_ROUTES                   = 0x04, /**< Request Number of Routes. */
    OP_CODE_REQ_NAME_OF_ROUTE                = 0x05, /**< Request Name of Route. */
    OP_CODE_SELECT_ROUTE                     = 0x06, /**< Select Route. */
    OP_CODE_SET_FIX_RATE                     = 0x07, /**< Set Fix Rate. */
    OP_CODE_SET_ELEVATION                    = 0x08, /**< Set Elevation. */
    OP_CODE_RESPONSE_CODE                    = 0x20  /**< Response code. */
} ble_lns_ctrl_point_opcodes_t;

// Location and Navigation Control Point response values
typedef enum {
    CONTROL_POINT_RESP_RESERVED              = 0x00,                                        /**< Reserved for future use. */
    CONTROL_POINT_RESP_SUCCESS               = 0x01,                                        /**< Success. */
    CONTROL_POINT_RESP_OP_CODE_NOT_SUPPORTED = 0x02,                                        /**< Op Code not supported. */
    CONTROL_POINT_RESP_INVALID_PARAMETER     = 0x03,                                        /**< Invalid Parameter. */
    CONTROL_POINT_RESP_OPERATION_FAILED      = 0x04,                                        /**< Operation Failed. */
    CONTROL_POINT_RESP_PROC_ALR_IN_PROG      = BLE_GATT_STATUS_ATTERR_CPS_PROC_ALR_IN_PROG, /**< Control point procedure is already in progress. */
    CONTROL_POINT_RESP_CCCD_CONFIG_IMPROPER  = BLE_GATT_STATUS_ATTERR_CPS_CCCD_CONFIG_ERROR /**< CCCD is improperly configured. */
} ble_lns_ctrl_point_response_codes_t;
    
/** @brief Location and Navigation event type. This list defines the possible events types from the Location and Navigation Service. */
typedef enum {  
    BLE_LNS_LOC_SPEED_EVT_NOTIFICATION_ENABLED,     /**< Location and Speed value notification was enabled. */
    BLE_LNS_LOC_SPEED_EVT_NOTIFICATION_DISABLED,    /**< Location and Speed value notification was disabled. */  
    BLE_LNS_LOC_SPEED_EVT_MASK_SET,                 /**< Location and Speed feature mask was set. */
    BLE_LNS_LOC_SPEED_EVT_TOTAL_DISTANCE_SET,       /**< Location and Navigation total distance was set. */
    BLE_LNS_LOC_SPEED_EVT_ELEVATION_SET,            /**< Location and Navigation elevation was set. */
      
    BLE_LNS_POS_QUAL_EVT_FIX_RATE_SET,              /**< Fix rate was set. */

    BLE_LNS_CONTROL_POINT_EVT_INDICATION_ENABLED,   /**< Location and Navigation Control point indication was enabled. */
    BLE_LNS_CONTROL_POINT_EVT_INDICATION_DISABLED,  /**< Location and Navigation Control point indication was disabled. */
    
    BLE_LNS_NAVIGATION_EVT_COMMAND,                 /**< A navigation command was issued. */
    BLE_LNS_NAVIGATION_EVT_ROUTE_SELECTED,          /**< A route was selected. */
    BLE_LNS_NAVIGATION_EVT_NOTIFICATION_ENABLED,    /**< Navigation value notification was enabled. */
    BLE_LNS_NAVIGATION_EVT_NOTIFICATION_DISABLED,   /**< Navigation value notification was disabled. */    
} ble_lns_evt_type_t;

/** @brief Navigation commands. These commands can be sent to the control point and returned by an event callback. */
typedef enum {
    BLE_LNS_NAVIGATION_STOP          = 0x00,        /**< When received, is_navigation_running in @ref ble_lns_s will be set to false. */
    BLE_LNS_NAVIGATION_START         = 0x01,        /**< When received, is_navigation_running in @ref ble_lns_s will be set to true. */
    BLE_LNS_NAVIGATION_PAUSE         = 0x02,        /**< When received, is_navigation_running in @ref ble_lns_s will be set to false. */
    BLE_LNS_NAVIGATION_CONTINUE      = 0x03,        /**< When received, is_navigation_running in @ref ble_lns_s will be set to true. */
    BLE_LNS_NAVIGATION_SKIP_WAYPOINT = 0x04,        /**< When received, is_navigation_running in @ref ble_lns_s will not be affected. */
    BLE_LNS_NAVIGATION_NEAREST       = 0x05         /**< When received, is_navigation_running in @ref ble_lns_s will be set to true. */
} ble_lns_navigation_command_t;

/** @brief A mask can be used to temporarily enable and disable features of the Location and Speed characteristic.*/
typedef struct {
    bool instantaneous_speed;
    bool total_distance;
    bool location;
    bool elevation;
    bool heading;
    bool rolling_time;
    bool utc_time;
} ble_lns_mask_t;

/** @brief Location and Navigation event structure. When an event occurs, the data structures of the module are automatically updated. */
typedef struct {
    ble_lns_evt_type_t               evt_type;
    union {
        ble_lns_mask_t               mask;
        uint32_t                     total_distance;
        uint32_t                     elevation;
        int8_t                       fix_rate;
        ble_lns_navigation_command_t navigation_command;
        uint16_t                     selected_route;
    } params;
} ble_lns_evt_t;

// Forward declarations of the ble_lns types. 
typedef struct ble_lns_init_s        ble_lns_init_t;
typedef struct ble_lns_s             ble_lns_t;
typedef struct ble_lns_loc_speed_s   ble_lns_loc_speed_t;
typedef struct ble_lns_pos_quality_s ble_lns_pos_quality_t;
typedef struct ble_lns_navigation_s  ble_lns_navigation_t;

/**@brief Location and Navigation Service event handler type. */
typedef void (*ble_lns_evt_handler_t) (ble_lns_t * p_lns, ble_lns_evt_t * p_evt);

/**@brief Location and Navigation Service init structure. This structure contains all options and data needed to
 *        initialize the service. */
struct ble_lns_init_s
{
    ble_lns_evt_handler_t   evt_handler;                             /**< Event handler to be called for handling events in the Location and Navigation Service. */
    ble_srv_error_handler_t error_handler;                           /**< Errors will be sent back to this function. */
    
    bool                    is_position_quality_present;             /**< If set to true, the position quality characteristic will be added. Else not. */
    bool                    is_control_point_present;                /**< If set to true, the control point characteristic will be added. Else not. */
    bool                    is_navigation_present;                   /**< If set to true, the navigation characteristic will be added. Else not. */
    
    security_req_t          loc_nav_feature_security_req_read_perm;  /**< Read security level of the LN Feature characteristic. */
    security_req_t          loc_speed_security_req_cccd_write_perm;  /**< CCCD write security level of the Write Location and Speed characteristic. */
    security_req_t          position_quality_security_req_read_perm; /**< Read security level of the Position Quality characteristic. */
    security_req_t          navigation_security_req_cccd_write_perm; /**< CCCD write security level of the Navigation characteristic. */ 
    security_req_t          ctrl_point_security_req_write_perm;      /**< Read security level of the LN Control Point characteristic. */
    security_req_t          ctrl_point_security_req_cccd_write_perm; /**< CCCD write security level of the LN Control Point characteristic. */
                            
    uint32_t                available_features;                      /**< Value of the LN feature. */
    ble_lns_loc_speed_t     * p_location_speed;                      /**< Initial Location and Speed. */
    ble_lns_pos_quality_t   * p_position_quality;                    /**< Initial Position Quality. */
    ble_lns_navigation_t    * p_navigation;                          /**< Initial Navigation data structure. */
};

/**@brief Definition of a navigation route.*/
typedef struct ble_lns_routes_s
{
    uint16_t route_id;
    uint8_t  route_name[BLE_LNS_MAX_ROUTE_NAME_LEN];
} ble_lns_route_t;


/** @brief Location and Navigation Control Point procedure status (indicates if a procedure is in progress or not and which procedure is in progress).*/
typedef enum {
    BLE_LNS_CTRLPT_STATE_NO_PROC_IN_PROGRESS,                        /**< No procedure in progress. */
    BLE_LNS_CTRLPT_STATE_INDICATION_PENDING,                         /**< Control Point indication is pending. */
    BLE_LNS_CTRLPT_STATE_IND_CONFIRM_PENDING,                        /**< Waiting for the indication confirmation. */
} ble_lns_ctrlpt_procedure_status_t;

/** @brief Information included in a control point write response indication. */
typedef struct {
    uint8_t op_code;                                                 /**< Opcode of the control point write action. */
    uint8_t response_code;                                           /**< Response code of the control point write action. */
    uint8_t response_param_len;
    uint8_t response_param[MAX_CTRL_POINT_RESP_PARAM_LEN];  
} ble_lns_ctrlpt_response_t;

/**@brief Location and Navigation Service structure. This structure contains various status information for the service. */
struct ble_lns_s
{
    ble_lns_evt_handler_t             evt_handler;                                       /**< Event handler to be called for handling events in the Location and Navigation Service. */
    ble_srv_error_handler_t           error_handler;                                     /**< Error handler. */

    bool                              is_navigation_present;                             /**< If set to true, the navigation characteristic is present. Else not. */

    uint16_t                          conn_handle;                                       /**< Handle of the current connection (as provided by the BLE stack; BLE_CONN_HANDLE_INVALID if not in a connection). */
    uint16_t                          service_handle;                                    /**< Handle of Location and Navigation Service (as provided by the BLE stack). */
    ble_gatts_char_handles_t          loc_speed_handles;                                 /**< Handles related to the Location and Speed characteristic. */
    ble_gatts_char_handles_t          feature_handles;                                   /**< Handles related to the Location and Navigation Feature characteristic. */
    ble_gatts_char_handles_t          ctrl_point_handles;                                /**< Handles related to the Location and Navigation Control Point characteristic. */
    ble_gatts_char_handles_t          navigation_handles;                                /**< Handles related to the Navigation characteristic. */
    ble_gatts_char_handles_t          pos_qual_handles;                                  /**< Handles related to the Position Quality characteristic. */
    uint32_t                          available_features;                                /**< Value of Location and Navigation feature. */
   
    uint8_t                           loc_speed_cccd_handle_status[BLE_CCCD_VALUE_LEN];  /**< A buffer that stores the value of the CCCD handle of the Location and Speed characteristic.*/
    uint8_t                           ctrl_pnt_cccd_handle_status[BLE_CCCD_VALUE_LEN];   /**< A buffer that stores the value of the CCCD handle of the Control Point characteristic.*/
    uint8_t                           navigation_cccd_handle_status[BLE_CCCD_VALUE_LEN]; /**< A buffer that stores the value of the CCCD handle of the Navigation characteristic.*/

    uint8_t                           fix_rate;                                          /**< The fix rate in seconds. */
    ble_lns_mask_t                    mask;                                              /**< When a field is set to true, the feature is turned off. Can be set by writing to the control point. */
    ble_lns_loc_speed_t               * p_location_speed;                                /**< Location and Speed. */
    ble_lns_pos_quality_t             * p_position_quality;                              /**< Position measurement quality. */
    ble_lns_navigation_t              * p_navigation;                                    /**< Navigation data structure. */
    ble_lns_ctrlpt_procedure_status_t control_point_procedure_status;                    /**< Status of a control point action. */
    ble_lns_ctrlpt_response_t         pending_ctrlpt_response;                           /**< Pending response data from a control point action. */
    bool                              is_navigation_running;                             /**< This variable can be set using the control point. Must be true to be able to send navigation updates. */
    uint16_t                          number_of_routes;                                  /**< The number of routes present. */
    uint16_t                          selected_route;                                    /**< Index of route selected. */
    ble_lns_route_t                   routes[BLE_LNS_MAX_NUM_ROUTES];                    /**< The stored routes. */
};

/** @brief Position status. This enumeration defines how to interpret the position data. */
typedef enum
{
    BLE_LNS_NO_POSITION         = 0,
    BLE_LNS_POSITION_OK         = 1,
    BLE_LNS_ESTIMATED           = 2,
    BLE_LNS_LAST_KNOWN_POSITION = 3
} ble_lns_pos_status_type_t;

/** @brief The format of the position and speed measurements. */
typedef enum
{
    BLE_LNS_SPEED_DISTANCE_FORMAT_2D = 0,
    BLE_LNS_SPEED_DISTANCE_FORMAT_3D = 1
} ble_lns_speed_distance_format_t;

/** @brief Elevation source. */
typedef enum 
{
    BLE_LNS_ELEV_SOURCE_POSITIONING_SYSTEM = 0,
    BLE_LNS_ELEV_SOURCE_BAROMETRIC         = 1,
    BLE_LNS_ELEV_SOURCE_DATABASE_SERVICE   = 2,
    BLE_LNS_ELEV_SOURCE_OTHER              = 3
} ble_lns_elevation_source_t;

/** @brief Heading source. */
typedef enum 
{
    BLE_LNS_HEADING_SOURCE_MOVEMENT        = 0,
    BLE_LNS_HEADING_SOURCE_COMPASS         = 1
} ble_lns_heading_source_t;

/**@brief Location and Speed data structure. */
struct ble_lns_loc_speed_s
{
    bool                            instant_speed_present;                     /**< Instantaneous Speed present (0=not present, 1=present). */
    bool                            total_distance_present;                    /**< Total Distance present (0=not present, 1=present). */
    bool                            location_present;                          /**< Location present (0=not present, 1=present). */
    bool                            elevation_present;                         /**< Elevation present (0=not present, 1=present). */
    bool                            heading_present;                           /**< Heading present (0=not present, 1=present). */
    bool                            rolling_time_present;                      /**< Rolling Time present (0=not present, 1=present). */
    bool                            utc_time_time_present;                     /**< UTC Time present (0=not present, 1=present). */
    ble_lns_pos_status_type_t       position_status;                           /**< Status of current position */ 
    ble_lns_speed_distance_format_t data_format;                               /**< Format of data (either 2D or 3D). */
    ble_lns_elevation_source_t      elevation_source;                          /**< Source of the elevation measurement. */
    ble_lns_heading_source_t        heading_source;                            /**< Source of the heading measurement. */
    uint16_t                        instant_speed;                             /**< Instantaneous Speed (1/10 meter per sec). */
    uint32_t                        total_distance;                            /**< Total Distance (meters), size=24 bits. */
    int32_t                         latitude;                                  /**< Latitude (10e-7 degrees). */
    int32_t                         longitude;                                 /**< Longitude (10e-7 degrees). */
    int32_t                         elevation;                                 /**< Elevation (1/100 meters), size=24 bits. */
    uint16_t                        heading;                                   /**< Heading (1/100 degrees). */
    uint8_t                         rolling_time;                              /**< Rolling Time (seconds). */
    ble_date_time_t                 utc_time;                                  /**< UTC Time. */
};

/** @brief Position quality structure. */
struct ble_lns_pos_quality_s
{
    bool                            number_of_satellites_in_solution_present;  /**< The number of satellites present in solution (0=not present, 1=present). */
    bool                            number_of_satellites_in_view_present;      /**< The number of satellites present in solution (0=not present, 1=present). */
    bool                            time_to_first_fix_present;                 /**< Time to the first position fix present (0=not present, 1=present). */
    bool                            ehpe_present;                              /**< Error in horizontal position estimate present (0=not present, 1=present). */
    bool                            evpe_present;                              /**< Error in vertical position estimate present (0=not present, 1=present). */
    bool                            hdop_present;                              /**< Horizontal dilution of precision present (0=not present, 1=present). */
    bool                            vdop_present;                              /**< Vertical dilution of precision present (0=not present, 1=present). */
    ble_lns_pos_status_type_t       position_status;                           /**< Status of last measured position. */   
    uint8_t                         number_of_satellites_in_solution;          /**< The number of satellites in solution (unitless, with a resolution of 1). */
    uint8_t                         number_of_satellites_in_view;              /**< The number of satellites in view (unitless, with a resolution of 1). */
    uint16_t                        time_to_first_fix;                         /**< Time to the first position fix (seconds, with a resolution of 1/10). */
    uint32_t                        ehpe;                                      /**< Error in horizontal position estimate (meters, with a resolution of 1/100). */
    uint32_t                        evpe;                                      /**< Error in vertical position estimate (meters, with a resolution of 1/100). */
    uint8_t                         hdop;                                      /**< Horizontal dilution of precision (unitless, with a resolution of 2/10). */
    uint8_t                         vdop;                                      /**< Vertical dilution of precision (unitless, with a resolution of 2/10). */
};

/** @brief Navigation indicator type. */
typedef enum 
{
    BLE_LNS_NAV_TO_WAYPOINT    = 0,
    BLE_LNS_NAV_TO_DESTINATION = 1
} ble_lns_nav_indicator_type_t;

/**@brief Navigation data structure. */
struct ble_lns_navigation_s
{
    bool                             remaining_dist_present;                    /**< Remaining Distance present (0=not present, 1=present). */
    bool                             remaining_vert_dist_present;               /**< Remaining Vertical Distance present (0=not present, 1=present). */
    bool                             eta_present;                               /**< Estimated Time of Arrival present (0=not present, 1=present). */
    ble_lns_pos_status_type_t        position_status;                           /**< Status of last measured position. */   
    ble_lns_heading_source_t         heading_source;                            /**< Source of the heading measurement. */
    ble_lns_nav_indicator_type_t     navigation_indicator_type;                 /**< Navigation indicator type. */
    bool                             waypoint_reached;                          /**< Waypoint Reached (0=not reached, 1=reached). */
    bool                             destination_reached;                       /**< Destination Reached (0=not reached, 1=reached). */
    uint16_t                         bearing;                                   /**< Bearing (1/100 degrees).*/
    uint16_t                         heading;                                   /**< Heading (1/100 degrees), size=24 bit. */
    uint32_t                         remaining_distance;                        /**< Remaining Distance (1/10 meters), size=24 bit. */
    int32_t                          remaining_vert_distance;                   /**< Remaining Vertical Distance (1/100 meters), size=24 bit. */
    ble_date_time_t                  eta;                                       /**< Estimated Time of Arrival. */
};


/**@brief Function for initializing the Location and Navigation Service.
 *
 * @param[out]    p_lns                  Location and Navigation Service structure. This structure must be supplied by
 *                                       the application. It is initialized by this function, and will later
 *                                       be used to identify this particular service instance.
 * @param[in]     p_lns_init             Information needed to initialize the service.
 *
 * @retval        NRF_SUCCESS            If the service was initialized successfully.
 * @retval        NRF_ERROR_NULL         If a NULL parameter was provided.
 * @retval        NRF_INVALID_PARAMS     If there is an inconsistency in the initialization structure. 
 * @return        Otherwise, an error code from either sd_ble_gatts_service_add() or sd_ble_gatts_characteristic_add() is returned.
 */
uint32_t ble_lns_init(ble_lns_t * p_lns, const ble_lns_init_t * p_lns_init);

/**@brief   Function for handling Location and Navigation Service BLE stack events.
 *
 * @details This function handles all events from the BLE stack that are of interest to the Location and Navigation Service.
 *
 * @note    The function returns when a NULL parameter is provided.
 *
 * @param[in]     p_lns                   Location and Navigation Service structure.
 * @param[in]     p_ble_evt               Event received from the BLE stack.
 *
 */
void ble_lns_on_ble_evt(ble_lns_t * p_lns, ble_evt_t * p_ble_evt);

/**@brief   Function for sending location and speed data if notification has been enabled.
 *
 * @details The application calls this function after having performed a location and speed determination.
 *          If notification has been enabled, the location and speed data is encoded and sent to
 *          the client.
 *
 * @param[in]     p_lns                   Location and Navigation Service structure holding the location and speed data.
 *
 * @retval        NRF_SUCCESS             If the data was sent successfully.
 * @retval        NRF_ERROR_NULL          If a NULL parameter was provided.
 * @retval        NRF_ERROR_INVALID_STATE If notification is disabled.
 * @return        Otherwise, an error code from @ref sd_ble_gatts_hvx() is returned.
 */
uint32_t ble_lns_loc_speed_send(ble_lns_t * p_lns);

/**@brief Function for sending navigation data if notification has been enabled.
 *
 * @details The application calls this function after having performed a navigation determination.
 *          If notification has been enabled, the navigation data is encoded and sent to
 *          the client.
 *
 * @param[in]     p_lns                   Location and Navigation Service structure holding the navigation data.
 *
 * @retval        NRF_SUCCESS             If the data was sent successfully.
 * @retval        NRF_ERROR_NULL          If a NULL parameter was provided.
 * @retval        NRF_ERROR_NOT_SUPPORTED If the navigation characteristic is absent.
 * @retval        NRF_ERROR_INVALID_STATE If navigation is not running or notification is disabled.
 * @return        Otherwise, an error code from @ref sd_ble_gatts_hvx() is returned.
 */
uint32_t ble_lns_navigation_send(ble_lns_t * p_lns);

/**@brief Function for adding a route to the Location and Navigation Service.
 *
 * @param[in]     p_lns                   Location and Navigation Service structure.
 * @param[in,out] p_route                 The new route to be added. The route ID is updated.
 *
 * @retval        NRF_SUCCESS             If the route was added successfully.
 * @retval        NRF_ERROR_NULL          If a NULL parameter was provided.
 * @retval        NRF_ERROR_NOT_SUPPORTED If the navigation characteristic is absent.
 * @retval        NRF_ERROR_NO_MEM        If there is no memory left. 
 * @retval        NRF_ERROR_INTERNAL      If there is an inconsistency in the routes table.
 */
uint32_t ble_lns_add_route(ble_lns_t * p_lns, ble_lns_route_t * p_route);

/**@brief Function for removing a route from the Location and Navigation Service.
 *
 * @param[in]     p_lns                   Location and Navigation Service structure.
 * @param[in]     route_id                The ID of the route to be removed.
 *
 * @retval        NRF_SUCCESS             If the route was removed successfully.
 * @retval        NRF_ERROR_NULL          If a NULL parameter was provided.
 * @retval        NRF_ERROR_NOT_SUPPORTED If the navigation characteristic is absent.
 * @retval        NRF_INVALID_PARAM       If the route ID does not exist.
 */
uint32_t ble_lns_remove_route(ble_lns_t * p_lns, uint16_t route_id);

#endif // BLE_LNS_H__

/** @} */
