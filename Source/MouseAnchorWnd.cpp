// MouseAnchorWnd.cpp : implementation file
//

#include "stdafx.h"
#include "zoomview.h"
#include "FHZoomView.h"
#include "MouseAnchorWnd.h"


//#define MM_FH_NONE             0
//#define MM_SCALETOFIT       (-1)

#ifndef AFX_CX_ANCHOR_BITMAP	

#define AFX_CX_ANCHOR_BITMAP	32
#define AFX_CY_ANCHOR_BITMAP	32
#define ID_FH_TIMER_TRACKING	0xE000

#endif


// CMouseAnchorWnd

IMPLEMENT_DYNAMIC(CMouseAnchorWnd, CWnd)

// CMouseAnchorWnd message handlers

BEGIN_MESSAGE_MAP(CMouseAnchorWnd, CWnd)
    ON_WM_PAINT()
    ON_WM_TIMER()
END_MESSAGE_MAP()

CMouseAnchorWnd::CMouseAnchorWnd(CPoint& ptAnchor)
: m_ptAnchor(ptAnchor), m_bQuitTracking(FALSE)
{
}

CMouseAnchorWnd::~CMouseAnchorWnd()
{
}

BOOL CMouseAnchorWnd::PreTranslateMessage(MSG* pMsg)
{
    BOOL bRetVal = FALSE;

    switch (pMsg->message)
    {
        // any of these messages cause us to quit scrolling
    case WM_MOUSEWHEEL:
    case WM_KEYDOWN:
    case WM_CHAR:
    case WM_KEYUP:
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MBUTTONDOWN:
        m_bQuitTracking = TRUE;
        bRetVal = TRUE;
        break;

        // Button up message depend on the position of cursor
        // This enables the user to click and drag for a quick pan.
    case WM_MBUTTONUP:
        {
            CPoint pt(pMsg->lParam);
            ClientToScreen(&pt);
            if (!PtInRect(&m_rectDrag, pt))
                m_bQuitTracking = TRUE;
            bRetVal = TRUE;
        }
        break;
    }

    return bRetVal;
}

void CMouseAnchorWnd::OnTimer(UINT_PTR nIDEvent)
{
    ASSERT(nIDEvent == ID_FH_TIMER_TRACKING);
    UNUSED(nIDEvent);

    int nCursor = -1;

    CPoint ptNow;
    GetCursorPos(&ptNow);

    CRect rectClient;
    GetWindowRect(&rectClient);

    // decide where the relative position of the cursor is
    // pick a cursor that points where we're going
    nCursor = 0;

    // if vertical scrolling allowed, consider vertical
    // directions for the cursor we'll show
    if (m_nAnchorID == AFX_IDC_MOUSE_ORG_HV || m_nAnchorID == AFX_IDC_MOUSE_ORG_VERT)
    {
        if (ptNow.y < rectClient.top)
            nCursor = AFX_IDC_MOUSE_PAN_N;
        else if (ptNow.y > rectClient.bottom)
            nCursor = AFX_IDC_MOUSE_PAN_S;
    }

    // if horizontal scrolling allowed, cosider horizontal
    // directions for the cursor, too. Only consider diagonal
    // if we can scroll both ways.
    if (m_nAnchorID == AFX_IDC_MOUSE_ORG_HV || m_nAnchorID == AFX_IDC_MOUSE_ORG_HORZ)
    {
        if (ptNow.x < rectClient.left)
        {
            if (nCursor == 0)
                nCursor = AFX_IDC_MOUSE_PAN_W;
            else if (m_nAnchorID == AFX_IDC_MOUSE_ORG_HV)
                nCursor--;
        }
        else if (ptNow.x > rectClient.right)
        {
            if (nCursor == 0)
                nCursor = AFX_IDC_MOUSE_PAN_E;
            else if (m_nAnchorID == AFX_IDC_MOUSE_ORG_HV)
                nCursor++;
        }
    }

    if (m_bQuitTracking)
    {
        // Someone knows we should stop playing with the mouse
        // kill the timer, quit capturing the mouse, clear the cursor,
        // destroy the window, and make sure that CFHZoomView knows the
        // window isn't valid anymore.

        KillTimer(ID_FH_TIMER_TRACKING);
        ReleaseCapture();
        SetCursor(NULL);
        CFHZoomView* pView = (CFHZoomView*) GetOwner();
        DestroyWindow();
        delete pView->m_pAnchorWindow;
        pView->m_pAnchorWindow = NULL;
    }
    else if (nCursor == 0)
    {
        // The cursor is over the anchor window; use a cursor that
        // looks like the anchor bitmap.
        SetCursor(m_hAnchorCursor);
    }
    else
    {
        // We're still actively tracking, so we need to find a cursor and
        // set it up.

        HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(nCursor),
            RT_GROUP_CURSOR);
        HICON hCursor = ::LoadCursor(hInst, MAKEINTRESOURCE(nCursor));
        ASSERT(hCursor != NULL);
        SetCursor(hCursor);

        // ask the view how much to scroll this time
        CSize sizeDistance; // = ptNow - rectClient.CenterPoint();

        if (ptNow.x > rectClient.right)
            sizeDistance.cx = ptNow.x - rectClient.right;
        else if (ptNow.x < rectClient.left)
            sizeDistance.cx = ptNow.x - rectClient.left;
        else
            sizeDistance.cx = 0;

        if (ptNow.y > rectClient.bottom)
            sizeDistance.cy = ptNow.y - rectClient.bottom;
        else if (ptNow.y < rectClient.top)
            sizeDistance.cy = ptNow.y - rectClient.top;
        else
            sizeDistance.cy = 0;

        CFHZoomView* pView = (CFHZoomView*) GetOwner();

        CSize sizeToScroll = pView->GetWheelScrollDistance(sizeDistance,
            m_nAnchorID == AFX_IDC_MOUSE_ORG_HV || m_nAnchorID == AFX_IDC_MOUSE_ORG_HORZ,
            m_nAnchorID == AFX_IDC_MOUSE_ORG_HV || m_nAnchorID == AFX_IDC_MOUSE_ORG_VERT );

        // hide ourselves to minimize flicker
        ShowWindow(SW_HIDE);

        CWnd* pViewParent = pView->GetParent();
        CSplitterWnd* pSplitter = DYNAMIC_DOWNCAST(CSplitterWnd, pViewParent);

        // if it is in a splitter, then we need to handle it accordingly
        if (pSplitter == NULL)
        {
            // scroll the view
            pView->OnScrollBy(sizeToScroll, TRUE);
        }
        else
        {
            // ask the splitter to scroll
            pSplitter->DoScrollBy(pView, CSize(sizeToScroll.cx,sizeToScroll.cy), TRUE);
        }	

        // restore ourselves and repaint
        // SetFocus();
        UpdateWindow();

        // move ourselves back (since we're a child, we scroll too!)
        SetWindowPos(&CWnd::wndTop,
            m_ptAnchor.x - AFX_CX_ANCHOR_BITMAP/2,
            m_ptAnchor.y - AFX_CY_ANCHOR_BITMAP/2, 0, 0,
            SWP_NOACTIVATE | SWP_NOSIZE | SWP_SHOWWINDOW);
    }
}

void CMouseAnchorWnd::SetBitmap(UINT nID)
{
    HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(nID), RT_GROUP_CURSOR);
    ASSERT(hInst != NULL);
    m_hAnchorCursor = ::LoadCursor(hInst, MAKEINTRESOURCE(nID));
    m_nAnchorID = nID;
}

BOOL CMouseAnchorWnd::Create(CFHZoomView* pParent)
{
    ASSERT(pParent != NULL);
    ASSERT_KINDOF(CFHZoomView, pParent);

    pParent->ClientToScreen(&m_ptAnchor);

    m_rectDrag.top = m_ptAnchor.y - GetSystemMetrics(SM_CYDOUBLECLK);
    m_rectDrag.bottom = m_ptAnchor.y + GetSystemMetrics(SM_CYDOUBLECLK);
    m_rectDrag.left = m_ptAnchor.x - GetSystemMetrics(SM_CXDOUBLECLK);
    m_rectDrag.right = m_ptAnchor.x + GetSystemMetrics(SM_CXDOUBLECLK);

    BOOL bRetVal = 
        CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST,
        AfxRegisterWndClass(CS_SAVEBITS),
        NULL,
        WS_POPUP,
        m_ptAnchor.x - AFX_CX_ANCHOR_BITMAP/2,
        m_ptAnchor.y - AFX_CY_ANCHOR_BITMAP/2,
        AFX_CX_ANCHOR_BITMAP, AFX_CY_ANCHOR_BITMAP,
        NULL, NULL);
    SetOwner(pParent);

    if (bRetVal)
    {
        // set window's region for round effect
        CRgn rgn;
        rgn.CreateEllipticRgn(0, 0, AFX_CX_ANCHOR_BITMAP, AFX_CY_ANCHOR_BITMAP);
        SetWindowRgn(rgn, TRUE);

        // fire timer ever 50ms
        SetCapture();
        SetTimer(ID_FH_TIMER_TRACKING, 50, NULL);
    }
#ifdef _DEBUG
    else
    {
        DWORD dwLastError = GetLastError();
        TRACE(traceAppMsg, 0, "Failed to create mouse anchor window! Last error is 0x%8.8X\n",
            dwLastError);
    }
#endif

    return bRetVal;
}

void CMouseAnchorWnd::OnPaint()
{
    CPaintDC dc(this);
    CRect rect;
    GetClientRect(&rect);

    // shrink a pixel in every dimension for border
    rect.DeflateRect(1, 1, 1, 1);
    dc.Ellipse(rect);

    // draw anchor shape
    dc.DrawIcon(0, 0, m_hAnchorCursor);
}
