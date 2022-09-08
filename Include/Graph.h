#if !defined(AFX_GRAPH_H__9DB68B4D_3C7C_47E2_9F72_EEDA5D2CDBB0__INCLUDED_)
#define AFX_GRAPH_H__9DB68B4D_3C7C_47E2_9F72_EEDA5D2CDBB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Graph.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGraph window

#include "zoomview.h"

#include "GraphSeries.h"
#include "GraphLegend.h"

class _FHZOOMVIEW_EXT_CLASS CGraph : public CStatic
{
// Construction
public:
	CGraph();
	CGraph(int type);

// Attributes
public:


private:
	float m_fTickSpace;	//number of pixels between "y" axis ticks
	CObList* m_listGraphSeries;
	int m_iGraphType;	//0 - bar graph, 1 - line graph, 2 - pie
	int m_iGraphAlignment;	//0 - horizontal, 1 - vertical
	int m_iMaxHeight;
	int m_iMaxWidth;
	CString m_sAxisYLabel;
	CString m_sAxisXLabel;
	int m_iYApexPoint;
	int m_iXApexPoint;
	int m_iYAxisHeight;
	int m_iXAxisWidth;
	float m_fTickRange;
	int m_iSeriesSize;	//number of data elements to map per series
	CGraphLegend m_CGraphLegend;
	BOOL m_bGaphHasLegend;
	int m_iLegendWidth;
	COLORREF m_clrDataColor[10];
	int m_iXAxisAlign;	//0 - horizontal, # - degree rotation
	int m_ixAxisLabelLength;
	CString m_sGraphTitle;

	int m_nTitleHeight;

	CRect m_rectGraph;
	CFont m_fontTitle;	
	CFont m_fontText;

// Operations
public:

	void SetTickSpace(float yDist);
	void SetTickRange(float maxTick);
	void DrawGraph(CDC* pDC,CRect graphRect);
	void DrawAxis(CDC* pDC);
	void DrawSeries(CDC* pDC);
	void DrawLegend(CDC* pDC);
	void Draw3DPie(CDC *pDC);

	void AddSeries(CGraphSeries* dataSet);
	void SetXAxisLabel(CString label);
	void SetYAxisLabel(CString label);
	void SetLegend(int datagroup, CString label);
	void SetColor(int group, COLORREF groupColor);
	COLORREF GetColor(int group);
	void SetXAxisAlignment(int alignValue);
	int GetXAxisAlignment();
	void SetGraphType(int gType);
	void SetGraphTitle(CString title);

	void RemoveAll();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraph)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraph();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGraph)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPH_H__9DB68B4D_3C7C_47E2_9F72_EEDA5D2CDBB0__INCLUDED_)
