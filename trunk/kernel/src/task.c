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

#include "task.h"
#include "port.h"
#include "BR-RTOSConfig.h"
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

#define __BR_TASK_ST_READY__    (0U)
#define __BR_TASK_ST_RUNNING__  (1U)
#define __BR_TASK_ST_WAITING__  (2U)

typedef struct BR_Task_t BR_Task_t;
struct BR_Task_t
{
    volatile BR_StackPointer_t stackPointer;
    uint32_t counter;
    uint8_t state;
    BR_Task_t* nextTask;
    char name[__BR_MAX_TASK_NAME_LEN];
};

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

static uint8_t nTasks = 0U;

/**@}*/

/******************************************************************************/
/* P U B L I C  V A R I A B L E S                                             */
/******************************************************************************/

/**
 * @defgroup PublicVar Public Variables
 * @{
 */

/**
 * Points to the only task within the running state.
 */
BR_Task_t* runningTask = NULL;

/**
 * Points to the first task within the ready state list.
 */
BR_Task_t* readyTaskList = NULL;

/**
 * Points to the first task within the waiting state list.
 */
BR_Task_t* waitingTaskList = NULL;

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

void BR_KernelStartScheduler(void)
{
  BR_PortSchedulerStart();
}

BR_Err_t BR_TaskCreate(const char* name, void (*run)(void), uint8_t stackLen, void* param, uint8_t* taskID)
{
  BR_Err_t ret = E_INVAL;
  BR_Task_t* task = NULL;

  /* Check the parameters. */
  __BR_ASSERT(NULL != run);
  if (NULL != run)
  {
    /* Allocate space for the new task on the heap. */
    task = BR_MemAlloc(sizeof(BR_Task_t));
    if (NULL != task)
    {
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
        nTasks++;
        if (NULL != taskID)
        {
          *taskID = nTasks;
        }
        /* Initialize the new task structure. */
        task->counter = 0U;
        task->state = __BR_TASK_ST_READY__;
        strncpy(task->name, name, __BR_MAX_TASK_NAME_LEN);
        task->name[__BR_MAX_TASK_NAME_LEN - 1U] = '\0';
        /* Insert the new task within the ready task list. */
        task->nextTask = readyTaskList;
        readyTaskList = task;
        runningTask = readyTaskList;
        /* Initialize the task stack. */
        task->stackPointer = BR_PortInitStack(task->stackPointer, run, param);
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
    ret = E_INVAL;
  }

  return ret;
}

/**
 * @brief TODO
 */
void BR_TaskSwitch(void)
{
  if (runningTask->nextTask != NULL)
  {
    runningTask = runningTask->nextTask;
  }
  else
  {
    runningTask = readyTaskList;
  }
}

void BR_TaskTickUpdate(void)
{
  BR_Task_t* prevTask = NULL;
  BR_Task_t* currTask = waitingTaskList;

  /* Walk through the waiting list and update each task counter. */
  while (currTask != NULL)
  {
    /* Decrement the task tick counter. */
    currTask->counter--;
    if (0U == currTask->counter)
    {
      /* The task counter has been expired, so remove the task from waiting
       * list and insert it on the ready list.
       */
      currTask->state = __BR_TASK_ST_READY__;
      if (NULL == prevTask)
      {
        waitingTaskList = currTask->nextTask;
        currTask->nextTask = readyTaskList;
        readyTaskList = currTask;
        currTask = waitingTaskList;
      }
      else
      {
        prevTask->nextTask = currTask->nextTask;
        currTask->nextTask = readyTaskList;
        readyTaskList = currTask;
        currTask = prevTask->nextTask;
      }
    }
    else
    {
      /* Next task. */
      prevTask = currTask;
      currTask = currTask->nextTask;
    }
  }
}

/**@}*/
