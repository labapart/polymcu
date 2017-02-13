/* Copyright (c) Nordic Semiconductor. All Rights Reserved.
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
#include "app_atfifo.h"
#include "app_atfifo_internal.h"
#include <string.h>
#include "app_util.h"


/* Unions testing */
STATIC_ASSERT(sizeof(nrf_atfifo_postag_t) == sizeof(uint32_t));


ret_code_t app_atfifo_init(app_atfifo_t * const p_fifo, void * p_buf, uint16_t buf_size, uint16_t item_size)
{
    if(NULL == p_buf)
    {
        return NRF_ERROR_NULL;
    }
    if(0 != (buf_size % item_size))
    {
        return NRF_ERROR_INVALID_LENGTH;
    }

    p_fifo->p_buf     = p_buf;
    p_fifo->tail.tag  = 0;
    p_fifo->head.tag  = 0;
    p_fifo->buf_size  = buf_size;
    p_fifo->item_size = item_size;

    return NRF_SUCCESS;
}


ret_code_t app_atfifo_clear(app_atfifo_t * const p_fifo)
{
    bool resleased = app_atfifo_space_clear(p_fifo);
    return resleased ? NRF_SUCCESS : NRF_ERROR_BUSY;
}


ret_code_t app_atfifo_put(app_atfifo_t * const p_fifo, void const * p_var, size_t size, bool * const p_visible)
{
    app_atfifo_wcontext_t context;
    bool visible;
    void * p_d = app_atfifo_wopen(p_fifo, &context, size);
    if(NULL == p_d)
    {
        return NRF_ERROR_NO_MEM;
    }

    memcpy(p_d, p_var, size);

    visible = app_atfifo_wcommit(p_fifo, &context);
    if(NULL != p_visible)
    {
        *p_visible = visible;
    }
    return NRF_SUCCESS;
}


void * app_atfifo_wopen_internal(app_atfifo_t * const p_fifo, app_atfifo_wcontext_t * p_context)
{
    if(app_atfifo_wspace_req(p_fifo, &(p_context->last_tail)))
    {
        return ((uint8_t*)(p_fifo->p_buf)) + p_context->last_tail.pos.wr;
    }
    return NULL;
}


bool app_atfifo_wcommit(app_atfifo_t * const p_fifo, app_atfifo_wcontext_t * p_context)
{
    if((p_context->last_tail.pos.wr) == (p_context->last_tail.pos.rd))
    {
        app_atfifo_wspace_close(p_fifo);
        return true;
    }
    return false;
}


ret_code_t app_atfifo_get(app_atfifo_t * const p_fifo, void * const p_var, size_t size, bool * p_released)
{
    app_atfifo_rcontext_t context;
    bool released;
    void const * p_s = app_atfifo_ropen(p_fifo, &context, size);
    if(NULL == p_s)
    {
        return NRF_ERROR_NOT_FOUND;
    }

    memcpy(p_var, p_s, size);

    released = app_atfifo_rflush(p_fifo, &context);
    if(NULL != p_released)
    {
        *p_released = released;
    }
    return NRF_SUCCESS;
}


void const * app_atfifo_ropen_internal(app_atfifo_t * const p_fifo, app_atfifo_rcontext_t * p_context)
{
    if(app_atfifo_rspace_req(p_fifo, &(p_context->last_head)))
    {
        return ((uint8_t*)(p_fifo->p_buf)) + p_context->last_head.pos.rd;
    }
    return NULL;
}


bool app_atfifo_rflush(app_atfifo_t * const p_fifo, app_atfifo_rcontext_t * p_context)
{
    if((p_context->last_head.pos.wr) == (p_context->last_head.pos.rd))
    {
        app_atfifo_rspace_close(p_fifo);
        return true;
    }
    return false;
}
