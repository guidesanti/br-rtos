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

/* --------------------------------------------------------------------------- */
/* USART                                                                       */
/* --------------------------------------------------------------------------- */
#define __BR_BSP_STM32F10X_USE_USART1 (1U)
#define __BR_BSP_STM32F10X_USE_USART2 (0U)
#define __BR_BSP_STM32F10X_USE_USART3 (0U)
#define __BR_BSP_STM32F10X_USE_UART4  (0U)
#define __BR_BSP_STM32F10X_USE_UART5  (0U)

/* --------------------------------------------------------------------------- */
/* ADC                                                                         */
/* --------------------------------------------------------------------------- */
#define __BR_BSP_STM32F10X_USE_ADC1       (1U)
#define __BR_BSP_STM32F10X_USE_ADC1_CH_0  (0U)
#define __BR_BSP_STM32F10X_USE_ADC1_CH_1  (0U)
#define __BR_BSP_STM32F10X_USE_ADC1_CH_2  (1U)
#define __BR_BSP_STM32F10X_USE_ADC1_CH_3  (0U)
#define __BR_BSP_STM32F10X_USE_ADC1_CH_4  (0U)
#define __BR_BSP_STM32F10X_USE_ADC1_CH_5  (0U)
#define __BR_BSP_STM32F10X_USE_ADC1_CH_6  (0U)
#define __BR_BSP_STM32F10X_USE_ADC1_CH_7  (0U)
#define __BR_BSP_STM32F10X_USE_ADC1_CH_8  (0U)
#define __BR_BSP_STM32F10X_USE_ADC1_CH_9  (0U)
#define __BR_BSP_STM32F10X_USE_ADC1_CH_10 (0U)
#define __BR_BSP_STM32F10X_USE_ADC1_CH_11 (0U)
#define __BR_BSP_STM32F10X_USE_ADC1_CH_12 (0U)
#define __BR_BSP_STM32F10X_USE_ADC1_CH_13 (0U)
#define __BR_BSP_STM32F10X_USE_ADC1_CH_14 (0U)
#define __BR_BSP_STM32F10X_USE_ADC1_CH_15 (0U)

#define __BR_BSP_STM32F10X_USE_ADC2       (0U)
#define __BR_BSP_STM32F10X_USE_ADC2_CH_0  (0U)
#define __BR_BSP_STM32F10X_USE_ADC2_CH_1  (0U)
#define __BR_BSP_STM32F10X_USE_ADC2_CH_2  (0U)
#define __BR_BSP_STM32F10X_USE_ADC2_CH_3  (0U)
#define __BR_BSP_STM32F10X_USE_ADC2_CH_4  (0U)
#define __BR_BSP_STM32F10X_USE_ADC2_CH_5  (0U)
#define __BR_BSP_STM32F10X_USE_ADC2_CH_6  (0U)
#define __BR_BSP_STM32F10X_USE_ADC2_CH_7  (0U)
#define __BR_BSP_STM32F10X_USE_ADC2_CH_8  (0U)
#define __BR_BSP_STM32F10X_USE_ADC2_CH_9  (0U)
#define __BR_BSP_STM32F10X_USE_ADC2_CH_10 (0U)
#define __BR_BSP_STM32F10X_USE_ADC2_CH_11 (0U)
#define __BR_BSP_STM32F10X_USE_ADC2_CH_12 (0U)
#define __BR_BSP_STM32F10X_USE_ADC2_CH_13 (0U)
#define __BR_BSP_STM32F10X_USE_ADC2_CH_14 (0U)
#define __BR_BSP_STM32F10X_USE_ADC2_CH_15 (0U)

#define __BR_BSP_STM32F10X_USE_ADC3       (0U)
#define __BR_BSP_STM32F10X_USE_ADC3_CH_0  (0U)
#define __BR_BSP_STM32F10X_USE_ADC3_CH_1  (0U)
#define __BR_BSP_STM32F10X_USE_ADC3_CH_2  (0U)
#define __BR_BSP_STM32F10X_USE_ADC3_CH_3  (0U)
#define __BR_BSP_STM32F10X_USE_ADC3_CH_4  (0U)
#define __BR_BSP_STM32F10X_USE_ADC3_CH_5  (0U)
#define __BR_BSP_STM32F10X_USE_ADC3_CH_6  (0U)
#define __BR_BSP_STM32F10X_USE_ADC3_CH_7  (0U)
#define __BR_BSP_STM32F10X_USE_ADC3_CH_8  (0U)
//#define __BR_BSP_STM32F10X_USE_ADC3_CH_9  (0U)
#define __BR_BSP_STM32F10X_USE_ADC3_CH_10 (0U)
#define __BR_BSP_STM32F10X_USE_ADC3_CH_11 (0U)
#define __BR_BSP_STM32F10X_USE_ADC3_CH_12 (0U)
#define __BR_BSP_STM32F10X_USE_ADC3_CH_13 (0U)
//#define __BR_BSP_STM32F10X_USE_ADC3_CH_14 (0U)
//#define __BR_BSP_STM32F10X_USE_ADC3_CH_15 (0U)


/******************************************************************************/
/* V A R I A B L E S                                                          */
/******************************************************************************/


/******************************************************************************/
/* F U N C T I O N S                                                          */
/******************************************************************************/


/** @} */

#endif /* _BOARD_CONFIG_H_ */
