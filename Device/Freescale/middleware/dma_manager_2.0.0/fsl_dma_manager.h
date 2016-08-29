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

#ifndef _FSL_DMAMGR_H_
#define _FSL_DMAMGR_H_

#include "fsl_common.h"
#include "fsl_dmamux.h"
#if defined(FSL_FEATURE_SOC_DMA_COUNT) && FSL_FEATURE_SOC_DMA_COUNT > 0U
#include "fsl_dma.h"
#endif /* FSL_FEATURE_SOC_DMA_COUNT */
#if defined(FSL_FEATURE_SOC_EDMA_COUNT) && FSL_FEATURE_SOC_EDMA_COUNT > 0U
#include "fsl_edma.h"
#endif /* FSL_FEATURE_SOC_EDMA_COUNT */

/*!
 * @addtogroup dmamgr
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!< @brief Dynamic channel allocate mechanism. */
#define DMAMGR_DYNAMIC_ALLOCATE 0xFFU

/*! @brief DMA manager status. */
enum _dma_manager_status
{
    kStatus_DMAMGR_ChannelOccupied = MAKE_STATUS(kStatusGroup_DMAMGR, 0),       /*!< Channel has been occupied */
    kStatus_DMAMGR_ChannelNotUsed = MAKE_STATUS(kStatusGroup_DMAMGR, 1),        /*!< Channel has not been used */
    kStatus_DMAMGR_NoFreeChannel = MAKE_STATUS(kStatusGroup_DMAMGR, 2),         /*!< All channel has been occupied */
    kStatus_DMAMGR_ChannelNotMatchSource = MAKE_STATUS(kStatusGroup_DMAMGR, 3), /*!< Channel do not match the
                                                                                     request source */
};

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*!
 * @name DMAMGR Initialize and De-initialize
 * @{
 */

/*!
 * @brief Initializes the DAM manager.
 *
 * This function initializes the DMA manager, ungates all DMAMUX clocks, and
 * initializes the eDMA or DMA peripheral.
 */
void DMAMGR_Init(void);

/*!
 * @brief Deinitializes the DMA manager.
 *
 * This function deinitializes the DMA manager, disables all DMAMUX channel,
 * gates all DMAMUX clock, and deinitializes the eDMA or DMA peripheral.
 */
void DMAMGR_Deinit(void);

/* @} */
/*!
 * @name DMAMGR Operation
 * @{
 */

/*!
 * @brief Requests a DMA channel.
 *
 * This function request a DMA channel which is not occupied. There are two channels to allocate the mechanism dynamic
 * and static. For the dynamic allocation mechanism (virtualChannel = DMAMGR_DYNAMIC_ALLOCATE), DMAMGR  allocates a DMA
 * channel according to the given request source and then configure it. For static allocation mechanism, DMAMGR
 * configures the given channel according to the given request source and channel number.
 *
 * @param requestSource DMA channel request source number. See the soc.h.
 * @param virtualChannel The channel number user wants to occupy. If using the dynamic channel allocate mechanism, set the
 *                       virtualChannel equal to DMAMGR_DYNAMIC_ALLOCATE.
 * @param handle DMA or EDMA handle pointer.
 * @retval kStatus_Success In dynamic/static channel allocate mechanism, allocate DMAMUX channel successfully.
 * @retval kStatus_DMAMGR_NoFreeChannel In dynamic channel allocate mechanism, all DMAMUX channels has been occupied.
 * @retval kStatus_DMAMGR_ChannelNotMatchSource In static channel allocate mechanism, the given channel do not match
                                                the given request.
 * @retval kStatus_DMAMGR_ChannelOccupied In static channel allocate mechanism, the given channel has been occupied.
 */
status_t DMAMGR_RequestChannel(dma_request_source_t requestSource, uint8_t virtualChannel, void *handle);

/*!
 * @brief Releases a DMA channel.
 *
 * This function releases an occupied DMA channel.
 *
 * @param handle DMA or eDMA handle pointer.
 * @retval kStatus_Success Release the given channel successfully.
 * @retval kStatus_DMAMGR_ChannelNotUsed The given channel which to be released is not been used before.
 */
status_t DMAMGR_ReleaseChannel(void *handle);

/* @} */

#if defined(__cplusplus)
}
#endif /* __cplusplus */

/* @} */

#endif /* _FSL_DMAMGR_H_ */
