// .\source\StaticInCtl.cpp : implementation file
//

#include "stdafx.h"
#include "zoomview.h"
#include "staticinctl.h"


// CStaticInCtl

IMPLEMENT_DYNAMIC(CStaticInCtl, CStatic)
CStaticInCtl::CStaticInCtl()
{
}

CStaticInCtl::~CStaticInCtl()
{
}


BEGIN_MESSAGE_MAP(CStaticInCtl, CStatic)
    ON_WM_ERASEBKGND()
    ON_WM_PAINT()
END_MESSAGE_MAP()



// CStaticInCtl message handlers

void CStaticInCtl::SetBackGroudColor(COLORREF nColor)
{
    m_nColorBack=nColor;
    Invalidate();
}
BOOL CStaticInCtl::OnEraseBkgnd(CDC* pDC)
{
    CRect rect;
    this->GetClientRect(&rect);
    pDC->FillSolidRect(0,0,rect.Width(),rect.Height(),m_nColorBack);

    return CStatic::OnEraseBkgnd(pDC);
}

void CStaticInCtl::OnPaint()
{
    CPaintDC dc(this); // device context for painting
}
