/**
 * @file        list.c
 * @date        2012, July 26
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

#include "list.h"
#include "port.h"

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

/**@}*/

/******************************************************************************/
/* P R I V A T E  F U N C T I O N S                                           */
/******************************************************************************/

/**
 * @defgroup PrivateFunc Private Functions
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

void __BR_ListInit( BR_ListNode_t* list )
{
  __BR_ASSERT(NULL != list);
  list->prev = list;
  list->next = list;
}

void __BR_ListInsertAfter( BR_ListNode_t* list, BR_ListNode_t* node )
{
  __BR_ASSERT(NULL != list);
  __BR_ASSERT(NULL != node);
  node->next = list->next;
  node->prev = list;
  list->next->prev = node;
  list->next = node;
}

void __BR_ListInsertBefore( BR_ListNode_t* list, BR_ListNode_t* node )
{
  __BR_ASSERT(NULL != list);
  __BR_ASSERT(NULL != node);
  node->prev = list->prev;
  node->next = list;
  list->prev->next = node;
  list->prev = node;
}

void __BR_ListRemove( BR_ListNode_t* node )
{
  __BR_ASSERT(NULL != node);
  node->prev->next = node->next;
  node->next->prev = node->prev;
  node->next = node;
  node->prev = node;
}

BR_Boolean_t __BR_ListIsEmpty( BR_ListNode_t* node )
{
  if (node->next == node) return TRUE;
  return FALSE;
}

/**@}*/
