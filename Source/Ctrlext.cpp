// ctrlext.cpp :
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "ctrlext.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// Inline function declarations


/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlNode
CListCtrlEx::CListCtrlEx() : CListCtrl()
 {
 }
CListCtrlEx::~CListCtrlEx()
 { 
}
CImageList* CListCtrlEx::SetImageList(CImageList* pImageList, int nImageListType)
{
	return CListCtrl::SetImageList(pImageList,nImageListType); 
}

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx

BOOL CListCtrlEx::AddColumn(LPCTSTR strItem,int nItem,int nSubItem,int nMask,int nFmt)
{
	LV_COLUMN lvc;
	lvc.mask = nMask;
	lvc.fmt = nFmt;
	lvc.pszText = (LPTSTR) strItem;
	lvc.cx = GetStringWidth(lvc.pszText) + 15;
	if(nMask & LVCF_SUBITEM){
		if(nSubItem != -1)
			lvc.iSubItem = nSubItem;
		else
			lvc.iSubItem = nItem;
	}
	return InsertColumn(nItem,&lvc);
}

BOOL CListCtrlEx::AddItem(int nItem,int nSubItem,LPCTSTR strItem,int nImageIndex)
{
	LV_ITEM lvItem;
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = nItem;
	lvItem.iSubItem = nSubItem;
	lvItem.pszText = (LPTSTR) strItem;
	if(nImageIndex != -1){
		lvItem.mask |= LVIF_IMAGE;
		lvItem.iImage |= LVIF_IMAGE;
	}
	if(nSubItem == 0)
		return InsertItem(&lvItem);
	return SetItem(&lvItem);
}

BOOL CListCtrlEx::AddItem(int nItem,int nSubItem,long item)
{
	CString sItem;
	sItem.Format("%ld",item);
	return AddItem(nItem,nSubItem,sItem);
}

BOOL CListCtrlEx::AddItem(int nItem,int nSubItem,float item)
{
	CString sItem;
	sItem.Format("%1.1f",item);
	return AddItem(nItem,nSubItem,sItem);
}

BOOL CListCtrlEx::AddItem(int nItem,int nSubItem,double item)
{
	CString sItem;
	sItem.Format("%1.1lf",item);
	return AddItem(nItem,nSubItem,sItem);	
}

BOOL CListCtrlEx::AddItem(int nItem,int nSubItem,int item)
{
	CString sItem;
	sItem.Format("%i",item);
	return AddItem(nItem,nSubItem,sItem);
}

/////////////////////////////////////////////////////////////////////////////
