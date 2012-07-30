/**
 * @file        BR-RTOSDefs.h
 * @date        2012, July 27
 * @author      Guilherme A. de Santi <guidesanti@yahoo.com.br>
 * @copyright   Guilherme A. de Santi &copy; 2012
 * @brief       TODO: add brief description
 *
 * TODO: add detailed description
 */


#ifndef _BR_RTOS_DEFS_H_
#define _BR_RTOS_DEFS_H_

/******************************************************************************/
/* I N C L U D E S                                                            */
/******************************************************************************/
#include "BR-RTOSConfig.h"
#include <stdint.h>
#include <stdlib.h>

/* Define the mask of a bit within a variable. */
#define BIT_MASK_0  (0x00000001U)
#define BIT_MASK_1  (0x00000002U)
#define BIT_MASK_2  (0x00000004U)
#define BIT_MASK_3  (0x00000008U)
#define BIT_MASK_4  (0x00000010U)
#define BIT_MASK_5  (0x00000020U)
#define BIT_MASK_6  (0x00000040U)
#define BIT_MASK_7  (0x00000080U)
#define BIT_MASK_8  (0x00000100U)
#define BIT_MASK_9  (0x00000200U)
#define BIT_MASK_10 (0x00000400U)
#define BIT_MASK_11 (0x00000800U)
#define BIT_MASK_12 (0x00001000U)
#define BIT_MASK_13 (0x00002000U)
#define BIT_MASK_14 (0x00004000U)
#define BIT_MASK_15 (0x00008000U)
#define BIT_MASK_16 (0x00010000U)
#define BIT_MASK_17 (0x00020000U)
#define BIT_MASK_18 (0x00040000U)
#define BIT_MASK_19 (0x00080000U)
#define BIT_MASK_20 (0x00100000U)
#define BIT_MASK_21 (0x00200000U)
#define BIT_MASK_22 (0x00400000U)
#define BIT_MASK_23 (0x00800000U)
#define BIT_MASK_24 (0x01000000U)
#define BIT_MASK_25 (0x02000000U)
#define BIT_MASK_26 (0x04000000U)
#define BIT_MASK_27 (0x08000000U)
#define BIT_MASK_28 (0x10000000U)
#define BIT_MASK_29 (0x20000000U)
#define BIT_MASK_30 (0x40000000U)
#define BIT_MASK_31 (0x80000000U)

/* Helper macros to work with bit fields. */
#define BITS_GET(VAR, MASK) ((VAR) & (MASK))
#define BITS_SET(VAR, MASK) ((VAR) |= (MASK))
#define BITS_CLEAR(VAR, MASK) ((VAR) &= (~(MASK)))
#define BITS_TOGGLE(VAR, MASK) ((VAR) ^= (MASK))

/* The available task priorities. */
#define BR_TASK_PRIORITY_TRIVIAL    (0U)
#define BR_TASK_PRIORITY_LOW        (1U)
#define BR_TASK_PRIORITY_MEDIUM     (2U)
#define BR_TASK_PRIORITY_HIGH       (3U)
#define BR_TASK_PRIORITY_CRITICAL   (4U)
/* The number of task priorities. */
#define BR_N_TASK_PRIORITIES        (5U)

typedef uint32_t BR_Size_t;

/**
 * @brief Error code list.
 *
 * This is the list of error codes that may be returned by the BR-RTOS API
 * functions.
 */
typedef enum
{
  E_OK,
  E_ERROR,
  E_INVAL,
  E_NORES,
  E_ENOSYS,
  E_TIMEOUT,
} BR_Err_t;

#ifndef TRUE
#define TRUE (1U)
#endif

#ifndef FALSE
#define FALSE (0U)
#endif

typedef uint8_t BR_Boolean_t;

#define __BR_LIST_DECLARE(NAME) BR_ListNode_t NAME = { &NAME, &NAME }
#define __BR_LIST_ENTRY(NODE, TYPE, FIELD) ((TYPE*)(((uint8_t*)NODE) - ((uint8_t*)&(((TYPE*)0U)->FIELD))))

typedef struct BR_ListNode BR_ListNode_t;
struct BR_ListNode
{
    BR_ListNode_t* prev;
    BR_ListNode_t* next;
};

typedef enum
{
  BR_OBJ_TYPE_TASK = 0U,
  BR_OBJ_TYPE_DEVICE,
  BR_OBJ_TYPE_TIMER,
  BR_N_OBJ_TYPES,
} BR_ObjectType_t;

typedef struct
{
    uint16_t        id;
    BR_ObjectType_t type;
    void*           child;
    BR_ListNode_t   node;
    char            name[__BR_MAX_OBJ_NAME_LEN];
} BR_Object_t;

#define __BR_TASK_ST_READY      (0U)
#define __BR_TASK_ST_RUNNING    (1U)
#define __BR_TASK_ST_WAITING    (2U)
#define __BR_TASK_ST_SUSPENDED  (3U)

/**
 * A new type to represent the stack pointer.
 */
typedef uint32_t* BR_StackPointer_t;

typedef struct
{
    volatile BR_StackPointer_t stackPointer;  /**< This MUST be the first structure field. */
    BR_Object_t* parent;
    uint32_t counter;
    uint8_t state;
    uint8_t priority;
    BR_ListNode_t list;
    char name[__BR_MAX_TASK_NAME_LEN + 1U];
} BR_Task_t;

/* Timer flags. */
#define BR_TIMER_FLAG_SINGLE  (0x00U) /**< Single shot timer. */
#define BR_TIMER_FLAG_CONT    (0x01U) /**< Continuous timer. */
#define BR_TIMER_FLAG_MS      (0x00U) /**< Milliseconds based timer. */
#define BR_TIMER_FLAG_SEC     (0x02U) /**< Seconds based timer. */
#define BR_TIMER_FLAG_SOFT    (0x00U) /**< Soft timer. */
#define BR_TIMER_FLAG_HARD    (0x04U) /**< Hard timer. */

/**
 * List of timer control commands.
 */
typedef enum
{
  BR_TIMER_CMD_SET_TIME = 0U, /**< Set the timer time. */
  BR_TIMER_CMD_GET_TIME,      /**< Get the timer time. */
  BR_TIMER_CMD_SET_SINGLE,    /**< Set the timer to single shot. */
  BR_TIMER_CMD_SET_CONT,      /**< Set the timer to continuous. */
  BR_TIMER_CMD_SET_MS,        /**< Set the timer base to milliseconds. */
  BR_TIMER_CMD_SET_SEC,       /**< Set the timer base to seconds. */
  BR_TIMER_CMD_SET_SOFT,      /**< Set the timer to sfot. */
  BR_TIMER_CMD_SET_HARD,      /**< Set the timer to hard. */
  BR_N_TIMER_CMDS,            /**< The total number of timer control commands. */
} BR_TimerCmd_t;

/**
 * The timer callback typedef.
 */
typedef void (*BR_TimerCallback_t)(void* param);

/**
 * The timer structure.
 */
typedef struct
{
    BR_Object_t*        parent;
    BR_ListNode_t       node;
    uint32_t            counter;
    uint32_t            reload;
    BR_TimerCallback_t  callbak;
    void*               param;
    uint8_t             flags;
} BR_Timer_t;

/**
 * @addtogroup IPC
 * @{
 */

/**
 * The mutex structure.
 */
typedef struct
{
    BR_Object_t*  parent;
    BR_Task_t*    owner;
    uint8_t       state;
} BR_Mutex_t;

/** @} */


#endif /* _BR_RTOS_DEFS_H_ */
