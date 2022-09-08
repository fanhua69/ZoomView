// Spline.h: interface for the CSpline class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPLINE_H__BE894681_1118_4449_A3B5_6FD1B7E32E05__INCLUDED_)
#define AFX_SPLINE_H__BE894681_1118_4449_A3B5_6FD1B7E32E05__INCLUDED_

#include "Mypoint.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "zoomview.h"


class _FHZOOMVIEW_EXT_CLASS CSpline  
{
public:
	void Reset();
	bool DeletePoint(double x,double y);
	bool AddPoint(double x,double y);
	CMyPoint GetPoint(long nOrder);
	long GetPointNumber();
	CSpline();
	virtual ~CSpline();

	bool SetPointNumber(long nPointNumber);
	bool SetData(long nOrder,double x, double y);
	bool SetData(double *pX, double *pY,long nNumber);
	bool Over();
	
	double GetValue(double x);
	double GetDerivative1(double x);
	double GetDerivative2(double x);
	double GetDerivative3(double x);	
	

protected:
	bool SetMemory(long nPointNumber);
	bool Spline();

	double *m_pX;
	double *m_pY;
	double *m_pY0;
	double *m_pY1;
	double *m_pY2;
	double *m_pY3;
	long m_nPointNumber;
};

#endif // !defined(AFX_SPLINE_H__BE894681_1118_4449_A3B5_6FD1B7E32E05__INCLUDED_)
