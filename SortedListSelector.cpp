
#include "stdafx.h"
#include "sortedlistselector.h"
#include "progdlg.h"

CSortedListSelector::CSortedListSelector(void)
{
    m_pListLeft=NULL;
    m_pListRight=NULL;
}

CSortedListSelector::~CSortedListSelector(void)
{
}


void CSortedListSelector::SetLists(CSortedListCtrl *pListLeft,
                                   CSortedListCtrl *pListRight)
{
    m_pListLeft=pListLeft;
    m_pListRight=pListRight;
}

void CSortedListSelector::SetLeftItems (    CItemInfo    **pItemInfo,
                                            int          iItemQty)
{
    SetItems(m_pListLeft,pItemInfo,iItemQty);
    GetItems(m_pListLeft,   &m_arrayLeft);

}

void CSortedListSelector::SetRightItems(    CItemInfo   **pItemInfo,
                                            int         iItemQty)
{
    SetItems(m_pListRight,pItemInfo,iItemQty);
    GetItems(m_pListRight,  &m_arrayRight);
}

void CSortedListSelector::GetLeftItems ( CStringArray *pArray)
{
    pArray->Copy(m_arrayLeft);
}

void CSortedListSelector::GetRightItems( CStringArray *pArray)
{
    pArray->Copy(m_arrayRight);
}


void CSortedListSelector::GetItems(CSortedListCtrl  *pListCtrl, 
                                   CStringArray     *pArrayReturn)
{
    //
    if(!pListCtrl)return;

    pArrayReturn->RemoveAll();

    CItemInfo *pItemInfo=NULL;
    int iIDCol=0;
    CString sIDName;
    for(int i=0;i<pListCtrl->GetItemCount();i++)
    {
        pItemInfo=(CItemInfo *)pListCtrl->GetItemData(i);
        iIDCol=pItemInfo->GetIDCol();
        sIDName=pItemInfo->GetColName(iIDCol);
        pArrayReturn->Add(sIDName);
    }

    return;
}

void CSortedListSelector::SetItems(CSortedListCtrl  *pListCtrl, 
                                   CItemInfo        **pItemInfo,
                                   int              iItemQty)
{
    //
    if(!pListCtrl)return;

    // Delete all of the columns.
    pListCtrl->DeleteAllItems ();
    if(iItemQty==0)return;

    int iColQty=pItemInfo[0]->GetColQty(),i,j;
    for(i=0;i<iColQty;i++)
    {
        pListCtrl->InsertColumn (i,pItemInfo[0]->GetColName(i));
    }

    //
    CProgressDlg dlg;
    dlg.Create ();
    dlg.SetRange (0,iItemQty);
    dlg.SetStatus ("正在加载数据...");
    bool bFailed=false;

    for(i=0;i<iItemQty;i++)
    {
        dlg.SetPos (i);
        if(dlg.CheckCancelButton())
        {
            bFailed=true;
            break;
        }
        pListCtrl->InsertItem(i, LPSTR_TEXTCALLBACK);    
        pListCtrl->SetItemData(i, (DWORD)(pItemInfo[i]));

        for(j=0;j<iColQty;j++)
        {
            pListCtrl->SetItemText(i, j, pItemInfo[i]->GetColValue(j));
        }
    }

    for(j=0;j<iColQty;j++)
    {
        if(pListCtrl->GetItemCount ()==0)
        {
            pListCtrl->SetColumnWidth ( j, LVSCW_AUTOSIZE_USEHEADER);
        }
        else
        {
            pListCtrl->SetColumnWidth ( j, LVSCW_AUTOSIZE);
        }
    }	

    // Add full row selection to listctrl
    pListCtrl->SetFullRowSel(TRUE);

    // Sort the list according to the Name column
    pListCtrl->SortColumn(0, TRUE);

    dlg.DestroyWindow ();

    return;
}

void CSortedListSelector::OnAdd()
{
    if(!m_pListLeft||!m_pListRight)return;

    // 获得选择数据
    POSITION pos = m_pListLeft->GetFirstSelectedItemPosition();
    if (pos == NULL)
    {
        return;
        //TRACEERROR("没有选定数据!");
    }

    int nItem ;
    CItemInfo *pShotItem=NULL;

    int iRightItemQty=0;

    while (pos)
    {
        nItem = m_pListLeft->GetNextSelectedItem(pos);
        pShotItem=(CItemInfo* )m_pListLeft->GetItemData (nItem );

        // Delete the item from left list:
        m_pListLeft->DeleteItem(nItem);

        // Add the item to the right list:
        iRightItemQty=m_pListRight->GetItemCount();
        m_pListRight->InsertItem(iRightItemQty, LPSTR_TEXTCALLBACK);    
        m_pListRight->SetItemData(iRightItemQty, (DWORD)pShotItem);
    }

    //
    GetItems(m_pListLeft,   &m_arrayLeft);
    GetItems(m_pListRight,  &m_arrayRight);


    //
    return;
}

void CSortedListSelector::OnAddAll()
{


    GetItems(m_pListLeft,   &m_arrayLeft);
    GetItems(m_pListRight,  &m_arrayRight);


}

void CSortedListSelector::OnRemove()
{
    GetItems(m_pListLeft,   &m_arrayLeft);
    GetItems(m_pListRight,  &m_arrayRight);

}

void CSortedListSelector::OnRemoveAll()
{
    GetItems(m_pListLeft,   &m_arrayLeft);
    GetItems(m_pListRight,  &m_arrayRight);


}