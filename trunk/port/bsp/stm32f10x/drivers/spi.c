/**
 * @file        spi.c
 * @date        2012, August 24
 * @author      Guilherme A. de Santi <guidesanti@yahoo.com.br>
 * @copyright   &copy; 2012, BR-RTOS Development Team
 * @brief       TODO: add brief description
 *
 * This file is part of the BR-RTOS.
 * The license distribution terms may found at COPYING or LICENSE file
 * within the source directory tree.
 */

/**
 * @addtogroup STM32F10x_SPI
 * @{
 */

/******************************************************************************/
/* I N C L U D E S                                                            */
/******************************************************************************/
#include "board_config.h"
#include "spi.h"
#include "spi_core.h"
#include "port.h"
#include "stm32f10x.h"
#include "misc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"


/******************************************************************************/
/* C O N S T A N T ,  M A C R O  A N D  T Y P E  D E F I N I T I O N S        */
/******************************************************************************/

#ifndef SPI1_DEV_NAME
#define SPI1_DEV_NAME "spi1"
#warning "SPI1_DEV_NAME is not defined, assuming default value 'spi1'"
#endif

#ifndef SPI2_DEV_NAME
#define SPI2_DEV_NAME "spi2"
#warning "SPI2_DEV_NAME is not defined, assuming default value 'spi2'"
#endif

#ifndef SPI3_DEV_NAME
#define SPI3_DEV_NAME "spi3"
#warning "SPI3_DEV_NAME is not defined, assuming default value 'spi3'"
#endif

#define SPI_DUMMY_DATA  (0xFFFFU)

typedef struct
{
    SPI_TypeDef*  spi;
    void*         txBuffer;
    void*         rxBuffer;
} __BR_Spi_t;


/******************************************************************************/
/* P R I V A T E  F U N C T I O N S  P R O T O T Y P E S                      */
/******************************************************************************/
static void __SpiRccConfig(void);
static void __SpiGpioConfig(void);
static void __SpiNvicConfig(void);
static void __SpiDmaConfig(void);
static BR_Err_t __SpiInit(BR_Device_t* device);
static BR_Err_t __SpiControl(BR_Device_t* device, uint8_t cmd, void* param);
static BR_Err_t __SpiConfigure(BR_SpiBus_t* bus, BR_SpiConfig_t* config);
static BR_Err_t __SpiTransfer(BR_SpiBus_t* bus, void* txBuffer, void* rxBuffer, uint32_t length);


/******************************************************************************/
/* V A R I A B L E S                                                          */
/******************************************************************************/

static const uint8_t spiMode[BR_SPI_N_MODES] =
{
    /* SPI mode index     SPI mode configuration. */
    /* BR_SPI_MODE_0  */  0x00U,
    /* BR_SPI_MODE_1  */  0x01U,
    /* BR_SPI_MODE_2  */  0x02U,
    /* BR_SPI_MODE_3  */  0x03U,
};

static const uint8_t spiBaudRate[8U] =
{
    /* SPI baud rate index  SPI baud rate configuration. */
    /* FPCLK/2    */  0x00U,
    /* FPCLK/4    */  0x08U,
    /* FPCLK/8    */  0x10U,
    /* FPCLK/16   */  0x18U,
    /* FPCLK/32   */  0x20U,
    /* FPCLK/64   */  0x28U,
    /* FPCLK/128  */  0x30U,
    /* FPCLK/256  */  0x38U,
};

/* -------------------------------------------------------------------------- */
/* SPI1                                                                       */
/* -------------------------------------------------------------------------- */

#if (1U == __BR_BSP_STM32F10X_USE_SPI1)

static const __BR_Spi_t spi1 =
{
    .spi = SPI1,
    .txBuffer = NULL,
    .rxBuffer = NULL,
};

static BR_SpiBus_t spi1Bus =
{
    .parent.init    = __SpiInit,
    .parent.open    = NULL,
    .parent.close   = NULL,
    .parent.read    = NULL,
    .parent.write   = NULL,
    .parent.control = __SpiControl,
    .parent.custom  = (void*)&spi1,
    .configure      = __SpiConfigure,
    .transfer       = __SpiTransfer,
};

#endif

/* -------------------------------------------------------------------------- */
/* SPI2                                                                       */
/* -------------------------------------------------------------------------- */

#if (1U == __BR_BSP_STM32F10X_USE_SPI2)

static const __BR_Spi_t spi2 =
{
    .spi = SPI2,
    .txBuffer = NULL,
    .rxBuffer = NULL,
};

static BR_SpiBus_t spi2Bus =
{
    .parent.init    = __SpiInit,
    .parent.open    = NULL,
    .parent.close   = NULL,
    .parent.read    = NULL,
    .parent.write   = NULL,
    .parent.control = __SpiControl,
    .parent.custom  = (void*)&spi2,
    .configure      = __SpiConfigure,
    .transfer       = __SpiTransfer,
};

#endif

/* -------------------------------------------------------------------------- */
/* SPI3                                                                       */
/* -------------------------------------------------------------------------- */

#if (1U == __BR_BSP_STM32F10X_USE_SPI3)

static const __BR_Spi_t spi3 =
{
    .spi = SPI3,
    .txBuffer = NULL,
    .rxBuffer = NULL,
};

static BR_SpiBus_t spi3Bus =
{
    .parent.init    = __SpiInit,
    .parent.open    = NULL,
    .parent.close   = NULL,
    .parent.read    = NULL,
    .parent.write   = NULL,
    .parent.control = __SpiControl,
    .parent.custom  = (void*)&spi3,
    .configure      = __SpiConfigure,
    .transfer       = __SpiTransfer,
};

#endif


/******************************************************************************/
/* F U N C T I O N S                                                          */
/******************************************************************************/

/**
 * @brief Execute the RCC configuration for SPI controllers.
 */
static void __SpiRccConfig()
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
#if (1U == __BR_BSP_STM32F10X_USE_SPI1)
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
#endif
#if (1U == __BR_BSP_STM32F10X_USE_SPI2)
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
#endif
#if (1U == __BR_BSP_STM32F10X_USE_SPI3)
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
#endif
}

/**
 * @brief Execute the GPIO configuration for SPI controllers.
 */
static void __SpiGpioConfig(void)
{
  GPIO_InitTypeDef gpioInitSt;

  /* ------------------------------------------------------------------------ */
  /* SPI1                                                                     */
  /* ------------------------------------------------------------------------ */
#if (1U == __BR_BSP_STM32F10X_USE_SPI1)
  /* SCK pin. */
  gpioInitSt.GPIO_Pin = GPIO_Pin_5;
  gpioInitSt.GPIO_Mode = GPIO_Mode_AF_PP;
  gpioInitSt.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &gpioInitSt);
  /* MISO pin. */
  gpioInitSt.GPIO_Pin = GPIO_Pin_6;
  gpioInitSt.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  gpioInitSt.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &gpioInitSt);
  /* MOSI pin. */
  gpioInitSt.GPIO_Pin = GPIO_Pin_7;
  gpioInitSt.GPIO_Mode = GPIO_Mode_AF_PP;
  gpioInitSt.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &gpioInitSt);
#endif

  /* ------------------------------------------------------------------------ */
  /* SPI2                                                                     */
  /* ------------------------------------------------------------------------ */
#if (1U == __BR_BSP_STM32F10X_USE_SPI2)
  /* SCK pin. */
  gpioInitSt.GPIO_Pin = GPIO_Pin_13;
  gpioInitSt.GPIO_Mode = GPIO_Mode_AF_PP;
  gpioInitSt.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &gpioInitSt);
  /* MISO pin. */
  gpioInitSt.GPIO_Pin = GPIO_Pin_14;
  gpioInitSt.GPIO_Mode = GPIO_Mode_AF_PP;
  gpioInitSt.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &gpioInitSt);
  /* MOSI pin. */
  gpioInitSt.GPIO_Pin = GPIO_Pin_15;
  gpioInitSt.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  gpioInitSt.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &gpioInitSt);
#endif

  /* ------------------------------------------------------------------------ */
  /* SPI3                                                                     */
  /* ------------------------------------------------------------------------ */
#if (1U == __BR_BSP_STM32F10X_USE_SPI3)
  /* SCK pin. */
  gpioInitSt.GPIO_Pin = GPIO_Pin_3;
  gpioInitSt.GPIO_Mode = GPIO_Mode_AF_PP;
  gpioInitSt.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &gpioInitSt);
  /* MISO pin. */
  gpioInitSt.GPIO_Pin = GPIO_Pin_4;
  gpioInitSt.GPIO_Mode = GPIO_Mode_AF_PP;
  gpioInitSt.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &gpioInitSt);
  /* MOSI pin. */
  gpioInitSt.GPIO_Pin = GPIO_Pin_5;
  gpioInitSt.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  gpioInitSt.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &gpioInitSt);
#endif
}

/**
 * @brief Execute the NVIC configuration for SPI controllers.
 */
static void __SpiNvicConfig(void)
{
  NVIC_InitTypeDef nvicInitSt;

  nvicInitSt.NVIC_IRQChannelCmd = ENABLE;
  nvicInitSt.NVIC_IRQChannelPreemptionPriority = 0U;
  nvicInitSt.NVIC_IRQChannelSubPriority = 15U;

  /* ------------------------------------------------------------------------ */
  /* SPI1                                                                     */
  /* ------------------------------------------------------------------------ */
#if (1U == __BR_BSP_STM32F10X_USE_SPI1)
  nvicInitSt.NVIC_IRQChannel = SPI1_IRQn;
  NVIC_Init(&nvicInitSt);
#endif

  /* ------------------------------------------------------------------------ */
  /* SPI2                                                                     */
  /* ------------------------------------------------------------------------ */
#if (1U == __BR_BSP_STM32F10X_USE_SPI2)
  nvicInitSt.NVIC_IRQChannel = SPI2_IRQn;
  NVIC_Init(&nvicInitSt);
#endif

  /* ------------------------------------------------------------------------ */
  /* SPI3                                                                     */
  /* ------------------------------------------------------------------------ */
#if (1U == __BR_BSP_STM32F10X_USE_SPI3)
  nvicInitSt.NVIC_IRQChannel = SPI3_IRQn;
  NVIC_Init(&nvicInitSt);
#endif
}

/**
 * @brief Execute the DMA configuration for SPI controllers.
 */
static void __SpiDmaConfig(void)
{
  /* Not using the DMA in the current version. */
}

static BR_Err_t __SpiInit(BR_Device_t* device)
{
  // TODO
  return E_ENOSYS;
}

static BR_Err_t __SpiControl(BR_Device_t* device, uint8_t cmd, void* param)
{
  // TODO
  return E_ENOSYS;
}

static BR_Err_t __SpiConfigure(BR_SpiBus_t* bus, BR_SpiConfig_t* config)
{
  BR_Err_t ret = E_OK;
  SPI_TypeDef* spi = NULL;
  RCC_ClocksTypeDef clocks;
  uint32_t spiClock = 0U;
  uint8_t index = 0U;

  /* Check the parameters. */
  __BR_ASSERT(NULL != bus);
  __BR_ASSERT(NULL != config);
#if (__BR_CHECK_FUNC_PARAMETERS)
  if ((NULL != bus) && (NULL != config))
#endif
  {
    /* Set the SPI device. */
    spi = ((__BR_Spi_t*)bus->parent.custom)->spi;
    /* Clear the current configuration and disable the SPI controller temporarily. */
    spi->CR1 = 0U;
    spi->CR2 = 0U;
    /* Configure the frequency. */
    RCC_GetClocksFreq(&clocks);
    if (SPI1 == spi)
    {
      spiClock = clocks.PCLK2_Frequency;
    }
    else
    {
      spiClock = clocks.PCLK1_Frequency;
    }
    for (index = 0U; index < 8U; index++)
    {
      spiClock >>= 1U;
      if (spiClock < config->maxFreqHz)
      {
        break;
      }
    }
    if (index >= 8U)
    {
      index = 7U;
    }
    spi->CR1 |= spiBaudRate[index];
    /* Configure the SPI mode. */
    spi->CR1 |= (uint32_t)spiMode[config->mode];
    /* Configure the data size. */
    if (BR_SPI_DATA_SIZE_16_BITS == config->dataSize)
    {
      spi->CR1 |= SPI_CR1_DFF;
    }
    /* Set the SSM bit, so the slave select will be by software.*/
    spi->CR1 |= (SPI_CR1_SSM | SPI_CR1_SSI);
    /* Set the controller as master and enable it. */
    spi->CR1 |= (SPI_CR1_MSTR | SPI_CR1_SPE);

    ret = E_OK;
  }
#if (__BR_CHECK_FUNC_PARAMETERS)
  else
  {
    ret = E_INVAL;
  }
#endif

  return ret;
}

static BR_Err_t __SpiTransfer(BR_SpiBus_t* bus, void* txBuffer, void* rxBuffer, uint32_t length)
{
  BR_Err_t ret = E_OK;
  volatile uint16_t reg = 0U;
  SPI_TypeDef* spi = NULL;

  /* Check the parameters. */
  __BR_ASSERT(NULL != bus);
  __BR_ASSERT((NULL != txBuffer) || (NULL != rxBuffer));
  __BR_ASSERT(length > 0U);
#if (__BR_CHECK_FUNC_PARAMETERS)
  if ((NULL != bus) &&
      ((NULL != txBuffer) || (NULL != rxBuffer)) &&
      (length > 0U))
#endif
  {
    /* Set the SPI device. */
    spi = ((__BR_Spi_t*)bus->parent.custom)->spi;
    /* Execute the SPI transfer. */
    while (length > 0U)
    {
      /* Clear the RXNE flag. */
      reg = spi->DR;
      /* Wait for TX buffer empty and send byte. */
      while (!BITS_GET(spi->SR, SPI_SR_TXE));
      if (NULL != txBuffer)
      {
        if (BR_SPI_DATA_SIZE_8_BITS == bus->owner->config.dataSize)
        {
          spi->DR = *((uint8_t*)txBuffer);
          txBuffer = (((uint8_t*)txBuffer) + 1U);
        }
        else
        {
          spi->DR = *((uint16_t*)txBuffer);
          txBuffer = (((uint16_t*)txBuffer) + 1U);
        }
      }
      else
      {
        spi->DR = SPI_DUMMY_DATA;
      }
      /* Wait for RX buffer not empty. */
      while (!BITS_GET(spi->SR, SPI_SR_RXNE));
      if (NULL != rxBuffer)
      {
        if (BR_SPI_DATA_SIZE_8_BITS == bus->owner->config.dataSize)
        {
          *((uint8_t*)rxBuffer) = spi->DR;
          rxBuffer = (((uint8_t*)rxBuffer) + 1U);
        }
        else
        {
          *((uint16_t*)rxBuffer) = spi->DR;
          rxBuffer = (((uint16_t*)rxBuffer) + 1U);
        }
      }
      length--;
    }

    /* Wait for the end of transmission. */
    while (!BITS_GET(spi->SR, SPI_SR_TXE) &&
           BITS_GET(SPI1->SR, SPI_SR_BSY));

    ret = E_OK;
  }
#if (__BR_CHECK_FUNC_PARAMETERS)
  else
  {
    ret = E_INVAL;
  }
#endif

  return ret;
}

/**
 * @brief SPI start up initialization code.
 *
 * This function is to be executed once at the system initialization.
 * It will execute the initial configuration for each SPI controller and
 * register its device drivers within the BR-RTOS kernel.
 */
void __BR_SpiStartUpInit(void)
{
  /* RCC configuration. */
  __SpiRccConfig();

  /* GPIO configuration. */
  __SpiGpioConfig();

  /* NVIC configuration. */
  __SpiNvicConfig();

  /* DMA configuration. */
  __SpiDmaConfig();

  /* SPI 1 configuration. */
#if (1U == __BR_BSP_STM32F10X_USE_SPI1)
  BR_SpiBusRegister(&spi1Bus, SPI1_DEV_NAME);
#endif

  /* SPI 2 configuration. */
#if (1U == __BR_BSP_STM32F10X_USE_SPI2)
  BR_SpiBusRegister(&spi2Bus, SPI2_DEV_NAME);
#endif

  /* SPI 3 configuration. */
#if (1U == __BR_BSP_STM32F10X_USE_SPI3)
  BR_SpiBusRegister(&spi3Bus, SPI3_DEV_NAME);
#endif
}

/**
 * @brief SPI1 interrupt service routine.
 */
void __BR_Spi1Isr(void)
{
  // TODO
}

/**
 * @brief SPI2 interrupt service routine.
 */
void __BR_Spi2Isr(void)
{
  // TODO
}

/**
 * @brief SPI3 interrupt service routine.
 */
void __BR_Spi3Isr(void)
{
  // TODO
}


/** @} */

