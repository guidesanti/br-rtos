/**
 * @file        board.c
 * @date        2012, August 01
 * @author      Guilherme A. de Santi <guidesanti@yahoo.com.br>
 * @copyright   &copy; 2012, BR-RTOS Development Team
 * @brief       TODO: add brief description
 *
 * This file is part of the BR-RTOS.
 * The license distribution terms may found at COPYING or LICENSE file
 * within the source directory tree.
 */

/**
 * @addtogroup STM32F10xBSP STM32F10x BSP
 * @{
 */

/******************************************************************************/
/* I N C L U D E S                                                            */
/******************************************************************************/
#include "board_config.h"
#include "rtc.h"
#include "spi.h"
#include "usart.h"
#include "adc.h"
#include "stm32f10x.h"

/******************************************************************************/
/* F U N C T I O N S  P R O T O T Y P E S                                     */
/******************************************************************************/
static void __BR_RccConfig(void);
static void __BR_NvicConfig(void);

/******************************************************************************/
/* C O N S T A N T ,  M A C R O  A N D  T Y P E  D E F I N I T I O N S        */
/******************************************************************************/

/******************************************************************************/
/* F U N C T I O N S                                                          */
/******************************************************************************/

/**
 * @brief Initialize the STM32F10x board clock system.
 */
static void __BR_RccConfig(void)
{
#if 0
  /* Configure the clock tree. */
  RCC->CFGR = (config->adcPre | config->ahbPre | config->apb1Pre |
               config->apb2Pre | config->mcoConf | config->pllMul |
               config->pllSrc | config->usbPre);
  /* Enable the HSE clock. */
  if ((RCC_RTCSRC_HSEDIV128 == config->rtcSrc) ||
      (RCC_SYSSRC_HSE == config->sysSrc) ||
      ((RCC_SYSSRC_PLL == config->sysSrc) &&
       ((RCC_PLLSRC_HSEDIV1 == config->pllSrc) ||
        (RCC_PLLSRC_HSEDIV2 == config->pllSrc))))
  {
    BITS_SET(RCC->CR, RCC_CR_HSEON);
    while (!BITS_GET(RCC->CR, RCC_CR_HSERDY)) { }
  }
  /* Enable the PLL clock. */
  if (RCC_SYSSRC_PLL == config->sysSrc)
  {
    BITS_SET(RCC->CR, RCC_CR_PLLON);
    while (!BITS_GET(RCC->CR, RCC_CR_PLLRDY)) { }
  }
  /* Select the SYS clock. */
  RCC->CFGR |= config->sysSrc;
  while (BITS_GET(RCC->CFGR, RCC_CFGR_SW) != (BITS_GET(RCC->CFGR, RCC_CFGR_SWS) >> 2U)) { }

  /* Select the MCO clock. */
  RCC->CFGR |= config->mcoConf;

  /* Configure the RTC clock. */
  if (RCC_RTCSRC_NOCLOCK != config->rtcSrc)
  {
    /* Enable BKP and PWR clock source. */
    RccPeriphClockEnable(RCC_PERIPH_BKP, TRUE);
    RccPeriphClockEnable(RCC_PERIPH_PWR, TRUE);
    /* Enable access to BDCR, BKP and RTC. */
    BITS_SET(PWR->CR, PWR_CR_DBP);
    /* Enable the LSE clock. */
    if (RCC_RTCSRC_LSE == config->rtcSrc)
    {
      BITS_SET(RCC->BDCR, RCC_BDCR_LSEON);
      while (!BITS_GET(RCC->BDCR, RCC_BDCR_LSERDY)) { }
    }
    /* Configure and enable the RTC clock. */
    RCC->BDCR |= (RCC_BDCR_RTCEN | config->rtcSrc);
    /* Disable access to BDCR, BKP and RTC. */
    BITS_CLEAR(PWR->CR, PWR_CR_DBP);
  }
#endif
}

/**
 * @brief Initialize the STM32F10x board interrupt controller.
 */
static void __BR_NvicConfig(void)
{
  // TODO
}

/**
 * @brief Board initialization code.
 */
void __BR_BoardInit(void)
{
  /* Initialize the clock system. */
  __BR_RccConfig();

  /* Initialize the system control, reset and interrupts. */
  __BR_NvicConfig();

  /* Initialize the board devices. */
  __BR_GpioStartUpInit();
  __BR_RtcInit();
  __BR_SpiStartUpInit();
  __BR_UsartInit();
  __BR_AdcInit();
}

/** @} */

