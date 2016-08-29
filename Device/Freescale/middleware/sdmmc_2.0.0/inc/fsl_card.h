/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _FSL_CARD_H_
#define _FSL_CARD_H_

#include "fsl_sdhc.h"
#include "fsl_specification.h"

/*!
 * @addtogroup CARD
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief Driver version. */
#define FSL_SDMMC_DRIVER_VERSION (MAKE_VERSION(2U, 0U, 0U)) /*2.0.0*/

/*! @brief Default block size */
#define FSL_SDMMC_DEFAULT_BLOCK_SIZE (512U)

/*! @brief SD/MMC card API's running status. */
enum _sdmmc_status
{
    kStatus_SDMMC_NotSupportYet = MAKE_STATUS(kStatusGroup_SDMMC, 0U),             /*!< Haven't supported */
    kStatus_SDMMC_TransferFailed = MAKE_STATUS(kStatusGroup_SDMMC, 1U),            /*!< Send command failed */
    kStatus_SDMMC_SetCardBlockSizeFailed = MAKE_STATUS(kStatusGroup_SDMMC, 2U),    /*!< Set block size failed */
    kStatus_SDMMC_HostNotSupport = MAKE_STATUS(kStatusGroup_SDMMC, 3U),            /*!< Host doesn't support */
    kStatus_SDMMC_CardNotSupport = MAKE_STATUS(kStatusGroup_SDMMC, 4U),            /*!< Card doesn't support */
    kStatus_SDMMC_AllSendCidFailed = MAKE_STATUS(kStatusGroup_SDMMC, 5U),          /*!< Send CID failed */
    kStatus_SDMMC_SendRelativeAddressFailed = MAKE_STATUS(kStatusGroup_SDMMC, 6U), /*!< Send relative address failed */
    kStatus_SDMMC_SendCsdFailed = MAKE_STATUS(kStatusGroup_SDMMC, 7U),             /*!< Send CSD failed */
    kStatus_SDMMC_SelectCardFailed = MAKE_STATUS(kStatusGroup_SDMMC, 8U),          /*!< Select card failed */
    kStatus_SDMMC_SendScrFailed = MAKE_STATUS(kStatusGroup_SDMMC, 9U),             /*!< Send SCR failed */
    kStatus_SDMMC_SetDataBusWidthFailed = MAKE_STATUS(kStatusGroup_SDMMC, 10U),    /*!< Set bus width failed */
    kStatus_SDMMC_GoIdleFailed = MAKE_STATUS(kStatusGroup_SDMMC, 11U),             /*!< Go idle failed */
    kStatus_SDMMC_HandShakeOperationConditionFailed =
        MAKE_STATUS(kStatusGroup_SDMMC, 12U), /*!< Send Operation Condition failed */
    kStatus_SDMMC_SendApplicationCommandFailed =
        MAKE_STATUS(kStatusGroup_SDMMC, 13U),                                    /*!< Send application command failed */
    kStatus_SDMMC_SwitchFailed = MAKE_STATUS(kStatusGroup_SDMMC, 14U),           /*!< Switch command failed */
    kStatus_SDMMC_StopTransmissionFailed = MAKE_STATUS(kStatusGroup_SDMMC, 15U), /*!< Stop transmission failed */
    kStatus_SDMMC_WaitWriteCompleteFailed = MAKE_STATUS(kStatusGroup_SDMMC, 16U),    /*!< Wait write complete failed */
    kStatus_SDMMC_SetBlockCountFailed = MAKE_STATUS(kStatusGroup_SDMMC, 17U),        /*!< Set block count failed */
    kStatus_SDMMC_SetRelativeAddressFailed = MAKE_STATUS(kStatusGroup_SDMMC, 18U),   /*!< Set relative address failed */
    kStatus_SDMMC_SwitchHighSpeedFailed = MAKE_STATUS(kStatusGroup_SDMMC, 19U),      /*!< Switch high speed failed */
    kStatus_SDMMC_SendExtendedCsdFailed = MAKE_STATUS(kStatusGroup_SDMMC, 20U),      /*!< Send EXT_CSD failed */
    kStatus_SDMMC_ConfigureBootFailed = MAKE_STATUS(kStatusGroup_SDMMC, 21U),        /*!< Configure boot failed */
    kStatus_SDMMC_ConfigureExtendedCsdFailed = MAKE_STATUS(kStatusGroup_SDMMC, 22U), /*!< Configure EXT_CSD failed */
    kStatus_SDMMC_EnableHighCapacityEraseFailed =
        MAKE_STATUS(kStatusGroup_SDMMC, 23U), /*!< Enable high capacity erase failed */
    kStatus_SDMMC_SendTestPatternFailed = MAKE_STATUS(kStatusGroup_SDMMC, 24U),    /*!< Send test pattern failed */
    kStatus_SDMMC_ReceiveTestPatternFailed = MAKE_STATUS(kStatusGroup_SDMMC, 25U), /*!< Receive test pattern failed */
};

/*! @brief SD card flags */
typedef enum _sd_card_flag
{
    kSD_SupportHighCapacityFlag = (1U << 1U), /*!< Support high capacity */
    kSD_Support4BitWidthFlag = (1U << 2U),    /*!< Support 4-bit data width */
    kSD_SupportSdhcFlag = (1U << 3U),         /*!< Card is SDHC */
    kSD_SupportSdxcFlag = (1U << 4U),         /*!< Card is SDXC */
} sd_card_flag_t;

/*! @brief MMC card flags */
typedef enum _mmc_card_flag
{
    kMMC_SupportHighSpeedFlag = (1U << 0U),      /*!< Support high speed */
    kMMC_SupportHighSpeed52MHZFlag = (1U << 1U), /*!< Support high speed 52MHZ */
    kMMC_SupportHighSpeed26MHZFlag = (1U << 2U), /*!< Support high speed 26MHZ */
    kMMC_SupportHighCapacityFlag = (1U << 3U),   /*!< Support high capacity */
    kMMC_SupportAlternateBootFlag = (1U << 4U),  /*!< Support alternate boot */
} mmc_card_flag_t;

/*!
 * @brief SD card state
 *
 * Define the card structure including the necessary fields to identify and describe the card.
 */
typedef struct _sd_card
{
    sdhc_host_t host; /*!< Host information */

    uint32_t busClock_Hz;     /*!< SD bus clock frequency united in Hz */
    uint32_t relativeAddress; /*!< Relative address of the card */
    uint32_t version;         /*!< Card version */
    uint32_t flags;           /*!< Flags in sd_card_flag_t */
    uint32_t rawCid[4U];      /*!< Raw CID content */
    uint32_t rawCsd[4U];      /*!< Raw CSD content */
    uint32_t rawScr[2U];      /*!< Raw CSD content */
    uint32_t ocr;             /*!< Raw OCR content */
    sd_cid_t cid;             /*!< CID */
    sd_csd_t csd;             /*!< CSD */
    sd_scr_t scr;             /*!< SCR */
    uint32_t blockCount;      /*!< Card total block number */
    uint32_t blockSize;       /*!< Card block size */
} sd_card_t;

/*!
 * @brief SD card state
 *
 * Define the card structure including the necessary fields to identify and describe the card.
 */
typedef struct _mmc_card
{
    sdhc_host_t host; /*!< Host information */

    uint32_t busClock_Hz;                                 /*!< MMC bus clock united in Hz */
    uint32_t relativeAddress;                             /*!< Relative address of the card */
    bool enablePreDefinedBlockCount;                      /*!< Enable PRE-DEFINED block count when read/write */
    uint32_t flags;                                       /*!< Capability flag in mmc_card_flag_t */
    uint32_t rawCid[4U];                                  /*!< Raw CID content */
    uint32_t rawCsd[4U];                                  /*!< Raw CSD content */
    uint32_t rawExtendedCsd[MMC_EXTENDED_CSD_BYTES / 4U]; /*!< Raw MMC Extended CSD content */
    uint32_t ocr;                                         /*!< Raw OCR content */
    mmc_cid_t cid;                                        /*!< CID */
    mmc_csd_t csd;                                        /*!< CSD */
    mmc_extended_csd_t extendedCsd;                       /*!< Extended CSD */
    uint32_t blockSize;                                   /*!< Card block size */
    uint32_t userPartitionBlocks;                         /*!< Card total block number in user partition */
    uint32_t bootPartitionBlocks;                         /*!< Boot partition size united as block size */
    uint32_t eraseGroupBlocks;                            /*!< Erase group size united as block size */
    mmc_access_partition_t currentPartition;              /*!< Current access partition */
    mmc_voltage_window_t hostVoltageWindow;               /*!< Host voltage window */
} mmc_card_t;

/*! @brief MMC card boot configuration definition. */
typedef struct _mmc_boot_config
{
    bool enableBootAck;                        /*!< Enable boot ACK */
    mmc_boot_partition_enable_t bootPartition; /*!< Boot partition */
    bool retainBootBusWidth;                   /*!< If retain boot bus width */
    mmc_data_bus_width_t bootDataBusWidth;     /*!< Boot data bus width */
} mmc_boot_config_t;

/*************************************************************************************************
 * API
 ************************************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name SDCARD Function
 * @{
 */

/*!
 * @brief Initialize the card on a specific host controller.
 *
 * This function initializes the card on a specific host controller.
 *
 * @param card The pointer to save card related information.
 * @retval kStatus_SDMMC_GoIdleFailed Go idle failed.
 * @retval kStatus_SDMMC_NotSupportYet Card not support.
 * @retval kStatus_SDMMC_SendOperationConditionFailed Send operation condition failed.
 * @retval kStatus_SDMMC_AllSendCidFailed Send CID failed.
 * @retval kStatus_SDMMC_SendRelativeAddressFailed Send relative address failed.
 * @retval kStatus_SDMMC_SendCsdFailed Send CSD failed.
 * @retval kStatus_SDMMC_SelectCardFailed Send SELECT_CARD command failed.
 * @retval kStatus_SDMMC_SendScrFailed Send SCR failed.
 * @retval kStatus_SDMMC_SetBusWidthFailed Set bus width failed.
 * @retval kStatus_SDMMC_SwitchHighSpeedFailed Switch high speed failed.
 * @retval kStatus_SDMMC_SetCardBlockSizeFailed Set card block size failed.
 * @retval kStatus_Success Operate successfully.
 */
status_t SD_Init(sd_card_t *card);

/*!
 * @brief Deinitialize the card.
 *
 * This function deinitializes the specific card.
 *
 * @param card The specific card.
 */
void SD_Deinit(sd_card_t *card);

/*!
 * @brief Check whether the card is write-protected.
 *
 * This function checks if the card is write-protected via CSD register.
 *
 * @param card The specific card.
 * @retval true Card is read only.
 * @retval false Card isn't read only.
 */
bool SD_CheckReadOnly(sd_card_t *card);

/*!
 * @brief Read blocks from the specific card.
 *
 * This function reads blocks from specific card, with default block size defined by SDHC_CARD_DEFAULT_BLOCK_SIZE.
 *
 * @param card The card descriptor.
 * @param buffer The buffer to save the data read from card.
 * @param startBlock The start block index.
 * @param blockCount The number of blocks to read.
 * @retval kStatus_InvalidArgument Invalid argument.
 * @retval kStatus_SDMMC_CardNotSupport Card not support.
 * @retval kStatus_SDMMC_NotSupportYet Not support now.
 * @retval kStatus_SDMMC_WaitWriteCompleteFailed Send status failed.
 * @retval kStatus_SDMMC_TransferFailed Transfer failed.
 * @retval kStatus_SDMMC_StopTransmissionFailed Stop transmission failed.
 * @retval kStatus_Success Operate successfully.
 */
status_t SD_ReadBlocks(sd_card_t *card, uint8_t *buffer, uint32_t startBlock, uint32_t blockCount);

/*!
 * @brief Write blocks of data to the specific card.
 *
 * This function writes blocks to specific card, with default block size 512 bytes.
 *
 * @param card The card descriptor.
 * @param buffer The buffer holding the data to be written to the card.
 * @param startBlock The start block index.
 * @param blockCount The number of blocks to write.
 * @retval kStatus_InvalidArgument Invalid argument.
 * @retval kStatus_SDMMC_NotSupportYet Not support now.
 * @retval kStatus_SDMMC_CardNotSupport Card not support.
 * @retval kStatus_SDMMC_WaitWriteCompleteFailed Send status failed.
 * @retval kStatus_SDMMC_TransferFailed Transfer failed.
 * @retval kStatus_SDMMC_StopTransmissionFailed Stop transmission failed.
 * @retval kStatus_Success Operate successfully.
 */
status_t SD_WriteBlocks(sd_card_t *card, const uint8_t *buffer, uint32_t startBlock, uint32_t blockCount);

/*!
 * @brief Erase blocks of the specific card.
 *
 * This function erases blocks of a specific card, with default block size 512 bytes.
 *
 * @param card The card descriptor.
 * @param startBlock The start block index.
 * @param blockCount The number of blocks to erase.
 * @retval kStatus_InvalidArgument Invalid argument.
 * @retval kStatus_SDMMC_WaitWriteCompleteFailed Send status failed.
 * @retval kStatus_SDMMC_TransferFailed Transfer failed.
 * @retval kStatus_SDMMC_WaitWriteCompleteFailed Send status failed.
 * @retval kStatus_Success Operate successfully.
 */
status_t SD_EraseBlocks(sd_card_t *card, uint32_t startBlock, uint32_t blockCount);

/* @} */

/*!
 * @name MMCCARD Function
 * @{
 */

/*!
 * @brief Initialize the MMC card.
 *
 * @param card Card descriptor.
 * @retval kStatus_SDMMC_GoIdleFailed Go idle failed.
 * @retval kStatus_SDMMC_SendOperationConditionFailed Send operation condition failed.
 * @retval kStatus_SDMMC_AllSendCidFailed Send CID failed.
 * @retval kStatus_SDMMC_SetRelativeAddressFailed Set relative address failed.
 * @retval kStatus_SDMMC_SendCsdFailed Send CSD failed.
 * @retval kStatus_SDMMC_CardNotSupport Card not support.
 * @retval kStatus_SDMMC_SelectCardFailed Send SELECT_CARD command failed.
 * @retval kStatus_SDMMC_SendExtendedCsdFailed Send EXT_CSD failed.
 * @retval kStatus_SDMMC_SetBusWidthFailed Set bus width failed.
 * @retval kStatus_SDMMC_SwitchHighSpeedFailed Switch high speed failed.
 * @retval kStatus_SDMMC_SetCardBlockSizeFailed Set card block size failed.
 * @retval kStatus_Success Operate successfully.
 */
status_t MMC_Init(mmc_card_t *card);

/*!
 * @brief Deinitialize the card.
 *
 * @param card Card descriptor.
 */

void MMC_Deinit(mmc_card_t *card);

/*!
 * @brief Check if the card is read only.
 *
 * @param card Card descriptor.
 * @retval true Card is read only.
 * @retval false Card isn't read only.
 */
bool MMC_CheckReadOnly(mmc_card_t *card);

/*!
 * @brief Read data blocks from the card.
 *
 * @param card Card descriptor.
 * @param buffer The buffer to save data.
 * @param startBlock The start block index.
 * @param blockCount The number of blocks to read.
 * @retval kStatus_InvalidArgument Invalid argument.
 * @retval kStatus_SDMMC_CardNotSupport Card not support.
 * @retval kStatus_SDMMC_SetBlockCountFailed Set block count failed.
 * @retval kStatus_SDMMC_TransferFailed Transfer failed.
 * @retval kStatus_SDMMC_StopTransmissionFailed Stop transmission failed.
 * @retval kStatus_Success Operate successfully.
 */
status_t MMC_ReadBlocks(mmc_card_t *card, uint8_t *buffer, uint32_t startBlock, uint32_t blockCount);

/*!
 * @brief Write data blocks to the card.
 *
 * @param card Card descriptor.
 * @param buffer The buffer to save data blocks.
 * @param startBlock Start block number to write.
 * @param blockCount Block count.
 * @retval kStatus_InvalidArgument Invalid argument.
 * @retval kStatus_SDMMC_NotSupportYet Not support now.
 * @retval kStatus_SDMMC_SetBlockCountFailed Set block count failed.
 * @retval kStatus_SDMMC_WaitWriteCompleteFailed Send status failed.
 * @retval kStatus_SDMMC_TransferFailed Transfer failed.
 * @retval kStatus_SDMMC_StopTransmissionFailed Stop transmission failed.
 * @retval kStatus_Success Operate successfully.
 */
status_t MMC_WriteBlocks(mmc_card_t *card, const uint8_t *buffer, uint32_t startBlock, uint32_t blockCount);

/*!
 * @brief Erase groups of the card.
 *
 * Erase group is the smallest erase unit in MMC card. The erase range is [startGroup, endGroup].
 *
 * @param  card Card descriptor.
 * @param  startGroup Start group number.
 * @param  endGroup End group number.
 * @retval kStatus_InvalidArgument Invalid argument.
 * @retval kStatus_SDMMC_WaitWriteCompleteFailed Send status failed.
 * @retval kStatus_SDMMC_TransferFailed Transfer failed.
 * @retval kStatus_Success Operate successfully.
 */
status_t MMC_EraseGroups(mmc_card_t *card, uint32_t startGroup, uint32_t endGroup);

/*!
 * @brief Select the partition to access.
 *
 * @param card Card descriptor.
 * @param partitionNumber The partition number.
 * @retval kStatus_SDMMC_ConfigureExtendedCsdFailed Configure EXT_CSD failed.
 * @retval kStatus_Success Operate successfully.
 */
status_t MMC_SelectPartition(mmc_card_t *card, mmc_access_partition_t partitionNumber);

/*!
 * @brief Configure boot activity of the card.
 *
 * @param card Card descriptor.
 * @param config Boot configuration structure.
 * @retval kStatus_SDMMC_NotSupportYet Not support now.
 * @retval kStatus_SDMMC_ConfigureExtendedCsdFailed Configure EXT_CSD failed.
 * @retval kStatus_SDMMC_ConfigureBootFailed Configure boot failed.
 * @retval kStatus_Success Operate successfully.
 */
status_t MMC_SetBootConfig(mmc_card_t *card, const mmc_boot_config_t *config);

/* @} */
#if defined(__cplusplus)
}
#endif
/*! @} */
#endif /* _FSL_CARD_H_*/
