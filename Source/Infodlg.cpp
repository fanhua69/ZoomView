// InfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "zoomview.h"
#include "InfoDlg.h"
#include "FHZoomViewGlobal.h"
#include "..\include\infodlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInfoDlg dialog


CInfoDlg::CInfoDlg(CWnd* pParent /*=NULL*/)
: CDialog(CInfoDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CInfoDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_sInfo="";
    m_pointOrg.x=m_pointOrg.y=300;
    m_bAnchor=false; 
    m_nAnchorMousePos.x=m_nAnchorMousePos.y=0;

    m_nColorFore=RGB(255,0,0);
    m_nColorBack=RGB(255,255,0);
    m_pParent=NULL;

    m_bCanShowInfo=false;
}


void CInfoDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CInfoDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInfoDlg, CDialog)
    //{{AFX_MSG_MAP(CInfoDlg)
    ON_WM_LBUTTONDBLCLK()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_PAINT()
    ON_MESSAGE(WM_WNDCHANGED, OnWndChange) 
    //}}AFX_MSG_MAP
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInfoDlg message handlers

void CInfoDlg::Show(int nx,int ny,CString sInfo)
{
    m_pointOrg.x=nx;
    m_pointOrg.y=ny;
    m_sInfo=sInfo;

    Invalidate();
}

LRESULT CInfoDlg::OnWndChange(WPARAM wParam, LPARAM lParam)
{
    HWND hWndCurrent=(HWND)lParam;
    if(hWndCurrent!=m_pParent->GetSafeHwnd ()&&hWndCurrent!=GetSafeHwnd())
    {
        ShowWindow(SW_HIDE);
    }
    else
    {
        if(m_bCanShowInfo )
        {
            ShowWindow(SW_SHOW);
            Invalidate();
        }
    }
    return 0;
}


void CInfoDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
    // TODO: Add your message handler code here and/or call default
    m_bCanShowInfo=false;
    ShowWindow(SW_HIDE);

    CDialog::OnLButtonDblClk(nFlags, point);
}

void CInfoDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
    // TODO: Add your message handler code here and/or call default
    SetCapture();

    CPoint pointBack=point;
    ClientToScreen(&pointBack);

    m_bAnchor=true;
    CRect rect;
    GetWindowRect(&rect);
    m_nAnchorMousePos.x=rect.left-pointBack.x ;
    m_nAnchorMousePos.y=rect.top-pointBack.y ;

    CDialog::OnLButtonDown(nFlags, point);
}

void CInfoDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{

    // TODO: Add your message handler code here and/or call default
    m_bAnchor=false;
    m_nAnchorMousePos.x=m_nAnchorMousePos.y=0;

    ReleaseCapture();
    CDialog::OnLButtonUp(nFlags, point);
}

void CInfoDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
    // TODO: Add your message handler code here and/or call default
    if(m_bAnchor){
        CPoint pointBack=point;
        ClientToScreen(&pointBack);

        CRect rect;
        GetWindowRect(&rect);

        m_pointOrg.x=pointBack.x+m_nAnchorMousePos.x;
        m_pointOrg.y=pointBack.y+m_nAnchorMousePos.y+rect.Height();
        OnPaint();
    }

    CDialog::OnMouseMove(nFlags, point);
}

CString CInfoDlg::GetName(CString sString)
{
    CString sName;
    int n=sString.Find(':');
    if(n!=-1)
    {
        sName=sString.Left(n);
    }
    else
    {
        n=sString.Find('：');
        if(n!=-1)
        {
            sName=sString.Left(n);
        }
        else
        {
            sName="";
        }
    }
    return sName;
}

void CInfoDlg::OnPaint() 
{
    CPaintDC dc(this); // device context for painting
    m_sInfo.TrimLeft ();
    m_sInfo.TrimRight ();

    if(m_sInfo=="")m_sInfo="没有信息";

    // TODO: Add your message handler code here
    CFont *pOldFont=dc.SelectObject(&m_fontFore);	

    CString sSubString;
    CSize size(0,0);
    CSize sizeSub(0,0);
    int i=0,n=0;
    int iNameLenMax=0;
    CString sName;
    for(i=0;i<m_sInfo.GetLength ();i++)
    {
        sSubString=GetSubString(m_sInfo,i);
        if(sSubString=="")break;
        sName=GetName(sSubString);
        if(sName.GetLength()>iNameLenMax)
        {
            iNameLenMax=sName.GetLength();
        }
    }

    CString sInfo;
    int iSubStringMaxLen=0;
    for(i=0;i<m_sInfo.GetLength ();i++)
    {
        sSubString=GetSubString(m_sInfo,i);
        if(sSubString=="")break;

        sName=GetName(sSubString);
        if(sName!="")
        {
            while(sName.GetLength()<iNameLenMax)
            {
                sName=" "+sName;
                sSubString=" "+sSubString;
            }
        }
        if(sSubString.GetLength()>iSubStringMaxLen)
        {
            iSubStringMaxLen=sSubString.GetLength();
        }
        sInfo+=sSubString+"\r\n";

        sizeSub=dc.GetTextExtent(sSubString,sSubString.GetLength());
        if(sizeSub.cx >size.cx)size.cx=sizeSub.cx;
        size.cy+=sizeSub.cy ;
    }

    CString sInfoNew="";
    for(i=0;i<sInfo.GetLength ();i++)
    {
        sSubString=GetSubString(sInfo,i);
        if(sSubString=="")break;
        if(sSubString.Find("==")!=-1)
        {
            while(sSubString.GetLength()<iSubStringMaxLen)
            {
                sSubString="="+sSubString;
            }
        }
        sInfoNew+=sSubString+"\r\n";
    }

    int nRowHeight=sizeSub.cy;

    size.cx+=6;
    size.cy+=6;
    int cxScreen=GetSystemMetrics(SM_CXSCREEN)-1;
    int cyScreen=GetSystemMetrics(SM_CYSCREEN)-1;
    if((m_pointOrg.x+size.cx)>cxScreen)m_pointOrg.x=cxScreen-size.cx;
    if(m_pointOrg.y>cyScreen)m_pointOrg.y=cyScreen;	
    if(m_pointOrg.x<1)m_pointOrg.x=1;
    if((m_pointOrg.y-size.cy)<1)m_pointOrg.y=size.cy+1;

    // Clear the screen:
    CBrush brush;
    brush.CreateSolidBrush (m_nColorBack);
    CBrush *pOldBrush=dc.SelectObject(&brush);
    dc.Rectangle(0,0,size.cx,size.cy);
    dc.SelectObject(pOldBrush);	

    // The the text:
    dc.SetTextColor(m_nColorFore);
    dc.SetBkColor (m_nColorBack);
    CRect rect(3,3,size.cx,nRowHeight);
    for(i=0;i<sInfoNew.GetLength ();i++)
    {
        sSubString=GetSubString(sInfoNew,i);
        if(sSubString=="")break;

        sName=GetName(sSubString);

        if(sName!="")
        {
            dc.DrawText((LPCTSTR)sSubString,&rect,DT_VCENTER|DT_LEFT|DT_NOCLIP);	
        }
        else
        {
            dc.DrawText((LPCTSTR)sSubString,&rect,DT_VCENTER|DT_CENTER|DT_NOCLIP);	
        }

        sizeSub=dc.GetTextExtent(sSubString,sSubString.GetLength());
        if(sizeSub.cx>size.cx)
        {
            size.cx=sizeSub.cx;
        }

        rect.top+=nRowHeight;
        rect.bottom+=nRowHeight ;
    }

    dc.SelectObject(pOldFont);

    ControlPos(m_pParent,size.cx,size.cy);

    if(m_bCanShowInfo)
    {
        MoveWindow(m_pointOrg.x,m_pointOrg.y-size.cy,size.cx,size.cy);
    }


    //CDialog::OnPaint() ; 
    //Do not call CDialog::OnPaint() for painting messages
}

void CInfoDlg::SetMsg(CString sMsg,bool bRedraw)
{
    m_sInfo=sMsg;
    if(bRedraw){
        Invalidate();
    }
}

void CInfoDlg::SetForeColor(int nRed,int nGreen,int nBlue)
{
    if(nRed<0)nRed=0;
    if(nRed>255)nRed=255;
    if(nGreen<0)nGreen=0;
    if(nGreen>255)nGreen=255;
    if(nBlue<0)nBlue=0;
    if(nBlue>255)nBlue=255;

    m_nColorFore=RGB(nRed,nGreen,nBlue);
}

void CInfoDlg::SetBackColor(int nRed, int nGreen, int nBlue)
{
    if(nRed<0)nRed=0;
    if(nRed>255)nRed=255;
    if(nGreen<0)nGreen=0;
    if(nGreen>255)nGreen=255;
    if(nBlue<0)nBlue=0;
    if(nBlue>255)nBlue=255;

    m_nColorBack=RGB(nRed,nGreen,nBlue);
}

void CInfoDlg::Show(CString sInfo)
{
    if(sInfo.Right(2)=="\r\n")
    {
        sInfo=sInfo.Left(sInfo.GetLength()-2);
    }

    m_sInfo=sInfo;
    Invalidate();
}

void CInfoDlg::ControlPos(CWnd *pWnd,int cx,int cy)
{
    if(!pWnd)
    {
        pWnd=m_pParent;
    }

    if(!pWnd||!pWnd->m_hWnd||!IsWindow(pWnd->m_hWnd))return;

    CRect rectClient;
    pWnd->GetClientRect (rectClient);
    pWnd->ClientToScreen(rectClient);

    if(cx==0||cy==0)
    {
        CRect rectDlg;
        GetWindowRect (&rectDlg);
        cx=rectDlg.Width ();
        cy=rectDlg.Height();
    }

    if(m_pointOrg .x < rectClient.left         ) m_pointOrg .x = rectClient.left;
    if(m_pointOrg .y - cy <rectClient.top   ) m_pointOrg .y = rectClient.top   + cy;
    if(m_pointOrg .x + cx >rectClient.right ) m_pointOrg .x = rectClient.right - cx;
    if(m_pointOrg .y >rectClient.bottom        ) m_pointOrg .y = rectClient.bottom;
}

void CInfoDlg::SetControlWnd(CWnd *pEdgeWnd)
{
    m_pParent=pEdgeWnd;
}


BOOL CInfoDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_hookMouse.AddToHook (this);

    m_fontFore.CreateFont (12, /*MulDiv(8,GetDC()->GetDeviceCaps (LOGPIXELSY),48)*,*/
        0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_STROKE_PRECIS,
        CLIP_STROKE_PRECIS,DRAFT_QUALITY,VARIABLE_PITCH|FF_SWISS,_T("宋体"));

    SetFont(&m_fontFore);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CInfoDlg::OnEraseBkgnd(CDC* pDC)
{
    return true;
}
