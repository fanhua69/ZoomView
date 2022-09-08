// GeoInstDataFileShowView.cpp : implementation file
//

#include "stdafx.h"
#include "ProgDlg.h"
#include "FHZoomViewGlobal.h"
#include "FHEditViewInCtl.h"

// CFHEditViewInCtl

IMPLEMENT_DYNCREATE(CFHEditViewInCtl, CFHEditView)

CFHEditViewInCtl::CFHEditViewInCtl()
{
	m_pMyParent=NULL;
}

CFHEditViewInCtl::~CFHEditViewInCtl()
{
}


BEGIN_MESSAGE_MAP(CFHEditViewInCtl, CFHEditView)
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()


// CFHEditViewInCtl diagnostics

#ifdef _DEBUG
void CFHEditViewInCtl::AssertValid() const
{
	CFHEditView::AssertValid();
}

void CFHEditViewInCtl::Dump(CDumpContext& dc) const
{
	CFHEditView::Dump(dc);
}
#endif //_DEBUG


int CFHEditViewInCtl::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	return 0; //CFHEditView::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

bool CFHEditViewInCtl::SetParent(CWnd *pParent)
{
	ASSERT_VALID(pParent);	
	if(!pParent->IsKindOf (RUNTIME_CLASS(CWnd)))return false;
	m_pMyParent=pParent;

    OnInitialUpdate();
    UpdateGrid();
    m_Grid.AutoSize();

	return true;
}

CMenu * CFHEditViewInCtl::GetPopupMenu()
{
	return NULL;
}

void  CFHEditViewInCtl::UpdateGrid()
{
    SetSize(10,10);
    m_Grid.SetFixedRowCount (1);
    m_Grid.SetFixedColumnCount (1);

    WriteRowOrder();
}