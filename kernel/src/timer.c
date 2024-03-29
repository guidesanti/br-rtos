/**
 * @file        timer.c
 * @date        2012, July 27
 * @author      Guilherme A. de Santi <guidesanti@yahoo.com.br>
 * @copyright   Guilherme A. de Santi &copy; 2012
 * @brief       Implementation of the timer control API.
 *
 * TODO: Add detailed description
 */

/**
 * @addtogroup TimerCtrl Timer Control
 * @{
 */

/******************************************************************************/
/* I N C L U D E S                                                            */
/******************************************************************************/
#include "BR-RTOS.h"
#include "port.h"
#include "list.h"
#include "timer.h"
#include "object.h"


/******************************************************************************/
/* C O N S T A N T ,  M A C R O  A N D  T Y P E  D E F I N I T I O N S        */
/******************************************************************************/

/**
 * @name Constants, Macro and Type Definitions
 * @{
 */

/* @{ */

/**
 * @name Timer flags
 * Definition of timer flags.
 */
#define __BR_TIMER_FLAG_SHOT  (BIT_MASK_0)
#define __BR_TIMER_FLAG_BASE  (BIT_MASK_1)
#define __BR_TIMER_FLAG_TYPE  (BIT_MASK_2)

/** @} */

/** @} */


/******************************************************************************/
/* P R I V A T E  F U N C T I O N S  P R O T O T Y P E S                      */
/******************************************************************************/


/******************************************************************************/
/* V A R I A B L E S                                                          */
/******************************************************************************/

/**
 * The timer task.
 */
BR_Task_t* timerTask = NULL;

/**
 * The list of running timers.
 */
static BR_ListNode_t runningTimers;

/**
 * The list of expired timers to be handled by the timer task (soft timers).
 */
static BR_ListNode_t expiredTimers;


/******************************************************************************/
/* F U N C T I O N S                                                          */
/******************************************************************************/

/**
 * @brief Initializes the timer control sub-system.
 */
void __BR_TimerStartUpInit(void)
{
  /* Initialize the running timers list and the soft expired timers list. */
  __BR_ListInit(&runningTimers);
  __BR_ListInit(&expiredTimers);
}

/**
 * @brief Updates the current running timers.
 */
void __BR_TimerDecrement(void)
{
  BR_Timer_t* timer = NULL;
  BR_ListNode_t* node = runningTimers.next;

  while (node != &runningTimers)
  {
    timer = __BR_LIST_ENTRY(node, BR_Timer_t, node);
    node = node->next;
    timer->counter--;
    if (0U == timer->counter)
    {
      /* Treating hard timers.*/
      if (BR_TIMER_FLAG_HARD == BITS_GET(timer->flags, __BR_TIMER_FLAG_TYPE))
      {
        timer->callbak(timer->param);
        if (BR_TIMER_FLAG_CONT == BITS_GET(timer->flags, __BR_TIMER_FLAG_SHOT))
        {
          timer->counter = timer->reload;
        }
        else
        {
          __BR_ListRemove(&(timer->node));
        }
      }
      /* Treating soft timers. */
      else
      {
        __BR_ListRemove(&(timer->node));
        __BR_ListInsertBefore(&expiredTimers, &(timer->node));
      }
    }
  }
  if (FALSE == __BR_ListIsEmpty(&expiredTimers))
  {
    BR_TaskResume(timerTask);
  }
}

/**
 * @brief Implementation of the timer task.
 */
void __BR_TimerTask(void)
{
  BR_Timer_t* timer = NULL;
  BR_ListNode_t* node = NULL;

  while (TRUE)
  {
    __BR_ENTER_CRITICAL();
    node = expiredTimers.next;
    __BR_EXIT_CRITICAL();
    while (node != &expiredTimers)
    {
      __BR_ENTER_CRITICAL();
      timer = __BR_LIST_ENTRY(node, BR_Timer_t, node);
      node = node->next;
      __BR_ListRemove(&(timer->node));
      timer->callbak(timer->param);
      if (BR_TIMER_FLAG_CONT == BITS_GET(timer->flags, __BR_TIMER_FLAG_SHOT))
      {
        timer->counter = timer->reload;
        __BR_ListInsertAfter(&runningTimers, &(timer->node));
      }
      __BR_EXIT_CRITICAL();
    }
    BR_TaskSuspend(timerTask);
  }
}

/**
 * @brief Initialize a statically created timer.
 * @param [in] timer A pointer to the statically allocated timer.
 * @param [in] name The timer name.
 * @param [in] time The value to count down to zero before calling the callback.
 * @param [in] callback A pointer to the function to be executed when the timer expires.
 * @param [in] param An optional parameter to be passed back to the callback.
 * @param [in] flags The timer resource flags.
 * @return Error code:
 * @retval E_OK If the timer has been initialized successfully.
 * @retval E_INAL If any of the parameters is invalid and the parameters check is enabled.
 *
 * Initializes a timer resource and insert it within the kernel objects list.
 *  * The flags are used to specify some timer configurations:
 * - BR_TIMER_FLAG_SINGLE -> Single shot timer.
 * - BR_TIMER_FLAG_CONT   -> Continuous timer, the timer will restart automatically after executing the callback.
 * - BR_TIMER_FLAG_MS     -> Milliseconds based timer.
 * - BR_TIMER_FLAG_SEC    -> Seconds based timer.
 * - BR_TIMER_FLAG_SOFT   -> Soft timer, the callback will be executed from timer task.
 * - BR_TIMER_FLAG_HARD   -> Hard timer, the callback will be executed from interrupt handler.
 *
 * If the flags parameter is zero, it is assumed a single shot timer with
 * milliseconds base time and a soft timer.
 */
BR_Err_t BR_TimerInit(BR_Timer_t* timer, const char* name, uint32_t time, BR_TimerCallback_t callback, void* param, uint8_t flags)
{
  BR_Err_t ret = E_OK;

  /* Check the parameters. */
  __BR_ASSERT(NULL != timer);
  __BR_ASSERT(NULL != name);
  __BR_ASSERT(NULL != callback);
#if (1U == __BR_CHECK_FUNC_PARAMETERS)
  if ((NULL != timer) && (NULL != name) && (NULL != callback))
#endif
  {
    __BR_ENTER_CRITICAL();

    /* Initialize the timer object. */
    __BR_ObjectInit(&(timer->parent), BR_OBJ_TYPE_TIMER, name);
    /* Initialize the timer attributes. */
    if (BR_TIMER_FLAG_SEC == BITS_GET(flags, __BR_TIMER_FLAG_BASE))
    {
      time *= 1000U;
    }
    __BR_ListInit(&(timer->node));
    timer->counter = 0U;
    timer->reload = time;
    timer->callbak = callback;
    timer->param = param;
    timer->flags = flags;

    __BR_EXIT_CRITICAL();
  }
#if (1U == __BR_CHECK_FUNC_PARAMETERS)
  else
  {
    ret = E_INVAL;
  }
#endif

  return ret;
}

/**
 * @brief Create a new timer resource.
 * @param [in] name The timer name.
 * @param [in] time The value to count down to zero before calling the callback.
 * @param [in] callback A pointer to the function to be executed when the timer expires.
 * @param [in] param An optional parameter to be passed back to the callback.
 * @param [in] flags The timer resource flags.
 * @return Error code:
 * @retval E_OK If the timer has been created successfully.
 * @retval E_INAL If any of the parameters is invalid and the parameters check is enabled.
 * @retval E_NORES If there no enough memory to allocate to the timer resource.
 *
 * Creates a new timer resource. A pointer to the new created timer is returned within
 * the timer pointer.
 * If there is no enough memory to create the timer E_NORES will be returned.
 * The flags are used to specify some timer configurations:
 * - BR_TIMER_FLAG_SINGLE -> Single shot timer.
 * - BR_TIMER_FLAG_CONT   -> Continuous timer, the timer will restart automatically after executing the callback.
 * - BR_TIMER_FLAG_MS     -> Milliseconds based timer.
 * - BR_TIMER_FLAG_SEC    -> Seconds based timer.
 * - BR_TIMER_FLAG_SOFT   -> Soft timer, the callback will be executed from timer task.
 * - BR_TIMER_FLAG_HARD   -> Hard timer, the callback will be executed from interrupt handler.
 *
 * If the flags parameter is zero, it is assumed a single shot timer with
 * milliseconds base time and a soft timer.
 */
BR_Timer_t* BR_TimerCreate(const char* name, uint32_t time, BR_TimerCallback_t callback, void* param, uint8_t flags)
{
  BR_Timer_t* timer = NULL;

  __BR_ASSERT(NULL != callback);

#if (1U == __BR_CHECK_FUNC_PARAMETERS)
  if (NULL != callback)
  {
#endif
    timer = NULL;
    __BR_ENTER_CRITICAL();
    /* Allocating memory for the timer. */
    timer = BR_MemAlloc(sizeof(BR_Timer_t));
    if (NULL != timer)
    {
      /* Initialize the timer object. */
      __BR_ObjectInit(&(timer->parent), BR_OBJ_TYPE_TIMER, name);
      /* Initialize the timer attributes. */
      if (BR_TIMER_FLAG_SEC == BITS_GET(flags, __BR_TIMER_FLAG_BASE))
      {
        time *= 1000U;
      }
      __BR_ListInit(&(timer->node));
      timer->counter = 0U;
      timer->reload = time;
      timer->callbak = callback;
      timer->param = param;
      timer->flags = flags;
    }
    __BR_EXIT_CRITICAL();
#if (1U == __BR_CHECK_FUNC_PARAMETERS)
  }
#endif

  return timer;
}

/**
 * @brief Get/set parameters to a timer resource.
 * @param [in] timer A pointer to the timer to get/set parameters.
 * @param [in] cmd The command to execute.
 * @param [in] param A pointer to the parameter of the command.
 * @return Error code:
 * @retval E_OK If the command is successfully executed.
 * @retval E_INVAL If any of the parameters is invalid and the parameters check is enabled.
 */
BR_Err_t BR_TimerControl(BR_Timer_t* timer, BR_TimerCmd_t cmd, void* param)
{
  BR_Err_t ret = E_OK;

  __BR_ASSERT(NULL != timer);
  __BR_ASSERT(cmd < BR_N_TIMER_CMDS);

#if (1U == __BR_CHECK_FUNC_PARAMETERS)
  if ((NULL != timer) && (cmd < BR_N_TIMER_CMDS))
  {
#endif
    switch (cmd)
    {
      case BR_TIMER_CMD_SET_TIME:
      {
        __BR_ASSERT(NULL != param);
        timer->reload = *((uint32_t*)param);
      }
      break;

      case BR_TIMER_CMD_GET_TIME:
      {
        __BR_ASSERT(NULL != param);
        *((uint32_t*)param) = timer->reload;
      }
      break;

      case BR_TIMER_CMD_SET_SINGLE:
      {
        BITS_CLEAR(timer->flags, __BR_TIMER_FLAG_SHOT);
      }
      break;

      case BR_TIMER_CMD_SET_CONT:
      {
        BITS_SET(timer->flags, __BR_TIMER_FLAG_SHOT);
      }
      break;

      case BR_TIMER_CMD_SET_MS:
      {
        BITS_CLEAR(timer->flags, __BR_TIMER_FLAG_BASE);
      }
      break;

      case BR_TIMER_CMD_SET_SEC:
      {
        BITS_SET(timer->flags, __BR_TIMER_FLAG_BASE);
      }
      break;

      case BR_TIMER_CMD_SET_SOFT:
      {
        BITS_CLEAR(timer->flags, __BR_TIMER_FLAG_TYPE);
      }
      break;

      case BR_TIMER_CMD_SET_HARD:
      {
        BITS_SET(timer->flags, __BR_TIMER_FLAG_TYPE);
      }
      break;

      default:
      {
        ret = E_INVAL;
      }
      break;
    }
#if (1U == __BR_CHECK_FUNC_PARAMETERS)
  }
  else
  {
    ret = E_INVAL;
  }
#endif

  return ret;
}

/**
 * @brief Starts a previously created timer.
 * @param [in] timer A pointer to the timer.
 * @return Error code.
 * @retval E_OK If the timer is successfully started.
 * @retval E_INVAL If timer is NULL and the check parameters feature is enabled.
 */
BR_Err_t BR_TimerStart(BR_Timer_t* timer)
{
  BR_Err_t ret = E_OK;

  /* Check the parameters. */
  __BR_ASSERT(NULL != timer);
#if (1U == __BR_CHECK_FUNC_PARAMETERS)
  if (NULL != timer)
#endif
  {
    __BR_ENTER_CRITICAL();
    timer->counter = timer->reload;
    __BR_ListRemove(&(timer->node));
    __BR_ListInsertAfter(&runningTimers, &(timer->node));
    __BR_EXIT_CRITICAL();
  }
#if (1U == __BR_CHECK_FUNC_PARAMETERS)
  else
  {
    ret = E_INVAL;
  }
#endif

  return ret;
}

/**
 * @brief Stops a previously started timer.
 * @param [in] timer A pointer to the timer.
 * @return Error code.
 * @retval E_OK If the timer is successfully stopped.
 * @retval E_INVAL If timer is NULL and the check parameters feature is enabled.
 */
BR_Err_t BR_TimerStop(BR_Timer_t* timer)
{
  BR_Err_t ret = E_OK;

  __BR_ASSERT(NULL != timer);

#if (1U == __BR_CHECK_FUNC_PARAMETERS)
  if (NULL != timer)
#endif
  {
    __BR_ENTER_CRITICAL();
    timer->counter = 0U;
    __BR_ListRemove(&(timer->node));
    __BR_EXIT_CRITICAL();
  }
#if (1U == __BR_CHECK_FUNC_PARAMETERS)
  else
  {
    ret = E_INVAL;
  }
#endif

  return ret;
}

/**
 * @brief Restarts a timer.
 * @param [in] timer A pointer to the timer.
 * @return Error code.
 * @retval E_OK If the timer is successfully restarted.
 * @retval E_INVAL If timer is NULL and the check parameters feature is enabled.
 */
BR_Err_t BR_TimerRestart(BR_Timer_t* timer)
{
  BR_Err_t ret = E_OK;

  __BR_ASSERT(NULL != timer);

#if (1U == __BR_CHECK_FUNC_PARAMETERS)
  if (NULL != timer)
#endif
  {
    __BR_ENTER_CRITICAL();
    timer->counter = timer->reload;
    __BR_ListRemove(&(timer->node));
    __BR_ListInsertAfter(&runningTimers, &(timer->node));
    __BR_EXIT_CRITICAL();
  }
#if (1U == __BR_CHECK_FUNC_PARAMETERS)
  else
  {
    ret = E_INVAL;
  }
#endif

  return ret;
}

/** @} */
