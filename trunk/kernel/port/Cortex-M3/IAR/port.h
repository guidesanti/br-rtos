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
 * @addtogroup PortLayer Port Layer
 * @{
 */

/******************************************************************************/
/* I N C L U D E S                                                            */
/******************************************************************************/
#include "BR-RTOSDefs.h"


/******************************************************************************/
/* C O N S T A N T ,  M A C R O  A N D  T Y P E  D E F I N I T I O N S        */
/******************************************************************************/

/**
 * @name Constants, Macros and Type definitions
 * @{
 */

#define __BR_ENTER_CRITICAL()     __BR_PortDisableIRQ()
#define __BR_EXIT_CRITICAL()      __BR_PortEnableIRQ()
#define __BR_DISABLE_INTERRUPTS() __BR_PortDisableIRQ()
#define __BR_ENABLE_INTERRUPTS()  __BR_PortEnableIRQ()
#define __BR_RESET()              __BR_PortReset()

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

/** @} */


/******************************************************************************/
/* K E R N E L  I N T E R N A L  F U N C T I O N S                            */
/******************************************************************************/
BR_StackPointer_t __BR_PortInitStack(BR_StackPointer_t stackPointer, void (*run)(void), void* param);
void __BR_PortSchedulerStart(void);
void __BR_PortYield(void);
void __BR_PortSVCHandler(void);
void __BR_PortPendSVCHandler(void);
void __BR_PortSysTickHandler(void);
void __BR_PortReset(void);
void __BR_PortEnableIRQ(void);
void __BR_PortDisableIRQ(void);
void __BR_PortSetBasePriorityMask(uint8_t basePriMask);
void __BR_PortStartFirstTask(void);


/** @} */

#endif /* _PORT_H_ */
