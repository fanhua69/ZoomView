#pragma once


#include "MyPoint.h"
#include "MyRect.h"
#include "MyObList.h"

enum RectEdge
{
	left,
	right,
	low,
	high
};


class CRectClipper
{
public:
	CRectClipper(void);
	~CRectClipper(void);

	void SetRect(CMyRect &rect);
	bool SetPolygon(CMyPoint *pPolygon,long nPointQty);
	bool SetPolygon(CMyRect &rect);

	CMyObList * GetResult();
	CMyRect GetResultRect();

protected:
	CMyRect m_rect;
	CMyObList m_result;
	
	CMyPoint *m_pPolygon;
	long m_nPolygonPtQty;


	bool ClipSingleEdge(double dEdge,
						RectEdge edgeType, 
						CMyObList &listIn, 
						CMyObList &listOut);

	void TestIntersect(double dEdge,
						RectEdge edgeType,
						double x1,
						double y1,
						double x2,
						double y2,
						double &xOut,
						double &yOut,
						bool &bDiffSides,
						bool &bInside);

};
