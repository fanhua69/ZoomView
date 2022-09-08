// RefPtColorView.cpp : implementation file
//

#include "stdafx.h"
#include "MultiRgnBaseView.h"
#include "FHCGlobal.h"
#include "FHApp.h"
#include "FHMainFrm.h"
#include "ColorScale.h"

// CMultiRgnBaseView

IMPLEMENT_DYNCREATE(CMultiRgnBaseView, CFHZoomView)

COLORREF gRGBData[]=
{
    RGB(0,0,0),
        RGB(255,0,0),
        RGB(0,255,0),
        RGB(0,0,255),
        RGB(255,255,0),
        RGB(0,255,255),
        RGB(255,0,255),

        RGB(100,0,0),
        RGB(0,100,0),
        RGB(0,0,100),
        RGB(100,100,0),
        RGB(0,100,100),
        RGB(100,0,100),
        RGB(100,100,100),

        RGB(200,0,0),
        RGB(0,200,0),
        RGB(0,0,200),
        RGB(200,200,0),
        RGB(0,200,200),
        RGB(200,0,200),
        RGB(200,0,200),


        RGB(80,0,0),
        RGB(0,80,0),
        RGB(0,0,80),
        RGB(80,80,0),
        RGB(0,80,80),
        RGB(80,0,80),
        RGB(80,80,80),

        RGB(230,0,0),
        RGB(0,230,0),
        RGB(0,0,230),
        RGB(230,230,0),
        RGB(0,230,230),
        RGB(230,0,230),
        RGB(230,230,230),

        RGB(60,0,0),
        RGB(0,60,0),
        RGB(0,0,60),
        RGB(60,60,0),
        RGB(0,60,60),
        RGB(60,0,60),
        RGB(60,60,60),

        RGB(150,0,0),
        RGB(0,150,0),
        RGB(0,0,150),
        RGB(150,150,0),
        RGB(0,150,150),
        RGB(150,0,150),
        RGB(150,150,150)
};

CMultiRgnBaseView::CMultiRgnBaseView()
{
    m_iCurrentPoint         =0;
    m_iCurrentRgn           =0;
    m_iCurrentSeries        =0;


    m_iSeriesQty            =1;
    m_iRgnQty               =1;
    m_iPointQty             =1;

    m_dMinValue             =   0;
    m_dMaxValue             =   1000;

    m_bBaseBeZero           =   true;

    m_bSeriesSameColor      =   false;

    m_fCurrentColorBrighter =   1.2;

    m_bDrawValue            =   false;

    m_eShowShotSortBy       =   eShowShotByDistance;;
    m_bShowShotSortFixed    =   false;

    m_bRefShotCanBeOnePoint =   true;


    ::GetPar("CMultiRgnBaseView_bSeriesSameColor",m_bSeriesSameColor);
    ::GetPar("CMultiRgnBaseView_fCurrentColorBrighter",m_fCurrentColorBrighter);


    SetCoorStyle(eCS_LeftDown);


    m_eDataType         =eDDT_Data;
    m_eMapType          =eMDT_Curve;

    int n=m_eMapType;
    ::GetPar("CMultiRgnBaseView_eMapType",n);
    m_eMapType=(EMapDrawType)n;

    n=m_eDataType;
    ::GetPar("CMultiRgnBaseView_eDataType",n);
    m_eDataType=(EDataDrawType)n;

    m_dRatioDoorHigh    =1.3;
    m_dRatioDoorLow     =0.7;    
    ::GetPar("CMultiRgnBaseView_dRatioDoorLow",m_dRatioDoorLow);
    ::GetPar("CMultiRgnBaseView_dRatioDoorHigh",m_dRatioDoorHigh);

    m_eShowRefMethod      =eRefAsSeries;
    ::GetPar("CMultiRgnBaseView_iRefShowType",n);m_eShowRefMethod=(EShowRefMethod)n;


    m_menuManager.RemoveMenu(ID_FHEnableCor);

}

CMultiRgnBaseView::~CMultiRgnBaseView()
{
    ::AddPar("CMultiRgnBaseView_eMapType",(int)m_eMapType);
    ::AddPar("CMultiRgnBaseView_eDataType",(int)m_eDataType);
    ::AddPar("CMultiRgnBaseView_dRatioDoorLow",m_dRatioDoorLow);
    ::AddPar("CMultiRgnBaseView_dRatioDoorHigh",m_dRatioDoorHigh);
    ::AddPar("CMultiRgnBaseView_bSeriesSameColor",m_bSeriesSameColor);
    ::AddPar("CMultiRgnBaseView_fCurrentColorBrighter",m_fCurrentColorBrighter);
    int n=(int)m_eShowRefMethod;
    ::AddPar("CMultiRgnBaseView_iRefShowType",n);

    ClearPens();


}

void CMultiRgnBaseView::ClearPens()
{
    int i=0;
    for(i=0;i<m_vecPenSeries.size();i++)
    {
        delete m_vecPenSeries.at(i);
    }
    for(i=0;i<m_vecBrushSeries.size();i++)
    {
        delete m_vecBrushSeries.at(i);
    }
    for(i=0;i<m_vecPenSeriesCurrent.size();i++)
    {
        delete m_vecPenSeriesCurrent.at(i);
    }
    for(i=0;i<m_vecBrushSeriesCurrent.size();i++)
    {
        delete m_vecBrushSeriesCurrent.at(i);
    }

    m_vecPenSeries.clear();
    m_vecBrushSeries.clear();
    m_vecPenSeriesCurrent.clear();
    m_vecBrushSeriesCurrent.clear();
}

void CMultiRgnBaseView::CreatePens()
{
    ClearPens();
    int i=0;
    COLORREF nColor,nColorCurrent;
    CPen *pPen=NULL;
    CBrush *pBrush=NULL;
    for(i=0;i<m_iSeriesQty;i++)
    {
        nColor=GetSeriesColor(i);

        pPen=new CPen;
        pPen->CreatePen(PS_SOLID,0,nColor);
        m_vecPenSeries.push_back(pPen);

        pBrush=new CBrush;
        pBrush->CreateSolidBrush(nColor);
        m_vecBrushSeries.push_back(pBrush);

        nColorCurrent=GetSeriesCurrentColor(i,nColor); 

        pPen=new CPen;
        pPen->CreatePen(PS_SOLID,0,nColorCurrent);
        m_vecPenSeriesCurrent.push_back(pPen);

        pBrush=new CBrush;
        pBrush->CreateSolidBrush(nColorCurrent);
        m_vecBrushSeriesCurrent.push_back(pBrush);
    }
}

BEGIN_MESSAGE_MAP(CMultiRgnBaseView, CFHZoomView)
END_MESSAGE_MAP()


#ifdef _DEBUG
void CMultiRgnBaseView::AssertValid() const
{
    CFHZoomView::AssertValid();
}

void CMultiRgnBaseView::Dump(CDumpContext& dc) const
{
    CFHZoomView::Dump(dc);
}
#endif //_DEBUG

double CMultiRgnBaseView::GetRgnMinValue(int iRgn)
{
    if(iRgn<0||iRgn>=m_vecMinRgnValue.size())
    {
        return 100;
    }
    return m_vecMinRgnValue.at(iRgn);
}

double CMultiRgnBaseView::GetRgnMaxValue(int iRgn)
{
    if(iRgn<0||iRgn>=m_vecMaxRgnValue.size())
    {
        return 100;
    }
    return m_vecMaxRgnValue.at(iRgn);
}

void CMultiRgnBaseView::DrawCoor(CDC *pDC, int iRgnOrder)
{
    /////////////////////////////////////////
    // The coordinate:
    /////////////////////////////////////////
    double  dMaxHeight      =m_dMaxValue-m_dMinValue;
    float   fRgnDrawRatio  = GetRgnValueRatio(iRgnOrder);

    double  dLeft,dRight;
    GetCoorRange(dLeft,dRight);

    CFHZoomView::DrawCoorByHorValues(pDC,dLeft,dRight,m_dMinValue,m_dMinValue+dMaxHeight*0.7,1,fRgnDrawRatio);

    /////////////////////////////////////////
    // The region's name:
    /////////////////////////////////////////
    double x=dRight;
    double y=GetRgnMinValue(iRgnOrder);
    CFHZoomView::LPtoWorld(x,y);
    x+=30;
    y-=20;
    pDC->TextOut(x,y,GetRgnName(iRgnOrder));
}

void CMultiRgnBaseView::OnDraw(CDC* pDC)
{
    EnableCoor(false);

    CFont font;
    if(pDC->IsPrinting ())
    {
        LOGFONT fontPrint=m_logfontCoor;
        fontPrint.lfHeight*=5;
        font.CreateFontIndirect(&fontPrint);
    }
    else
    {
        font.CreateFontIndirect(&m_logfontCoor);
    }
    CFont *pOldFont=pDC->SelectObject(&font);


    int i;
    double dHeight=m_dMaxValue-m_dMinValue;
    StoreCoorOri();
    if(m_eMapType==eMDT_Curve)
    {
        for(i=0;i<m_iRgnQty;i++)
        {
            DrawCoor(pDC,i);
            DrawValueCurve(pDC,i);
            DrawRatioLimitLine(pDC,i);
            MoveCoor(0,dHeight);
        }
    }
    else if(m_eMapType==eMDT_Rectangle)
    {
        for(i=0;i<m_iRgnQty;i++)
        {
            DrawCoor(pDC,i);
            DrawValueRect(pDC,i);
            DrawRatioLimitLine(pDC,i);
            MoveCoor(0,dHeight);
        }
    }
    else
    {
        for(i=0;i<m_iRgnQty;i++)
        {
            DrawCoor(pDC,i);
            DrawRgn(pDC,i);
            DrawRatioLimitLine(pDC,i);
            MoveCoor(0,dHeight);
        }
    }
    RestoreCoorOri();
    DrawSeriesName(pDC,i);

    //
    pDC->SelectObject(pOldFont);

}

void  CMultiRgnBaseView::DrawRatioLimitLine (CDC *pDC, int iRgnOrder)
{
    return;
}

void CMultiRgnBaseView::DrawRgn(CDC *pDC, int iRgnOrder)
{
    return;
}

void CMultiRgnBaseView::OnInitialUpdate()
{
    CFHZoomView::OnInitialUpdate();

    m_penCommon.CreatePen(PS_SOLID,1,m_colorMapContent);
    m_penCurrent.CreatePen(PS_SOLID,1,m_colorMapContentSpecial);
    m_brushCommon.CreateSolidBrush(m_colorMapContent);
    m_brushCurrent.CreateSolidBrush(m_colorMapContentSpecial);
}

float CMultiRgnBaseView::GetValueRatio(double dValue, double dMaxValue)
{
    if(!m_bApplyValueRatio)
    {
        return 1.0;
    }

    float fValueShowRatio=1.0;

    if(dValue<dMaxValue/10000)
    {
        fValueShowRatio=10000;
    }

    else if(dValue<dMaxValue/5000)
    {
        fValueShowRatio=5000;
    }

    else if(dValue<dMaxValue/2000)
    {
        fValueShowRatio=2000;
    }

    else if(dValue<dMaxValue/1000)
    {
        fValueShowRatio=1000;
    }

    else if(dValue<dMaxValue/500)
    {
        fValueShowRatio=500;
    }

    else if(dValue<dMaxValue/200)
    {
        fValueShowRatio=200;
    } 

    else if(dValue<dMaxValue/100)
    {
        fValueShowRatio=100;
    }

    else if(dValue<dMaxValue/50)
    {
        fValueShowRatio=50;
    }        

    else if(dValue<dMaxValue/30)
    {
        fValueShowRatio=30;
    }

    else if(dValue<dMaxValue/10)
    {
        fValueShowRatio=10;
    }
    else if(dValue<dMaxValue/5)
    {
        fValueShowRatio=5;
    }
    else
    {
        fValueShowRatio=1.0;
    }
    return fValueShowRatio;
}



int CMultiRgnBaseView::GetPointQty()
{
    return 0;
}


void CMultiRgnBaseView::CalcMostValue()
{
    m_iPointQty     =GetPointQty();
    m_iRgnQty       =GetRgnQty();
    m_iSeriesQty    =GetSeriesQty();

    /////////////////////////////////////////////////////////////
    //  Calculate biggest value and ratio region by region:
    /////////////////////////////////////////////////////////////
    m_vecMaxRgnValue.clear();
    long iRgn,iPoint;
    double dValue;
    double dBigValue=9999999999999.9;
    double dRgnMaxValue,dRgnMinValue;
    m_dMaxValue=-dBigValue;
    m_dMinValue=dBigValue;

    int iSeries;

    for(iRgn=0;iRgn<m_iRgnQty;iRgn++)
    {
        dRgnMaxValue=-dBigValue;
        dRgnMinValue=dBigValue;
        for(iPoint=0;iPoint<m_iPointQty;iPoint++)
        {
            for(iSeries=0;iSeries<m_iSeriesQty;iSeries++)
            {
                dValue=GetValue(iRgn, iPoint,iSeries);
                if(fabs(dValue+1)<0.000000001)
                {
                    continue;
                }

                if(dValue>dRgnMaxValue)
                {
                    dRgnMaxValue=dValue;
                }

                if(dValue<dRgnMinValue)
                {
                    dRgnMinValue=dValue;
                }
            }
        }

        if(m_bBaseBeZero)
        {
            dRgnMinValue=0.0;
        }

        if(dRgnMaxValue>m_dMaxValue)
        {
            m_dMaxValue=dRgnMaxValue;
        }

        if(dRgnMinValue<m_dMinValue)
        {
            m_dMinValue=dRgnMinValue;
        }

        if(dRgnMinValue<=dRgnMaxValue)
        {
            m_vecMinRgnValue.push_back(dRgnMinValue);
            m_vecMaxRgnValue.push_back(dRgnMaxValue);
        }
        else
        {
            m_vecMinRgnValue.push_back(0);
            m_vecMaxRgnValue.push_back(100);
        }
    }

    if(m_dMinValue>m_dMaxValue)
    {
        m_dMinValue=0;
        m_dMaxValue=100;
    }

    m_dMaxValue=(m_dMaxValue-m_dMinValue)*1.5+m_dMinValue;

    //////////////////////////////////////////////////////////////
    // Save the value ratio to the maximum value of the region:
    //////////////////////////////////////////////////////////////
    m_vecRgnValueRatio.clear();
    float fRgnValueRatio;
    for(iRgn=0;iRgn<m_iRgnQty;iRgn++)
    {
        fRgnValueRatio=CalcRgnValueRatio(iRgn);
        m_vecRgnValueRatio.push_back(fRgnValueRatio);
    }

    //
    CreatePens();
}

void CMultiRgnBaseView::LPtoWorld(double &x, double &y, int iRgnOrder)
{
    double dMinRgnValue=GetRgnMinValue(iRgnOrder);
    y-=dMinRgnValue;
    y*=GetRgnValueRatio(iRgnOrder);
    y+=dMinRgnValue;
    CFHZoomView::LPtoWorld(x,y);
}

void CMultiRgnBaseView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
    SetDrawProperty	();
}

void CMultiRgnBaseView::SetDrawProperty()
{
    CalcMostValue();

    //
    double dLeft,dRight;
    GetCoorRange(dLeft,dRight);
    double dWidth=dRight-dLeft;

    double dLow, dHigh;
    GetValueRange(dLow,dHigh);
    if(m_bBaseBeZero)
    {
        dLow=0.0;
    }
    double dHeight=dHigh-dLow;
    dHeight*=m_iRgnQty;
    if(dHeight<0.00001)
    {
        dHeight=0.00001;
    }
    dHeight*=1.5;

    SetDrawRange(dLeft,dLow,dRight,dHeight,1.0/dWidth*2.0,1.0/dHeight*m_iRgnQty);

    ZoomFull();
}

//
bool CMultiRgnBaseView::CheckPointingChange(double x,double y,double &dMinDistance)
{
    bool b1=CheckPointingRgn(x,y);

    double dRgnHeight=m_dMaxValue-m_dMinValue;
    y-=dRgnHeight*m_iCurrentRgn;
    float  fRgnDrawRatio  = GetRgnValueRatio(m_iCurrentRgn);
    y/=fRgnDrawRatio;
    y+=m_dMinValue;

    bool b2=CheckPointingPoint(x,y);
    bool b3=CheckPointingSeries(x,y);

    return b1||b2||b3;
}

bool CMultiRgnBaseView::CheckPointingSeries(double x,double y)
{
    int i=0,n=0;
    int iCurrentSeries=m_iCurrentSeries;
    if(m_eMapType==eMDT_Curve)
    {
        double yThis,dDis,dMinDis=99999999999.9;
        for(i=0;i<m_iSeriesQty;i++)
        {
            yThis=GetValue(m_iCurrentRgn,m_iCurrentPoint,i);
            dDis=fabs(y-yThis);

            if(dDis<dMinDis||i==0)
            {
                n=i;
                dMinDis=dDis;
            }
        }       
        m_iCurrentSeries=n;
    }
    else
    {
        double dWidth=GetMinPointDis(m_iCurrentRgn,0);
        double dSeriesWidth=dWidth/m_iSeriesQty;
        double dSeriesCoor;
        dSeriesCoor=GetCoor(m_iCurrentRgn,m_iCurrentPoint,0);
        dSeriesCoor-=dWidth/2;
        dSeriesCoor+=dSeriesWidth/2;

        x-=dSeriesCoor;        
        m_iCurrentSeries=x/=dSeriesWidth;
    }

    return (iCurrentSeries!=m_iCurrentSeries);
}

// Check Selected Shot:
bool CMultiRgnBaseView::CheckPointingPoint(double x,double y)
{
    int n=0;
    double xThis,dDis,dMinDis=99999999999.9;
    for(int i=0;i<GetPointQty();i++)
    {
        xThis=GetCoor(m_iCurrentRgn,i,0);
        dDis=fabs(x-xThis);

        if(dDis<dMinDis||i==0)
        {
            n=i;
            dMinDis=dDis;
        }
    }

    long nSelectedBak=m_iCurrentPoint;
    m_iCurrentPoint=n;

    if(m_iCurrentPoint!=nSelectedBak)
    {
        return true;
    }

    return false;
}

// Check Selected Region:
bool CMultiRgnBaseView::CheckPointingRgn(double x,double y)
{
    int nRgn;
    float y1,y2;
    int nSelectedRgnBak=m_iCurrentRgn;

    double dRgnHeight=0;
    dRgnHeight=m_dMaxValue-m_dMinValue;

    //
    for(nRgn=0;nRgn<m_iRgnQty;nRgn++)
    {
        y1=nRgn*dRgnHeight;
        y2=y1+dRgnHeight;
        if(y>=y1&&y<=y2)
        {
            m_iCurrentRgn=nRgn;
            break;
        }
    }

    return (nSelectedRgnBak!=m_iCurrentRgn);

}

void CMultiRgnBaseView::GetValueRange(double &dLow, double &dHigh)
{
    dLow=9999999999999999999999999.9;
    dHigh=-dLow;

    double dValue;
    int i,j,k;
    for(i=0;i<m_iSeriesQty;i++)
    {
        for(j=0;j<m_iRgnQty;j++)
        {
            for(k=0;k<m_iPointQty;k++)
            {
                dValue=GetValue(j,k,i);
                if(dValue<dLow)
                {
                    dLow=dValue;
                }
                if(dValue>dHigh)
                {
                    dHigh=dValue;
                }
            }
        }
    }

    if(dHigh<dLow)
    {
        dLow=0;
        dHigh=100;
    }

    if((dHigh-dLow)<0.00001)
    {
        dHigh=dLow+0.00001;
    }

    return ;
}

void CMultiRgnBaseView::GetCoorRange(double &dLeft, double &dRight)
{
    dLeft=99999999999.9;
    dRight=-999999999999.9;

    double dCoor;
    int i,j,k;
    for(i=0;i<m_iSeriesQty;i++)
    {
        for(j=0;j<m_iRgnQty;j++)
        {
            for(k=0;k<m_iPointQty;k++)
            {
                dCoor=GetCoor(j,k,i);
                if(dCoor<dLeft)
                {
                    dLeft=dCoor;
                }
                if(dCoor>dRight)
                {
                    dRight=dCoor;
                }
            }
        }
    }

    if(dRight<dLeft)
    {
        dLeft=0;
        dRight=100;
    }

    if((dRight-dLeft)<0.00001)
    {
        dRight=dLeft+0.00001;
    }

    //
    double dMaxDis=0,dDis;
    for(i=0;i<m_iRgnQty;i++)
    {
        for(j=0;j<m_iSeriesQty;j++)
        {
            dDis=GetMinPointDis(i, j);
            if(dDis>dMaxDis)
            {
                dMaxDis=dDis;
            }
        }
    }

    dLeft-=dMaxDis;
    dRight+=dMaxDis;

    return ;
}

float CMultiRgnBaseView::GetRgnValueRatio(int iRgnOrder)
{
    if(iRgnOrder<0||iRgnOrder>=m_iRgnQty)
    {
        return 1.0;
    }
    return m_vecRgnValueRatio.at(iRgnOrder);
}

float CMultiRgnBaseView::CalcRgnValueRatio(int iRgnOrder)
{
    double  dMaxHeight      =m_dMaxValue-m_dMinValue;

    dMaxHeight*=0.7;

    double  dRgnMaxValue    =GetRgnMaxValue(iRgnOrder);
    double  dRgnMinValue    =GetRgnMinValue(iRgnOrder);
    double  dRgnHeight      =dRgnMaxValue-dRgnMinValue;

    float   fRgnDrawRatio  =GetValueRatio(dRgnHeight,dMaxHeight);

    return fRgnDrawRatio;
}

double CMultiRgnBaseView::GetValue(int iRgnOrder, int iPointOrder, int iSeries)
{
    return 0;
}

double  CMultiRgnBaseView::GetCoor  (int iRgnOrder, int iPointOrder, int iSeries)
{
    return iPointOrder;
}

void CMultiRgnBaseView::GetCoorNote(CStringList &listNotes,
                                    int         iRgnOrder, 
                                    int         iPointOrder, 
                                    int         iSeries)
{
    listNotes.AddHead(vtos(iPointOrder));
}


void CMultiRgnBaseView::DrawValueCurve(CDC *pDC, int iRgnOrder)
{
    ////////////////////////////////////////////
    //  Draw the lines:
    ////////////////////////////////////////////
    CPen * pOldPen=pDC->SelectObject (&m_penCommon);
    CBrush *pOldBrush=pDC->SelectObject (&m_brushCommon);

    // The curve:
    double x1,y1,x2,y2;
    int iSeries=0,iPoint;
    for(iSeries=0;iSeries<m_iSeriesQty;iSeries++)
    {
        y1=GetValue (iRgnOrder,0,iSeries);
        x1=GetCoor  (iRgnOrder,0,iSeries);

        if(!m_bSeriesSameColor)
        {
            pDC->SelectObject(m_vecPenSeries.at(iSeries));
            pDC->SelectObject(m_vecBrushSeries.at(iSeries));
        }


        LPtoWorld(x1,y1,iRgnOrder);
        pDC->MoveTo(x1,y1);

        for(iPoint=1;iPoint<m_iPointQty ;iPoint++)
        {
            y2=GetValue (iRgnOrder,iPoint,iSeries);
            x2=GetCoor  (iRgnOrder,iPoint,iSeries);

            LPtoWorld(x2,y2,iRgnOrder);
            pDC->LineTo(x2,y2);
        }
    }

    // The circle:
    for(iSeries=0;iSeries<m_iSeriesQty;iSeries++)
    {
        for(iPoint=0;iPoint<m_iPointQty ;iPoint++)
        {
            DrawPoint(pDC,iSeries,iRgnOrder,iPoint);
        }
    }

    /////////////////////////////
    //
    pDC->SelectObject (pOldBrush);
    pDC->SelectObject (pOldPen);

    return;	
}

double CMultiRgnBaseView::GetMinPointDis(int iRgnOrder,int iSeries)
{
    if(m_iPointQty<2)
    {
        return 1;
    }

    double dDis,dMinDis=999999999.9;
    double dPos1,dPos2=GetCoor(iRgnOrder,0,iSeries);
    for(int i=1;i<m_iPointQty;i++)
    {
        dPos1=dPos2;
        dPos2=GetCoor(iRgnOrder,i,iSeries);
        dDis=fabs(dPos2-dPos1);
        if(dDis>0.00000001&&dDis<dMinDis)
        {
            dMinDis=dDis;
        }
    }

    return dMinDis;
}

void CMultiRgnBaseView::DrawValueRect(CDC *pDC, int iRgnOrder)
{
    CPen * pOldPen=pDC->SelectObject (&m_penCommon);
    CBrush *pOldBrush=pDC->SelectObject (&m_brushCommon);

    CRect rectChar;

    // The curve:
    double x1,y1,x2,y2;
    CString sValue;
    int iSeries=0,iPoint;
    double dWidth=GetMinPointDis(iRgnOrder,0);
    double dSeriesWidth=dWidth/m_iSeriesQty;
    double dStartCoor,dSeriesCoor;
    for(iPoint=0;iPoint<m_iPointQty ;iPoint++)
    {
        dStartCoor=GetCoor  (iRgnOrder,iPoint,0);
        dStartCoor-=dWidth/2;
        for(iSeries=0;iSeries<m_iSeriesQty;iSeries++)
        {
            dSeriesCoor=dStartCoor+iSeries*dSeriesWidth;
            if(!IsCurrentPoint(iSeries,iRgnOrder,iPoint))
            {
                if(m_bSeriesSameColor)
                {
                    pDC->SelectObject (&m_penCommon);
                    pDC->SelectObject (&m_brushCommon);
                }
                else
                {
                    pDC->SelectObject (m_vecPenSeries.at(iSeries));
                    pDC->SelectObject (m_vecBrushSeries.at(iSeries));
                }
            }
            else
            {
                if(m_bSeriesSameColor)
                {
                    pDC->SelectObject (&m_penCurrent);
                    pDC->SelectObject (&m_brushCurrent);
                }
                else
                {
                    pDC->SelectObject (m_vecPenSeriesCurrent.at(iSeries));
                    pDC->SelectObject (m_vecBrushSeriesCurrent.at(iSeries));
                }
            }
            y1  = 0;
            y2  = GetValue (iRgnOrder,iPoint,iSeries);
            sValue=vtos(y2);

            x1  = dSeriesCoor;
            x2  = x1+dSeriesWidth;

            LPtoWorld(x1,y1,iRgnOrder);
            LPtoWorld(x2,y2,iRgnOrder);
            x1+=1;
            if(x2-x1<1)
            {
                x2=x1+1;
            }

            pDC->Rectangle(x1,y1,x2,y2);

            if(m_bDrawValue)
            {
                rectChar.SetRect(x1,y2,x2,y2-10);		
                pDC->DrawText(sValue,rectChar,DT_SINGLELINE|DT_BOTTOM|DT_CENTER|DT_NOCLIP);		
            }
        }
    }

    pDC->SelectObject (pOldPen);
    pDC->SelectObject (pOldBrush);

    return;	
}


void CMultiRgnBaseView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
    CFHZoomView::OnContextMenu(pWnd,point);
}

void CMultiRgnBaseView::PreSettingDlg()
{
    CFHZoomView::PreSettingDlg();

    //
    CDlgMultiRgnViewProperty *pDlg2=new CDlgMultiRgnViewProperty;
    pDlg2->SetMapType               (m_eMapType);
    pDlg2->SetDataType              (m_eDataType);
    pDlg2->SetHighDoor              (m_dRatioDoorHigh);
    pDlg2->SetLowDoor               (m_dRatioDoorLow);
    pDlg2->SetRefShowType           (m_eShowRefMethod);
    pDlg2->SetShotShowOrder         (m_eShowShotSortBy);
    pDlg2->SetShotShowOrderFixed    (m_bShowShotSortFixed);
    pDlg2->SetRefShotCanBeOnePoint  (m_bRefShotCanBeOnePoint);
    pDlg2->m_fCurrentColorBrighter  = m_fCurrentColorBrighter*100;

    //
    std::vector<CSeriesColor> vecSeriesColor;
    int iSeries=0;
    for(iSeries=0;iSeries<m_iSeriesQty;iSeries++)
    {
        COLORREF iColor=GetSeriesColor(iSeries);
        CString sName=GetSeriesName(iSeries);
        CSeriesColor sc(sName,iColor);
        vecSeriesColor.push_back(sc);
    }
    pDlg2->SetSeriesColor(vecSeriesColor);

    //
    if(m_bSeriesSameColor)
    {
        pDlg2->m_iSeriesSameColor=0;
    }
    else
    {
        pDlg2->m_iSeriesSameColor=1;
    }

    //
    AddPropertyPage(pDlg2);
}

void CMultiRgnBaseView::PostSettingDlg()
{
    CFHZoomView::PostSettingDlg();

    m_penCommon.Detach();
    m_penCurrent.Detach();
    m_brushCommon.Detach();
    m_brushCurrent.Detach();

    m_penCommon.CreatePen(PS_SOLID,1,m_colorMapContent);
    m_penCurrent.CreatePen(PS_SOLID,1,m_colorMapContentSpecial);
    m_brushCommon.CreateSolidBrush(m_colorMapContent);
    m_brushCurrent.CreateSolidBrush(m_colorMapContentSpecial);

    //

    CDlgMultiRgnViewProperty * pDlg2= GetPropertyPage(CDlgMultiRgnViewProperty);
    if(pDlg2)
    {
        double          dDoorHigh               =m_dRatioDoorHigh;
        double          dDoorLow                =m_dRatioDoorLow;
        EDataDrawType   eDataType               =m_eDataType;
        EMapDrawType    eMapType                =m_eMapType;
        float           fCurrentColorBrighter   =m_fCurrentColorBrighter;
        EShowRefMethod  eShowRefMethod          =m_eShowRefMethod;

        m_dRatioDoorHigh    =pDlg2->GetHighDoor();
        m_dRatioDoorLow     =pDlg2->GetLowDoor();
        m_eDataType         =pDlg2->GetDataType();
        m_eMapType          =pDlg2->GetMapType();
        m_eShowRefMethod    =pDlg2->GetRefShowType();
        m_eShowShotSortBy   =pDlg2->GetShotShowOrder();

        //
        std::vector<CSeriesColor> vecSeriesColor=pDlg2->GetSeriesColor();
        int iSeries=0;
        for(iSeries=0;iSeries<vecSeriesColor.size();iSeries++)
        {
            CSeriesColor &sc=vecSeriesColor.at(iSeries);
            SetSeriesColor(sc.m_sSeriesName,sc.m_nColor);
        }

        //
        if(pDlg2->m_iSeriesSameColor==0)
        {
            m_bSeriesSameColor=true;
        }
        else
        {
            m_bSeriesSameColor=false;
        }
        m_fCurrentColorBrighter=pDlg2->m_fCurrentColorBrighter/100.0;


        //
        if(m_eDataType!=eDataType||m_eShowRefMethod!=eShowRefMethod)
        {
            SetDrawProperty();
        }

        //
        CreatePens();
        SetInvalidate();
    }
}

CString CMultiRgnBaseView::GetRgnName(int iRgnOrder)
{
    return CString("时窗 "+vtos(iRgnOrder));
}

int  CMultiRgnBaseView::GetRgnQty()
{
    return 1;
}

CPen * CMultiRgnBaseView::GetCommonPen()
{
    return &m_penCommon;
}

CPen * CMultiRgnBaseView::GetCurrentPen()
{
    return &m_penCurrent;
}

CBrush * CMultiRgnBaseView::GetCommonBrush()
{
    return &m_brushCommon;
}

CBrush * CMultiRgnBaseView::GetCurrentBrush()
{
    return &m_brushCurrent;
}

bool CMultiRgnBaseView::IsCurrentPoint(int iSeries, 
                                       int iRgnOrder,
                                       int iPointOrder)
{
    return (    iSeries         ==  m_iCurrentSeries
                &&  iPointOrder ==  m_iCurrentPoint
                &&  iRgnOrder   ==  m_iCurrentRgn);
}

void CMultiRgnBaseView::DrawPoint(CDC *pDC, 
                                  int iSeries,
                                  int iRgnOrder,
                                  int iPoint)
{
    static int iPointSize=0;
    EPointDrawPattern ePattern;

    if(     iSeries     ==m_iCurrentSeries
        &&  iPoint      ==m_iCurrentPoint
        &&  iRgnOrder   ==m_iCurrentRgn)
    {
        if(!m_bSeriesSameColor)
        {
            pDC->SelectObject(m_vecPenSeriesCurrent.at(iSeries));
            pDC->SelectObject(m_vecBrushSeriesCurrent.at(iSeries));
        }
        else
        {
            pDC->SelectObject (&m_penCurrent);
            pDC->SelectObject (&m_brushCurrent);
        }

        iPointSize=m_iContentCurrSize;
        ePattern=m_ePointPatternCurrent;
    }
    else
    {
        if(!m_bSeriesSameColor)
        {
            pDC->SelectObject(m_vecPenSeries.at(iSeries));
            pDC->SelectObject(m_vecBrushSeries.at(iSeries));
        }
        else
        {
            pDC->SelectObject (&m_penCommon);
            pDC->SelectObject (&m_brushCommon);
        }

        iPointSize=m_iContentSize;
        ePattern=m_ePointPatternCommon;
    }

    static double x,y;
    y=GetValue(iRgnOrder,iPoint,iSeries);
    x=GetCoor(iRgnOrder,iPoint,iSeries);

    LPtoWorld(x,y,iRgnOrder);

    DrawPointOfType(pDC,x,y,ePattern,iPointSize);
}

CString CMultiRgnBaseView::GetSeriesName(int iOrder)
{
    return "Series_"+vtos(iOrder);
}

int   CMultiRgnBaseView::GetSeriesQty()
{
    return 0;
}

//
COLORREF CMultiRgnBaseView::GetSeriesColor(int iSeriesOrder)
{
    //
    CString sName=GetSeriesName(iSeriesOrder);
    UINT nColor=-1;
    ::GetPar("CMultiRgnBaseView_SeriesColor_"+sName,nColor);
    if(nColor!=-1)
    {
        return nColor;
    }

    //
    int nColorQty=sizeof(gRGBData)/sizeof(COLORREF);
    int iColorOrder=iSeriesOrder%nColorQty;

    nColor=gRGBData[iColorOrder];

    return nColor;
}

void CMultiRgnBaseView::SetSeriesColor(CString sName, COLORREF nColor)
{
    unsigned int n=nColor;
    ::AddPar("CMultiRgnBaseView_SeriesColor_"+sName,n);
}


void CMultiRgnBaseView::DrawSeriesName(CDC *pDC, int iRgn)
{
    if(m_iSeriesQty<1)
    {
        return ;
    }

    /////////////////////////////
    // 绘制比较炮与参考炮的比值
    long n=0;
    double  dDoorLowValue=0, dDoorHighValue=0;
    int nR=2;
    long nShotColor=0;

    //////////////////////////////////////////////////
    // Get the position of the lines and texts for file number:
    CString s="1234567890";
    CSize sizeText=pDC->GetTextExtent (s);

    CMyRect rectCoor=GetCoorRect();
    CFHZoomView::LPtoWorld(rectCoor.m_dRight ,rectCoor.m_dLow );
    LPtoWorldY(rectCoor.m_dHigh);

    double x1,y1,x2,y2;
    this->GetVisibleArea(x1,y1,x2,y2);
    LPtoWorldY(y1);

    long nYStart=rectCoor.m_dLow ;
    if(rectCoor.m_dHigh>nYStart)
    {
        nYStart=rectCoor.m_dHigh;
    }

    long nXStart=rectCoor.m_dRight+100; 
    long nTextStart=nXStart+sizeText.cx/2+2;

    CPen *pOldPen= pDC->SelectObject (m_vecPenSeries[0]);
    for(int i=0;i<m_iSeriesQty;i++)
    {
        pDC->SelectObject (m_vecPenSeries[i]);

        pDC->MoveTo (nXStart,nYStart);
        pDC->LineTo (nTextStart-2,nYStart);

        CString sSeriesName=GetSeriesName(i);
        pDC->TextOut (nTextStart,nYStart-sizeText.cy/2,sSeriesName);
        nYStart-=sizeText.cy;
    }

    pDC->SelectObject (pOldPen);
}

COLORREF CMultiRgnBaseView::GetSeriesCurrentColor(int iSeriesOrder, COLORREF colorSeries)
{
    COLORREF color=CColorDlg::Brighter (colorSeries,m_fCurrentColorBrighter);
    return color;
}
