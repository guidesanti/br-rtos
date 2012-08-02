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
 * @addtogroup TaskCtrl Task Control
 * @{
 */

/******************************************************************************/
/* I N C L U D E S                                                            */
/******************************************************************************/
#include "BR-RTOSDefs.h"
#include "port.h"
#include "list.h"
#include <stdint.h>


/******************************************************************************/
/* C O N S T A N T ,  M A C R O  A N D  T Y P E  D E F I N I T I O N S        */
/******************************************************************************/

/**
 * @name Constants, Macros and Type definitions
 * @{
 */

/** @} */

/******************************************************************************/
/* K E R N E L  I N T E R N A L  F U N C T I O N S                            */
/******************************************************************************/
void __BR_IdleTask(void);
void __BR_TasklInit(void);
void __BR_TaskStartScheduler(void);
void __BR_TaskSwitch(void);
void __BR_TaskTickUpdate(void);

/** @} */

#endif /* _TASK_H_ */
