// ColorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "zoomview.h"
#include "ColorScale.h"
#include "FontGroup.h"
#include "math.h"
#include "templates.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorDlg dialog


CColorDlg::CColorDlg(CWnd* pParent /*=NULL*/)
: CDialog(CColorDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CColorDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_bParentDisabled = FALSE;

    m_dMinValue=0;
    m_dMaxValue=500;
    m_nColorNumber=500;
    m_dScale=1;
    m_dMapLenScale=1;

    m_nMaxLen=100;
    SetColorSerial(COLOR_RGB);

    LOGFONT logFont;
    memset(&logFont,0,sizeof(LOGFONT));
    logFont.lfHeight       = 15;
    logFont.lfCharSet      = ANSI_CHARSET;
    logFont.lfStrikeOut    = false;
    logFont.lfUnderline    = false;
    logFont.lfOutPrecision = OUT_TT_PRECIS ;
    logFont.lfQuality      = PROOF_QUALITY;
    strcpy(logFont.lfFaceName,"Arial");

    m_font.CreateFontIndirect(&logFont);
}

CColorDlg::~CColorDlg()
{
    if(m_hWnd!=NULL)
    {
        DestroyWindow();
    }
}

void CColorDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CColorDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColorDlg, CDialog)
    //{{AFX_MSG_MAP(CColorDlg)
    ON_WM_PAINT()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorDlg message handlers

BOOL CColorDlg::OnInitDialog() 
{
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CColorDlg::Create(CWnd *pParent)
{
    // Get the true parent of the dialog
    m_pParentWnd = CWnd::GetSafeOwner(pParent);

    // m_bParentDisabled is used to re-enable the parent window
    // when the dialog is destroyed. So we don't want to set
    // it to TRUE unless the parent was already enabled.

    if((m_pParentWnd!=NULL) && m_pParentWnd->IsWindowEnabled())
    {
        m_pParentWnd->EnableWindow(FALSE);
        m_bParentDisabled = TRUE;
    }

    if(!CDialog::Create(CColorDlg::IDD,pParent))
    {
        return FALSE;
    }
    ReEnableParent();

    return TRUE;
}
void CColorDlg::ReEnableParent()
{
    if(m_bParentDisabled && (m_pParentWnd!=NULL))
        m_pParentWnd->EnableWindow(TRUE);
    m_bParentDisabled=FALSE;
}

BOOL CColorDlg::DestroyWindow()
{
    ReEnableParent();
    return CDialog::DestroyWindow();
}
void CColorDlg::SetRange(double dMinValue, double dMaxValue)
{
    m_dMinValue=dMinValue;
    m_dMaxValue=dMaxValue;
    if(m_dMinValue>m_dMaxValue)
    {
        Swap(m_dMinValue,m_dMaxValue);
    }

    if(fabs(m_dMaxValue-m_dMinValue)<0.00000000000001)
    {
        m_dMaxValue=m_dMinValue+0.1;
    }

    m_dScale=m_nColorNumber/(m_dMaxValue-m_dMinValue);
}

COLORREF CColorDlg::GetColor(double dValue)
{
    int n=(dValue-m_dMinValue)*m_dScale;
    if(n<0)n=0;
    if(n>=(m_nColorNumber-1))n=m_nColorNumber-1;
    return m_nColor[n];
}

COLORREF CColorDlg::GetRevColor(double dValue)
{
    COLORREF nColor=GetColor(dValue);
    long  nRed  =GetRValue(nColor);
    long  nGreen=GetGValue(nColor);
    long  nBlue =GetBValue(nColor);
    return RGB(255-nRed,255-nGreen,255-nBlue);
}
COLORREF CColorDlg::GetRevColor(COLORREF nColor)
{
    long  nRed	=GetRValue(nColor);
    long  nGreen=GetGValue(nColor);
    long  nBlue	=GetBValue(nColor);
    return RGB(255-nRed,255-nGreen,255-nBlue);
}

void CColorDlg::OnPaint() 
{
    /////////////////////////////////////////////////////////
    // Get the width and height of the marks:
    /////////////////////////////////////////////////////////
    CPaintDC dc(this); 
    CFont *pOldFont=dc.SelectObject(&m_font);

    // The fonts:
    int nSmallNumber=3;
    if(fabs(m_dMaxValue-m_dMinValue)<0.02)
    {
        nSmallNumber=3;
    }
    else if(fabs(m_dMaxValue-m_dMinValue)<0.2)
    {
        nSmallNumber=2;
    }
    else if(fabs(m_dMaxValue-m_dMinValue)<5)
    {
        nSmallNumber=1;
    }
    else
    {
        nSmallNumber=0;
    }

    CString s,sFormat;
    sFormat.Format("%%1.%d\lf",nSmallNumber);
    s.Format(sFormat,m_dMinValue);
    if(s.GetLength()>8)
    {
        s.Format("%1.1E",m_dMinValue);
    }
    CSize sizeA=dc.GetTextExtent (s);
    s.Format(sFormat,m_dMaxValue);
    if(s.GetLength()>8)
    {
        s.Format("%1.1E",m_dMaxValue);
    }

    CSize sizeB=dc.GetTextExtent (s);
    int nFontWidth  =sizeA.cx;
    int nFontHeight =sizeA.cy ;

    if(sizeB.cx>nFontWidth)
    {
        nFontWidth=sizeB.cx;
    }

    /////////////////////////////////////////////////////////
    // Resize the window:
    /////////////////////////////////////////////////////////
    CRect rectClient,rectWindow;
    GetWindowRect (&rectWindow);
    GetClientRect (&rectClient);


    int nScaleWidth = 30;
    int nScaleEdge  = 15;
    int nWindowEdge = (rectWindow.Width()-rectClient.Width())/2;
    int nWindowTopEdge = (rectWindow.Height()-rectClient.Height())-nWindowEdge;

    rectWindow.right=rectWindow.left+ nFontWidth+ nScaleWidth + nScaleEdge * 2 ;
    MoveWindow(&rectWindow);

    /////////////////////////////////////////////////////////
    // Draw the Color Scale:
    /////////////////////////////////////////////////////////
    CRect rectScale;
    rectScale.left      =   nScaleEdge;
    rectScale.right     =   rectScale.left + nScaleWidth;
    rectScale.top       =   nScaleEdge;
    rectScale.bottom    =   rectClient.Height() - nScaleEdge;
    dc.Rectangle (rectScale);

    double dOneScaleHeight  =   (double)(rectScale.Height ()) /(double)m_nColorNumber;
    int nOneScaleHeight = int(dOneScaleHeight)+1;

    int nTop;
    for(int i=0;i<m_nColorNumber;i++)
    {
        nTop = rectScale.bottom - dOneScaleHeight* (i+1);
        if(nTop <= rectScale.top)
        {
            break;
        }

        dc.FillSolidRect(   rectScale.left+1,
                            nTop,
                            rectScale.Width()-2,
                            nOneScaleHeight,
                            m_nColor[i]);
    }

    /////////////////////////////////////////////////////////
    // Draw the Scale Mark:
    /////////////////////////////////////////////////////////
    COLORREF nColor,nBackColor=RGB(255,255,255);
    CString sValue;

    CFontGroup fontGroup;
    fontGroup.Set(m_dMinValue,m_dMaxValue,rectScale.Height (),nFontHeight);
    double dStart,dEnd,dStep;
    fontGroup.CalcFontGroup (dStart,dEnd,dStep);

    CBrush brush;
    brush.CreateSolidBrush (nBackColor);
    CBrush *pOldBrush=dc.SelectObject(&brush);
    dc.SetBkColor (nBackColor);

    int nMiddle;
    for(double v=dStart;v<=dEnd;v+=dStep)
    {
        sFormat.Format("%%1.%d\lf",nSmallNumber);
        sValue.Format(sFormat,v);
        if(sValue.GetLength()>8)
        {
            sValue.Format("%1.1E",v);
        }

        nColor  = int((v-m_dMinValue)*m_dScale);
        nMiddle = rectScale.bottom - (nColor+0.5) * dOneScaleHeight;
        if(nMiddle<rectScale.top)
        {
            break;
        }

        dc.TextOut (rectScale.right+1,nMiddle-nFontHeight/2,sValue);
    }

    /////////////////////////////////////////////////////////
    // Restore the objects:
    /////////////////////////////////////////////////////////
    dc.SelectObject(pOldBrush);
    dc.SelectObject(pOldFont);
    // Do not call CDialog::OnPaint() for painting messages
}



void CColorDlg::OnCancel() 
{
    //CDialog::OnCancel();

    // TODO: Add your message handler code here

}
void CColorDlg::OnOK() 
{
    //CDialog::OnCancel();

    // TODO: Add your message handler code here

}

void CColorDlg::SetMaxLength(long nMaxLen)
{
    if(nMaxLen>0)m_nMaxLen=nMaxLen;
    m_dMapLenScale=m_nMaxLen/(m_dMaxValue-m_dMinValue);
}


unsigned long CColorDlg::GetMapLen(double dValue)
{
    unsigned long n=(dValue-m_dMinValue)*m_dMapLenScale;

    if(n<0)n=0;
    if(n>=m_nMaxLen)n=m_nMaxLen;

    return n;
}

void CColorDlg::SetColorSerial(UINT nColor)
{
    int n,j;

    if(nColor==COLOR_RED)
    {
        for(j=0;j<500;j++)
        {
            m_nColor[j]=RGB(j/500.0*255,0,0);
        }
    }
    else if(nColor==COLOR_GREEN)
    {
        for(j=0;j<500;j++)
        {
            m_nColor[j]=RGB(0,j/500.0*255,0);
        }
    }
    else if(nColor==COLOR_BLUE)
    {
        for(j=0;j<500;j++)
        {
            m_nColor[j]=RGB(0,0,j/500.0*255);
        }
    }
    else if(nColor==COLOR_RGB)
    {
        for(j=0;j<100;j++)
        {
            m_nColor[j]=RGB(0,0,j*2.5);
        }

        for(j=100;j<200;j++){
            n=(j-100)*2.55;
            m_nColor[j]=RGB(0,n,255);
        }

        for(j=200;j<300;j++)
        {
            n=(300-j)*2.55;
            m_nColor[j]=RGB(0,255,n);
        }

        for(j=300;j<400;j++)
        {
            n=(j-300)*2.55;
            m_nColor[j]=RGB(n,255,0);
        }

        for(j=400;j<500;j++)
        {
            n=(500-j)*2.55;
            m_nColor[j]=RGB(255,n,0);
        }
    }

    else if (nColor==COLOR_RWB)
    {
        for(j=0;j<=125;j++)
        {
            m_nColor[j]=RGB(0,0,j*2);
        }

        int n;
        float k=255.0/125.0;
        for(j=125;j<=250;j++)
        {
            n=(j-125)*k;
            m_nColor[j]=RGB(n,n,255);
        }

        k=-255.0/248.0;
        float b=-k*499.0;
        for(j=251;j<500;j++)
        {
            n=506-j;
            m_nColor[j]=RGB(255,n,n);
        }
    }
}

///////////////////////////////////
// fRate>=0, fRate<=1;
// =1: Same with input color
COLORREF CColorDlg::Brighter(COLORREF nColor,float fRate)
{
    if(fRate<0)
    {
        fRate=0;
    }
    else if(fRate>1.0)
    {
        fRate=1.0;
    }

    UINT nRed=GetRValue(nColor);
    UINT nGreen=GetGValue(nColor);
    UINT nBlue=GetBValue(nColor);

    nRed    =   nRed    +fRate*(255-nRed);
    nGreen  =   nGreen  +fRate*(255-nGreen);
    nBlue   =   nBlue   +fRate*(255-nBlue);

    return RGB(nRed,nGreen,nBlue);

}

