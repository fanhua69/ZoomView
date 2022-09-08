// Hyperbola.h: interface for the CHyperbola class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HYPERBOLA_H__E41A899E_4314_4E19_826E_FAFDCF2F9867__INCLUDED_)
#define AFX_HYPERBOLA_H__E41A899E_4314_4E19_826E_FAFDCF2F9867__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "zoomview.h"

class _FHZOOMVIEW_EXT_CLASS CHyperbola  
{
protected:
	double *pX;
	double *pY;
	long m_nItem;

public:
	bool SetItemNumber(long nItemNumber);
	bool SetValue(double *pXOrg,double *pYOrg,long nItemNumber);
	bool SetValue(double x,double y,long nItemPos);
	bool Over(double &t0,double &v);

	CHyperbola();
	virtual ~CHyperbola();

};

#endif // !defined(AFX_HYPERBOLA_H__E41A899E_4314_4E19_826E_FAFDCF2F9867__INCLUDED_)
