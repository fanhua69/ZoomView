// TreeViewText.cpp : implementation file
//

#include "stdafx.h"
#include "zoomview.h"
#include "TreeViewText.h"


// CTreeViewText

IMPLEMENT_DYNCREATE(CTreeViewText, CFHTreeView)

CTreeViewText::CTreeViewText()
{
    SetDataBaseProperty("��ر����ļ���(*.DBReport)","DBReport");
    SetFileProperty("��ر����ļ�(*.txt)","txt");	
}

CTreeViewText::~CTreeViewText()
{
}

BEGIN_MESSAGE_MAP(CTreeViewText, CFHTreeView)
END_MESSAGE_MAP()


// CTreeViewText diagnostics

#ifdef _DEBUG
void CTreeViewText::AssertValid() const
{
	CFHTreeView::AssertValid();
}

void CTreeViewText::Dump(CDumpContext& dc) const
{
	CFHTreeView::Dump(dc);
}
#endif //_DEBUG


// CTreeViewText message handlers

bool CTreeViewText::AddTemplates()
{
    return true;
}