/**
 * @file        task.h
 * @date        2012, July 20
 * @author      Guilherme A. de Santi <guidesanti@yahoo.com.br>
 * @copyright   Guilherme A. de Santi &copy; 2012
 * @brief       TODO: add brief description
 *
 * TODO: add detailed description
 */


#ifndef _TASK_H_
#define _TASK_H_

/**
 * @defgroup ModuleName The Module Name
 * @{
 */

/******************************************************************************/
/* I N C L U D E S                                                            */
/******************************************************************************/

/**
 * @defgroup Includes Header File Include
 * @{
 */

#include "BR-RTOS.h"
#include "BR-RTOSConfig.h"
#include "port.h"
#include "list.h"
#include <stdint.h>

/**@}*/

/******************************************************************************/
/* C O N S T A N T ,  M A C R O  A N D  T Y P E  D E F I N I T I O N S       */
/******************************************************************************/

/**
 * @defgroup Defs Constant, Macro and Type Definitions
 * @{
 */

#define __BR_TASK_ST_READY      (0U)
#define __BR_TASK_ST_RUNNING    (1U)
#define __BR_TASK_ST_WAITING    (2U)
#define __BR_TASK_ST_SUSPENDED  (3U)

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

/**@}*/

/******************************************************************************/
/* P U B L I C  V A R I A B L E S                                             */
/******************************************************************************/

/**
 * @defgroup PublicVar Public Variables
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

void __BR_TaskSwitch(void);
void __BR_TaskTickUpdate(void);

/**@}*/

/**@}*/


#endif /* _TASK_H_ */
