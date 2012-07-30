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


#ifndef _IPC_H_
#define _IPC_H_

/**
 * @addtogroup IPC Inter Process Communication
 * @{
 */

/******************************************************************************/
/* I N C L U D E S                                                            */
/******************************************************************************/

/******************************************************************************/
/* C O N S T A N T ,  M A C R O  A N D  T Y P E  D E F I N I T I O N S        */
/******************************************************************************/

/**
 * @name Constants, Macros and Type definitions
 * @{
 */

#define __BR_MUTEX_RELEASED (0U)  /**< The mutex is released and may be acquired by some task. */
#define __BR_MUTEX_ACQUIRED (1U)  /**< The mutex is already acquired by some task. */

/** @} */

/******************************************************************************/
/* K E R N E L  I N T E R N A L  F U N C T I O N S                            */
/******************************************************************************/
void __BR_IpcInit(void);

/** @} */

#endif /* _IPC_H_ */