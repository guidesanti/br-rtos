/**
 * @file        device.h
 * @date        2012, August 02
 * @author      Guilherme A. de Santi <guidesanti@yahoo.com.br>
 * @copyright   &copy; 2012, BR-RTOS Development Team
 * @brief       TODO: add brief description
 *
 * This file is part of the BR-RTOS.
 * The license distribution terms may found at COPYING or LICENSE file
 * within the source directory tree.
 */


#ifndef _DEVICE_H_
#define _DEVICE_H_

/**
 * @addtogroup DevCtrl Device Control
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

/** @} */


/******************************************************************************/
/* K E R N E L  I N T E R N A L  F U N C T I O N S                            */
/******************************************************************************/
void __BR_DeviceInit(void);
void __BR_DeviceInitAll(void);


/** @} */

#endif /* _DEVICE_H_ */
