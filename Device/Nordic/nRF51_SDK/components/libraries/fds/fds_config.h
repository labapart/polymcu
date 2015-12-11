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

#ifndef FDS_CONFIG_H__
#define FDS_CONFIG_H__

 /**
 * @file fds_config.h
 *
 * @addtogroup flash_data_storage
 * @{
 */

/**@brief Configures the size of the internal queue. */
#define FDS_CMD_QUEUE_SIZE          (8)
/**@brief Determines how many @ref fds_record_chunk_t structures can be buffered at any time. */
#define FDS_CHUNK_QUEUE_SIZE        (8)

/**@brief Configures the number of physical flash pages to use. Out of the total, one is reserved
 *        for garbage collection, hence, two pages is the minimum: one for the application data
 *        and one for the system. */
#define FDS_MAX_PAGES               (2)
/**@brief Configures the maximum number of callbacks which can be registred. */
#define FDS_MAX_USERS               (10)

/** Page tag definitions. */
#define FDS_PAGE_TAG_WORD_0_SWAP    (0xA5A5A5A5)
#define FDS_PAGE_TAG_WORD_0_VALID   (0xA4A4A4A4)
#define FDS_PAGE_TAG_WORD_1         (0xAABBCCDD)
#define FDS_PAGE_TAG_WORD_2         (0xAABB01DD) /**< Includes version. */
#define FDS_PAGE_TAG_WORD_3         (0x1CEB00DA)
#define FDS_PAGE_TAG_WORD_3_GC      (0x1CEB00D8)

/** @} */

#endif // FDS_CONFIG_H__
