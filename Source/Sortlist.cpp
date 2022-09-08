// SortList.cpp : implementation file
//

#include "stdafx.h"
#include "zoomview.h"
#include "SortList.h"
#include ".\sortlist.h"


// CSortList

IMPLEMENT_DYNAMIC(CSortList, CListCtrl)
int CALLBACK SortListCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

CSortList::CSortList()
{
	m_fAsc=TRUE;
	m_nSortedCol=0;
	m_nCallBackFunction=(UINT)SortListCompare;		// 回调函数	
}



CSortList::~CSortList()
{
}


BEGIN_MESSAGE_MAP(CSortList, CListCtrl)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnLvnColumnclick)
END_MESSAGE_MAP()


void CSortList::SetCallBack(UINT  nCallBack)
{
	m_nCallBackFunction=nCallBack;
}

void CSortList::OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if( pNMListView->iSubItem == m_nSortedCol )
		m_fAsc = !m_fAsc;
	else
	{
		m_fAsc = TRUE;
		m_nSortedCol = pNMListView->iSubItem;
	}
	if(m_nCallBackFunction)
	{
		SortItems( (PFNLVCOMPARE)m_nCallBackFunction, (DWORD)this);        
	}

	*pResult = 0;
}

void  CSortList::SortColumn(int nColumn)
{
	m_nSortedCol = nColumn;
	SortItems( (PFNLVCOMPARE)m_nCallBackFunction, (DWORD)this);        
}

int CALLBACK SortListCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CSortList* pV=(CSortList*)lParamSort;
	CString szComp1,szComp2;
	int iCompRes;
	
	DWORD_PTR n;
	for(long i=0;i<pV->GetItemCount ();i++)
	{
		n=pV->GetItemData (i);
		if(n==lParam1)
		{
			szComp1=pV->GetItemText(i,pV->m_nSortedCol);
		}
		else if(n==lParam2)
		{
			szComp2=pV->GetItemText(i,pV->m_nSortedCol);
		}

		if(szComp1!=""&&szComp2!="")break;
	}
	
	iCompRes=szComp1.Compare(szComp2);

	//根据当前的排序方式进行调整
	if(pV->m_fAsc)
		return iCompRes;
	else
		return iCompRes*-1;
}

