/**
 * @file        object.c
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

#include "BR-RTOS.h"
#include "object.h"
#include "list.h"
#include "port.h"
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

/**@}*/

/******************************************************************************/
/* P R I V A T E  V A R I A B L E S                                           */
/******************************************************************************/

/**
 * @defgroup PrivateVar Private Variables
 * @{
 */

static uint16_t nextId = 0U;

static BR_ListNode_t objList;

#if (1U == __BR_DEBUG)
static BR_Object_t* objListDebug[__BR_MAX_OBJ_LIST_DEBUG];
#endif

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

void __BR_ObjectInit(void)
{
  __BR_ListInit(&objList);
}

BR_Object_t* __BR_ObjectCreate(char* name, BR_ObjectType_t type, void* child)
{
  BR_Object_t* obj = NULL;

  __BR_ASSERT(NULL != name);
  __BR_ASSERT(type < BR_N_OBJ_TYPES);
  __BR_ASSERT(NULL != child);

  /* Allocating memory for the new object. */
  obj = BR_MemAlloc(sizeof(BR_Object_t));
  if (NULL != obj)
  {
#if (1U == __BR_DEBUG)
    objListDebug[nextId] = obj;
#endif
    /* Set the object ID and update the nextId value. */
    obj->id = nextId++;
    /* Set the object type. */
    obj->type = type;
    /* Initialize the object list node and insert it within the objects list. */
    obj->child = child;
    __BR_ListInit(&(obj->node));
    __BR_ListInsertBefore(&objList, &(obj->node));
    /* Set the object name. */
    strncpy(obj->name, name, __BR_MAX_OBJ_NAME_LEN);
    obj->name[__BR_MAX_TASK_NAME_LEN - 1U] = '\0';
  }

  return obj;
}

BR_Object_t* __BR_ObjectFind(uint16_t id)
{
  BR_Object_t* obj = NULL;
  BR_ListNode_t* node = &objList;

  node = node->next;
  while (node != &objList)
  {
    obj = __BR_LIST_ENTRY(node, BR_Object_t, node);
    if (id == obj->id)
    {
      break;
    }
    node = node->next;
  }

  return obj;
}

BR_Object_t* __BR_ObjectFindByName(char* name)
{
  BR_Object_t* obj = NULL;
  BR_ListNode_t* node = &objList;

  node = node->next;
  while (node != &objList)
  {
    obj = __BR_LIST_ENTRY(node, BR_Object_t, node);
    if (0U == strncmp(name, obj->name, __BR_MAX_OBJ_NAME_LEN))
    {
      break;
    }
    node = node->next;
  }

  return obj;
}

/**@}*/
