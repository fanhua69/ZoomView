// MultiView.cpp : implementation file
//

#include "stdafx.h"
#include "zoomview.h"
#include "MultiView.h"
#include <vector>
#include "MultiDocument.h"
#include "FHMainFrm.h"
#include "3DRange.h"

// CMultiView

IMPLEMENT_DYNCREATE(CMultiView, CFHZoomView)

CMultiView::CMultiView()
{
    m_iMinDistDoc = -1;
}

CMultiView::~CMultiView()
{
}

BEGIN_MESSAGE_MAP(CMultiView, CFHZoomView)
END_MESSAGE_MAP()


void  CMultiView::OnUpdate (CView* pSender, LPARAM lHint, CObject* pHint) 
{
    C3DRange<double> range;
 
    CMultiDocument * pDoc = (CMultiDocument * )GetDocument();
    CMultiChildView* pView = NULL;

    std::vector<CMyDocTemplate> ::iterator it = pDoc->m_vecDocTemplates.begin();
    while(it!= pDoc->m_vecDocTemplates.end())
    {
        pView = it->getView();
        if(pView && pView->UpdateDataRange())
        {
            range.EnlargeBy(pView->GetDrawRange());
        }
        it->releaseView();

        it++;
    }

    if(range.m_minX > range.m_maxX)
    {
        range.Set(0,1000,0,1000,0,1000);
    }

    SetDrawRange(   range.m_minX,
                    range.m_minY,
                    range.m_maxX,
                    range.m_maxY);

    m_dlgColor.SetRange(range.m_minZ,range.m_maxZ);

    CFHZoomView::OnUpdate(pSender, lHint, pHint) ;

}

// CMultiView drawing

void CMultiView::OnDraw(CDC* pDC)
{
    CMultiDocument * pDoc = (CMultiDocument * )GetDocument();

    std::vector<CMyDocTemplate> ::iterator it = pDoc->m_vecDocTemplates.begin();
    CMultiChildView* pView = NULL;
    while(it!= pDoc->m_vecDocTemplates.end())
    {
        pView = it->getView();
        if(pView)
        {
            pView->CopyViewPar(this);
            pView->OnDraw(pDC);
        }

        it->releaseView();

        it++;
    }

    // TODO: add draw code here
}


// CMultiView diagnostics

#ifdef _DEBUG
void CMultiView::AssertValid() const
{
    CFHZoomView::AssertValid();
}

void CMultiView::Dump(CDumpContext& dc) const
{
    CFHZoomView::Dump(dc);
}
#endif //_DEBUG

void CMultiView::OnActivateView(    BOOL bActivate, 
                                    CView* pActivateView,
                                    CView* pDeactiveView)
{
    CFHMainFrm *pFrm = dynamic_cast<CFHMainFrm*>(AfxGetApp()->GetMainWnd());
    if(!pFrm)
    {
        return;
    }

    pFrm->SetActiveMultiDocument(GetDocument());
}

bool CMultiView::CheckPointingChange(double x,double y,double &dMinDistance)
{
    dMinDistance = 999999999999.9;
    double dMinDistanceBak;

    CMultiDocument * pDoc = (CMultiDocument * )GetDocument();

    int i,nSize = pDoc->m_vecDocTemplates.size();
    CMultiChildView* pView = NULL;
    for(i=0;i<nSize;i++)
    {
        CMyDocTemplate &t=pDoc->m_vecDocTemplates[i];
        pView = t.getView();
        if(!pView)
        {
            continue;
        }

        dMinDistanceBak = dMinDistance;
        pView->CheckPointingChange(x,y,dMinDistance);
        if(dMinDistance<dMinDistanceBak)
        {
            m_iMinDistDoc = i;
        }
        t.releaseView();
    }

    for(i=0;i<nSize;i++)
    {
        CMyDocTemplate &t=pDoc->m_vecDocTemplates[i];
        pView = t.getView();
        if(pView)
        {
            if(i == m_iMinDistDoc)
            {
                pView->SetDrawCurrentItem(true);
            }
            else
            {
                pView->SetDrawCurrentItem(false);
            }
            t.releaseView();
        }
    }


    return true;
}

CString CMultiView::GetInfo(double x, double y)
{
    if(m_iMinDistDoc ==-1)
    {
        return CString("");
    }

    CMultiDocument * pDoc = (CMultiDocument * )GetDocument();
    CMyDocTemplate &t=pDoc->m_vecDocTemplates[m_iMinDistDoc];
    CMultiChildView *pView = t.getView();
    CString sInfo;
    if(pView)
    {
        sInfo = pView->GetInfo(x,y);
        t.releaseView();
    }
    
    return sInfo;
}

void CMultiView::PreSettingDlg()
{
    __super::PreSettingDlg();

    CMultiDocument * pDoc = (CMultiDocument * )GetDocument();
    if(pDoc->m_vecDocTemplates.size()<1)
    {
        return;
    }

    CMyDocTemplate &t=pDoc->m_vecDocTemplates[0];
    CMultiChildView *pView = t.getView();
    if(pView)
    {
        pView->setMultiView(this);
        pView->PreSettingDlg();
        t.releaseView();
    }
}

void CMultiView::PostSettingDlg()
{
    __super::PostSettingDlg();

    CMultiDocument * pDoc = (CMultiDocument * )GetDocument();
    if(pDoc->m_vecDocTemplates.size()<1)
    {
        return;
    }

    CMyDocTemplate &t=pDoc->m_vecDocTemplates[0];
    CMultiChildView *pView = t.getView();
    if(pView)
    {
        pView->setMultiView(this);
        pView->PostSettingDlg();
        t.releaseView();
    }
}

void CMultiView::getChildDocs(std::vector<CMultiChildDoc*> &vecChildDocs)
{
    CMultiDocument * pDoc = (CMultiDocument * )GetDocument();
    pDoc->getChildDocs(vecChildDocs);

    return ;
}

void CMultiView::getChildViews(std::vector<CMultiChildView*> &vecChildViews)
{
    CMultiDocument * pDoc = (CMultiDocument * )GetDocument();
    pDoc->getChildViews(vecChildViews);

    return ;
}

void CMultiView::releaseChildViews()
{
    CMultiDocument * pDoc = (CMultiDocument * )GetDocument();
    pDoc->releaseChildViews();

    return ;
}

void CMultiView::SetCheckNearLine(bool bCheckNearLine)
{
    std::vector<CMultiChildView*> vecChildViews;
    getChildViews(vecChildViews);

    for(int i=0;i<vecChildViews.size();i++)
    {
        CMultiChildView* pView = vecChildViews[i];
        pView->SetCheckNearLine(bCheckNearLine);
    }

    return;
}
