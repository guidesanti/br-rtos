/**
 * @file        list.h
 * @date        2012, July 24
 * @author      Guilherme A. de Santi <guidesanti@yahoo.com.br>
 * @copyright   Guilherme A. de Santi &copy; 2012
 * @brief       TODO: add brief description
 *
 * TODO: add detailed description
 */


#ifndef _LIST_H_
#define _LIST_H_

/**
 * @defgroup ModuleName The Module Name
 * @{
 */

/******************************************************************************/
/* I N C L U D E S                                                            */
/******************************************************************************/

/**
 * @defgroup Includes Header File Include
 * @{
 */

#include "BR-RTOSDefs.h"

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
/* P U B L I C  V A R I A B L E S                                             */
/******************************************************************************/

/**
 * @defgroup PublicVar Public Variables
 * @{
 */

/**@}*/


/******************************************************************************/
/* P U B L I C  F U N C T I O N S                                             */
/******************************************************************************/

/**
 * @defgroup PublicFunc Public Functions
 * @{
 */

void __BR_ListInit( BR_ListNode_t* list );
void __BR_ListInsertAfter( BR_ListNode_t* list, BR_ListNode_t* node );
void __BR_ListInsertBefore( BR_ListNode_t* list, BR_ListNode_t* node );
void __BR_ListRemove( BR_ListNode_t* node );
BR_Boolean_t __BR_ListIsEmpty( BR_ListNode_t* node );

/**@}*/

/**@}*/


#endif /* _LIST_H_ */
