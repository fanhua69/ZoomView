// ctrlext.h :
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

#ifndef __TREECTLX_H__
#define __TREECTLX_H__

#ifdef _AFX_NO_AFXCMN_SUPPORT
	#error Windows Common Control classes not supported in this library variant.
#endif

#ifndef __AFXWIN_H__
	#include <afxwin.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx
#include "zoomview.h"

class _FHZOOMVIEW_EXT_CLASS CListCtrlEx : public CListCtrl
{
	// Attributes
protected:

	// Operation
public:
	CListCtrlEx();
	~CListCtrlEx();
	CImageList* SetImageList(CImageList* pImageList, int nImageListType = TVSIL_NORMAL);
	BOOL AddColumn(
		LPCTSTR strItem,int nItem,int nSubItem = -1,
		int nMask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM,
		int nFmt = LVCFMT_LEFT);
	BOOL AddItem(int nItem,int nSubItem,LPCTSTR strItem,int nImageIndex = -1);
	BOOL AddItem(int nItem,int nSubItem,long item);
	BOOL AddItem(int nItem,int nSubItem,double item);
	BOOL AddItem(int nItem,int nSubItem,float item);
	BOOL AddItem(int nItem,int nSubItem,int item);	
};

/////////////////////////////////////////////////////////////////////////////
#endif //__TREECTLX_H__
