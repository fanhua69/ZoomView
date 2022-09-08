
#ifndef FH_MenuDef_TAG
#define FH_MenuDef_TAG

#include "zoomview.h"


// 用于动态菜单
struct _FHZOOMVIEW_EXT_CLASS MenuDef 
{
   UINT     m_nString;                  // 菜单的内容(在string table中)
   UINT     m_nID;                      // 菜单的ID
   UINT     m_nTag;                     // 说明其他特性，或者指向某个结构的指针 
};
    
#endif