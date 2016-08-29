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

#ifndef _FSL_SPECIFICATION_H_
#define _FSL_SPECIFICATION_H_

#include <stdint.h>

/*!
 * @addtogroup Card Specification
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief SD/MMC card initialization clock frequency */
#define SDMMC_CLOCK_400KHZ (400000U)
/*! @brief SD card bus frequency 1 in high speed mode */
#define SD_CLOCK_25MHZ (25000000U)
/*! @brief SD card bus frequency 2 in high speed mode */
#define SD_CLOCK_50MHZ (50000000U)
/*! @brief MMC card bus frequency 1 in high speed mode */
#define MMC_CLOCK_26MHZ (26000000U)
/*! @brief MMC card bus frequency 2 in high speed mode */
#define MMC_CLOCK_52MHZ (52000000U)

/*! @brief Card status bit in R1 */
typedef enum _sdmmc_r1_card_status_flag
{
    kSDMMC_R1OutOfRangeFlag = (1U << 31U),                 /*!< Out of range status bit */
    kSDMMC_R1AddressErrorFlag = (1U << 30U),               /*!< Address error status bit */
    kSDMMC_R1BlockLengthErrorFlag = (1U << 29U),           /*!< Block length error status bit */
    kSDMMC_R1EraseSequenceErrorFlag = (1U << 28U),         /*!< Erase sequence error status bit */
    kSDMMC_R1EraseParameterErrorFlag = (1U << 27U),        /*!< Erase parameter error status bit */
    kSDMMC_R1WriteProtectViolationFlag = (1U << 26U),      /*!< Write protection violation status bit */
    kSDMMC_R1CardIsLockedFlag = (1U << 25U),               /*!< Card locked status bit */
    kSDMMC_R1LockUnlockFailedFlag = (1U << 24U),           /*!< lock/unlock error status bit */
    kSDMMC_R1CommandCrcErrorFlag = (1U << 23U),            /*!< CRC error status bit */
    kSDMMC_R1IllegalCommandFlag = (1U << 22U),             /*!< Illegal command status bit */
    kSDMMC_R1CardEccFailedFlag = (1U << 21U),              /*!< Card ecc error status bit */
    kSDMMC_R1CardControllerErrorFlag = (1U << 20U),        /*!< Internal card controller error status bit */
    kSDMMC_R1ErrorFlag = (1U << 19U),                      /*!< A general or an unknown error status bit */
    kSDMMC_R1CidCsdOverwriteFlag = (1U << 16U),            /*!< Cid/csd overwrite status bit */
    kSDMMC_R1WriteProtectEraseSkipFlag = (1U << 15U),      /*!< Write protection erase skip status bit */
    kSDMMC_R1CardEccDisabledFlag = (1U << 14U),            /*!< Card ecc disabled status bit */
    kSDMMC_R1EraseResetFlag = (1U << 13U),                 /*!< Erase reset status bit */
    kSDMMC_R1ReadyForDataFlag = (1U << 8U),                /*!< Ready for data status bit */
    kSDMMC_R1SwitchErrorFlag = (1U << 7U),                 /*!< Switch error status bit */
    kSDMMC_R1ApplicationCommandFlag = (1U << 5U),          /*!< Application command enabled status bit */
    kSDMMC_R1AuthenticationSequenceErrorFlag = (1U << 3U), /*!< error in the sequence of authentication process */

    kSDMMC_R1ErrorAllFlag =
        (kSDMMC_R1OutOfRangeFlag | kSDMMC_R1AddressErrorFlag | kSDMMC_R1BlockLengthErrorFlag |
         kSDMMC_R1EraseSequenceErrorFlag | kSDMMC_R1EraseParameterErrorFlag | kSDMMC_R1WriteProtectViolationFlag |
         kSDMMC_R1CardIsLockedFlag | kSDMMC_R1LockUnlockFailedFlag | kSDMMC_R1CommandCrcErrorFlag |
         kSDMMC_R1IllegalCommandFlag | kSDMMC_R1CardEccFailedFlag | kSDMMC_R1CardControllerErrorFlag |
         kSDMMC_R1ErrorFlag | kSDMMC_R1CidCsdOverwriteFlag |
         kSDMMC_R1AuthenticationSequenceErrorFlag), /*!< Card error status */
} sdmmc_r1_card_status_flag_t;

/*! @brief R1: current state */
#define SDMMC_R1_CURRENT_STATE(x) (((x)&0x00001E00U) >> 9U)

/*! @brief CURRENT_STATE filed in R1 */
typedef enum _sdmmc_r1_current_state
{
    kSDMMC_R1StateIdle = 0U,        /*!< R1: current state: idle */
    kSDMMC_R1StateReady = 1U,       /*!< R1: current state: ready */
    kSDMMC_R1StateIdentify = 2U,    /*!< R1: current state: identification */
    kSDMMC_R1StateStandby = 3U,     /*!< R1: current state: standby */
    kSDMMC_R1StateTransfer = 4U,    /*!< R1: current state: transfer */
    kSDMMC_R1StateSendData = 5U,    /*!< R1: current state: sending data */
    kSDMMC_R1StateReceiveData = 6U, /*!< R1: current state: receiving data */
    kSDMMC_R1StateProgram = 7U,     /*!< R1: current state: programming */
    kSDMMC_R1StateDisconnect = 8U,  /*!< R1: current state: disconnect */
} sdmmc_r1_current_state_t;

/*! @brief Error bit in SPI mode R1 */
typedef enum _sdspi_r1_error_status_flag
{
    kSDSPI_R1InIdleStateFlag = (1U << 0U),        /*!< In idle state */
    kSDSPI_R1EraseResetFlag = (1U << 1U),         /*!< Erase reset */
    kSDSPI_R1IllegalCommandFlag = (1U << 2U),     /*!< Illegal command */
    kSDSPI_R1CommandCrcErrorFlag = (1U << 3U),    /*!< Com crc error */
    kSDSPI_R1EraseSequenceErrorFlag = (1U << 4U), /*!< Erase sequence error */
    kSDSPI_R1AddressErrorFlag = (1U << 5U),       /*!< Address error */
    kSDSPI_R1ParameterErrorFlag = (1U << 6U),     /*!< Parameter error */
} sdspi_r1_error_status_flag_t;

/*! @brief Error bit in SPI mode R2 */
typedef enum _sdspi_r2_error_status_flag
{
    kSDSPI_R2CardLockedFlag = (1U << 0U),            /*!< Card is locked */
    kSDSPI_R2WriteProtectEraseSkip = (1U << 1U),     /*!< Write protect erase skip */
    kSDSPI_R2LockUnlockFailed = (1U << 1U),          /*!< Lock/unlock command failed */
    kSDSPI_R2ErrorFlag = (1U << 2U),                 /*!< Unknown error */
    kSDSPI_R2CardControllerErrorFlag = (1U << 3U),   /*!< Card controller error */
    kSDSPI_R2CardEccFailedFlag = (1U << 4U),         /*!< Card ecc failed */
    kSDSPI_R2WriteProtectViolationFlag = (1U << 5U), /*!< Write protect violation */
    kSDSPI_R2EraseParameterErrorFlag = (1U << 6U),   /*!< Erase parameter error */
    kSDSPI_R2OutOfRangeFlag = (1U << 7U),            /*!< Out of range */
    kSDSPI_R2CsdOverwriteFlag = (1U << 7U),          /*!< CSD overwrite */
} sdspi_r2_error_status_flag_t;

/*! @brief The bit mask for COMMAND VERSION field in R7 */
#define SDSPI_R7_VERSION_SHIFT (28U)
/*! @brief The bit mask for COMMAND VERSION field in R7 */
#define SDSPI_R7_VERSION_MASK (0xFU)
/*! @brief The bit shift for VOLTAGE ACCEPTED field in R7 */
#define SDSPI_R7_VOLTAGE_SHIFT (8U)
/*! @brief The bit mask for VOLTAGE ACCEPTED field in R7 */
#define SDSPI_R7_VOLTAGE_MASK (0xFU)
/*! @brief The bit mask for VOLTAGE 2.7V to 3.6V field in R7 */
#define SDSPI_R7_VOLTAGE_27_36_MASK (0x1U << SDSPI_R7_VOLTAGE_SHIFT)
/*! @brief The bit shift for ECHO field in R7 */
#define SDSPI_R7_ECHO_SHIFT (0U)
/*! @brief The bit mask for ECHO field in R7 */
#define SDSPI_R7_ECHO_MASK (0xFFU)

/*! @brief Data error token mask */
#define SDSPI_DATA_ERROR_TOKEN_MASK (0xFU)
/*! @brief Data Error Token mask bit */
typedef enum _sdspi_data_error_token
{
    kSDSPI_DataErrorTokenError = (1U << 0U),               /*!< Data error */
    kSDSPI_DataErrorTokenCardControllerError = (1U << 1U), /*!< Card controller error */
    kSDSPI_DataErrorTokenCardEccFailed = (1U << 2U),       /*!< Card ecc error */
    kSDSPI_DataErrorTokenOutOfRange = (1U << 3U),          /*!< Out of range */
} sdspi_data_error_token_t;

/*! @brief Data Token */
typedef enum _sdspi_data_token
{
    kSDSPI_DataTokenBlockRead = 0xFEU,          /*!< Single block read, multiple block read */
    kSDSPI_DataTokenSingleBlockWrite = 0xFEU,   /*!< Single block write */
    kSDSPI_DataTokenMultipleBlockWrite = 0xFCU, /*!< Multiple block write */
    kSDSPI_DataTokenStopTransfer = 0xFDU,       /*!< Stop transmission */
} sdspi_data_token_t;

/* Data Response Token mask */
#define SDSPI_DATA_RESPONSE_TOKEN_MASK (0x1FU) /*!< Mask for data response bits */
/*! @brief Data Response Token */
typedef enum _sdspi_data_response_token
{
    kSDSPI_DataResponseTokenAccepted = 0x05U,   /*!< Data accepted */
    kSDSPI_DataResponseTokenCrcError = 0x0BU,   /*!< Data rejected due to CRC error */
    kSDSPI_DataResponseTokenWriteError = 0x0DU, /*!< Data rejected due to write error */
} sdspi_data_response_token_t;

/*! @brief SD card individual commands */
typedef enum _sd_command
{
    kSD_SendRelativeAddress = 3U,    /*!< Send Relative Address */
    kSD_Switch = 6U,                 /*!< Switch Function */
    kSD_SendInterfaceCondition = 8U, /*!< Send Interface Condition */
    kSD_VoltageSwitch = 11U,         /*!< Voltage Switch */
    kSD_SpeedClassControl = 20U,     /*!< Speed Class control */
    kSD_EraseWriteBlockStart = 32U,  /*!< Write Block Start */
    kSD_EraseWriteBlockEnd = 33U,    /*!< Write Block End */
} sd_command_t;

/*! @brief SD card individual application commands */
typedef enum _sd_application_command
{
    kSD_ApplicationSetBusWdith = 6U,              /*!< Set Bus Width */
    kSD_ApplicationStatus = 13U,                  /*!< Send SD status */
    kSD_ApplicationSendNumberWriteBlocks = 22U,   /*!< Send Number Of Written Blocks */
    kSD_ApplicationSetWriteBlockEraseCount = 23U, /*!< Set Write Block Erase Count */
    kSD_ApplicationSendOperationCondition = 41U,  /*!< Send Operation Condition */
    kSD_ApplicationSetClearCardDetect = 42U,      /*!< Set Connnect/Disconnect pull up on detect pin */
    kSD_ApplicationSendScr = 51U,                 /*!< Send Scr */
} sd_application_command_t;

/*! @brief SD/MMC card common commands */
typedef enum _sdmmc_command
{
    kSDMMC_GoIdleState = 0U,         /*!< Go Idle State */
    kSDMMC_AllSendCid = 2U,          /*!< All Send CID */
    kSDMMC_SetDsr = 4U,              /*!< Set DSR */
    kSDMMC_SelectCard = 7U,          /*!< Select Card */
    kSDMMC_SendCsd = 9U,             /*!< Send CSD */
    kSDMMC_SendCid = 10U,            /*!< Send CID */
    kSDMMC_StopTransmission = 12U,   /*!< Stop Transmission */
    kSDMMC_SendStatus = 13U,         /*!< Send Status */
    kSDMMC_GoInactiveState = 15U,    /*!< Go Inactive State */
    kSDMMC_SetBlockLength = 16U,     /*!< Set Block Length */
    kSDMMC_ReadSingleBlock = 17U,    /*!< Read Single Block */
    kSDMMC_ReadMultipleBlock = 18U,  /*!< Read Multiple Block */
    kSDMMC_SendTuningBlock = 19U,    /*!< Send Tuning Block */
    kSDMMC_SetBlockCount = 23U,      /*!< Set Block Count */
    kSDMMC_WriteSingleBlock = 24U,   /*!< Write Single Block */
    kSDMMC_WriteMultipleBlock = 25U, /*!< Write Multiple Block */
    kSDMMC_ProgramCsd = 27U,         /*!< Program CSD */
    kSDMMC_SetWriteProtect = 28U,    /*!< Set Write Protect */
    kSDMMC_ClearWriteProtect = 29U,  /*!< Clear Write Protect */
    kSDMMC_SendWriteProtect = 30U,   /*!< Send Write Protect */
    kSDMMC_Erase = 38U,              /*!< Erase */
    kSDMMC_LockUnlock = 42U,         /*!< Lock Unlock */
    kSDMMC_ApplicationCommand = 55U, /*!< Send Application Command */
    kSDMMC_GeneralCommand = 56U,     /*!< General Purpose Command */
    kSDMMC_ReadOcr = 58U,            /*!< Read OCR */
} sdmmc_command_t;

/*! @brief MMC card individual commands */
typedef enum _mmc_command
{
    kMMC_SendOperationCondition = 1U, /*!< Send Operation Condition */
    kMMC_SetRelativeAddress = 3U,     /*!< Set Relative Address */
    kMMC_SleepAwake = 5U,             /*!< Sleep Awake */
    kMMC_Switch = 6U,                 /*!< Switch */
    kMMC_SendExtendedCsd = 8U,        /*!< Send EXT_CSD */
    kMMC_ReadDataUntilStop = 11U,     /*!< Read Data Until Stop */
    kMMC_BusTestRead = 14U,           /*!< Test Read */
    kMMC_WriteDataUntilStop = 20U,    /*!< Write Data Until Stop */
    kMMC_ProgramCid = 26U,            /*!< Program CID */
    kMMC_EraseGroupStart = 35U,       /*!< Erase Group Start */
    kMMC_EraseGroupEnd = 36U,         /*!< Erase Group End */
    kMMC_FastInputOutput = 39U,       /*!< Fast IO */
    kMMC_GoInterruptState = 40U,      /*!< Go interrupt State */
} mmc_command_t;

/*! @brief SD card command class */
typedef enum _sdmmc_command_class
{
    kSDMMC_CommandClassBasic = (1U << 0U),               /*!< Card command class 0 */
    kSDMMC_CommandClassBlockRead = (1U << 2U),           /*!< Card command class 2 */
    kSDMMC_CommandClassBlockWrite = (1U << 4U),          /*!< Card command class 4 */
    kSDMMC_CommandClassErase = (1U << 5U),               /*!< Card command class 5 */
    kSDMMC_CommandClassWriteProtect = (1U << 6U),        /*!< Card command class 6 */
    kSDMMC_CommandClassLockCard = (1U << 7U),            /*!< Card command class 7 */
    kSDMMC_CommandClassApplicationSpecific = (1U << 8U), /*!< Card command class 8 */
    kSDMMC_CommandClassInputOutputMode = (1U << 9U),     /*!< Card command class 9 */
    kSDMMC_CommandClassSwitch = (1U << 10U),             /*!< Card command class 10 */
} sdmmc_command_class_t;

/*! @brief OCR register in SD card */
typedef enum _sd_ocr_flag
{
    kSD_OcrPowerUpBusyFlag = (1U << 31U),                            /*!< Power up busy status */
    kSD_OcrHostCapacitySupportFlag = (1U << 30U),                    /*!< Card capacity status */
    kSD_OcrCardCapacitySupportFlag = kSD_OcrHostCapacitySupportFlag, /*!< Card capacity status */
    kSD_OcrSwitch18RequestFlag = (1U << 24U),                        /*!< Switch to 1.8V request */
    kSD_OcrSwitch18AcceptFlag = kSD_OcrSwitch18RequestFlag,          /*!< Switch to 1.8V accepted */
    kSD_OcrVdd27_28Flag = (1U << 15U),                               /*!< VDD 2.7-2.8 */
    kSD_OcrVdd28_29Flag = (1U << 16U),                               /*!< VDD 2.8-2.9 */
    kSD_OcrVdd29_30Flag = (1U << 17U),                               /*!< VDD 2.9-3.0 */
    kSD_OcrVdd30_31Flag = (1U << 18U),                               /*!< VDD 2.9-3.0 */
    kSD_OcrVdd31_32Flag = (1U << 19U),                               /*!< VDD 3.0-3.1 */
    kSD_OcrVdd32_33Flag = (1U << 20U),                               /*!< VDD 3.1-3.2 */
    kSD_OcrVdd33_34Flag = (1U << 21U),                               /*!< VDD 3.2-3.3 */
    kSD_OcrVdd34_35Flag = (1U << 22U),                               /*!< VDD 3.3-3.4 */
    kSD_OcrVdd35_36Flag = (1U << 23U),                               /*!< VDD 3.4-3.5 */
} sd_ocr_flag_t;

/*! @brief SD card specification version number */
typedef enum _sd_specification_version
{
    kSD_SpecificationVersion1_0 = (1U << 0U), /*!< SD card version 1.0-1.01 */
    kSD_SpecificationVersion1_1 = (1U << 1U), /*!< SD card version 1.10 */
    kSD_SpecificationVersion2_0 = (1U << 2U), /*!< SD card version 2.00 */
    kSD_SpecificationVersion3_0 = (1U << 3U), /*!< SD card version 3.0 */
} sd_specification_version_t;

/*! @brief SD card bus width */
typedef enum _sd_data_bus_width
{
    kSD_DataBusWidth1Bit = (1U << 0U), /*!< SD data bus width 1-bit mode */
    kSD_DataBusWidth4Bit = (1U << 2U), /*!< SD data bus width 4-bit mode */
} sd_data_bus_width_t;

/*! @brief SD card switch mode */
typedef enum _sd_switch_mode
{
    kSD_SwitchCheck = 0U, /*!< SD switch mode 0: check function */
    kSD_SwitchSet = 1U,   /*!< SD switch mode 1: set function */
} sd_switch_mode_t;

/*! @brief SD card CSD register flags */
typedef enum _sd_csd_flag
{
    kSD_CsdReadBlockPartialFlag = (1U << 0U),         /*!< Partial blocks for read allowed [79:79] */
    kSD_CsdWriteBlockMisalignFlag = (1U << 1U),       /*!< Write block misalignment [78:78] */
    kSD_CsdReadBlockMisalignFlag = (1U << 2U),        /*!< Read block misalignment [77:77] */
    kSD_CsdDsrImplementedFlag = (1U << 3U),           /*!< DSR implemented [76:76] */
    kSD_CsdEraseBlockEnabledFlag = (1U << 4U),        /*!< Erase single block enabled [46:46] */
    kSD_CsdWriteProtectGroupEnabledFlag = (1U << 5U), /*!< Write protect group enabled [31:31] */
    kSD_CsdWriteBlockPartialFlag = (1U << 6U),        /*!< Partial blocks for write allowed [21:21] */
    kSD_CsdFileFormatGroupFlag = (1U << 7U),          /*!< File format group [15:15] */
    kSD_CsdCopyFlag = (1U << 8U),                     /*!< Copy flag [14:14] */
    kSD_CsdPermanentWriteProtectFlag = (1U << 9U),    /*!< Permanent write protection [13:13] */
    kSD_CsdTemporaryWriteProtectFlag = (1U << 10U),   /*!< Temporary write protection [12:12] */
} sd_csd_flag_t;

/*! @brief SD card SCR register flags */
typedef enum _sd_scr_flag
{
    kSD_ScrDataStatusAfterErase = (1U << 0U), /*!< Data status after erases [55:55] */
    kSD_ScrSdSpecification3 = (1U << 1U),     /*!< Specification version 3.00 or higher [47:47]*/
} sd_scr_flag_t;

/*! @brief MMC card classified as voltage range */
typedef enum _mmc_classified_voltage
{
    kMMC_ClassifiedVoltageHigh = 0U, /*!< High voltage MMC card */
    kMMC_ClassifiedVoltageDual = 1U, /*!< Dual voltage MMC card */
} mmc_classified_voltage_t;

/*! @brief MMC card classified as density level */
typedef enum _mmc_classified_density
{
    kMMC_ClassifiedDensityWithin2GB = 0U, /*!< Density byte is less than or equal 2GB */
    kMMC_ClassifiedDensityHigher2GB = 1U, /* Density byte is higher than 2GB */
} mmc_classified_density_t;

/*! @brief The bit mask for VOLTAGE WINDOW 1.70V to 1.95V field in OCR */
#define MMC_OCR_V170TO195_SHIFT (7U)
/*! @brief The bit mask for VOLTAGE WINDOW 1.70V to 1.95V field in OCR */
#define MMC_OCR_V170TO195_MASK (0x00000080U)
/*! @brief The bit shift for VOLTAGE WINDOW 2.00V to 2.60V field in OCR */
#define MMC_OCR_V200TO260_SHIFT (8U)
/*! @brief The bit mask for VOLTAGE WINDOW 2.00V to 2.60V field in OCR */
#define MMC_OCR_V200TO260_MASK (0x00007F00U)
/*! @brief The bit shift for VOLTAGE WINDOW 2.70V to 3.60V field in OCR */
#define MMC_OCR_V270TO360_SHIFT (15U)
/*! @brief The bit mask for VOLTAGE WINDOW 2.70V to 3.60V field in OCR */
#define MMC_OCR_V270TO360_MASK (0x00FF8000U)
/*! @brief The bit shift for ACCESS MODE field in OCR */
#define MMC_OCR_ACCESS_MODE_SHIFT (29U)
/*! @brief The bit mask for ACCESS MODE field in OCR */
#define MMC_OCR_ACCESS_MODE_MASK (0x60000000U)
/*! @brief The bit shift for BUSY field in OCR */
#define MMC_OCR_BUSY_SHIFT (31U)
/*! @brief The bit mask for BUSY field in OCR */
#define MMC_OCR_BUSY_MASK (1U << MMC_OCR_BUSY_SHIFT)

/*! @brief MMC card access mode(Access mode in OCR). */
typedef enum _mmc_access_mode
{
    kMMC_AccessModeByte = 0U,   /*!< The card should be accessed as byte */
    kMMC_AccessModeSector = 2U, /*!< The card should be accessed as sector */
} mmc_access_mode_t;

/*! @brief MMC card voltage window(VDD voltage window in OCR). */
typedef enum _mmc_voltage_window
{
    kMMC_VoltageWindow170to195 = 0x01U,   /*!< Voltage window is 1.70V to 1.95V */
    kMMC_VoltageWindows270to360 = 0x1FFU, /*!< Voltage window is 2.70V to 3.60V */
} mmc_voltage_window_t;

/*! @brief CSD structure version(CSD_STRUCTURE in CSD). */
typedef enum _mmc_csd_structure_version
{
    kMMC_CsdStrucureVersion10 = 0U,       /*!< CSD version No. 1.0 */
    kMMC_CsdStrucureVersion11 = 1U,       /*!< CSD version No. 1.1 */
    kMMC_CsdStrucureVersion12 = 2U,       /*!< CSD version No. 1.2 */
    kMMC_CsdStrucureVersionInExtcsd = 3U, /*!< Version coded in Extended CSD */
} mmc_csd_structure_version_t;

/*! @brief MMC card specification version(SPEC_VERS in CSD). */
typedef enum _mmc_specification_version
{
    kMMC_SpecificationVersion0 = 0U, /*!< Allocated by MMCA */
    kMMC_SpecificationVersion1 = 1U, /*!< Allocated by MMCA */
    kMMC_SpecificationVersion2 = 2U, /*!< Allocated by MMCA */
    kMMC_SpecificationVersion3 = 3U, /*!< Allocated by MMCA */
    kMMC_SpecificationVersion4 = 4U, /*!< Version 4.1/4.2/4.3 */
} mmc_specification_version_t;

/*! @brief The bit shift for FREQUENCY UNIT field in TRANSFER SPEED(TRAN-SPEED in Extended CSD) */
#define MMC_TRANSFER_SPEED_FREQUENCY_UNIT_SHIFT (0U)
/*! @brief The bit mask for FRQEUENCY UNIT in TRANSFER SPEED */
#define MMC_TRANSFER_SPEED_FREQUENCY_UNIT_MASK (0x07U)
/*! @brief The bit shift for MULTIPLIER field in TRANSFER SPEED */
#define MMC_TRANSFER_SPEED_MULTIPLIER_SHIFT (3U)
/*! @brief The bit mask for MULTIPLIER field in TRANSFER SPEED  */
#define MMC_TRANSFER_SPEED_MULTIPLIER_MASK (0x78U)

/*! @brief Read the value of FREQUENCY UNIT in TRANSFER SPEED. */
#define READ_MMC_TRANSFER_SPEED_FREQUENCY_UNIT(CSD) \
    (((CSD.transferSpeed) & MMC_TRANSFER_SPEED_FREQUENCY_UNIT_MASK) >> MMC_TRANSFER_SPEED_FREQUENCY_UNIT_SHIFT)
/*! @brief Read the value of MULTIPLER filed in TRANSFER SPEED. */
#define READ_MMC_TRANSFER_SPEED_MULTIPLIER(CSD) \
    (((CSD.transferSpeed) & MMC_TRANSFER_SPEED_MULTIPLIER_MASK) >> MMC_TRANSFER_SPEED_MULTIPLIER_SHIFT)

/*! @brief MMC card Extended CSD fix version(EXT_CSD_REV in Extended CSD) */
typedef enum _mmc_extended_csd_revision
{
    kMMC_ExtendedCsdRevision10 = 0U, /*!< Revision 1.0 */
    kMMC_ExtendedCsdRevision11 = 1U, /*!< Revision 1.1 */
    kMMC_ExtendedCsdRevision12 = 2U, /*!< Revision 1.2 */
    kMMC_ExtendedCsdRevision13 = 3U, /*!< Revision 1.3 */
} mmc_extended_csd_revision_t;

/*! @brief MMC card command set(COMMAND_SET in Extended CSD) */
typedef enum _mmc_command_set
{
    kMMC_CommandSetStandard = 0U, /*!< Standard MMC */
    kMMC_CommandSet1 = 1U,        /*!< Command set 1 */
    kMMC_CommandSet2 = 2U,        /*!< Command set 2 */
    kMMC_CommandSet3 = 3U,        /*!< Command set 3 */
    kMMC_CommandSet4 = 4U,        /*!< Command set 4 */
} mmc_command_set_t;

/*! @brief Alternate boot support(BOOT_INFO in Extended CSD) */
#define MMC_ALTERNATE_BOOT_SUPPORT_MASK (0x01)

/*! @brief The power class value bit mask when bus in 4 bit mode */
#define MMC_POWER_CLASS_4BIT_MASK (0x0FU)
/*! @brief The power class current value bit mask when bus in 8 bit mode */
#define MMC_POWER_CLASS_8BIT_MASK (0xF0U)

/*! @brief MMC card high speed timing(HS_TIMING in Extended CSD) */
typedef enum _mmc_high_speed_timing
{
    kMMC_HighSpeedTimingNone = 0U, /*!< MMC card using none high speed timing */
    kMMC_HighSpeedTiming = 1U,     /*!< MMC card using high speed timing */
} mmc_high_speed_timing_t;

/*! @brief MMC card high speed frequency type(CARD_TYPE in Extended CSD) */
typedef enum _mmc_high_speed_frequency
{
    kMMC_HighSpeedFrequency26MHZ = 0x1U, /*!< The bit mask for high speed frequency at 26MHZ flag */
    kMMC_HighSpeedFrequency52MHZ = 0x2U, /*!< The bit mask for high speed frequency at 52MHZ flag */
} mmc_high_speed_frequency_t;

/*! @brief The number of data bus width type */
#define MMC_DATA_BUS_WIDTH_TYPE_NUMBER (3U)
/*! @brief MMC card data bus width(BUS_WIDTH in Extended CSD) */
typedef enum _mmc_data_bus_width
{
    kMMC_DataBusWidth1bit = 0U, /*!< MMC data bus width is 1 bit */
    kMMC_DataBusWidth4bit = 1U, /*!< MMC data bus width is 4 bits */
    kMMC_DataBusWidth8bit = 2U, /*!< MMC data bus width is 8 bits */
} mmc_data_bus_width_t;

/*! @brief MMC card boot partition enabled(BOOT_PARTITION_ENABLE in Extended CSD) */
typedef enum _mmc_boot_partition_enable
{
    kMMC_BootPartitionEnableNot = 0U,        /*!< Device not boot enabled (default) */
    kMMC_BootPartitionEnablePartition1 = 1U, /*!< Boot partition 1 enabled for boot */
    kMMC_BootPartitionEnablePartition2 = 2U, /*!< Boot partition 2 enabled for boot */
    kMMC_BootPartitionEnableUserAera = 7U,   /*!< User area enabled for boot */
} mmc_boot_partition_enable_t;

/*! @brief MMC card partition to be accessed(BOOT_PARTITION_ACCESS in Extended CSD) */
typedef enum _mmc_access_partition
{
    kMMC_AccessPartitionUserAera = 0U, /*!< No access to boot partition (default), normal partition */
    kMMC_AccessPartitionBoot1 = 1U,    /*!< Read/Write boot partition 1 */
    kMMC_AccessPartitionBoot2 = 2U,    /*!< Read/Write boot partition 2*/
} mmc_access_partition_t;

/*! @brief The bit shift for PARTITION ACCESS filed in BOOT CONFIG (BOOT_CONFIG in Extend CSD) */
#define MMC_BOOT_CONFIG_PARTITION_ACCESS_SHIFT (0U)
/*! @brief The bit mask for PARTITION ACCESS field in BOOT CONFIG */
#define MMC_BOOT_CONFIG_PARTITION_ACCESS_MASK (0x00000007U)
/*! @brief The bit shift for PARTITION ENABLE field in BOOT CONFIG */
#define MMC_BOOT_CONFIG_PARTITION_ENABLE_SHIFT (3U)
/*! @brief The bit mask for PARTITION ENABLE field in BOOT CONFIG */
#define MMC_BOOT_CONFIG_PARTITION_ENABLE_MASK (0x00000038U)
/*! @brief The bit shift for ACK field in BOOT CONFIG */
#define MMC_BOOT_CONFIG_ACK_SHIFT (6U)
/*! @brief The bit mask for ACK field in BOOT CONFIG */
#define MMC_BOOT_CONFIG_ACK_MASK (0x00000040U)
/*! @brief The bit shift for BOOT BUS WIDTH field in BOOT CONFIG */
#define MMC_BOOT_BUS_WIDTH_WIDTH_SHIFT (8U)
/*! @brief The bit mask for BOOT BUS WIDTH field in BOOT CONFIG */
#define MMC_BOOT_BUS_WIDTH_WIDTH_MASK (0x00000300U)
/*! @brief The bit shift for BOOT BUS WIDTH RESET field in BOOT CONFIG */
#define MMC_BOOT_BUS_WIDTH_RESET_SHIFT (10U)
/*! @brief The bit mask for BOOT BUS WIDTH RESET field in BOOT CONFIG */
#define MMC_BOOT_BUS_WIDTH_RESET_MASK (0x00000400U)

/*! @brief MMC card CSD register flags */
typedef enum _mmc_csd_flag
{
    kMMC_CsdReadBlockPartialFlag = (1U << 0U),         /*!< Partial blocks for read allowed */
    kMMC_CsdWriteBlockMisalignFlag = (1U << 1U),       /*!< Write block misalignment */
    kMMC_CsdReadBlockMisalignFlag = (1U << 2U),        /*!< Read block misalignment */
    kMMC_CsdDsrImplementedFlag = (1U << 3U),           /*!< DSR implemented */
    kMMC_CsdWriteProtectGroupEnabledFlag = (1U << 4U), /*!< Write protect group enabled */
    kMMC_CsdWriteBlockPartialFlag = (1U << 5U),        /*!< Partial blocks for write allowed */
    kMMC_ContentProtectApplicationFlag = (1U << 6U),   /*!< Content protect application */
    kMMC_CsdFileFormatGroupFlag = (1U << 7U),          /*!< File format group */
    kMMC_CsdCopyFlag = (1U << 8U),                     /*!< Copy flag */
    kMMC_CsdPermanentWriteProtectFlag = (1U << 9U),    /*!< Permanent write protection */
    kMMC_CsdTemporaryWriteProtectFlag = (1U << 10U),   /*!< Temporary write protection */
} mmc_csd_flag_t;

/*! @brief Extended CSD register access mode(Access mode in CMD6). */
typedef enum _mmc_extended_csd_access_mode
{
    kMMC_ExtendedCsdAccessModeCommandSet = 0U, /*!< Command set related setting */
    kMMC_ExtendedCsdAccessModeSetBits = 1U,    /*!< Set bits in specific byte in Extended CSD  */
    kMMC_ExtendedCsdAccessModeClearBits = 2U,  /*!< Clear bits in specific byte in Extended CSD */
    kMMC_ExtendedCsdAccessModeWriteBits = 3U,  /*!< Write a value to specific byte in Extended CSD */
} mmc_extended_csd_access_mode_t;

/*! @brief EXT CSD byte index */
typedef enum _mmc_extended_csd_index
{
    kMMC_ExtendedCsdIndexEraseGroupDefinition = 175U, /*!< Erase Group Def */
    kMMC_ExtendedCsdIndexBootBusWidth = 177U,         /*!< Boot Bus Width */
    kMMC_ExtendedCsdIndexBootConfig = 179U,           /*!< Boot Config */
    kMMC_ExtendedCsdIndexBusWidth = 183U,             /*!< Bus Width */
    kMMC_ExtendedCsdIndexHighSpeedTiming = 185U,      /*!< High speed Timing */
    kMMC_ExtendedCsdIndexPowerClass = 187U,           /*!< Power Class */
    kMMC_ExtendedCsdIndexCommandSet = 191U,           /*!< Command Set */
} mmc_extended_csd_index_t;

/*! @brief The length of Extended CSD register, unit as bytes. */
#define MMC_EXTENDED_CSD_BYTES (512U)

/*! @brief MMC card default relative address */
#define MMC_DEFAULT_RELATIVE_ADDRESS (2U)

/*! @brief SD card product name length united as bytes. */
#define SD_PRODUCT_NAME_BYTES (5U)
/*! @brief SD card CID register */
typedef struct _sd_cid
{
    uint8_t manufacturerID;                     /*!< Manufacturer ID [127:120] */
    uint16_t applicationID;                     /*!< OEM/Application ID [119:104] */
    uint8_t productName[SD_PRODUCT_NAME_BYTES]; /*!< Product name [103:64] */
    uint8_t productVersion;                     /*!< Product revision [63:56] */
    uint32_t productSerialNumber;               /*!< Product serial number [55:24] */
    uint16_t manufacturerData;                  /*!< Manufacturing date [19:8] */
} sd_cid_t;

/*! @brief SD card CSD register */
typedef struct _sd_csd
{
    uint8_t csdStructure;        /*!< CSD structure [127:126] */
    uint8_t dataReadAccessTime1; /*!< Data read access-time-1 [119:112] */
    uint8_t dataReadAccessTime2; /*!< Data read access-time-2 in clock cycles (NSAC*100) [111:104] */
    uint8_t transferSpeed;       /*!< Maximum data transfer rate [103:96] */
    uint16_t cardCommandClass;   /*!< Card command classes [95:84] */
    uint8_t readBlockLength;     /*!< Maximum read data block length [83:80] */
    uint16_t flags;              /*!< Flags in sd_csd_flag_t */
    uint32_t deviceSize;         /*!< Device size [73:62] */
    /* Following fields from 'readCurrentVddMin' to 'deviceSizeMultiplier' exist in CSD version 1 */
    uint8_t readCurrentVddMin;    /*!< Maximum read current at VDD min [61:59] */
    uint8_t readCurrentVddMax;    /*!< Maximum read current at VDD max [58:56] */
    uint8_t writeCurrentVddMin;   /*!< Maximum write current at VDD min [55:53] */
    uint8_t writeCurrentVddMax;   /*!< Maximum write current at VDD max [52:50] */
    uint8_t deviceSizeMultiplier; /*!< Device size multiplier [49:47] */

    uint8_t eraseSectorSize;       /*!< Erase sector size [45:39] */
    uint8_t writeProtectGroupSize; /*!< Write protect group size [38:32] */
    uint8_t writeSpeedFactor;      /*!< Write speed factor [28:26] */
    uint8_t writeBlockLength;      /*!< Maximum write data block length [25:22] */
    uint8_t fileFormat;            /*!< File format [11:10] */
} sd_csd_t;

/*! @brief The bit shift for RATE UNIT field in TRANSFER SPEED */
#define SD_TRANSFER_SPEED_RATE_UNIT_SHIFT (0U)
/*! @brief The bit mask for RATE UNIT field in TRANSFER SPEED */
#define SD_TRANSFER_SPEED_RATE_UNIT_MASK (0x07U)
/*! @brief The bit shift for TIME VALUE field in TRANSFER SPEED */
#define SD_TRANSFER_SPEED_TIME_VALUE_SHIFT (2U)
/*! @brief The bit mask for TIME VALUE field in TRANSFER SPEED */
#define SD_TRANSFER_SPEED_TIME_VALUE_MASK (0x78U)
/*! @brief Read the value of FREQUENCY UNIT in TRANSFER SPEED field */
#define SD_RD_TRANSFER_SPEED_RATE_UNIT(x) \
    (((x.transferSpeed) & SD_TRANSFER_SPEED_RATE_UNIT_MASK) >> SD_TRANSFER_SPEED_RATE_UNIT_SHIFT)
/*! @brief Read the value of TIME VALUE in TRANSFER SPEED field */
#define SD_RD_TRANSFER_SPEED_TIME_VALUE(x) \
    (((x.transferSpeed) & SD_TRANSFER_SPEED_TIME_VALUE_MASK) >> SD_TRANSFER_SPEED_TIME_VALUE_SHIFT)

/*! @brief SD card SCR register */
typedef struct _sd_scr
{
    uint8_t scrStructure;             /*!< SCR Structure [63:60] */
    uint8_t sdSpecification;          /*!< SD memory card specification version [59:56] */
    uint16_t flags;                   /*!< SCR flags in sd_scr_flag_t */
    uint8_t sdSecurity;               /*!< Security specification supported [54:52] */
    uint8_t sdBusWidths;              /*!< Data bus widths supported [51:48] */
    uint8_t extendedSecurity;         /*!< Extended security support [46:43] */
    uint8_t commandSupport;           /*!< Command support bits [33:32] */
    uint32_t reservedForManufacturer; /*!< reserved for manufacturer usage [31:0] */
} sd_scr_t;

/*! @brief MMC card product name length united as bytes. */
#define MMC_PRODUCT_NAME_BYTES (6U)
/*! @brief MMC card CID register. */
typedef struct _mmc_cid
{
    uint8_t manufacturerID;                      /*!< Manufacturer ID */
    uint16_t applicationID;                      /*!< OEM/Application ID */
    uint8_t productName[MMC_PRODUCT_NAME_BYTES]; /*!< Product name */
    uint8_t productVersion;                      /*!< Product revision */
    uint32_t productSerialNumber;                /*!< Product serial number */
    uint8_t manufacturerData;                    /*!< Manufacturing date */
} mmc_cid_t;

/*! @brief MMC card CSD register. */
typedef struct _mmc_csd
{
    uint8_t csdStructureVersion;        /*!< CSD structure [127:126] */
    uint8_t systemSpecificationVersion; /*!< System specification version [125:122] */
    uint8_t dataReadAccessTime1;        /*!< Data read access-time 1 [119:112] */
    uint8_t dataReadAccessTime2;        /*!< Data read access-time 2 in CLOCK cycles (NSAC*100) [111:104] */
    uint8_t transferSpeed;              /*!< Max. bus clock frequency [103:96] */
    uint16_t cardCommandClass;          /*!< card command classes [95:84] */
    uint8_t readBlockLength;            /*!< Max. read data block length [83:80] */
    uint16_t flags;                     /*!< Contain flags in mmc_csd_flag_t */
    uint16_t deviceSize;                /*!< Device size [73:62] */
    uint8_t readCurrentVddMin;          /*!< Max. read current @ VDD min [61:59] */
    uint8_t readCurrentVddMax;          /*!< Max. read current @ VDD max [58:56] */
    uint8_t writeCurrentVddMin;         /*!< Max. write current @ VDD min [55:53] */
    uint8_t writeCurrentVddMax;         /*!< Max. write current @ VDD max [52:50] */
    uint8_t deviceSizeMultiplier;       /*!< Device size multiplier [49:47] */
    uint8_t eraseGroupSize;             /*!< Erase group size [46:42] */
    uint8_t eraseGroupSizeMultiplier;   /*!< Erase group size multiplier [41:37] */
    uint8_t writeProtectGroupSize;      /*!< Write protect group size [36:32] */
    uint8_t defaultEcc;                 /*!< Manufacturer default ECC [30:29] */
    uint8_t writeSpeedFactor;           /*!< Write speed factor [28:26] */
    uint8_t maxWriteBlockLength;        /*!< Max. write data block length [25:22] */
    uint8_t fileFormat;                 /*!< File format [11:10] */
    uint8_t eccCode;                    /*!< ECC code [9:8] */
} mmc_csd_t;

/*! @brief MMC card Extended CSD register (unit: byte). */
typedef struct _mmc_extended_csd
{
    uint8_t highDensityEraseGroupDefinition;  /*!< High-density erase group definition [175] */
    uint8_t bootDataBusWidth;                 /*!< Boot bus width [177] */
    uint8_t bootConfig;                       /*!< Boot configuration [179] */
    uint8_t eraseMemoryContent;               /*!< Erased memory content [181] */
    uint8_t dataBusWidth;                     /*!< Data bus width mode [183] */
    uint8_t highSpeedTiming;                  /*!< High-speed interface timing [185] */
    uint8_t powerClass;                       /*!< Power class [187] */
    uint8_t commandSetRevision;               /*!< Command set revision [189] */
    uint8_t commandSet;                       /*!< Command set [191] */
    uint8_t extendecCsdVersion;               /*!< Extended CSD revision [192] */
    uint8_t csdStructureVersion;              /*!< CSD structure version [194] */
    uint8_t cardType;                         /*!< Card Type [196] */
    uint8_t powerClass52MHz195V;              /*!< Power Class for 52MHz @ 1.95V [200] */
    uint8_t powerClass26MHz195V;              /*!< Power Class for 26MHz @ 1.95V [201] */
    uint8_t powerClass52MHz360V;              /*!< Power Class for 52MHz @ 3.6V [202] */
    uint8_t powerClass26MHz360V;              /*!< Power Class for 26MHz @ 3.6V [203] */
    uint8_t minimumReadPerformance4Bit26MHz;  /*!< Minimum Read Performance for 4bit at 26MHz [205] */
    uint8_t minimumWritePerformance4Bit26MHz; /*!< Minimum Write Performance for 4bit at 26MHz [206] */
    uint8_t
        minimumReadPerformance8Bit26MHz4Bit52MHz; /*!< Minimum read Performance for 8bit at 26MHz/4bit @52MHz [207] */
    uint8_t
        minimumWritePerformance8Bit26MHz4Bit52MHz; /*!< Minimum Write Performance for 8bit at 26MHz/4bit @52MHz [208] */
    uint8_t minimumReadPerformance8Bit52MHz;       /*!< Minimum Read Performance for 8bit at 52MHz [209] */
    uint8_t minimumWritePerformance8Bit52MHz;      /*!< Minimum Write Performance for 8bit at 52MHz [210] */
    uint32_t sectorCount;                          /*!< Sector Count [215:212] */
    uint8_t sleepAwakeTimeout;                     /*!< Sleep/awake timeout [217] */
    uint8_t sleepCurrentVCCQ;                      /*!< Sleep current (VCCQ) [219] */
    uint8_t sleepCurrentVCC;                       /*!< Sleep current (VCC) [220] */
    uint8_t highCapacityWriteProtectGroupSize;     /*!< High-capacity write protect group size [221] */
    uint8_t reliableWriteSectorCount;              /*!< Reliable write sector count [222] */
    uint8_t highCapacityEraseTimeout;              /*!< High-capacity erase timeout [223] */
    uint8_t highCapacityEraseUnitSize;             /*!< High-capacity erase unit size [224] */
    uint8_t accessSize;                            /*!< Access size [225] */
    uint8_t bootSizeMultiplier;                    /*!< Boot partition size [226] */
    uint8_t bootInformation;                       /*!< Boot information [228] */
    uint8_t supportedCommandSet;                   /*!< Supported Command Sets [504] */
} mmc_extended_csd_t;

/*! @brief The bit shift for COMMAND SET field in SWITCH command. */
#define MMC_SWITCH_COMMAND_SET_SHIFT (0U)
/*! @brief The bit mask for COMMAND set field in SWITCH command. */
#define MMC_SWITCH_COMMAND_SET_MASK (0x00000007U)
/*! @brief The bit shift for VALUE field in SWITCH command */
#define MMC_SWITCH_VALUE_SHIFT (8U)
/*! @brief The bit mask for VALUE field in SWITCH command */
#define MMC_SWITCH_VALUE_MASK (0x0000FF00U)
/*! @brief The bit shift for BYTE INDEX field in SWITCH command */
#define MMC_SWITCH_BYTE_INDEX_SHIFT (16U)
/*! @brief The bit mask for BYTE INDEX field in SWITCH command */
#define MMC_SWITCH_BYTE_INDEX_MASK (0x00FF0000U)
/*! @brief The bit shift for ACCESS MODE field in SWITCH command */
#define MMC_SWITCH_ACCESS_MODE_SHIFT (24U)
/*! @brief The bit mask for ACCESS MODE field in SWITCH command */
#define MMC_SWTICH_ACCESS_MODE_MASK (0x03000000U)

/*! @brief MMC Extended CSD configuration. */
typedef struct _mmc_extended_csd_config
{
    mmc_command_set_t commandSet;              /*!< Command set */
    uint8_t ByteValue;                         /*!< The value to set */
    uint8_t ByteIndex;                         /*!< The byte index in Extended CSD(mmc_extended_csd_index_t) */
    mmc_extended_csd_access_mode_t accessMode; /*!< Access mode */
} mmc_extended_csd_config_t;

/*! @} */
#endif /* _FSL_SPECIFICATION_H_ */
