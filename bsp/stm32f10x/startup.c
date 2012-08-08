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
#include "port.h"
#include "startup.h"
#include "object.h"
#include "task.h"
#include "timer.h"
#include "ipc.h"
#include "device.h"
#include "board.h"

/******************************************************************************/
/* F U N C T I O N S  P R O T O T Y P E S                                     */
/******************************************************************************/
extern void BR_AppInit(void);
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
  __BR_ObjectInit();
  __BR_TasklInit();
  __BR_TimerInit();
  __BR_IpcInit();
  __BR_DeviceInit();

  /* Board initialization. */
  __BR_BoardInit();

  /* Device drivers initializetion. */
  __BR_DeviceInitAll();

  /* Create the idle task. */
  BR_TaskCreate("Idle", __BR_IdleTask, 32U, NULL, BR_TASK_PRIORITY_TRIVIAL, NULL);

  /* Create the timer task. */
  BR_TaskCreate("Timer Task", __BR_TimerTask, 40U, NULL, BR_TASK_PRIORITY_CRITICAL, &timerTask);

  /* Application initialization. */
  BR_AppInit();

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
  __BR_ASSERT(FALSE);
#else
  /* Force a system reset. */
  __BR_RESET();
#endif
}


/** @} */

