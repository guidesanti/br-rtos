/**
 * @file        template.c
 * @date        TODO
 * @author      Guilherme A. de Santi <guidesanti@yahoo.com.br>
 * @copyright   Guilherme A. de Santi &copy; 2012
 * @brief       TODO: Add brief description
 *
 * TODO: Add detailed description
 */


/******************************************************************************/
/* I N C L U D E S                                                            */
/******************************************************************************/

/**
 * @defgroup Includes Header File Include
 * @{
 */

#include "BR-RTOS.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "rtc.h"
#include "gpio.h"
#include "AT25DB161D.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**@}*/

/******************************************************************************/
/* C O N S T A N T ,  M A C R O  A N D  T Y P E  D E F I N I T I O N S       */
/******************************************************************************/

/**
 * @defgroup Defs Constant, Macro and Type Definitions
 * @{
 */

/**@}*/

/******************************************************************************/
/* P R I V A T E  F U N C T I O N S  P R O T O T Y P E S                      */
/******************************************************************************/

/**
 * @defgroup PrivateFuncProto Private Function Prototypes
 * @{
 */

static void Print(char* str);
static void Task1OnTimeout(void* param);
static void OnRtcAlarm(void);
static void MyTask1(void);

/**@}*/

/******************************************************************************/
/* P R I V A T E  V A R I A B L E S                                           */
/******************************************************************************/

/**
 * @defgroup PrivateVar Private Variables
 * @{
 */

BR_Task_t* task1 = NULL;
BR_Mutex_t* tmutex = NULL;
BR_Device_t* terminal = NULL;
BR_Device_t* rtc = NULL;
BR_Device_t* gpioA = NULL;
BR_Device_t* flash = NULL;
//BR_SpiBus_t* spi1 = NULL;
//BR_SpiDevice_t spiDev1 =
//{
//    .config.maxFreqHz = 3000000U,
//    .config.mode = BR_SPI_MODE_0,
//    .config.dataSize = BR_SPI_DATA_SIZE_8_BITS,
//};

/**@}*/

/******************************************************************************/
/* P U B L I C  V A R I A B L E S                                             */
/******************************************************************************/

/**
 * @defgroup PublicVar Public Variables
 * @{
 */

uint32_t tickCounter = 0U;
GPIO_InitTypeDef gpioInit;
BR_Timer_t* timer1 = NULL;

/**@}*/

/******************************************************************************/
/* P R I V A T E  F U N C T I O N S                                           */
/******************************************************************************/

static void Print(char* str)
{
  if (E_OK == BR_IpcMutexAcquire(tmutex, 30U))
  {
    BR_DeviceWrite(terminal, 0U, (uint8_t*)str, strlen(str));
    BR_IpcMutexRelease(tmutex);
  }
}

static void Task1OnTimeout(void* param)
{
  Print("Timeout\r\n");
}

static void OnRtcAlarm(void)
{
  Print("RTC ALARM");
}

static void MyTask1(void)
{
  /* Flash chip select. */
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//  gpioInit.GPIO_Pin = GPIO_Pin_4;
//  gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
//  gpioInit.GPIO_Mode = GPIO_Mode_Out_PP;
//  GPIO_Init(GPIOA, &gpioInit);
//  GPIO_SetBits(GPIOA, GPIO_Pin_4);
//  gpioConfig.bit = GPIO_BIT_4;
//  gpioConfig.function = GPIO_FUNC_OUT_PUSH_PULL;
//  gpioConfig.speed = GPIO_SPEED_50MHZ;
//  BR_DeviceControl(gpioA, GPIO_CMD_SET_CONFIG, &gpioConfig);
//  BR_DeviceWrite(gpioA, GPIO_ADDR_BIT_4, &aux, 1U);

//  BR_DeviceControl(rtc, RTC_CMD_SET_ALARM_CALLBACK, (void*)OnRtcAlarm);
//  time = 30U;
//  BR_DeviceControl(rtc, RTC_CMD_SET_ALARM_TIME, &time);
//  BR_DeviceControl(rtc, RTC_CMD_SET_ALARM_ENABLED, NULL);

  /* Start a test timer. */
  timer1 = BR_TimerCreate("Timer1", 1U, Task1OnTimeout, NULL, 0U);
  BR_ASSERT(NULL != timer1);
  BR_TimerControl(timer1, BR_TIMER_CMD_SET_CONT, NULL);
  BR_TimerStart(timer1);

  while (1U)
  {
//    /* Check for new data within the terminal. */
//    if (BR_DeviceRead(terminal, 0U, buffer, 1U))
//    {
//      BR_DeviceWrite(terminal, 0U, buffer, 1U);
//    }

//    BR_DeviceControl(rtc, RTC_CMD_GET_TIME, &time);
//    snprintf(ch, 15U, "%lu\r\n", time);
//    Print(ch);

//    BR_DeviceControl(flash, AT25DB161D_IOCTL_GET_MANUFACTURE_ID, buffer);

    Print("Task 1\r\n");

    BR_TaskWait(1U);
  }
}


/******************************************************************************/
/* P U B L I C  F U N C T I O N S                                             */
/******************************************************************************/

void BR_AppInit(void)
{
  /* Looking for devices. */
  rtc = BR_DeviceFind("rtc");
  terminal = BR_DeviceFind("usart1");
  BR_DeviceOpen(terminal, 0U);
  gpioA = BR_DeviceFind("gpioA");

  /* Initialize the flash memory device driver. */
  AT25DB161DInit();
  /* Get the flash device. */
  flash = BR_DeviceFind("flash1");

  /* Creating the tmutex. */
  tmutex = BR_IpcMutexCreate("tmutex1");

  /* Creating the application tasks. */
  task1 = BR_TaskCreate("Task 1", MyTask1, 40U, NULL, BR_TASK_PRIORITY_CRITICAL);
}
