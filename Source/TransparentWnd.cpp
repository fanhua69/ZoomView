// TranparentWnd.cpp : implementation file
//

#include "stdafx.h"
#include "zoomview.h"
#include "TransparentWnd.h"
#include "..\include\transparentwnd.h"



// CTransparentWnd dialog

IMPLEMENT_DYNAMIC(CTransparentWnd, CDialog)
CTransparentWnd::CTransparentWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CTransparentWnd::IDD, pParent)
{
	m_nBkColor=RGB(200,0,0);
}

CTransparentWnd::~CTransparentWnd()
{
}

void CTransparentWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTransparentWnd, CDialog)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CTransparentWnd message handlers

BOOL CTransparentWnd::OnInitDialog()
{
	CDialog::OnInitDialog();

	//////////////////////////////////////
	// 使窗体半透明，只有在2000以上才能用：
	SetWindowLong(GetSafeHwnd(),GWL_EXSTYLE,
		GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);

	HINSTANCE hInst=LoadLibrary("User32.DLL");

	if(hInst)
	{
		typedef BOOL (WINAPI* MYFUNC)(HWND,COLORREF,BYTE,DWORD);
		MYFUNC fun=NULL;

		// 取得SetLayeredWindowAttributes函数指针
		fun=(MYFUNC)GetProcAddress(hInst,"SetLayeredWindowAttributes");
		if(fun)fun(GetSafeHwnd(),RGB(120,0,0),128,LWA_ALPHA);

		FreeLibrary(hInst);
	}
	
	return TRUE;  
}

void CTransparentWnd::SetRgn(CMyRgn *pRgn,CPoint pointOff)
{
	CMyObList *pList=pRgn->GetRgnPoints ();
	CPoint *pPoints=new CPoint[pList->GetCount ()+10];

	POSITION pos=pList->GetHeadPosition ();
	CMyPoint *pMyPoint=NULL;
	int n=0;
	while(pos)
	{
		pMyPoint=(CMyPoint*)pList->GetNext (pos);
		pPoints[n].x=pMyPoint->x-pointOff.x;
		pPoints[n].y=pMyPoint->y-pointOff.y;
		n++;
	}

	m_rgn.Detach ();
	bool b= m_rgn.CreatePolygonRgn (pPoints,n,WINDING);	

	//////////////////////////////////////
	//
	int mm=::SetWindowRgn (GetSafeHwnd(),(HRGN)m_rgn,TRUE);

	
	delete []pPoints;

	return ;
}


BOOL CTransparentWnd::OnEraseBkgnd(CDC* pDC)
{
	CBrush brush;
	brush.CreateSolidBrush (m_nBkColor);
	CBrush *pOldBrush=pDC->SelectObject (&brush);
	CRect rc;
	GetClientRect(&rc);
	pDC->Rectangle (&rc);
	pDC->SelectObject(pOldBrush);

	return true;
	//return CDialog::OnEraseBkgnd(pDC);
}

void CTransparentWnd::SetBackColor(COLORREF nColor)
{
	m_nBkColor=nColor;
}
