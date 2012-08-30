/**
 * @file        task.c
 * @date        2012, July 20
 * @author      Guilherme A. de Santi <guidesanti@yahoo.com.br>
 * @copyright   Guilherme A. de Santi &copy; 2012
 * @brief       TODO: Add brief description
 *
 * TODO: Add detailed description
 */

/**
 * @addtogroup TaskCtrl Task Control
 * @{
 */

/******************************************************************************/
/* I N C L U D E S                                                            */
/******************************************************************************/
#include "BR-RTOS.h"
#include "object.h"
#include "timer.h"
#include "task.h"
#include <stdlib.h>
#include <string.h>


/******************************************************************************/
/* C O N S T A N T ,  M A C R O  A N D  T Y P E  D E F I N I T I O N S        */
/******************************************************************************/

/**
 * @name Constants, Macro and Type Definitions
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

/**
 * Points to the task within the running state.
 *
 * This variable points to the current running task and is updated to the new
 * running task on every time the kernel schedules a new task to run.
 *
 * @warning This is part of the kernel and MUST never be touched by the user code.
 */
BR_Task_t* runningTask = NULL;

/**
 * The task priority table.
 *
 * All tasks within the priority table are in the ready state.
 */
static BR_ListNode_t priorityTable[BR_N_TASK_PRIORITIES];

/**
 * The current priority level.
 */
static uint8_t currentPriority = (BR_N_TASK_PRIORITIES - 1U);

/**
 * The list of tasks within suspended state.
 */
static BR_ListNode_t suspendedTaskList;

/**
 * The list of tasks within waiting state.
 */
static BR_ListNode_t waitingTaskList;

/** @} */

/******************************************************************************/
/* K E R N E L  I N T E R N A L  F U N C T I O N S                            */
/******************************************************************************/

/**
 * @name Kernel Internal Functions
 * @{
 */

/**
 * @brief The idle task code.
 *
 * This function implements the code to be executed as the idle task.
 */
void __BR_IdleTask(void)
{
  while (TRUE) { }
}

/**
 * @brief Initialize the kernel.
 */
void __BR_TaskStartUpInit(void)
{
  uint8_t index = 0U;

  /* Initialize the suspended and waiting lists. */
  __BR_ListInit(&suspendedTaskList);
  __BR_ListInit(&waitingTaskList);

  /* Initialize the priority table. */
  for (index = 0U; index < BR_N_TASK_PRIORITIES; index++)
  {
    __BR_ListInit(&priorityTable[index]);
  }
}

/**
 * @brief Start the task scheduler.
 *
 * This function should be called from main() and will not return unless the
 * scheduler is not able to be started.
 * This function will start the task switching and must be called after
 * all tasks has been successfully created.
 */
void __BR_TaskStartScheduler(void)
{
  /* Set the first task to run. */
  while (TRUE == __BR_ListIsEmpty(&(priorityTable[currentPriority])))
  {
    currentPriority--;
  }
  __BR_ASSERT(currentPriority < BR_N_TASK_PRIORITIES);
  runningTask = __BR_LIST_ENTRY(priorityTable[currentPriority].next, BR_Task_t, list);
  __BR_ListRemove(&(runningTask->list));
  __BR_ListInsertBefore(&(priorityTable[currentPriority]), &(runningTask->list));

  /* Start the scheduler. */
  __BR_PortSchedulerStart();
}

/**
 * @brief Select the next task to run.
 * @note This function is part of the kernel and should never be called from the
 * user code.
 *
 * This function selects the next task on the ready list to run.
 */
void __BR_TaskSwitch(void)
{
  while (TRUE == __BR_ListIsEmpty(&(priorityTable[currentPriority])))
  {
    currentPriority--;
  }
  __BR_ASSERT(currentPriority < BR_N_TASK_PRIORITIES);
  runningTask = __BR_LIST_ENTRY(priorityTable[currentPriority].next, BR_Task_t, list);
  __BR_ListRemove(&(runningTask->list));
  __BR_ListInsertBefore(&(priorityTable[currentPriority]), &(runningTask->list));
}


/**
 * @brief Update the task tick counter.
 * @note This function is part of the kernel and should never be called from the
 * user code.
 *
 * This function walk through the waiting list and decrements the tick counter
 * for each task.
 * If the counter expires the task is removed from the waiting list and inserted
 * into the ready list.
 */
void __BR_TaskTickUpdate(void)
{
  BR_Task_t* task = NULL;
  BR_ListNode_t* node = NULL;

  /* Walk through the waiting list and update each task counter. */
  if (FALSE == __BR_ListIsEmpty(&waitingTaskList))
  {
    node = waitingTaskList.next;
    while (node != &waitingTaskList)
    {
      /* Get the task of the node. */
      task = __BR_LIST_ENTRY(node, BR_Task_t, list);
      /* Next node. */
      node = node->next;
      /* Decrement the task counter. */
      task->counter--;
      if (0U == task->counter)
      {
        /* Change the task state to ready and add it to the correct list within the priority table. */
        task->state = __BR_TASK_ST_READY;
        __BR_ListRemove(&(task->list));
        __BR_ListInsertAfter(&priorityTable[task->priority], &(task->list));
        /* Set the current priority. */
        if (task->priority > currentPriority)
        {
          currentPriority = task->priority;
        }
      }
    }
  }
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
 * @brief Initializes a statically created task.
 * @param [in] task A pointer to the task structure.
 * @param [in] name The task name. Should be at a maximum of __BR_MAX_TASK_NAME_LEN characters.
 * @param [in] run A pointer to the task execution code.
 * @param [in] stackLen The length of the task stack (specified in number of CPU words).
 * @param [in] param A pointer to the parameters to be passed to the task.
 * @param [in] priority The task priority.
 * @return Error code.
 * @retval E_OK If the task is successfully initialized and added to the ready queue.
 * @retval E_INVAL If any of the parameters is invalid and the parameters check featrue is enabled.
 * @retval E_NOMEM If there no space to create the task stack.
 */
BR_Err_t BR_TaskInit(BR_Task_t* task, const char* name, void (*run)(void), uint8_t stackLen, void* param, uint8_t priority)
{
  BR_Err_t ret = E_OK;

  /* Check the parameters. */
  __BR_ASSERT(NULL != task);
  __BR_ASSERT(NULL != name);
  __BR_ASSERT(NULL != run);
  __BR_ASSERT(priority < BR_N_TASK_PRIORITIES);
#if (1U == __BR_CHECK_FUNC_PARAMETERS)
  if ((NULL != task) &&
      (NULL != name) &&
      (NULL != run) &&
      (priority < BR_N_TASK_PRIORITIES))
#endif
  {
    __BR_ENTER_CRITICAL();

    /* Initialize the task object. */
    __BR_ObjectInit(&(task->parent), BR_OBJ_TYPE_TASK, name);
    /* Allocate space for the task stack. */
    task->stackPointer = BR_MemAlloc(stackLen * __BR_WORD_LEN);
    if (NULL != task->stackPointer)
    {
      /* Fill the stack with default values for debugging. */
      memset(task->stackPointer, 0xFFU, stackLen * __BR_WORD_LEN);
      /* Set the stack pointer to the end of allocated memory. */
      task->stackPointer += (stackLen - 1U);
      /* Initialize the new task structure. */
      task->counter = 0U;
      task->state = __BR_TASK_ST_READY;
      /* Set the task priority and initialize the task list. */
      task->priority = priority;
      __BR_ListInit(&(task->list));
      __BR_ListInsertBefore(&(priorityTable[priority]), &(task->list));
      /* Initialize the task reseource wait list. */
      __BR_ListInit(&(task->resWaitList));
      /* Initialize the task stack. */
      task->stackPointer = __BR_PortInitStack(task->stackPointer, run, param);
    }
    else
    {
      ret = E_NOMEM;
    }

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
 * @brief Dynamically create a new task.
 * @param [in] name The task name. Should be at a maximum of __BR_MAX_TASK_NAME_LEN characters.
 * @param [in] run A pointer to the task execution code.
 * @param [in] stackLen The length of the task stack (specified in number of CPU words).
 * @param [in] param A pointer to the parameters to be passed to the task.
 * @param [in] priority The task priority.
 * @return A pointer to the created task or NULL if the task could not be created.
 *
 * This function should be called once for each task before the scheduler is started.
 * It creates a new task and put it on the ready list.
 */
BR_Task_t*  BR_TaskCreate(const char* name, void (*run)(void), uint8_t stackLen, void* param, uint8_t priority)
{
  BR_Task_t* task = NULL;

  /* Check the parameters. */
  __BR_ASSERT(NULL != name);
  __BR_ASSERT(NULL != run);
  __BR_ASSERT(priority < BR_N_TASK_PRIORITIES);
#if (1U == __BR_CHECK_FUNC_PARAMETERS)
  if ((NULL != name) && (NULL != run) && (priority < BR_N_TASK_PRIORITIES))
  {
#endif
    __BR_ENTER_CRITICAL();

    /* Allocate space for the new task on the heap. */
    task = BR_MemAlloc(sizeof(BR_Task_t));
    if (NULL != task)
    {
      /* Initialize the task object. */
      __BR_ObjectInit(&(task->parent), BR_OBJ_TYPE_TASK, name);
      /* Allocate space for the task stack. */
      task->stackPointer = BR_MemAlloc(stackLen * __BR_WORD_LEN);
      if (NULL != task->stackPointer)
      {
        /* Fill the stack with default values for debugging. */
        memset(task->stackPointer, 0xFFU, stackLen * __BR_WORD_LEN);
        /* Set the stack pointer to the end of allocated memory. */
        task->stackPointer += (stackLen - 1U);
        /* Initialize the new task structure. */
        task->counter = 0U;
        task->state = __BR_TASK_ST_READY;
        /* Set the task priority and initialize the task list. */
        task->priority = priority;
        __BR_ListInit(&(task->list));
        __BR_ListInsertBefore(&(priorityTable[priority]), &(task->list));
        /* Initialize the task reseource wait list. */
        __BR_ListInit(&(task->resWaitList));
        /* Initialize the task stack. */
        task->stackPointer = __BR_PortInitStack(task->stackPointer, run, param);
      }
    }

    __BR_EXIT_CRITICAL();
#if (1U == __BR_CHECK_FUNC_PARAMETERS)
  }
#endif

  return task;
}

/**
 * @brief Provides the CPU to another task.
 *
 * This function will make the current running task to leave the CPU and
 * another task will be schedule to run.
 */
void BR_TaskYield(void)
{
  __BR_PortYield();
}

/**
 * @brief Put a task in suspended state.
 * @param [in] task A pointer to the task to be suspended.
 *
 * This function will put a task in suspended state.
 * If task is NULL the current running task will be suspended, otherwise the
 * task pointed by task parameter will be suspended.
 * No matter the current state of the task it will be suspended and on resume it
 * will be put in the ready state. If the task to suspend is the same task that
 * is current running on the CPU it will be suspended and a new task will be
 * immediately be scheduled to run.
 */
void BR_TaskSuspend(BR_Task_t* task)
{
  __BR_ENTER_CRITICAL();

  task->counter = 0U;
  task->state = __BR_TASK_ST_SUSPENDED;
  __BR_ListRemove(&(task->list));
  __BR_ListInsertAfter(&suspendedTaskList, &(task->list));
  if (task == runningTask)
  {
    __BR_PortYield();
  }

  __BR_EXIT_CRITICAL();
}

/**
 * @brief Resume a previously suspended task.
 * @param [in] task A pointer to the task to be suspended.
 *
 * Put the task with ID taskID in ready state if it has been previously suspended.
 * If the task is not within the suspended task list nothing will be done.
 * No matter the state of the task when it was suspended, it will be resumed
 * within the ready state and if the priority of the resumed task is greater
 * than the current running task priority it will be immediately put on the
 * CPU to run.
 */
void BR_TaskResume(BR_Task_t* task)
{
  __BR_ENTER_CRITICAL();

  /* Remove the task from its current list. */
  __BR_ListRemove(&(task->list));
  /* Insert the task within the priority table. */
  __BR_ListInsertAfter(&(priorityTable[task->priority]), &(task->list));
  /* Set the task counter and state. */
  task->counter = 0U;
  task->state = __BR_TASK_ST_READY;
  /* Check the task priority. */
  if (task->priority >= currentPriority)
  {
    currentPriority = task->priority;
    __BR_PortYield();
  }

  __BR_EXIT_CRITICAL();
}

/**
 * @brief Put the running task in waiting state for a number of ticks.
 * @param [in] ticks The number of ticks to wait.
 */
void BR_TaskWait(uint32_t ticks)
{
  /* Enter the critical section. */
  __BR_ENTER_CRITICAL();

  if (ticks > 0U)
  {
    /* Change the running task state to waiting and set its counter. */
    runningTask->state = __BR_TASK_ST_WAITING;
    runningTask->counter = ticks;
    /* Remove the task from priority table and insert it in the waiting list. */
    __BR_ListRemove(&(runningTask->list));
    __BR_ListInsertAfter(&waitingTaskList, &(runningTask->list));
  }

  __BR_PortYield();

  /* Exit the critical section. */
  __BR_EXIT_CRITICAL();
}

/** @} */

/** @} */
