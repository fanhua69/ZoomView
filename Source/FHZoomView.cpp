// FHZoomViewEx.cpp : implementation file
//

#include "stdafx.h"
#include "zoomview.h"
#include "FHZoomView.h"
#include "MouseAnchorWnd.h"
#include "FontGroup.h"
#include "FHZoomApprDlg.h"
#include "BmpProc.h" 
#include "MemDC.h"
#include "FHZoomViewGlobal.h"
#include "fhzoomview.h"
#include <algorithm>
#include "RectClipper.h"
#include "FHMainFrm.h"
#include "fhapp.h"
#include "DataType.h"
#include "..\include\fhzoomview.h"
#include "DlgAskPage.h"
#include "DlgBackGroundPic.h"

extern "C" const long MACHINE_NUMBER[];


// CFHZoomView

IMPLEMENT_DYNCREATE(CFHZoomView, CView)

BEGIN_MESSAGE_MAP(CFHZoomView, CView)
    //{{AFX_MSG_MAP(CFHZoomView)
    //}}AFX_MSG_MAP
    ON_WM_SIZE()
    ON_WM_HSCROLL()
    ON_WM_VSCROLL()
    ON_WM_MOUSEWHEEL()
    ON_MESSAGE(WM_MBUTTONDOWN, HandleMButtonDown)
    ON_WM_SETFOCUS()
    ON_WM_RBUTTONDOWN()
    ON_WM_TIMER()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_CONTEXTMENU()
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()	

    ON_COMMAND(ID_FHEnableInfoWnd, OnFHEnableInfoWnd)
    ON_COMMAND(ID_FHZoomFull, OnFHZoomFull)
    ON_COMMAND(ID_FHEnableCor, OnFHEnableCor)
    ON_COMMAND(ID_FHZoomInHor, OnFHZoomInHor)
    ON_COMMAND(ID_FHZoomInVer, OnFHZoomInVer)
    ON_COMMAND(ID_FHZoomOutHor, OnFHZoomOutHor)
    ON_COMMAND(ID_FHZoomOutVer, OnFHZoomOutVer)
    ON_COMMAND(ID_FHZoomIn, OnFHZoomIn)
    ON_COMMAND(ID_FHZoomOut, OnFHZoomOut)
    ON_COMMAND(ID_FHZoomErrorMsg, OnFHZoomErrorMsg)
    ON_COMMAND(ID_FHZoomViewLast,OnFHZoomViewLast)
    ON_COMMAND(ID_FHZoomViewNext,OnFHZoomViewNext)
    ON_COMMAND(ID_FHZoomViewExportBitmap, OnFHZoomViewExportBitmap)
    ON_COMMAND(ID_FHZoomInArea, OnFHZoomInArea)
    ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT, OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
    ON_COMMAND(ID_FHSettings, OnFhsettings)

    ON_WM_CREATE()
    ON_WM_KILLFOCUS()

    ON_MESSAGE(WM_WNDCHANGED, OnWndChange) 

    ON_UPDATE_COMMAND_UI(ID_FHZoomViewLast, OnUpdateFhzoomviewlast)
    ON_UPDATE_COMMAND_UI(ID_FHZoomViewNext, OnUpdateFhzoomviewnext)
    ON_UPDATE_COMMAND_UI(ID_FHZoomSearch, OnUpdateFhzoomsearch)
    ON_COMMAND(ID_FHZoomSearch, OnFhzoomsearch)
    ON_UPDATE_COMMAND_UI(ID_FHEnableInfoWnd, OnUpdateFhEnableInfoWnd)
    ON_UPDATE_COMMAND_UI(ID_FHEnableCor, OnUpdateFhEnableCor)
    ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


CFHZoomView::CFHZoomView()
{
    m_bZoomAreaAnchored=false;

    //AFX_ZERO_INIT_OBJECT(CView);
    m_pAnchorWindow = NULL;
    m_nMapMode = ULT_PIXEL;

    m_pointLogOrg.x=0 ;
    m_pointLogOrg.y=0 ;
    m_pointLogOrgBak=m_pointLogOrg;


    m_totalDev.SetSize (14000,14000);
    m_totalLog.SetSize(1000,1000);
    m_nEdgeWidth=50;

    m_pageDev.cx = 100;
    m_pageDev.cy = 100;
    m_lineDev.cx = 10;
    m_lineDev.cy = 10;

    m_eCoorStyle = eCS_Rectangle;

    m_sizeScrollBar.cx =m_sizeScrollBar.cy =32767;

    m_dCooeHor=1.5;      // 水平方向的坐标缩放系数
    m_dCooeVer=1.5;      // 垂直方向的坐标缩放系数

    m_rectUserThickLine.SetRect(0,0,1000,1000);
    m_rectUserThinLine.SetRect(0,0,1000,1000);
    m_sizeUserThickStep.SetSize(1000,1000);
    m_sizeUserThinStep.SetSize(1000,1000);

    /////////////////////////////////////////////
    //     Title     :
    m_bDrawTitle=false;
    m_sDrawTitle="图头";
    memset(&m_logfontTitle, 0, sizeof(LOGFONT));
    m_logfontTitle.lfHeight = 16;
    m_logfontTitle.lfOutPrecision = OUT_TT_PRECIS ;
    m_logfontTitle.lfQuality =PROOF_QUALITY;
    strcpy(m_logfontTitle.lfFaceName,"Arial");
    m_colorTitle=RGB(0,0,0);	

    // Coor:
    m_bDrawCoor=true;
    m_nMidScaleDensity=5;

    memset(&m_logfontCoor, 0, sizeof(LOGFONT));
    m_logfontCoor.lfHeight = 14;
    m_logfontCoor.lfOutPrecision = OUT_TT_PRECIS ;
    m_logfontCoor.lfQuality =PROOF_QUALITY;
    strcpy(m_logfontCoor.lfFaceName,"Arial");

    m_colorFontCoor=RGB(0,0,0);
    m_colorThickCoorHor=RGB(100,100,100);
    m_colorThinCoorHor=RGB(200,200,200); 
    m_colorThickCoorVer=RGB(100,100,100);
    m_colorThinCoorVer=RGB(200,200,200); 

    m_bUseUserCoor=false;

    m_bDevSizeLimit=true;

    // Map Char:
    m_nFont=90;
    memset(&m_logfontMapChar, 0, sizeof(LOGFONT));
    m_logfontMapChar.lfCharSet = DEFAULT_CHARSET;
    m_logfontMapChar.lfHeight = 12;
    m_logfontMapChar.lfOutPrecision = OUT_TT_PRECIS ;
    m_logfontMapChar.lfQuality =PROOF_QUALITY;
    strcpy(m_logfontMapChar.lfFaceName,"Arial");
    m_colorMapChar=RGB(0,0,0);	


    memset(&m_logfontCurrChar, 0, sizeof(LOGFONT));
    m_logfontCurrChar.lfCharSet = DEFAULT_CHARSET;
    m_logfontCurrChar.lfHeight = 16;
    m_logfontCurrChar.lfOutPrecision = OUT_TT_PRECIS ;
    m_logfontCurrChar.lfQuality =PROOF_QUALITY;
    strcpy(m_logfontCurrChar.lfFaceName,"Arial");


    m_fontTextCommon.CreateFontIndirect(&m_logfontMapChar);
    m_fontTextCurrent.CreateFontIndirect(&m_logfontCurrChar);


    // Ordinary lines, for derived class:
    m_colorMapContent=RGB(0,0,0);
    m_colorMapContentSpecial=RGB(255,0,0);

    // Direction:
    m_bUpIncrease=true;

    // The ToolBar:
    m_pZoomViewToolBar=NULL;

    // The bar:
    m_nBarWidth=40;

    m_fontCorVer.CreateFont(14,5,900,0,400,FALSE,FALSE,0,
        ANSI_CHARSET,OUT_TT_PRECIS,
        CLIP_DEFAULT_PRECIS,PROOF_QUALITY    ,
        DEFAULT_PITCH|FF_SWISS,"Arial");

    m_fontCorHor.CreateFont(14,5,0,0,400,FALSE,FALSE,0,
        ANSI_CHARSET,OUT_RASTER_PRECIS,
        CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
        DEFAULT_PITCH|FF_SWISS,"Arial");


    m_pFontTilt=NULL;
    SetTiltFont(NULL,NULL);

    m_nFirstShow=true;

    // Related to showing infomation:
    m_dlgInfo.Create(IDD_InfoDlg);
    m_dlgInfo.ShowWindow(SW_HIDE);
    m_dlgInfo.m_bCanShowInfo=true;
    m_bInfoCanShowBak=false;

    m_bInfoOnShow=false;
    m_timeInterval=1;
    m_timeToShowInfo=3;
    m_timeMouseInSamePlace=0;
    m_pointLastMouse.x=m_pointLastMouse.y=-1;
    m_pointCurrentMouse.x=m_pointCurrentMouse.y=0;

    m_eZoomState=statueIdle;

    // Related with showing the error message:
    m_dlgErrorMsg.Create(IDD_InfoDlg);
    m_dlgErrorMsg.SetForeColor(255,0,0);
    m_dlgErrorMsg.SetBackColor(0,255,0);
    m_dlgErrorMsg.m_bCanShowInfo=false;
    m_dlgErrorMsg.ShowWindow(SW_HIDE);
    m_bErrorCanShowBak=false;

    m_bInsideUpdate=false;
    m_bCenter=false;

    m_bEnableInfoTimer=false;

    //
    m_nCurrentPage=0;
    m_nPageQty=0;

    // 
    m_nEmptyZoomView=0;
    m_nEmptyZoomView2=0;
    m_nEmptyZoomView3=0;

    m_iContentSize=2;
    m_iContentCurrSize=3;

    //
    m_bImgBackKeepRatio=true;
    m_rectBackGroundPic.SetRect(-1,-1,-1,-1);


    //
    m_penZoomRect.CreatePen (PS_DOT,0,RGB(0,0,0));

    m_bMouseAnchored=false;

    m_bDrawBar=true;
    //
    CFHMainFrm *pFrame=(CFHMainFrm*)AfxGetMainWnd();	
    if(pFrame->IsKindOf (RUNTIME_CLASS(CFHMainFrm)))
    {
        SetToolBar(&pFrame->m_FHZoomViewToolBar) ;
    }

    SetPopupMenu();
}

CFHZoomView::~CFHZoomView()
{
    if (m_pAnchorWindow != NULL)
    {
        m_pAnchorWindow->DestroyWindow();
        delete m_pAnchorWindow;
    }
}

// CFHZoomView drawing

void CFHZoomView::OnDraw(CDC* pDC)
{
}


// CFHZoomView diagnostics

#ifdef _DEBUG
void CFHZoomView::AssertValid() const
{
    CView::AssertValid();
}

void CFHZoomView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}
#endif //_DEBUG


void CFHZoomView::SetToolBarButtonState()
{
    if(!m_pZoomViewToolBar)return;

    CToolBarCtrl  &barCtrl=m_pZoomViewToolBar->GetToolBarCtrl();

    barCtrl.PressButton (ID_FHZoomOut,m_eZoomState==statueZoomOut);
    barCtrl.PressButton (ID_FHZoomIn,m_eZoomState==statueZoomIn);	
    barCtrl.PressButton (ID_FHZoomInVer,m_eZoomState==statueZoomInVer);	
    barCtrl.PressButton (ID_FHZoomInHor,m_eZoomState==statueZoomInHor);	
    barCtrl.PressButton (ID_FHZoomOutVer,m_eZoomState==statueZoomOutVer);	
    barCtrl.PressButton (ID_FHZoomOutHor,m_eZoomState==statueZoomOutHor);	

    barCtrl.PressButton (ID_FHZoomInArea,m_eZoomState==statueZoomInArea);	
    barCtrl.PressButton (ID_FHEnableInfoWnd,m_dlgInfo.m_bCanShowInfo  );	

    barCtrl.PressButton (ID_FHEnableCor,m_bDrawCoor);
    barCtrl.PressButton (ID_FHZoomErrorMsg,m_dlgErrorMsg.m_bCanShowInfo);	
}


void CFHZoomView::DrawCoor(CDC *pDC)
{
    DrawCoor(pDC,-1,-1,-1,-1);
}

void CFHZoomView::DrawBackPicture(CDC *pDC)
{
    ////////////////////////////////////////////
    // Show the background picture:
    ////////////////////////////////////////////
    CMyRect rectPic;
    if(fabs(m_rectBackGroundPic.m_dLow+1)<0.00001&&fabs(m_rectBackGroundPic.m_dHigh+1)<0.00001)
    {
        rectPic.SetRect(m_pointLogOrgBak.x,
                        m_pointLogOrgBak.y,
                        m_pointLogOrgBak.x+m_totalLog.cx,
                        m_pointLogOrgBak.y+m_totalLog.cy);
    }
    else
    {
        rectPic=m_rectBackGroundPic;
    }

    double dMapLeft =rectPic.m_dLeft;
    double dMapRight=rectPic.m_dRight;
    double dMapLow  =rectPic.m_dLow;
    double dMapHigh =rectPic.m_dHigh;
    LPtoWorld(dMapLeft,dMapLow);
    LPtoWorld(dMapRight,dMapHigh);	
    if(dMapLeft>dMapRight)std::swap(dMapLeft,dMapRight);
    if(dMapLow>dMapHigh)std::swap(dMapLow,dMapHigh);
    if((dMapRight-dMapLeft)*(dMapHigh-dMapLow)<100000000&&m_sBackGroundFileName!="")
    {
        CFHImage image;
        image.Load(m_sBackGroundFileName);
        image.Show(pDC,CRect(dMapLeft-4,dMapLow-4,dMapRight+2,dMapHigh+4),m_bImgBackKeepRatio);
    }
}

void CFHZoomView::DrawCoor(CDC      *pDC,
                           double   dLeftCoor,
                           double   dRightCoor, 
                           double   dLowCoor, 
                           double   dHighCoor,
                           float    fHorValueRatio,
                           float    fVerValueRatio)
{
    ////////////////////////////////////////////
    // Prepare:
    //////////////////////////////////////////////

    ////////////////////////////////////
    // The font of The Map:
    ////////////////////////////////////
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
    COLORREF nOldColor=pDC->SetTextColor(m_colorFontCoor);		

    ////////////////////////////////////////////
    // Line Steps:
    ////////////////////////////////////////////
    double dXThickValueStep,dYThickValueStep;
    double dXThinValueStep,dYThinValueStep;
    double dEdgeLeft,dEdgeRight,dEdgeTop,dEdgeBottom;
    double dMinX,dMinY,dMaxX,dMaxY;

    CMyRect rectDraw;
    int nWidth,nHeight;


    CString sValue;
    if(m_bUseUserCoor)
    {
        dEdgeLeft=m_rectUserThickLine.m_dLeft;
        dEdgeRight=m_rectUserThickLine.m_dRight;
        dEdgeBottom=m_rectUserThickLine.m_dLow;
        dEdgeTop=m_rectUserThickLine.m_dHigh;

        dXThickValueStep=m_sizeUserThickStep.cx;
        dYThickValueStep=m_sizeUserThickStep.cy;
        dXThinValueStep=m_sizeUserThinStep.cx;
        dYThinValueStep=m_sizeUserThinStep.cy;

        rectDraw.m_dLeft =dEdgeLeft;
        rectDraw.m_dRight=dEdgeRight;
        rectDraw.m_dLow=dEdgeBottom;
        rectDraw.m_dHigh=dEdgeTop;

        dMinX=dEdgeLeft;
        dMinY=dEdgeBottom;
        dMaxX=dEdgeRight;
        dMaxY=dEdgeTop;

    }


    else
    {
        // Let this class decide the drawing range :
        if(fabs(dLeftCoor+1)<0.0001&&fabs(dRightCoor+1)<0.0001) 
        {
            if(!pDC->IsPrinting ())
            {
                GetClientLogicalRange(dMinX,dMinY,dMaxX,dMaxY);	

                CRect rectClient;
                GetClientRect(&rectClient);
                nWidth=rectClient.Width ();
                nHeight=rectClient.Height();
                if(nWidth==0||nHeight==0)return;
            }
            else
            {
                dMinX=m_pointLogOrg.x;
                dMinY=m_pointLogOrg.y;
                dMaxX=dMinX+m_totalLog.cx;
                dMaxY=dMinY+m_totalLog.cy;

                double x1=dMinX,y1=dMinY,x2=dMaxX,y2=dMaxY;
                LPtoWorld(x1,y1);
                LPtoWorld(x2,y2);

                nWidth=x2-x1;
                nHeight=fabs(y2-y1);
            }
        }

        // Let this user defined parameters decide the drawing range :
        else  
        {
            dMinX=dLeftCoor;
            dMaxX=dRightCoor;
            dMinY=dLowCoor;
            dMaxY=dHighCoor;

            double x1=dMinX,y1=dMinY,x2=dMaxX,y2=dMaxY;
            LPtoWorld(x1,y1);
            LPtoWorld(x2,y2);

            nWidth=x2-x1;
            nHeight=fabs(y2-y1);

        }

        CFontGroup font;

        sValue.Format("%1.3lf",(dMinX+dMaxX)/2);
        long  nValueWidth=(pDC->GetTextExtent (sValue)).cx;					
        font.Set(dMinX,dMaxX,nWidth,nValueWidth);
        font.CalcFontGroup (dEdgeLeft,dEdgeRight,dXThickValueStep);
        dXThinValueStep=dXThickValueStep/m_nMidScaleDensity;

        sValue.Format("%1.3lf",(dMinY+dMaxY)/2);
        nValueWidth=(pDC->GetTextExtent (sValue)).cx;	
        font.Set(dMinY,dMaxY,nHeight,nValueWidth);
        font.CalcFontGroup (dEdgeBottom,dEdgeTop,dYThickValueStep);
        dYThinValueStep=dYThickValueStep/m_nMidScaleDensity;

        rectDraw.m_dLeft  = m_pointLogOrgBak.x;
        rectDraw.m_dLow   = m_pointLogOrgBak.y;
        rectDraw.m_dRight = rectDraw.m_dLeft  + m_totalLog.cx ;
        rectDraw.m_dHigh  = rectDraw.m_dLow   + m_totalLog.cy ;

    }


    CRectClipper clipper;
    clipper.SetRect (CMyRect(dMinX,dMinY,dMaxX,dMaxY));

    clipper.SetPolygon (rectDraw);

    CMyRect rectDrawbak(rectDraw);
    rectDraw=clipper.GetResultRect ();
    if(rectDraw==CMyRect(0,0,0,0))
    {
        return;
    }
    rectDraw.Normalize ();

    //////////////////////////////////////////////
    //  The Pens:
    //////////////////////////////////////////////
    CPen penThinCoorHor,penThickCoorHor,penThinCoorVer,penThickCoorVer,penFrame;

    long nThinLineWidth;
    long nThickLineWidth;
    long nFrameLineWidth;

    if(pDC->IsPrinting ())
    {
        nThinLineWidth=1;
        nThickLineWidth=10;
        nFrameLineWidth=16;
    }
    else
    {
        nThinLineWidth=1;
        nThickLineWidth=2;
        nFrameLineWidth=4;
    }

    ////////////////////////////////////////////
    // Prepare pens:
    ////////////////////////////////////////////
    penThinCoorHor.CreatePen (PS_SOLID,nThinLineWidth,m_colorThinCoorHor);
    penThickCoorHor.CreatePen (PS_SOLID,nThickLineWidth,m_colorThickCoorHor);
    penThinCoorVer.CreatePen (PS_SOLID,nThinLineWidth,m_colorThinCoorVer);
    penThickCoorVer.CreatePen (PS_SOLID,nThickLineWidth,m_colorThickCoorVer);
    penFrame.CreatePen (PS_SOLID,nFrameLineWidth,m_colorThickCoorHor);

    CPen *pOldPen=pDC->SelectObject (&penFrame);

    ////////////////////////////////////////////
    // Preparation of the Coordinates:
    ////////////////////////////////////////////
    double i,x,y;	
    CSize sizeValue;
    CRect rectChar;

    double x11,y11,x22,y22;	
    x11=rectDraw.m_dLeft;
    x22=rectDraw.m_dRight;
    y11=rectDraw.m_dLow;
    y22=rectDraw.m_dHigh;

    LPtoWorld(x11,y11);
    LPtoWorld(x22,y22);	
    if(x11>x22)std::swap(x11,x22);
    if(y11>y22)std::swap(y11,y22);

    ////////////////////////////////////////////////
    // Draw Left-Right Thin Lines:
    ////////////////////////////////////////////////
    pDC->SelectObject(&penThinCoorHor);

    if(dYThinValueStep<0.00000000001)
    {
        dYThinValueStep=0.00000000001;
    }	
    for(i=dEdgeBottom;i<=dEdgeTop;i+=dYThinValueStep)
    {
        if(i<m_pointLogOrg.y||i>m_pointLogOrg.y+m_totalLog.cy)continue;
        if(i<rectDraw.m_dLow||i>rectDraw.m_dHigh)continue;

        Line(pDC,rectDraw.m_dLeft,i,rectDraw.m_dRight,i);
    }

    ////////////////////////////////////////////////
    // Draw Left-Right Thick Lines:
    ////////////////////////////////////////////////
    pDC->SelectObject(&penThickCoorHor);

    double dRight=x22+nFrameLineWidth*3;
    double dBottom=y22+nFrameLineWidth*3;

    if(dYThickValueStep<0.00000000001)
    {
        dYThickValueStep=0.00000000001;
    }

    double dRealValue;
    for(i=dEdgeBottom;i<=dEdgeTop;i+=dYThickValueStep)
    {
        // Draw the coordinate line:
        if(i<rectDraw.m_dLow||i>rectDraw.m_dHigh)continue;
        Line(pDC,rectDraw.m_dLeft,i,rectDraw.m_dRight,i);

        // Write character:
        dRealValue=m_rectCoor.m_dLow+(i-m_rectCoor.m_dLow)/fVerValueRatio;

        sValue=GetValueString(dRealValue,dYThickValueStep/fVerValueRatio);
        sizeValue=pDC->GetTextExtent (sValue);
        y=i;
        LPtoWorldY(y);

        rectChar.SetRect(dRight, y-sizeValue.cy*0.5, dRight+sizeValue.cx, y+sizeValue.cy);
        pDC->DrawText(sValue,rectChar,DT_SINGLELINE|DT_VCENTER|DT_CENTER|DT_NOCLIP);		
    }

    //////////////////////////////////////////////
    // The Up-Down Line Thin Lines:
    //////////////////////////////////////////////
    pDC->SelectObject(&penThinCoorVer);
    if(dXThinValueStep<0.00000000001)
    {
        dXThinValueStep=0.00000000001;
    }
    for(i=dEdgeLeft;i<=dEdgeRight;i+=dXThinValueStep)
    {
        if(i<rectDraw.m_dLeft||i>rectDraw.m_dRight)continue;
        Line(pDC,i,rectDraw.m_dLow,i,rectDraw.m_dHigh);
    }

    ////////////////////////////////////////////////
    // Draw Up-Down Thick Lines:
    ////////////////////////////////////////////////
    pDC->SelectObject(&penThickCoorVer);
    if(dXThickValueStep<0.00000000001)
    {
        dXThickValueStep=0.00000000001;
    }
    for(i=dEdgeLeft;i<=dEdgeRight;i+=dXThickValueStep)
    {
        if(i<rectDraw.m_dLeft||i>rectDraw.m_dRight)continue;
        Line(pDC,i,rectDraw.m_dLow,i,rectDraw.m_dHigh);

        // Write character:
        sValue=GetValueString(i/fHorValueRatio,dXThickValueStep/fHorValueRatio);
        sizeValue=pDC->GetTextExtent (sValue);

        x=i;
        LPtoWorldX(x);
        rectChar.SetRect(x-sizeValue.cx ,dBottom,x+sizeValue.cx ,dBottom+sizeValue.cy);		
        pDC->DrawText(sValue,rectChar,DT_SINGLELINE|DT_VCENTER|DT_CENTER|DT_NOCLIP);		
    }

    ////////////////////////////////////////
    // The Frame:
    ////////////////////////////////////////
    long  nW=nFrameLineWidth/2;
    long  n4=nW*4,n2=nW*2,n1=nW;

    // left frame:
    pDC->FillSolidRect(CRect(x11-n4,y11-n4,x11-n2,y22+n4),RGB(0,0,0));
    pDC->FillSolidRect(CRect(x11-n1,y11-n1,x11,y22+n1),RGB(0,0,0));

    //top frame:
    pDC->FillSolidRect(CRect(x11-n4,y11-n4,x22+n4,y11-n2),RGB(0,0,0));
    pDC->FillSolidRect(CRect(x11-n1,y11-n1,x22+n1,y11),RGB(0,0,0));

    //right frame:
    pDC->FillSolidRect(CRect(x22+n2,y11-n4,x22+n4,y22+n4),RGB(0,0,0));
    pDC->FillSolidRect(CRect(x22,y11-n1,x22+n1,y22+n1),RGB(0,0,0));


    // bottom frame:
    pDC->FillSolidRect(CRect(x11-n4,y22+n2,x22+n4,y22+n4),RGB(0,0,0));
    pDC->FillSolidRect(CRect(x11-n1,y22,x22+n1,y22+n1),RGB(0,0,0));

    pDC->SetBkColor(RGB(255,255,255));

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    m_rectCoor=rectDraw;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 返回:
    pDC->SelectObject(pOldFont);
    pDC->SelectObject(pOldPen);
    pDC->SetTextColor(nOldColor);

    return;
}

void CFHZoomView::DrawCoorByHorValues(  CDC      *pDC,
                                        double   dLeftCoor,
                                        double   dRightCoor, 
                                        double   dLowCoor, 
                                        double   dHighCoor,
                                        float    fHorValueRatio,
                                        float    fVerValueRatio)
{
    ////////////////////////////////////////////
    // Prepare:
    //////////////////////////////////////////////

    ////////////////////////////////////
    // The font of The Map:
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
    COLORREF nOldColor=pDC->SetTextColor(m_colorFontCoor);		

    ////////////////////////////////////////////
    // Line Steps:
    double dXThickValueStep,dYThickValueStep;
    double dXThinValueStep,dYThinValueStep;
    double dEdgeLeft,dEdgeRight,dEdgeTop,dEdgeBottom;
    double dMinX,dMinY,dMaxX,dMaxY;

    CMyRect rectDraw;
    int nWidth,nHeight;


    CString sValue;
    if(m_bUseUserCoor)
    {
        dEdgeLeft=m_rectUserThickLine.m_dLeft;
        dEdgeRight=m_rectUserThickLine.m_dRight;
        dEdgeBottom=m_rectUserThickLine.m_dLow;
        dEdgeTop=m_rectUserThickLine.m_dHigh;

        dXThickValueStep=m_sizeUserThickStep.cx;
        dYThickValueStep=m_sizeUserThickStep.cy;
        dXThinValueStep=m_sizeUserThinStep.cx;
        dYThinValueStep=m_sizeUserThinStep.cy;

        rectDraw.m_dLeft =dEdgeLeft;
        rectDraw.m_dRight=dEdgeRight;
        rectDraw.m_dLow=dEdgeBottom;
        rectDraw.m_dHigh=dEdgeTop;

        dMinX=dEdgeLeft;
        dMinY=dEdgeBottom;
        dMaxX=dEdgeRight;
        dMaxY=dEdgeTop;
    }
    else
    {
        if(fabs(dLeftCoor+1)<0.0001&&fabs(dRightCoor+1)<0.0001) // Let this class decide the drawing range :
        {
            if(!pDC->IsPrinting ())
            {
                GetClientLogicalRange(dMinX,dMinY,dMaxX,dMaxY);	

                CRect rectClient;
                GetClientRect(&rectClient);
                nWidth=rectClient.Width ();
                nHeight=rectClient.Height();
                if(nWidth==0||nHeight==0)return;
            }
            else
            {
                dMinX=m_pointLogOrg.x;
                dMinY=m_pointLogOrg.y;
                dMaxX=dMinX+m_totalLog.cx;
                dMaxY=dMinY+m_totalLog.cy;

                double x1=dMinX,y1=dMinY,x2=dMaxX,y2=dMaxY;
                LPtoWorld(x1,y1);
                LPtoWorld(x2,y2);

                nWidth=x2-x1;
                nHeight=fabs(y2-y1);
            }
        }
        else  // Draw the rect transfered from parameters:
        {
            dMinX=dLeftCoor;
            dMaxX=dRightCoor;
            dMinY=dLowCoor;
            dMaxY=dHighCoor;

            double x1=dMinX,y1=dMinY,x2=dMaxX,y2=dMaxY;
            LPtoWorld(x1,y1);
            LPtoWorld(x2,y2);

            nWidth=x2-x1;
            nHeight=fabs(y2-y1);

        }

        CFontGroup font;

        sValue.Format("%1.3lf",(dMinX+dMaxX)/2);
        long  nValueWidth=(pDC->GetTextExtent (sValue)).cx;
        font.Set(dMinX,dMaxX,nWidth,nValueWidth);
        font.CalcFontGroup (dEdgeLeft,dEdgeRight,dXThickValueStep);
        dXThinValueStep=dXThickValueStep/m_nMidScaleDensity;

        sValue.Format("%1.3lf",(dMinY+dMaxY)/2);
        nValueWidth=(pDC->GetTextExtent (sValue)).cy;	
        font.Set(dMinY,dMaxY,nHeight,nValueWidth);
        font.CalcFontGroup (dEdgeBottom,dEdgeTop,dYThickValueStep);
        dYThinValueStep=dYThickValueStep/m_nMidScaleDensity;

        rectDraw.m_dLeft  = m_pointLogOrgBak.x;
        rectDraw.m_dLow   = m_pointLogOrgBak.y;
        rectDraw.m_dRight = rectDraw.m_dLeft  + m_totalLog.cx ;
        rectDraw.m_dHigh  = rectDraw.m_dLow   + m_totalLog.cy ;
    }

    CRectClipper clipper;
    clipper.SetRect (CMyRect(dMinX,dMinY,dMaxX,dMaxY));

    clipper.SetPolygon (rectDraw);

    CMyRect rectDrawbak(rectDraw);
    rectDraw=clipper.GetResultRect ();
    if(rectDraw==CMyRect(0,0,0,0))
    {
        return;
    }
    rectDraw.Normalize ();

    //////////////////////////////////////////////
    //  The Pens:
    CPen penThinCoorHor,penThickCoorHor,penThinCoorVer,penThickCoorVer,penFrame;

    long nThinLineWidth;
    long nThickLineWidth;
    long nFrameLineWidth;

    if(pDC->IsPrinting ())
    {
        nThinLineWidth=1;
        nThickLineWidth=10;
        nFrameLineWidth=16;
    }
    else
    {
        nThinLineWidth=1;
        nThickLineWidth=2;
        nFrameLineWidth=4;
    }

    penThinCoorHor.CreatePen (PS_SOLID,nThinLineWidth,m_colorThinCoorHor);
    penThickCoorHor.CreatePen (PS_SOLID,nThickLineWidth,m_colorThickCoorHor);
    penThinCoorVer.CreatePen (PS_SOLID,nThinLineWidth,m_colorThinCoorVer);
    penThickCoorVer.CreatePen (PS_SOLID,nThickLineWidth,m_colorThickCoorVer);
    penFrame.CreatePen (PS_SOLID,nFrameLineWidth,m_colorThickCoorHor);

    ////////////////////////////////////////
    // The Frame:
    double x11,y11,x22,y22;	
    x11=rectDraw.m_dLeft;
    x22=rectDraw.m_dRight;
    y11=rectDraw.m_dLow;
    y22=rectDraw.m_dHigh;

    LPtoWorld(x11,y11);
    LPtoWorld(x22,y22);
    if(x11>x22)std::swap(x11,x22);
    if(y11>y22)std::swap(y11,y22);

    long  nW=nFrameLineWidth/2;
    long  n=nW*4;
    pDC->FillSolidRect(CRect(x11-n,y11-n,x22+n,y22+n),RGB(0,0,0));
    n=nW*2;
    pDC->FillSolidRect(CRect(x11-n,y11-n,x22+n,y22+n),RGB(255,255,255));
    n=nW;
    pDC->FillSolidRect(CRect(x11-n,y11-n,x22+n,y22+n),RGB(0,0,0));
    n=0;
    pDC->FillSolidRect(CRect(x11-n,y11-n,x22+n,y22+n),RGB(255,255,255));

    ////////////////////////////////////////////
    // Preparation of the Coordinates:
    double i,x,y;
    CSize sizeValue;
    CRect rectChar;

    ////////////////////////////////////////////////
    // Draw Left-Right Thin Lines:
    pDC->SelectObject(&penThinCoorHor);

    if(dYThinValueStep<0.00000000001)
    {
        dYThinValueStep=0.00000000001;
    }

    for(i=dEdgeBottom+dYThinValueStep;i<=rectDraw.m_dHigh;i+=dYThinValueStep)
    {
        if(i<m_pointLogOrg.y||i>m_pointLogOrg.y+m_totalLog.cy)continue;
        if(i<rectDraw.m_dLow||i>rectDraw.m_dHigh)continue;

        Line(pDC,rectDraw.m_dLeft,i,rectDraw.m_dRight,i);
    }

    ////////////////////////////////////////////////
    // Draw Left-Right Thick Lines:
    CPen *pOldPen=pDC->SelectObject(&penThickCoorHor);

    double dRight=x22+nFrameLineWidth*3;
    double dBottom=y22+nFrameLineWidth*3;

    if(dYThickValueStep<0.00000000001)
    {
        dYThickValueStep=0.00000000001;
    }

    double dRealValue;
    for(i=dEdgeBottom+dYThickValueStep;i<=dEdgeTop;i+=dYThickValueStep)
    {
        // Draw the coordinate line:
        if(i<rectDraw.m_dLow||i>rectDraw.m_dHigh)continue;
        Line(pDC,rectDraw.m_dLeft,i,rectDraw.m_dRight,i);

        // Write character:
        dRealValue=m_rectCoor.m_dLow+(i-m_rectCoor.m_dLow)/fVerValueRatio;

        sValue=GetValueString(dRealValue,dYThickValueStep/fVerValueRatio);
        sizeValue=pDC->GetTextExtent (sValue);
        y=i;
        LPtoWorldY(y);

        rectChar.SetRect(dRight, y-sizeValue.cy*0.5, dRight+sizeValue.cx, y+sizeValue.cy);
        pDC->DrawText(sValue,rectChar,DT_SINGLELINE|DT_VCENTER|DT_CENTER|DT_NOCLIP);		
    }

    ////////////////////////////////////////////////
    // Draw Up-Down Thick Lines:
    pDC->SelectObject(&penFrame);
    if(dXThickValueStep<0.00000000001)
    {
        dXThickValueStep=0.00000000001;
    }

    double xBak,xx,dNoteYPos;
    int nPointQty=GetPointQty();
    CStringList listNotes;
    for(i=0;i<nPointQty;i++)
    {
        x=GetCoor(0,i,0);
        if(x<rectDraw.m_dLeft||x>rectDraw.m_dRight)continue;
        xBak=x;
        listNotes.RemoveAll();
        GetCoorNote(listNotes,0,i,0);

        y=rectDraw.m_dLow;
        LPtoWorld(x,y);

        pDC->Rectangle(x-1,y+2,x+1,y+3);

        // Write character:
        n=(xBak-dEdgeLeft)/dXThickValueStep;
        xx=n*dXThickValueStep;
        dNoteYPos=dBottom;
        if(fabs(xx-(xBak-dEdgeLeft))<dXThickValueStep/100)
        {
            POSITION pos=listNotes.GetHeadPosition();
            while(pos)
            {
                sValue=listNotes.GetNext(pos);
                sizeValue=pDC->GetTextExtent (sValue);

                rectChar.SetRect(x-sizeValue.cx ,dNoteYPos,x+sizeValue.cx ,dNoteYPos+sizeValue.cy);
                pDC->DrawText(sValue,rectChar,DT_SINGLELINE|DT_VCENTER|DT_CENTER|DT_NOCLIP);

                dNoteYPos+=sizeValue.cy+3;
            }
        }
    }

    m_rectCoor=rectDraw;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 返回:
    pDC->SelectObject(pOldFont);
    pDC->SelectObject(pOldPen);
    pDC->SetTextColor(nOldColor);

    return;
}


CMyRect CFHZoomView::GetCoorRect()
{
    return m_rectCoor;
}

void CFHZoomView::GetClientLogicalRange(double &dMinX,double &dMinY,double &dMaxX,double &dMaxY)
{
    CRect rectClient;
    GetClientRect(&rectClient);

    //////////////////////////////////////////////
    // When drawing, we only care the client area:
    // we care the m_pointDevOrg in WorldToLP, and LPtoWorld
    dMinX=0;
    dMaxX=rectClient.right;

    dMinY=0;
    dMaxY=rectClient.bottom ;

    WorldtoLP(dMinX,dMinY);
    WorldtoLP(dMaxX,dMaxY);
}

bool CFHZoomView::Line(CDC *pDC,double x1, double y1, double x2, double y2)
{
    LPtoWorld(x1,y1);
    LPtoWorld(x2,y2);
    CMyRect rect;

    int nCapsHor,nCapsVer;
    if(pDC->IsPrinting ())
    {
        nCapsHor=pDC->GetDeviceCaps (HORZRES);
        nCapsVer=pDC->GetDeviceCaps (VERTRES);
        rect.SetRect(0,0,nCapsHor,nCapsVer);
    }
    else if(IsWindow(m_hWnd))
    {
        CRect r;
        GetClientRect(&r);
        rect.SetRect (r.left ,r.bottom ,r.right ,r.top );
    }
    else
    {
        rect.SetRect(x1,y1,x2,y2);
    }

    CLine line(x1,y1,x2,y2);
    CLine lineInter=rect.Intersect (line);
    if(lineInter.x1!=0||lineInter.x2!=0||lineInter.y1!=0||lineInter.y2!=0)
    {
        pDC->MoveTo (lineInter.x1,lineInter.y1);
        pDC->LineTo (lineInter.x2,lineInter.y2);
        return true;
    }

    return false;
}

CString CFHZoomView::GetValueString(double dValue, double dStep)
{
    CString sValue;

    if(dStep<0.000001)
        sValue.Format("%1.8lf",dValue);
    else if(dStep<0.00001)
        sValue.Format("%1.7lf",dValue);
    else if (dStep<0.0001)
        sValue.Format("%1.6lf",dValue);
    else if(dStep<0.001)
        sValue.Format("%1.5lf",dValue);
    else if(dStep<0.01)
        sValue.Format("%1.4lf",dValue);
    else if(dStep<0.1)
        sValue.Format("%1.3lf",dValue);
    else if(dStep<=1)
        sValue.Format("%1.2lf",dValue);
    else if(dStep<2)
        sValue.Format("%1.1lf",dValue);
    else
        sValue.Format("%1.0lf",dValue);


    return sValue;
}


void CFHZoomView::SetTiltFont(LOGFONT *pLogFont,CDC *pDC)
{
    ////////////////////////////////////////////////
    // Make the logfont:
    if(m_pFontTilt)
    {
        delete []m_pFontTilt;
        m_pFontTilt=NULL;
    }
    m_pFontTilt=new CFont[m_nFont];

    if(pLogFont==NULL)
    {
        memset(&m_logfontMapChar,0,sizeof(LOGFONT));
        m_logfontMapChar.lfHeight       = 15;
        m_logfontMapChar.lfCharSet      = ANSI_CHARSET;
        m_logfontMapChar.lfStrikeOut    = false;
        m_logfontMapChar.lfUnderline    = false;
        m_logfontMapChar.lfOutPrecision = OUT_TT_PRECIS ;
        m_logfontMapChar.lfQuality      = PROOF_QUALITY;
        strcpy(m_logfontMapChar.lfFaceName,"Arial");
    }
    else
    {
        m_logfontMapChar=*pLogFont;
    }


    ////////////////////////////////////////////////
    // Create the font:
    long i;
    for(i=0;i<m_nFont/2;i++)
    {
        m_logfontMapChar.lfEscapement=-i*(180/m_nFont)*10;
        m_pFontTilt[i].CreateFontIndirect(&m_logfontMapChar);
        //m_pFontTilt[i].CreatePointFontIndirect (&thisLogFont,pDC);
    }

    for(i=m_nFont/2;i<m_nFont;i++)
    {
        m_logfontMapChar.lfEscapement=900-(i-m_nFont/2)*(180/m_nFont)*10;
        m_pFontTilt[i].CreateFontIndirect(&m_logfontMapChar);
        //m_pFontTilt[i].CreatePointFontIndirect (&thisLogFont,pDC);
    }	
}

void CFHZoomView::OnSetFocus(CWnd* pOldWnd) 
{
    SetToolBarButtonState();
    CView::OnSetFocus(pOldWnd);
}


void CFHZoomView::HideSearchButton(bool bHide)
{
    if(!m_pZoomViewToolBar)return;

    CToolBarCtrl  &barCtrl=m_pZoomViewToolBar->GetToolBarCtrl();

    barCtrl.HideButton(ID_FHZoomViewLast,bHide);
    barCtrl.HideButton(ID_FHZoomViewNext,bHide);
    barCtrl.HideButton(ID_FHZoomViewSearch,bHide); 

    m_menuManager.RemoveMenu (ID_FHZoomViewLast);
    m_menuManager.RemoveMenu (ID_FHZoomViewNext);
    m_menuManager.RemoveMenu (ID_FHZoomSearch);

    CFrameWnd*pFrame= m_pZoomViewToolBar->GetDockingFrame ( ) ;
    pFrame->RecalcLayout ();

}


void CFHZoomView::OnFHZoomInArea() 
{
    if(m_eZoomState!=statueZoomInArea)
    {
        m_eZoomState=statueZoomInArea;
    }
    else
    {
        m_eZoomState=statueIdle;
    }
    SetToolBarButtonState();
}

void CFHZoomView::OnFHZoomViewExportBitmap() 
{
    ///////////////////////////////////////
    // Firstly get the file name:
    CFileDialog dlg(false);
    dlg.m_ofn.lpstrFilter="Bitmap file (*.bmp)\0*.bmp";
    dlg.m_ofn.lpstrTitle="Save";
    dlg.m_ofn.lpstrDefExt=".bmp";

    if(dlg.DoModal()==IDCANCEL)return ;

    CString sFile=dlg.GetPathName();

    /////////////////////////////
    //  Draw a Bitmap:
    long nWidth=500;
    long nHeight=500;

    CClientDC ClientDC(this);

    CBitmap bitmap;
    if(!bitmap.CreateCompatibleBitmap(&ClientDC,nWidth,nHeight))
    {
        AfxMessageBox("Error: memory is NOT enough!");
        return;
    }

    CDC MemDC;
    MemDC.CreateCompatibleDC (&ClientDC);
    CBitmap *pOldBitmap=MemDC.SelectObject (&bitmap);

    MemDC.PatBlt        (0,0,nWidth,nHeight,WHITENESS);

    ScrollToPosition    (CMyPoint(0,0));
    MemDC.SetBkMode     (TRANSPARENT);

    OnPrepareDC         (&MemDC);
    DrawCoor            (&MemDC);
    DrawBackPicture     (&MemDC);
    OnDraw              (&MemDC);	

    MemDC.SelectObject  (pOldBitmap);


    //////////////////////////////
    // Save to the file:
    CBmpProc bmp;
    if(!bmp.LoadFromHbmp (HBITMAP(bitmap)))
    {
        AfxMessageBox("Error: can not copy from the inner bitmap!");
        return;
    } 

    if(!bmp.Save (sFile))
    {
        AfxMessageBox("Error: can not save the file:"+sFile);
        return;
    }
}

void CFHZoomView::OnFHZoomErrorMsg() 
{
    m_dlgErrorMsg.m_bCanShowInfo=!m_dlgErrorMsg.m_bCanShowInfo;

    if(m_dlgErrorMsg.m_bCanShowInfo)
    {
        if(m_sError=="")m_sError="没有错误信息";
        m_dlgErrorMsg.SetMsg(m_sError);
        m_dlgErrorMsg.ShowWindow(SW_SHOWNA);
    }
    
    else
    {
        m_dlgErrorMsg.ShowWindow (SW_HIDE);
    }

    SetToolBarButtonState();

}


void CFHZoomView::SetDirection(bool bUpIncrease)
{
    m_bUpIncrease=bUpIncrease;
}

bool CFHZoomView::GetDirection()
{
    return m_bUpIncrease;
}

void CFHZoomView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
    ZoomFull();
    UpdateBars();

    CView::OnUpdate(pSender, lHint,  pHint) ;
}

void CFHZoomView::SetToolBar(CToolBar *pBar)
{
    ASSERT_VALID(pBar);
    m_pZoomViewToolBar=pBar;

    m_pZoomViewToolBar->LoadToolBar (IDR_FHZoomViewToolBar);

    CFrameWnd*pFrame= m_pZoomViewToolBar->GetDockingFrame ( ) ;
    pFrame->RecalcLayout ();

}

void CFHZoomView::OnPaint() 
{
    CPaintDC dc(this); // device context for painting

    OnPrepareDC(&dc);
    CMemDC memDC(&dc,this);
    if(!memDC.Succeed())
    {
        return ;
    }

    if(m_bDrawCoor)
    {
        DrawCoor(&memDC);
    }

    DrawBackPicture(&memDC);

    if(m_sError=="")m_sError="没有错误信息";
    m_dlgErrorMsg.SetMsg(m_sError);

    if(m_bDrawTitle)
    {
        DrawTitle(&memDC);
    }

    OnDraw(&memDC);

    if(m_bDrawBar)
    {
        DrawBar(&memDC);
    }

    if(m_nFirstShow)
    {
        ZoomFull();
        m_nFirstShow=false;
    }

    // Do not call CScrollView::OnPaint() for painting messages
}

BOOL CFHZoomView::OnEraseBkgnd(CDC* pDC) 
{
    return false;
}

void CFHZoomView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
    //////////////////////////////////////////////
    //
    //////////////////////////////////////////////
    /*
    DWORD nVolumeSerialNumber=9999;  // address of volume serial number
    if(!GetVolumeInformation(
    NULL,        // address of root directory of the 
    // file system
    NULL,     // address of name of the volume
    0,         // length of lpVolumeNameBuffer
    &nVolumeSerialNumber,  // address of volume serial number
    NULL,
    // address of system's maximum 
    // filename length
    NULL,     // address of file system flags
    NULL, // address of name of file system
    0// length of lpFileSystemNameBuffer
    ))
    {
    return;
    }
    else
    {	
    bool bValid=false;
    int n;
    for(n=0;n<nMachineQuantity;n++)
    {
    if(nVolumeSerialNumber==MACHINE_NUMBER[n])
    {
    bValid=true;
    break;
    }
    }

    if(!bValid)return;

    }
    */
    ////////////////////////////////////////////////////////////
    // For the printer:
    ////////////////////////////////////////////////////////////
    if(pDC->IsPrinting ())
    {
        ///////////////////////////////////////////
        // The capcity of the device:
        int nCapsHor=pDC->GetDeviceCaps (HORZRES);
        int nCapsVer=pDC->GetDeviceCaps (VERTRES);

        ///////////////////////////////////////////
        // The Device Size:
        if(nCapsHor<nCapsVer)
        {
            m_totalDev.cx = nCapsHor*0.9;
            m_totalDev.cy = m_totalDev.cx;
        }
        else
        {
            m_totalDev.cy = nCapsVer*0.9; 
            m_totalDev.cx = m_totalDev.cy;
        }

        if(m_bPrintByScale)
        {
            double dScale=m_totalLog.cy /m_totalLog.cx;
            m_totalDev.cy*=dScale;

            long nLimit=nCapsVer*0.9;
            while(m_totalDev.cy>nLimit)
            {
                m_totalDev.cx/=1.01;
                m_totalDev.cy/=1.01;
            }
        }

        pDC->SetMapMode(MM_TEXT);
        pDC->SetViewportOrg(nCapsHor*0.05,nCapsVer*0.05);
        pDC->SetViewportExt(m_totalDev.cx,m_totalDev.cy);
        pDC->SetWindowExt(0,0);
        pDC->SetWindowExt(m_totalDev.cx,m_totalDev.cy);
    }

    SetTiltFont(&m_logfontMapChar,pDC);

    // Do NOT need to call CView::OnPrepareDC:
    //CView::OnPrepareDC(pDC, pInfo);
}

void CFHZoomView::OnRButtonDown(UINT nFlags, CPoint point) 
{
    m_eZoomState=statueIdle;

    SetToolBarButtonState();
    SetCurrentCursor();
    CView::OnRButtonDown(nFlags, point);
}


void CFHZoomView::OnTimer(UINT nIDEvent) 
{
    if(nIDEvent==ID_SHOWINFOMATION && m_dlgInfo.m_bCanShowInfo)
    {
        CString sInfo;

        // 鼠标位置不变：
        if(     m_pointCurrentMouse==m_pointLastMouse
            &&  (GetFocus()==this||GetFocus()==&m_dlgInfo))
        {
            // Info not showing:
            if(!m_bInfoOnShow)
            {
                m_timeMouseInSamePlace+=m_timeInterval;
                if(m_timeMouseInSamePlace>=m_timeToShowInfo)
                {
                    m_bInfoOnShow=true;

                    CMyPoint pointClient=m_pointCurrentMouse;
                    double x=pointClient.x;
                    double y=pointClient.y;

                    WorldtoLP(x,y);

                    sInfo = GetInfo(x,y);
                    if(sInfo == "")
                    {
                        m_dlgInfo.SetMsg ("");
                        m_dlgInfo.ShowWindow(SW_HIDE);
                    }
                    else
                    {
                        m_dlgInfo.SetMsg (GetInfo(x,y));
                        m_dlgInfo.ShowWindow(SW_SHOWNA);
                    }
                }
            } 
        }

        // 鼠标位置变化了：
        else
        {
            if(m_bInfoOnShow)
            {
                m_bInfoOnShow=false;
            }
            m_pointLastMouse=m_pointCurrentMouse;
            m_timeMouseInSamePlace=0;
        }
    }

    CView::OnTimer(nIDEvent);
}

void CFHZoomView::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    m_pointLastMouse=m_pointCurrentMouse;
    m_pointCurrentMouse=point;
    if(m_pointCurrentMouse==m_pointLastMouse)
    {
        return;
    }

    double x,y;

    if(m_eZoomState==statueZoomInArea&&m_bZoomAreaAnchored)
    {
        CDC *pDC=GetDC();
        if(pDC)
        {
            int nOldROP2=pDC->GetROP2 ();
            pDC->SetROP2(R2_XORPEN);

            CPen *pOldPen=pDC->SelectObject (&m_penZoomRect);
            pDC->MoveTo(m_pointZoomAreaAnchor.x,m_pointZoomAreaAnchor.y);
            pDC->LineTo(m_pointZoomAreaAnchor.x,m_pointLastMouse.y);
            pDC->LineTo(m_pointLastMouse.x,m_pointLastMouse.y);
            pDC->LineTo(m_pointLastMouse.x,m_pointZoomAreaAnchor.y);
            pDC->LineTo(m_pointZoomAreaAnchor.x,m_pointZoomAreaAnchor.y);

            pDC->MoveTo(m_pointZoomAreaAnchor.x,m_pointZoomAreaAnchor.y);
            pDC->LineTo(m_pointZoomAreaAnchor.x,point.y);
            pDC->LineTo(point.x,point.y);
            pDC->LineTo(point.x,m_pointZoomAreaAnchor.y);
            pDC->LineTo(m_pointZoomAreaAnchor.x,m_pointZoomAreaAnchor.y);

            pDC->SelectObject (pOldPen);
            pDC->SetROP2(nOldROP2);
        }
    }
    else
    {
        if(m_bMouseAnchored)
        {
            x=point.x;
            y=point.y;
            WorldtoLP(x,y);
            x-=m_pointMouseAnchorPoint.x;
            y-=m_pointMouseAnchorPoint.y;
            MoveAnchoredPoint(x,y);
            m_pointMouseAnchorPoint.x+=x;
            m_pointMouseAnchorPoint.y+=y;
        } 
    }


    SetCurrentCursor();

    x=point.x;
    y=point.y;
    WorldtoLP(x,y);

    double dMinDistance=99999999999.9;
    if(CheckPointingChange(x,y,dMinDistance))
    {
        Invalidate();
        m_dlgInfo.Show(GetInfo(x,y));
    }

    CView::OnMouseMove(nFlags, point);
}

void CFHZoomView::OnFHEnableInfoWnd() 
{
    // TODO: Add your command handler code here
    m_dlgInfo.m_bCanShowInfo=!m_dlgInfo.m_bCanShowInfo;

    if(m_dlgInfo.m_bCanShowInfo)
        m_dlgInfo.ShowWindow(SW_SHOW);
    else
        m_dlgInfo.ShowWindow(SW_HIDE);

    SetToolBarButtonState();
}

#include "winuser.h"

void CFHZoomView::OnLButtonDown(UINT nFlags, CPoint point) 
{
    // 判断鼠标单击双击的办法，适合鼠标单击后弹起时的操作：
    //MSG     msg     =   {   0   }; 
    //DWORD   timeMsg   =   ::GetMessageTime(); 
    //int     nDoubleClickTime = ::GetDoubleClickTime();
    //int nLBUp = WM_LBUTTONUP;
    //int nLBDC = WM_LBUTTONDBLCLK;

    //while(true)
    //{ 
    //     ::PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE);

    //     TRACEERROR("Msg:"+vtos(int(msg.message)));

    //     if( msg.message == WM_LBUTTONDBLCLK ) 
    //     {
    //         return;
    //     }

    //    if(::GetTickCount() - timeMsg > nDoubleClickTime) 
    //    {
    //        break; 
    //    }
    //} 
    if(m_eZoomState!=statueIdle)
    {
        if(m_eZoomState==statueZoomInArea)
        {
            SetCapture();
            m_pointZoomAreaAnchor=point;
            m_bZoomAreaAnchored=true;
        }
        else
        {
            CMyPoint pointMine=point;
            Zoom(&pointMine);
        }
    }
    else
    {
        double x=point.x;
        double y=point.y;
        WorldtoLP(x,y);
        m_pointMouseAnchorPoint.x=x;
        m_pointMouseAnchorPoint.y=y;

        m_bMouseAnchored        = true;
        CheckAnchoredPoint(x,y);
    }

    m_pointLastMouse=point;
    OnTimer(ID_SHOWINFOMATION);
    SetFocus();

    CView::OnLButtonDown(nFlags, point);
}

HCURSOR CFHZoomView::SetCurrentCursor()
{
    if(m_eZoomState==statueZoomOut)
        return SetCursor(m_hCursorZoomOut);
    else if(m_eZoomState==statueZoomIn)
        return SetCursor(m_hCursorZoomIn);
    else
        return SetCursor(m_hCursorDefault);
}


void CFHZoomView::OnLButtonUp(UINT nFlags, CPoint point) 
{
    // TODO: Add your message handler code here and/or call default
    SetCurrentCursor();

    if(m_eZoomState==statueZoomInArea)
    {
        m_bZoomAreaAnchored=false;
        CDC *pDC=GetDC();
        if(pDC)
        {
            int nOldROP2=pDC->GetROP2 ();
            pDC->SetROP2(R2_XORPEN);

            CPoint pL=m_pointZoomAreaAnchor;

            CPen *pOldPen=pDC->SelectObject (&m_penZoomRect);
            pDC->MoveTo(pL.x,pL.y);
            pDC->LineTo(pL.x,point.y);
            pDC->LineTo(point.x,point.y);
            pDC->LineTo(point.x,m_pointZoomAreaAnchor.y);
            pDC->LineTo(pL.x,pL.y);

            pDC->SelectObject (pOldPen);
            pDC->SetROP2(nOldROP2);

            ReleaseCapture();
            CMyRect rectZoom(pL.x,pL.y,point.x,point.y);
            WorldtoLP(rectZoom.m_dLeft,rectZoom.m_dLow);
            WorldtoLP(rectZoom.m_dRight,rectZoom.m_dHigh);

            if(!ZoomArea(rectZoom))
            {
                Invalidate();
            }
        }
    }
    else
    {
        m_bMouseAnchored=false;
    }

    CView::OnLButtonUp(nFlags, point);
}

void CFHZoomView::OnFHZoomFull() 
{
    ZoomFull();
}


BOOL CFHZoomView::OnPreparePrinting(CPrintInfo* pInfo) 
{
    pInfo->SetMinPage (1);
    pInfo->SetMaxPage (1);
    return CView::DoPreparePrinting (pInfo);
}

void CFHZoomView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
    m_totalDev=m_totalDevBak;
    CView::OnEndPrinting(pDC, pInfo);
}


void CFHZoomView::OnFHEnableCor() 
{
    // TODO: Add your command handler code here
    ToggleCoor();
}

void CFHZoomView::ToggleCoor()
{
    m_bDrawCoor=!m_bDrawCoor;
    Invalidate();
    SetToolBarButtonState();
}

void CFHZoomView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
    m_totalDevBak=m_totalDev;
    CView::OnBeginPrinting(pDC, pInfo);
}

bool CFHZoomView::Zoom(CMyPoint *pPointCenter,bool bLogical)
{
    double xCenter=0.0;
    double yCenter=0.0;

    if(pPointCenter)
    {
        if(bLogical)
        {
            xCenter=pPointCenter->x;
            yCenter=pPointCenter->y;
        }
        else
        {
            xCenter=pPointCenter->x;
            yCenter=pPointCenter->y;
            WorldtoLP(xCenter,yCenter);
        }
    }

    //
    double dx,dy;
    bool bLogicalChanged=true;
    switch (m_eZoomState)
    {
    case statueZoomOut:
        dx=m_totalDev.cx/m_dCooeHor;
        dy=m_totalDev.cy/m_dCooeVer;
        break;
    case  statueZoomIn:
        dx=m_totalDev.cx*m_dCooeHor;
        dy=m_totalDev.cy*m_dCooeVer;
        break;
    case  statueZoomInHor:
        dx=m_totalDev.cx*m_dCooeHor;
        dy=m_totalDev.cy;
        break;
    case  statueZoomOutHor:
        dx=m_totalDev.cx/m_dCooeHor;
        dy=m_totalDev.cy;
        break;
    case  statueZoomInVer:
        dx=m_totalDev.cx;
        dy=m_totalDev.cy*m_dCooeVer;
        break;
    case  statueZoomOutVer:
        dx=m_totalDev.cx;
        dy=m_totalDev.cy/m_dCooeVer;
        break;
    case statueZoomInArea:
        dx=m_totalDev.cx;
        dy=m_totalDev.cy;
        break;
    default:
        dx=m_totalDev.cx;
        dy=m_totalDev.cy;
        bLogicalChanged=false;
    }

    if(bLogicalChanged&&dx>0.001&&dx<9999999999.9&&dy>0.001&&dy<9999999999.9)
    {
        m_totalDev.cx=dx;
        m_totalDev.cy=dy;

        CRect rect;
        GetClientRect(&rect);

        m_pageDev.cx = rect.Width ()/3.0;
        m_pageDev.cy = rect.Height()/3.0;
        m_lineDev.cx = rect.Width ()/15.0;
        m_lineDev.cy = rect.Height()/15.0;

        /*if(int(m_lineDev.cx / m_totalDev.cx* m_sizeScrollBar.cx)<1)
        {
        m_lineDev.cx =1.0/m_sizeScrollBar.cx*m_totalDev.cx;
        }

        if(int(m_lineDev.cy / m_totalDev.cy* m_sizeScrollBar.cy)<1)
        {
        m_lineDev.cy =1.0/m_sizeScrollBar.cy*m_totalDev.cy;
        }

        if(int(m_pageDev.cx / m_totalDev.cx* m_sizeScrollBar.cx)<5)
        {
        m_pageDev.cx =5.0/m_sizeScrollBar.cx*m_totalDev.cx;
        }

        if(int(m_pageDev.cy / m_totalDev.cy* m_sizeScrollBar.cy)<5)
        {
        m_pageDev.cy =5.0/m_sizeScrollBar.cy*m_totalDev.cy;
        }*/


        UpdateBars();
    }

    // The mouse point is in the valid area:
    if(pPointCenter)
    {
        CalcScrollPos(xCenter,yCenter);	// Will Invalidate in this;
    }
    else
    {
        Invalidate();
    }
    return true;
}

bool CFHZoomView::ZoomArea(CMyRect rectLogicalArea)
{
    rectLogicalArea.Normalize ();
    if(rectLogicalArea.Height()<=0.000001)return false;

    ///////////////////////////////////////
    // 获得所防大范围的物理范围：
    double x1=rectLogicalArea.m_dLeft ;
    double y1=rectLogicalArea.m_dLow;
    double x2=rectLogicalArea.m_dRight ;
    double y2=rectLogicalArea.m_dHigh;
    LPtoWorld(x1,y1);
    LPtoWorld(x2,y2);

    double wl=fabs(x2-x1);
    double hl=fabs(y2-y1);

    ///////////////////////////////////////
    // 获得所防大范围的物理范围：
    CRect rectClient;
    GetClientRect(&rectClient);
    double wc=rectClient.Width()-m_nBarWidth-m_nEdgeWidth;
    double hc=rectClient.Height()-m_nBarWidth-m_nEdgeWidth;

    if(wc<1)wc=10;
    if(hc<1)hc=10;
    while(wc<100||hc<100)
    {
        wc*=1.1;
        hc*=1.1;
    }

    ///////////////////////////
    // 计算所需要的放大系数
    double dScale,dScale1,dScale2;
    dScale1=wc/wl;
    dScale2=hc/hl;

    if(dScale1<dScale2)
        dScale=dScale1;
    else
        dScale=dScale2;

    /////////////////////////////////////////
    // 重新计算设备宽度和高度
    // dScale的许可范围很大，这是因为
    // 当放大到很大时，再返回来，dScale就会很小。
    // 当缩小到很小时，再返回来，dScale就会很大。
    if(     m_totalDev.cx   >   0.00000000000001
        &&  m_totalDev.cx   <   9999999999999.9
        &&  m_totalDev.cy   >   0.00000000000001
        &&  m_totalDev.cy   <   999999999999.9
        &&  dScale          >   0.00000000001
        &&  dScale          <   10000000000)
    {
        m_totalDev.cx *= dScale;
        m_totalDev.cy *= dScale;

        CMyPoint pointCenter(rectLogicalArea.MidHor(),rectLogicalArea.MidVer());
        Zoom(&pointCenter,true);
    }

    if(m_totalDev.cx<10)m_totalDev.cx=10;
    if(m_totalDev.cy<10)m_totalDev.cy=10;

    UpdateBars();

    return true;
}

void CFHZoomView::CalcScrollPos(double x, double y)
{
    LPtoWorld(x,y);

    CRect rect;
    GetClientRect(&rect);

    double xm=rect.left   + (rect.right - rect.left   )/2;
    double ym=rect.bottom + (rect.top   - rect.bottom )/2;

    OnMyScrollBy(CMySize(x-xm,y-ym),true);

    return;
}

CString CFHZoomView::GetInfo(double x,double y)
{
    CString sInfo;
    sInfo.Format ("%1.0lf,%1.0lf",x,y);
    return sInfo;
}

void CFHZoomView::GetVisibleArea(double &x1, double &y1, double &x2, double &y2)
{
    CRect clientRect;
    if(IsWindow(m_hWnd))
    {
        GetClientRect(&clientRect);
    }
    else
    {
        clientRect.SetRect(0,0,2000,1600);
    }

    x1=clientRect.left ;
    y1=clientRect.bottom;
    x2=clientRect.right ;
    y2=clientRect.top ;

    WorldtoLP(x1,y1);
    WorldtoLP(x2,y2);

    double temp;
    if(x1>x2)
    {
        temp=x1;
        x1=x2;
        x2=temp;
    }

    if(y1>y2)
    {
        temp=y1;
        y1=y2;
        y2=temp;
    }
}

void CFHZoomView::WriteFontString(  double x1, 
                                    double y1, 
                                    double x2, 
                                    double y2, 
                                    CString sString, 
                                    CDC  *pDC ,   //=null
                                    UINT  position  )
{
    ////////////////////////////////////
    // Prepare the DC:
    bool bGetDC=false;
    if(pDC==NULL)
    {
        pDC=GetDC();
        bGetDC=true;
    }

    ////////////////////////////////////////////////////////
    // 
    if(position==0)
        position=DT_SINGLELINE|DT_TOP|DT_LEFT|DT_NOCLIP;
    else
        position|=DT_SINGLELINE|DT_NOCLIP;


    ////////////////////////////////////////
    // Select the font :
    long  nFont=-1;
    double dx=x2-x1;
    double dy=y2-y1;
    double angle=atan2(dy,dx)/PI*180;
    if(angle<=-180||angle>=180)angle=0;
    if(angle<0)
        nFont=(angle+180)/180*m_nFont;
    else
        nFont=angle/180*m_nFont;
    ASSERT(nFont>=0&&nFont<m_nFont);
    CFont *pOldFont=pDC->SelectObject(&m_pFontTilt[nFont]);
    COLORREF nOldColor=pDC->SetTextColor(m_colorTitle);

    ////////////////////////////////////////////////////////
    // Make the rect that the string will be written in:
    CSize sizeChar=pDC->GetTextExtent(sString);

    CRect rectChar;
    y1+=sizeChar.cy*0.1;
    y2+=sizeChar.cy*0.1;

    double k=sizeChar.cx/sqrt(dx*dx+dy*dy);
    if((position&DT_LEFT)==DT_LEFT)
    {
        x2=x1+(x2-x1)*k;
        y2=y1+(y2-y1)*k;
    }
    else if((position&DT_CENTER)==DT_CENTER)
    {
        double x=x1+(x2-x1)/2;
        double y=y1+(y2-y1)/2;
        x1=x+sizeChar.cx/2*k*(x1-x2);
        x2=x+sizeChar.cx/2*k*(x2-x1);
        y1=y+sizeChar.cx/2*k*(y1-y2);
        y2=y+sizeChar.cx/2*k*(y2-y1);
    }
    else if((position&DT_RIGHT)==DT_RIGHT)
    {
        x1=x2+(x1-x2)*k;
        y1=y2+(y1-y2)*k;
    }

    if(x1<x2)
        rectChar.SetRect(x1,y1,x2,y1+sizeChar.cy/2);
    else if(y2>y1)
        rectChar.SetRect(x2,y1,x2+sizeChar.cx,y2+sizeChar.cy/2);
    else
        rectChar.SetRect(x2,y2,x2+sizeChar.cx,y2+sizeChar.cy/2);

    ////////////////////////////////////////
    // write the text:	
    pDC->DrawText(sString,rectChar,position);

    ////////////////////////////////////////////
    // End:
    pDC->SetTextColor(nOldColor);
    pDC->SelectObject(pOldFont);	
    if(bGetDC)
    {
        ReleaseDC(pDC);
    }

}


void CFHZoomView::ZoomFull()
{
    ZoomArea(CMyRect(   m_pointLogOrg.x,
                        m_pointLogOrg.y,
                        m_pointLogOrg.x+m_totalLog.cx,
                        m_pointLogOrg.y+m_totalLog.cy));
}

void CFHZoomView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
    if(m_bDrawCoor)
    {
        DrawCoor(pDC);
    }

    DrawBackPicture(pDC);

    if(m_bDrawTitle)DrawTitle(pDC);

    CView::OnPrint(pDC, pInfo);
}

void CFHZoomView::DrawTitle(CDC *pDC)
{
    double dLeft   = m_pointLogOrg.x;
    double dRight  = m_pointLogOrg.x + m_totalLog.cx;
    double dBottom,dTop ;
    if(!m_bUpIncrease)
    {
        dBottom = m_pointLogOrg.y + m_totalLog.cy;
        dTop    = dBottom;
    }
    else
    {
        dBottom = m_pointLogOrg.y ;
        dTop    = dBottom;
    }

    LPtoWorld(dLeft,dBottom);
    LPtoWorld(dRight,dTop);

    CFont font;
    if(pDC->IsPrinting ())
    {
        LOGFONT fontPrint=m_logfontTitle;
        fontPrint.lfHeight*=10;
        font.CreateFontIndirect(&fontPrint);
    }
    else
    {
        font.CreateFontIndirect(&m_logfontTitle);
    }

    CFont *pOldFont=pDC->SelectObject(&font);
    COLORREF nOldColor=pDC->SetTextColor(m_colorTitle);

    CSize size=pDC->GetTextExtent (m_sDrawTitle);
    CRect rectTitle(dLeft,dBottom,dRight,dTop);

    rectTitle.NormalizeRect ();
    rectTitle.bottom += 2*size.cy;
    rectTitle.top    += 3*size.cy;

    pDC->DrawText(m_sDrawTitle,rectTitle,DT_VCENTER|DT_SINGLELINE|DT_CENTER|DT_NOCLIP);

    pDC->SelectObject(pOldFont);
    pDC->SetTextColor(nOldColor);
}

void CFHZoomView::DrawBar(CDC *pDC)
{
    double dMinX,dMinY,dMaxX,dMaxY;
    GetClientLogicalRange(dMinX,dMinY,dMaxX,dMaxY);
    CRect rectClient;
    GetClientRect(&rectClient);
    if(rectClient.Width ()==0||rectClient.Height ()==0)return;

    UINT nColorFont=RGB(0,0,0);
    UINT nColorBar=GetSysColor(COLOR_ACTIVEBORDER);

    UINT nColorShine=RGB(255,255,255);
    UINT nColorWhite=RGB(255,255,255);
    UINT nColorBlack=RGB(0,0,0);
    UINT nColorBarInside=RGB(240,240,240);
    UINT nBackColor=nColorBarInside;

    CRect rectChar;
    CString sValue;

    ////////////////////////////////////////////////
    // Draw X Bar:
    // Make 2 pens:
    CPen penBlack,penWhite;
    penBlack.CreatePen(PS_SOLID,0,RGB(0,0,0));
    penWhite.CreatePen(PS_SOLID,0,RGB(255,255,255));	

    CPen *pOldPen=pDC->SelectObject(&penBlack);

    ////////////////////////////////////////////////
    // Draw the X Bar:
    ////////////////////////////////////////////////
    long nBarInSideWidth=m_nBarWidth*0.6;
    long nEdgeWidth=m_nBarWidth*0.2;
    CRect rectXBar(0,0,rectClient.Width ()+2,m_nBarWidth);
    Draw3DRect(pDC,rectXBar,true);

    CRect rectXBarInSide(m_nBarWidth-4,nEdgeWidth,rectXBar.right,m_nBarWidth-nEdgeWidth);
    Draw3DRect(pDC,rectXBarInSide,false,nColorBarInside);

    // Draw lines and caractars.
    pDC->SetBkColor(nBackColor);

    CFont* pOldFont=pDC->SelectObject(&m_fontCorHor);

    // Get the average width of the value:
    sValue.Format("%1.3lf",dMaxX);
    long  n1=(pDC->GetTextExtent (sValue)).cx;
    sValue.Format("%1.3lf",dMinX);
    long  n2=(pDC->GetTextExtent (sValue)).cx;

    long  nValueWidth=n1;
    if(n2>n1)nValueWidth=n2;

    long  nWindowWidth=rectClient.Width ()-m_nBarWidth;
    CFontGroup font;
    font.Set(dMinX,dMaxX,nWindowWidth,nValueWidth);
    double dStart,dEnd,dStep;
    font.CalcFontGroup (dStart,dEnd,dStep);

    //  Draw lines using Black Pen:
    long  nLineBottom=m_nBarWidth-nEdgeWidth;
    long  nLineTop=nLineBottom-5;
    long  nCharTop=nEdgeWidth;
    long  nCharBottom=nLineTop;

    double x,y;
    double i;
    for(i=dStart;i<=dEnd;i+=dStep)
    {
        x=i;
        LPtoWorldX(x);

        pDC->MoveTo(x-1,nLineTop);
        pDC->LineTo(x-1,nLineBottom);
    }


    //  Draw lines using White Pen:
    pDC->SelectObject(&penWhite);
    for(i=dStart;i<=dEnd;i+=dStep)
    {
        x=i;
        LPtoWorldX(x);

        pDC->MoveTo(x,nLineTop);
        pDC->LineTo(x,nLineBottom);

        // Write character:
        sValue=GetValueString(i,dStep);
        rectChar.SetRect(x-10,nCharTop,x+10,nCharBottom);

        pDC->DrawText(sValue,rectChar,DT_SINGLELINE|DT_VCENTER|DT_CENTER|DT_NOCLIP);		
    }

    ////////////////////////////////////////////////
    //  Draw Y Cor:
    ////////////////////////////////////////////////
    CRect rectYBar(0,m_nBarWidth,m_nBarWidth,rectClient.Height ());
    pDC->FillSolidRect (rectYBar,GetSysColor(COLOR_ACTIVEBORDER));
    pDC->DrawEdge (rectYBar,EDGE_RAISED,BF_LEFT);
    pDC->DrawEdge (rectYBar,EDGE_RAISED,BF_RIGHT);

    CRect rectYBarInSide(nEdgeWidth,m_nBarWidth-2,rectYBar.right-nEdgeWidth,rectYBar.bottom);
    Draw3DRect(pDC,rectYBarInSide,false,nColorBarInside);

    pDC->SetBkColor(nBackColor);

    // Get the average width of the value:
    pDC->SelectObject(&m_fontCorVer);

    sValue.Format("%1.3lf",dMaxY);
    n1=(pDC->GetTextExtent (sValue)).cx;
    sValue.Format("%1.3lf",dMinY);
    n2=(pDC->GetTextExtent (sValue)).cx;
    nValueWidth=n1;
    if(n2>n1)nValueWidth=n2;

    long  nWindowHeight=rectClient.Height()-m_nBarWidth;
    font.Set(dMinY,dMaxY,nWindowHeight,nValueWidth);
    font.CalcFontGroup (dStart,dEnd,dStep);

    //  Draw lines using Black Pen:
    long  nLineLeft=m_nBarWidth-nEdgeWidth-5;
    long  nLineRight=m_nBarWidth-nEdgeWidth;

    pDC->SelectObject(&penBlack);
    LPtoWorldX(x);
    for(i=dStart;i<=dEnd;i+=dStep)
    {
        y=i;
        LPtoWorldY(y);

        if(y<m_nBarWidth)continue;

        pDC->MoveTo(nLineLeft,y-1);
        pDC->LineTo(nLineRight,y-1);
    }

    //  Draw lines using White Pen:
    pDC->SelectObject(&penWhite);
    pDC->SetBkColor (nBackColor);
    long  nCharRight=nLineLeft-5;
    long  nCharLeft=nEdgeWidth-5;
    long  nUp,nRight;
    for(i=dStart;i<=dEnd;i+=dStep)
    {
        y=i;
        LPtoWorldY(y);

        pDC->MoveTo(nLineLeft,y);
        pDC->LineTo(nLineRight,y);

        // Write caractar:
        sValue=GetValueString(i,dStep);

        nRight=pDC->GetTextExtent(sValue).cx/2;
        nUp=pDC->GetTextExtent(sValue).cx/2*1.2;
        rectChar.SetRect(nCharLeft+nRight,y-5+nUp,nCharRight+nRight,y+15+nUp);

        pDC->DrawText(sValue,rectChar,DT_SINGLELINE|DT_VCENTER|DT_CENTER|DT_NOCLIP);
    }

    //
    CRect rectCorner;
    rectCorner.left=0;
    rectCorner.right=m_nBarWidth-2;
    rectCorner.top=0;
    rectCorner.bottom=m_nBarWidth;

    //
    pDC->FillSolidRect (rectCorner,nColorBar);
    pDC->DrawEdge (rectCorner,EDGE_RAISED,BF_TOPLEFT);

    pDC->DrawEdge (rectYBarInSide,EDGE_SUNKEN,BF_TOP);
    pDC->DrawEdge (rectXBarInSide,EDGE_SUNKEN,BF_LEFT);

    //
    pDC->SelectObject(pOldFont);
    pDC->SelectObject(pOldPen);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 返回.
    return ;
}

C3DRange<double> CFHZoomView::GetDrawRange()
{
    C3DRange<double> r;
    r.m_minX = m_pointLogOrg.x;
    r.m_minY = m_pointLogOrg.y;
    r.m_maxX = r.m_minX + m_totalLog.cx;
    r.m_maxY = r.m_minY + m_totalLog.cy;

    return r;
}

void CFHZoomView::SetDrawRange(double   dLeft,
                               double   dLow,
                               double   dRight,
                               double   dHigh, 
                               float    fXScale,
                               float    fYScale,
                               bool     bReset)
{
    // Make the draw port from small to big:
    double temp;
    if(dLeft>dRight)
    {
        temp=dLeft;
        dLeft=dRight;
        dRight=temp;
    }

    if(dLow>dHigh)
    {
        temp=dLow;
        dLow=dHigh;
        dHigh=temp;
    }

    if((dHigh-dLow)<=0.00001)dHigh=dLow+0.00001;
    if((dRight-dLeft)<=0.00001)dRight=dLeft+0.00001;
    if((dHigh-dLow)>MAX_DOUBLE)
    {
        dLow=0;
        dHigh=dLow+10000;
    }

    if((dRight-dLeft)>MAX_DOUBLE)
    {
        dLeft=0;
        dRight=dLeft+10000;
    }


    // 逻辑坐标：
    m_pointLogOrg.x=dLeft;
    m_pointLogOrg.y=dLow;
    m_pointLogOrgBak=m_pointLogOrg;

    m_totalLog.cx = dRight-dLeft;
    m_totalLog.cy = dHigh-dLow;


    // 坐标系范围：
    double dStep;
    CFontGroup group;
    group.Set(dLeft,dRight,100,10);
    group.CalcFontGroup (dLeft,dRight,dStep);

    group.Set(dLow,dHigh,100,10);
    group.CalcFontGroup (dLow,dHigh,dStep);

    m_rectUserThickLine.m_dLeft  = dLeft;
    m_rectUserThickLine.m_dRight = dRight;
    m_rectUserThickLine.m_dLow   = dLow;
    m_rectUserThickLine.m_dHigh  = dHigh;

    m_rectUserThinLine=m_rectUserThickLine;

    // 设备坐标
    if(fabs(fXScale)<0.000000000001)
    {
        fYScale=1;
        fXScale=1;
    }

    m_totalDev.cx = 1000;
    m_totalDev.cy = fYScale/fXScale*m_totalDev.cx *(m_totalLog.cy /m_totalLog.cx);

    // 如果m_bDevSizeLimit=true,会影响文件导入模块(那些需要固定设备宽度的视)：
    if(m_bDevSizeLimit)
    {
        while(m_totalDev.cy >5000)
        {
            m_totalDev.cx /=1.1;
            m_totalDev.cy = fYScale/fXScale*m_totalDev.cx *(m_totalLog.cy /m_totalLog.cx);
        }
    }

    m_totalDev.cx +=m_nBarWidth+m_nEdgeWidth;
    m_totalDev.cy +=m_nBarWidth+m_nEdgeWidth;


    return;
}

void CFHZoomView::OnInitialUpdate()
{
    __super::OnInitialUpdate();

    SetToolBarButtonState();

    // timer related with info showing:
    if(m_bEnableInfoTimer)
    {
        SetTimer(ID_SHOWINFOMATION,1,NULL);
    }

    // Set the cursor:
    CWinApp *pApp=AfxGetApp();
    m_hCursorZoomIn=pApp->LoadCursor(IDC_CursorZoomIn);
    m_hCursorZoomOut=pApp->LoadCursor(IDC_CursorZoomOut);	
    m_hCursorDefault=pApp->LoadStandardCursor(IDC_ARROW);

    SetCurrentCursor();

    // Set the initial position of the info and error dlg:
    CRect rectClient;
    GetClientRect(&rectClient);
    ClientToScreen(&rectClient);

    m_dlgInfo.Show(rectClient.left ,rectClient.bottom,"信息窗口");
    m_dlgErrorMsg.Show(rectClient.left ,rectClient.bottom-50,"错误信息窗口");

    if(m_dlgInfo.m_bCanShowInfo )
        m_dlgInfo.ShowWindow(SW_SHOW);
    else 
        m_dlgInfo.ShowWindow(SW_HIDE);

    if(m_dlgErrorMsg.m_bCanShowInfo )
        m_dlgErrorMsg.ShowWindow(SW_SHOW);
    else
        m_dlgErrorMsg.ShowWindow(SW_HIDE);

    m_hookMouse.AddToHook (this);


    //
    //
    m_ePointPatternCommon=eDP_Ellipse;
    m_ePointPatternCurrent=eDP_Ellipse;

    CRuntimeClass *pRuntimeClass=GetRuntimeClass();
    CString sClassName=pRuntimeClass->m_lpszClassName;


    unsigned int n=(unsigned int)eDP_Ellipse;
    ::GetPar("FHZOOMVIEW_PointPatternCommon"+sClassName,n);
    m_ePointPatternCommon=(EPointDrawPattern)n;

    n=( unsigned int)eDP_Ellipse;
    ::GetPar("FHZOOMVIEW_PointPatternCurrent"+sClassName,n);
    m_ePointPatternCurrent=(EPointDrawPattern)n;

    m_colorMapContent=RGB(0,0,0);
    n=m_colorMapContent;
    ::GetPar("FHZOOMVIEW_colorMapContent"+sClassName,n);
    m_colorMapContent=n;

    m_colorMapContentSpecial=RGB(50,50,50);
    n=m_colorMapContentSpecial;
    ::GetPar("FHZOOMVIEW_colorMapContentSpecial"+sClassName,n);
    m_colorMapContentSpecial=n;

    ::GetPar("FHZOOMVIEW_ContentSize"+sClassName,m_iContentSize);
    ::GetPar("FHZOOMVIEW_ContentCurrSize"+sClassName,m_iContentCurrSize);

    //
    if(sClassName!="CFHZoomView")
    {
        ::GetPar("BACK_GROUND_PIC_ZOOMVIEW_"+sClassName,m_sBackGroundFileName);
        ::GetPar("BACK_GROUND_PIC_ZOOMVIEW_"+sClassName+"_EAST",m_rectBackGroundPic.m_dRight);
        ::GetPar("BACK_GROUND_PIC_ZOOMVIEW_"+sClassName+"_WEST",m_rectBackGroundPic.m_dLeft);
        ::GetPar("BACK_GROUND_PIC_ZOOMVIEW_"+sClassName+"_SOUTH",m_rectBackGroundPic.m_dLow);
        ::GetPar("BACK_GROUND_PIC_ZOOMVIEW_"+sClassName+"_NORTH",m_rectBackGroundPic.m_dHigh);
        ::GetPar("BACK_GROUND_PIC_ZOOMVIEW_"+sClassName+"_KEEPRATIO",m_bImgBackKeepRatio);
    }
}

void CFHZoomView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
    CMenu *pMenu=m_menuManager.GetPopupMenu();
    if(pMenu)
    {
        m_menuManager.ShowPopupMenu(pMenu,point,this);
    }
}

void CFHZoomView::OnSize(UINT nType, int cx, int cy)
{
    CView::OnSize(nType, cx, cy);
    if (m_nMapMode == ULT_SCALETOFIT)
    {
        // force recalculation of scale to fit parameters
        SetScaleToFitSize(m_totalLog);
    }
    else
    {
        // UpdateBars() handles locking out recursion
        UpdateBars();
    }
}

CSize CFHZoomView::GetClientPageSize(CSize sizeClient)
{
    int cx = (sizeClient.cx-m_nBarWidth-m_nEdgeWidth*2)/m_totalDev.cx * m_sizeScrollBar.cx;		
    int cy = (sizeClient.cy-m_nBarWidth-m_nEdgeWidth*2)/m_totalDev.cy * m_sizeScrollBar.cy;
    return CSize(cx,cy);
}

void CFHZoomView::Rectangle(CDC *pDC, double x1,double y1,double x2,double y2)
{
    LPtoWorld(x1,y1);
    LPtoWorld(x2,y2);

    CMyRect r1(x1,y1,x2,y2);

    CRect r;
    GetClientRect(&r);

    CMyRect r2(r);

    CMyRect rect=r1.Intersect (r2);

    if(rect!=CMyRect())
    {
        pDC->Rectangle (rect.m_dLeft ,rect.m_dLow,rect.m_dRight ,rect.m_dHigh );
    }
}

void  CFHZoomView::EnableInfo (bool bEnable)
{
    m_dlgInfo.m_bCanShowInfo = bEnable;
    m_bInfoCanShowBak=bEnable;
}
void CFHZoomView::GetLogicalRange (double &x1,double &y1,double &x2,double &y2)
{
    x1=m_pointLogOrg.x;
    y1=m_pointLogOrg.y;
    x2=x1+m_totalLog.cx;
    y2=y1+m_totalLog.cy;
}

//  设定细比例线的密度。
bool CFHZoomView::SetMidScaleDensity(long n)
{
    if(n<1||n>100)return false;
    this->m_nMidScaleDensity = n;
    return true;
}


int CFHZoomView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (__super::OnCreate(lpCreateStruct) == -1)
        return -1;

    m_dlgInfo.SetControlWnd (this);	
    m_dlgErrorMsg.SetControlWnd (this);	

    return 0;
}

void CFHZoomView::OnKillFocus(CWnd* pNewWnd)
{
    CView::OnKillFocus(pNewWnd);
    //HideSearchButton(false);
}

LRESULT CFHZoomView::OnWndChange(WPARAM wParam, LPARAM lParam)
{
    HWND hWndCurrent=(HWND)lParam;
    if(hWndCurrent==GetSafeHwnd())
    {
        SetToolBarButtonState();
    }
    return 0;
}
void CFHZoomView::EnableCoor(bool bEnable)
{
    m_bDrawCoor=bEnable;
}

void CFHZoomView::OnUpdateFhzoomviewlast(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_nCurrentPage>0);
}

void CFHZoomView::OnUpdateFhzoomviewnext(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_nCurrentPage< m_nPageQty-1);
}

void CFHZoomView::OnUpdateFhzoomsearch(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(m_nPageQty>1);
}

void CFHZoomView::OnFhzoomsearch()
{
    SearchPage();
}


bool CFHZoomView::CheckPointingChange(double x,double y,double &dMinDistance)
{
    return false;
}

bool CFHZoomView::SetPopupMenu()
{
    m_menuManager.Empty();
    m_menuManager.AppendMenu(ID_FHSettings,     "设置",         MF_MENUITEM,'E');
    m_menuManager.AppendMenu(ID_FHZoomIn,       "放大",         MF_MENUITEM,'Z');
    m_menuManager.AppendMenu(ID_FHZoomOut,      "缩小",         MF_MENUITEM,'D');
    m_menuManager.AppendMenu(ID_FHZoomInHor,    "横向放大",     MF_MENUITEM,'H');
    m_menuManager.AppendMenu(ID_FHZoomOutHor,   "横向缩小",     MF_MENUITEM,'O');
    m_menuManager.AppendMenu(ID_FHZoomInVer,    "纵向放大",     MF_MENUITEM,'V');
    m_menuManager.AppendMenu(ID_FHZoomOutVer,   "纵向缩小",     MF_MENUITEM,'R');
    m_menuManager.AppendMenu(ID_FHZoomFull,     "充满屏幕",     MF_MENUITEM,'F');
    m_menuManager.AppendMenu(ID_FHEnableCor,    "绘制坐标",     MF_MENUITEM,'C');
    m_menuManager.AppendMenu(ID_FHZoomViewNext, "下一项",       MF_MENUITEM,'N');
    m_menuManager.AppendMenu(ID_FHZoomViewLast, "前一项",       MF_MENUITEM,'P');
    m_menuManager.AppendMenu(ID_FHZoomSearch,   "快速查找项",   MF_MENUITEM,'S');
    m_menuManager.AppendMenu(ID_FHEnableInfoWnd,"打开信息窗",   MF_MENUITEM,'I');


    return true;
}

bool CFHZoomView::ShowJPG(  CDC *pDC,
                          CString sFile,
                          CRect rectTarget,
                          bool bKeepSrcScale)
{
    IPicture *pPic;
    IStream *pStm;

    CFileStatus fstatus;
    CFile file;
    LONG cb;

    CRect rectShow(100,100,500,500);

    if(     file.Open(sFile,CFile::modeRead)
        &&  file.GetStatus(sFile,fstatus)
        &&  ((cb=fstatus.m_size )!=-1))
    {
        HGLOBAL hGlobal=GlobalAlloc(GMEM_MOVEABLE,cb);
        LPVOID pvData=NULL;
        if(hGlobal!=NULL)
        {
            if((pvData=GlobalLock(hGlobal))!=NULL)
            {
                file.Read(pvData,cb);
                GlobalUnlock(hGlobal);
                CreateStreamOnHGlobal(hGlobal,TRUE,&pStm);
                if(SUCCEEDED(OleLoadPicture(pStm,fstatus.m_size,TRUE,IID_IPicture,(LPVOID*)&pPic)))
                {
                    OLE_XSIZE_HIMETRIC hmWidth;
                    OLE_YSIZE_HIMETRIC hmHeight;

                    pPic->get_Width (&hmWidth);
                    pPic->get_Height (&hmHeight);

                    if(bKeepSrcScale)
                    {
                        rectTarget.right =rectTarget.left +
                            rectTarget.Height ()*(hmWidth/hmHeight);
                    }

                    if(FAILED(pPic->Render (*pDC,
                        rectTarget.left ,
                        rectTarget.top, 
                        rectTarget.Width (),
                        rectTarget.Height (),
                        0,
                        hmHeight,
                        hmWidth,
                        -hmHeight,
                        NULL)))
                    {
                        AfxMessageBox("Failed to render the picture!");
                        pPic->Release ();
                        return false;
                    }
                }
                else
                {
                    AfxMessageBox("Error loading picture from stream!");
                    return false;
                }
            }
        }
    }
    else
    {
        AfxMessageBox("Error in opening the picture!");
        return false;
    }

    return true;
}

void CFHZoomView::DrawLine(CDC      *pDC, 
                           CRect    &rect,
                           int      iPenStyle, 
                           int      iPenWidth,
                           COLORREF nColor)
{
    int iNext=0;
    if(iPenStyle==PS_DASH)
    {
        if(rect.Width()==iPenWidth)
        {
            for(int i=rect.bottom;i<=rect.top;)
            {
                iNext=i+iPenWidth*2;
                if(iNext>rect.top)iNext=rect.top;

                pDC->FillSolidRect(rect.left,i,iPenWidth,iNext-i,nColor);
                i+=iPenWidth*3;
            }
        }
        else
        {
            for(int i=rect.left;i<=rect.right;)
            {
                iNext=i+iPenWidth*2;
                if(iNext>rect.right)iNext=rect.right;

                pDC->FillSolidRect(i,rect.bottom,iNext-i,iPenWidth,nColor);
                i+=iPenWidth*3;
            }
        }
    }

    else if(iPenStyle==PS_DOT)
    {
        if(rect.Width()==iPenWidth)
        {
            for(int i=rect.bottom;i<=rect.top;)
            {
                iNext=i+iPenWidth;
                if(iNext>rect.top)iNext=rect.top;

                pDC->FillSolidRect(rect.left,i,iPenWidth,iNext-i,nColor);
                i+=iPenWidth*2;
            }
        }
        else
        {
            for(int i=rect.left;i<=rect.right;)
            {
                iNext=i+iPenWidth;
                if(iNext>rect.right)iNext=rect.right;

                pDC->FillSolidRect(i,rect.bottom,iNext-i,iPenWidth,nColor);
                i+=iPenWidth*2;
            }
        }
    }

    else if(iPenStyle==PS_SOLID)
    {
        pDC->FillSolidRect(rect,nColor);
    }
}

int CFHZoomView::GetPointQty()
{
    return 0;
}

double  CFHZoomView::GetCoor  (int iRgnOrder, int iPointOrder, int iSeries)
{
    return iPointOrder;
}

void CFHZoomView::GetCoorNote(CStringList &listNotes,int iRgnOrder, int iPointOrder, int iSeries)
{
    listNotes.AddHead(vtos(iPointOrder));
    return;
}

void CFHZoomView::SetCoorStyle(EZoomViewCoorStyle eCoorStyle)
{
    m_eCoorStyle=eCoorStyle;
}

void  CFHZoomView::DrawPointOfType( CDC *pDC,
                                   double x,
                                   double y,
                                   EPointDrawPattern eType,
                                   int iPointSize)
{
    int iHalfPointSize=iPointSize/2.0;

    switch(eType)
    {
    case eDP_Ellipse:
        pDC->Ellipse(x-iHalfPointSize,y-iHalfPointSize,x+iHalfPointSize,y+iHalfPointSize);
        break;

    case eDP_Rectangle:
        pDC->Rectangle(x-iHalfPointSize,y-iHalfPointSize,x+iHalfPointSize,y+iHalfPointSize);
        break;

    case eDP_Triangle:
        {
            int iTriAngleOffsetL=iPointSize/3.0*2;
            int iTriAngleOffsetS=iTriAngleOffsetL/2.0;
            int iTriAngleOffsetA=iTriAngleOffsetS*1.732;
            pDC->MoveTo(x,y-iTriAngleOffsetL);
            pDC->LineTo(x-iTriAngleOffsetA,y+iTriAngleOffsetS);
            pDC->LineTo(x+iTriAngleOffsetA,y+iTriAngleOffsetS);
            pDC->LineTo(x,y-iTriAngleOffsetL);
        }
        break;

    case eDP_Cross:
        pDC->MoveTo(x,y-iHalfPointSize);
        pDC->LineTo(x,y+iHalfPointSize);
        pDC->MoveTo(x-iHalfPointSize,y);
        pDC->LineTo(x+iHalfPointSize,y);
        break;
    }
}

void  CFHZoomView::CheckAnchoredPoint (double dLogicalX, double dLogicalY)
{

}

void CFHZoomView::MoveAnchoredPoint(double dAddedLogicalX, double dAddedLogicalY)
{

}

void CFHZoomView::OnUpdateFhEnableInfoWnd(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(m_dlgInfo.m_bCanShowInfo);
}

void CFHZoomView::OnUpdateFhEnableCor(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(m_bDrawCoor);
}

CPoint CFHZoomView::GetMousePos()
{
    return m_pointLastMouse;
}

void CFHZoomView::InvalidateInfo()
{
    m_dlgInfo.Show(GetInfo(m_pointCurrentMouse.x,m_pointCurrentMouse.y));
}

void CFHZoomView::SetAxisUnit( CString sXUnit, CString sYUnit)
{
    m_sXUnit    =   sXUnit;
    m_sYUnit    =   sYUnit;
}

void CFHZoomView::CopyViewPar(CFHZoomView  *pView)
{
    m_pointLogOrg   = pView->m_pointLogOrg;
    m_totalDev      = pView->m_totalDev;
    m_totalLog      = pView->m_totalLog;
    m_pointDevOrg   = pView->m_pointDevOrg;
    m_nBarWidth     = pView->m_nBarWidth;
    m_nEdgeWidth    = pView->m_nEdgeWidth;
    
    m_pointLogOrgBak= pView->m_pointLogOrgBak;
    m_bUpIncrease   = pView->m_bUpIncrease;

    m_pageDev       = pView->m_pageDev;                                  // 滚动页的大小
    m_lineDev       = pView->m_lineDev;                                  // 滚动行的大小
    m_sizeScrollBar = pView->m_sizeScrollBar;
    m_bDevSizeLimit = pView->m_bDevSizeLimit;                            // 是否限制设备大小
    m_bCenter       = pView->m_bCenter;                                  // 窗口居中
    m_bInsideUpdate = pView->m_bInsideUpdate;                            // OnSize的内部回访状态

    m_dCooeHor      = pView->m_dCooeHor;                                 // 水平方向的坐标缩放系数
    m_dCooeVer      = pView->m_dCooeVer;                                 // 垂直方向的坐标缩放系数

    m_rectCoor      = pView->m_rectCoor;

    m_nCurrentPage  = pView->m_nCurrentPage;
    m_nPageQty      = pView->m_nPageQty;


    // 与标题有关的参数
    m_bDrawTitle    = pView->m_bDrawTitle;
    memcpy(&m_logfontTitle,&(pView->m_logfontTitle),sizeof(LOGFONT));
    m_colorTitle    = pView->m_colorTitle;
    m_sDrawTitle    = pView->m_sDrawTitle;
    m_sError        = pView->m_sError;

    // 与打印有关的参数
    m_bPrintByScale = pView->m_bPrintByScale;
    m_totalDevBak   = pView->m_totalDevBak; // 打印之前存储设备的大小

    m_bDrawCoor     = pView->m_bDrawCoor;
    m_bDrawBar      = pView->m_bDrawBar;

    m_nCoorType     = pView->m_nCoorType;
    memcpy(&m_logfontCoor,&(pView->m_logfontCoor),sizeof(LOGFONT));
    m_colorThickCoorHor = pView->m_colorThickCoorHor;
    m_colorThinCoorHor  = pView->m_colorThinCoorHor;
    m_colorThickCoorVer = pView->m_colorThickCoorVer;
    m_colorThinCoorVer  = pView->m_colorThinCoorVer;
    m_colorFontCoor     = pView->m_colorFontCoor;
    m_nMidScaleDensity  = pView->m_nMidScaleDensity;

    m_bUseUserCoor      = pView->m_bUseUserCoor;
    m_rectUserThickLine = pView->m_rectUserThickLine;
    m_rectUserThinLine  = pView->m_rectUserThinLine;
    m_sizeUserThickStep = pView->m_sizeUserThickStep;
    m_sizeUserThinStep  = pView->m_sizeUserThinStep;

    m_iContentSize      = pView->m_iContentSize;
    m_iContentCurrSize  = pView->m_iContentCurrSize;

    // 与图中
    memcpy(&m_logfontMapChar,&(pView->m_logfontMapChar),sizeof(LOGFONT));

    m_colorMapChar      = pView->m_colorMapChar;
    memcpy(&m_logfontCurrChar,&(pView->m_logfontCurrChar),sizeof(LOGFONT));

    m_colorCurrChar     = pView->m_colorCurrChar;

    LOGFONT lf;
    pView->m_fontTextCommon.GetLogFont(&lf);
    m_fontTextCommon.DeleteObject();
    m_fontTextCommon.CreateFontIndirect(&lf);


    pView->m_fontTextCurrent.GetLogFont(&lf);
    m_fontTextCurrent.DeleteObject();
    m_fontTextCurrent.CreateFontIndirect(&lf);

    m_nFont             = pView->m_nFont;                // =90
    
    // 与信息窗有关的函数
    m_bEnableInfoTimer  = pView->m_bEnableInfoTimer;

    m_bInfoOnShow       = pView->m_bInfoOnShow;
    m_timeInterval      = pView->m_timeInterval;
    m_timeToShowInfo    = pView->m_timeToShowInfo;
    m_timeMouseInSamePlace  = pView->m_timeMouseInSamePlace;
    m_pointLastMouse        = pView->m_pointLastMouse;
    m_pointCurrentMouse     = pView->m_pointCurrentMouse;
    m_bInfoCanShowBak       = pView->m_bInfoCanShowBak;

    m_pointZoomAreaAnchor   = pView->m_pointZoomAreaAnchor;
    m_bZoomAreaAnchored     = pView->m_bZoomAreaAnchored;
    
    LOGPEN lp;
    m_penZoomRect.DeleteObject();
    pView->m_penZoomRect.GetLogPen(&lp);
    m_penZoomRect.CreatePenIndirect(&lp);


    m_bMouseAnchored        = pView->m_bMouseAnchored;
    m_pointMouseAnchorPoint = pView->m_pointMouseAnchorPoint;

    ///////////////////////////////////
    // 与错误信息窗有关的函数
    ///////////////////////////////////
    m_bErrorCanShowBak      = pView->m_bErrorCanShowBak;
    m_nFirstShow            = pView->m_nFirstShow;

    ///////////////////////////////////
    // 为继承类预设图形颜色：
    ///////////////////////////////////
    m_colorMapContent       = pView->m_colorMapContent;
    m_colorMapContentSpecial= pView->m_colorMapContentSpecial;

    m_sXUnit                = pView->m_sXUnit;
    m_sYUnit                = pView->m_sYUnit;

    ///////////////////////////////////
    // Cursors:
    ///////////////////////////////////
    m_hCursorZoomIn         = pView->m_hCursorZoomIn;
    m_hCursorZoomOut        = pView->m_hCursorZoomOut;
    m_hCursorDefault        = pView->m_hCursorDefault;

    m_eZoomState            = pView->m_eZoomState;                            // 放大标识
    m_nMapMode              = pView->m_nMapMode;                            // 映射方式

}

int CFHZoomView::GetContentSize()
{
    return m_iContentSize;
}

int CFHZoomView::GetCurrentContentSize()
{
    return m_iContentCurrSize;
}


void CFHZoomView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    KillTimer(m_nMouseLBDownTimer);

    CView::OnLButtonDblClk(nFlags, point);
}

void CFHZoomView::SetScaleToFitSize(CMySize sizeTotal)
{
    // Note: It is possible to set sizeTotal members to negative values to
    //  effectively invert either the X or Y axis.

    ASSERT(m_hWnd != NULL);
    //m_nMapMode = MM_SCALETOFIT;     // special internal value
    m_totalLog = sizeTotal;

    // reset and turn any scroll bars off
    if (m_hWnd != NULL && (GetStyle() & (WS_HSCROLL|WS_VSCROLL)))
    {
        SetMyScrollPos(SB_HORZ, 0);
        SetMyScrollPos(SB_VERT, 0);
        EnableScrollBarCtrl(SB_BOTH, FALSE);
        ASSERT((GetStyle() & (WS_HSCROLL|WS_VSCROLL)) == 0);
    }

    CRect rectT;
    GetClientRect(rectT);
    CSize size= rectT.Size();
    m_totalDev.cx =size.cx ;
    m_totalDev.cy =size.cy ;


    if (m_hWnd != NULL)
    {
        // window has been created, invalidate
        UpdateBars();
        Invalidate(TRUE);
    }
}


void CFHZoomView::SetScrollPage(CMySize &sizePage)
{
    m_pageDev=sizePage;
    if (m_pageDev.cy < 0)
        m_pageDev.cy = -m_pageDev.cy;

}
void CFHZoomView::SetScrollLine(CMySize &sizeLine)
{
    m_lineDev=sizeLine;
    if (m_lineDev.cy < 0)
        m_lineDev.cy = -m_lineDev.cy;
}

/////////////////////////////////////////////////////////////////////////////
// Getting information

CMyPoint CFHZoomView::GetMyScrollPos()   // logical coordinates
{
    return GetDeviceScrollPosition();
}

///////////////////////////////////////////////
//             滚动到逻辑位置
///////////////////////////////////////////////
void CFHZoomView::ScrollToPosition(CMyPoint pt)    // logical coordinates
{
    ASSERT(m_nMapMode > 0);     // not allowed for shrink to fit

    LPtoWorld(pt);

    // now in device coordinates - limit if out of range
    double xMax = GetMyScrollLimit(SB_HORZ);
    double yMax = GetMyScrollLimit(SB_VERT);

    if (pt.x < 0)
        pt.x = 0;
    else if (pt.x > xMax)
        pt.x = xMax;

    if (pt.y < 0)
        pt.y = 0;
    else if (pt.y > yMax)
        pt.y = yMax;

    ScrollToDevicePosition(pt);
}

CMyPoint CFHZoomView::GetDeviceScrollPosition()
{
    return m_pointDevOrg;

    /* I used these lines Before ,
    but for unity, I descarded them:

    double x=GetMyScrollPos(SB_HORZ);
    double y=GetMyScrollPos(SB_VERT);

    CMyPoint pt(x,y);
    ASSERT(pt.x >= 0 && pt.y >= 0);
    return pt;
    */
}

void CFHZoomView::GetDeviceScrollSizes(int& nMapMode, CMySize& sizeTotal,
                                       CMySize& sizePage, CMySize& sizeLine) const
{
    if (m_nMapMode <= 0)
    {
        TRACE(traceAppMsg, 0, "Warning: CFHZoomView::GetDeviceScrollSizes returning invalid mapping mode.\n");
    }

    nMapMode = m_nMapMode;
    sizeTotal = m_totalDev;
    sizePage = m_pageDev;
    sizeLine = m_lineDev;
}

void CFHZoomView::ScrollToDevicePosition(CMyPoint ptDev)
{
    ASSERT(ptDev.x >= 0);
    ASSERT(ptDev.y >= 0);

    SetMyScrollPos(SB_HORZ, ptDev.x);
    SetMyScrollPos(SB_VERT, ptDev.y);
}

CSize CFHZoomView::GetWheelScrollDistance(CSize sizeDistance, BOOL bHorz, BOOL bVert)
{
    CSize sizeRet;

    if (bHorz)
        sizeRet.cx = sizeDistance.cx / 10;
    else
        sizeRet.cx = 0;

    if (bVert)
        sizeRet.cy = sizeDistance.cy / 10;
    else
        sizeRet.cy = 0;

    return sizeRet;
}

/////////////////////////////////////////////////////////////////////////////
// Other helpers

void CFHZoomView::FillOutsideRect(CDC* pDC, CBrush* pBrush)
{
    ASSERT_VALID(pDC);
    ASSERT_VALID(pBrush);

    // fill rect outside the image
    CRect rect;
    GetClientRect(rect);
    ASSERT(rect.left == 0 && rect.top == 0);
    rect.left = m_totalDev.cx;
    if (!rect.IsRectEmpty())
        pDC->FillRect(rect, pBrush);    // vertical strip along the side
    rect.left = 0;
    rect.right = m_totalDev.cx;
    rect.top = m_totalDev.cy;
    if (!rect.IsRectEmpty())
        pDC->FillRect(rect, pBrush);    // horizontal strip along the bottom
}

void CFHZoomView::ResizeParentToFit(BOOL bShrinkOnly)
{
    // adjust parent rect so client rect is appropriate size

    // determine current size of the client area as if no scrollbars present
    CRect rectClient;
    GetWindowRect(rectClient);
    CRect rect = rectClient;
    CalcWindowRect(rect);
    rectClient.left += rectClient.left - rect.left;
    rectClient.top += rectClient.top - rect.top;
    rectClient.right -= rect.right - rectClient.right;
    rectClient.bottom -= rect.bottom - rectClient.bottom;
    rectClient.OffsetRect(-rectClient.left, -rectClient.top);
    ASSERT(rectClient.left == 0 && rectClient.top == 0);

    // determine desired size of the view
    CRect rectView(0, 0, m_totalDev.cx, m_totalDev.cy);
    if (bShrinkOnly)
    {
        if (rectClient.right <= m_totalDev.cx)
            rectView.right = rectClient.right;
        if (rectClient.bottom <= m_totalDev.cy)
            rectView.bottom = rectClient.bottom;
    }
    CalcWindowRect(rectView, CWnd::adjustOutside);
    rectView.OffsetRect(-rectView.left, -rectView.top);
    ASSERT(rectView.left == 0 && rectView.top == 0);
    if (bShrinkOnly)
    {
        if (rectClient.right <= m_totalDev.cx)
            rectView.right = rectClient.right;
        if (rectClient.bottom <= m_totalDev.cy)
            rectView.bottom = rectClient.bottom;
    }

    // dermine and set size of frame based on desired size of view
    CRect rectFrame;
    CFrameWnd* pFrame = GetParentFrame();
    ASSERT_VALID(pFrame);
    pFrame->GetWindowRect(rectFrame);
    CSize size = rectFrame.Size();
    size.cx += rectView.right - rectClient.right;
    size.cy += rectView.bottom - rectClient.bottom;
    pFrame->SetWindowPos(NULL, 0, 0, size.cx, size.cy,
        SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

}

/////////////////////////////////////////////////////////////////////////////

LRESULT CFHZoomView::HandleMButtonDown(WPARAM wParam, LPARAM lParam)
{
    UINT nFlags = static_cast<UINT>(wParam);
    CPoint point(lParam);

    // if the user has CTRL or SHIFT down, we certainly
    // do not handle the message

    if (nFlags & (MK_SHIFT | MK_CONTROL))
    {
        CView::OnMButtonDown(nFlags, point);
        return FALSE;
    }

    // Otherwise, we actually have scrolling work to do.
    // See if we have a wheel mouse...

    BOOL bSupport = FALSE;

    // if it's Win98, we might still have system metric
    if (!bSupport)
    {
        // WinNT-- just ask the system
        bSupport = ::GetSystemMetrics(SM_MOUSEWHEELPRESENT);
    }

    //  If not a wheel mouse, the middle button is really the
    //  middle button and not the wheel; the application should've
    //  handled it.

    if (!bSupport)
        CView::OnMButtonDown(nFlags, point);
    else
    {
        if (m_pAnchorWindow == NULL)
        {
            BOOL bVertBar;
            BOOL bHorzBar;
            CheckScrollBars(bHorzBar, bVertBar);

            UINT nBitmapID = 0;

            // based on which scrollbars we have, figure out which
            // anchor cursor to use for the anchor window
            if (bVertBar)
            {
                if (bHorzBar)
                    nBitmapID = AFX_IDC_MOUSE_ORG_HV;
                else
                    nBitmapID = AFX_IDC_MOUSE_ORG_VERT;
            }
            else if (bHorzBar)
                nBitmapID = AFX_IDC_MOUSE_ORG_HORZ;

            // if there's no scrollbars, we don't do anything!
            if (nBitmapID == 0)
            {
                CView::OnMButtonDown(nFlags, point);
                return FALSE;
            }
            else
            {
                m_pAnchorWindow = new CMouseAnchorWnd(point);
                m_pAnchorWindow->SetBitmap(nBitmapID);
                m_pAnchorWindow->Create(this);
                m_pAnchorWindow->ShowWindow(SW_SHOWNA);
            }
        }
        else
        {
            m_pAnchorWindow->DestroyWindow();
            delete m_pAnchorWindow;
            m_pAnchorWindow = NULL;
        }
    }

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Scrolling Helpers

void CFHZoomView::CenterOnPoint(CMyPoint ptCenter) // center in device coords
{
    CRect rect;
    GetClientRect(&rect);           // find size of client window

    double xDesired = ptCenter.x - rect.Width() / 2;
    double yDesired = ptCenter.y - rect.Height() / 2;

    DWORD dwStyle = GetStyle();

    if ((dwStyle & WS_HSCROLL) == 0 || xDesired < 0)
    {
        xDesired = 0;
    }
    else
    {
        int xMax = GetMyScrollPos(SB_HORZ);
        if (xDesired > xMax)
            xDesired = xMax;
    }

    if ((dwStyle & WS_VSCROLL) == 0 || yDesired < 0)
    {
        yDesired = 0;
    }
    else
    {
        double yMax = GetMyScrollPos(SB_VERT);
        if (yDesired > yMax)
            yDesired = yMax;
    }

    ASSERT(xDesired >= 0);
    ASSERT(yDesired >= 0);

    SetScrollPos(SB_HORZ, xDesired);
    SetScrollPos(SB_VERT, yDesired);
}

/////////////////////////////////////////////////////////////////////////////
// Tie to scrollbars and CWnd behaviour

void CFHZoomView::GetScrollBarSizes(CSize& sizeSb)
{
    sizeSb.cx = sizeSb.cy = 0;
    DWORD dwStyle = GetStyle();

            // fanhua
    /*
    if (GetScrollBarCtrl(SB_VERT) == NULL)
    {
        // vert scrollbars will impact client area of this window
        sizeSb.cx = afxData.cxVScroll;
        if (dwStyle & WS_BORDER)
            sizeSb.cx -= 1; //CX_BORDER;
    }
    if (GetScrollBarCtrl(SB_HORZ) == NULL)
    {
        // horz scrollbars will impact client area of this window
        sizeSb.cy = afxData.cyHScroll;

        sizeSb.cy = afxData.cyHScroll;
        sizeSb.cy = afxData.cyHScroll;
        if (dwStyle & WS_BORDER)
            sizeSb.cy -= 1;//CY_BORDER;
    }
    */
}

BOOL CFHZoomView::GetTrueClientSize(CSize& size, CSize& sizeSb)
// return TRUE if enough room to add scrollbars if needed
{
    CRect rect;
    GetClientRect(&rect);
    ASSERT(rect.top == 0 && rect.left == 0);
    size.cx = rect.right;
    size.cy = rect.bottom;
    DWORD dwStyle = GetStyle();


    // first get the size of the scrollbars for this window
    GetScrollBarSizes(sizeSb);

    // first calculate the size of a potential scrollbar
    // (scroll bar controls do not get turned on/off)
    if (sizeSb.cx != 0 && (dwStyle & WS_VSCROLL))
    {
        // vert scrollbars will impact client area of this window
        size.cx += sizeSb.cx;   // currently on - adjust now
    }
    if (sizeSb.cy != 0 && (dwStyle & WS_HSCROLL))
    {
        // horz scrollbars will impact client area of this window
        size.cy += sizeSb.cy;   // currently on - adjust now
    }

    // return TRUE if enough room
    return (size.cx > sizeSb.cx && size.cy > sizeSb.cy);
}

// helper to return the state of the scrollbars without actually changing
//  the state of the scrollbars
void CFHZoomView::GetScrollBarState(CSize sizeClient, 
                                    CSize& needSb,
                                    CSize& sizeRange, 
                                    CMyPoint& ptMove, 
                                    BOOL bInsideClient)
{
    // get scroll bar sizes (the part that is in the client area)
    CSize sizeSb;
    GetScrollBarSizes(sizeSb);

    // enough room to add scrollbars
    sizeRange = m_sizeScrollBar-GetClientPageSize(sizeClient);
    //sizeRange = m_totalDev - sizeClient;
    // > 0 => need to scroll
    ptMove = GetDeviceScrollPosition();
    // point to move to (start at current scroll pos)

    BOOL bNeedH = sizeRange.cx > 0;   
    if (!bNeedH)
        ptMove.x = 0;                       // jump back to origin
    else if (bInsideClient)
        sizeRange.cy += sizeSb.cy;          // need room for a scroll bar

    BOOL bNeedV = sizeRange.cy > 0;
    if (!bNeedV)
        ptMove.y = 0;                       // jump back to origin
    else if (bInsideClient)
        sizeRange.cx += sizeSb.cx;          // need room for a scroll bar

    if (bNeedV && !bNeedH && sizeRange.cx > 0)
    {
        ASSERT(bInsideClient);
        // need a horizontal scrollbar after all
        bNeedH = TRUE;
        sizeRange.cy += sizeSb.cy;
    }

    // if current scroll position will be past the limit, scroll to limit
    /*if (sizeRange.cx > 0 && ptMove.x >= sizeRange.cx)
    ptMove.x = sizeRange.cx;
    if (sizeRange.cy > 0 && ptMove.y >= sizeRange.cy)
    ptMove.y = sizeRange.cy;
    */

    // now update the bars as appropriate
    needSb.cx = bNeedH;
    needSb.cy = bNeedV;

    // needSb, sizeRange, and ptMove area now all updated
}

void CFHZoomView::UpdateBars()
{
    // UpdateBars may cause window to be resized - ignore those resizings
    if (m_bInsideUpdate)
    {
        return;         // Do not allow recursive calls
    }

    // Lock out recursion
    m_bInsideUpdate = true;

    if(m_totalDev.cx <= 0 || m_totalDev.cy <=0)return ;

    // update the horizontal to reflect reality
    // NOTE: turning on/off the scrollbars will cause 'OnSize' callbacks
    ASSERT(m_totalDev.cx >= 0 && m_totalDev.cy >= 0);

    CRect rectClient;
    BOOL bCalcClient = TRUE;

    // allow parent to do inside-out layout first
    CWnd* pParentWnd = GetParent();
    if (pParentWnd != NULL)
    {
        // if parent window responds to this message, use just
        //  client area for scroll bar calc -- not "true" client area
        if ((BOOL)pParentWnd->SendMessage(WM_RECALCPARENT, 0,
            (LPARAM)(LPCRECT)&rectClient) != 0)
        {
            // use rectClient instead of GetTrueClientSize for
            //  client size calculation.
            bCalcClient = FALSE;
        }

    }

    CSize sizeClient;
    CSize sizeSb;

    if (bCalcClient)
    {
        // get client rect
        if (!GetTrueClientSize(sizeClient, sizeSb))
        {
            // no room for scroll bars (common for zero sized elements)
            CRect rect;
            GetClientRect(&rect);
            if (rect.right > 0 && rect.bottom > 0)
            {
                // if entire client area is not invisible, assume we have
                //  control over our scrollbars
                EnableScrollBarCtrl(SB_BOTH, FALSE);
            }
            m_bInsideUpdate = FALSE;
            return;
        }
    }
    else
    {
        // let parent window determine the "client" rect
        GetScrollBarSizes(sizeSb);
        sizeClient.cx = rectClient.right - rectClient.left;
        sizeClient.cy = rectClient.bottom - rectClient.top;
    }

    // enough room to add scrollbars
    CSize sizeRange;
    CMyPoint ptMove;
    CSize needSb;

    // get the current scroll bar state given the true client area
    GetScrollBarState(sizeClient, needSb, sizeRange, ptMove, bCalcClient);
    if (needSb.cx)
        sizeClient.cy -= sizeSb.cy;
    if (needSb.cy)
        sizeClient.cx -= sizeSb.cx;

    // first scroll the window as needed
    ScrollToDevicePosition(ptMove); // will set the scroll bar positions too

    // this structure needed to update the scrollbar page range
    SCROLLINFO info;
    info.fMask = SIF_PAGE|SIF_RANGE;
    info.nMin = 0;

    // now update the bars as appropriate
    EnableScrollBarCtrl(SB_HORZ, needSb.cx);
    CSize sizePage=GetClientPageSize(sizeClient);

    if (needSb.cx)
    {
        info.nMax  = m_sizeScrollBar.cx;
        info.nPage = sizePage.cx ;

        if (!SetScrollInfo(SB_HORZ, &info, TRUE))
            SetScrollRange(SB_HORZ, 0, sizeRange.cx, TRUE);
    }

    EnableScrollBarCtrl(SB_VERT, needSb.cy);

    if (needSb.cy)
    {
        info.nMax  = m_sizeScrollBar.cy;
        info.nPage = sizePage.cy;

        if (!SetScrollInfo(SB_VERT, &info, TRUE))
            SetScrollRange(SB_VERT, 0, sizeRange.cy, TRUE);
    }

    // remove recursion lockout
    m_bInsideUpdate = FALSE;

}

void CFHZoomView::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType)
{
    if (nAdjustType == adjustOutside)
    {
        // allow for special client-edge style
        ::AdjustWindowRectEx(lpClientRect, 0, FALSE, GetExStyle());

        if (m_nMapMode != ULT_SCALETOFIT)
        {
            // if the view is being used in-place, add scrollbar sizes
            //  (scollbars should appear on the outside when in-place editing)
            CMySize sizeClient(
                lpClientRect->right - lpClientRect->left,
                lpClientRect->bottom - lpClientRect->top);

            CMySize sizeRange = m_totalDev - sizeClient;
            // > 0 => need to scroll

            // get scroll bar sizes (used to adjust the window)
            CSize sizeSb;
            GetScrollBarSizes(sizeSb);

            // adjust the window size based on the state
            if (sizeRange.cy > 0)
            {   // vertical scroll bars take up horizontal space
                lpClientRect->right += sizeSb.cx;
            }
            if (sizeRange.cx > 0)
            {   // horizontal scroll bars take up vertical space
                lpClientRect->bottom += sizeSb.cy;
            }
        }
    }
    else
    {
        // call default to handle other non-client areas
        ::AdjustWindowRectEx(lpClientRect, GetStyle(), FALSE,
            GetExStyle() & ~(WS_EX_CLIENTEDGE));
    }
}

/////////////////////////////////////////////////////////////////////////////
// CFHZoomView scrolling

void CFHZoomView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    if (pScrollBar != NULL && pScrollBar->SendChildNotifyLastMsg())
        return;     // eat it

    // ignore scroll bar msgs from other controls
    if (pScrollBar != GetScrollBarCtrl(SB_HORZ))
        return;

    OnScroll(MAKEWORD(nSBCode, 0xff), nPos);
}

void CFHZoomView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    if (pScrollBar != NULL && pScrollBar->SendChildNotifyLastMsg())
        return;     // eat it

    // ignore scroll bar msgs from other controls
    if (pScrollBar != GetScrollBarCtrl(SB_VERT))
        return;

    OnScroll(MAKEWORD(0xff, nSBCode), nPos);
}

BOOL CFHZoomView::OnMouseWheel(UINT fFlags, short zDelta, CPoint point)
{
    // we don't handle anything but scrolling
    if (fFlags & (MK_SHIFT | MK_CONTROL))
        return FALSE;

    return DoMouseWheel(fFlags, zDelta, point);
}

void CFHZoomView::CheckScrollBars(BOOL& bHasHorzBar, BOOL& bHasVertBar) 
{
    DWORD dwStyle = GetStyle();
    CScrollBar* pBar = GetScrollBarCtrl(SB_VERT);
    bHasVertBar = ((pBar != NULL) && pBar->IsWindowEnabled()) ||
        (dwStyle & WS_VSCROLL);

    pBar = GetScrollBarCtrl(SB_HORZ);
    bHasHorzBar = ((pBar != NULL) && pBar->IsWindowEnabled()) ||
        (dwStyle & WS_HSCROLL);
}

// This function isn't virtual. If you need to override it,
// you really need to override OnMouseWheel() here or in
// CSplitterWnd.

BOOL CFHZoomView::DoMouseWheel(UINT fFlags, short zDelta, CPoint point)
{
    UNUSED_ALWAYS(point);
    UNUSED_ALWAYS(fFlags);

    // if we have a vertical scroll bar, the wheel scrolls that
    // if we have _only_ a horizontal scroll bar, the wheel scrolls that
    // otherwise, don't do any work at all

    BOOL bHasHorzBar, bHasVertBar;
    CheckScrollBars(bHasHorzBar, bHasVertBar);
    if (!bHasVertBar && !bHasHorzBar)
        return FALSE;

    BOOL bResult = FALSE;
    UINT uWheelScrollLines =3; // _AfxGetMouseScrollLines();
    int nToScroll = ::MulDiv(-zDelta, uWheelScrollLines, WHEEL_DELTA);
    int nDisplacement;

    if (bHasVertBar)
    {
        if (uWheelScrollLines == WHEEL_PAGESCROLL)
        {
            nDisplacement = m_pageDev.cy;
            if (zDelta > 0)
                nDisplacement = -nDisplacement;
        }
        else
        {
            nDisplacement = nToScroll * m_lineDev.cy;
            if(nDisplacement >0)
            {
                nDisplacement = min(nDisplacement, m_pageDev.cy);
            }
            else
            {
                nDisplacement = max(nDisplacement, -m_pageDev.cy);
            }

        }
        bResult = OnMyScrollBy(CMySize(0, nDisplacement), TRUE);
    }
    else if (bHasHorzBar)
    {
        if (uWheelScrollLines == WHEEL_PAGESCROLL)
        {
            nDisplacement = m_pageDev.cx;
            if (zDelta > 0)
                nDisplacement = -nDisplacement;
        }
        else
        {
            nDisplacement = nToScroll * m_lineDev.cx;

            if(nDisplacement >0)
            {
                nDisplacement = min(nDisplacement, m_pageDev.cx);
            }
            else
            {
                nDisplacement = max(nDisplacement, -m_pageDev.cx);
            }
        }
        bResult = OnMyScrollBy(CMySize(nDisplacement, 0), TRUE);
    }

    if (bResult)
        UpdateWindow();


    return bResult; 

}
BOOL CFHZoomView::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll)
{
    double dPos=0;
    switch (LOBYTE(nScrollCode))
    {
    case SB_THUMBTRACK:
        dPos=GetMyDevPosition(SB_HORZ,nPos);
    }

    switch (HIBYTE(nScrollCode))
    {
    case SB_THUMBTRACK:
        dPos=GetMyDevPosition(SB_VERT,nPos);
    }

    return OnMyScroll(nScrollCode, dPos, bDoScroll);
}

BOOL CFHZoomView::OnMyScroll(UINT nScrollCode, double dPos, BOOL bDoScroll)
{
    // calc new x position
    double x = GetMyScrollPos(SB_HORZ);
    double xOrig = x;

    switch (LOBYTE(nScrollCode))
    {
    case SB_TOP:
        x = 0;
        break;
    case SB_BOTTOM:
        x = m_totalDev.cx ;
        break;
    case SB_LINEUP:
        x -= m_lineDev.cx;
        break;
    case SB_LINEDOWN:
        x += m_lineDev.cx;
        break;
    case SB_PAGEUP:
        x -= m_pageDev.cx;
        break;
    case SB_PAGEDOWN:
        x += m_pageDev.cx;
        break;
    case SB_THUMBTRACK:
        x = dPos;
        break;
    }

    // calc new y position
    double y = GetMyScrollPos(SB_VERT);
    double yOrig = y;

    switch (HIBYTE(nScrollCode))
    {
    case SB_TOP:
        y = 0;
        break;
    case SB_BOTTOM:
        y = m_totalDev.cy ;
        break;
    case SB_LINEUP:
        y -= m_lineDev.cy;
        break;
    case SB_LINEDOWN:
        y += m_lineDev.cy;
        break;
    case SB_PAGEUP:
        y -= m_pageDev.cy;
        break;
    case SB_PAGEDOWN:
        y += m_pageDev.cy;
        break;
    case SB_THUMBTRACK:
        y = dPos;
        break;
    }

    BOOL bResult = OnMyScrollBy(CMySize(x - xOrig, y - yOrig), bDoScroll);
    if (bResult && bDoScroll)
        UpdateWindow();

    return bResult;
}

BOOL CFHZoomView::OnScrollBy(CSize sizeScroll, BOOL bDoScroll)
{
    double dcx=GetMyDevPosition(SB_HORZ,(double)sizeScroll.cx );
    double dcy=GetMyDevPosition(SB_VERT,(double)sizeScroll.cy );

    return OnMyScrollBy(CMySize(dcx,dcy),bDoScroll);
}

BOOL CFHZoomView::OnMyScrollBy(CMySize sizeScroll, BOOL bDoScroll)
{
    double xOrig, x;
    double yOrig, y;

    // don't scroll if there is no valid scroll range (ie. no scroll bar)
    CScrollBar* pBar;
    DWORD dwStyle = GetStyle();
    pBar = GetScrollBarCtrl(SB_VERT);
    if ((pBar != NULL && !pBar->IsWindowEnabled()) ||
        (pBar == NULL && !(dwStyle & WS_VSCROLL)))
    {
        // vertical scroll bar not enabled
        sizeScroll.cy = 0;
    }
    pBar = GetScrollBarCtrl(SB_HORZ);
    if ((pBar != NULL && !pBar->IsWindowEnabled()) ||
        (pBar == NULL && !(dwStyle & WS_HSCROLL)))
    {
        // horizontal scroll bar not enabled
        sizeScroll.cx = 0;
    }

    // adjust current x position
    xOrig = x = GetMyScrollPos(SB_HORZ);
    double xMax = GetMyScrollLimit(SB_HORZ);
    x += sizeScroll.cx;
    if (x < 0)
        x = 0;
    else if (x > xMax)
        x = xMax;

    // adjust current y position
    yOrig = y = GetMyScrollPos(SB_VERT);
    double yMax = GetMyScrollLimit(SB_VERT);
    y += sizeScroll.cy;
    if (y < 0)
        y = 0;
    else if (y > yMax)
        y = yMax;

    // did anything change?
    if (x == xOrig && y == yOrig)
    {
        Invalidate();
        return FALSE;
    }

    if (bDoScroll)
    {
        ////////////////////////////////////////////////////
        // ScrollWindow 移动客户区的内容. 重绘新增加的部分.
        // 由于其管理的范围只限在int范围，而且无法重载（？），
        // 所以忽略之。
        // ScrollWindow(-(x-xOrig), -(y-yOrig));
        ////////////////////////////////////////////////////
        if (x != xOrig)
            SetMyScrollPos(SB_HORZ, x);
        if (y != yOrig)
            SetMyScrollPos(SB_VERT, y);

    }
    Invalidate();
    return TRUE;
}

void CFHZoomView::LPtoWorld(double &x, double &y)
{
    LPtoWorldX(x);
    LPtoWorldY(y);
}

void CFHZoomView::WorldtoLP(double &x, double &y)
{
    WorldtoLPX(x);
    WorldtoLPY(y);
}

void CFHZoomView::LPtoWorld(CMyPoint &pt)
{
    LPtoWorldX(pt.x);
    LPtoWorldY(pt.y);
}

void CFHZoomView::WorldtoLP(CMyPoint &pt)
{
    WorldtoLPX(pt.x);
    WorldtoLPY(pt.y);
}

void CFHZoomView::LPtoWorldM(CMyPoint *pPoint,long nPoint)
{
    for(long i=0;i<nPoint;i++)
    {
        LPtoWorldX(pPoint[i].x);
        LPtoWorldY(pPoint[i].y);
    }
}

void CFHZoomView::WorldtoLPM(CMyPoint *pPoint,long nPoint)
{
    for(long i=0;i<nPoint;i++)
    {
        WorldtoLPX(pPoint[i].x);
        WorldtoLPY(pPoint[i].y);
    }
}

void CFHZoomView::LPtoWorldX(double &x)
{
    x=x-m_pointLogOrg.x ;
    x*=m_totalDev.cx/m_totalLog.cx;
    x-=m_pointDevOrg.x ;
    x+=m_nBarWidth;
    x+=m_nEdgeWidth;
}

// For draw many coordinate systems, move the coordinate to upper, and then draw another.
// Before, draw another, you must store the original logical coordinate,
// After drawing the coordinates, you must restore the coordinate system.
void CFHZoomView::StoreCoorOri()
{
    m_pointLogOrgBak=m_pointLogOrg;
}
void CFHZoomView::RestoreCoorOri()
{
    m_pointLogOrg=m_pointLogOrgBak;
}

void CFHZoomView::MoveCoor(double dMoveRight, double dMoveUp)
{
    m_pointLogOrg.x-=dMoveRight;
    m_pointLogOrg.y-=dMoveUp;
}

void CFHZoomView::LPtoWorldY(double &y)
{
    y-=m_pointLogOrg.y ;
    y*=m_totalDev.cy/m_totalLog.cy;

    if(m_bUpIncrease)
    {
        y=m_totalDev.cy-y ;
    }

    y-=m_pointDevOrg.y ;
    y+=m_nBarWidth;
    y+=m_nEdgeWidth;

}

void CFHZoomView::WorldtoLPX(double &x)
{
    x-=m_nBarWidth;
    x-=m_nEdgeWidth;

    x+= m_pointDevOrg.x; 
    x*=m_totalLog.cx/m_totalDev.cx;
    x=x+m_pointLogOrg.x;
}

void CFHZoomView::WorldtoLPY(double &y)
{
    y-=m_nBarWidth;
    y-=m_nEdgeWidth;

    y+= m_pointDevOrg.y;

    if(m_bUpIncrease)
    {
        y=m_totalDev.cy-y;
    }

    y*=m_totalLog.cy/m_totalDev.cy;
    y=y+m_pointLogOrg.y;
}

///////////////////////////////////////////////////////
// 获得客户区开始点的物理坐标
///////////////////////////////////////////////////////
double CFHZoomView::GetMyScrollPos(int nBar)
{
    SCROLLINFO info;
    if(!GetScrollInfo(nBar,&info))
    {
        TRACE(" 错误：无法得到滚动条信息!\n");
        return 0;
    }

    if(nBar==SB_HORZ)
    {
        return m_pointDevOrg.x; 
    }
    else if(nBar==SB_VERT)
    {
        return m_pointDevOrg.y; 
    }

    return 0;
}

///////////////////////////////////////////////////////
// 设置客户区开始点的物理坐标
///////////////////////////////////////////////////////
void CFHZoomView::SetMyScrollPos(int nBar,double dPos)
{
    CRect rectClient;
    GetClientRect(&rectClient);
    ASSERT(rectClient.left ==0&&rectClient.top ==0);
    double dTW=m_totalDev.cx+m_nEdgeWidth*2+m_nBarWidth*2;
    double dTH=m_totalDev.cy+m_nEdgeWidth*2+m_nBarWidth*2;

    int nPos;
    if(nBar==SB_HORZ)
    {
        m_pointDevOrg.x =dPos;
        if(m_pointDevOrg.x+rectClient.right>dTW)
        {
            m_pointDevOrg.x=dTW-rectClient.right;
        }
        if(m_pointDevOrg.x<0)
        {
            m_pointDevOrg.x=0;
        }

        nPos=GetPhyDevPosition(SB_HORZ,m_pointDevOrg.x);
    }
    else if(nBar==SB_VERT)
    {
        m_pointDevOrg.y =dPos;
        if(m_pointDevOrg.y+rectClient.bottom>dTH)
        {
            m_pointDevOrg.y=dTH-rectClient.bottom;
        }
        if(m_pointDevOrg.y<0)
        {
            m_pointDevOrg.y=0;
        }

        nPos=GetPhyDevPosition(SB_VERT,m_pointDevOrg.y);
    }
    else
    {
        ASSERT(false);
    }

    SetScrollPos(nBar,nPos);
}

double CFHZoomView::GetMyScrollLimit(int nBar)
{
    if(nBar==SB_HORZ)
    {
        return m_totalDev.cx;
    }
    else if(nBar==SB_VERT)
    {
        return m_totalDev.cy;
    }
    else
    {
        ASSERT(false);
    }
    return 0;
}

void CFHZoomView::OnFHZoomIn() 
{
    // TODO: Add your command handler code here
    if(m_eZoomState!=statueZoomIn)
    {
        m_eZoomState=statueZoomIn;
    }
    else
    {
        m_eZoomState=statueIdle;
    }

    SetToolBarButtonState();
}

void CFHZoomView::OnFHZoomOut() 
{
    // TODO: Add your command handler code here
    if(m_eZoomState!=statueZoomOut)
    {
        m_eZoomState=statueZoomOut;
    }
    else
    {
        m_eZoomState=statueIdle;
    }

    SetToolBarButtonState();
}

void CFHZoomView::OnFHZoomIdle() 
{
    m_eZoomState=statueIdle;
    SetToolBarButtonState();
}

void CFHZoomView::OnFHZoomInHor() 
{
    // TODO: Add your command handler code here
    if(m_eZoomState!=statueZoomInHor)
    {
        m_eZoomState=statueZoomInHor;
    }
    
    else
    {
        m_eZoomState=statueIdle;
    }
    SetToolBarButtonState();
}

void CFHZoomView::OnFHZoomInVer() 
{
    // TODO: Add your command handler code here
    if(m_eZoomState!=statueZoomInVer)
    {
        m_eZoomState=statueZoomInVer;
    }
    else
    {
        m_eZoomState=statueIdle;
    }
    SetToolBarButtonState();
}

void CFHZoomView::OnFHZoomOutHor() 
{
    // TODO: Add your command handler code here
    if(m_eZoomState!=statueZoomOutHor)
    {
        m_eZoomState=statueZoomOutHor;
    }
    
    else
    {
        m_eZoomState=statueIdle;
    }
    SetToolBarButtonState();
}

void CFHZoomView::OnFHZoomOutVer() 
{
    // TODO: Add your command handler code here
    if(m_eZoomState!=statueZoomOutVer)
    {
        m_eZoomState=statueZoomOutVer;
    }
    
    else
    {
        m_eZoomState=statueIdle;
    }

    SetToolBarButtonState();
}


void CFHZoomView::OnFHZoomViewLast()
{
    if(m_nCurrentPage>0)
    {
        m_nCurrentPage--;
        UpdatePage();
    }

}
void CFHZoomView::OnFHZoomViewNext()
{
    if(m_nCurrentPage<m_nPageQty-1)
    {
        m_nCurrentPage++;
        UpdatePage();
    }
}

void CFHZoomView::UpdatePage()
{
}

enumZoomStatue CFHZoomView::GetZoomStatue()
{
    return m_eZoomState;
}

void CFHZoomView::SearchPage()
{
    CDlgAskPage dlg;
    dlg.m_dMinValue =0;
    dlg.m_dMaxValue = m_nPageQty-1;
    dlg.m_nPageSelected =m_nCurrentPage;

    if(dlg.DoModal ()==IDCANCEL)return;

    long nBak=m_nCurrentPage;
    m_nCurrentPage=dlg.m_nPageSelected;

    if(m_nCurrentPage<0)
    {
        m_nCurrentPage=0;
    }

    if(m_nCurrentPage>=m_nPageQty)
    {
        m_nCurrentPage=m_nPageQty-1;
    }

    if(m_nCurrentPage!=nBak)
    {
        UpdatePage();
    }
}

bool CFHZoomView::SetPageQty(long nPageQty)
{
    if(nPageQty<1||nPageQty>100000)return false;
    m_nPageQty=nPageQty;
    m_nCurrentPage=0;

    return true;
}

long CFHZoomView::GetCurrentPage()
{
    return m_nCurrentPage;
}

void CFHZoomView::SetScrollLogPage(CMySize &sizePage)
{
    double x1=0;
    double x2=sizePage.cx;
    LPtoWorldX(x1);
    LPtoWorldX(x2);

    double y1=0;
    double y2=sizePage.cy;
    LPtoWorldY(y1);
    LPtoWorldY(y2);

    SetScrollPage(CMySize(x2-x1,y2-y1));
}


void CFHZoomView::SetScrollLogLine(CMySize &sizeLine)
{
    double x1=0;
    double x2=sizeLine.cx;
    LPtoWorldX(x1);
    LPtoWorldX(x2);

    double y1=0;
    double y2=sizeLine.cy;
    LPtoWorldY(y1);
    LPtoWorldY(y2);

    SetScrollLine(CMySize(x2-x1,y2-y1));
}

double CFHZoomView::GetMyDevPosition(int nBar, int nScrollPos)
{
    CRect rectClient;
    GetClientRect(rectClient);

    SCROLLINFO info;

    double dDevPos=0;
    if(nBar==SB_HORZ)
    {
        if(!GetScrollInfo(SB_HORZ,&info))
        {
            TRACE(" 错误：无法得到滚动条信息!\n");
            return FALSE;
        }
        dDevPos=(double)nScrollPos/(info.nMax-info.nMin -info.nPage );
        dDevPos*=(m_totalDev.cx -( rectClient.Width () - 2*m_nBarWidth- 2*m_nEdgeWidth));
    }

    else if(nBar==SB_VERT)
    {
        if(!GetScrollInfo(SB_VERT,&info))
        {
            TRACE(" 错误：无法得到滚动条信息!\n");
            return FALSE;
        }
        dDevPos=(double)nScrollPos/(info.nMax-info.nMin -info.nPage );
        dDevPos*=(m_totalDev.cy -( rectClient.Height() - 2*m_nBarWidth- 2*m_nEdgeWidth));
    }
    return dDevPos;
}

int CFHZoomView::GetPhyDevPosition(int nBar, double nMyDevPos)
{
    SCROLLINFO info;

    CRect rectClient;
    GetClientRect(rectClient);

    int nPos=0;
    if(nBar==SB_HORZ)
    {
        if(!GetScrollInfo(SB_HORZ,&info))
        {
            TRACE(" 错误：无法得到滚动条信息!\n");
            return FALSE;
        }

        double d=(double)nMyDevPos/(double)(m_totalDev.cx-(rectClient.Width()-m_nBarWidth*2-m_nEdgeWidth*2));
        nPos=d*(info.nMax -info.nMin-info.nPage);
    }

    else
    {
        if(!GetScrollInfo(SB_VERT,&info))
        {
            TRACE(" 错误：无法得到滚动条信息!\n");
            return FALSE;
        }

        double d=(double)nMyDevPos/(double)(m_totalDev.cy-(rectClient.Height()-m_nBarWidth*2-m_nEdgeWidth*2));
        nPos=d*(info.nMax -info.nMin-info.nPage);
    }

    return nPos;

}

void CFHZoomView::OnFhsettings()
{
    ProcessSettings();
}


void  CFHZoomView::PreSettingDlg()
{
    CFHZoomApprDlg *pDlg=new CFHZoomApprDlg;

    pDlg->m_logfontCurrChar     =m_logfontCurrChar;
    pDlg->m_colorCurrChar       =m_colorCurrChar;

    pDlg->m_colorMapChar        =m_colorMapChar ;	
    pDlg->m_colorFontCoor       =m_colorFontCoor;
    pDlg->m_colorThickCoorHor   =m_colorThickCoorHor;
    pDlg->m_colorThinCoorHor    =m_colorThinCoorHor ;
    pDlg->m_colorThickCoorVer   =m_colorThickCoorVer;
    pDlg->m_colorThinCoorVer    =m_colorThinCoorVer ;


    pDlg->m_colorTitle          =m_colorTitle ;
    pDlg->m_colorMapContent     =m_colorMapContent;
    pDlg->m_colorMapContentSpecial=m_colorMapContentSpecial;

    pDlg->m_logfontCoor         =m_logfontCoor ;
    pDlg->m_logfontMapChar      =m_logfontMapChar;
    pDlg->m_logfontTitle        =m_logfontTitle ;

    pDlg->m_bDrawCoor           =m_bDrawCoor ;
    pDlg->m_bDrawTitle          =m_bDrawTitle ;
    pDlg->m_sTitle              =m_sDrawTitle;

    pDlg->m_bUseUserDefinedCoor =m_bUseUserCoor ;
    pDlg->m_nThickLineStartHor  =m_rectUserThickLine.m_dLeft;
    pDlg->m_nThickLineEndHor    =m_rectUserThickLine.m_dRight;
    pDlg->m_nThickLineStepHor   =m_sizeUserThickStep.cx;
    pDlg->m_nThinLineStartHor   =m_rectUserThinLine.m_dLeft;
    pDlg->m_nThinLineEndHor     =m_rectUserThinLine.m_dRight;
    pDlg->m_nThinLineStepHor    =m_sizeUserThinStep.cx ;

    pDlg->m_nThickLineStartVer  =m_rectUserThickLine.m_dLow;
    pDlg->m_nThickLineEndVer    =m_rectUserThickLine.m_dHigh;
    pDlg->m_nThickLineStepVer   =m_sizeUserThickStep.cy;
    pDlg->m_nThinLineStartVer   =m_rectUserThinLine.m_dLow;
    pDlg->m_nThinLineEndVer     =m_rectUserThinLine.m_dHigh;
    pDlg->m_nThinLineStepVer    =m_sizeUserThinStep.cy;

    pDlg->m_iContentSize        =m_iContentSize;
    pDlg->m_iContentCurrSize    =m_iContentCurrSize;

    pDlg->m_ePointPatternCurrent=m_ePointPatternCurrent;
    pDlg->m_ePointPatternCommon =m_ePointPatternCommon;

    AddSettingPage(pDlg);

    //
    CDlgBackGroundPic *pDlgBack=new CDlgBackGroundPic ;

    pDlgBack->m_sFileName=m_sBackGroundFileName;	
    pDlgBack->m_dSouthCoor=m_rectBackGroundPic.m_dLow;
    pDlgBack->m_dNorthCoor=m_rectBackGroundPic.m_dHigh;
    pDlgBack->m_dWestCoor=m_rectBackGroundPic.m_dLeft;
    pDlgBack->m_dEastCoor=m_rectBackGroundPic.m_dRight;
    pDlgBack->m_bKeepRatio=m_bImgBackKeepRatio;

    AddPropertyPage (pDlgBack);

}

void  CFHZoomView::PostSettingDlg()
{
    CFHZoomApprDlg *pDlg=GetPropertyPage(CFHZoomApprDlg );
    if(pDlg!=NULL)
    {
        m_colorFontCoor                 =pDlg->m_colorFontCoor;
        m_colorThickCoorHor             =pDlg->m_colorThickCoorHor;
        m_colorThinCoorHor              =pDlg->m_colorThinCoorHor ;
        m_colorThickCoorVer             =pDlg->m_colorThickCoorVer;
        m_colorThinCoorVer              =pDlg->m_colorThinCoorVer ;

        m_colorTitle                    =pDlg->m_colorTitle ;

        m_colorMapContent               =pDlg->m_colorMapContent;
        m_colorMapContentSpecial        =pDlg->m_colorMapContentSpecial;

        m_logfontCoor                   =pDlg->m_logfontCoor ;
        m_logfontTitle                  =pDlg->m_logfontTitle ;

        m_logfontMapChar                =pDlg->m_logfontMapChar ;
        m_colorMapChar                  =pDlg->m_colorMapChar  ;

        m_logfontCurrChar               =pDlg->m_logfontCurrChar;
        m_colorCurrChar                 =pDlg->m_colorCurrChar;

        m_fontTextCommon    .DeleteObject();  
        m_fontTextCurrent   .DeleteObject();
        m_fontTextCommon    .CreateFontIndirect(&m_logfontMapChar);
        m_fontTextCurrent   .CreateFontIndirect(&m_logfontCurrChar);


        m_bDrawCoor                     =pDlg->m_bDrawCoor ;
        m_bDrawTitle                    =pDlg->m_bDrawTitle ;
        m_sDrawTitle                    =pDlg->m_sTitle;

        m_bUseUserCoor                  =pDlg->m_bUseUserDefinedCoor ;
        if(m_bUseUserCoor)
        {
            SetDrawRange(   pDlg->m_nThickLineStartHor ,
                            pDlg->m_nThickLineStartVer ,
                            pDlg->m_nThickLineEndHor,
                            pDlg->m_nThickLineEndVer);
        }

        m_rectUserThickLine.m_dLeft     =pDlg->m_nThickLineStartHor ;
        m_rectUserThickLine.m_dRight    =pDlg->m_nThickLineEndHor;
        m_sizeUserThickStep.cx          =pDlg->m_nThickLineStepHor;
        m_rectUserThinLine.m_dLeft      =pDlg->m_nThinLineStartHor ;
        m_rectUserThinLine.m_dRight     =pDlg->m_nThinLineEndHor;
        m_sizeUserThinStep.cx           =pDlg->m_nThinLineStepHor;

        m_rectUserThickLine.m_dLow      =pDlg->m_nThickLineStartVer ;
        m_rectUserThickLine.m_dHigh     =pDlg->m_nThickLineEndVer;
        m_sizeUserThickStep.cy          =pDlg->m_nThickLineStepVer;
        m_rectUserThinLine.m_dLow       =pDlg->m_nThinLineStartVer ;
        m_rectUserThinLine.m_dHigh      =pDlg->m_nThinLineEndVer;
        m_sizeUserThinStep.cy           =pDlg->m_nThinLineStepVer;

        m_iContentSize                  =pDlg->m_iContentSize ;
        m_iContentCurrSize              =pDlg->m_iContentCurrSize;

        m_ePointPatternCurrent          =pDlg->m_ePointPatternCurrent;
        m_ePointPatternCommon           =pDlg->m_ePointPatternCommon;


        SetTiltFont(&m_logfontMapChar);
        SetInvalidate();
    }    

    CDlgBackGroundPic *pDlgBack=GetPropertyPage(CDlgBackGroundPic);
    if(pDlgBack!=NULL)
    {
        m_sBackGroundFileName           =pDlgBack->m_sFileName;
        m_rectBackGroundPic.m_dLow      =pDlgBack->m_dSouthCoor;
        m_rectBackGroundPic.m_dHigh     =pDlgBack->m_dNorthCoor;
        m_rectBackGroundPic.m_dLeft     =pDlgBack->m_dWestCoor;
        m_rectBackGroundPic.m_dRight    =pDlgBack->m_dEastCoor;
        m_bImgBackKeepRatio             =pDlgBack->m_bKeepRatio;
    }
}

void CFHZoomView::InvalidateView()
{
    Invalidate();
}

void CFHZoomView::SavePars()
{
    CString sClassName=GetClassName();

    ::AddPar("FHZOOMVIEW_PointPatternCommon"+sClassName,(int)m_ePointPatternCommon);
    ::AddPar("FHZOOMVIEW_PointPatternCurrent"+sClassName,(int)m_ePointPatternCurrent);
    ::AddPar("FHZOOMVIEW_colorMapContent"+sClassName,(unsigned int)m_colorMapContent);
    ::AddPar("FHZOOMVIEW_colorMapContentSpecial"+sClassName,(unsigned int)m_colorMapContentSpecial);
    ::AddPar("FHZOOMVIEW_ContentSize"+sClassName,m_iContentSize);
    ::AddPar("FHZOOMVIEW_ContentCurrSize"+sClassName,m_iContentCurrSize);

    ::AddPar("BACK_GROUND_PIC_ZOOMVIEW_"+sClassName,m_sBackGroundFileName);
    ::AddPar("BACK_GROUND_PIC_ZOOMVIEW_"+sClassName+"_EAST",m_rectBackGroundPic.m_dRight);
    ::AddPar("BACK_GROUND_PIC_ZOOMVIEW_"+sClassName+"_WEST",m_rectBackGroundPic.m_dLeft);
    ::AddPar("BACK_GROUND_PIC_ZOOMVIEW_"+sClassName+"_SOUTH",m_rectBackGroundPic.m_dLow);
    ::AddPar("BACK_GROUND_PIC_ZOOMVIEW_"+sClassName+"_NORTH",m_rectBackGroundPic.m_dHigh);
    ::AddPar("BACK_GROUND_PIC_ZOOMVIEW_"+sClassName+"_KEEPRATIO",m_bImgBackKeepRatio);
}

CString CFHZoomView::GetClassName()
{
    CRuntimeClass *pRuntimeClass=GetRuntimeClass();
    return pRuntimeClass->m_lpszClassName;
}

void  CFHZoomView::AddMenu(UINT MenuID, CString MenuString)
{
    m_menuManager.AppendMenu(MenuID,MenuString, MF_MENUITEM);
}