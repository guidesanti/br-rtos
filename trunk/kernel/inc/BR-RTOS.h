/**
 * @file        BR-RTOS.h
 * @date        2012, July 19
 * @author      Guilherme A. de Santi <guidesanti@yahoo.com.br>
 * @copyright   Guilherme A. de Santi &copy; 2012
 * @brief       BR-RTOS API declaration header file.
 *
 * This file declares all the BR-RTOS functions that are exported for the user
 * code as the public API.
 * The user application should only include this file to get access to all the
 * public functions provided by the BR-RTOS.
 * Any BR-RTOS function that is not declared within this file must not be called
 * by the user code, otherwise unpredictable events may occur or the system
 * may crash or freeze.
 */


#ifndef _BR_RTOS_H_
#define _BR_RTOS_H_


/******************************************************************************/
/* I N C L U D E S                                                            */
/******************************************************************************/
#include "BR-RTOSDefs.h"
#include "list.h"
#include <stdint.h>
#include <stdlib.h>


/******************************************************************************/
/* C O N S T A N T ,  M A C R O  A N D  T Y P E  D E F I N I T I O N S       */
/******************************************************************************/


/******************************************************************************/
/* P U B L I C  V A R I A B L E S                                             */
/******************************************************************************/


/******************************************************************************/
/* P U B L I C  F U N C T I O N S                                             */
/******************************************************************************/

/**
 * @addtogroup KernelCtrl Kernel Control
 * @{
 */

void BR_KernelInit(void);
void BR_KernelStartScheduler(void);

/**@}*/

/**
 * @addtogroup PublicAPIFunc Public API Functions
 * @{
 */

BR_Err_t BR_TaskCreate(const char* name, void (*run)(void), uint8_t stackLen, void* param, uint8_t priority, BR_Task_t** taskArg);
void BR_TaskYield(void);
void BR_TaskSuspend(BR_Task_t* task);
void BR_TaskResume(BR_Task_t* task);
void BR_TaskWait(uint32_t ticks);

/**@}*/

/**
 * @addtogroup TimerCtrl Timer Control
 * @{
 */

BR_Err_t BR_TimerCreate(const char* name, uint32_t time, BR_TimerCallback_t callback, void* param, uint8_t flags, BR_Timer_t** timerArg);
BR_Err_t BR_TimerControl(BR_Timer_t* timer, BR_TimerCmd_t cmd, void* param);
BR_Err_t BR_TimerStart(BR_Timer_t* timer);
BR_Err_t BR_TimerStop(BR_Timer_t* timer);
BR_Err_t BR_TimerRestart(BR_Timer_t* timer);

/**@}*/

/**
 * @addtogroup MemCtrl Memory Control
 * @{
 */

void* BR_MemAlloc(BR_Size_t nBytes);

/**@}*/


#endif /* _BR_RTOS_H_ */
