// MultiChildView.cpp : implementation file
//

#include "stdafx.h"
#include "zoomview.h"
#include "MultiChildView.h"
#include "MultiView.h"


// CMultiChildView

IMPLEMENT_DYNCREATE(CMultiChildView, CFHZoomView)

CMultiChildView::CMultiChildView()
{
    m_bDrawCurrentItem  = false;
    m_pMultiView        = NULL;
    m_bCheckNearLine    = true;
}

CMultiChildView::~CMultiChildView()
{
}

BEGIN_MESSAGE_MAP(CMultiChildView, CFHZoomView)
END_MESSAGE_MAP()

// CMultiChildView drawing
void CMultiChildView::SetCheckNearLine(bool bCheckNearLine)
{
    m_bCheckNearLine = bCheckNearLine;
}

void CMultiChildView::OnDraw(CDC* pDC)
{
    CDocument* pDoc = GetDocument();
}

bool CMultiChildView::UpdateDataRange()
{
    return false;
}

void CMultiChildView::SetDrawCurrentItem(bool bDraw)
{
    m_bDrawCurrentItem = bDraw;
}

void  CMultiChildView::setMultiView(CMultiView *pMultiView)
{
    m_pMultiView = pMultiView;
}

void  CMultiChildView::AddSettingPage(CPropertyPage *pPage)
{
    if(m_pMultiView)
    {
        m_pMultiView->AddSettingPage(pPage);
    }
}

void CMultiChildView::PreSettingDlg()
{
}

void CMultiChildView::PostSettingDlg()
{
}

CPropertyPage * CMultiChildView::GetSettingPage(CRuntimeClass *pClass)
{
    if(m_pMultiView)
    {
        return m_pMultiView->GetSettingPage(pClass);
    }
    return NULL;
}

// CMultiChildView diagnostics

#ifdef _DEBUG
void CMultiChildView::AssertValid() const
{
    CFHZoomView::AssertValid();
}

void CMultiChildView::Dump(CDumpContext& dc) const
{
    CFHZoomView::Dump(dc);
}
#endif //_DEBUG


void  CMultiChildView::AddMenu(UINT MenuID, CString MenuString)
{
    if(m_pMultiView)
    {
        return m_pMultiView->AddMenu(MenuID, MenuString);
    }
}
