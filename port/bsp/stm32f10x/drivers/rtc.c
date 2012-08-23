/**
 * @file        rtc.c
 * @date        2012, August 20
 * @author      Guilherme A. de Santi <guidesanti@yahoo.com.br>
 * @copyright   &copy; 2012, BR-RTOS Development Team
 * @brief       TODO: add brief description
 *
 * This file is part of the BR-RTOS.
 * The license distribution terms may found at COPYING or LICENSE file
 * within the source directory tree.
 */

/**
 * @addtogroup ModuleName
 * @{
 */

/******************************************************************************/
/* I N C L U D E S                                                            */
/******************************************************************************/
#include "BR-RTOS.h"
#include "port.h"
#include "board_config.h"
#include "rtc.h"
#include "misc.h"
#include "stm32f10x_rcc.h"


/******************************************************************************/
/* C O N S T A N T ,  M A C R O  A N D  T Y P E  D E F I N I T I O N S        */
/******************************************************************************/

#ifndef RTC_DEV_NAME
#define RTC_DEV_NAME "rtc"
#warning "RTC_DEV_NAME is not defined, assuming default value 'rtc'"
#endif

#if (BOARD_RTC_USE_HSEDIV128 == BOARD_RTC_CLOCK_CONFIG)
#define BOARD_RTC_CLOCK_HZ (HSE_CLOCK_HZ / 128U)
#elif (BOARD_RTC_USE_LSE == BOARD_RTC_CLOCK_CONFIG)
#define BOARD_RTC_CLOCK_HZ (LSE_CLOCK_HZ)
#elif (BOARD_RTC_USE_LSI == BOARD_RTC_CLOCK_CONFIG)
#define BOARD_RTC_CLOCK_HZ (LSI_CLOCK_HZ)
#else
#error "RTC clock configuration not defined in board_config.h"
#endif


/******************************************************************************/
/* P R I V A T E  F U N C T I O N S  P R O T O T Y P E S                      */
/******************************************************************************/
static void     __RtcRccConfig(void);
static void     __RtcNvicConfig(void);
static void     __RtcWaitWriteOp(void);
static void     __RtcEnterConfigMode(void);
static void     __RtcExitConfigMode( void );
static BR_Err_t __RtcControl(BR_Device_t* device, uint8_t cmd, void* param);


/******************************************************************************/
/* V A R I A B L E S                                                          */
/******************************************************************************/

static BR_Device_t rtc =
{
    .type     = BR_DEVICE_TYPE_RTC,
    .init     = NULL,
    .open     = NULL,
    .close    = NULL,
    .read     = NULL,
    .write    = NULL,
    .control  = __RtcControl,
    .custom   = NULL,
};

static void (*rtcAlarmCallback)(void) = NULL;

static uint32_t rtcClockConfig[BOARD_RTC_N_CONFIGS] =
{
    RCC_RTCCLKSource_HSE_Div128,
    RCC_RTCCLKSource_LSE,
    RCC_RTCCLKSource_LSI,
};


/******************************************************************************/
/* F U N C T I O N S                                                          */
/******************************************************************************/

static void __RtcRccConfig(void)
{
  /* Enable the PWR and BKP. */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
  /* Select the RTC clock source and enable the RTC clock. */
  BITS_SET(PWR->CR, PWR_CR_DBP);
  if (BOARD_RTC_USE_LSE == BOARD_RTC_CLOCK_CONFIG)
  {
    BITS_SET(RCC->BDCR, RCC_BDCR_LSEON);
    while (!BITS_GET(RCC->BDCR, RCC_BDCR_LSERDY)) { }
  }
  RCC_RTCCLKConfig(rtcClockConfig[BOARD_RTC_CLOCK_CONFIG]);
  RCC_RTCCLKCmd(ENABLE);
  BITS_CLEAR(PWR->CR, PWR_CR_DBP);
}

static void __RtcNvicConfig(void)
{
  NVIC_InitTypeDef nvicInitSt;

  nvicInitSt.NVIC_IRQChannelCmd = ENABLE;
  nvicInitSt.NVIC_IRQChannelPreemptionPriority = 0U;
  nvicInitSt.NVIC_IRQChannelSubPriority = 15U;
  nvicInitSt.NVIC_IRQChannel = RTC_IRQn;
  NVIC_Init(&nvicInitSt);
}

static void __RtcWaitWriteOp(void)
{
  while ((0U) == (RTC->CRL & RTC_CRL_RTOFF)) { asm ("nop"); }
}

static void __RtcEnterConfigMode(void)
{
  RTC->CRL |= RTC_CRL_CNF;
}

static void __RtcExitConfigMode( void )
{
  RTC->CRL &= ~(RTC_CRL_CNF);
}

static BR_Err_t __RtcControl(BR_Device_t* device, uint8_t cmd, void* param)
{
  BR_Err_t ret = E_OK;
  BR_RtcTime_t aux = 0U;

  __BR_ASSERT(NULL != device);

  switch (cmd)
  {
    case RTC_CMD_GET_TIME:
    {
      aux = RTC->CNTL;
      aux |= (RTC->CNTH << 16);
      *((BR_RtcTime_t*)param) = aux;
    }
    break;

    case RTC_CMD_SET_TIME:
    {
      /* Enable access to RTC registers that are within backup domain. */
      BITS_SET(PWR->CR, PWR_CR_DBP);
      __RtcWaitWriteOp();
      __RtcEnterConfigMode();
      /* Set the counter. */
      RTC->CNTH = U32_HIGH(*((BR_RtcTime_t*)param));
      __RtcWaitWriteOp();
      RTC->CNTL = U32_LOW(*((BR_RtcTime_t*)param));
      /* Disable access to RTC registers that are within backup domain. */
      __RtcExitConfigMode();
      __RtcWaitWriteOp();
      BITS_CLEAR(PWR->CR, PWR_CR_DBP);
    }
    break;

    case RTC_CMD_SET_ALARM_TIME:
    {
      /* Enable access to RTC registers that are within backup domain. */
      BITS_SET(PWR->CR, PWR_CR_DBP);
      __RtcWaitWriteOp();
      __RtcEnterConfigMode();
      /* Set the counter. */
      aux = *((BR_RtcTime_t*)param);
      RTC->ALRH = U32_HIGH(aux);
      RTC->ALRL = U32_LOW(aux);
      /* Disable access to RTC registers that are within backup domain. */
      __RtcExitConfigMode();
      __RtcWaitWriteOp();
      BITS_CLEAR(PWR->CR, PWR_CR_DBP);
    }
    break;

    case RTC_CMD_SET_ALARM_ENABLED:
    {
      BITS_SET(RTC->CRH, RTC_CRH_ALRIE);
      BITS_CLEAR(RTC->CRL, RTC_CRL_ALRF);
    }
    break;

    case RTC_CMD_SET_ALARM_DISABLED:
    {
      BITS_CLEAR(RTC->CRH, RTC_CRH_ALRIE);
      BITS_CLEAR(RTC->CRL, RTC_CRL_ALRF);
    }
    break;

    case RTC_CMD_SET_ALARM_CALLBACK:
    {
      rtcAlarmCallback = (void (*)(void))param;
    }
    break;

    default:
    {
      ret = E_INVAL;
    }
    break;
  }

  return ret;
}

void __BR_RtcInit(void)
{
  /* RCC configuration. */
  __RtcRccConfig();

  /* NVIC configuration. */
  __RtcNvicConfig();

  /* Enable access to RTC registers that are within backup domain. */
  BITS_SET(PWR->CR, PWR_CR_DBP);
  __RtcWaitWriteOp();
  __RtcEnterConfigMode();
  /* Set the prescaler to RTC clock - 1, so the second period will be exactly 1 second. */
  RTC->PRLH = (uint16_t)((BOARD_RTC_CLOCK_HZ - (1U)) >> 16);
  RTC->PRLL = (uint16_t)((BOARD_RTC_CLOCK_HZ - (1U)) & 0xFFFF);
  /* Set the counter. */
  RTC->CNTH = 0U;
  RTC->CNTL = 0U;
  /* Disable access to RTC registers that are within backup domain. */
  __RtcExitConfigMode();
  __RtcWaitWriteOp();
  BITS_CLEAR(PWR->CR, PWR_CR_DBP);
  /* Disable all RTC interrupts. */
  RTC->CRH = 0U;
  /* Set the alarm callback to NULL. */
  rtcAlarmCallback = NULL;

  /* Register the RTC device driver. */
  BR_DeviceRegister(RTC_DEV_NAME, &rtc);
}

void __BR_RtcIsr(void)
{
  /* Check for overflow interrupt. */
  if (BITS_GET(RTC->CRL, RTC_CRL_OWF))
  {
    /* Clear the interrupt flag. */
    BITS_CLEAR(RTC->CRL, RTC_CRL_OWF);
  }
  /* Check for alarm interrupt. */
  if (BITS_GET(RTC->CRL, RTC_CRL_ALRF))
  {
    /* Clear the interrupt flag. */
    BITS_CLEAR(RTC->CRL, RTC_CRL_ALRF);
    /* Run the alarm callback. */
    if (NULL != rtcAlarmCallback)
    {
      rtcAlarmCallback();
    }
  }
  /* Check for seconds interrupt. */
  if (BITS_GET(RTC->CRL, RTC_CRL_SECF))
  {
    /* Clear the interrupt flag. */
    BITS_CLEAR(RTC->CRL, RTC_CRL_SECF);
  }
}


/** @} */

