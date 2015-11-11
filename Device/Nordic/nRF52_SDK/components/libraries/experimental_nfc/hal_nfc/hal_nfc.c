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

#include <stdint.h>
#include <stdbool.h>
#include "hal_nfc.h"
#include "nfc_lib.h"
#include "nfc_fixes.h"
#include "nrf52.h"
#include "nrf52_bitfields.h"
#include "app_util_platform.h"
#include "nordic_common.h"


/* NFC library version history: 
 * #define NFC_LIB_VERSION             0x00 first experimental version intended for nRF52 IC rev. Engineering A (PCA10036, part of nRF52 Preview Development Kit)
 * #define NFC_LIB_VERSION             0x01 experimental version intended for nRF52 IC rev. Engineering B (PCA10040, part of nRF52 Development Kit)
 */
#define NFC_LIB_VERSION             0x01

#define T2T_INTERNAL_BYTES_NR       10u     /**< Number of internal bytes defined by Type 2 Tag Operation Technical Specification */
#define T2T_INTERNAL_BYTE_SN0_SHIFT 0u      /**< Internal Byte SN0, NRF_FICR->NFC.TAGHEADER0.MFGID which is Manufacturer ID */
#define T2T_INTERNAL_BYTE_SN1_SHIFT 8u      /**< Internal Byte SN1, NRF_FICR->NFC.TAGHEADER0.UID0 */
#define T2T_INTERNAL_BYTE_SN2_SHIFT 16u     /**< Internal Byte SN2, NRF_FICR->NFC.TAGHEADER0.UID1 */
#define T2T_INTERNAL_BYTE_SN3_SHIFT 0u      /**< Internal Byte SN3, NRF_FICR->NFC.TAGHEADER1.UID3 */
#define T2T_INTERNAL_BYTE_SN4_SHIFT 8u      /**< Internal Byte SN4, NRF_FICR->NFC.TAGHEADER1.UID4 */
#define T2T_INTERNAL_BYTE_SN5_SHIFT 16u     /**< Internal Byte SN5, NRF_FICR->NFC.TAGHEADER1.UID5 */
#define T2T_INTERNAL_BYTE_SN6_SHIFT 24u     /**< Internal Byte SN6, NRF_FICR->NFC.TAGHEADER1.UID6 */
#define CASCADE_TAG_BYTE            0x88    /**< Constant defined by ISO/EIC 14443-3 */

#define NFCID1_2ND_LAST_BYTE2_SHIFT 16u
#define NFCID1_2ND_LAST_BYTE1_SHIFT 8u
#define NFCID1_2ND_LAST_BYTE0_SHIFT 0u
#define NFCID1_LAST_BYTE3_SHIFT     24u
#define NFCID1_LAST_BYTE2_SHIFT     16u
#define NFCID1_LAST_BYTE1_SHIFT     8u
#define NFCID1_LAST_BYTE0_SHIFT     0u

#ifdef HAL_NFC_ENGINEERING_A_FTPAN_WORKAROUND
/* Begin: Bugfix for FTPAN-57 (IC-9563) */
#define NRF_NFCT_SHUNTREGTHRESHOLDS  (*(uint32_t volatile *)(0x40005610))
#define NRF_NFCT_MODSTEPFIXED        (*(uint32_t volatile *)(0x40005614))
#define NRF_NFCT_MODSTEPMULTIPLIER   (*(uint32_t volatile *)(0x40005618))
#define NRF_NFCT_INITIALLOADCTRLVAL  (*(uint32_t volatile *)(0x40005688))
/* End:   Bugfix for FTPAN-57 (IC-9563) */

/* Begin: Bugfix for FTPAN-24 */
#define NRF_NFCT_AUTOCOLRESSTATUS  (*(uint32_t volatile *)(0x40005408))
/* End: Bugfix for FTPAN-24 */

/* Begin: Bugfix for FTPAN-27 */
#define NRF_NFCT_TASKS_DISABLERXDATA  (*(uint32_t volatile *)(0x40005020))
/* End: Bugfix for FTPAN-27 */

/* Begin: Bugfix for FTPAN-17 */
#define NFC_HAL_FIELDPRESENT_MASK      (NFCT_FIELDPRESENT_LOCKDETECT_Msk | NFCT_FIELDPRESENT_FIELDPRESENT_Msk)

#define NFC_HAL_FIELDPRESENT_IS_LOST   ((NFCT_FIELDPRESENT_FIELDPRESENT_NoField << NFCT_FIELDPRESENT_FIELDPRESENT_Pos) \
                                       | (NFCT_FIELDPRESENT_LOCKDETECT_NotLocked << NFCT_FIELDPRESENT_LOCKDETECT_Pos))
                                     
#define NFC_HAL_FIELDPRESENT_NO_FIELD   (NFCT_FIELDPRESENT_FIELDPRESENT_NoField << NFCT_FIELDPRESENT_FIELDPRESENT_Pos)
/* End: Bugfix for FTPAN-17*/


#endif // HAL_NFC_ENGINEERING_A_FTPAN_WORKAROUND

#define NFC_RX_BUFFER 16
#define READ_CMD      0x30

static hal_nfc_callback nfc_lib_callback;
static void *nfc_lib_context;
static uint8_t nfc_rx_buffer[NFC_RX_BUFFER];

static uint8_t nfcInternal[T2T_INTERNAL_BYTES_NR] = {0};
//        {0x5F, 0x12, 0x34, 0xF1, 0x56, 0x78, 0x9A, 0xBC, 0x08, NFC_LIB_VERSION};


#ifdef HAL_NFC_ENGINEERING_A_FTPAN_WORKAROUND
/* Begin: Bugfix for FTPAN-45 (IC-6915) */
volatile uint8_t m_nfc_active = 0;
/* End: Bugfix for FTPAN-45  (IC-6915) */

/* Begin: Bugfix for FTPAN-17 (IC-9563) */

/* The following three function definitions are a workaround for IC-9563: NFC in nRF52 IC rev. Engineering A does not
 * give the field lost signal when field is turned off. */
static void hal_nfc_field_check(void);
 
static bool field_on = false;

volatile bool hal_nfc_fielddetected = false;
volatile bool hal_nfc_fieldlost     = false;

static void field_timer_with_callback_config()
{
    NRF_TIMER4->MODE      = TIMER_MODE_MODE_Timer << TIMER_MODE_MODE_Pos;
    NRF_TIMER4->BITMODE   = TIMER_BITMODE_BITMODE_08Bit << TIMER_BITMODE_BITMODE_Pos;
    NRF_TIMER4->PRESCALER = 4 << TIMER_PRESCALER_PRESCALER_Pos;
    NRF_TIMER4->CC[0] = 100 << TIMER_CC_CC_Pos;
    NRF_TIMER4->SHORTS = TIMER_SHORTS_COMPARE0_CLEAR_Enabled << TIMER_SHORTS_COMPARE0_CLEAR_Pos;
    NRF_TIMER4->INTENSET = TIMER_INTENSET_COMPARE0_Set << TIMER_INTENSET_COMPARE0_Pos;
    NVIC_ClearPendingIRQ(TIMER4_IRQn);
    NVIC_SetPriority(TIMER4_IRQn, APP_IRQ_PRIORITY_LOW);
    NVIC_EnableIRQ(TIMER4_IRQn);
}

void TIMER4_IRQHandler(void)
{
    hal_nfc_field_check();
    NRF_TIMER4->EVENTS_COMPARE[0] = 0;
}

static void hal_nfc_field_check(void)
{
    static uint32_t field_state_cnt = 0;
    volatile uint32_t dummy;    
    uint32_t        nfc_fieldpresen_masked;

    /* Begin: Bugfix for FTPAN-24 */
    NRF_NFCT_AUTOCOLRESSTATUS = 0; /* dummy write - no effect. */
    NRF_NFCT_AUTOCOLRESSTATUS = 0; /* dummy write - no effect. */
    // Don't worry about interrupted case - NRF_NFCT->FIELDPRESENT is read each 100 us, so the workaround should succeed most of the time.
    /* End: Bugfix for FTPAN-24 */
    
    nfc_fieldpresen_masked    = NRF_NFCT->FIELDPRESENT & NFC_HAL_FIELDPRESENT_MASK;
    
    
    if (field_on)
    {
        if (nfc_fieldpresen_masked == NFC_HAL_FIELDPRESENT_IS_LOST)
        {
            ++field_state_cnt;
            if (field_state_cnt > 7)
            {
                field_state_cnt = 0;
                hal_nfc_fieldlost = true;
                dummy = hal_nfc_fieldlost;
                field_on = false;
                NVIC_SetPendingIRQ(NFCT_IRQn);
                UNUSED_VARIABLE(dummy);
            }
            
            return;
        }
    }
    else
    {     
        nfc_fieldpresen_masked &= NFCT_FIELDPRESENT_FIELDPRESENT_Msk;
        if (nfc_fieldpresen_masked !=  NFC_HAL_FIELDPRESENT_NO_FIELD)
        {
            ++field_state_cnt;
            if (field_state_cnt > 7)
            {
                field_state_cnt = 0;
                hal_nfc_fielddetected = true;
                dummy = hal_nfc_fielddetected;
                field_on = true;
                NVIC_SetPendingIRQ(NFCT_IRQn);
                UNUSED_VARIABLE(dummy);
            }
            
            return;
        }
    }
    
    field_state_cnt = 0;
}
/* End: Bugfix for FTPAN-17, FTPAN-27 */
#endif // HAL_NFC_ENGINEERING_A_FTPAN_WORKAROUND

hal_nfc_retval hal_nfc_setup(hal_nfc_callback callback, void *cbContext)
{
    nfc_lib_callback = callback;
    nfc_lib_context  = cbContext;
    uint32_t nfc_tag_header0 = NRF_FICR->NFC.TAGHEADER0;
    uint32_t nfc_tag_header1 = NRF_FICR->NFC.TAGHEADER1;
    
/* Begin: Bugfix for FTPAN-17 */
/* fixed by avoiding usage of FIELDLOST and FIELDETECTED events */
#ifndef  HAL_NFC_ENGINEERING_A_FTPAN_WORKAROUND

    NRF_NFCT->SHORTS =
            (NFCT_SHORTS_FIELDDETECTED_ACTIVATE_Enabled << NFCT_SHORTS_FIELDDETECTED_ACTIVATE_Pos) |
            (NFCT_SHORTS_FIELDLOST_SENSE_Enabled        << NFCT_SHORTS_FIELDLOST_SENSE_Pos);

    NRF_NFCT->INTENSET = (NFCT_INTENSET_FIELDDETECTED_Enabled   << NFCT_INTENSET_FIELDDETECTED_Pos);
    NRF_NFCT->INTENSET = (NFCT_INTENSET_FIELDLOST_Enabled  << NFCT_INTENSET_FIELDLOST_Pos);
#endif // HAL_NFC_ENGINEERING_A_FTPAN_WORKAROUND
/* End: Bugfix for FTPAN-17 */

    NRF_NFCT->INTENSET = (NFCT_INTENSET_RXFRAMEEND_Enabled << NFCT_INTENSET_RXFRAMEEND_Pos);
    NRF_NFCT->INTENSET = (NFCT_INTENSET_ERROR_Enabled      << NFCT_INTENSET_ERROR_Pos);
    NRF_NFCT->INTENSET = (NFCT_INTENSET_RXERROR_Enabled    << NFCT_INTENSET_RXERROR_Pos);
    NRF_NFCT->INTENSET = (NFCT_INTENSET_SELECTED_Enabled   << NFCT_INTENSET_SELECTED_Pos);

#ifdef  HAL_NFC_ENGINEERING_A_FTPAN_WORKAROUND
    /* Begin:   Bugfix for FTPAN-45 (IC-6915) */
    NRF_NFCT->INTENSET = (NFCT_INTENSET_RXFRAMESTART_Enabled   << NFCT_INTENSET_RXFRAMESTART_Pos);
    NRF_NFCT->INTENSET = (NFCT_INTENSET_TXFRAMESTART_Enabled   << NFCT_INTENSET_TXFRAMESTART_Pos);
    /* End:   Bugfix for FTPAN-45 (IC-6915) */
#endif // HAL_NFC_ENGINEERING_A_FTPAN_WORKAROUND

    /* According to ISO/EIC 14443-3 */
    nfcInternal[0] = (uint8_t) ((nfc_tag_header0 >> T2T_INTERNAL_BYTE_SN0_SHIFT) & 0x000000FF);         //SN0
    nfcInternal[1] = (uint8_t) ((nfc_tag_header0 >> T2T_INTERNAL_BYTE_SN1_SHIFT) & 0x000000FF);         //SN1
    nfcInternal[2] = (uint8_t) ((nfc_tag_header0 >> T2T_INTERNAL_BYTE_SN2_SHIFT) & 0x000000FF);         //SN2
    nfcInternal[3] = (uint8_t) ((CASCADE_TAG_BYTE) ^ nfcInternal[0] ^ nfcInternal[1] ^ nfcInternal[2]); //BCC0 = CASCADE_TAG_BYTE ^ SN0 ^ SN1 ^ SN2
    nfcInternal[4] = (uint8_t) ((nfc_tag_header1 >> T2T_INTERNAL_BYTE_SN3_SHIFT) & 0x000000FF);         //SN3
    nfcInternal[5] = (uint8_t) ((nfc_tag_header1 >> T2T_INTERNAL_BYTE_SN4_SHIFT) & 0x000000FF);         //SN4
    nfcInternal[6] = (uint8_t) ((nfc_tag_header1 >> T2T_INTERNAL_BYTE_SN5_SHIFT) & 0x000000FF);         //SN5
    nfcInternal[7] = (uint8_t) ((nfc_tag_header1 >> T2T_INTERNAL_BYTE_SN6_SHIFT) & 0x000000FF);         //SN6
    nfcInternal[8] = (uint8_t) (nfcInternal[4] ^ nfcInternal[5] ^ nfcInternal[6] ^ nfcInternal[7]);     //BCC1 = SN3 ^ SN4 ^ SN5 ^ SN6
    nfcInternal[9] = (uint8_t) (NFC_LIB_VERSION);                                                       //For internal use
    (void) nfcSetInternal((char *) nfcInternal, sizeof(nfcInternal));

    /* MSB of NFCID1_2ND_LAST register is not used - always 0 */
    NRF_NFCT->NFCID1_2ND_LAST = ((uint32_t) nfcInternal[0] << NFCID1_2ND_LAST_BYTE2_SHIFT) |
                                ((uint32_t) nfcInternal[1] << NFCID1_2ND_LAST_BYTE1_SHIFT) |
                                ((uint32_t) nfcInternal[2] << NFCID1_2ND_LAST_BYTE0_SHIFT);

    NRF_NFCT->NFCID1_LAST = ((uint32_t) nfcInternal[4] << NFCID1_LAST_BYTE3_SHIFT) |
                            ((uint32_t) nfcInternal[5] << NFCID1_LAST_BYTE2_SHIFT) |
                            ((uint32_t) nfcInternal[6] << NFCID1_LAST_BYTE1_SHIFT) |
                            ((uint32_t) nfcInternal[7] << NFCID1_LAST_BYTE0_SHIFT);

//    NRF_NFCT->NFCID1_2ND_LAST = 0x005F1234;
//    NRF_NFCT->NFCID1_LAST     = 0x56789ABC;
    

    /* Begin: Bugfix for FTPAN-25 (IC-9929) */
    /* Workaround for wrong SENSRES values require to use SDD00001, but here SDD00100 is used
       because it's required to operate with Windows Phone */
    NRF_NFCT->SENSRES =
            (NFCT_SENSRES_NFCIDSIZE_NFCID1Double << NFCT_SENSRES_NFCIDSIZE_Pos) |
            (NFCT_SENSRES_BITFRAMESDD_SDD00100 << NFCT_SENSRES_BITFRAMESDD_Pos);
    /* End:   Bugfix for FTPAN-25 (IC-9929)*/

    
#ifdef  HAL_NFC_ENGINEERING_A_FTPAN_WORKAROUND    
    /* Begin: Bugfix for FTPAN-57 (IC-9563) */
    /* Values taken from IC-9563 */
    NRF_NFCT_SHUNTREGTHRESHOLDS = 0x00000005;
    NRF_NFCT_MODSTEPFIXED       = 0x0000003F;
    NRF_NFCT_MODSTEPMULTIPLIER  = 0x00000001;
    NRF_NFCT_INITIALLOADCTRLVAL = 0x00000001;
    /* End: Bugfix for FTPAN-57  (IC-9563) */

    /* Begin: Bugfix for FTPAN-17 (IC-9563) */
    /* Activating workaround. */
    field_timer_with_callback_config();
    NRF_TIMER4->TASKS_START = 1;
    /* End:   Bugfix for FTPAN-17 (IC-9563) */
#endif // HAL_NFC_ENGINEERING_A_FTPAN_WORKAROUND

    return HAL_NFC_RETVAL_OK;
}

hal_nfc_retval hal_nfc_set_parameter(hal_nfc_param_id id, void *data, size_t dataLength)
{
    (void)id;
    (void)data;
    (void)dataLength;

    return HAL_NFC_RETVAL_OK;
} 

hal_nfc_retval hal_nfc_get_parameter(hal_nfc_param_id id, void *data, size_t *maxDataLength)
{
    (void)id;
    (void)data;
    (void)maxDataLength;

    return HAL_NFC_RETVAL_OK;
}


hal_nfc_retval hal_nfc_start(void)
{
    NRF_NFCT->MAXLEN    = NFC_RX_BUFFER;
    NRF_NFCT->PACKETPTR = (uint32_t)nfc_rx_buffer;

    NRF_NFCT->ERRORSTATUS = 0xFFFFFFFF; // clear all error flags, it works for nRF52 IC rev. Engineering A and nRF52 IC rev. Engineering B.
    NVIC_ClearPendingIRQ(NFCT_IRQn);
    NVIC_SetPriority(NFCT_IRQn, APP_IRQ_PRIORITY_LOW);
    NVIC_EnableIRQ(NFCT_IRQn);
    NRF_NFCT->TASKS_SENSE = 1;

    return HAL_NFC_RETVAL_OK;
}

hal_nfc_retval hal_nfc_send(const char *data, size_t dataLength)
{
    NRF_NFCT->PACKETPTR     = (uint32_t)data;
    NRF_NFCT->TXD.AMOUNT    = dataLength<<3;
    NRF_NFCT->INTENSET      = (NFCT_INTENSET_TXFRAMEEND_Enabled << NFCT_INTENSET_TXFRAMEEND_Pos);
    NRF_NFCT->TASKS_STARTTX = 1;

    return HAL_NFC_RETVAL_OK;
}

hal_nfc_retval hal_nfc_stop(void)
{
    NRF_NFCT->TASKS_DISABLE;

    return HAL_NFC_RETVAL_OK;
}

hal_nfc_retval hal_nfc_done(void)
{
    return HAL_NFC_RETVAL_OK;
}

void NFCT_IRQHandler(void)
{
    volatile uint32_t dummy;

#ifdef  HAL_NFC_ENGINEERING_A_FTPAN_WORKAROUND
    /* Begin: Bugfix for FTPAN-27 */
    if (hal_nfc_fieldlost)
    {
        hal_nfc_fieldlost = false;
    
        nfc_lib_callback(nfc_lib_context, HAL_NFC_EVENT_FIELD_OFF, 0, 0);
        NRF_NFCT->TASKS_SENSE      = 1;
    }
    /* End: Bugfix for FTPAN-27 */
#else
    if (NRF_NFCT->EVENTS_FIELDLOST)
    {
        NRF_NFCT->EVENTS_FIELDLOST = 0;
        /* Perform read to ensure clearing is effective */
        dummy = NRF_NFCT->EVENTS_FIELDLOST;
        (void)dummy;

        nfc_lib_callback(nfc_lib_context, HAL_NFC_EVENT_FIELD_OFF, 0, 0);
    }
#endif
        
    if (NRF_NFCT->EVENTS_TXFRAMEEND)
    {
        nfc_lib_callback(nfc_lib_context, HAL_NFC_EVENT_DATA_TRANSMITTED, 0, 0);

        NRF_NFCT->PACKETPTR          = (uint32_t)nfc_rx_buffer;
        NRF_NFCT->MAXLEN             = NFC_RX_BUFFER;
        NRF_NFCT->TASKS_ENABLERXDATA = 1;
        NRF_NFCT->INTENCLR           = (NFCT_INTENSET_TXFRAMEEND_Enabled <<
                                            NFCT_INTENSET_TXFRAMEEND_Pos);
        NRF_NFCT->EVENTS_TXFRAMEEND  = 0;
        /* Perform read to ensure clearing is effective */
        dummy = NRF_NFCT->EVENTS_TXFRAMEEND;
        (void)dummy;

#ifdef  HAL_NFC_ENGINEERING_A_FTPAN_WORKAROUND
        /* Begin:   Bugfix for FTPAN-45 (IC-6915) */
        m_nfc_active = 0;
        /* End:   Bugfix for FTPAN-45 (IC-6915) */
#endif
    }

    if (NRF_NFCT->EVENTS_RXFRAMEEND)
    {
        if (READ_CMD == nfc_rx_buffer[0])
        {
            nfc_lib_callback(nfc_lib_context, HAL_NFC_EVENT_DATA_RECEIVED, (void*)nfc_rx_buffer, 2);
        }
        else
        {
            NRF_NFCT->TASKS_ENABLERXDATA = 1;
        }

        NRF_NFCT->EVENTS_RXFRAMEEND = 0;
        /* Perform read to ensure clearing is effective */
        dummy = NRF_NFCT->EVENTS_RXFRAMEEND;
        (void)dummy;

#ifdef  HAL_NFC_ENGINEERING_A_FTPAN_WORKAROUND
        /* Begin:   Bugfix for FTPAN-45 (IC-6915) */
        m_nfc_active = 0;
        /* End:   Bugfix for FTPAN-45 (IC-69150) */
#endif
    }

#ifdef  HAL_NFC_ENGINEERING_A_FTPAN_WORKAROUND
    /* Begin:   Bugfix for FTPAN-45 (IC-6915) */
    if (NRF_NFCT->EVENTS_RXFRAMESTART)
    {
        NRF_NFCT->EVENTS_RXFRAMESTART = 0;
        /* Perform read to ensure clearing is effective */
        dummy = NRF_NFCT->EVENTS_RXFRAMESTART;
        (void)dummy;

        if (m_nfc_active == 0)
        {
            m_nfc_active = 1;
        }
    }
    if (NRF_NFCT->EVENTS_TXFRAMESTART)
    {
        NRF_NFCT->EVENTS_TXFRAMESTART = 0;
        /* Perform read to ensure clearing is effective */
        dummy = NRF_NFCT->EVENTS_TXFRAMESTART;
        (void)dummy;

        if (m_nfc_active == 0)
        {
            m_nfc_active = 1;
        }
    }
    /* End:   Bugfix for FTPAN-45 (IC-6915) */
#endif

    if (NRF_NFCT->EVENTS_ERROR)
    {
        NRF_NFCT->PACKETPTR          = (uint32_t)nfc_rx_buffer;
        NRF_NFCT->MAXLEN             = NFC_RX_BUFFER;
        NRF_NFCT->TASKS_ENABLERXDATA = 1;
        NRF_NFCT->ERRORSTATUS        = 0xFFFFFFFF; // clear all error flags
        NRF_NFCT->EVENTS_ERROR       = 0;
        /* Perform read to ensure clearing is effective */
        dummy = NRF_NFCT->EVENTS_ERROR;
        (void)dummy;
    }

    if (NRF_NFCT->EVENTS_RXERROR)
    {
        NRF_NFCT->FRAMESTATUS.RX     = 1;
        NRF_NFCT->TASKS_ENABLERXDATA = 1;
        NRF_NFCT->EVENTS_RXERROR     = 0;
        /* Perform read to ensure clearing is effective */
        dummy = NRF_NFCT->EVENTS_RXERROR;
        (void)dummy;
    }

    if (NRF_NFCT->EVENTS_SELECTED)
    {
        NRF_NFCT->PACKETPTR          = (uint32_t)nfc_rx_buffer;
        NRF_NFCT->MAXLEN             = NFC_RX_BUFFER;
        NRF_NFCT->TASKS_ENABLERXDATA = 1;
        nfc_lib_callback(nfc_lib_context, HAL_NFC_EVENT_FIELD_ON, 0, 0);
        NRF_NFCT->EVENTS_SELECTED    = 0;
        /* Perform read to ensure clearing is effective */
        dummy = NRF_NFCT->EVENTS_SELECTED;
        (void)dummy;
    }

    //Additional handler of FIELDDETECTED event
#ifdef  HAL_NFC_ENGINEERING_A_FTPAN_WORKAROUND
        /* Begin: Bugfix for FTPAN-27 */
    if (hal_nfc_fielddetected)
    {
        hal_nfc_fielddetected = false;
        
        NRF_NFCT->TASKS_ACTIVATE = 1;
        NRF_NFCT_TASKS_DISABLERXDATA = 1;
        /* End: Bugfix for FTPAN-27 */
#else
    if (NRF_NFCT->EVENTS_FIELDDETECTED) 
    {
        
        NRF_NFCT->EVENTS_FIELDDETECTED = 0;
        /* Perform read to ensure clearing is effective */
        dummy = NRF_NFCT->EVENTS_FIELDDETECTED;
        (void)dummy;
#endif


#if defined(SOFTDEVICE_PRESENT)
        (void) sd_clock_hfclk_request();
        uint32_t hfclk_running;
        do
        {
            (void) sd_clock_hfclk_is_running(&hfclk_running);
        }
        while(!hfclk_running);
#else
        NRF_CLOCK->TASKS_HFCLKSTART = 1;
        while(!NRF_CLOCK->EVENTS_HFCLKSTARTED);
#endif /* defined(SOFTDEVICE_PRESENT) */

    }
}
