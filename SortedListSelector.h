#pragma once

#include "Iteminfo.h"
#include "SortedListCtrl.h"

class _FHZOOMVIEW_EXT_CLASS CSortedListSelector
{
public:
    CSortedListSelector(void);
    ~CSortedListSelector(void);

    void SetLists(      CSortedListCtrl *pListLeft,
                        CSortedListCtrl *pListRight);


    void GetLeftItems ( CStringArray *pArray);
    void GetRightItems( CStringArray *pArray);

    void SetLeftItems ( CItemInfo   **pItemInfo,
                        int         iItemQty);

    void SetRightItems( CItemInfo   **pItemInfo,
                        int         iItemQty);
   
    void OnAdd();
    void OnAddAll();
    void OnRemove();
    void OnRemoveAll();

protected:
    CSortedListCtrl * m_pListLeft;
    CSortedListCtrl * m_pListRight;

    void SetItems(      CSortedListCtrl *pListCtrl, 
                        CItemInfo       **pItemInfo,
                        int             iItemQty);

    void GetItems(      CSortedListCtrl  *pListCtrl, 
                        CStringArray     *pArrayReturn);

    CStringArray m_arrayLeft;
    CStringArray m_arrayRight;



};
