// ShotListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "StringListCtrl.h"
#include "FHCGlobal.h"

// CStringListCtrl

IMPLEMENT_DYNAMIC(CStringListCtrl, CSortedListCtrl)


CStringListCtrl::CStringListCtrl():
 CSortedListCtrl(TRUE, TRUE)
{
	m_bIncreaseColQty=true;
}

CStringListCtrl::~CStringListCtrl()
{
	/*
	CStringListItem * pItemInfo=NULL;
	for (int i=0;i <GetItemCount() ;i++)
	{
		pItemInfo=(CStringListItem *)GetItemData (i);
		if(pItemInfo!=NULL)
		{
			delete pItemInfo;
		}
	}
	*/
}


BEGIN_MESSAGE_MAP(CStringListCtrl, CSortedListCtrl)
	ON_NOTIFY_REFLECT(LVN_DELETEITEM, OnDeleteItem)

	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetDispInfo)
	ON_WM_CREATE()
END_MESSAGE_MAP()

void CStringListCtrl::OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (pDispInfo->item.mask & LVIF_TEXT) 
	{

		CStringListItem* pAppItem = reinterpret_cast<CStringListItem*>(pDispInfo->item.lParam);
		if(pAppItem )
		{
			lstrcpy (pDispInfo->item.pszText, pAppItem->GetAt (pDispInfo->item.iSubItem));
		}
	}

	*pResult = 0;
}


// CStringListCtrl message handlers

int CStringListCtrl::CompareItems(CItemInfo *pItemInfo1, CItemInfo *pItemInfo2)
{
	CStringListItem *pInfo1 = (CStringListItem *)static_cast<CItemInfo*>(pItemInfo1);
	CStringListItem *pInfo2 = (CStringListItem *)static_cast<CItemInfo*>(pItemInfo2);
	if(!pInfo1||!pInfo2)return 0;

	int nResult;

	int nColumn=GetSortedColumn();
	char *s1=pInfo1->GetAt (nColumn);
	char *s2=pInfo2->GetAt (nColumn);
	if(!s1||!s2)return 0;

	nResult = strcmp(s1,s2);
	
	return IsAscSorted() ? nResult : -nResult;
}

BOOL CStringListCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	// TODO: Add your specialized code here and/or call the base class

	bool b=CSortedListCtrl::Create(dwStyle, rect, pParentWnd, nID);

	return b;

}

int CStringListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CSortedListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

int CStringListCtrl::AppendRowItem(CStringListItem *pItemInfo)
{
	int n=GetItemCount();
	return InsertRowItem(n,pItemInfo);
}

int CStringListCtrl::InsertRowItem(int nInserOrder,CStringListItem *pItemInfo)
{
	int i,j;
	
	if(pItemInfo->m_list .size ()>this->m_HeaderCtrl.GetItemCount ())
	{
		if(!m_arrayColNames.IsEmpty ()&&m_HeaderCtrl.GetItemCount ()==0)
		{
			for(i=0;i<m_arrayColNames.GetCount ();i++)
			{
				InsertColumn (i,m_arrayColNames[i]);
			}
		}

		if(m_bIncreaseColQty)
		{
			for(i=m_HeaderCtrl.GetItemCount ();i<pItemInfo->m_list .size ();i++)
			{
				InsertColumn (i,"КэѕЭПо");
			}
		}
	}

	InsertItem(nInserOrder, LPSTR_TEXTCALLBACK);    
	SetItemData(nInserOrder, (DWORD)pItemInfo);
    
	for(j=0;j<pItemInfo->m_list .size ();j++)
	{
		SetItemText(nInserOrder, j, LPSTR_TEXTCALLBACK);
	}

	return nInserOrder;
}

void CStringListCtrl::OnDeleteItem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    if (m_bDeleteItemData) 
	{
		delete (CStringListItem*)pNMListView->lParam;
    }

	*pResult = 0;
}

void CStringListCtrl::DeleteSelected()
{
	int i,j;

	int iCount=GetItemCount ();
	CStringListItem *pItem=NULL;
	CStringListItem **pItems=new CStringListItem* [iCount+2];
	for(i=0;i<iCount;i++)
	{
		pItems[i]=NULL;
	}

	POSITION pos=GetFirstSelectedItemPosition ();
	int n=0;
	i=0;
	while(pos)
	{
		n=GetNextSelectedItem (pos);
		pItem=(CStringListItem *)GetItemData (n);
		pItems[i]=pItem;
		i++;
	}
	int iSelQty=i;

	////////////////////////////////////
	// Delete them:
	////////////////////////////////////
	bool bDeleted=false;
	while(true)
	{
		bDeleted=false;
		iCount=GetItemCount ();
		for(i=0;i<iCount;i++)
		{
			pItem=(CStringListItem *)GetItemData (i);
			
			for(j=0;j<iSelQty;j++)
			{
				if(pItem==pItems[j])
				{
					DeleteItem (i);
					pItems[j]=NULL;
					bDeleted=true;
					break;
				}
			}
			if(bDeleted)break;
		}

		if(!bDeleted)break;
	}

	delete []pItems;


}

void CStringListCtrl::SetColName(CStringArray &array)
{
	m_arrayColNames.Copy (array);
}
void CStringListCtrl::AutoSize()
{
	int iRowQty=this->GetItemCount ();
	int nCol,nRow;
	CString s;
	char sText[1000];
	int iMaxWidth=0;
	int iWidth=0;
	for(nCol=0;nCol<m_HeaderCtrl.GetItemCount ();nCol++)
	{
		iMaxWidth=0;
		for(nRow=0;nRow<iRowQty;nRow++)
		{
			GetItemText(nRow,nCol,sText,1000);
			iWidth=GetStringWidth (sText);
			if(iWidth>iMaxWidth)
			{
				iMaxWidth=iWidth;
			}
		}
		SetColumnWidth(nCol,iMaxWidth+30);
		//iWidth=SetColumnWidth ( j,LVSCW_AUTOSIZE); this does not work;
	}	
}

void CStringListCtrl::SetIncCol(bool bIncCol)
{
	m_bIncreaseColQty=bIncCol;
}
