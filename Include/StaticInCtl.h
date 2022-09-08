#pragma once


// CStaticInCtl
#include "zoomview.h"

class _FHZOOMVIEW_EXT_CLASS  CStaticInCtl : public CStatic
{
	DECLARE_DYNAMIC(CStaticInCtl)

public:
	CStaticInCtl();
	virtual ~CStaticInCtl();

    void SetBackGroudColor(COLORREF nColor);
protected:
    COLORREF m_nColorBack;

protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
};


