/**
 * @file        spi.h
 * @date        2012, August 24
 * @author      Guilherme A. de Santi <guidesanti@yahoo.com.br>
 * @copyright   &copy; 2012, BR-RTOS Development Team
 * @brief       TODO: add brief description
 *
 * This file is part of the BR-RTOS.
 * The license distribution terms may found at COPYING or LICENSE file
 * within the source directory tree.
 */


#ifndef _SPI_H_
#define _SPI_H_

/**
 * @addtogroup STM32F10x_SPI
 * @{
 */

/******************************************************************************/
/* I N C L U D E S                                                            */
/******************************************************************************/


/******************************************************************************/
/* C O N S T A N T ,  M A C R O  A N D  T Y P E  D E F I N I T I O N S        */
/******************************************************************************/


/******************************************************************************/
/* V A R I A B L E S                                                          */
/******************************************************************************/


/******************************************************************************/
/* F U N C T I O N S                                                          */
/******************************************************************************/
void __BR_SpiStartUpInit(void);
void __BR_Spi1Isr(void);
void __BR_Spi2Isr(void);
void __BR_Spi3Isr(void);


/** @} */

#endif /* _SPI_H_ */
