#ifndef FS_CONFIG_H__
#define FS_CONFIG_H__

#include <stdint.h>
#include "nrf.h"

/**
 * @defgroup fstorage_config FStorage configuration
 * @ingroup fstorage
 * @{
 * @brief FStorage configuration.
 */


/**@brief Macro for max number of operations in the fs cmd queue.
 */
#define FS_CMD_QUEUE_SIZE   (8)


/**@brief Macro for max number of retries for a flash command before it notifies as failed.
 */
#define FS_CMD_MAX_RETRIES  (3)


/**@brief Macro for the content of a flash address that has not been written to.
 */
#define FS_EMPTY_MASK       (0xFFFFFFFF)


/**@brief Macro for flash page size according to chip family
 */
#if defined (NRF51)
    #define FS_PAGE_SIZE    (1024)
#elif defined (NRF52)
    #define FS_PAGE_SIZE    (4096)
#else
    #error "Device family must be defined. See nrf.h."
#endif


/*@brief Macro for flash page size according to chip family
*/
#define FS_PAGE_SIZE_WORDS  (FS_PAGE_SIZE/4)


/**@brief Static inline function that provides last page address
 *
 * @note    If there is a bootloader present the bootloader address read from UICR
 *          will act as the page beyond the end of the available flash storage
 */
static __INLINE uint32_t fs_flash_page_end_addr()
{
    uint32_t const bootloader_addr = NRF_UICR->NRFFW[0];
    return  ((bootloader_addr != FS_EMPTY_MASK) ?
             bootloader_addr : NRF_FICR->CODESIZE * FS_PAGE_SIZE);
}


/**@brief Macro for last page address
 *
 * @note    If there is a bootloader present the bootloader address read from UICR
 *          will act as the page beyond the end of the available flash storage
 */
#define FS_PAGE_END_ADDR  fs_flash_page_end_addr()


/**@brief Macro to describe the write
 *
 */
#if defined (NRF51)
    #define FS_MAX_WRITE_SIZE_WORDS	    (256)
#elif defined (NRF52)
    #define FS_MAX_WRITE_SIZE_WORDS     (1024)
#else
    #error "Device family must be defined. see nrf.h"
#endif

/** @} */

#endif // FS_CONFIG_H__

