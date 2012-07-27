/**
 * @file        task.c
 * @date        2012, July 20
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

#include "BR-RTOS.h"
#include "object.h"
#include "timer.h"
#include "task.h"
#include <stdlib.h>
#include <string.h>

/**@}*/

/******************************************************************************/
/* C O N S T A N T ,  M A C R O  A N D  T Y P E  D E F I N I T I O N S       */
/******************************************************************************/

/**
 * @defgroup Defs Constant, Macro and Type Definitions
 * @{
 */

/**@}*/

/******************************************************************************/
/* P R I V A T E  F U N C T I O N S  P R O T O T Y P E S                      */
/******************************************************************************/

/**
 * @defgroup PrivateFuncProto Private Function Prototypes
 * @{
 */
static void __BR_IdleTask(void);

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

/**
 * Points to the task within the running state.
 */
BR_Task_t* runningTask = NULL;

/**
 * The list of tasks within suspended state.
 */
BR_ListNode_t suspendedTaskList;

/**
 * The list of tasks within waiting state.
 */
BR_ListNode_t waitingTaskList;

/**
 * The task priority table.
 *
 * All tasks within the priority table are in the ready state.
 */
BR_ListNode_t priorityTable[BR_N_TASK_PRIORITIES];

/**
 * The current priority level.
 */
uint8_t currentPriority = (BR_N_TASK_PRIORITIES - 1U);

/**@}*/

/******************************************************************************/
/* P R I V A T E  F U N C T I O N S                                           */
/******************************************************************************/

/**
 * @defgroup PrivateFunc Private Functions
 * @{
 */

static void __BR_IdleTask(void)
{
  while (TRUE) { }
}

/**@}*/

/******************************************************************************/
/* P U B L I C  F U N C T I O N S                                             */
/******************************************************************************/

/**
 * @defgroup PublicFunc Public Functions
 * @{
 */

/**
 * @brief Initialize the kernel.
 */
void BR_KernelInit(void)
{
  uint8_t index = 0U;

  __BR_ObjectInit();

  __BR_ListInit(&suspendedTaskList);
  __BR_ListInit(&waitingTaskList);
  /* Initialize the priority table. */
  for (index = 0U; index < BR_N_TASK_PRIORITIES; index++)
  {
    __BR_ListInit(&priorityTable[index]);
  }

  /* Initialize the timers sub-system. */
  __BR_TimerInit();

  /* Create the idle task. */
  BR_TaskCreate("Idle", __BR_IdleTask, 32U, NULL, BR_TASK_PRIORITY_TRIVIAL, NULL);
}

/**
 * @brief Start the task scheduler.
 *
 * This function should be called from main() and will not return unless the
 * scheduler is not able to be started.
 * This function will start the task switching and must be called after
 * all tasks has been successfully created.
 */
void BR_KernelStartScheduler(void)
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
 * @brief Create a new task.
 * @param [in] name The task name. Should be at a maximum of __BR_MAX_TASK_NAME_LEN characters.
 * @param [in] run A pointer to the task execution code.
 * @param [in] stackLen The length of the task stack (specified in number of CPU words).
 * @param [in] param A pointer to the parameters to be passed to the task.
 * @param [out] task The task structure.
 * @return Error code.
 * @retval E_INVAL If any parameter is invalid.
 * @retval E_NORES If there is no enough memory within the heap to allocate for the new task.
 * @retval E_OK If the task has been created successfully.
 *
 * This function should be called once for each task before the scheduler is started.
 * It creates a new task and put it on the ready list.
 *
 * E_INVAL
 * If the parameters check are enabled (__BR_CHECK_FUNC_PARAMETERS == 1U) and any of
 * the function parameters is invalid the operation will be aborted and the function
 * will return E_INVAL.
 *
 * E_OK
 * If the task is created successfully the task ID is returned within the taskID parameter
 * if it is different from NULL and the function will return with error code E_OK.
 *
 * E_NORES
 * If there is no memory space available to create the task, the function will
 * abort the operation and return E_NORES.
 */
BR_Err_t BR_TaskCreate(const char* name, void (*run)(void), uint8_t stackLen, void* param, uint8_t priority, BR_Task_t** taskArg)
{
  BR_Err_t ret = E_OK;
  BR_Task_t* task = NULL;
  BR_Object_t* obj = NULL;

  /* Check the parameters. */
  __BR_ASSERT(NULL != run);
  __BR_ASSERT(priority < BR_N_TASK_PRIORITIES);
#if (1U == __BR_CHECK_FUNC_PARAMETERS)
  if ((NULL != run) && (priority < BR_N_TASK_PRIORITIES))
  {
#endif
    __BR_ENTER_CRITICAL();
    /* Allocate space for the new task on the heap. */
    task = BR_MemAlloc(sizeof(BR_Task_t));
    if (NULL != task)
    {
      /* Create the task parent object. */
      obj = __BR_ObjectCreate((char*)name, BR_OBJ_TYPE_TASK, task);
      if (NULL != obj)
      {
        task->parent = obj;
        /* Allocate space for the task stack. */
        task->stackPointer = BR_MemAlloc(stackLen * __BR_WORD_LEN);
        if (NULL != task->stackPointer)
        {
          /* Fill the stack with default values for debugging. */
          memset(task->stackPointer, 0xFFU, stackLen * __BR_WORD_LEN);
          /* Set the stack pointer to the end of allocated memory. */
          task->stackPointer += (stackLen - 1U);
          /* Increment the created number of tasks.
           * The number of the created task will be the task ID.
           */
          if (NULL != taskArg)
          {
            *(taskArg) = task;
          }
          /* Initialize the new task structure. */
          task->counter = 0U;
          task->state = __BR_TASK_ST_READY;
          strncpy(task->name, name, __BR_MAX_TASK_NAME_LEN);
          task->name[__BR_MAX_TASK_NAME_LEN - 1U] = '\0';
          /* Set the task priority and initialize the task list. */
          task->priority = priority;
          __BR_ListInit(&(task->list));
          __BR_ListInsertBefore(&(priorityTable[priority]), &(task->list));
          /* Initialize the task stack. */
          task->stackPointer = __BR_PortInitStack(task->stackPointer, run, param);
          ret = E_OK;
        }
        else
        {
          ret = E_NORES;
        }
      }
      else
      {
        ret = E_NORES;
      }
    }
    else
    {
      ret = E_NORES;
    }
    __BR_EXIT_CRITICAL();
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
 * @param [in] taskID The ID of the task to be suspended.
 *
 * This function will put a task with ID taskID in suspended state.
 * If the ID is not valid nothing will be done.
 * If the ID is valid the task will be put in suspended state until a call
 * to BR_TaskResume() is called for the same task ID. No matter the current
 * state of the task it will be suspended and on resume it will be put in the ready state.
 * If the task identified by taskID is the same task that is current running
 * within the CPU it will be suspended and a new task will be immediately be
 * scheduled to run.
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
 * @param [in] taskID The ID of the task to be resumed.
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
  BR_Task_t* taskAux = NULL;
  BR_ListNode_t* node = NULL;

  __BR_ENTER_CRITICAL();

  /* Look at the suspended list to find the task. */
  node = suspendedTaskList.next;
  while (node != &suspendedTaskList)
  {
    taskAux = __BR_LIST_ENTRY(node, BR_Task_t, list);
    node = node->next;
    if (task == taskAux)
    {
      /* Remove the task from suspended list. */
      __BR_ListRemove(&(task->list));
      /* Insert the task within the priority table. */
      __BR_ListInsertAfter(&(priorityTable[task->priority]), &(task->list));
      /* Set the task state. */
      task->counter = 0U;
      task->state = __BR_TASK_ST_READY;
      /* Check the task priority. */
      if (task->priority > currentPriority)
      {
        currentPriority = task->priority;
        __BR_PortYield();
      }
      break;
    }
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

/**@}*/
