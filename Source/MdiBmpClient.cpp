#include "stdafx.h"
#include "MdiBmpClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMdiBmpClient::CMdiBmpClient()
{
	m_pBmp = NULL;
	m_nBackColor = CLR_INVALID;
	m_nBkType=BK_STRETCH;
}

CMdiBmpClient::~CMdiBmpClient()
{
	if (m_pBmp)
	{
		m_pBmp->DeleteObject();
		delete m_pBmp;
		m_pBmp = NULL;
	}
}

IMPLEMENT_DYNCREATE(CMdiBmpClient, CWnd)

BEGIN_MESSAGE_MAP(CMdiBmpClient, CWnd)
	//{{AFX_MSG_MAP(CMdiBmpClient)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CMdiBmpClient::OnEraseBkgnd(CDC* pDC) 
{
	CRect rect;
	GetClientRect(&rect);

	if (m_nBackColor != CLR_INVALID)
		pDC->FillSolidRect(&rect, m_nBackColor);
	else
		if (m_pBmp != NULL)
		{
			BITMAP bm;
			CDC dcMem;
			VERIFY(m_pBmp->GetObject(sizeof(BITMAP), (LPVOID)&bm));
			dcMem.CreateCompatibleDC(NULL);
			
			CBitmap* pOldBitmap = dcMem.SelectObject(m_pBmp);

			// 拉伸位图以适应背景区
			if(m_nBkType=BK_STRETCH)
			{
				pDC->StretchBlt (0,0,rect.Width (),rect.Height (),&dcMem,0,0,bm.bmWidth,bm.bmHeight ,SRCCOPY  );
			}

			// 下面的代码是排列位图
			else
			{			
				for (register int nX = 0; nX < rect.Width(); nX += bm.bmWidth)
				{
					for (register int nY = 0; nY < rect.Height(); nY += bm.bmHeight)
					{
						pDC->BitBlt(nX, nY, bm.bmWidth, bm.bmHeight, &dcMem, 0, 0, SRCCOPY);
					}
				}
			}

			dcMem.SelectObject(pOldBitmap);
		}
		else
			CWnd::OnEraseBkgnd(pDC);

	return TRUE;
}

COLORREF CMdiBmpClient::SetBackColor(COLORREF nBackColor)
{
	LockWindowUpdate();

	COLORREF cr = m_nBackColor;
	m_nBackColor = nBackColor;

	UnlockWindowUpdate();

	return cr;
}

BOOL CMdiBmpClient::SetBitmap(UINT nID)
{
	BOOL bResult = true;

	LockWindowUpdate();

	if (m_pBmp)
	{
		m_pBmp->DeleteObject();
		delete m_pBmp;
		m_pBmp = NULL;
	}

	m_pBmp = new CBitmap();
	bResult = m_pBmp->LoadBitmap(nID);

	UnlockWindowUpdate();

	return bResult;
}

void CMdiBmpClient::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (m_pBmp != NULL)
		Invalidate();
}

void CMdiBmpClient::SetBkType(enumBkType type)
{
	m_nBkType=type;
}