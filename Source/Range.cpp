
#include "stdafx.h"
#include "Range.h"

CRange::CRange()
{
	start=end=0;
	nInc=1;
};

CRange::CRange(long nStart,long nEnd)
{
	if(nStart<nEnd){
		start=nStart;
		end=nEnd;
	}
	else{
		start=nStart;
		end=nEnd;
	}
};

void CRange::Set(long nStart,long nEnd,long nIncTag)
{
	if(nStart<nEnd){
		start=nStart;
		end=nEnd;
		nInc=nIncTag;
	}
	else{
		start=nStart;
		end=nEnd;
		nInc=nIncTag;
	}
};

void CRange::Normalize()
{
	if(start<end){
		long n=start;
		start=end;
		end=n;
	}
};

bool CRange::overlap(CRange range)
{
	Normalize();
	range.Normalize ();
	if(start>=range.start &&start<=range.end)return true;
	if(end>=range.start &&end<=range.end)return true;
	return false;
};

void CRange::operator =(CRange &other)
{
	start=other.start ;
	end=other.end ;
	nInc=other.nInc;
	return;
};

long CRange::GetNumber()
{
	if(nInc==0)return 0;
	return (end-start)/nInc+1;
};
