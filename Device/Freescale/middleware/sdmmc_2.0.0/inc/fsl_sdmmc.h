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
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _FSL_SDMMC_H_
#define _FSL_SDMMC_H_

#include "fsl_card.h"

/*!
 * @addtogroup SDMMC
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief Reverse byte sequence in uint32_t */
#define SWAP_WORD_BYTE_SEQUENCE(x) (__REV(x))
/*! @brief Reverse byte sequence for each half word in uint32_t */
#define SWAP_HALF_WROD_BYTE_SEQUENCE(x) (__REV16(x))

/*! @brief Maximum loop count to check the card operation voltage range */
#define FSL_SDMMC_MAX_VOLTAGE_RETRIES (1000U)

/*************************************************************************************************
 * API
 ************************************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name SDMMC Function
 * @{
 */

/*!
 * @brief Select the card to put it into transfer state.
 *
 * @param base SDHC peripheral base address.
 * @param transfer SDHC transfer function.
 * @param relativeAddress Relative address.
 * @param isSelected True to put card into transfer state.
 * @retval kStatus_SDMMC_TransferFailed Transfer failed.
 * @retval kStatus_Success Operate successfully.
 */
status_t SDMMC_SelectCard(SDHC_Type *base,
                          sdhc_transfer_function_t transfer,
                          uint32_t relativeAddress,
                          bool isSelected);

/*!
 * @brief Wait write process complete.
 *
 * @param base SDHC peripheral base address.
 * @param transfer SDHC transfer function.
 * @param relativeAddress Card relative address.
 * @retval kStatus_SDMMC_TransferFailed Transfer failed.
 * @retval kStatus_Success Operate successfully.
 */
status_t SDMMC_WaitWriteComplete(SDHC_Type *base, sdhc_transfer_function_t transfer, uint32_t relativeAddress);

/*!
 * @brief Send application command.
 *
 * @param base SDHC peripheral base address.
 * @param transfer SDHC transfer function.
 * @param relativeAddress Card relative address.
 * @retval kStatus_SDMMC_TransferFailed Transfer failed.
 * @retval kStatus_SDMMC_CardNotSupport Card doesn't support.
 * @retval kStatus_Success Operate successfully.
 */
status_t SDMMC_SendApplicationCommand(SDHC_Type *base, sdhc_transfer_function_t transfer, uint32_t relativeAddress);

/*!
 * @brief Set block count.
 *
 * @param base SDHC peripheral base address.
 * @param transfer SDHC transfer function.
 * @param blockCount Block count.
 * @retval kStatus_SDMMC_TransferFailed Transfer failed.
 * @retval kStatus_Success Operate successfully.
 */
status_t SDMMC_SetBlockCount(SDHC_Type *base, sdhc_transfer_function_t transfer, uint32_t blockCount);

/*!
 * @brief Set the card to be idle state.
 *
 * @param base SDHC peripheral base address.
 * @param transfer SDHC transfer function.
 * @retval kStatus_SDMMC_TransferFailed Transfer failed.
 * @retval kStatus_Success Operate successfully.
 */
status_t SDMMC_GoIdle(SDHC_Type *base, sdhc_transfer_function_t transfer);

/*!
 * @brief Stop transfer in multiple block read/write transaction.
 *
 * @param base SDHC peripheral base address.
 * @param transfer SDHC transfer function.
 * @retval kStatus_SDMMC_TransferFailed Transfer failed.
 * @retval kStatus_Success Operate successfully.
 */
status_t SDMMC_StopTransmission(SDHC_Type *base, sdhc_transfer_function_t transfer);

/*!
 * @brief Set data block size.
 *
 * @param base SDHC peripheral base address.
 * @param transfer SDHC transfer function.
 * @param blockSize Block size.
 * @retval kStatus_SDMMC_TransferFailed Transfer failed.
 * @retval kStatus_Success Operate successfully.
 */
status_t SDMMC_SetBlockSize(SDHC_Type *base, sdhc_transfer_function_t transfer, uint32_t blockSize);

/* @} */
#if defined(__cplusplus)
}
#endif
/*! @} */

#endif /* _FSL_SDMMC_H_ */
