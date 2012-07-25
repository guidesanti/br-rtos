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

#include "port.h"

/**@}*/

/******************************************************************************/
/* C O N S T A N T ,  M A C R O  A N D  T Y P E  D E F I N I T I O N S       */
/******************************************************************************/

/**
 * @defgroup Defs Constant, Macro and Type Definitions
 * @{
 */

typedef struct BR_ListNode
{
    struct BR_ListNode* prev;
    struct BR_ListNode* next;
} BR_ListNode_t;

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

BR_Err_t BR_ListInit(BR_ListNode_t* list)
{
  BR_Err_t ret = E_INVAL;

  __BR_ASSERT(NULL != list);

#if (1U == __BR_CHECK_FUNC_PARAMETERS)
  if (NULL != list)
  {
#endif
  list->prev = list;
  list->next = list;
#if (1U == __BR_CHECK_FUNC_PARAMETERS)
  }
#endif

  return ret;
}

BR_Err_t BR_ListInsertAfter(BR_ListNode_t* list, BR_ListNode_t* node)
{
  BR_Err_t ret = E_INVAL;

  __BR_ASSERT(NULL != list);
  __BR_ASSERT(NULL != node);

#if (1U == __BR_CHECK_FUNC_PARAMETERS)
  if ((NULL != list) && (NULL != node))
  {
#endif
  node->next = list->next;
  node->prev = list;
  list->next->prev = node;
  list->next = node;
#if (1U == __BR_CHECK_FUNC_PARAMETERS)
  }
#endif

  return ret;
}

BR_Err_t BR_ListInsertBefore(BR_ListNode_t* list, BR_ListNode_t* node)
{
  BR_Err_t ret = E_INVAL;

  __BR_ASSERT(NULL != list);
  __BR_ASSERT(NULL != node);

#if (1U == __BR_CHECK_FUNC_PARAMETERS)
  if ((NULL != list) && (NULL != node))
  {
#endif
  node->prev = list->prev;
  node->next = list;
  list->prev->next = node;
  list->prev = node;
#if (1U == __BR_CHECK_FUNC_PARAMETERS)
  }
#endif

  return ret;
}

BR_Err_t BR_ListRemove(BR_ListNode_t* node)
{
  BR_Err_t ret = E_INVAL;

  __BR_ASSERT(NULL != node);

#if (1U == __BR_CHECK_FUNC_PARAMETERS)
  if (NULL != node)
  {
#endif
    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->next = node;
    node->prev = node;
    ret = E_OK;
#if (1U == __BR_CHECK_FUNC_PARAMETERS)
  }
#endif

  return ret;
}

/**@}*/

/**@}*/


#endif /* _LIST_H_ */
