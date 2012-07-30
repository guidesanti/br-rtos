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
#include "port.h"


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
 * @brief Creates a mutex.
 * @return NULL if the mutex can not be created, otherwise returns a pointer to
 * the created mutex.
 *
 * If there is enough memory to create the mutex it will be created and initialized
 * and returned as a pointer.
 * If there is no enough memory to create the mutex a NULL is returned.
 */
BR_Mutex_t* BR_IpcMutexCreate(void)
{
  BR_Mutex_t* mutex = NULL;
  BR_Object_t* obj = NULL;

  __BR_ENTER_CRITICAL();

  /* Allocate memory for the mutex object. */
  obj = __BR_Malloc(sizeof(BR_Object_t));
  if (NULL != obj)
  {
    /* Allocate memory for the mutex. */
    mutex = __BR_Malloc(sizeof(BR_Mutex_t));
    if (NULL != mutex)
    {
      mutex->parent = obj;
      mutex->owner = NULL;
      mutex->state = __BR_MUTEX_RELEASED;
    }
  }

  __BR_EXIT_CRITICAL();

  return mutex;
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
 * @brief Executed a control command to a mutex.
 * @return Error code.
 * @retval E_ENOSYS Always returns this value.
 * @warning This function is currently not supported.
 */
BR_Err_t BR_IpcMutexControl(BR_Mutex_t* mutex, uint8_t cmd, void* param)
{
  return E_ENOSYS;
}

/** @} */

/** @} */

