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
void Func1(void);

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
uint32_t task = 0U;

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
  Func1();

  while (1U)
  {
    task = 1;
  }
}

void MyTask2(void)
{
  while (1U)
  {
    task = 2;
  }
}

void Func1(void)
{
  uint32_t aux = 1000;

  while (aux--) { }

  aux = 1000;

  while (aux--) { }

  aux = 1000;

  while (aux--) { }

  aux = 1000;

  while (aux--) { }
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
  BR_TaskCreate("My Task 1", MyTask1, 30U, NULL, NULL);
  BR_TaskCreate("My Task 2", MyTask2, 30U, NULL, NULL);

  BR_KernelStartScheduler();

  return 0U;
}

/**@}*/

