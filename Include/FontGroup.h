// FontGroup.h: interface for the CFontGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FONTGROUP_H__199DF6E4_FA65_11D3_AEF5_00C04FCCB957__INCLUDED_)
#define AFX_FONTGROUP_H__199DF6E4_FA65_11D3_AEF5_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "zoomview.h"

class _FHZOOMVIEW_EXT_CLASS CFontGroup  
{
public:
	void SetValueRange(double dMinValue,double dMaxValue);
	CFontGroup();
	virtual ~CFontGroup();

	void SetWindowHeight(double dWindowHeight);
	void SetFontHeight(long  nFontHeight);
	void Set(double dMinValue, double dMaxValue,double dWindowHeight,long  nFontHeight);
	
	bool CalcFontGroup(double &dValueFrom,double &dValueTo,double &dValueStep);


protected:
	double m_dMinValue;
	double m_dMaxValue;
	double m_dWindowHeight;
	long  m_nFontHeight;
};

#endif // !defined(AFX_FONTGROUP_H__199DF6E4_FA65_11D3_AEF5_00C04FCCB957__INCLUDED_)
