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

#ifndef APP_USBD_AUDIO_INTERNAL_H__
#define APP_USBD_AUDIO_INTERNAL_H__

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @defgroup app_usbd_audio_internal USB Audio internals
 * @brief @tagAPI52840 USB Audio class internals.
 * @ingroup app_usbd_audio
 * @{
 */

/**
 * @brief Forward declaration of type defined by @ref APP_USBD_CLASS_TYPEDEF in audio class
 *
 */
APP_USBD_CLASS_FORWARD(app_usbd_audio);

/*lint -save -e165*/
/**
 * @brief Forward declaration of @ref app_usbd_audio_user_event_e
 *
 */
enum app_usbd_audio_user_event_e;

/*lint -restore*/

/**
 * @brief User event handler
 *
 * @param[in] p_inst    Class instance
 * @param[in] event     User event
 *
 * */
typedef void (*app_usbd_audio_user_ev_handler_t)(app_usbd_class_inst_t const *  p_inst,
                                                 enum app_usbd_audio_user_event_e event);

/**
 * @brief Audio class part of class instance data
 */
typedef struct {
    uint8_t const * p_raw_desc;         //!< Audio class descriptors
    size_t          raw_desc_size;      //!< Audio class descriptors size

    app_usbd_audio_user_ev_handler_t user_ev_handler; //!< User event handler
} app_usbd_audio_inst_t;

/**
 * @brief Audio class request target
 */
typedef enum {
    APP_USBD_AUDIO_CLASS_REQ_IN,  /**< Audio class request IN              */
    APP_USBD_AUDIO_CLASS_REQ_OUT, /**< Audio class request OUT             */
    APP_USBD_AUDIO_EP_REQ_IN,     /**< Audio class endpoint request IN     */
    APP_USBD_AUDIO_EP_REQ_OUT,    /**< Audio class endpoint request OUT    */
} app_usbd_audio_class_req_target_t;

/**
 * @brief Audio class specific request handled via control endpoint
 * */
typedef struct {
    app_usbd_audio_class_req_target_t req_target;   //!< Request target
    app_usbd_audio_req_type_t         req_type;     //!< Request type

    uint8_t  control;       //!< Request control field
    uint8_t  channel;       //!< Channel ID
    uint8_t  interface;     //!< Interface ID
    uint8_t  entity;        //!< Entity ID
    uint16_t length;        //!< Request payload length

    uint8_t payload[64];    //!< Request payload
} app_usbd_audio_req_t;


/**
 * @brief Audio class context
 *
 * */
typedef struct {
    app_usbd_audio_req_t    request;    //!< Audio class request

    const void * p_tx_buff;  //!< Transfer buffer (IN endpoint transfers)
    void *       p_rx_buff;  //!< Transfer buffer (OUT endpoint transfers)

    size_t tx_size;          //!< Transfer buffer size (IN endpoint transfers)
    size_t rx_size;          //!< Transfer buffer size (OUT endpoint transfers)
    bool   streaming;        //!< Streaming flag
} app_usbd_audio_ctx_t;


/**
 * @brief Audio class configuration macro
 *
 * Used by @ref APP_USBD_AUDIO_GLOBAL_DEF
 *
 * @param iface_control     Interface number of audio control
 * @param iface_stream      Interface number of audio stream
 * */
#define APP_USBD_AUDIO_CONFIG(iface_control, iface_stream)  \
        ((iface_control),                                   \
         (iface_stream, 0))

/**
 * @brief Only IN audio stream configuration
 *
 * @param iface_control     Interface number of audio control
 * @param iface_stream_in   Interface number of audio stream on IN endpoint
 * */
#define APP_USBD_AUDIO_CONFIG_IN(iface_control, iface_stream_in)  \
        ((iface_control), (iface_stream_in, NRF_DRV_USBD_EPIN8))


/**
 * @brief Only OUT audio stream configuration
 *
 * @param iface_control     Interface number of audio control
 * @param iface_stream_out  Interface number of audio stream on OUT endpoint
 * */
#define APP_USBD_AUDIO_CONFIG_OUT(iface_control, iface_stream_out)  \
        ((iface_control), (iface_stream_out, NRF_DRV_USBD_EPOUT8))

/**
 * @brief Specific class constant data for audio class
 *
 * @ref app_usbd_audio_inst_t
 */
#define APP_USBD_AUDIO_INSTANCE_SPECIFIC_DEC app_usbd_audio_inst_t inst;


/**
 * @brief Configures audio class instance
 *
 * @param descriptors           Mass storage class descriptors (raw table)
 * @param user_event_handler    User event handler
 */
#define APP_USBD_AUDIO_INST_CONFIG(descriptors, user_event_handler) \
    .inst = {                                                       \
         .p_raw_desc = descriptors,                                 \
         .raw_desc_size = sizeof(descriptors),                      \
         .user_ev_handler = user_event_handler,                     \
    }

/**
 * @brief Specific class data for audio class
 *
 * @ref app_usbd_audio_ctx_t
 * */
#define APP_USBD_AUDIO_DATA_SPECIFIC_DEC app_usbd_audio_ctx_t ctx;


/**
 * @brief Audio class descriptors config macro
 *
 * @param interface_number Interface number
 * @param ...              Extracted endpoint list
 * */
#define APP_USBD_AUDIO_DSC_CONFIG(interface_number, ...) {              \
    APP_USBD_AUDIO_INTERFACE_DSC(interface_number,                      \
                                 0,                                     \
                                 0,                                     \
                                 APP_USBD_AUDIO_SUBCLASS_AUDIOCONTROL)  \
    }

/**
 * @brief Public audio class interface
 *
 * */
extern const app_usbd_class_methods_t app_usbd_audio_class_methods;

/**
 * @brief Global definition of @ref app_usbd_audio_t class
 *
 */
#define APP_USBD_AUDIO_GLOBAL_DEF_INTERNAL(instance_name,       \
                                           interfaces_configs,  \
                                           user_ev_handler,     \
                                           raw_descriptors)     \
    APP_USBD_CLASS_INST_GLOBAL_DEF(                             \
        instance_name,                                          \
        app_usbd_audio,                                         \
        &app_usbd_audio_class_methods,                          \
        interfaces_configs,                                     \
        (APP_USBD_AUDIO_INST_CONFIG(raw_descriptors,            \
                                    user_ev_handler))           \
    )


/** @} */


#ifdef __cplusplus
}
#endif

#endif /* APP_USBD_AUDIO_INTERNAL_H__ */
