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
#include <stdint.h>
#include <stdlib.h>

/******************************************************************************/
/* SCHEDULER CONTROL API                                                      */
/******************************************************************************/
void BR_KernelInit(void);
void BR_KernelStartScheduler(void);

/******************************************************************************/
/* TASK CONTROL API                                                           */
/******************************************************************************/
BR_Err_t BR_TaskCreate(const char* name, void (*run)(void), uint8_t stackLen, void* param, uint8_t priority, BR_Task_t** taskArg);
void BR_TaskYield(void);
void BR_TaskSuspend(BR_Task_t* task);
void BR_TaskResume(BR_Task_t* task);
void BR_TaskWait(uint32_t ticks);

/******************************************************************************/
/* TIMER CONTROL API                                                          */
/******************************************************************************/
BR_Err_t BR_TimerCreate(const char* name, uint32_t time, BR_TimerCallback_t callback, void* param, uint8_t flags, BR_Timer_t** timerArg);
BR_Err_t BR_TimerControl(BR_Timer_t* timer, BR_TimerCmd_t cmd, void* param);
BR_Err_t BR_TimerStart(BR_Timer_t* timer);
BR_Err_t BR_TimerStop(BR_Timer_t* timer);
BR_Err_t BR_TimerRestart(BR_Timer_t* timer);

/******************************************************************************/
/* DEVICE CONTROL API                                                         */
/******************************************************************************/
BR_Err_t BR_DeviceRegister(const char* name, BR_Device_t* device);
BR_Device_t* BR_DeviceFind(const char* name);
BR_Err_t BR_DeviceInit(BR_Device_t* device);
BR_Err_t BR_DeviceOpen(BR_Device_t* device, uint8_t flags);
BR_Err_t BR_DeviceClose(BR_Device_t* device);
uint32_t BR_DeviceRead(BR_Device_t* device, uint32_t address, uint8_t* buffer, uint32_t nBytes);
uint32_t BR_DeviceWrite(BR_Device_t* device, uint32_t address, uint8_t* buffer, uint32_t nBytes);
BR_Err_t BR_DeviceControl(BR_Device_t* device, uint8_t cmd, void* param);

/******************************************************************************/
/* INTER PROCESS COMMUNICATION API                                            */
/******************************************************************************/
BR_Mutex_t* BR_IpcMutexCreate(const char* name);
BR_Err_t BR_IpcMutexAcquire(BR_Mutex_t* mutex, uint32_t ticks);
BR_Err_t BR_IpcMutexRelease(BR_Mutex_t* mutex);
BR_Err_t BR_IpcMutexControl(BR_Mutex_t* mutex, uint8_t cmd, void* param);

/******************************************************************************/
/* MEMORY ALLOCATION API                                                      */
/******************************************************************************/
void* BR_MemAlloc(BR_Size_t nBytes);

#endif /* _BR_RTOS_H_ */
