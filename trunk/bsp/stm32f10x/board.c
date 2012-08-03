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
  // TODO
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
  // TODO
}

/** @} */

