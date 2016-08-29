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

#include "fsl_dma_manager.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Macro to get the DMAMUX physical channel indicator from the virtual channel indicator. */
#define VIRTUAL_CHN_TO_DMAMUX_CHN(chn) (chn % FSL_FEATURE_DMAMUX_MODULE_CHANNEL)

/*! @brief Macro to get the dma physical module indicator from the virtual channel indicator. */
#define VIRTUAL_CHN_TO_DMAMUX_INSTANCE(chn) (chn / FSL_FEATURE_DMAMUX_MODULE_CHANNEL)

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Variable to store dma manager state. */
static uint32_t s_DMAMGR_Channels;

/*! @brief Array to map DMAMUX instance number to base pointer. */
static DMAMUX_Type *const s_dmamuxBase[] = DMAMUX_BASE_PTRS;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

void DMAMGR_Init(void)
{
    uint32_t i;

    /* Reset the dma manager channels */
    s_DMAMGR_Channels = 0;
    /* Initialize DMAMUX */
    for (i = 0; i < FSL_FEATURE_SOC_DMAMUX_COUNT; i++)
    {
        DMAMUX_Init(s_dmamuxBase[i]);
    }
/* Initialize DMA or EDMA */
#if defined(FSL_FEATURE_SOC_DMA_COUNT) && FSL_FEATURE_SOC_DMA_COUNT > 0U
    DMA_Init(DMA0);
#endif /* FSL_FEATURE_SOC_DMA_COUNT */

#if defined(FSL_FEATURE_SOC_EDMA_COUNT) && FSL_FEATURE_SOC_EDMA_COUNT > 0U
    edma_config_t config;

    EDMA_GetDefaultConfig(&config);
    EDMA_Init(DMA0, &config);
#endif /* FSL_FEATURE_SOC_EDMA_COUNT */
}

void DMAMGR_Deinit(void)
{
    uint32_t i;
    uint32_t j;

    /* Deinitialize DMAMUX */
    for (i = 0; i < FSL_FEATURE_SOC_DMAMUX_COUNT; i++)
    {
        for (j = 0; j < FSL_FEATURE_DMAMUX_MODULE_CHANNEL; j++)
        {
            DMAMUX_DisableChannel(s_dmamuxBase[i], j);
        }
        DMAMUX_Deinit(s_dmamuxBase[i]);
    }
/* Deinitialize DMA or EDMA */
#if defined(FSL_FEATURE_SOC_DMA_COUNT) && FSL_FEATURE_SOC_DMA_COUNT > 0U
    DMA_Deinit(DMA0);
#endif /* FSL_FEATURE_SOC_DMA_COUNT */

#if defined(FSL_FEATURE_SOC_EDMA_COUNT) && FSL_FEATURE_SOC_EDMA_COUNT > 0U
    EDMA_Deinit(DMA0);
#endif /* FSL_FEATURE_SOC_EDMA_COUNT */
}

status_t DMAMGR_RequestChannel(dma_request_source_t requestSource, uint8_t virtualChannel, void *handle)
{
    assert(handle != NULL);

    status_t retval;
    uint32_t primask;

    /* Dynamic channel allocate mechanism */
    if (virtualChannel == DMAMGR_DYNAMIC_ALLOCATE)
    {
#if defined(FSL_FEATURE_EDMA_CHANNEL_GROUP_COUNT) && FSL_FEATURE_EDMA_CHANNEL_GROUP_COUNT > 0U
        /*
            For some chip like MK70F15/MK70F12, it has only 1 DMAMUX(32 channels), but 2 source groups(16 channels per
           group).
        */
        uint32_t dmamux_module_channel = FSL_FEATURE_DMAMUX_MODULE_CHANNEL / FSL_FEATURE_EDMA_CHANNEL_GROUP_COUNT;
#else
        uint32_t dmamux_module_channel = FSL_FEATURE_DMAMUX_MODULE_CHANNEL;
#endif /* FSL_FEATURE_EDMA_CHANNEL_GROUP_COUNT */

        uint32_t i = 0;
        uint32_t j = 0;
        uint32_t map = ((uint32_t)requestSource) >> 8U;
        bool channel_found = false;

        while (map != 0U)
        {
            if (map & (1U << i))
            {
                map &= ~(0x1U << i);
                for (j = i * dmamux_module_channel; j < (i + 1) * dmamux_module_channel; j++)
                {
                    primask = DisableGlobalIRQ();
                    if ((s_DMAMGR_Channels & (1U << j)) == 0U)
                    {
                        /*
                            DMA and EDMA shall be only 1 instance, but DMAMUX may has 1 or 2
                            instances, so the channel number in DMA or EDMA must be converted
                            to channel number in DMAMUX.
                        */
                        uint32_t dmamux_instance = VIRTUAL_CHN_TO_DMAMUX_INSTANCE(j);
                        uint32_t dmamux_channel = VIRTUAL_CHN_TO_DMAMUX_CHN(j);

                        s_DMAMGR_Channels |= (1U << j);
                        EnableGlobalIRQ(primask);
                        /* Configure DMAMUX channel */
                        DMAMUX_SetSource(s_dmamuxBase[dmamux_instance], dmamux_channel, (uint8_t)requestSource);
                        DMAMUX_EnableChannel(s_dmamuxBase[dmamux_instance], dmamux_channel);
/* Creat DMA or EDMA handle */
#if defined(FSL_FEATURE_SOC_DMA_COUNT) && FSL_FEATURE_SOC_DMA_COUNT > 0U
                        DMA_CreateHandle((dma_handle_t *)handle, DMA0, j);
#endif /* FSL_FEATURE_SOC_DMA_COUNT */

#if defined(FSL_FEATURE_SOC_EDMA_COUNT) && FSL_FEATURE_SOC_EDMA_COUNT > 0U
                        EDMA_CreateHandle((edma_handle_t *)handle, DMA0, j);
#endif /* FSL_FEATURE_SOC_EDMA_COUNT */

                        channel_found = true;
                        break;
                    }
                    else /* Channel has been occupied, so continue to search. */
                    {
                        EnableGlobalIRQ(primask);
                        continue;
                    }
                }
            }
            else
            {
                i++;
            }
        }
        if (channel_found)
        {
            retval = kStatus_Success;
        }
        else
        {
            retval = kStatus_DMAMGR_NoFreeChannel;
        }
    }
    else /* Static channel allocate mechanism */
    {
        uint32_t channel = virtualChannel;
        uint32_t dmamux_instance = VIRTUAL_CHN_TO_DMAMUX_INSTANCE(channel);
        uint32_t dmamux_channel = VIRTUAL_CHN_TO_DMAMUX_CHN(channel);
        uint32_t map = ((uint32_t)requestSource) >> 8U;

        /* Check if channel match the request source. */
        if ((map != 0x1U << dmamux_instance) && (map != 0x2U << dmamux_instance))
        {
            retval = kStatus_DMAMGR_ChannelNotMatchSource;
        }
        else
        {
            primask = DisableGlobalIRQ();
            if ((s_DMAMGR_Channels & (1U << channel)) != 0U)
            {
                EnableGlobalIRQ(primask);
                retval = kStatus_DMAMGR_ChannelOccupied;
            }
            else
            {
                s_DMAMGR_Channels |= (1U << channel);
                EnableGlobalIRQ(primask);
                /* Configure DMAMUX channel */
                DMAMUX_SetSource(s_dmamuxBase[dmamux_instance], dmamux_channel, (uint8_t)requestSource);
                DMAMUX_EnableChannel(s_dmamuxBase[dmamux_instance], dmamux_channel);
/* Creat DMA or EDMA handle */
#if defined(FSL_FEATURE_SOC_DMA_COUNT) && FSL_FEATURE_SOC_DMA_COUNT > 0U
                DMA_CreateHandle((dma_handle_t *)handle, DMA0, channel);
#endif /* FSL_FEATURE_SOC_DMA_COUNT */

#if defined(FSL_FEATURE_SOC_EDMA_COUNT) && FSL_FEATURE_SOC_EDMA_COUNT > 0U
                EDMA_CreateHandle((edma_handle_t *)handle, DMA0, channel);
#endif /* FSL_FEATURE_SOC_EDMA_COUNT */

                retval = kStatus_Success;
            }
        }
    }

    return retval;
}

status_t DMAMGR_ReleaseChannel(void *handle)
{
    assert(handle != NULL);

#if defined(FSL_FEATURE_SOC_DMA_COUNT) && FSL_FEATURE_SOC_DMA_COUNT > 0U
    uint32_t channel = ((dma_handle_t *)handle)->channel;
#endif /* FSL_FEATURE_SOC_DMA_COUNT */

#if defined(FSL_FEATURE_SOC_EDMA_COUNT) && FSL_FEATURE_SOC_EDMA_COUNT > 0U
    uint32_t channel = ((edma_handle_t *)handle)->channel;
#endif /* FSL_FEATURE_SOC_EDMA_COUNT */

    uint32_t dmamux_instance = VIRTUAL_CHN_TO_DMAMUX_INSTANCE(channel);
    uint32_t dmamux_channel = VIRTUAL_CHN_TO_DMAMUX_CHN(channel);

    /* Check if channel is used */
    if ((s_DMAMGR_Channels & (1U << channel)) == 0U)
    {
        return kStatus_DMAMGR_ChannelNotUsed;
    }
    s_DMAMGR_Channels &= ~(1U << channel);
    DMAMUX_DisableChannel(s_dmamuxBase[dmamux_instance], dmamux_channel);

    return kStatus_Success;
}
