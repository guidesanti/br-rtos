/**
 * @file        ipc.h
 * @date        2012, July 30
 * @author      Guilherme A. de Santi <guidesanti@yahoo.com.br>
 * @copyright   &copy; 2012, BR-RTOS Development Team
 * @brief       TODO: add brief description
 *
 * This file is part of the BR-RTOS.
 * The license distribution terms may found at COPYING or LICENSE file
 * within the source directory tree.
 */

/**
 * @addtogroup IPC Inter Process Communication
 * @{
 */

/******************************************************************************/
/* I N C L U D E S                                                            */
/******************************************************************************/
#include "ipc.h"


/******************************************************************************/
/* C O N S T A N T ,  M A C R O  A N D  T Y P E  D E F I N I T I O N S        */
/******************************************************************************/

/**
 * @name Kernel Internal Definitions
 * @{
 */

/** @} */

/******************************************************************************/
/* P R I V A T E  F U N C T I O N S  P R O T O T Y P E S                      */
/******************************************************************************/


/******************************************************************************/
/* K E R N E L  I N T E R N A L  V A R I A B L E S                            */
/******************************************************************************/

/**
 * @name Kernel Internal Variables
 * @{
 */

/** @} */

/******************************************************************************/
/* K E R N E L  I N T E R N A L  F U N C T I O N S                            */
/******************************************************************************/

/**
 * @name Kernel Internal Functions
 * @{
 */

/**
 * @brief TODO
 */
void __BR_IpcInit(void)
{
  // TODO
}

/** @} */

/******************************************************************************/
/* P U B L I C  A P I  F U N C T I O N S                                      */
/******************************************************************************/

/**
 * @name Public API Functions
 * @{
 */

/**
 * @brief TODO
 */
BR_Mutex_t* BR_IpcMutexCreate(void)
{
  // TODO
  return E_ENOSYS;
}

/**
 * @brief TODO
 */
BR_Err_t BR_IpcMutexAcquire(BR_Mutex_t* mutex)
{
  // TODO
  return E_ENOSYS;
}

/**
 * @brief TODO
 */
BR_Err_t BR_IpcMutexRelease(BR_Mutex_t* mutex)
{
  // TODO
  return E_ENOSYS;
}

/**
 * @brief TODO
 */
BR_Err_t BR_IpcMutexControl(BR_Mutex_t* mutex, uint8_t cmd, void* param)
{
  return E_ENOSYS;
}

/** @} */

/** @} */

