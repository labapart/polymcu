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

#include "fsl_specification.h"
#include "fsl_card.h"

/*******************************************************************************
 * Code
 ******************************************************************************/
status_t SDMMC_SelectCard(SDHC_Type *base, sdhc_transfer_function_t transfer, uint32_t relativeAddress, bool isSelected)
{
    assert(transfer);

    sdhc_transfer_t content = {0};
    sdhc_command_t command = {0};

    command.index = kSDMMC_SelectCard;
    if (isSelected)
    {
        command.argument = relativeAddress << 16U;
        command.responseType = kSDHC_ResponseTypeR1;
    }
    else
    {
        command.argument = 0U;
        command.responseType = kSDHC_ResponseTypeNone;
    }

    content.command = &command;
    content.data = NULL;
    if ((kStatus_Success != transfer(base, &content)) || (command.response[0U] & kSDMMC_R1ErrorAllFlag))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    /* Wait until card to transfer state */
    return kStatus_Success;
}

status_t SDMMC_WaitWriteComplete(SDHC_Type *base, sdhc_transfer_function_t transfer, uint32_t relativeAddress)
{
    assert(transfer);

    sdhc_transfer_t content = {0};
    sdhc_command_t command = {0};

    command.index = kSDMMC_SendStatus;
    command.argument = relativeAddress << 16U;
    command.responseType = kSDHC_ResponseTypeR1;

    do
    {
        content.command = &command;
        content.data = 0U;
        if ((kStatus_Success != transfer(base, &content)) || (command.response[0U] & kSDMMC_R1ErrorAllFlag))
        {
            return kStatus_SDMMC_TransferFailed;
        }

        if ((command.response[0U] & kSDMMC_R1ReadyForDataFlag) &&
            (SDMMC_R1_CURRENT_STATE(command.response[0U]) != kSDMMC_R1StateProgram))
        {
            break;
        }
    } while (true);

    return kStatus_Success;
}

status_t SDMMC_SendApplicationCommand(SDHC_Type *base, sdhc_transfer_function_t transfer, uint32_t relativeAddress)
{
    assert(transfer);

    sdhc_transfer_t content = {0};
    sdhc_command_t command = {0};

    command.index = kSDMMC_ApplicationCommand;
    command.argument = 0U;
    command.argument = (relativeAddress << 16U);
    command.responseType = kSDHC_ResponseTypeR1;

    content.command = &command;
    content.data = 0U;
    if ((kStatus_Success != transfer(base, &content)) || (command.response[0U] & kSDMMC_R1ErrorAllFlag))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    if (!(command.response[0U] & kSDMMC_R1ApplicationCommandFlag))
    {
        return kStatus_SDMMC_CardNotSupport;
    }

    return kStatus_Success;
}

status_t SDMMC_SetBlockCount(SDHC_Type *base, sdhc_transfer_function_t transfer, uint32_t blockCount)
{
    assert(transfer);

    sdhc_transfer_t content = {0};
    sdhc_command_t command = {0};

    command.index = kSDMMC_SetBlockCount;
    command.argument = blockCount;
    command.responseType = kSDHC_ResponseTypeR1;

    content.command = &command;
    content.data = 0U;
    if ((kStatus_Success != transfer(base, &content)) || (command.response[0U] & kSDMMC_R1ErrorAllFlag))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    return kStatus_Success;
}

status_t SDMMC_GoIdle(SDHC_Type *base, sdhc_transfer_function_t transfer)
{
    assert(transfer);

    sdhc_transfer_t content = {0};
    sdhc_command_t command = {0};

    command.index = kSDMMC_GoIdleState;

    content.command = &command;
    content.data = 0U;
    if (kStatus_Success != transfer(base, &content))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    return kStatus_Success;
}

status_t SDMMC_StopTransmission(SDHC_Type *base, sdhc_transfer_function_t transfer)
{
    assert(transfer);

    sdhc_transfer_t content = {0};
    sdhc_command_t command = {0};

    command.index = kSDMMC_StopTransmission;
    command.argument = 0U;
    command.type = kSDHC_CommandTypeAbort;
    command.responseType = kSDHC_ResponseTypeR1b;

    content.command = &command;
    content.data = 0U;
    if ((kStatus_Success != transfer(base, &content)) || (command.response[0U] & kSDMMC_R1ErrorAllFlag))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    return kStatus_Success;
}

status_t SDMMC_SetBlockSize(SDHC_Type *base, sdhc_transfer_function_t transfer, uint32_t blockSize)
{
    assert(transfer);

    sdhc_transfer_t content = {0};
    sdhc_command_t command = {0};

    command.index = kSDMMC_SetBlockLength;
    command.argument = blockSize;
    command.responseType = kSDHC_ResponseTypeR1;

    content.command = &command;
    content.data = 0U;
    if ((kStatus_Success != transfer(base, &content)) || (command.response[0U] & kSDMMC_R1ErrorAllFlag))
    {
        return kStatus_SDMMC_TransferFailed;
    }

    return kStatus_Success;
}
