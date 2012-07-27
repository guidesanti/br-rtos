/**
 * @file        heap.c
 * @date        2012, July 20
 * @author      Guilherme A. de Santi <guidesanti@yahoo.com.br>
 * @copyright   Guilherme A. de Santi &copy; 2012
 * @brief       TODO: Add brief description
 *
 * TODO: Add detailed description
 */


/******************************************************************************/
/* I N C L U D E S                                                            */
/******************************************************************************/

/**
 * @defgroup Includes Header File Include
 * @{
 */

#include "BR-RTOS.h"
#include "BR-RTOSConfig.h"
#include "port.h"

/**@}*/

/******************************************************************************/
/* C O N S T A N T ,  M A C R O  A N D  T Y P E  D E F I N I T I O N S       */
/******************************************************************************/

/**
 * @defgroup Defs Constant, Macro and Type Definitions
 * @{
 */

/**@}*/

/******************************************************************************/
/* P R I V A T E  F U N C T I O N S  P R O T O T Y P E S                      */
/******************************************************************************/

/**
 * @defgroup PrivateFuncProto Private Function Prototypes
 * @{
 */

/**@}*/

/******************************************************************************/
/* P R I V A T E  V A R I A B L E S                                           */
/******************************************************************************/

/**
 * @defgroup PrivateVar Private Variables
 * @{
 */

static uint8_t heap[__BR_HEAP_SIZE];

static uint8_t* nextFreeByte = heap;

static uint32_t bytesFree = __BR_HEAP_SIZE;

/**@}*/

/******************************************************************************/
/* P U B L I C  V A R I A B L E S                                             */
/******************************************************************************/

/**
 * @defgroup PublicVar Public Variables
 * @{
 */

/**@}*/

/******************************************************************************/
/* P R I V A T E  F U N C T I O N S                                           */
/******************************************************************************/

/**
 * @defgroup PrivateFunc Private Functions
 * @{
 */

/**
 * @brief Allocate an amount of memory.
 * @param [in] nBytes The number of bytes to allocate.
 * @return A pointer to the first byte of the allocated block or NULL if there
 *         is no enough free memory.
 *
 * This function will allocate a contiguous block of memory with size specified
 * by the nBytes parameter and on success will return a pointer to the first
 * byte of the allocated block.
 * If there is no enough free memory NULL will be returned.
 * It is responsibility of the user code to not overrun the allocated memory,
 * otherwise unpredictable events may occur like the system crash or freeze, or
 * any other dynamic allocated memory me be corrupted.
 * Use this function sparingly.
 */
void* BR_MemAlloc(BR_Size_t nBytes)
{
  uint8_t* ptr = NULL;
  uint32_t index = 0U;

  /* Check if there is enough memory to allocate. */
  if (bytesFree >= nBytes)
  {
    ptr = nextFreeByte;
    nextFreeByte += nBytes;
    bytesFree -= nBytes;
    /* Clear all bytes within the allocated memory. */
    for (index = 0U; index < nBytes; index++)
    {
      *(ptr + index) = 0x00U;
    }
  }
  else
  {
    __BR_ASSERT(FALSE);
  }

  return ptr;
}

/**@}*/

/******************************************************************************/
/* P U B L I C  F U N C T I O N S                                             */
/******************************************************************************/

/**
 * @defgroup PublicFunc Public Functions
 * @{
 */



/**@}*/
