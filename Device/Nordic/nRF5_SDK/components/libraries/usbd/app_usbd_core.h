/* Copyright (c) Nordic Semiconductor. All Rights Reserved.
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
#ifndef APP_USBD_CORE_H__
#define APP_USBD_CORE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "sdk_common.h"
#include "nrf_drv_usbd.h"
#include "app_usbd_types.h"
#include "app_usbd_class_base.h"


/**
 * @defgroup app_usbd_core USB Device high level library core module
 * @ingroup app_usbd
 *
 * @brief @tagAPI52840 Core module that manages current USB state and process device requests.
 * @{
 */

/**
 * @brief USB Device state
 *
 * Possible USB Device states according to specification.
 */
typedef enum
{
    APP_USBD_STATE_Disabled   = 0x00, /**< The whole USBD library is disabled */
    APP_USBD_STATE_Unattached = 0x01, /**< Device is currently not connected to the host */
    APP_USBD_STATE_Powered    = 0x02, /**< Device is connected to the host but has not been enumerated */
    APP_USBD_STATE_Default    = 0x03, /**< USB Reset condition detected, waiting for the address */
    APP_USBD_STATE_Addressed  = 0x04, /**< Device has been addressed but have not been configured */
    APP_USBD_STATE_Configured = 0x05, /**< Device is addressed and configured */

    APP_USBD_STATE_SuspendedMask       = 0x10, /**< Mask used to mark suspended state, suspended state remembers also the target state after wake up */

    APP_USBD_STATE_SuspendedPowered    = APP_USBD_STATE_SuspendedMask | APP_USBD_STATE_Powered,    /**< Device is Suspended and on wakeup it will return to Powered state */
    APP_USBD_STATE_SuspendedDefault    = APP_USBD_STATE_SuspendedMask | APP_USBD_STATE_Default,    /**< Device is Suspended and on wakeup it will return to Default state */
    APP_USBD_STATE_SuspendedAddressed  = APP_USBD_STATE_SuspendedMask | APP_USBD_STATE_Addressed,  /**< Device is Suspended and on wakeup it will return to Addressed state */
    APP_USBD_STATE_SuspendedConfigured = APP_USBD_STATE_SuspendedMask | APP_USBD_STATE_Configured  /**< Device is Suspended and on wakeup it will return to Configured state */
}app_usbd_state_t;

/**
 * @brief Remove @ref APP_USBD_STATE_SuspendedMask bit from core state
 */
#define APP_USB_STATE_BASE(state) ((state) & (~APP_USBD_STATE_SuspendedMask))

/**
 * @brief Core interface configuration
 *
 * Core instance would have 2 endpoints (IN0 and OUT0).
 * The interface number does not care because it would not be used.
 */
#define APP_USBD_CORE_CLASS_CONFIGURATION ((0, NRF_DRV_USBD_EPOUT0, NRF_DRV_USBD_EPIN0))

/**
 * @brief EP0 handler function pointer
 *
 * Type of the variable that would hold the pointer to the handler for
 * endpoint 0 messages processing.
 *
 * @param p_contex Context variable configured with the transmission request.
 */
typedef ret_code_t (*app_usbd_core_setup_data_handler_t)(nrf_drv_usbd_ep_status_t status,
                                                         void * p_context);

/**
 * @brief Variable type used to register EP0 transfer handler
 *
 * EP0 messages are processed by core instance.
 * Another class can register itself to receive messages from EP0 when requesting
 * for Setup data transfer.
 */
typedef struct
{
    app_usbd_core_setup_data_handler_t   handler;   //!< Event handler to be called when transmission is ready
    void                               * p_context; //!< Context pointer to be send to every called event.
} app_usbd_core_setup_data_handler_desc_t;

/*lint -save -e10 -e26 -e93 -e123 -e505 */
/**
 * @brief Declare Core instance type
 *
 * USBD core instance type definition.
 */
APP_USBD_CLASS_TYPEDEF(app_usbd_core,
        APP_USBD_CORE_CLASS_CONFIGURATION,
        APP_USBD_CLASS_INSTANCE_SPECIFIC_DEC_NONE,
        APP_USBD_CLASS_DATA_SPECIFIC_DEC_NONE);
/*lint -restore*/

/**
 * @brief Access to core instance
 *
 * Function that returns pointer to the USBD core instance.
 *
 * @return pointer to the core instance
 */
static inline app_usbd_class_inst_t const * app_usbd_core_instance_access(void)
{
    extern const APP_USBD_CLASS_INSTANCE_TYPE(app_usbd_core) app_usbd_core_inst;
    return (app_usbd_class_inst_t const *)&app_usbd_core_inst;
}

/**
 * @brief Default simple response to setup command
 *
 * This function generates default simple response.
 * It sends ZLP when required and on takes care on allowing status stage when
 * transfer is finished.
 *
 * @param p_setup Pointer to original setup message
 * @param p_data  Pointer to the response. This has to be globaly aviable data.
 * @param size    Total size of the answer - The function takes care about
 *                limiting the size of transfered data to the size required
 *                by setup command.
 */
ret_code_t app_usbd_core_setup_rsp(app_usbd_setup_t const * p_setup,
                                   void const *             p_data,
                                   size_t                   size);

/**
 * @brief Configure the handler for the nearest setup data endpoint transfer
 *
 * This function would be called on incomming setup data.
 * The correct place to set the handler for a data is when SETUP command
 * was received.
 *
 * @param ep             Endpoint number (only IN0 and OUT0) are supported.
 * @param p_handler_desc Descriptor of the handler to be called.
 *
 * @retval NRF_SUCCESS            Successfully configured
 * @retval NRF_ERROR_INVALID_ADDR Last received setup direction does not match
 *                                configured endpoint.
 */
ret_code_t app_usbd_core_setup_data_handler_set(
    nrf_drv_usbd_ep_t ep,
    app_usbd_core_setup_data_handler_desc_t const * const p_handler_desc);

/**
 * @brief Endpoint transfer
 *
 * @param ep         Endpoint number
 * @param p_transfer Description of the transfer to be performed.
 *                   The direction of the transfer is determined by the
 *                   endpoint number.
 * @param p_handler  The transfer handler - function that is called when
 *                   transfer is going to end or overload the buffer.
 *                   This function can prepare another buffer.
 *                   Can be NULL if no such functionality is expected.
 *
 * @retval NRF_ERROR_INVALID_STATE The state of the USB device does not allow
 *                                 data transfer on the endpoint.
 *
 * @return Values returned by @ref nrf_drv_usbd_ep_transfer
 *
 * @note This function can work only if the USB is in Configured state.
 *       See @ref app_usbd_core_setup_data_transfer for transfers before
 *       device configuration is done.
 * @sa app_usbd_core_setup_data_transfer
 */
ret_code_t app_usbd_core_ep_transfer(
    nrf_drv_usbd_ep_t                                  ep,
    nrf_drv_usbd_transfer_t              const * const p_transfer,
    nrf_drv_usbd_transfer_handler_desc_t const * const p_handler);

/**
 * @brief Setup data transfer
 *
 * Function similar to @ref app_usbd_core_ep_transfer.
 * The only technical diference is that it should be used with setup transfers
 * that are going to be performed before device is configured.
 *
 * @param ep         See @ref app_usbd_core_ep_transfer.
 * @param p_transfer See @ref app_usbd_core_ep_transfer.
 * @param p_handler  See @ref app_usbd_core_ep_transfer.
 *
 * @return The same values like @ref app_usbd_core_ep_transfer
 */
ret_code_t app_usbd_core_setup_data_transfer(
    nrf_drv_usbd_ep_t                                  ep,
    nrf_drv_usbd_transfer_t              const * const p_transfer,
    nrf_drv_usbd_transfer_handler_desc_t const * const p_handler);

/**
 * @brief Setup data transfer buffer
 *
 * Returns special internal buffer that can be used in setup transfer.
 * @return Internal buffer pointer
 */
void * app_usbd_core_setup_transfer_buff_get(size_t * p_size);


/**@brief Return internal USBD core state
 *
 * @return Check @ref app_usbd_state_t to find possible USBD core states
 */
app_usbd_state_t app_usbd_core_state_get(void);


/**
 * @brief Check current USBD regulator status
 *
 * @return true when regulator is ready
 */
static inline bool app_usbd_core_power_regulator_is_ready(void)
{
    return 0 != ( (NRF_POWER->USBREGSTATUS) & POWER_USBREGSTATUS_OUTPUTRDY_Msk);
}

/**
 * @brief Register class on remote wake-up feature
 * @param[in] p_inst Instance of the class
 */
void app_usbd_core_class_rwu_register(app_usbd_class_inst_t const * const p_inst);

/**
 * @brief Unregister class from remote wake-up feature
 * @param[in] p_inst Instance of the class
 */
void app_usbd_core_class_rwu_unregister(app_usbd_class_inst_t const * const p_inst);

/**
 * @brief Set remote wake-up pending
 * */
void app_usbd_core_class_rwu_pend(void);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* APP_USBD_CORE_H__ */
