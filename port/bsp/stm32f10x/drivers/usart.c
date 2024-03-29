/**
 * @file        usart.c
 * @date        2012, August 03
 * @author      Guilherme A. de Santi <guidesanti@yahoo.com.br>
 * @copyright   &copy; 2012, BR-RTOS Development Team
 * @brief       TODO: add brief description
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
 * @addtogroup USART
 * @{
 */

/******************************************************************************/
/* I N C L U D E S                                                            */
/******************************************************************************/
#include "BR-RTOS.h"
#include "usart.h"
#include "device.h"
#include "port.h"
#include "board_config.h"
#include "misc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"

/******************************************************************************/
/* C O N S T A N T ,  M A C R O  A N D  T Y P E  D E F I N I T I O N S        */
/******************************************************************************/

#define USART_DEFAULT_RX_BUFFER_SIZE  (100U)

#ifndef USART1_DEV_NAME
#define USART1_DEV_NAME "usart1"
#warning "USART1_DEV_NAME is not defined, assuming default value 'usart1'"
#endif

#ifndef USART2_DEV_NAME
#define USART2_DEV_NAME "usart2"
#warning "USART2_DEV_NAME is not defined, assuming default value 'usart2'"
#endif

#ifndef USART3_DEV_NAME
#define USART3_DEV_NAME "usart3"
#warning "USART3_DEV_NAME is not defined, assuming default value 'usart3'"
#endif

#ifndef UART4_DEV_NAME
#define UART4_DEV_NAME "uart4"
#warning "UART4_DEV_NAME is not defined, assuming default value 'uart4'"
#endif

#ifndef UART5_DEV_NAME
#define UART5_DEV_NAME "uart5"
#warning "UART5_DEV_NAME is not defined, assuming default value 'uart5'"
#endif

#ifndef USART1_RX_BUFFER_SIZE
#define USART1_RX_BUFFER_SIZE USART_DEFAULT_RX_BUFFER_SIZE
#warning "USART1_RX_BUFFER_SIZE is not defined, assuming default USART_DEFAULT_RX_BUFFER_SIZE"
#endif

#ifndef USART2_RX_BUFFER_SIZE
#define USART2_RX_BUFFER_SIZE USART_DEFAULT_RX_BUFFER_SIZE
#warning "USART2_RX_BUFFER_SIZE is not defined, assuming default USART_DEFAULT_RX_BUFFER_SIZE"
#endif

#ifndef USART3_RX_BUFFER_SIZE
#define USART3_RX_BUFFER_SIZE USART_DEFAULT_RX_BUFFER_SIZE
#warning "USART3_RX_BUFFER_SIZE is not defined, assuming default USART_DEFAULT_RX_BUFFER_SIZE"
#endif

#ifndef UART4_RX_BUFFER_SIZE
#define UART4_RX_BUFFER_SIZE USART_DEFAULT_RX_BUFFER_SIZE
#warning "UART4_RX_BUFFER_SIZE is not defined, assuming default USART_DEFAULT_RX_BUFFER_SIZE"
#endif

#ifndef UART5_RX_BUFFER_SIZE
#define UART5_RX_BUFFER_SIZE USART_DEFAULT_RX_BUFFER_SIZE
#warning "UART5_RX_BUFFER_SIZE is not defined, assuming default USART_DEFAULT_RX_BUFFER_SIZE"
#endif

typedef struct
{
    USART_TypeDef*    usart;
    uint8_t*          rxBuffer;
    uint16_t          rxBufferIni;
    uint16_t          rxBufferEnd;
    uint16_t          rxBufferSize;
} UsartCtrl;


/******************************************************************************/
/* F U N C T I O N S  P R O T O T Y P E S                                     */
/******************************************************************************/
static BR_Err_t __UsartOpen(BR_Device_t* device, uint8_t flags);
static BR_Err_t __UsartClose(BR_Device_t* device);
static uint32_t __UsartRead(BR_Device_t* device, uint32_t address, uint8_t* buffer, uint32_t nBytes);
static uint32_t __UsartWrite(BR_Device_t* device, uint32_t address, uint8_t* buffer, uint32_t nBytes);
static BR_Err_t __UsartControl(BR_Device_t* device, uint8_t cmd, void* param);
static void __UsartRccConfig(void);
static void __UsartGpioConfig(void);
static void __UsartNvicConfig(void);
static void __UsartDmaConfig(void);
static void __UsartIsr(UsartCtrl* usartCtrl);


/******************************************************************************/
/* V A R I A B L E S                                                          */
/******************************************************************************/

#if (__BR_BSP_STM32F10X_USE_USART1)

static uint8_t usart1RxBuffer[USART1_RX_BUFFER_SIZE];

static UsartCtrl usart1Ctrl =
{
    .usart = USART1,
    .rxBuffer = usart1RxBuffer,
    .rxBufferIni = 0U,
    .rxBufferEnd = 0U,
    .rxBufferSize = USART1_RX_BUFFER_SIZE,
};

static BR_Device_t usart1 =
{
    .type     = BR_DEVICE_TYPE_CHAR,
    .init     = NULL,
    .open     = __UsartOpen,
    .close    = __UsartClose,
    .read     = __UsartRead,
    .write    = __UsartWrite,
    .control  = __UsartControl,
    .custom   = (void*)&usart1Ctrl,
};

#endif

#if (__BR_BSP_STM32F10X_USE_USART2)

static uint8_t usart2RxBuffer[USART2_RX_BUFFER_SIZE];

static UsartCtrl usart2Ctrl =
{
    .usart = USART2,
    .rxBuffer = usart2RxBuffer,
    .rxBufferIni = 0U,
    .rxBufferEnd = 0U,
    .rxBufferSize = USART2_RX_BUFFER_SIZE,
};

static BR_Device_t usart2 =
{
    .type     = BR_DEVICE_TYPE_CHAR,
    .init     = NULL,
    .open     = __UsartOpen,
    .close    = __UsartClose,
    .read     = __UsartRead,
    .write    = __UsartWrite,
    .control  = __UsartControl,
    .custom   = (void*)&usart2Ctrl,
};

#endif

#if (__BR_BSP_STM32F10X_USE_USART3)

static uint8_t usart3RxBuffer[USART3_RX_BUFFER_SIZE];

static UsartCtrl usart3Ctrl =
{
    .usart = USART3,
    .rxBuffer = usart3RxBuffer,
    .rxBufferIni = 0U,
    .rxBufferEnd = 0U,
    .rxBufferSize = USART3_RX_BUFFER_SIZE,
};

static BR_Device_t usart3 =
{
    .type     = BR_DEVICE_TYPE_CHAR,
    .init     = NULL,
    .open     = __UsartOpen,
    .close    = __UsartClose,
    .read     = __UsartRead,
    .write    = __UsartWrite,
    .control  = __UsartControl,
    .custom   = (void*)&usart3Ctrl,
};

#endif

#if (__BR_BSP_STM32F10X_USE_UART4)

static uint8_t uart4RxBuffer[UART4_RX_BUFFER_SIZE];

static UsartCtrl uart4Ctrl =
{
    .usart = USART4,
    .rxBuffer = usart4RxBuffer,
    .rxBufferIni = 0U,
    .rxBufferEnd = 0U,
    .rxBufferSize = USART4_RX_BUFFER_SIZE,
};

static BR_Device_t usart4 =
{
    .type     = BR_DEVICE_TYPE_CHAR,
    .init     = NULL,
    .open     = __UsartOpen,
    .close    = __UsartClose,
    .read     = __UsartRead,
    .write    = __UsartWrite,
    .control  = __UsartControl,
    .custom   = (void*)&uart4Ctrl,
};

#endif

#if (__BR_BSP_STM32F10X_USE_UART5)

static uint8_t uart5RxBuffer[UART5_RX_BUFFER_SIZE];

static UsartCtrl uart5Ctrl =
{
    .usart = USART5,
    .rxBuffer = usart5RxBuffer,
    .rxBufferIni = 0U,
    .rxBufferEnd = 0U,
    .rxBufferSize = USART5_RX_BUFFER_SIZE,
};

static BR_Device_t usart5 =
{
    .type     = BR_DEVICE_TYPE_CHAR,
    .init     = NULL,
    .open     = __UsartOpen,
    .close    = __UsartClose,
    .read     = __UsartRead,
    .write    = __UsartWrite,
    .control  = __UsartControl,
    .custom   = (void*)&uart5Ctrl,
};

#endif


/******************************************************************************/
/* F U N C T I O N S                                                          */
/******************************************************************************/

static BR_Err_t __UsartOpen(BR_Device_t* device, uint8_t flags)
{
  UsartCtrl* usartCtrl = (UsartCtrl*)device->custom;

  __BR_ASSERT(NULL != device);

  USART_Cmd(usartCtrl->usart, ENABLE);

  return E_OK;
}

static BR_Err_t __UsartClose(BR_Device_t* device)
{
  UsartCtrl* usartCtrl = (UsartCtrl*)device->custom;

  __BR_ASSERT(NULL != device);

  USART_Cmd(usartCtrl->usart, DISABLE);

  return E_OK;
}

static uint32_t __UsartRead(BR_Device_t* device, uint32_t address, uint8_t* buffer, uint32_t nBytes)
{
  UsartCtrl* usartCtrl = (UsartCtrl*)device->custom;
  uint32_t bytesRead = 0U;

  /* Check the parameters. */
  __BR_ASSERT(NULL != device);
  __BR_ASSERT(NULL != buffer);

  while ((usartCtrl->rxBufferIni != usartCtrl->rxBufferEnd) &&
         (bytesRead < nBytes))
  {
    buffer[bytesRead] = usartCtrl->rxBuffer[usartCtrl->rxBufferIni];
    usartCtrl->rxBufferIni = ((usartCtrl->rxBufferIni + 1U) % usartCtrl->rxBufferSize);
    bytesRead++;
  }

  return bytesRead;
}

static uint32_t __UsartWrite(BR_Device_t* device, uint32_t address, uint8_t* buffer, uint32_t nBytes)
{
  UsartCtrl* usartCtrl = (UsartCtrl*)device->custom;
  uint8_t* ptr = buffer;

  __BR_ASSERT(NULL != device);
  __BR_ASSERT(NULL != buffer);

  while (nBytes > 0U)
  {
    while (RESET == USART_GetFlagStatus(usartCtrl->usart, USART_FLAG_TXE)) { }
    USART_SendData(usartCtrl->usart, *ptr);
    ptr++;
    nBytes--;
  }

  return nBytes;
}

static BR_Err_t __UsartControl(BR_Device_t* device, uint8_t cmd, void* param)
{
#if 0U
  BR_Err_t ret = E_OK;

  __BR_ASSERT(NULL != device);

  switch (cmd)
  {
    case USART_CMD_SET_BAUDRATE:
    {
      // TODO
    }
    break;

    case USART_CMD_SET_WORDLEN:
    {
      // TODO
    }
    break;

    case USART_CMD_SET_STOPBITS:
    {
      // TODO
    }
    break;

    case USART_CMD_SET_PARITY:
    {
      // TODO
    }
    break;

    case USART_CMD_SET_HW_FL:
    {
      // TODO
    }
    break;

    case USART_CMD_SET_MODE:
    {
      // TODO
    }
    break;

    default:
    {
      ret = E_ERROR;
    }
    break;
  }

  return ret;
#endif

  return E_ENOSYS;
}

static void __UsartRccConfig(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
}

static void __UsartGpioConfig(void)
{
  GPIO_InitTypeDef gpioInitSt;

  /* ------------------------------------------------------------------------ */
  /* USART1                                                                   */
  /* ------------------------------------------------------------------------ */
#if (__BR_BSP_STM32F10X_USE_USART1)
  /* TX pin. */
  gpioInitSt.GPIO_Pin = GPIO_Pin_9;
  gpioInitSt.GPIO_Mode = GPIO_Mode_AF_PP;
  gpioInitSt.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &gpioInitSt);
  /* RX pin. */
  gpioInitSt.GPIO_Pin = GPIO_Pin_10;
  gpioInitSt.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  gpioInitSt.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &gpioInitSt);
#endif

  /* ------------------------------------------------------------------------ */
  /* USART2                                                                   */
  /* ------------------------------------------------------------------------ */
#if (__BR_BSP_STM32F10X_USE_USART2)
  /* TX pin. */
  gpioInitSt.GPIO_Pin = GPIO_Pin_2;
  gpioInitSt.GPIO_Mode = GPIO_Mode_AF_PP;
  gpioInitSt.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &gpioInitSt);
  /* RX pin. */
  gpioInitSt.GPIO_Pin = GPIO_Pin_3;
  gpioInitSt.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  gpioInitSt.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &gpioInitSt);
#endif

  /* ------------------------------------------------------------------------ */
  /* USART3                                                                   */
  /* ------------------------------------------------------------------------ */
#if (__BR_BSP_STM32F10X_USE_USART3)
  /* TX pin. */
  gpioInitSt.GPIO_Pin = GPIO_Pin_10;
  gpioInitSt.GPIO_Mode = GPIO_Mode_AF_PP;
  gpioInitSt.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &gpioInitSt);
  /* RX pin. */
  gpioInitSt.GPIO_Pin = GPIO_Pin_11;
  gpioInitSt.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  gpioInitSt.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &gpioInitSt);
#endif

  /* ------------------------------------------------------------------------ */
  /* UART4                                                                    */
  /* ------------------------------------------------------------------------ */
#if (__BR_BSP_STM32F10X_USE_UART4)
  /* TX pin. */
  gpioInitSt.GPIO_Pin = GPIO_Pin_10;
  gpioInitSt.GPIO_Mode = GPIO_Mode_AF_PP;
  gpioInitSt.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &gpioInitSt);
  /* RX pin. */
  gpioInitSt.GPIO_Pin = GPIO_Pin_11;
  gpioInitSt.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  gpioInitSt.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &gpioInitSt);
#endif

  /* ------------------------------------------------------------------------ */
  /* UART5                                                                    */
  /* ------------------------------------------------------------------------ */
#if (__BR_BSP_STM32F10X_USE_UART5)
  /* TX pin. */
  gpioInitSt.GPIO_Pin = GPIO_Pin_12;
  gpioInitSt.GPIO_Mode = GPIO_Mode_AF_PP;
  gpioInitSt.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &gpioInitSt);
  /* RX pin. */
  gpioInitSt.GPIO_Pin = GPIO_Pin_2;
  gpioInitSt.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  gpioInitSt.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &gpioInitSt);
#endif
}

static void __UsartNvicConfig(void)
{
  NVIC_InitTypeDef nvicInitSt;

  nvicInitSt.NVIC_IRQChannelCmd = ENABLE;
  nvicInitSt.NVIC_IRQChannelPreemptionPriority = 0U;
  nvicInitSt.NVIC_IRQChannelSubPriority = 15U;

  /* ------------------------------------------------------------------------ */
  /* USART1                                                                   */
  /* ------------------------------------------------------------------------ */
#if (__BR_BSP_STM32F10X_USE_USART1)
  nvicInitSt.NVIC_IRQChannel = USART1_IRQn;
  NVIC_Init(&nvicInitSt);
#endif

  /* ------------------------------------------------------------------------ */
  /* USART2                                                                   */
  /* ------------------------------------------------------------------------ */
#if (__BR_BSP_STM32F10X_USE_USART2)
  nvicInitSt.NVIC_IRQChannel = USART2_IRQn;
  NVIC_Init(&nvicInitSt);
#endif

  /* ------------------------------------------------------------------------ */
  /* USART3                                                                   */
  /* ------------------------------------------------------------------------ */
#if (__BR_BSP_STM32F10X_USE_USART3)
  nvicInitSt.NVIC_IRQChannel = USART3_IRQn;
  NVIC_Init(&nvicInitSt);
#endif

  /* ------------------------------------------------------------------------ */
  /* UART4                                                                    */
  /* ------------------------------------------------------------------------ */
#if (__BR_BSP_STM32F10X_USE_UART4)
  nvicInitSt.NVIC_IRQChannel = UART4_IRQn;
  NVIC_Init(&nvicInitSt);
#endif

  /* ------------------------------------------------------------------------ */
  /* UART5                                                                    */
  /* ------------------------------------------------------------------------ */
#if (__BR_BSP_STM32F10X_USE_UART5)
  nvicInitSt.NVIC_IRQChannel = UART5_IRQn;
  NVIC_Init(&nvicInitSt);
#endif
}

static void __UsartDmaConfig(void)
{
  // TODO
}

static void __UsartIsr(UsartCtrl* usartCtrl)
{
  USART_TypeDef* usart = usartCtrl->usart;

  /* Check for RX interrupt flag. */
  if (BITS_GET(usart->SR, USART_SR_RXNE))
  {
    /* Read the received data. */
    usartCtrl->rxBuffer[usartCtrl->rxBufferEnd] = usart->DR;
    usartCtrl->rxBufferEnd = ((usartCtrl->rxBufferEnd + 1U) % usartCtrl->rxBufferSize);
    /* Clear the interrupt flag. */
    BITS_CLEAR(usart->SR, USART_SR_RXNE);
  }

  /*
   * NOTE: The other interrupts should not occur since they are never enabled by this driver.
   */
}

void __BR_UsartInit(void)
{
  USART_InitTypeDef usartInitSt;
  USART_ClockInitTypeDef usartClockInitSt;

  /* RCC configuration. */
  __UsartRccConfig();

  /* GPIO configuration. */
  __UsartGpioConfig();

  /* NVIC configuration. */
  __UsartNvicConfig();

  /* DMA configuration. */
  __UsartDmaConfig();

  usartInitSt.USART_BaudRate = 2400U;
  usartInitSt.USART_WordLength = USART_WordLength_8b;
  usartInitSt.USART_StopBits = USART_StopBits_1;
  usartInitSt.USART_Parity = USART_Parity_No;
  usartInitSt.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  usartInitSt.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  usartClockInitSt.USART_Clock = USART_Clock_Disable;
  usartClockInitSt.USART_CPOL = USART_CPOL_Low;
  usartClockInitSt.USART_CPHA = USART_CPHA_2Edge;
  usartClockInitSt.USART_LastBit = USART_LastBit_Disable;

  /* ------------------------------------------------------------------------ */
  /* USART1                                                                   */
  /* ------------------------------------------------------------------------ */
#if (__BR_BSP_STM32F10X_USE_USART1)
  USART_DeInit(USART1);
  USART_Init(USART1, &usartInitSt);
  USART_ClockInit(USART1, &usartClockInitSt);
  /* Register the usart1 device driver. */
  BR_DeviceRegister(USART1_DEV_NAME, &usart1);
  /* Enable interrupts. */
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
#endif

  /* ------------------------------------------------------------------------ */
  /* USART2                                                                   */
  /* ------------------------------------------------------------------------ */
#if (__BR_BSP_STM32F10X_USE_USART2)
  USART_DeInit(USART2);
  USART_Init(USART2, &usartInitSt);
  USART_ClockInit(USART2, &usartClockInitSt);
  /* Register the usart2 device driver. */
  BR_DeviceRegister(USART2_DEV_NAME, &usart2);
  /* Enable interrupts. */
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
#endif

  /* ------------------------------------------------------------------------ */
  /* USART3                                                                   */
  /* ------------------------------------------------------------------------ */
#if (__BR_BSP_STM32F10X_USE_USART3)
  USART_DeInit(USART3);
  USART_Init(USART3, &usartInitSt);
  USART_ClockInit(USART3, &usartClockInitSt);
  /* Register the usart3 device driver. */
  BR_DeviceRegister(USART3_DEV_NAME, &usart3);
  /* Enable interrupts. */
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
#endif

  /* ------------------------------------------------------------------------ */
  /* USART4                                                                   */
  /* ------------------------------------------------------------------------ */
#if (__BR_BSP_STM32F10X_USE_UART4)
  USART_DeInit(USART4);
  USART_Init(USART4, &usartInitSt);
  /* Register the usart4 device driver. */
  BR_DeviceRegister(USART4_DEV_NAME, &usart4);
  /* Enable interrupts. */
  USART_ITConfig(USART4, USART_IT_RXNE, ENABLE);
#endif

  /* ------------------------------------------------------------------------ */
  /* USART5                                                                   */
  /* ------------------------------------------------------------------------ */
#if (__BR_BSP_STM32F10X_USE_UART5)
  USART_DeInit(USART5);
  USART_Init(USART5, &usartInitSt);
  /* Register the usart5 device driver. */
  BR_DeviceRegister(USART5_DEV_NAME, &usart5);
  /* Enable interrupts. */
  USART_ITConfig(USART5, USART_IT_RXNE, ENABLE);
#endif
}

/**
 * @brief USART1 interrupt service routine.
 */
void __BR_UsartIsr1(void)
{
#if (__BR_BSP_STM32F10X_USE_USART1)
  __UsartIsr(&usart1Ctrl);
#endif
}

/**
 * @brief USART2 interrupt service routine.
 */
void __BR_UsartIsr2(void)
{
#if (__BR_BSP_STM32F10X_USE_USART2)
  __UsartIsr(&usart2Ctrl);
#endif
}

/**
 * @brief USART3 interrupt service routine.
 */
void __BR_UsartIsr3(void)
{
#if (__BR_BSP_STM32F10X_USE_USART3)
  __UsartIsr(&usart3Ctrl);
#endif
}

/**
 * @brief UART4 interrupt service routine.
 */
void __BR_UartIsr4(void)
{
#if (__BR_BSP_STM32F10X_USE_UART4)
  __UsartIsr(&usart4Ctrl);
#endif
}

/**
 * @brief UART5 interrupt service routine.
 */
void __BR_UartIsr5(void)
{
#if (__BR_BSP_STM32F10X_USE_UART5)
  __UsartIsr(&usart5Ctrl);
#endif
}


/** @} */

/** @} */
