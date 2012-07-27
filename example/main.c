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

void MyTask1(void);
void MyTask2(void);

/**@}*/

/******************************************************************************/
/* P R I V A T E  V A R I A B L E S                                           */
/******************************************************************************/

/**
 * @defgroup PrivateVar Private Variables
 * @{
 */

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

/**@}*/

/******************************************************************************/
/* P R I V A T E  F U N C T I O N S                                           */
/******************************************************************************/

/**
 * @defgroup PrivateFunc Private Functions
 * @{
 */

void MyTask1(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  gpioInit.GPIO_Pin = GPIO_Pin_8;
  gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
  gpioInit.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &gpioInit);

  while (1U)
  {
    GPIO_SetBits(GPIOC, GPIO_Pin_8);
    BR_TaskWait(2U);
    GPIO_ResetBits(GPIOC, GPIO_Pin_8);
    BR_TaskWait(2U);
  }
}

void MyTask2(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  gpioInit.GPIO_Pin = GPIO_Pin_9;
  gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
  gpioInit.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &gpioInit);

  while (1U)
  {
    GPIO_SetBits(GPIOC, GPIO_Pin_9);
    BR_TaskWait(1U);
    GPIO_ResetBits(GPIOC, GPIO_Pin_9);
    BR_TaskWait(1U);
  }
}

/**@}*/

/******************************************************************************/
/* P U B L I C  F U N C T I O N S                                             */
/******************************************************************************/

/**
 * @defgroup PublicFunc Public Functions
 * @{
 */

int main(void)
{
  BR_KernelInit();

  BR_TaskCreate("My Task 1", MyTask1, 30U, NULL, BR_TASK_PRIORITY_CRITICAL, NULL);
  BR_TaskCreate("My Task 2", MyTask2, 30U, NULL, BR_TASK_PRIORITY_CRITICAL, NULL);

  BR_KernelStartScheduler();

  return 0U;
}

/**@}*/

