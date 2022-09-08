#include "stdafx.h"
#include "rectclipper.h"

CRectClipper::CRectClipper(void)
{
	m_pPolygon=0;
	m_nPolygonPtQty=0;
}

CRectClipper::~CRectClipper(void)
{
	if(m_pPolygon)
	{
		delete []m_pPolygon;
		m_pPolygon=NULL;
	}

	m_result.RemoveAll();
}

void CRectClipper::SetRect(CMyRect &rect)
{
	m_rect=rect;
	m_rect.Normalize ();
}

bool CRectClipper::SetPolygon(CMyPoint *pPolygon,long nPointQty)
{
	if(nPointQty<3||nPointQty>10000)return false;

	if(m_pPolygon)
	{
		delete []m_pPolygon;
		m_pPolygon=NULL;
	}
	m_pPolygon=new CMyPoint [nPointQty+5];
	for(long i=0;i<nPointQty;i++)
	{
		m_pPolygon[i]=pPolygon[i];
	}

	m_nPolygonPtQty=nPointQty;

	return true;
}

bool CRectClipper::SetPolygon(CMyRect &rect)
{
	if(m_pPolygon)
	{
		delete []m_pPolygon;
		m_pPolygon=NULL;
	}
	m_pPolygon=new CMyPoint [5];
	m_pPolygon[0].x=rect.m_dLeft;
	m_pPolygon[0].y=rect.m_dLow;
	m_pPolygon[1].x=rect.m_dRight;
	m_pPolygon[1].y=rect.m_dLow;
	m_pPolygon[2].x=rect.m_dRight;
	m_pPolygon[2].y=rect.m_dHigh;
	m_pPolygon[3].x=rect.m_dLeft;
	m_pPolygon[3].y=rect.m_dHigh;
	
	m_pPolygon[4]=m_pPolygon[0];


	m_nPolygonPtQty=5;

	return true;
}



CMyObList * CRectClipper::GetResult()
{
	CMyObList list2;

	for(int i=0;i<m_nPolygonPtQty;i++)
	{
		m_result.AddHead (new CMyPoint(m_pPolygon[i].x,m_pPolygon[i].y));
	}

	if(!ClipSingleEdge(m_rect.m_dRight ,right, m_result, list2))return NULL;
	int n=list2.GetCount ();
	if(n<4)
	{
		TRACE("Error: point qty is %ld\n",n);
		return NULL;
	}

	CMyPoint *pHead=(CMyPoint*)list2.GetHead ();
	CMyPoint *pTail=(CMyPoint*)list2.GetTail();
	if(*pHead!=*pTail)
	{
		list2.AddTail (new CMyPoint(pHead->x,pHead->y));
	}


	if(!ClipSingleEdge(m_rect.m_dLeft  ,left , list2, m_result))return NULL;
	n=m_result.GetCount ();
	if(n<4)
	{
		TRACE("Error: point qty is %ld\n",n);
		return NULL;
	}


	pHead=(CMyPoint*)m_result.GetHead ();
	pTail=(CMyPoint*)m_result.GetTail();
	if(*pHead!=*pTail)
	{
		m_result.AddTail (new CMyPoint(pHead->x,pHead->y));
	}

	if(!ClipSingleEdge(m_rect.m_dLow   ,low  , m_result, list2))return NULL;
	n=list2.GetCount ();
	if(n<4)
	{
		TRACE("Error: point qty is %ld\n",n);
		return NULL;
	}


	pHead=(CMyPoint*)list2.GetHead ();
	pTail=(CMyPoint*)list2.GetTail();
	if(*pHead!=*pTail)
	{
		list2.AddTail (new CMyPoint(pHead->x,pHead->y));
	}


	if(!ClipSingleEdge(m_rect.m_dHigh  ,high , list2, m_result))return NULL;
	n=m_result.GetCount ();
	if(n<4)
	{
		TRACE("Error: point qty is %ld\n",n);
		return NULL;
	}


	pHead=(CMyPoint*)m_result.GetHead ();
	pTail=(CMyPoint*)m_result.GetTail();
	if(*pHead!=*pTail)
	{
		m_result.AddTail (new CMyPoint(pHead->x,pHead->y));
	}


	return &m_result;
}

CMyRect CRectClipper::GetResultRect()
{
	CMyObList *plist=GetResult();
	if(!plist)
	{
		//AfxMessageBox("错误：裁剪错误！");
		return CMyRect(0,0,0,0);
	}

	int n=plist->GetCount ();
	if(n<4)
	{
		return CMyRect(0,0,0,0);
		//AfxMessageBox("错误：裁减出来的四边形顶点数目不等于四个！");
		//ASSERT(false);
	}

	CMyRect rect;
	
	POSITION pos=plist->GetHeadPosition ();

	CMyPoint *pPoint = (CMyPoint*)plist->GetNext (pos);
	rect.m_dLeft = pPoint->x;
	rect.m_dLow  = pPoint->y;

	pPoint = (CMyPoint*)plist->GetNext (pos);
	pPoint = (CMyPoint*)plist->GetNext (pos);
	rect.m_dRight = pPoint->x;
	rect.m_dHigh  = pPoint->y;

	return rect;
}

bool CRectClipper::ClipSingleEdge(double dEdge,RectEdge edgeType, CMyObList &listIn, CMyObList &listOut)
{
	bool bDiffSides;
	bool bInside;

	listOut.RemoveAll ();
	int n=listIn.GetCount ();
	if(n<3)
	{
		//AfxMessageBox("错误：多边形少于三个顶点！");
		return false;
	}

	double x1,y1,x2,y2,xIntersect,yIntersect;

	POSITION pos=listIn.GetHeadPosition ();
	CMyPoint *pPoint=(CMyPoint *)listIn.GetNext(pos);
	x1=pPoint->x;
	y1=pPoint->y;
	x2=x1;
	y2=y1;

	while(true)
	{
		TestIntersect(dEdge,
			edgeType,
			x1,
			y1,
			x2,
			y2,
			xIntersect,
			yIntersect,
			bDiffSides,
			bInside);

		if(bDiffSides)
		{
			listOut.AddHead (new CMyPoint(xIntersect,yIntersect));
		}

		if(bInside)
		{
			listOut.AddHead (new CMyPoint(x2,y2));
		}

		if(!pos)break;
	
		x1=x2;
		y1=y2;
		pPoint=(CMyPoint *)listIn.GetNext(pos);
		x2=pPoint->x;
		y2=pPoint->y;
	}

	return true;
}

void CRectClipper::TestIntersect(double dEdge,
				   RectEdge edgeType,
				   double x1,
				   double y1,
				   double x2,
				   double y2,
				   double &xOut,
				   double &yOut,
				   bool &bDiffSides,
				   bool &bInside)
{
	bInside=false;
	bDiffSides=false;


	switch(edgeType)
	{
	case right:
		if(x2<=dEdge)
		{
			bInside=true;
			if(x1>dEdge)
			{
				bDiffSides=true;
			}
		}
		else if(x1<=dEdge)
		{
			bDiffSides=true;
		}
		break;
	
	case low:
		if(y2>=dEdge)
		{
			bInside=true;
			if(y1<dEdge)
			{
				bDiffSides=true;
			}
		}
		else if(y1>=dEdge)
		{
			bDiffSides=true;
		}
		break;

	case left:
		if(x2>=dEdge)
		{
			bInside=true;
			if(x1<dEdge)
			{
				bDiffSides=true;
			}
		}
		else if(x1>=dEdge)
		{
			bDiffSides=true;
		}
		break;

	case high:
		if(y2<=dEdge)
		{
			bInside=true;
			if(y1>dEdge)
			{
				bDiffSides=true;
			}
		}
		else if(y1<=dEdge)
		{
			bDiffSides=true;
		}
	default:
		break;
	}

	//////////////////////////////////////////////////////
	//前后两点是一个点的情况
	//////////////////////////////////////////////////////
	if(fabs(x2-x1)<0.00000001&&fabs(y2-y1)<0.00000001)
	{
		return;
	}
	
	//////////////////////////////////////////////////////
	//前后两点不是一个点的情况
	//////////////////////////////////////////////////////	
	else
	{
		// 左右两边的情况：
		if(bDiffSides&&(edgeType==left||edgeType==right))
		{
			xOut=dEdge;

			if(fabs(x2-x1)<0.00000001)
			{
				if(y2>=m_rect.m_dHigh)
				{
					yOut=m_rect.m_dHigh;
				}

				else if(y2<=m_rect.m_dLow)
				{
					yOut=m_rect.m_dLow;
				}
			}

			else
			{
				yOut=y1+(y2-y1)/(x2-x1)*(xOut-x1);
			}
		}

		// 前后两边的情况：
		else
		{
			yOut=dEdge;
			if(fabs(y2-y1)<0.00000001)
			{
				if(x2>=m_rect.m_dRight)
				{
					xOut=m_rect.m_dRight;
				}

				else if(x2<=m_rect.m_dLeft)
				{
					xOut=m_rect.m_dLeft;
				}
			}

			else
			{
				xOut=x1+(x2-x1)/(y2-y1)*(yOut-y1);
			}
		}
	}

	return;
}






















