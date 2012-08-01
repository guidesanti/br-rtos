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
      mutex->counter = 0U;
    }
  }

  __BR_EXIT_CRITICAL();

  return mutex;
}

/**
 * @brief Attempt to acquire a mutex.
 * @param [in] mutex A pointer to the mutex.
 * @param [in] time A timeout value to wait for the mutex (in milliseconds).
 * @return Error code.
 * @retval E_OK If the mutex is successfully acquired.
 * @retval E_TIMEOUT If the timeout has been reached and the mutex was not acquired.
 *
 * This function will put the task in waiting state if the mutex is already acquired
 * by another task. If the timeout occurs the task requesting the mutex will be
 * put back in ready state and the function will return with E_TIMEOUT.
 * If the mutex is successfully acquired E_OK is returned.
 * The mutex may be recursively acquired by the same task, in this case the
 * task will need to release the mutex the same number of times t has acquired
 * the mutex before it can be acquired by another task.
 */
BR_Err_t BR_IpcMutexAcquire(BR_Mutex_t* mutex, uint32_t time)
{
  BR_Err_t ret = E_OK;

  __BR_ASSERT(NULL != mutex);

  __BR_ENTER_CRITICAL();

#if (1U == __BR_CHECK_FUNC_PARAMETERS)
  if (NULL != mutex)
#endif
  {
    if (NULL == mutex->owner)
    {
      mutex->owner = runningTask;
      mutex->counter++;
    }
    else if (mutex->owner == runningTask)
    {
      mutex->counter++;
    }
    else
    {
#warning "TODO: Put the task in the mutex waiting list"
      // TODO
    }
  }
#if (1U == __BR_CHECK_FUNC_PARAMETERS)
  else
  {
    ret = E_INVAL;
  }
#endif

  __BR_EXIT_CRITICAL();

  return ret;
}

/**
 * @brief Release a mutex.
 * @param [in] mutex A pointer to the mutex to be released.
 * @return Error code.
 * @retval E_OK If the mutex is released successfully.
 * @retval E_ERROR If the owner of the mutex is note the task that is calling this function.
 * @retval E_INVAL If mutex == NULL and the check parameters feature is enabled.
 *
 * This function will release a previously acquired mutex if the owner of the
 * mutex is the task that has been acquired it.
 * On successful release of the mutex E_OK is returned, otherwise E_ERROR
 * is returned.
 */
BR_Err_t BR_IpcMutexRelease(BR_Mutex_t* mutex)
{
  BR_Err_t ret = E_OK;

  __BR_ASSERT(NULL != mutex);

  __BR_ENTER_CRITICAL();

#if (1U == __BR_CHECK_FUNC_PARAMETERS)
  if (NULL != mutex)
#endif
  {
    if (mutex->owner == runningTask)
    {
      mutex->counter--;
      if (0U == mutex->counter)
      {
        mutex->owner = NULL;
#warning "TODO: Check if there is some task waiting for this mutex"
      }
    }
    else
    {
      ret = E_ERROR;
    }
  }
#if (1U == __BR_CHECK_FUNC_PARAMETERS)
  else
  {
    ret = E_INVAL;
  }
#endif

  __BR_EXIT_CRITICAL();

  return ret;
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

