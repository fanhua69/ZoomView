// FHPointLocView.cpp : implementation file
//

#include "stdafx.h"
//#include "zoomview.h"
#include "FHPointLocDoc.h"
#include "FHPointLocView.h"
#include "FHZoomViewGlobal.h"
#include "ProgDlg.h"
#include "..\include\fhpointlocview.h"
#include "FHMainFrm.h"
#include "FHApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFHPointLocView

IMPLEMENT_DYNCREATE(CFHPointLocView, CMultiChildView)

CFHPointLocView::CFHPointLocView()
{
    m_sLineCurrent="";
    m_sAreaCurrent="";

    m_nLinePointed=-1;
    m_nLineCurrent=-1;

    m_nPointPointed=-1;
    m_nPointCurrent=-1;

    m_nPointSize=2;

    m_nDrawStyle=1;
    m_bWriteName=true;

    m_bInfoForLine=true;
    m_bColorDlgCanShow=true;
    m_bUpIncrease=true;

    m_bCheckScreenPos=true;

    m_dlgColor.SetColorSerial (COLOR_RGB);

    m_penCommon.CreatePen(PS_SOLID,0,m_colorMapContent);
    m_penCurrent.CreatePen(PS_SOLID,2,m_colorMapContentSpecial);
    m_penPointed.CreatePen(PS_SOLID,3,RGB(0,255,0));
}

CFHPointLocView::~CFHPointLocView()
{
}


BEGIN_MESSAGE_MAP(CFHPointLocView, CMultiChildView)
    //{{AFX_MSG_MAP(CFHPointLocView)
    ON_COMMAND(ID_FHZoomViewLast, OnFHZoomViewLast)
    ON_COMMAND(ID_FHZoomViewNext, OnFHZoomViewNext)
    ON_UPDATE_COMMAND_UI(ID_FHZoomViewLast, OnUpdateLastLine)
    ON_UPDATE_COMMAND_UI(ID_FHZoomViewNext, OnUpdateNextLine)
    ON_WM_SETFOCUS()
    ON_WM_KILLFOCUS()
    ON_WM_LBUTTONDOWN()
    //}}AFX_MSG_MAP
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFHPointLocView drawing

void CFHPointLocView::OnInitialUpdate()
{
    m_dlgColor.Create ();

    if(m_bColorDlgCanShow)
    {
        m_dlgColor.ShowWindow(SW_SHOW);
    }
    else
    {
        m_dlgColor.ShowWindow(SW_HIDE);
    }


    SetDrawRange(0,0,1000,1000);
    CMultiChildView::OnInitialUpdate();
}

void CFHPointLocView::OnDraw(CDC* pDC)
{
    if(m_nDrawStyle==0)
    {
        DrawPoint(pDC);
    }

    if (m_nDrawStyle==1)
    {
        DrawLine(pDC);
    }

    if(m_nDrawStyle==2)
    {
        DrawLine(pDC);
        DrawPoint(pDC);
    }

    if(m_bWriteName)
    {
        DrawName(pDC);
    }
}

void CFHPointLocView::DrawPoint(CDC *pDC,long nLine)
{
    CFHPointLocDoc *pDoc=(CFHPointLocDoc *)GetDocument();
    if(!pDoc->GetAllPoints ()||pDoc->GetLineNumber ()==0)return ;

    long i=nLine;
    CXYZ *pXYZ=pDoc->GetLine(i);
    long nPointNumber=pDoc->GetLinePointNumber(i);
    if(!pXYZ||nPointNumber<2)return ;

    double x1,y1,x2,y2,x,y;
    GetVisibleArea(x1,y1,x2,y2);

    COLORREF nColor;
    COLORREF nShotColor=RGB(255,0,0);
    COLORREF nRcvColor=RGB(0,255,0);


    for(long j=0;j<nPointNumber;j++)
    {
        x=pXYZ[j].east;
        y=pXYZ[j].north;
        if(x<x1||x>x2||y<y1||y>y2)continue;

        LPtoWorld(x,y);

        if(m_bShowColorValue)
        {
            nColor=m_dlgColor.GetColor (pXYZ[i].value);
        }
        else
        {
            if(pXYZ[i].bShot )
                nColor=nShotColor;
            else
                nColor=nRcvColor;
        }

        pDC->FillSolidRect(CRect(x-m_nPointSize,y-m_nPointSize,x+m_nPointSize,y+m_nPointSize),nColor);
    }
}

void CFHPointLocView::DrawPoint(CDC         *   pDC)
{
    CFHPointLocDoc *pDoc = dynamic_cast<CFHPointLocDoc*>(GetDocument());
    if(!pDoc)
    {
        return;
    }

    if(!pDoc->GetAllPoints ()||pDoc->GetLineNumber ()==0)return ;

    //////////////////////////////////////////
    // Draw the points:
    CPen pen,penCurrent;
    pen.CreatePen(PS_SOLID,0,m_colorMapContent);
    penCurrent.CreatePen(PS_SOLID,0,m_colorMapContentSpecial);
    CPen *pOldPen=pDC->SelectObject(&pen);
    double x,y;

    double x1,y1,x2,y2;
    GetVisibleArea(x1,y1,x2,y2);

    long nPointNumber=pDoc->GetTotalPointNumber();
    CXYZ *pPoint=pDoc->GetAllPoints();
    COLORREF nColor;
    COLORREF nShotColor=RGB(255,0,0);
    COLORREF nRcvColor=RGB(0,255,0);
    long i;


    for( i=0;i<nPointNumber;i++)
    {
        x=pPoint[i].east;
        y=pPoint[i].north;
        if(x<x1||x>x2||y<y1||y>y2)continue;

        LPtoWorld(x,y);

        if(m_bShowColorValue)
        {
            nColor=m_dlgColor.GetColor (pPoint[i].value);
        }
        else
        {
            if(pPoint[i].bShot )
                nColor=nShotColor;
            else
                nColor=nRcvColor;
        }

        pDC->FillSolidRect(CRect(x-m_nPointSize,y-m_nPointSize,x+m_nPointSize,y+m_nPointSize),nColor);
    }

    //////////////////////////////////
    // Draw current point:
    if(m_nPointCurrent!=-1 && m_bDrawCurrentItem) 
    {
        i=m_nPointCurrent;
        int r=m_nPointSize;
        CXYZ *pXYZ=pDoc->m_pPoint ;
        x=pXYZ[i].east;
        y=pXYZ[i].north;
        LPtoWorld(x,y);

        if(m_bShowColorValue)
        {
            nColor=m_dlgColor.GetColor (pPoint[i].value);
        }
        else
        {
            if(pPoint[i].bShot )
                nColor=nShotColor;
            else
                nColor=nRcvColor;
        }

        pDC->FillSolidRect(CRect(x-r-2,y-r-2,x+r+2,y+r+2),RGB(100,100,100));
        pDC->FillSolidRect(CRect(x-r-1,y-r-1,x+r+1,y+r+1),RGB(255,255,255));
        pDC->FillSolidRect(CRect(x-r,y-r,x+r,y+r),nColor);
    }

    //////////////////////////////////
    // Draw the pointing point:
    if(m_nPointPointed!=-1 && m_bDrawCurrentItem)
    {
        i=m_nPointPointed;
        int r=m_nPointSize;
        CXYZ *pXYZ=pDoc->m_pPoint;
        x=pXYZ[i].east;
        y=pXYZ[i].north;
        LPtoWorld(x,y);

        if(m_bShowColorValue)
        {
            nColor=m_dlgColor.GetColor (pPoint[i].value);
        }
        else
        {
            if(pPoint[i].bShot )
                nColor=nShotColor;
            else
                nColor=nRcvColor;
        }

        pDC->FillSolidRect(CRect(x-r-2,y-r-2,x+r+2,y+r+2),RGB(0,0,0));
        pDC->FillSolidRect(CRect(x-r-1,y-r-1,x+r+1,y+r+1),RGB(255,255,255));
        pDC->FillSolidRect(CRect(x-r,y-r,x+r,y+r),nColor);

    }

    pDC->SelectObject(pOldPen);

}

void CFHPointLocView::DrawName(CDC *pDC)
{
    CFHPointLocDoc *pDoc=(CFHPointLocDoc *)GetDocument();
    if(!pDoc->GetAllPoints ()||pDoc->GetLineNumber ()==0)return ;	

    /////////////////////////////////
    // Write the line name:
    COLORREF colorBack=pDC->GetBkColor();

    long nLineNumber=pDoc->GetLineNumber ();
    pDC->SetBkColor(colorBack);
    pDC->SetBkMode(TRANSPARENT);
    CXYZ *pXYZ;
    long i,nPointNumber;
    double x1,y1,x2,y2;

    for(i=0;i<nLineNumber;i++)
    {
        pXYZ=pDoc->GetLine(i);
        nPointNumber=pDoc->GetLinePointNumber(i);
        if(!pXYZ||nPointNumber<2)continue;

        x1=pXYZ[0].east;
        y1=pXYZ[0].north;
        x2=pXYZ[1].east;
        y2=pXYZ[1].north;
        LPtoWorld(x1,y1);
        LPtoWorld(x2,y2);

        WriteFontString(x1,y1,x2,y2,SeperateName(pDoc->GetLineName(i)),pDC);
    }
}


/////////////////////////////////////////////////////////////////////////////
// CFHPointLocView diagnostics

#ifdef _DEBUG
void CFHPointLocView::AssertValid() const
{
    CMultiChildView::AssertValid();
}

void CFHPointLocView::Dump(CDumpContext& dc) const
{
    CMultiChildView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFHPointLocView message handlers
void CFHPointLocView::OnFHZoomViewLast() 
{
    // TODO: Add your command handler code here
    CFHPointLocDoc *pDoc=(CFHPointLocDoc *)GetDocument();
    long n=pDoc->GetLineOrder(m_sLineCurrent);
    n--;
    if(n<0)n=pDoc->GetLineNumber()-1;

    m_sLineCurrent=pDoc->GetLineName(n);

    m_dlgInfo.Show(m_sLineCurrent);
    Invalidate();
}

void CFHPointLocView::OnFHZoomViewNext() 
{
    CFHPointLocDoc *pDoc=(CFHPointLocDoc *)GetDocument();

    long n=pDoc->GetLineOrder(m_sLineCurrent);
    n++;
    if(n>=pDoc->GetLineNumber())n=0;

    m_sLineCurrent=pDoc->GetLineName(n);

    m_dlgInfo.Show(m_sLineCurrent);
    Invalidate();

}


void CFHPointLocView::OnUpdateLastLine(CCmdUI* pCmdUI)
{
    CFHPointLocDoc *pDoc=(CFHPointLocDoc *)GetDocument();
    long n=pDoc->GetLineOrder(m_sLineCurrent);

    long nLineQty=pDoc->GetLineNumber();
    if(n>=nLineQty)n=0;

    pCmdUI->Enable (n>0);

}

void CFHPointLocView::OnUpdateNextLine(CCmdUI* pCmdUI)
{
    CFHPointLocDoc *pDoc=(CFHPointLocDoc *)GetDocument();
    long n=pDoc->GetLineOrder(m_sLineCurrent);

    long nLineQty=pDoc->GetLineNumber();
    if(n>=nLineQty)n=0;

    pCmdUI->Enable (n<nLineQty-1);

}


bool CFHPointLocView::UpdateDataRange()
{
    CFHPointLocDoc *pDoc=dynamic_cast<CFHPointLocDoc *>(GetDocument());
    if(!pDoc)
    {
        return false;
    }

    double dMinEast,dMaxEast,dMinNorth,dMaxNorth,dMin,dMax;
    bool b1=pDoc->GetEastRange(dMinEast,dMaxEast);
    bool b2=pDoc->GetNorthRange(dMinNorth,dMaxNorth);
    bool b3=pDoc->GetValueRange(dMin,dMax);

    if(!b1||!b2)
    {
        SetDrawRange(0,0,1000,1000,1,1,false);
    }
    else
    {
        SetDrawRange(dMinEast,dMinNorth,dMaxEast,dMaxNorth,1,1,false);
    }

    if(!b3)
    {
        m_dlgColor.SetRange (0,100);
    }
    else
    {
        m_dlgColor.SetRange (dMin,dMax);
    }

    return true;
}

void CFHPointLocView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
    // TODO: Add your specialized code here and/or call the base class
    CMultiChildView::OnPrepareDC(pDC, pInfo);
}

void CFHPointLocView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
    // TODO: Add your specialized code here and/or call the base class
    UpdateDataRange();
    m_dlgColor.Invalidate();
    CMultiChildView::OnUpdate(pSender, lHint, pHint);
}


////////////////////////////////////////////
// The size of the point on the SCREEN:
void CFHPointLocView::SetPointSize(int nPointSize)
{
    m_nPointSize=nPointSize;
}

void CFHPointLocView::OnSetFocus(CWnd* pOldWnd) 
{
    CMultiChildView::OnSetFocus(pOldWnd);

    if(m_bColorDlgCanShow){
        m_dlgColor.ShowWindow(SW_SHOW);
    }
    else{
        m_dlgColor.ShowWindow(SW_HIDE);
    }

    HideSearchButton(false);
}

void CFHPointLocView::OnKillFocus(CWnd* pNewWnd) 
{
    CMultiChildView::OnKillFocus(pNewWnd);
    m_dlgColor.ShowWindow(SW_HIDE);

    // TODO: Add your message handler code here

}


void CFHPointLocView::SetDrawStyle(int nStyle)
{
    if(nStyle>=0||nStyle<2)
    {
        m_nDrawStyle=nStyle;
    }
}

void CFHPointLocView::DrawLine(CDC          *   pDC)
{
    CFHPointLocDoc *pDoc=dynamic_cast<CFHPointLocDoc *>(GetDocument());
    if(!pDoc)
    {
        return;
    }

    if(!pDoc->GetAllPoints ()||pDoc->GetLineNumber ()==0)return ;

    /////////////////////////////////////////
    // Draw lines:
    pDC->SetBkMode (TRANSPARENT   );
    CPen *pOldPen=pDC->SelectObject(&m_penCommon);
    long i;

    // Draw the line:
    for(i=0;i<pDoc->GetLineNumber();i++)
    {
        if(pDoc->GetLineProperty(i)->nIs2DLine)
        {
            DrawLine(pDC,i);
        }
        else
        {
            DrawPoint(pDC,i);
        }
    }


    ////////////////////////////////////////////////
    // Draw the Current line:
    if(m_bDrawCurrentItem && m_nLineCurrent!=-1&&pDoc->GetLineProperty(m_nLineCurrent)->nIs2DLine )
    {
        pDC->SelectObject(&m_penCurrent);
        DrawLine(pDC,m_nLineCurrent);
    }

    ////////////////////////////////////////////////
    // Draw the Pointed line:
    if(m_bDrawCurrentItem && m_nLinePointed!=-1&&pDoc->GetLineProperty(m_nLinePointed)->nIs2DLine )
    {
        pDC->SelectObject(&m_penPointed);
        DrawLine(pDC,m_nLinePointed);
    }

    ///////////////////////////////////
    // Select the old pen back:
    pDC->SelectObject(pOldPen);
}

CString CFHPointLocView::GetMapName()
{
    return CString("数据位置平面图");
}

CString CFHPointLocView::GetValueName    (   )
{
    return CString("数值");
}


CString CFHPointLocView::GetInfo(double x, double y)
{
    CFHPointLocDoc *pDoc=(CFHPointLocDoc *)GetDocument();
    if(!pDoc->GetAllPoints ()||pDoc->GetLineNumber ()==0)return CString(" ");	

    CString sMapName = GetMapName();

    CString s=sMapName+"\r\n=========\r\n";

    if(m_nLinePointed!=-1&&m_nPointPointed!=-1)
    {
        s+="测线:"+SeperateName(pDoc->GetLineName(m_nLinePointed))+"\r\n";

        CXYZ *pPointing=pDoc->GetAllPoints();
        CLineProperty *pProperty=pDoc->GetLineProperty (m_nLinePointed);
        pPointing =&pPointing[m_nPointPointed];

        if(pProperty->nIs2DLine )
        {
            s+="桩号:"+vtos(pPointing->dStation)+"\r\n";
        }
        else
        {
            s+="线号:"+CString(pPointing->sLine )+"\r\n";
            s+="点号:"+vtos(pPointing->dStation)+"\r\n";
        }
        s+=GetValueName()+":"+vtos(pPointing->value,1)+"\r\n";
    }
    s+="当前X坐标:"+vtos(x,1)+"\r\n";
    s+="当前Y坐标:"+vtos(y,1);


    return s;
}



void CFHPointLocView::OnLButtonDown(UINT nFlags, CPoint point) 
{
    CFHPointLocDoc *pDoc=(CFHPointLocDoc *)GetDocument();
    if(!pDoc->GetAllPoints ()||pDoc->GetLineNumber ()==0)return ;	

    if(m_eZoomState==statueIdle)
    {
        int nPointBak=m_nPointCurrent;
        int nLineBak=m_nLineCurrent;
        m_nLineCurrent=m_nLinePointed;
        m_nPointCurrent=m_nPointPointed;
        if(nPointBak!=m_nPointCurrent||nLineBak!=m_nLineCurrent)
        {
            Invalidate();
        }
    }

    CMultiChildView::OnLButtonDown(nFlags, point);
}

bool CFHPointLocView::DrawLine(CDC *pDC, long nLineOrder)
{
    CFHPointLocDoc *pDoc=(CFHPointLocDoc *)GetDocument();
    if(!pDoc->GetAllPoints ()||pDoc->GetLineNumber ()==0)return false;	

    long i=nLineOrder;
    CXYZ *pXYZ=pDoc->GetLine(i);
    long nPointNumber=pDoc->GetLinePointNumber(i);
    if(!pXYZ||nPointNumber<2)return false;

    CString s=SeperateName(pDoc->GetLineName(i));
    for(long j=1;j<nPointNumber;j++)
    {
        Line(pDC,pXYZ[j-1].east,pXYZ[j-1].north,pXYZ[j].east,pXYZ[j].north);
    }

    return true;

}

void CFHPointLocView::UpdateDrawingArea(CString sArea, 
                                        CLineProperty *pLines, 
                                        long nLineNumber, 
                                        CString sCurrentLine)
{
    CFHPointLocDoc *pDoc=(CFHPointLocDoc *)GetDocument();
    if(m_sAreaCurrent==sArea)return;

    pDoc->SetData(sArea,pLines, nLineNumber);

    m_sAreaCurrent=sArea;
    m_sLineCurrent=sCurrentLine;

    // Do call this after you set data:
    if(!pDoc->GetAllPoints ()||!pDoc->GetLineName (0))
    {
        SetDrawRange(0,0,1000,1000,1,1);
    }
    else
    {
        UpdateDataRange();
    }
    ZoomFull();

    return ;
}

void CFHPointLocView::UpdateDrawingLine(CString sArea, CString sLineCurrent) 
{
    m_sLineCurrent=sLineCurrent;
    Invalidate();
    return ;
}



bool CFHPointLocView::CheckPointingChange(double x,double y,double &dMinDistance)
{
    CFHPointLocDoc *pDoc=(CFHPointLocDoc *)GetDocument();
    if(!pDoc->GetAllPoints ()||pDoc->GetLineNumber ()==0)
    {
        return false;
    }

    if(m_bCheckScreenPos)
    {
        LPtoWorld(x,y);
    }

    int nLineBak=m_nLinePointed;
    int nPointBak=m_nPointPointed;
    double dMinDisLine,dMinDisPoint;

    m_nLinePointed=GetNearestLine(x,y,dMinDisLine);
    long n=-1;
    GetNearestPoint (x,y,n,m_nPointPointed,dMinDisPoint);

    if(dMinDisPoint<dMinDisLine)
    {
        m_nLinePointed=pDoc->GetLineOrder (m_nPointPointed);
        dMinDistance=dMinDisPoint;
    }
    else
    {
        GetNearestPoint (x,y,m_nLinePointed,m_nPointPointed,dMinDisPoint);
        dMinDistance=dMinDisLine;
    }

    if(nPointBak!=m_nPointPointed||nLineBak!=m_nLinePointed)
    {
        return true;
    }

    return false;
}

long CFHPointLocView::GetNearestLine(double x, double y,double &dMinDis)
{
    CFHPointLocDoc *pDoc=(CFHPointLocDoc *)GetDocument();

    ///////////////////////////////////////
    // Get the nearest Line:
    ///////////////////////////////////////
    CLine lineTemp;
    long nLineNear=0;
    int i,nPoint;
    double dDis;
    dMinDis=99999999999999.9;
    CXYZ *pXYZ=NULL;
    double xx,yy;
    for(i=0;i<pDoc->GetLineNumber();i++)
    {
        pXYZ=pDoc->GetLine(i);
        nPoint=pDoc->GetLinePointNumber(i);

        if(!pXYZ||nPoint<2)continue;

        lineTemp.x1=pXYZ[0].east ;
        lineTemp.y1=pXYZ[0].north ;		
        lineTemp.x2=pXYZ[nPoint-1].east;
        lineTemp.y2=pXYZ[nPoint-1].north;

        if(m_bCheckScreenPos)
        {
            LPtoWorld(lineTemp.x1,lineTemp.y1);
            LPtoWorld(lineTemp.x2,lineTemp.y2);
        }

        dDis=lineTemp.PtDistance(x,y,true,xx,yy);
        if(dDis<dMinDis)
        {
            dMinDis=dDis;
            nLineNear=i;
        }
    }

    return nLineNear;
}

///////////////////////////////////////
// Get the nearest Point:
///////////////////////////////////////
CXYZ * CFHPointLocView::GetNearestPoint(double x, 
                                        double y,
                                        long &nLine,
                                        long &nPoint,
                                        double &dMinDis)
{
    CFHPointLocDoc *pDoc=(CFHPointLocDoc *)GetDocument();

    long i,j;
    double dx,dy,dDis;
    dMinDis=99999999999999999999999.9;

    CXYZ *pXYZ=NULL;
    CXYZ *pLine=NULL;

    long nStart,nEnd;
    if(nLine==-1)
    {
        nStart=0;
        nEnd=pDoc->GetLineNumber();
    }
    else
    {
        nStart=nLine;
        nEnd=nLine+1;
    }

    double xx,yy;
    long n;
    for(i=nStart;i<nEnd;i++)
    {
        pXYZ=pDoc->GetLine(i);
        n=pDoc->GetLinePointNumber(i);

        if(!pXYZ)continue;

        for(j=0;j<n;j++)
        {
            xx=pXYZ[j].east;
            yy=pXYZ[j].north;

            if(m_bCheckScreenPos)
            {
                LPtoWorld(xx,yy);
            }

            dx=xx-x;
            dy=yy-y;
            dDis=sqrt(dx*dx+dy*dy);

            if(dDis<dMinDis)
            {
                pLine=pXYZ;
                dMinDis=dDis;
                nLine=i;
                nPoint=j;
            }

        }
    }

    nPoint+=pDoc->m_pLinePos [nLine];

    return pLine;
}
