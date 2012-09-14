/**
 * @file        startup.c
 * @date        2012, August 01
 * @author      Guilherme A. de Santi <guidesanti@yahoo.com.br>
 * @copyright   &copy; 2012, BR-RTOS Development Team
 * @brief       TODO: add brief description
 *
 * This file is part of the BR-RTOS.
 * The license distribution terms may found at COPYING or LICENSE file
 * within the source directory tree.
 */

/**
 * @addtogroup STM32F10xBSP STM32F10x BSP
 * @{
 */

/******************************************************************************/
/* I N C L U D E S                                                            */
/******************************************************************************/
#include "BR-RTOSConfig.h"
#include "BR-RTOS.h"
#include "startup.h"
#include "init.h"
#include "task.h"
#include "timer.h"
#include "device.h"

/* If using BSP include the default board header file. */
#if (1U == __BR_USE_BSP)
#include "board.h"
#endif

/******************************************************************************/
/* F U N C T I O N S  P R O T O T Y P E S                                     */
/******************************************************************************/
extern void AppInit(void);
static void __BR_StartUp(void);

/******************************************************************************/
/* C O N S T A N T ,  M A C R O  A N D  T Y P E  D E F I N I T I O N S        */
/******************************************************************************/

/******************************************************************************/
/* F U N C T I O N S                                                          */
/******************************************************************************/

/**
 * @brief System start up.
 */
static void __BR_StartUp(void)
{
  /* Initialize the internal BR-RTOS kernel structures. */
  __BR_InitKernel();

  /* Board initialization. */
#if (1U == __BR_USE_BSP)
  __BR_BoardInit();
#endif

  /* Device drivers initialization. */
  __BR_DeviceInitAll();

  /* Create the idle task. */
  BR_TaskCreate("Idle", __BR_IdleTask, 32U, NULL, BR_TASK_PRIORITY_TRIVIAL);

  /* Create the timer task. */
  timerTask = BR_TaskCreate("Timer Task", __BR_TimerTask, 40U, NULL, BR_TASK_PRIORITY_CRITICAL);

  /* Application initialization. */
  AppInit();

  /* Start the scheduler. */
  __BR_TaskStartScheduler();
}

void main(void)
{
  /* Disable interrupts. */
  __BR_DISABLE_INTERRUPTS();

  /* Start up the BR-RTOS and the user application. */
  __BR_StartUp();

  /* Should never reach here. */
#if (1U == __BR_DEBUG)
  /* Stop the software for debugging. */
  BR_ASSERT(FALSE);
#else
  /* Force a system reset. */
  __BR_RESET();

  /* If there is no support for reset by software, enter the infinite loop.
   * A chance for the watchdog (if enabled) to reset the system.
   */
  while (TRUE) { }
#endif
}


/** @} */

