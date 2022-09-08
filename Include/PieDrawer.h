#pragma once

#include "zoomview.h"

class _FHZOOMVIEW_EXT_CLASS CPieDrawer
{
public:
	CPieDrawer(void);
	~CPieDrawer(void);

protected:
	int m_nLongRadius;
	int m_nShortRadius;
	int m_nHeight;
	CPoint m_pointCenter;
	CRect m_rectPie;

	COLORREF m_nColorEdge;
	CPen m_penEdge;	

	COLORREF	*m_pPartColor;
	double		*m_pPartWeight;
	long		m_nPartQty;

	void DrawVerLine(CDC *pDC, float fPer);
	void DrawVerLines(CDC *pDC);

public:
	void SetPar(CRect rect, int nHeight);
	void SetPar(CPoint pointCenter, 
					int nLongRadius,
					int nShorRadius,
					int nHeight);


	bool SetPartQty(int nPartQty);
	bool SetColor(int nOrder, COLORREF nFrtColor);
	bool SetPartWeight(int nOrder, double dWeight);
	

	void Draw(CDC *pDC);
	void DrawUpperPie(CDC *pDC);
	void DrawEdge(CDC *pDC);



	static CPoint GetPerPoint(float fPer, int iLongRadius, int iShortRadius, CPoint pointCenter);

};
