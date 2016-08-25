/*
 * @brief Common IAP support functions
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2013
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licenser disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#ifndef __IAP_H_
#define __IAP_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup COMMON_IAP CHIP: Common Chip ISP/IAP commands and return codes
 * @ingroup CHIP_Common
 * @{
 */

/* IAP command definitions */
#define IAP_PREWRRITE_CMD           50	/*!< Prepare sector for write operation command */
#define IAP_WRISECTOR_CMD           51	/*!< Write Sector command */
#define IAP_ERSSECTOR_CMD           52	/*!< Erase Sector command */
#define IAP_BLANK_CHECK_SECTOR_CMD  53	/*!< Blank check sector */
#define IAP_REPID_CMD               54	/*!< Read PartID command */
#define IAP_READ_BOOT_CODE_CMD      55	/*!< Read Boot code version */
#define IAP_COMPARE_CMD             56	/*!< Compare two RAM address locations */
#define IAP_REINVOKE_ISP_CMD        57	/*!< Reinvoke ISP */
#define IAP_READ_UID_CMD            58	/*!< Read UID */
#define IAP_ERASE_PAGE_CMD          59	/*!< Erase page */
#define IAP_EEPROM_WRITE            61	/*!< EEPROM Write command */
#define IAP_EEPROM_READ             62	/*!< EEPROM READ command */

/* IAP response definitions */
#define IAP_CMD_SUCCESS             0	/*!< Command is executed successfully */
#define IAP_INVALID_COMMAND         1	/*!< Invalid command */
#define IAP_SRC_ADDR_ERROR          2	/*!< Source address is not on word boundary */
#define IAP_DST_ADDR_ERROR          3	/*!< Destination address is not on a correct boundary */
#define IAP_SRC_ADDR_NOT_MAPPED     4	/*!< Source address is not mapped in the memory map */
#define IAP_DST_ADDR_NOT_MAPPED     5	/*!< Destination address is not mapped in the memory map */
#define IAP_COUNT_ERROR             6	/*!< Byte count is not multiple of 4 or is not a permitted value */
#define IAP_INVALID_SECTOR          7	/*!< Sector number is invalid or end sector number is greater than start sector number */
#define IAP_SECTOR_NOT_BLANK        8	/*!< Sector is not blank */
#define IAP_SECTOR_NOT_PREPARED     9	/*!< Command to prepare sector for write operation was not executed */
#define IAP_COMPARE_ERROR           10	/*!< Source and destination data not equal */
#define IAP_BUSY                    11	/*!< Flash programming hardware interface is busy */
#define IAP_PARAM_ERROR             12	/*!< nsufficient number of parameters or invalid parameter */
#define IAP_ADDR_ERROR              13	/*!< Address is not on word boundary */
#define IAP_ADDR_NOT_MAPPED         14	/*!< Address is not mapped in the memory map */
#define IAP_CMD_LOCKED              15	/*!< Command is locked */
#define IAP_INVALID_CODE            16	/*!< Unlock code is invalid */
#define IAP_INVALID_BAUD_RATE       17	/*!< Invalid baud rate setting */
#define IAP_INVALID_STOP_BIT        18	/*!< Invalid stop bit setting */
#define IAP_CRP_ENABLED             19	/*!< Code read protection enabled */

/* IAP_ENTRY API function type */
typedef void (*IAP_ENTRY_T)(unsigned int[5], unsigned int[4]);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __IAP_H_ */
