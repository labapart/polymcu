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

#ifndef ES_SECURITY_TIMING_H__
#define ES_SECURITY_TIMING_H__

#include <stdint.h>

/**
 * @file
 * @addtogroup eddystone_security
 * @{
 */

/**@brief Function for getting the number of seconds passed since the last invocation.
 * @details If the function returns zero, the 'last time called' state is not updated. If a non-zero value
 *       is returned, the 'last time called' state will point to the last whole second.
 * @return    Number of seconds passed since the last invocation.
 */
uint32_t es_security_timing_seconds_passed(void);


/**@brief Function for initializing the security timing module.
 */
void es_security_timing_init(void);

/**
 * @}
 */

#endif // ES_SECURITY_TIMING_H__
