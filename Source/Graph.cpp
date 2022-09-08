// Graph.cpp : implementation file
//

#include "stdafx.h"
#include "FHCGlobal.h"
#include "GraphSeries.h"
#include "GraphLegend.h"
#include "math.h"
#include "Graph.h"
#include "PieDrawer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraph

CGraph::CGraph()
{
	m_listGraphSeries = new CObList();
	m_fTickRange = 100;
	m_iSeriesSize = 0;

	m_bGaphHasLegend = FALSE;
	m_iLegendWidth = 0;

	m_iGraphType = 0;

	m_clrDataColor[0] = RGB(0,255,0);  //green
	m_clrDataColor[1] = RGB(0,0,255);  //blue
	m_clrDataColor[2] = RGB(255,0,0);  //red
	m_clrDataColor[3] = RGB(255,255,0);  //yellow
	m_clrDataColor[4] = RGB(255,153,51);  //orange
	m_clrDataColor[5] = RGB(255,51,153);  //hot pink
	m_clrDataColor[6] = RGB(153,0,204);  //purple
	m_clrDataColor[7] = RGB(0,255,255);  //cyan
	m_clrDataColor[8] = RGB(0,0,0);  //black
	m_clrDataColor[9] = RGB(255,255,255);  //white

	m_iXAxisAlign = 0;  //horizontal
	m_ixAxisLabelLength = 0;
	m_nTitleHeight=50;

	m_fontTitle.CreateFont (20,0,0,0,0,false,false,false,0,0,0,0,0,"ËÎÌå");
	m_fontText.CreateFont (16,0,0,0,0,false,false,false,0,0,0,0,0,"ËÎÌå");

}

CGraph::CGraph(int type)
{
	m_listGraphSeries = new CObList();
	m_fTickRange = 100;
	m_iSeriesSize = 0;

	m_bGaphHasLegend = FALSE;
	m_iLegendWidth = 0;

	m_iGraphType = type;

	m_clrDataColor[0] = RGB(0,255,0);  //green
	m_clrDataColor[1] = RGB(0,0,255);  //blue
	m_clrDataColor[2] = RGB(255,0,0);  //red
	m_clrDataColor[3] = RGB(255,255,0);  //yellow
	m_clrDataColor[4] = RGB(255,153,51);  //orange
	m_clrDataColor[5] = RGB(255,51,153);  //hot pink
	m_clrDataColor[6] = RGB(153,0,204);  //purple
	m_clrDataColor[7] = RGB(0,255,255);  //cyan
	m_clrDataColor[8] = RGB(0,0,0);  //black
	m_clrDataColor[9] = RGB(255,255,255);  //white

	m_iXAxisAlign = 0;  //horizontal
	m_ixAxisLabelLength = 0;
}

CGraph::~CGraph()
{
}


BEGIN_MESSAGE_MAP(CGraph, CStatic)
	//{{AFX_MSG_MAP(CGraph)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraph message handlers

void CGraph::SetTickSpace(float yDist)
{
	m_fTickSpace = yDist;
}

void CGraph::SetTickRange(float maxTick)
{
	m_fTickRange = maxTick;
}

void CGraph::SetGraphType(int gType)
{
	m_iGraphType = gType;
}

void CGraph::SetXAxisAlignment(int alignValue)
{
	m_iXAxisAlign = alignValue;
}

int CGraph::GetXAxisAlignment()
{
	return m_iXAxisAlign;
}

void CGraph::DrawGraph(CDC* pDC,CRect graphRect)
{
	m_rectGraph=graphRect;

	CString tickLabel;
	
	m_iMaxHeight = graphRect.Height() - 20;  //for frame window and status bar
	m_iMaxWidth = graphRect.Width() - 5;  //for frame window
	//We will leave 5 pixels blank on all sides of the graph.  So
	//top-left side of graph is at 5,5 and the bottom-right side of
	//graph is at ((m_iMaxHeight - 5), (m_iMaxWidth - 5))
	//these settings are altered by axis labels and legends.

	//draw graph title
	CFont* pOldFont = (CFont*) pDC->SelectObject(&m_fontTitle);

	pDC->TextOut((m_iMaxWidth / 2) - ((m_sGraphTitle.GetLength() * 16) / 2),
				m_rectGraph.top, m_sGraphTitle);
	pDC->SelectObject(pOldFont);

	if(m_iGraphType == 2)  //pie
	{

		//since pie has not axis lines, set to full size minus 5 on each side
		//these are needed for legend to plot itself
		m_iXAxisWidth = m_iMaxWidth - 10;
		m_iYAxisHeight = m_iMaxHeight - 20;  //10 buffer and 20 for title
		m_iXApexPoint = 5;
		m_iYApexPoint = m_iMaxHeight - 5;
	}
	else
	{
		//X-axis will be raised by 20 pixels for tick-labels
		//Y-axis will be shifted by (max(tick-label size) * 10) + label * 10
		
		tickLabel=vtos(m_fTickRange);

		//determine axis specifications 
			//tick label offset is 4 (for the tick) + 6 (total 10) from axis line
		m_iXApexPoint = 5 + (tickLabel.GetLength() * 8) + 10 + 30/*(m_sAxisYLabel.GetLength() * 8)*/ + 5; //allowing 8 pixels per char in tick label
			//y apex based on 5 + 15 (x label) + 4 (for the tick) + 4 (text below tick) + 12 (tick label) + 10
		if(!m_iXAxisAlign)  //horizontal
			m_iYApexPoint = (m_iMaxHeight - 5) - 45;		//apex points are the cross section of axis lines
		else
			m_iYApexPoint = (m_iMaxHeight - 5) - (m_ixAxisLabelLength * 12) - 10;
		m_iYAxisHeight = m_iYApexPoint - 40;
		m_iXAxisWidth = (m_iMaxWidth - 5) - m_iXApexPoint;
	}

	//draw legend
	if(m_bGaphHasLegend)
	{
		DrawLegend(pDC);
	}

	if(m_iGraphType != 2&&m_iGraphType != 3)  //pie
	{
		//draw axis lines
		DrawAxis(pDC);
	}

	//draw series data and labels
	DrawSeries(pDC);

}

void CGraph::DrawAxis(CDC* pDC)
{
	pDC->SetTextColor(RGB(0,0,0));

	//draw y axis
	pDC->MoveTo(m_iXApexPoint, m_iYApexPoint);  
	pDC->LineTo(m_iXApexPoint, m_iYApexPoint - m_iYAxisHeight);

	//draw x axis
	pDC->MoveTo(m_iXApexPoint, m_iYApexPoint);  
	if(m_bGaphHasLegend)
		pDC->LineTo(m_iXApexPoint + (m_iXAxisWidth - m_iLegendWidth - 10), m_iYApexPoint);
	else
		pDC->LineTo(m_iXApexPoint + m_iXAxisWidth, m_iYApexPoint);

	//draw labels
	CFont* pOldFont = (CFont*) pDC->SelectObject(&m_fontText);

	pDC->TextOut(10, (m_iMaxHeight / 2) - 6, m_sAxisYLabel);
	pDC->TextOut(m_iXApexPoint + (m_iXAxisWidth / 2) - (m_sAxisXLabel.GetLength() * 19), m_iMaxHeight - 5 - 6, m_sAxisXLabel);

	//to allow scalability (height may be less than m_fTickRange)
	double tickScale = 0.00;
	if(m_fTickRange > m_iYAxisHeight)
		tickScale = ((m_iYAxisHeight * 1.00) / (m_fTickRange * 1.00)) * m_fTickSpace;
	else tickScale = m_fTickSpace;

	//draw y axis ticks
	for(int y = 1; y <= m_fTickRange / m_fTickSpace; y++)  //no tick at 0
	{
		int tickYLocation = m_iYApexPoint - (y * tickScale);

		pDC->MoveTo(m_iXApexPoint - 4, tickYLocation);
		pDC->LineTo(m_iXApexPoint + 4, tickYLocation);

		//draw tick label
		CString tickLabel;
		tickLabel.Format("%d", y * m_fTickSpace);

		pDC->TextOut(m_iXApexPoint - 10 - (tickLabel.GetLength() * 8), tickYLocation - 6, tickLabel);

	}

	//draw X axis tick marks
	POSITION pos;
	pos = m_listGraphSeries->GetHeadPosition();
	CGraphSeries* tmpSeries;
	for(int x = 1; x <= m_listGraphSeries->GetCount(); x++)
	{
		tmpSeries = (CGraphSeries*)m_listGraphSeries->GetNext(pos);

		int seriesSpace;
		int tickXLocation;
		if(m_bGaphHasLegend)
			seriesSpace= (m_iXAxisWidth - m_iLegendWidth - 10) / m_listGraphSeries->GetCount();
		else
			seriesSpace= m_iXAxisWidth / m_listGraphSeries->GetCount();
		tickXLocation = m_iXApexPoint + ((x * seriesSpace) - (seriesSpace / 2));

		pDC->MoveTo(tickXLocation,m_iYApexPoint - 4);
		pDC->LineTo(tickXLocation,m_iYApexPoint + 4);

		//draw tick label
		CString tickLabel;
		tickLabel = tmpSeries->GetLabel();
		if(!m_iXAxisAlign)  //horizontal
			pDC->TextOut(tickXLocation - ((tickLabel.GetLength() * 8) / 2), m_iYApexPoint + 8, tickLabel);
		else
		{
			CFont sideFont2;
			sideFont2.CreateFont(16, 0, (m_iXAxisAlign * 10), 0, 700, FALSE, FALSE, 0,
				ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,"Arial");
			pOldFont = (CFont*) pDC->SelectObject(&sideFont2);
			if(m_iXAxisAlign < 180)
				pDC->TextOut(tickXLocation - 8, m_iYApexPoint + 8 + (m_ixAxisLabelLength * 8), tickLabel);
			else
				pDC->TextOut(tickXLocation + 8, m_iYApexPoint + 8, tickLabel);

			pDC->SelectObject(pOldFont);
		}
	}

	pDC->SelectObject(pOldFont);


}

void CGraph::AddSeries(CGraphSeries* dataSet)
{
	int numData = dataSet->GetSeriQty();
	for(int i = 0; i < numData ; i++)
	{
		if(dataSet->GetLabel().GetLength() > m_ixAxisLabelLength)
		{
			m_ixAxisLabelLength = dataSet->GetLabel().GetLength();
		}
	}
	if(numData > m_iSeriesSize)
	{
		m_iSeriesSize = numData;
	}

	m_listGraphSeries->AddTail(dataSet);
}

void CGraph::RemoveAll()
{
	m_ixAxisLabelLength=0;
	m_iSeriesSize=0;

	POSITION posPre;
	POSITION pos=m_listGraphSeries->GetHeadPosition();
	CObject *pObject=NULL;
	while(pos)
	{
		posPre=pos;
		pObject=m_listGraphSeries->GetNext(pos);
		if(pObject)
		{
			delete pObject;
			m_listGraphSeries->SetAt(posPre,NULL);
		}
	}

	m_listGraphSeries->RemoveAll ();	
}


void CGraph::SetXAxisLabel(CString label)
{
	m_sAxisXLabel = label;
}
void CGraph::SetYAxisLabel(CString label)
{
	m_sAxisYLabel = label;
}

void CGraph::SetColor(int group, COLORREF groupColor)
{
	m_clrDataColor[group] = groupColor;
}

COLORREF CGraph::GetColor(int group)
{
	return m_clrDataColor[group];
}

void CGraph::DrawSeries(CDC* pDC)
{
	int barWidth;
	int dataPlotSize;   //used to plot rects of data
	int barL, barT, barR, barB;
	int tickXLocation;
	int seriesSpace;
	double barHeight;  //for scalability
	POSITION pos;
	CGraphSeries* tmpSeries;

	CFont *pOldFont = pDC->SelectObject (&m_fontText);
	if(m_iGraphType == 0) //bar
	{
		//determine width of barchart data blocks
		if(m_bGaphHasLegend)
			seriesSpace = (m_iXAxisWidth - m_iLegendWidth - 10) / m_listGraphSeries->GetCount();
		else
			seriesSpace = m_iXAxisWidth / m_listGraphSeries->GetCount();
		barWidth = (seriesSpace * .6) / m_iSeriesSize;
		dataPlotSize = m_iSeriesSize * barWidth;

		pos = m_listGraphSeries->GetHeadPosition();

		for(int x = 1; x <= m_listGraphSeries->GetCount(); x++)
		{
			tmpSeries = (CGraphSeries*)m_listGraphSeries->GetNext(pos);

			tickXLocation = m_iXApexPoint + ((x * seriesSpace) - (seriesSpace / 2));

			for(int s = 0; s < m_iSeriesSize; s++)
			{
				barHeight = 0.00;
				//to allow scalability (height may be less than m_fTickRange)
				double dataScale = 0.00;
				dataScale = (m_iYAxisHeight * 1.00) / (m_fTickRange * 1.00);
				barHeight = (tmpSeries->GetData(s) * 1.00) * dataScale;
				barL = tickXLocation - (dataPlotSize / 2) + (s * barWidth)+50;
				barT = m_iYApexPoint - barHeight+50;
				barR = barL + barWidth;
				barB = m_iYApexPoint;
				
				COLORREF barColor;
				barColor = GetColor(s);
				CBrush brush (barColor);
				CBrush* pOldBrush;
				pOldBrush = pDC->SelectObject(&brush);
				pDC->Rectangle(barL, barT, barR, barB);
				pDC->SelectObject(&pOldBrush);
			}
		}
	}

	if(m_iGraphType == 1)  //line
	{
		int lastXLoc, lastYLoc;
		for(int s = 0; s < m_iSeriesSize; s++)
		{
			//determine width of barchart data blocks
			if(m_bGaphHasLegend)
				seriesSpace = (m_iXAxisWidth - m_iLegendWidth - 10) / m_listGraphSeries->GetCount();
			else
				seriesSpace = m_iXAxisWidth / m_listGraphSeries->GetCount();
			barWidth = (seriesSpace * .6) / m_iSeriesSize;
			dataPlotSize = m_iSeriesSize * barWidth;

			pos = m_listGraphSeries->GetHeadPosition();

			for(int x = 1; x <= m_listGraphSeries->GetCount(); x++)
			{
				tmpSeries = (CGraphSeries*)m_listGraphSeries->GetNext(pos);

				tickXLocation = m_iXApexPoint + ((x * seriesSpace) - (seriesSpace / 2));
				
				barHeight = 0.00;
				//to allow scalability (height may be less than m_fTickRange)
				double dataScale = 0.00;
				if(m_fTickRange > m_iYAxisHeight)
					dataScale = (m_iYAxisHeight * 1.00) / (m_fTickRange * 1.00);
				else dataScale = 1;
				barHeight = (tmpSeries->GetData(s) * 1.00) * dataScale;

				int yDataLocation = m_iYApexPoint - barHeight;
				
				//now have x and y location of center of ellipse
				COLORREF barColor;
				barColor = GetColor(s);
				CBrush brush (barColor);
				CBrush* pOldBrush;
				pOldBrush = pDC->SelectObject(&brush);
				//draw line back to last data member
				if(x > 1)
				{
					CPen* pOldPen;
					CPen linePen (PS_SOLID, 1, barColor);
					pOldPen = pDC->SelectObject(&linePen);
					pDC->MoveTo(lastXLoc + 2, lastYLoc - 1);
					pDC->LineTo(tickXLocation - 3, yDataLocation - 1);
					pDC->SelectObject(pOldPen);
				}
				//now draw ellipse...
				pDC->Ellipse(tickXLocation - 3, yDataLocation - 3,
					tickXLocation + 3, yDataLocation + 3);
				lastXLoc = tickXLocation;
				lastYLoc = yDataLocation;
				pDC->SelectObject(&pOldBrush);
			}
		}
	}

	if(m_iGraphType == 2)  //pie
	{
		double dataSum = 0.00;  //for storing cumulative sum
		int lastXLocation, lastYLocation;
		int newXLocation, newYLocation;
		double percent = 0.00;
		int degrees;
		double totalSum = 0.00;
		int deltaXY;
		int radius;
		int x,s;
			
		lastXLocation = 0;
		lastYLocation = 0;

		//determine width of pie display area
		if(m_bGaphHasLegend)
			if(((m_iXAxisWidth - m_iLegendWidth - 10) / m_listGraphSeries->GetCount())
					> (m_iYAxisHeight - 10))
				seriesSpace = (m_iYAxisHeight - 10) / m_listGraphSeries->GetCount();
			else
				seriesSpace = (m_iXAxisWidth - m_iLegendWidth - 10) / m_listGraphSeries->GetCount();
		else
			if(m_iXAxisWidth > m_iYAxisHeight)
				seriesSpace = m_iYAxisHeight / m_listGraphSeries->GetCount();
			else
				seriesSpace = m_iXAxisWidth / m_listGraphSeries->GetCount();
		double tmpD = (seriesSpace - 60)* .9;  //max width of any pie
		radius = tmpD / 2;  

		int centerYPie = m_iYAxisHeight / 2;
		
		pos = m_listGraphSeries->GetHeadPosition();
		for(x = 1; x <= m_listGraphSeries->GetCount(); x++)
		{
			tmpSeries = (CGraphSeries*)m_listGraphSeries->GetNext(pos);
			totalSum = 0;
			for(s = 0; s < 10; s++)
				totalSum += tmpSeries->GetData(s);
			int pieLeft, pieRight;
			if(m_listGraphSeries->GetCount() == 1)
			{
				pieLeft = ((m_iXAxisWidth - m_iLegendWidth - 10) / 2) - radius - 25;
			}
			else
			{
				pieLeft = m_iXApexPoint + 25 + (x * 50) + ((x - 1) * (2 * radius));
			}
			pieRight = pieLeft + (2 * radius);
			CRect pieRect (pieLeft, 
					centerYPie - radius,
					pieRight, 
					centerYPie + radius);
			int centerXPie = pieLeft + radius;

			//plot series label
			CRect rectLabel(pieRect.left ,pieRect.bottom,pieRect.right ,pieRect.bottom +100);
			
			// Added By FanHua:
			pDC->DrawText (tmpSeries->GetLabel(),rectLabel,DT_NOCLIP|DT_CENTER);


			lastXLocation = pieLeft;
			lastYLocation = centerYPie;
			
			dataSum = 0;
			for(s = 0; s < 10; s++)
			{
				if(tmpSeries->GetData(s) > 0)
				{
					dataSum += tmpSeries->GetData(s);
					percent = (dataSum * 100) / totalSum;
					degrees = (360 * percent) / 100;

						//degress / 90 will never exceed 4.
						//this can tell us the quadrant of destination
					deltaXY = (degrees * radius) / 90;
						//deltaXY is change from start point of pie 0
						//this value is total change.  so if radius is 300
						//and degrees is 270, delta is 300.  The change 
						//would move both x and y 300 pixels.  For x, 100
						//to right is center, another 100 to right edge,
						//100 back to center.  For y, 100 to bottom, 100
						//back to center, 100 to top. thus gives 270 degree
						//rotation.

					//determine destination quadrant...
						//and set newXLocation and newYLocation values...
					int quadrant = degrees / 90;  //truncates decimal
					switch(quadrant)
					{
						//in the computations below, the remarked line is
						//the original computation.  The line above it, for
						//actual use, is the simplified line, and gives the
						//exact same result
						case 0 : newXLocation = pieLeft + deltaXY;
								 newYLocation = centerYPie + deltaXY;
								 break;
						case 1 : newXLocation = pieLeft + deltaXY;
								 newYLocation = centerYPie + (2 * radius) - deltaXY;
//								 newYLocation = centerYPie + radius - (deltaXY - radius);
								 break;
						case 2 : newXLocation = pieLeft + (4 * radius) - deltaXY;
//								 newXLocation = pieLeft + (2 * radius) - (deltaXY - (2 * radius));
								 newYLocation = centerYPie + (2 * radius) - deltaXY;
//								 newYLocation = centerYPie - (deltaXY - (2 * radius));
								 break;
						case 3 : newXLocation = pieLeft + (4 * radius) - deltaXY;
//								 newXLocation = pieLeft + radius - (deltaXY - (3 * radius));
								 newYLocation = centerYPie - (4 * radius) + deltaXY;
//								 newYLocation = centerYPie - radius + (deltaXY - (3 * radius));
								 break;
						case 4 : newXLocation = pieLeft;
								 newYLocation = centerYPie - 1;
								 break;
					}
					
					if(s == 0)
						lastYLocation -= 1;

					CPoint p1 (lastXLocation, lastYLocation);
					CPoint p2 (newXLocation, newYLocation);
					COLORREF barColor;
					barColor = GetColor(s);
					CBrush brush (barColor);
					CBrush* pOldBrush;
					pOldBrush = pDC->SelectObject(&brush);
					pDC->Pie(pieRect, p1, p2); 
					pDC->SelectObject(pOldBrush);

					lastXLocation = newXLocation;
					lastYLocation = newYLocation;
				}
			}
			
		}
	}

	else if(m_iGraphType == 3)  // 3Dpie
	{
		Draw3DPie(pDC);
	}

	pDC->SelectObject (pOldFont);

}

void CGraph::Draw3DPie(CDC *pDC)
{
	///////////////////////////////////
	// Space Height:
	int nYSpace = (m_rectGraph.Height ()-m_nTitleHeight)/ m_listGraphSeries->GetCount();

	///////////////////////////////////////
	//  Determine width of pie display area
	int nLongRadius =0;
	if(m_bGaphHasLegend)
	{
		nLongRadius = (m_iXAxisWidth - m_iLegendWidth - 20)*0.45;
	}
	else
	{
		nLongRadius = m_iXAxisWidth -20;
	}
	int nShortRadius = nLongRadius *0.6;
	
	while(nShortRadius*3>nYSpace)
	{
		nShortRadius/=1.1;
		nLongRadius/=1.1;
	}

	
	int nPieLeft, nPieRight,nPieTop,nPieBottom;
	int nPieHeight=nShortRadius*0.6;
	int nPartQty=0;
	int i,j;
	CPieDrawer pie;

	CGraphSeries *pSeries=NULL;

	nPieTop=m_rectGraph.top+50;
	POSITION pos = m_listGraphSeries->GetHeadPosition();
	for(i = 0; i < m_listGraphSeries->GetCount(); i++)
	{
		pSeries = (CGraphSeries*)m_listGraphSeries->GetNext(pos);

		nPieLeft = m_rectGraph.left ;
		nPieRight = nPieLeft + (2 * nLongRadius);

		CRect pieRect (nPieLeft,nPieTop,nPieRight,nPieTop+nShortRadius*2);

		//plot series label
		CRect rectLabel(pieRect.right+20,
			(pieRect.top+pieRect.bottom)/2,
			pieRect.right+100 ,
			pieRect.bottom +nPieHeight);		
		
		// Added By FanHua:
		pDC->DrawText (pSeries->GetLabel(),rectLabel,DT_NOCLIP|DT_LEFT);

		//
		nPartQty=pSeries->GetSeriQty();
		pie.SetPar(pieRect,nPieHeight);
		pie.SetPartQty (nPartQty);
		for(j=0;j<nPartQty;j++)
		{
			pie.SetPartWeight(j,pSeries->GetData (j));
			pie.SetColor (j,m_clrDataColor[j]);
		}
		pie.Draw (pDC);

		nPieTop+=nYSpace;
	}
}


void CGraph::SetLegend(int datagroup, CString label)
{
	m_CGraphLegend.SetLegendText(datagroup, label);
	m_bGaphHasLegend = TRUE;
}

void CGraph::DrawLegend(CDC* pDC)
{
	//determine size of legend
	//12 chars per m_iSeriesSize + 6 for spacing (3 top and bottom) 
	//+ 1 set for label title(3+12+6) + rectangle (2 chars) + 3 for final bottom buffer
	int legendHeight = 23 + (m_iSeriesSize * 18) + 3;
	int legendL, legendT, legendR, legendB;
	int barL, barT, barR, barB;

	legendT = m_rectGraph.top+m_nTitleHeight+20 ;
	legendB = legendT + legendHeight;
	legendR = m_iMaxWidth - 5;
	legendL = legendR - ((m_CGraphLegend.GetLegendLength() * 12) + 50);
		//allows 50 pixels for display of legend bar 45 + 5 space.
	m_iLegendWidth = legendR - legendL;

	CFont *pOldFont= pDC->SelectObject(&m_fontText);

	pDC->Rectangle(legendL, legendT, legendR, legendB);
	pDC->TextOut(legendL + (m_iLegendWidth / 2) - 24, 
					legendT + 3, "Í¼Àý");
	for(int i = 0; i < m_iSeriesSize; i++)
	{
		//top "Legend" text will use 12 + 3 top + 6 bottom (21 total)
		//each legend label will need 3 chars on top, so the 24 in the offset
		//each label than uses 12 + 3 below + 3 above next label, so 18
		// in the i * offset.  
		pDC->TextOut(legendL + 5, legendT + 24 + (i * 18), m_CGraphLegend.GetLegendText(i));

		//draw bar
		COLORREF barColor;
		barColor = GetColor(i);
		CBrush brush (barColor);
		CBrush* pOldBrush;
		pOldBrush = pDC->SelectObject(&brush);

		barL = legendL + 5 + (m_CGraphLegend.GetLegendText(i).GetLength() * 8) + 5;
		barT = legendT + 24 + (i * 18) + 1, m_CGraphLegend.GetLegendText(i);
		barR = legendR - 5;
		barB = barT + 12;
		pDC->Rectangle(barL, barT, barR, barB);

		pDC->SelectObject(&pOldBrush);

	
	}

	pDC->SelectObject (pOldFont);

}

void CGraph::SetGraphTitle(CString title)
{
	m_sGraphTitle = title;
}