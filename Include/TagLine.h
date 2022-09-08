// TagLine.h: interface for the CTagLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TAGLINE_H__C9E8C0A1_FF35_11D3_A4E4_00C04FCCB957__INCLUDED_)
#define AFX_TAGLINE_H__C9E8C0A1_FF35_11D3_A4E4_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "zoomview.h"
#include "Line.h"
#include "MyPoint.h"	// Added by ClassView

class _FHZOOMVIEW_EXT_CLASS CTagLine : public CLine  
{
public:
                CTagLine();
    virtual     ~CTagLine();
    CMyPoint    MidPoint();
    long        CommonPoint(CTagLine &other);
    bool        IsLinked(long i);


    void operator =(const CTagLine &other);

    long        nPointHead;
    long        nPointTail;
};

#endif // !defined(AFX_TAGLINE_H__C9E8C0A1_FF35_11D3_A4E4_00C04FCCB957__INCLUDED_)
