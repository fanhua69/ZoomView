
#ifndef CRange_tag
#define CRange_tag
#include "zoomview.h"

class _FHZOOMVIEW_EXT_CLASS CRange 
{
public:
    long start;
    long end;
    long nInc;

    CRange();
    CRange(long nStart,long nEnd);
    void Set(long nStart,long nEnd,long nIncTag=1);
    void Normalize();
    bool overlap(CRange range);
    void operator =(CRange &other);
    long GetNumber();
};

#endif

#ifndef VALID_CMP_RANGE_LIMIT 
#define VALID_CMP_RANGE_LIMIT 50
#endif