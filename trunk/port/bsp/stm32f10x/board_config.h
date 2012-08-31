/**
 * @file        board_config.h
 * @date        2012, August 20
 * @author      Guilherme A. de Santi <guidesanti@yahoo.com.br>
 * @copyright   &copy; 2012, BR-RTOS Development Team
 * @brief       STM32F10x BSP configuration file.
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

/* --------------------------------------------------------------------------- */
/* RCC                                                                         */
/* --------------------------------------------------------------------------- */
#define HSE_CLOCK_HZ  (8000000U)
#define LSE_CLOCK_HZ  (32768U)
#define HSI_CLOCK_HZ  (8000000U)
#define LSI_CLOCK_HZ  (40000U)

/* --------------------------------------------------------------------------- */
/* RTC                                                                         */
/* --------------------------------------------------------------------------- */
#define BOARD_RTC_USE_HSEDIV128 (0U)
#define BOARD_RTC_USE_LSE       (1U)
#define BOARD_RTC_USE_LSI       (2U)
#define BOARD_RTC_N_CONFIGS     (3U)
#define BOARD_RTC_CLOCK_CONFIG  (BOARD_RTC_USE_LSE)

/* --------------------------------------------------------------------------- */
/* SPI                                                                         */
/* --------------------------------------------------------------------------- */
#define __BR_BSP_STM32F10X_USE_SPI1 (1U)
#define __BR_BSP_STM32F10X_USE_SPI2 (0U)
#define __BR_BSP_STM32F10X_USE_SPI3 (0U)


/******************************************************************************/
/* V A R I A B L E S                                                          */
/******************************************************************************/


/******************************************************************************/
/* F U N C T I O N S                                                          */
/******************************************************************************/


/** @} */

#endif /* _BOARD_CONFIG_H_ */
