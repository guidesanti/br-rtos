/**
 * @file        ipc.h
 * @date        2012, July 30
 * @author      Guilherme A. de Santi <guidesanti@yahoo.com.br>
 * @copyright   &copy; 2012, BR-RTOS Development Team
 * @brief       Implementation of the IPC (Inter Process Communication) API.
 *
 * This file is part of the BR-RTOS.
 * The license distribution terms may found at COPYING or LICENSE file
 * within the source directory tree.
 */

/**
 * @addtogroup IPC IPC (Inter Process Communication)
 * @{
 */

/******************************************************************************/
/* I N C L U D E S                                                            */
/******************************************************************************/
#include "BR-RTOS.h"
#include "ipc.h"
#include "port.h"
#include "list.h"
#include "object.h"
#include "task.h"


/******************************************************************************/
/* C O N S T A N T ,  M A C R O  A N D  T Y P E  D E F I N I T I O N S        */
/******************************************************************************/


/******************************************************************************/
/* P R I V A T E  F U N C T I O N S  P R O T O T Y P E S                      */
/******************************************************************************/


/******************************************************************************/
/* V A R I A B L E S                                                          */
/******************************************************************************/


/******************************************************************************/
/* F U N C T I O N S                                                          */
/******************************************************************************/

/**
 * @brief Creates a mutex.
 * @param [in] name The mutex name.
 * @return NULL if the mutex can not be created, otherwise returns a pointer to
 * the created mutex.
 *
 * If there is enough memory to create the mutex it will be created and initialized
 * and returned as a pointer.
 * If there is no enough memory to create the mutex a NULL is returned.
 */
BR_Mutex_t* BR_IpcMutexCreate(const char* name)
{
  BR_Mutex_t* mutex = NULL;

  __BR_ASSERT(NULL != name);

  __BR_ENTER_CRITICAL();

  /* Allocate memory for the mutex. */
  mutex = BR_MemAlloc(sizeof(BR_Mutex_t));
  if (NULL != mutex)
  {
    /* Initialize the mutex parent object. */
    __BR_ObjectInit(&(mutex->parent), BR_OBJ_TYPE_MUTEX, name);
    /* Initialize the mutex attributes. */
    mutex->owner = NULL;
    mutex->counter = 0U;
    __BR_ListInit(&(mutex->waitList));
  }

  __BR_EXIT_CRITICAL();

  return mutex;
}

/**
 * @brief Initialize a static created mutex.
 * @param [in] mutex A pointer to the mutex.
 * @param [in] name The mutex name.
 * @return Error code.
 * @retval E_OK If the mutex is successfully initialized.
 */
void BR_IpcMutexInit(BR_Mutex_t* mutex, const char* name)
{
  /* Check the parameters. */
  __BR_ASSERT(NULL != mutex);
  __BR_ASSERT(NULL != name);

  __BR_ENTER_CRITICAL();

  /* Initialize the mutex parent object. */
  __BR_ObjectInit(&(mutex->parent), BR_OBJ_TYPE_MUTEX, name);
  /* Initialize the mutex attributes. */
  mutex->owner = NULL;
  mutex->counter = 0U;
  __BR_ListInit(&(mutex->waitList));

  __BR_EXIT_CRITICAL();
}

/**
 * @brief Attempt to acquire a mutex.
 * @param [in] mutex A pointer to the mutex.
 * @param [in] ticks A timeout value to wait for the mutex (number of system ticks).
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
BR_Err_t BR_IpcMutexAcquire(BR_Mutex_t* mutex, uint32_t ticks)
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
      mutex->counter = 1U;
    }
    else if (mutex->owner == runningTask)
    {
      mutex->counter++;
    }
    else
    {
      if (ticks > 0U)
      {
        /* Put the task within the mutex waiting list. */
        __BR_ListInsertBefore(&(mutex->waitList), &(runningTask->resWaitList));
        /* Set the task error code. */
        runningTask->errorCode = __BR_TASK_ERR_UNKNOW;
        /* Put the task in waiting or suspended state depending on ticks value. */
        if (BR_IPC_WAIT_FOREVER == ticks)
        {
          BR_TaskSuspend(runningTask);
        }
        else
        {
          BR_TaskWait(ticks);
        }
        /* Exit critical section to allow the task switch. */
        __BR_EXIT_CRITICAL();
        /* Check the task error code. */
        if (__BR_TASK_ERR_OK == runningTask->errorCode)
        {
          mutex->owner = runningTask;
          mutex->counter = 1U;
          ret = E_OK;
        }
        else
        {
          __BR_ListRemove(&(runningTask->resWaitList));
          ret = E_TIMEOUT;
        }
      }
      else
      {
        ret = E_ERROR;
      }
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
  BR_Task_t* task = NULL;

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
        /* Check if there is some task waiting for this mutex. */
        if (FALSE == __BR_ListIsEmpty(&(mutex->waitList)))
        {
          /* Get the first task within the waiting list. */
          task = __BR_LIST_ENTRY(mutex->waitList.next, BR_Task_t, resWaitList);
          /* Remove the task fro mmutes wait list. */
          __BR_ListRemove(&(task->resWaitList));
          /* Set the task error code. */
          task->errorCode = __BR_TASK_ERR_OK;
          /* Resume the task. */
          BR_TaskResume(task);
          /* Exit critical section to allow the context switch. */
          __BR_EXIT_CRITICAL();
        }
      }
      ret = E_OK;
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
