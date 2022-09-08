// GeoInstDataFileShowView.cpp : implementation file
//

#include "stdafx.h"
#include "ProgDlg.h"
#include "MemDC.H"
#include "FHZoomViewGlobal.h"
#include "TextFileShowView.h"
#include "FHMainFrm.h"

// CTextFileShowView

IMPLEMENT_DYNCREATE(CTextFileShowView, CFHZoomView)

CTextFileShowView::CTextFileShowView()
{
    m_nMinShowableChar=32;
    m_nMaxShowableChar=126;
    m_fontFile.CreateFont (12,0,0,0,0,false,false,false,0,0,0,0,0,"宋体");
    m_nTextLeft=0;
    m_nTextTop=0;
    m_nFileLineNumber=0;
    m_sizeMaxChar.cx =10;
    m_sizeMaxChar.cy =8;
    m_pMyParent=NULL;
    m_nTextStartRow=0;
    m_nTextStartCol=0;

    m_nEdgePlusedWidth=20;

    m_sTitle="";

    m_bUpIncrease=false;

    m_nEdgeWidth=0;
    m_nBarWidth=0;

    m_pointMouse.x=m_pointMouse.y=0;

    m_pLineStartPos=NULL;

}

CTextFileShowView::~CTextFileShowView()
{
}


BEGIN_MESSAGE_MAP(CTextFileShowView, CFHZoomView)
    ON_WM_MOUSEACTIVATE()
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_CREATE()
    ON_WM_HSCROLL()
    ON_WM_VSCROLL()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

void CTextFileShowView::SetTitle(CString sTitle)
{
    m_sTitle=sTitle;
}

// CTextFileShowView drawing

void CTextFileShowView::OnInitialUpdate()
{
    m_bDevSizeLimit=false;
}

void CTextFileShowView::OnDraw(CDC* pDC)
{
    DrawFile(pDC);	
}

// CTextFileShowView diagnostics

#ifdef _DEBUG
void CTextFileShowView::AssertValid() const
{
    CFHZoomView::AssertValid();
}

void CTextFileShowView::Dump(CDumpContext& dc) const
{
    CFHZoomView::Dump(dc);
}
#endif //_DEBUG


// CTextFileShowView message handlers

bool CTextFileShowView::SetFile(CString sFile)
{
    //////////////////////////
    //
    m_nMaxCharQtyInLine=0;
    if(m_pLineStartPos)
    {
        delete []m_pLineStartPos;
        m_pLineStartPos=NULL;
    }
    m_nFileLineNumber=0;

    //////////////////////////////////
    // 检查每行的开始位置
    m_sFile=sFile;
    m_nMaxCharQtyInLine		=GetMaxCharQtyInLine(m_sFile,m_nFileLineNumber);
    if(m_nMaxCharQtyInLine<1||m_nMaxCharQtyInLine>1000||m_nFileLineNumber<1||m_nFileLineNumber>200000)
    {
        TRACEERROR("错误：文件行数目不能超过200000,现在是:"+vtos(m_nFileLineNumber)+",每行最多字符数目不能大于1000,现在是:"+vtos(m_nMaxCharQtyInLine));
        Invalidate();
        return false;
    }

    m_nFileLineNumber+=2;

    m_pLineStartPos=new long[m_nFileLineNumber+2];

    FILE *fp=fopen(m_sFile,"rb");
    m_pLineStartPos[0]=0;
    char c='0';
    long  nLine=0;
    bool bFailed=false;
    while(!feof(fp))
    {
        c='0';
        while(!feof(fp)&&c!=10)
        {
            fread(&c,1,1,fp);
        }
        nLine++;
        if(nLine>=m_nFileLineNumber)
        {
            bFailed=true;
            break;
        }
        m_pLineStartPos[nLine]=ftell(fp);
    }
    fclose(fp);

    ///////////////////////////
    //
    if(!bFailed)
    {
        Invalidate();
        return true;
    }
    else
    {
        m_nFileLineNumber=0;
        return false;
    }
}

CSize CTextFileShowView::GetMaxChar(CDC *pDC)
{	
    CSize sizeMax,size;
    int nMaxW=0;
    char c[10];
    for(int i=m_nMinShowableChar;i<=m_nMaxShowableChar;i++)
    {
        memset(c,i,1);		
        size=pDC->GetTextExtent (c,1);
        if(size.cx >sizeMax.cx )
        {
            sizeMax.cx =size.cx ;
        }

        if(size.cy >sizeMax.cy )
        {
            sizeMax.cy =size.cy ;
        }

    }

    return sizeMax;
}

int CTextFileShowView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
    return 0; //CFHZoomView::OnMouseActivate(pDesktopWnd, nHitTest, message);
}


void CTextFileShowView::DrawFile(CDC *pDC)
{
    if(!m_pLineStartPos||m_sFile=="")return;

    //////////////////////////////
    // 
    CFont *pOldFont=pDC->SelectObject (&m_fontFile);
    m_sizeMaxChar =GetMaxChar(pDC);

    //////////////////////////////
    //	
    CString s;
    s=vtos(m_nFileLineNumber);
    if(m_sTitle!="")
    {
        m_nTextStartRow=3;
    }
    else
    {
        m_nTextStartRow=2;
    }

    CRect rect;
    GetClientRect(&rect);

    m_nTextStartCol=s.GetLength ();
    m_nTextLeft=m_nTextStartCol*m_sizeMaxChar.cx+m_nEdgePlusedWidth ;
    m_nTextTop=m_nTextStartRow*m_sizeMaxChar.cy+m_nEdgePlusedWidth ;

    m_sizeBitmap.cx=(m_nMaxCharQtyInLine+10)*m_sizeMaxChar .cx	+m_nTextLeft;
    m_sizeBitmap.cy=(m_nFileLineNumber	+10)*m_sizeMaxChar .cy	+m_nTextTop; //+rect.Height ();
    m_bDevSizeLimit=false;

    CMySize sizeLine(m_sizeMaxChar.cx,m_sizeMaxChar.cy);	
    CMySize sizePage(m_sizeMaxChar.cx*10,m_sizeMaxChar.cy*10);

    SetDrawRange(0,0,m_sizeBitmap.cx,m_sizeBitmap.cy);
    SetScrollLogPage(sizePage);
    SetScrollLogLine(sizeLine);

    /////////////////////////////
    // 
    CMyPoint pointStart=GetStartPos();
    CRect rectClient;
    GetClientRect(rectClient);

    long nStartLine=pointStart.y/m_sizeMaxChar.cy;
    long nEndLine=(pointStart.y+rectClient.bottom )/m_sizeMaxChar.cy+2;
    if(nStartLine<0)nStartLine=0;
    if(nEndLine>=m_nFileLineNumber)nEndLine=m_nFileLineNumber-1;

    long xPos=m_nTextLeft-pointStart.x+2;
    long yPos=m_nTextTop	;

    char sLine[300];

    if(m_sFile=="")return;
    FILE *fp=fopen(m_sFile,"rt");
    if(!fp)
    {
        return ;
    }

    for(int i=nStartLine;i<nEndLine;i++)
    {
        fseek(fp,m_pLineStartPos[i],SEEK_SET);
        memset(sLine,0,300);
        fgets(sLine,300,fp);
        pDC->TextOut (xPos,yPos,sLine,strlen(sLine));
        yPos+=m_sizeMaxChar.cy;

    }
    fclose(fp);

}

void CTextFileShowView::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    OnPrepareDC(&dc);
    CMemDC memDC(&dc,this);	

    OnDraw(&memDC);
    DrawMarks(&memDC);		
    DrawMarkLines(&memDC);

}

void CTextFileShowView::DrawMarkLines(CDC *pDC)
{
    CPen penMarkLine;
    penMarkLine.CreatePen(PS_DOT,0,RGB(125,125,125));
    CPen *pOldPen=pDC->SelectObject (&penMarkLine);
    pDC->MoveTo (0,m_pointMouse.y);
    pDC->LineTo (10000,m_pointMouse.y);
    pDC->MoveTo (m_pointMouse.x,0);
    pDC->LineTo (m_pointMouse.x,10000);
    pDC->SelectObject (pOldPen);
}

void CTextFileShowView::DrawMarks(CDC *pDC)
{
    if(m_nFileLineNumber==0)return;

    CRect rect;
    GetClientRect(&rect);
    int nRight	=rect.right ;
    int nBottom	=rect.bottom ;

    CMyPoint pointOrg=GetStartPos();

    CFont *pOldFont=pDC->SelectObject (&m_fontFile);

    COLORREF nInnerColor=RGB(230,230,230);
    COLORREF nBaseColor=RGB(190,190,190);
    COLORREF nBrightColor=RGB(255,255,255);
    COLORREF nDarkColor=RGB(0,0,0);

    CPen penDark,penBright,penBase,penInner;
    penDark.CreatePen (PS_SOLID,0,nDarkColor);
    penBright.CreatePen (PS_SOLID,0,nBrightColor);
    penBase.CreatePen (PS_SOLID,0,nBaseColor);
    penInner.CreatePen (PS_SOLID,0,nInnerColor);

    CBrush brushDark,brushBright,brushBase,brushInner;
    brushDark.CreateSolidBrush (nDarkColor);
    brushBright.CreateSolidBrush (nBrightColor);
    brushBase.CreateSolidBrush (nBaseColor);
    brushInner.CreateSolidBrush (nInnerColor);

    ///////////////////////////
    // 字体颜色
    pDC->SetTextColor (RGB(0,0,0));
    pDC->SetBkColor (nInnerColor); //::GetSysColor(COLOR_INACTIVEBORDER));

    /////////////////////////////////
    // 绘制黑底	
    CPen *pOldPen=pDC->SelectObject (&penDark);
    CBrush *pOldBrush=pDC->SelectObject(&brushDark);

    ////////////////////////////////
    // 绘制阴影
    pDC->SelectObject (&penBase);	
    pDC->SelectObject(&brushBase);
    pDC->Rectangle (0,0,10000,m_nTextTop);
    pDC->Rectangle (0,0,m_nTextLeft,10000);

    int nEdgeWidth=m_nEdgePlusedWidth/2;
    pDC->SelectObject (&penInner);
    pDC->SelectObject(&brushInner);
    pDC->Rectangle (nEdgeWidth,nEdgeWidth,m_nTextLeft-nEdgeWidth,m_nTextTop-nEdgeWidth);

    pDC->Rectangle(m_nTextLeft,nEdgeWidth,10000,m_nTextTop-nEdgeWidth);
    pDC->Rectangle(nEdgeWidth,m_nTextTop,m_nTextLeft-nEdgeWidth,10000);


    /////////////////////////////////////////
    // 0     1      2     3   ..
    CString s,s1;
    long i=0;
    while(s.GetLength()<m_nMaxCharQtyInLine+5	)
    {
        s1=vtos(i);
        while(s1.GetLength ()<10)
        {
            s1+=" ";
        }
        s+=s1;
        i++;
    }
    pDC->TextOut(-pointOrg.x+m_nTextLeft+2,nEdgeWidth,s);

    s="";
    while(s.GetLength()<m_nMaxCharQtyInLine+5	)
    {
        s+="0123456789";
    }
    pDC->TextOut(-pointOrg.x+m_nTextLeft+2,m_sizeMaxChar.cy+nEdgeWidth ,s);

    if(m_sTitle!="")
    {
        pDC->TextOut(-pointOrg.x+m_nTextLeft+2,m_sizeMaxChar.cy*2+nEdgeWidth ,m_sTitle);
    }


    //////////////////////////////////////
    //  Row Number:
    long nPos;
    s=vtos(m_nFileLineNumber);
    int nMax=s.GetLength ();

    double dStartDraw	=pointOrg.y/m_sizeMaxChar.cy -2;
    if(dStartDraw<0)dStartDraw=0;
    double dEndDraw		=dStartDraw+rect.Height ()/m_sizeMaxChar.cy+2;
    if(dEndDraw>=m_nFileLineNumber)dEndDraw=m_nFileLineNumber-1;

    for(i=dStartDraw;i<=dEndDraw;i++)
    {
        nPos=i*m_sizeMaxChar .cy+m_nTextTop ;

        s=vtos(i);
        while(s.GetLength ()<nMax)
        {
            s="0"+s;
        }

        pDC->TextOut(nEdgeWidth,nPos-pointOrg.y,s);
    }

    /////////////////////////////////
    // 绘制左上角区域
    pDC->SelectObject (&penBase);
    pDC->SelectObject (&brushBase);
    pDC->Rectangle (0,0,m_nTextLeft,m_nTextTop);

    //////////////////////////////////
    // 绘制暗边
    pDC->SelectObject (&penDark);

    pDC->MoveTo (m_nTextLeft,m_nTextTop);
    pDC->LineTo (10000,m_nTextTop);
    pDC->MoveTo (m_nTextLeft,m_nTextTop);
    pDC->LineTo (m_nTextLeft,10000);

    pDC->MoveTo (m_nTextLeft,nEdgeWidth);
    pDC->LineTo (10000,nEdgeWidth);
    pDC->MoveTo (m_nTextLeft,nEdgeWidth);
    pDC->LineTo (m_nTextLeft,m_nTextTop-nEdgeWidth);

    pDC->MoveTo (nEdgeWidth-1,m_nTextTop);
    pDC->LineTo (m_nTextLeft-nEdgeWidth,m_nTextTop);
    pDC->MoveTo (nEdgeWidth-1,m_nTextTop);
    pDC->LineTo (nEdgeWidth-1,10000);

    ///////////////////////////////
    //  绘制亮边
    pDC->SelectObject (&penBright);

    pDC->MoveTo (0,0);
    pDC->LineTo (10000,0);
    pDC->MoveTo (0,0);
    pDC->LineTo (0,10000);

    pDC->MoveTo (m_nTextLeft-nEdgeWidth,m_nTextTop);
    pDC->LineTo (m_nTextLeft-nEdgeWidth,10000);
    pDC->MoveTo (m_nTextLeft,m_nTextTop-nEdgeWidth);
    pDC->LineTo (10000,m_nTextTop-nEdgeWidth);



    pDC->SelectObject (pOldPen);
    pDC->SelectObject (pOldFont);
    pDC->SelectObject (pOldBrush);

}
BOOL CTextFileShowView::OnEraseBkgnd(CDC* pDC)
{
    return true;
}

int CTextFileShowView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFHZoomView::OnCreate(lpCreateStruct) == -1)
        return -1;

    return 0;
}

void CTextFileShowView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    Invalidate();
    CFHZoomView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CTextFileShowView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    Invalidate();
    CFHZoomView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CTextFileShowView::OnMouseMove(UINT nFlags, CPoint point)
{
    m_pointMouse.x=point.x;
    m_pointMouse.y=point.y;
    Invalidate();

    CFHZoomView::OnMouseMove(nFlags, point);
}

bool CTextFileShowView::SetParent(CWnd *pParent)
{
    ASSERT_VALID(pParent);	
    if(!pParent->IsKindOf (RUNTIME_CLASS(CWnd)))return false;
    m_pMyParent=pParent;
    return true;
}
void CTextFileShowView::OnLButtonDown(UINT nFlags, CPoint point)
{
    CMyPoint pointOrg=GetMyScrollPos();
    pointOrg+=point;
    m_pointMouse=pointOrg;	

    CMyPoint p=GetStartPos();
    int nLine=(p.y+point.y-m_nTextTop)/m_sizeMaxChar.cy;
    int nChar=(p.x+point.x-m_nTextLeft)/m_sizeMaxChar.cx;

    if(nLine<0)nLine=0;
    if(nChar<0)nChar=0;

    if(m_pMyParent)
    {
        m_pMyParent->SendMessage(FM_MOUSELBDOWN,nLine,nChar);
    }

    CFHZoomView::OnLButtonDown(nFlags, point);
}

BOOL CTextFileShowView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    // TODO: Add your message handler code here and/or call default

    return CFHZoomView::OnMouseWheel(nFlags, zDelta, pt);
}

CMenu * CTextFileShowView::GetPopupMenu()
{
    return NULL;
}

CMyPoint CTextFileShowView::GetStartPos()
{
    CRect rectClient;
    GetClientRect(rectClient);
    double w=rectClient.Width ();
    double h=rectClient.Height ();

    SCROLLINFO infoHor;
    GetScrollInfo(SB_HORZ   ,&infoHor);

    SCROLLINFO infoVer;
    GetScrollInfo(SB_VERT   ,&infoVer);

    double x=infoHor.nPos /(double)(infoHor.nMax -infoHor.nPage )*(m_sizeBitmap.cx-w);
    double y=infoVer.nPos /(double)(infoVer.nMax -infoVer.nPage )*(m_sizeBitmap.cy-h);

    x=int(x/m_sizeMaxChar.cx)*m_sizeMaxChar.cx;
    y=int(y/m_sizeMaxChar.cy)*m_sizeMaxChar.cy;

    if(m_sizeBitmap.cx<w)x=0;
    if(m_sizeBitmap.cy<h)y=0;


    return CMyPoint(x,y);
}

