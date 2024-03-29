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

/******************************************************************************/
/* G E N E R A L  D E F I N I T IO N S                                        */
/******************************************************************************/

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

/* Define the position of a bit within a variable. */
#define BIT_POS_0   (0U)
#define BIT_POS_1   (1U)
#define BIT_POS_2   (2U)
#define BIT_POS_3   (3U)
#define BIT_POS_4   (4U)
#define BIT_POS_5   (5U)
#define BIT_POS_6   (6U)
#define BIT_POS_7   (7U)
#define BIT_POS_8   (8U)
#define BIT_POS_9   (9U)
#define BIT_POS_10  (10U)
#define BIT_POS_11  (11U)
#define BIT_POS_12  (12U)
#define BIT_POS_13  (13U)
#define BIT_POS_14  (14U)
#define BIT_POS_15  (15U)
#define BIT_POS_16  (16U)
#define BIT_POS_17  (17U)
#define BIT_POS_18  (18U)
#define BIT_POS_19  (19U)
#define BIT_POS_20  (20U)
#define BIT_POS_21  (21U)
#define BIT_POS_22  (22U)
#define BIT_POS_23  (23U)
#define BIT_POS_24  (24U)
#define BIT_POS_25  (25U)
#define BIT_POS_26  (26U)
#define BIT_POS_27  (27U)
#define BIT_POS_28  (28U)
#define BIT_POS_29  (29U)
#define BIT_POS_30  (30U)
#define BIT_POS_31  (31U)

/* Helper macros to work with bit fields. */
#define BITS_GET(VAR, MASK) ((VAR) & (MASK))
#define BITS_SET(VAR, MASK) ((VAR) |= (MASK))
#define BITS_CLEAR(VAR, MASK) ((VAR) &= (~(MASK)))
#define BITS_TOGGLE(VAR, MASK) ((VAR) ^= (MASK))

#define U32_LOW(VAR)  ((uint16_t)((VAR) & 0x0000FFFFU))
#define U32_HIGH(VAR) ((uint16_t)(((VAR) & 0xFFFF0000U)) >> 16U)

#define U8_NIBBLE_0(VAR) ((VAR) & 0x0FU)
#define U8_NIBBLE_1(VAR) (((VAR) & 0xF0U) >> 4U)

#define U16_MSB(VAR)  ((uint8_t)(((VAR) & (0xFF00)) >> (8U)))
#define U16_LSB(VAR)  ((uint8_t)((VAR) & (0x00FF)))

#define U32_BYTE_0(VAR) ((uint8_t)((((uint32_t)VAR) & ((uint32_t)0x000000FFU)) >> 0U))
#define U32_BYTE_1(VAR) ((uint8_t)((((uint32_t)VAR) & ((uint32_t)0x0000FF00U)) >> 8U))
#define U32_BYTE_2(VAR) ((uint8_t)((((uint32_t)VAR) & ((uint32_t)0x00FF0000U)) >> 16U))
#define U32_BYTE_3(VAR) ((uint8_t)((((uint32_t)VAR) & ((uint32_t)0xFF000000U)) >> 24U))

typedef uint32_t BR_Size_t;

/**
 * Error code list.
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
  E_NOMEM,
} BR_Err_t;

#ifndef TRUE
#define TRUE (1U)
#endif

#ifndef FALSE
#define FALSE (0U)
#endif

typedef uint8_t BR_Boolean_t;

#define __BR_LIST_ENTRY(NODE, TYPE, FIELD) ((TYPE*)(((uint8_t*)NODE) - ((uint8_t*)&(((TYPE*)0U)->FIELD))))

typedef struct BR_ListNode
{
  struct BR_ListNode* prev;
  struct BR_ListNode* next;
} BR_ListNode_t;

/******************************************************************************/
/* O B J E C T  D E F I N I T I O N S                                         */
/******************************************************************************/

/**
 * Enumerates the kernel object types.
 */
typedef enum
{
  BR_OBJ_TYPE_TASK = 0U,  /**< Task object. */
  BR_OBJ_TYPE_TIMER,      /**< Timer object. */
  BR_OBJ_TYPE_DEVICE,     /**< Device object. */
  BR_OBJ_TYPE_MUTEX,      /**< Mutex object. */
  BR_N_OBJ_TYPES,         /**< Number of object types. */
} BR_ObjectType_t;

/**
 * The object structure definition.
 */
typedef struct
{
    BR_ObjectType_t type; /**< The object type. */
    BR_ListNode_t   node; /**< The node within the objects list. */
    char            name[__BR_MAX_OBJ_NAME_LEN];  /**< The object name. */
} BR_Object_t;

/******************************************************************************/
/* T A S K  C O N T R O L  D E F I N I T I O N S                              */
/******************************************************************************/

/**
 * @addtogroup TaskCtrl
 * @{
 */

/* The available task priorities. */
#define BR_TASK_PRIORITY_TRIVIAL    (0U)
#define BR_TASK_PRIORITY_LOW        (1U)
#define BR_TASK_PRIORITY_MEDIUM     (2U)
#define BR_TASK_PRIORITY_HIGH       (3U)
#define BR_TASK_PRIORITY_CRITICAL   (4U)
/* The number of task priorities. */
#define BR_N_TASK_PRIORITIES        (5U)

/* The task states. */
#define __BR_TASK_ST_READY      (0U)
#define __BR_TASK_ST_RUNNING    (1U)
#define __BR_TASK_ST_WAITING    (2U)
#define __BR_TASK_ST_SUSPENDED  (3U)

/* The task error codes. */
#define __BR_TASK_ERR_OK        (0U)
#define __BR_TASK_ERR_ERROR     (1U)
#define __BR_TASK_ERR_TIMEOUT   (2U)
#define __BR_TASK_ERR_UNKNOW    (3U)

/**
 * A new type to represent the stack pointer.
 */
typedef uint32_t* BR_StackPointer_t;

typedef struct
{
    volatile BR_StackPointer_t  stackPointer;  /**< This MUST be the first structure field. */
    BR_Object_t                 parent;
    uint32_t                    counter;
    uint8_t                     state;
    uint8_t                     priority;
    BR_ListNode_t               list;
    BR_ListNode_t               resWaitList;
    uint8_t                     errorCode;
} BR_Task_t;

/** @} */

/******************************************************************************/
/* T I M E R  C O N T R O L  D E F I N I T I O N S                            */
/******************************************************************************/

/**
 * @addtogroup TimerCtrl
 * @{
 */

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
    BR_Object_t         parent;
    BR_ListNode_t       node;
    uint32_t            counter;
    uint32_t            reload;
    BR_TimerCallback_t  callbak;
    void*               param;
    uint8_t             flags;
} BR_Timer_t;

/** @} */

/******************************************************************************/
/* D E V I C E  C O N T R O L  D E F I N I T I O N S                          */
/******************************************************************************/

/**
 * @addtogroup DeviceCtrl
 * @{
 */

typedef enum
{
  BR_DEVICE_TYPE_CHAR = 0U,
  BR_DEVICE_TYPE_SPI_BUS,
  BR_DEVICE_TYPE_SPI_DEV,
  BR_DEVICE_TYPE_ADC,
  BR_DEVICE_TYPE_RTC,
  BR_DEVICE_TYPE_UNKNOWN,
  BR_N_DEVICE_TYPES,
} BR_DeviceType_t;

typedef struct BR_Device BR_Device_t;

/**
 * The device structure.
 */
struct BR_Device
{
    BR_Object_t     parent;
    BR_ListNode_t   node;
    uint8_t         flags;
    BR_DeviceType_t type;
    BR_Err_t        (*init)   (BR_Device_t* device);
    BR_Err_t        (*open)   (BR_Device_t* device, uint8_t flags);
    BR_Err_t        (*close)  (BR_Device_t* device);
    uint32_t        (*read)   (BR_Device_t* device, uint32_t address, uint8_t* buffer, uint32_t nBytes);
    uint32_t        (*write)  (BR_Device_t* device, uint32_t address, uint8_t* buffer, uint32_t nBytes);
    BR_Err_t        (*control)(BR_Device_t* device, uint8_t cmd, void* param);
    void*           custom;
};

/** @} */

/******************************************************************************/
/* I N T E R  P R O C E S S  C O M M U N I C A T I O N  D E F I N I T I O N S */
/******************************************************************************/

/**
 * @addtogroup IPC
 * @{
 */

#define BR_IPC_WAIT_FOREVER ((uint32_t)0xFFFFFFFFU)

/**
 * The mutex structure.
 */
typedef struct
{
    BR_Object_t   parent;   /**< The mutex parent object. */
    BR_Task_t*    owner;    /**< The mutex owner. */
    uint8_t       counter;  /**< The counter of acquires by the mutex owner. */
    BR_ListNode_t waitList; /**< The task wait list for the mutex. */
} BR_Mutex_t;

/** @} */

#endif /* _BR_RTOS_DEFS_H_ */
