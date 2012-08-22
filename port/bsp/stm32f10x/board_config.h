/**
 * @file        board_config.h
 * @date        2012, August 20
 * @author      Guilherme A. de Santi <guidesanti@yahoo.com.br>
 * @copyright   &copy; 2012, BR-RTOS Development Team
 * @brief       TODO: add brief description
 *
 * This file is part of the BR-RTOS.
 * The license distribution terms may found at COPYING or LICENSE file
 * within the source directory tree.
 */


#ifndef _BOARD_CONFIG_H_
#define _BOARD_CONFIG_H_

/**
 * @addtogroup ModuleName
 * @{
 */

/******************************************************************************/
/* I N C L U D E S                                                            */
/******************************************************************************/


/******************************************************************************/
/* C O N S T A N T ,  M A C R O  A N D  T Y P E  D E F I N I T I O N S        */
/******************************************************************************/

/* Clocks configuration. */
#define HSE_CLOCK_HZ  (8000000U)
#define LSE_CLOCK_HZ  (32768U)
#define HSI_CLOCK_HZ  (8000000U)
#define LSI_CLOCK_HZ  (40000U)

/* RTC Configuration. */
#define BOARD_RTC_USE_HSEDIV128 (0U)
#define BOARD_RTC_USE_LSE       (1U)
#define BOARD_RTC_USE_LSI       (2U)
#define BOARD_RTC_N_CONFIGS     (3U)
#define BOARD_RTC_CLOCK_CONFIG  (BOARD_RTC_USE_LSE)


/******************************************************************************/
/* V A R I A B L E S                                                          */
/******************************************************************************/


/******************************************************************************/
/* F U N C T I O N S                                                          */
/******************************************************************************/


/** @} */

#endif /* _BOARD_CONFIG_H_ */
