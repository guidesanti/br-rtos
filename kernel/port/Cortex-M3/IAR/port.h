/**
 * @file        port.h
 * @date        2012, July 18
 * @author      Guilherme A. de Santi <guidesanti@yahoo.com.br>
 * @copyright   Guilherme A. de Santi &copy; 2012
 * @brief       Cortex-M3/IAR port header file.
 *
 * This is the port.h header file specific for the Cortex-M3 CPU and to be
 * compiled by the IAR compiler.
 */


#ifndef _PORT_H_
#define _PORT_H_

/**
 * @defgroup ModuleName The Module Name
 * @{
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

/**@}*/

/******************************************************************************/
/* C O N S T A N T ,  M A C R O  A N D  T Y P E  D E F I N I T I O N S       */
/******************************************************************************/

/**
 * @defgroup Defs Constant, Macro and Type Definitions
 * @{
 */

/**
 * The initial value to be set for the xPSR register when the task is created.
 */
#define __BR_INITIAL_XPSR (0x01000000U)

/**
 * The Cortex-M3 CPU word length.
 */
#define __BR_WORD_LEN     (4U)

/**
 * Definition of the assert macro.
 */
#if (1U == __BR_USE_ASSERT)

#define __BR_ASSERT(EXPRESSION)       \
{                                     \
  if (!(EXPRESSION)) asm("bkpt #0");  \
}

#else

#define __BR_ASSERT(EXPRESSION) ((void)0U)

#endif

/**
 * A new type to represent the stack pointer.
 */
typedef uint32_t* BR_StackPointer_t;

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
/* P U B L I C  F U N C T I O N S                                             */
/******************************************************************************/

/**
 * @defgroup PublicFunc Public Functions
 * @{
 */

BR_StackPointer_t BR_PortInitStack(BR_StackPointer_t stackPointer, void (*run)(void), void* param);
void BR_PortSchedulerStart(void);
void BR_PortSVCHandler(void);
void BR_PortPendSVCHandler(void);
void BR_PortSysTickHandler(void);
void BR_PortEnableIRQ(void);
void BR_PortDisableIRQ(void);
void BR_PortSetBasePriorityMask(uint8_t basePriMask);
void BR_PortStartFirstTask(void);

/**@}*/

/**@}*/


#endif /* _PORT_H_ */
