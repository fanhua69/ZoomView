#include "stdafx.h"
#include "piedrawer.h"
#include "math.h"

CPieDrawer::CPieDrawer(void)
{
	m_nLongRadius=100;
	m_nShortRadius=50;
	m_nHeight=30;
	m_pointCenter.x=100;
	m_pointCenter.y=100;

	m_pPartColor=NULL;
	m_pPartWeight=NULL;
	m_nPartQty=0;

	m_nColorEdge=RGB(0,0,0);	
	m_penEdge.CreatePen (PS_SOLID,0,m_nColorEdge);
}

CPieDrawer::~CPieDrawer(void)
{
}

bool CPieDrawer::SetPartQty(int nPartQty)
{
	if(nPartQty<1||nPartQty>100)return false;
	if(m_pPartColor)
	{
		delete []m_pPartColor;
		m_pPartColor=NULL;
	}
	m_pPartColor=new COLORREF[nPartQty];
	memset(m_pPartColor,0,sizeof(COLORREF)*nPartQty);

	if(m_pPartWeight)
	{
		delete []m_pPartWeight;
		m_pPartWeight=NULL;
	}
	m_pPartWeight=new double[nPartQty];
	memset(m_pPartWeight,0,sizeof(double)*nPartQty);

	m_nPartQty=nPartQty;

	return true;

}

bool CPieDrawer::SetColor(int nOrder, COLORREF nColor)
{
	if(nOrder<0||nOrder>=m_nPartQty||!m_pPartColor)
	{
		return false;
	}
	m_pPartColor[nOrder]=nColor;

	return true;
}

bool CPieDrawer::SetPartWeight(int nOrder, double dWeight)
{
	if(nOrder<0||nOrder>=m_nPartQty||!m_pPartWeight)
	{
		return false;
	}
	m_pPartWeight[nOrder]=dWeight;

	return true;
}



void CPieDrawer::SetPar(CPoint pointCenter, 
						int nLongRadius,
						int nShortRadius,
						int nHeight)
{
	m_nLongRadius=nLongRadius;
	m_nShortRadius=nShortRadius;

	if(m_nLongRadius<=0)
	{
		m_nLongRadius=10;
		m_nShortRadius=6;
	}
	else if(m_nLongRadius<10)
	{
		float f=m_nShortRadius/m_nLongRadius;
		m_nLongRadius=10;
		m_nShortRadius*=f;
	}

	m_pointCenter=pointCenter;
	m_nHeight=nHeight;
	if(m_nHeight<0)
	{
		m_nHeight=5;
	}

	m_rectPie.top		= m_pointCenter.y - m_nShortRadius;
	m_rectPie.bottom	= m_pointCenter.y + m_nShortRadius;
	m_rectPie.left	= m_pointCenter.x - m_nLongRadius;
	m_rectPie.right	= m_pointCenter.x + m_nLongRadius;
}

void CPieDrawer::SetPar(CRect rect,int nHeight)
{
	m_pointCenter.x =( rect.right - rect.left	)/2+rect.left ;
	m_pointCenter.y =( rect.bottom- rect.top	)/2+rect.top;
	
	m_nLongRadius=rect.Width ()/2;	
	m_nShortRadius=rect.Height ()/2;
	
	if(m_nLongRadius<=0)
	{
		m_nLongRadius=10;
		m_nShortRadius=6;
	}
	else if(m_nLongRadius<10)
	{
		float f=m_nShortRadius/m_nLongRadius;
		m_nLongRadius=10;
		m_nShortRadius*=f;
	}

	m_rectPie=rect;

	m_nHeight=nHeight;
}

void CPieDrawer::Draw(CDC *pDC)
{
	ASSERT_VALID(pDC);
	
	/////////////////////////////////////
	// 
	DrawVerLines(pDC);
	DrawUpperPie(pDC);
	DrawEdge(pDC);
}

void CPieDrawer::DrawUpperPie(CDC *pDC)
{
	float fTotalQty=0.0;
	for(int i=0;i<m_nPartQty;i++)
	{
		fTotalQty+=m_pPartWeight[i];
	}

	float fStartPer=0,fEndPer=0,fPer=0;
	CPoint pointStart , pointEnd;

	for(int i=0;i<m_nPartQty;i++)
	{
		if(fabs(m_pPartWeight[i])<0.00001)continue;
		fEndPer=fStartPer+m_pPartWeight[i]/fTotalQty*100.0;
		if(fEndPer>100)
		{	
			fEndPer=100;
		}

		CBrush brush(m_pPartColor[i]);
		CBrush *pOldBrush =	pDC->SelectObject (&brush );
		
		pointStart = GetPerPoint(fStartPer,	m_nLongRadius,m_nShortRadius,m_pointCenter);	 
		pointEnd   = GetPerPoint(fEndPer,	m_nLongRadius,m_nShortRadius,m_pointCenter);	 

		pDC->Pie(m_rectPie,pointEnd,pointStart);

		pDC->SelectObject (pOldBrush);

		fStartPer=fEndPer;
	}

	return;
}



CPoint CPieDrawer::GetPerPoint(float fPer, int iLongRadius, int iShortRadius, CPoint pointCenter)
{
	CPoint	pointReturn;
	int		iDX;
	int		iDY;
	float 	fTmpPer= 0;
	
	
	if(fPer<= 25)
	{
		fTmpPer = (fPer * 100) / 25;	
	}
	else if((fPer > 25) && (fPer <=  50))
	{
		fTmpPer = ((50 - fPer) * 100) / 25;
	}
	else if((fPer > 50) && (fPer <=  75))
	{
		fTmpPer = ((fPer - 50) * 100) / 25;
	}
	else
	{
		fTmpPer = ((100 - fPer) * 100) / 25;		
	}
	
	iDX = ( fTmpPer * iLongRadius) / 100 ; 
	
	iDY = (sqrt(pow((float)iLongRadius,2) - pow((float)iDX,2)) * iShortRadius) / iLongRadius;
	
	if(fPer <= 50)
	{
		pointReturn.x = pointCenter.x - iDX;
	}
	else
	{
		pointReturn.x = pointCenter.x + iDX;
	}

	if((fPer > 25 ) && (fPer <= 75)) 
	{
		pointReturn.y = pointCenter.y - iDY;
	}
	else
	{
		pointReturn.y = pointCenter.y + iDY;
	}

	pointReturn.x--;
	return pointReturn;
}

///////////////////////////////////
// Draw Edge Vertical lines:
void CPieDrawer::DrawVerLines(CDC *pDC)
{
	float fAngleStep=1.0/m_nLongRadius;
	float fTotalQty=0;
	int i;
	for(i=0;i<m_nPartQty;i++)
	{
		fTotalQty+=m_pPartWeight[i];
	}

	float fStartPer=0,fEndPer=0,fPer=0;
	for( i=0;i<m_nPartQty;i++)
	{
		fEndPer=fStartPer+m_pPartWeight[i]/fTotalQty*100.0;
		if(fStartPer<25||fStartPer>75||fEndPer<25||fEndPer>75)
		{
			CPen pen(PS_SOLID,0,RGB(GetRValue(m_pPartColor[i])*0.6,
									GetGValue(m_pPartColor[i])*0.6,
									GetBValue(m_pPartColor[i])*0.6));

			CPen *pOldPen	=	pDC->SelectObject (&pen);

			for( fPer=fStartPer;fPer<=fEndPer;fPer+=fAngleStep)
			{
				if(fPer>25&&fPer<75)continue;
				DrawVerLine(pDC, fPer);
			}
			pDC->SelectObject (pOldPen);
		}
		fStartPer=fEndPer;
	}

	fPer=0;
	CPen *pOldPen=pDC->SelectObject (&m_penEdge);
	for(i=0;i<m_nPartQty;i++)
	{
		fPer+=m_pPartWeight[i]/fTotalQty*100.0;
		if(fPer<25||fPer>75)
		{
			DrawVerLine(pDC, fPer);
		}		
	}
	pDC->SelectObject (pOldPen);

}

void CPieDrawer::DrawEdge(CDC *pDC)
{
	/////////////////////////////////////
	// Draw the special 2 sides Lines:
	CPen *pOldPen=pDC->SelectObject (&m_penEdge);
	int x1=m_pointCenter.x-m_nLongRadius;
	int x2=m_pointCenter.x+m_nLongRadius-1;
	int y1=m_pointCenter.y;
	int y2=m_pointCenter.y+m_nHeight;

	CRect rect(x1,y2-m_nShortRadius,x2,y2+m_nShortRadius);
	
	pDC->MoveTo(x1,y1);
	pDC->LineTo(x1,y2);

	pDC->MoveTo(x2,y1);
	pDC->LineTo(x2,y2);

	pDC->MoveTo(m_pointCenter.x-1,m_pointCenter.y+m_nShortRadius);
	pDC->LineTo(m_pointCenter.x-1,m_pointCenter.y+m_nShortRadius+m_nHeight);

	pDC->Arc (rect,CPoint(x1,y2),CPoint(x2,y2));

	pDC->SelectObject (pOldPen);
}

void CPieDrawer::DrawVerLine(CDC *pDC, float fPer)
{
    CPoint pt= GetPerPoint(fPer,m_nLongRadius,m_nShortRadius,m_pointCenter);
	pDC->MoveTo(pt.x,pt.y-1);
	pDC->LineTo(pt.x,pt.y+m_nHeight);
}
