/**
 * @file        BR-RTOSConfig.h
 * @date        2012, July 20
 * @author      Guilherme A. de Santi <guidesanti@yahoo.com.br>
 * @copyright   Guilherme A. de Santi &copy; 2012
 * @brief       BR-RTOS configuration header file.
 * @details
 *
 * This file contains the static configuration to be used by the BR-RTOS.
 * The keys defined within this file may be modified by the user to customize
 * the system to their needs.
 */


#ifndef _BR_RTOS_CONFIG_H_
#define _BR_RTOS_CONFIG_H_

/**
 * @addtogroup BR-RTOSConfig BR-RTOS Configuration Parameters
 * @{
 */

/******************************************************************************/
/* U S E R  C O N F I G U R A T I O N  P A R A M E T E R S                    */
/******************************************************************************/

/**
 * @addtogroup UserConfig User Configuration Parameters
 * @{
 */

/**
 * The maximum number of characters to be used by the kernel object names.
 */
#define __BR_MAX_OBJ_NAME_LEN       (16U)

/**
 * The heap size specified in number of bytes.
 */
#define __BR_HEAP_SIZE              (2048U)

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
#define __BR_CHECK_FUNC_PARAMETERS  (1U)

/** @} */

/******************************************************************************/
/* D E V E L O P E R  C O N F I G U R A T I O N  P A R A M E T E R S          */
/******************************************************************************/

/* -------------------------------------------------------------------------- */
/* WARNING:                                                                   */
/*                                                                            */
/* The configuration parameters bellow this point are for debugging purposes  */
/* only and must not be enabled and/or changed by the user unless the user    */
/* intends to debug the kernel and its features.                              */
/* -------------------------------------------------------------------------- */

/**
 * @addtogroup DevConfig Developer Configuration Parameters
 * @{
 */

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

/** @} */

/** @} */

#endif /* _BR_RTOS_CONFIG_H_ */
