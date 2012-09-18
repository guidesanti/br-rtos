/**
 * @file        adc.c
 * @date        2012, September 17
 * @author      Guilherme A. de Santi <guidesanti@yahoo.com.br>
 * @copyright   &copy; 2012, BR-RTOS Development Team
 * @brief       STM32F10x ADC device driver implementation file.
 *
 * This file is part of the BR-RTOS.
 * The license distribution terms may found at COPYING or LICENSE file
 * within the source directory tree.
 */

/**
 * @addtogroup BSPstm32f10x BSP STM32F10x
 * @{
 */

/**
 * @addtogroup ADC
 * @{
 */

/******************************************************************************/
/* I N C L U D E S                                                            */
/******************************************************************************/
#include "BR-RTOS.h"
#include "board_config.h"
#include "adc.h"
#include "device.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

/******************************************************************************/
/* C O N S T A N T ,  M A C R O  A N D  T Y P E  D E F I N I T I O N S        */
/******************************************************************************/

#define USART_DEFAULT_RX_BUFFER_SIZE  (100U)

#ifndef ADC1_DEV_NAME
#define ADC1_DEV_NAME "adc1"
#warning "ADC1_DEV_NAME is not defined, assuming default value 'adc1'"
#endif

#ifndef ADC2_DEV_NAME
#define ADC2_DEV_NAME "adc2"
#warning "ADC2_DEV_NAME is not defined, assuming default value 'adc2'"
#endif

#ifndef ADC3_DEV_NAME
#define ADC3_DEV_NAME "adc3"
#warning "ADC3_DEV_NAME is not defined, assuming default value 'adc3'"
#endif

#define ADC1_BUFFER_SIZE  (16U)
#define ADC2_BUFFER_SIZE  (16U)
#define ADC3_BUFFER_SIZE  (16U)

typedef struct
{
    uint8_t       use;
    GPIO_TypeDef* gpio;
    uint16_t      pin;
} AdcGpioMap_t;


/******************************************************************************/
/* F U N C T I O N S  P R O T O T Y P E S                                     */
/******************************************************************************/
static BR_Err_t __AdcOpen(BR_Device_t* device, uint8_t flags);
static BR_Err_t __AdcClose(BR_Device_t* device);
static uint32_t __AdcRead(BR_Device_t* device, uint32_t address, uint8_t* buffer, uint32_t nBytes);
static BR_Err_t __AdcControl(BR_Device_t* device, uint8_t cmd, void* param);
static void __AdcRccConfig(void);
static void __AdcGpioConfig(void);
static void __AdcNvicConfig(void);
static void __AdcDmaConfig(void);
static void __AdcIsr(UsartCtrl* usartCtrl);


/******************************************************************************/
/* V A R I A B L E S                                                          */
/******************************************************************************/

static uint16_t adc1Buffer[ADC1_BUFFER_SIZE];

static const BR_Device_t adc1 =
{
    .type     = BR_DEVICE_TYPE_ADC,
    .init     = NULL,
    .open     = __AdcOpen,
    .close    = __AdcClose,
    .read     = __AdcRead,
    .write    = NULL,
    .control  = __AdcControl,
    .custom   = (void*)ADC1,
};

static uint16_t adc2Buffer[ADC1_BUFFER_SIZE];

static const BR_Device_t adc2 =
{
    .type     = BR_DEVICE_TYPE_ADC,
    .init     = NULL,
    .open     = __AdcOpen,
    .close    = __AdcClose,
    .read     = __AdcRead,
    .write    = NULL,
    .control  = __AdcControl,
    .custom   = (void*)ADC2,
};

static uint16_t adc3Buffer[ADC1_BUFFER_SIZE];

static const BR_Device_t adc3 =
{
    .type     = BR_DEVICE_TYPE_ADC,
    .init     = NULL,
    .open     = __AdcOpen,
    .close    = __AdcClose,
    .read     = __AdcRead,
    .write    = NULL,
    .control  = __AdcControl,
    .custom   = (void*)ADC3,
};

static const AdcGpioMap_t adcGpioMap[] =
{
    /*  Use Flag                            Port    Pin         */

    /* ADC1 */
    {   __BR_BSP_STM32F10X_USE_ADC1_CH_0,   GPIOA,  GPIO_Pin_0    },
    {   __BR_BSP_STM32F10X_USE_ADC1_CH_1,   GPIOA,  GPIO_Pin_1    },
    {   __BR_BSP_STM32F10X_USE_ADC1_CH_2,   GPIOA,  GPIO_Pin_2    },
    {   __BR_BSP_STM32F10X_USE_ADC1_CH_3,   GPIOA,  GPIO_Pin_3    },
    {   __BR_BSP_STM32F10X_USE_ADC1_CH_4,   GPIOA,  GPIO_Pin_4    },
    {   __BR_BSP_STM32F10X_USE_ADC1_CH_5,   GPIOA,  GPIO_Pin_5    },
    {   __BR_BSP_STM32F10X_USE_ADC1_CH_6,   GPIOA,  GPIO_Pin_6    },
    {   __BR_BSP_STM32F10X_USE_ADC1_CH_7,   GPIOA,  GPIO_Pin_7    },
    {   __BR_BSP_STM32F10X_USE_ADC1_CH_8,   GPIOB,  GPIO_Pin_0    },
    {   __BR_BSP_STM32F10X_USE_ADC1_CH_9,   GPIOB,  GPIO_Pin_1    },
    {   __BR_BSP_STM32F10X_USE_ADC1_CH_10,  GPIOC,  GPIO_Pin_0    },
    {   __BR_BSP_STM32F10X_USE_ADC1_CH_11,  GPIOC,  GPIO_Pin_1    },
    {   __BR_BSP_STM32F10X_USE_ADC1_CH_12,  GPIOC,  GPIO_Pin_2    },
    {   __BR_BSP_STM32F10X_USE_ADC1_CH_13,  GPIOC,  GPIO_Pin_3    },
    {   __BR_BSP_STM32F10X_USE_ADC1_CH_14,  GPIOC,  GPIO_Pin_4    },
    {   __BR_BSP_STM32F10X_USE_ADC1_CH_15,  GPIOC,  GPIO_Pin_5    },

    /* ADC2 */
    {   __BR_BSP_STM32F10X_USE_ADC2_CH_0,   GPIOA,  GPIO_Pin_0    },
    {   __BR_BSP_STM32F10X_USE_ADC2_CH_1,   GPIOA,  GPIO_Pin_1    },
    {   __BR_BSP_STM32F10X_USE_ADC2_CH_2,   GPIOA,  GPIO_Pin_2    },
    {   __BR_BSP_STM32F10X_USE_ADC2_CH_3,   GPIOA,  GPIO_Pin_3    },
    {   __BR_BSP_STM32F10X_USE_ADC2_CH_4,   GPIOA,  GPIO_Pin_4    },
    {   __BR_BSP_STM32F10X_USE_ADC2_CH_5,   GPIOA,  GPIO_Pin_5    },
    {   __BR_BSP_STM32F10X_USE_ADC2_CH_6,   GPIOA,  GPIO_Pin_6    },
    {   __BR_BSP_STM32F10X_USE_ADC2_CH_7,   GPIOA,  GPIO_Pin_7    },
    {   __BR_BSP_STM32F10X_USE_ADC2_CH_8,   GPIOB,  GPIO_Pin_0    },
    {   __BR_BSP_STM32F10X_USE_ADC2_CH_9,   GPIOB,  GPIO_Pin_1    },
    {   __BR_BSP_STM32F10X_USE_ADC2_CH_10,  GPIOC,  GPIO_Pin_0    },
    {   __BR_BSP_STM32F10X_USE_ADC2_CH_11,  GPIOC,  GPIO_Pin_1    },
    {   __BR_BSP_STM32F10X_USE_ADC2_CH_12,  GPIOC,  GPIO_Pin_2    },
    {   __BR_BSP_STM32F10X_USE_ADC2_CH_13,  GPIOC,  GPIO_Pin_3    },
    {   __BR_BSP_STM32F10X_USE_ADC2_CH_14,  GPIOC,  GPIO_Pin_4    },
    {   __BR_BSP_STM32F10X_USE_ADC2_CH_15,  GPIOC,  GPIO_Pin_5    },

    /* ADC3 */
    {   __BR_BSP_STM32F10X_USE_ADC3_CH_0,   GPIOA,  GPIO_Pin_0    },
    {   __BR_BSP_STM32F10X_USE_ADC3_CH_1,   GPIOA,  GPIO_Pin_1    },
    {   __BR_BSP_STM32F10X_USE_ADC3_CH_2,   GPIOA,  GPIO_Pin_2    },
    {   __BR_BSP_STM32F10X_USE_ADC3_CH_3,   GPIOA,  GPIO_Pin_3    },
    {   __BR_BSP_STM32F10X_USE_ADC3_CH_4,   GPIOF,  GPIO_Pin_6,   },
    {   __BR_BSP_STM32F10X_USE_ADC3_CH_5,   GPIOF,  GPIO_Pin_7,   },
    {   __BR_BSP_STM32F10X_USE_ADC3_CH_6,   GPIOF,  GPIO_Pin_8,   },
    {   __BR_BSP_STM32F10X_USE_ADC3_CH_7,   GPIOF,  GPIO_Pin_9,   },
    {   __BR_BSP_STM32F10X_USE_ADC3_CH_8,   GPIOF,  GPIO_Pin_10,  },
    //{   __BR_BSP_STM32F10X_USE_ADC3_CH_9,   ,    },
    {   __BR_BSP_STM32F10X_USE_ADC3_CH_10,  GPIOC,  GPIO_Pin_0    },
    {   __BR_BSP_STM32F10X_USE_ADC3_CH_11,  GPIOC,  GPIO_Pin_1    },
    {   __BR_BSP_STM32F10X_USE_ADC3_CH_12,  GPIOC,  GPIO_Pin_2    },
    {   __BR_BSP_STM32F10X_USE_ADC3_CH_13,  GPIOC,  GPIO_Pin_3    },
    //{   __BR_BSP_STM32F10X_USE_ADC3_CH_14,  ,    },
    //{   __BR_BSP_STM32F10X_USE_ADC3_CH_15,  ,    },
};


/******************************************************************************/
/* F U N C T I O N S                                                          */
/******************************************************************************/

static BR_Err_t __AdcOpen(BR_Device_t* device, uint8_t flags)
{
#warning "TODO"
//  UsartCtrl* usartCtrl = (UsartCtrl*)device->custom;
//
//  __BR_ASSERT(NULL != device);
//
//  USART_Cmd(usartCtrl->usart, ENABLE);
//
//  return E_OK;
}

static BR_Err_t __AdcClose(BR_Device_t* device)
{
#warning "TODO"
//  UsartCtrl* usartCtrl = (UsartCtrl*)device->custom;
//
//  __BR_ASSERT(NULL != device);
//
//  USART_Cmd(usartCtrl->usart, DISABLE);
//
//  return E_OK;
}

static uint32_t __AdcRead(BR_Device_t* device, uint32_t address, uint8_t* buffer, uint32_t nBytes)
{
#warning "TODO"
//  UsartCtrl* usartCtrl = (UsartCtrl*)device->custom;
//  uint32_t bytesRead = 0U;
//
//  /* Check the parameters. */
//  __BR_ASSERT(NULL != device);
//  __BR_ASSERT(NULL != buffer);
//
//  while ((usartCtrl->rxBufferIni != usartCtrl->rxBufferEnd) &&
//         (bytesRead < nBytes))
//  {
//    buffer[bytesRead] = usartCtrl->rxBuffer[usartCtrl->rxBufferIni];
//    usartCtrl->rxBufferIni = ((usartCtrl->rxBufferIni + 1U) % usartCtrl->rxBufferSize);
//    bytesRead++;
//  }
//
//  return bytesRead;
}

static BR_Err_t __AdcControl(BR_Device_t* device, uint8_t cmd, void* param)
{
#warning "TODO"

  return E_ENOSYS;
}

static void __AdcRccConfig(void)
{
#if (__BR_BSP_STM32F10X_USE_ADC1)
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
#endif

#if (__BR_BSP_STM32F10X_USE_ADC2)
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
#endif

#if (__BR_BSP_STM32F10X_USE_ADC3)
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
#endif
}

static void __AdcGpioConfig(void)
{
  uint8_t index = 0U;
  GPIO_InitTypeDef gpioInitSt;

  gpioInitSt.GPIO_Mode = GPIO_Mode_AIN;
  gpioInitSt.GPIO_Speed = GPIO_Speed_2MHz;

  for (index = 0U; index < sizeof(adcGpioMap); index++)
  {
    if (adcGpioMap[index].use)
    {
      gpioInitSt.GPIO_Pin = adcGpioMap[index].pin;
      GPIO_Init(adcGpioMap[index].gpio, &gpioInitSt);
    }
  }
}

static void __AdcNvicConfig(void)
{
  NVIC_InitTypeDef nvicInitSt;

  nvicInitSt.NVIC_IRQChannelCmd = ENABLE;
  nvicInitSt.NVIC_IRQChannelPreemptionPriority = 0U;
  nvicInitSt.NVIC_IRQChannelSubPriority = 15U;

#if (__BR_BSP_STM32F10X_USE_ADC1 || __BR_BSP_STM32F10X_USE_ADC2)
  nvicInitSt.NVIC_IRQChannel = ADC1_2_IRQn;
  NVIC_Init(&nvicInitSt);
#endif

#if (__BR_BSP_STM32F10X_USE_ADC3)
  nvicInitSt.NVIC_IRQChannel = ADC3_IRQn;
  NVIC_Init(&nvicInitSt);
#endif
}

static void __AdcDmaConfig(void)
{
#warning "TODO"
  // TODO
}

static void __AdcIsr(UsartCtrl* usartCtrl)
{
#warning "TODO"
  // TODO
}

void __BR_AdcInit(void)
{
#warning "TODO"
  /* RCC configuration. */
  __AdcRccConfig();

  /* GPIO configuration. */
  __AdcGpioConfig();

  /* NVIC configuration. */
  __AdcNvicConfig();

  /* DMA configuration. */
  __AdcDmaConfig();

  /* ------------------------------------------------------------------------ */
  /* ADC1                                                                     */
  /* ------------------------------------------------------------------------ */
#warning "TODO"
//  USART_DeInit(USART1);
//  USART_Init(USART1, &usartInitSt);
//  USART_ClockInit(USART1, &usartClockInitSt);
//  /* Register the usart1 device driver. */
//  BR_DeviceRegister(USART1_DEV_NAME, &usart1);
//  /* Enable interrupts. */
//  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

/**
 * @brief ADC1 and ADC2 interrupt service routine.
 */
void __BR_AdcIsr1_2(void)
{
#warning "TODO"
  //__AdcIsr(&adc1_2Ctrl);
}

/**
 * @brief ADC3 interrupt service routine.
 */
void __BR_AdcIsr3(void)
{
#warning "TODO"
  //__AdcIsr(&adc3Ctrl);
}


/** @} */

/** @} */
