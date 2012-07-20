/**
 * @file        iar_port.c
 * @date        2012, July 18
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

#include "task.h"
#include "port.h"
#include "stm32f10x.h"

/**@}*/

/******************************************************************************/
/* C O N S T A N T ,  M A C R O  A N D  T Y P E  D E F I N I T I O N S       */
/******************************************************************************/

/**
 * @defgroup Defs Constant, Macro and Type Definitions
 * @{
 */

/* SCB registers. */
#define __BR_SCB_ICSR (*((volatile uint32_t*)0xE000ED04U))

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

/**@}*/

/******************************************************************************/
/* P U B L I C  F U N C T I O N S                                             */
/******************************************************************************/

/**
 * @defgroup PublicFunc Public Functions
 * @{
 */

BR_StackPointer_t BR_PortInitStack(BR_StackPointer_t stackPointer, void (*run)(void), void* param)
{
  /*
   * We are simulating as the task was already running and has been
   * switched off the CPU.
   */
  *(stackPointer) = __BR_INITIAL_XPSR; /* xPSR (program status register). */
  *(--stackPointer) = ( uint32_t ) run; /* PC (program counter). */
  *(--stackPointer) = 0U; /* LR (link register). */
  *(--stackPointer) = 0U; /* R12 */
  *(--stackPointer) = 0U; /* R3 */
  *(--stackPointer) = 0U; /* R2 */
  *(--stackPointer) = 0U; /* R1 */
  *(--stackPointer) = ( uint32_t ) param; /* R0 (parameter register) */
  *(--stackPointer) = 0U; /* R11 */
  *(--stackPointer) = 0U; /* R10 */
  *(--stackPointer) = 0U; /* R9 */
  *(--stackPointer) = 0U; /* R8 */
  *(--stackPointer) = 0U; /* R7 */
  *(--stackPointer) = 0U; /* R6 */
  *(--stackPointer) = 0U; /* R5 */
  *(--stackPointer) = 0U; /* R4 */

  return stackPointer;
}

void BR_PortSchedulerStart(void)
{
  /* Set the PendSV and SysTick interrupts priorities. */
  SCB->SHP[10U] = 0xF0U;
  SCB->SHP[11U] = 0xF0U;

  /* Configure the system tick for every millisecond. */
#warning "FIXME: This is just for testing, set it correctly"
  SysTick_Config(__BR_CPU_CLOCK_HZ);

  /* Start the first task. */
  BR_PortStartFirstTask();
}

void BR_PortSysTickHandler(void)
{
  /* Update the tick counter. */
  //tickCounter++;

  /* Set the PendSV exception to pending state, so a context switch can be executed. */
  __BR_SCB_ICSR |= SCB_ICSR_PENDSVSET;

  /* Call the task tick increment. */
  BR_TaskTickUpdate();
}

/**@}*/
