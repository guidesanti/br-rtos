/**
 * @file        BR-RTOSConfig.h
 * @date        2012, July 20
 * @author      Guilherme A. de Santi <guidesanti@yahoo.com.br>
 * @copyright   Guilherme A. de Santi &copy; 2012
 * @brief       BR-RTOS configuration header file.
 *
 * This file contains the static configuration to be used by the BR-RTOS.
 * The keys defined within this file may be modified by the user to customize
 * the system to their needs.
 */


#ifndef _BR_RTOS_CONFIG_H_
#define _BR_RTOS_CONFIG_H_

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

/**@}*/

/******************************************************************************/
/* C O N S T A N T ,  M A C R O  A N D  T Y P E  D E F I N I T I O N S       */
/******************************************************************************/

/**
 * @defgroup Defs Constant, Macro and Type Definitions
 * @{
 */

/**
 * The maximum number of characters to be used by the kernel object names.
 */
#define __BR_MAX_OBJ_NAME_LEN       (16U)

/**
 * The maximum number of characters to be used by the task name.
 */
#define __BR_MAX_TASK_NAME_LEN      (16U)

/**
 * The heap size specified in number of bytes.
 */
#define __BR_HEAP_SIZE              (1024U)

/**
 * The frequency of the CPU core specified in hertz.
 */
#define __BR_CPU_CLOCK_HZ           (8000000U)

/**
 * Enable/disable the use of asserts within the BR-RTOS code.
 *
 * If the asserts are enabled, many checks will be executed within the code
 * and if any of the asserts do not match the system will be stopped with a
 * breakpoint at the point of the assert.
 * It is recommend to keep asserts enabled during development and debugging
 * and for release versions keep the asserts disabled.
 *
 * The values allowed for this key are:
 * 0U -> disable the asserts
 * 1U -> enable the asserts
 */
#define __BR_USE_ASSERT             (1U)

/**
 * Enable/disable the check of the function parameters.
 *
 * If this feature is enabled the parameters of the functions will be checked and
 * on any invalid parameter found the function will return with E_INVAL if the
 * function returns any value different from void.
 * It is recommended to keep this feature enabled, so any value that is passed as
 * parameter to a function that uses it to index some array will not cause the
 * system to crash or freeze. But in the case that the user code is sure that
 * will not pass invalid parameters to BR-RTOS functions, this feature may be
 * safely disabled to enhance the system performance.
 *
 * The values allowed for this key are:
 * 0U -> disable the parameters check feature
 * 1U -> enable the parameters check feature
 */
#define __BR_CHECK_FUNC_PARAMETERS  (0U)

/* -------------------------------------------------------------------------- */
/* WARNING:                                                                   */
/*                                                                            */
/* The configuration parameters bellow this point are for debugging purposes  */
/* only and must not be enabled and/or changed by the user unless the user    */
/* intends to debug the kernel and its features.                              */
/* -------------------------------------------------------------------------- */

/**
 * Enable/disable debugging of the BR-RTOS kernel.
 *
 * This is used to debug the BR-RTOS internals and must be kept disabled unless
 * the user is debugging the BR-RTOS features.
 *
 * The values allowed for this key are:
 * 0U -> disable the kernel debugging
 * 1U -> enable the kernel debugging
 */
#define __BR_DEBUG (1U)

/**
 * The maxim number of kernel objects within the debugging objects list.
 *
 * The debugging objects list is created just to facilitate the visualization
 * of the kernel objects within the debugger.
 * This list may be accessed by the symbol objListDebug.
 */
#define __BR_MAX_OBJ_LIST_DEBUG (10U)



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

/**@}*/

/**@}*/


#endif /* _BR_RTOS_CONFIG_H_ */
