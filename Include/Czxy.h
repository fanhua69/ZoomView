#ifndef tag_CZXY
#define tag_CZXY

#include "zoomview.h"

class  _FHZOOMVIEW_EXT_CLASS CZXY
{
public:
	double Ph;
	double east;
	double north;
	bool bShot;

public:
	void operator =(CZXY &other)
	{
		Ph=other.Ph;
		east=other.east;
		north=other.north;
		bShot=other.bShot ;
	}
};    

#endif