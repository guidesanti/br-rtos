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

#include <stdint.h>
#include <stdlib.h>

/**@}*/

/******************************************************************************/
/* C O N S T A N T ,  M A C R O  A N D  T Y P E  D E F I N I T I O N S       */
/******************************************************************************/

/**
 * @defgroup Defs Constant, Macro and Type Definitions
 * @{
 */

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
} BR_Err_t;

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

/**
 * @defgroup KernelCtrl Kernel Control API
 * @{
 */

void BR_KernelStartScheduler(void);

/**@}*/

/**
 * @defgroup TaskCtrl Task Control API
 * @{
 */

BR_Err_t BR_TaskCreate(const char* name, void (*run)(void), uint8_t stackLen, void* param, uint8_t* taskID);

/**@}*/

/**
 * @defgroup MemCtrl Memory Control API
 * @{
 */

void* BR_MemAlloc(BR_Size_t nBytes);

/**@}*/

/**@}*/

/**@}*/


#endif /* _BR_RTOS_H_ */
