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
#ifndef SER_DBG_SD_STR_H
#define SER_DBG_SD_STR_H
#include <stdint.h>
const char * ser_dbg_sd_call_str_get(uint8_t opcode);
const char * ser_dbg_sd_evt_str_get(uint16_t opcode);
#endif //SER_DBG_SD_STR_H
