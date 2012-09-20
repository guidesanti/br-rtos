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
#include "stm32f10x_adc.h"

/******************************************************************************/
/* C O N S T A N T ,  M A C R O  A N D  T Y P E  D E F I N I T I O N S        */
/******************************************************************************/

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
static void __AdcIsr(ADC_TypeDef* adc);


/******************************************************************************/
/* V A R I A B L E S                                                          */
/******************************************************************************/

#if (__BR_BSP_STM32F10X_USE_ADC1)
static BR_Device_t adc1 =
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
#endif

#if (__BR_BSP_STM32F10X_USE_ADC2)
static BR_Device_t adc2 =
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
#endif

#if (__BR_BSP_STM32F10X_USE_ADC3)
static BR_Device_t adc3 =
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
#endif

static const AdcGpioMap_t adcGpioMap[45U] =
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
  ADC_TypeDef* adc = (ADC_TypeDef*)device->custom;

  __BR_ASSERT(NULL != device);

  /* Enable the ADC peripheral. */
  ADC_Cmd(adc, ENABLE);
  /* Calibrate the ADC peripheral. */
  ADC_StartCalibration(adc);
  while (SET == ADC_GetCalibrationStatus(adc)) { }

  return E_OK;
}

static BR_Err_t __AdcClose(BR_Device_t* device)
{
  ADC_TypeDef* adc = (ADC_TypeDef*)device->custom;

  __BR_ASSERT(NULL != device);

  ADC_Cmd(adc, DISABLE);

  return E_OK;
}

static uint32_t __AdcRead(BR_Device_t* device, uint32_t address, uint8_t* buffer, uint32_t nBytes)
{
  ADC_TypeDef* adc = (ADC_TypeDef*)device->custom;
  volatile uint16_t conversionValue = 0U;


  __BR_ASSERT(NULL != device);
  __BR_ASSERT(NULL != buffer);

  /* Convert the address to channel number. */
  address /= 2U;
  /* Configure the channel to convert. */
  ADC_RegularChannelConfig(adc, address, 1U, ADC_SampleTime_1Cycles5);
  /* Clear the EOC flag. */
  ADC_ClearFlag(adc, ADC_FLAG_EOC);
  /* Start the conversion. */
  ADC_SoftwareStartConvCmd(adc, ENABLE);
  /* Wait for the end of conversion. */
  while (SET == ADC_GetFlagStatus(adc, ADC_FLAG_EOC)) { }
  /* Get the conversion value. */
  conversionValue = ADC_GetConversionValue(adc);
  buffer[0U] = U16_LSB(conversionValue);
  buffer[1U] = U16_MSB(conversionValue);

  return 2U;
}

static BR_Err_t __AdcControl(BR_Device_t* device, uint8_t cmd, void* param)
{
  BR_Err_t ret = E_OK;
  ADC_TypeDef* adc = (ADC_TypeDef*)device->custom;

  __BR_ASSERT(NULL != device);

  switch (cmd)
  {
    case ADC_CTRL_SET_DATA_ALIGN:
    {
      uint8_t aux = *((uint8_t*)param);
      if (ADC_DATA_ALIGN_RIGHT == aux)
      {
        BITS_CLEAR(adc->CR2, ADC_CR2_ALIGN);
      }
      else
      {
        BITS_SET(adc->CR2, ADC_CR2_ALIGN);
      }
    }
    break;

    case ADC_CTRL_SET_CH_SAMPLE_TIME:
    {
      BR_AdcChannelSampleTime_t* aux = ((BR_AdcChannelSampleTime_t*)param);
      uint32_t bitMask = (0x07U << (3U * (aux->channel % 10U)));

      aux->sampleTime &= 0x07U;
      if (aux->channel < 10U)
      {
        BITS_CLEAR(adc->SMPR2, bitMask);
        adc->SMPR2 |= (aux->sampleTime << (3U * (aux->channel % 10U)));
      }
      else if (aux->channel < 16U)
      {
        BITS_CLEAR(adc->SMPR1, bitMask);
        adc->SMPR1 |= (aux->sampleTime << (3U * (aux->channel % 10U)));
      }
    }
    break;

    default:
    {
      ret = E_ERROR;
    }
    break;
  }

  return ret;
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

  for (index = 0U; index < 45U; index++)
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
  /* DMA is not used by this device driver. */
}

static void __AdcIsr(ADC_TypeDef* adc)
{
#warning "TODO"
  // TODO
}

void __BR_AdcInit(void)
{
  ADC_InitTypeDef adcInit;

  /* RCC configuration. */
  __AdcRccConfig();

  /* GPIO configuration. */
  __AdcGpioConfig();

  /* NVIC configuration. */
  __AdcNvicConfig();

  /* DMA configuration. */
  __AdcDmaConfig();

  adcInit.ADC_Mode = ADC_Mode_Independent;
  adcInit.ADC_ScanConvMode = DISABLE;
  adcInit.ADC_ContinuousConvMode = DISABLE;
  adcInit.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  adcInit.ADC_DataAlign = ADC_DataAlign_Right;
  adcInit.ADC_NbrOfChannel = 1U;

  /* ------------------------------------------------------------------------ */
  /* ADC1                                                                     */
  /* ------------------------------------------------------------------------ */
#if (__BR_BSP_STM32F10X_USE_ADC1)
  ADC_DeInit(ADC1);
  ADC_Init(ADC1, &adcInit);
  BR_DeviceRegister(ADC1_DEV_NAME, (BR_Device_t*)&adc1);
#endif

  /* ------------------------------------------------------------------------ */
  /* ADC2                                                                     */
  /* ------------------------------------------------------------------------ */
#if (__BR_BSP_STM32F10X_USE_ADC2)
  ADC_DeInit(ADC2);
  ADC_Init(ADC2, &adcInit);
  BR_DeviceRegister(ADC2_DEV_NAME, (BR_Device_t*)&adc2);
#endif

  /* ------------------------------------------------------------------------ */
  /* ADC3                                                                     */
  /* ------------------------------------------------------------------------ */
#if (__BR_BSP_STM32F10X_USE_ADC3)
  ADC_DeInit(ADC3);
  ADC_Init(ADC3, &adcInit);
  BR_DeviceRegister(ADC3_DEV_NAME, (BR_Device_t*)&adc3);
#endif
}

/**
 * @brief ADC1 and ADC2 interrupt service routine.
 */
void __BR_AdcIsr1_2(void)
{
#warning "TODO"
}

/**
 * @brief ADC3 interrupt service routine.
 */
void __BR_AdcIsr3(void)
{
#warning "TODO"
}


/** @} */

/** @} */
