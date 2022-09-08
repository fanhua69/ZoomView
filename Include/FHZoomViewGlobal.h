

#ifndef tag_FHZoomViewGlobal
#define tag_FHZoomViewGlobal


#include "zoomview.h"


#include "MyPoint.h"
#include "Line.h"
#include "ColorScale.h"
#include "FHCGlobal.h"
class CFHTreeView;


_FHZOOMVIEW_EXT_CLASS   void				   DisplayBitmap(
												CDC *pDC,
												CBitmap *pBitmap,
												int x,  // Horizontal  Start position for the bitmap in the CDC;
												int y   // Vertical Start position for the bitmap in the CDC;
												);

_FHZOOMVIEW_EXT_CLASS   void				 Draw3DRect(    CDC     *pDC,
                                                            CRect   &rect, 
                                                            bool    bRaised=true,
                                                            UINT    nInnerColor=-1);		

_FHZOOMVIEW_EXT_CLASS   CFHTreeView	*        GetTree    (   CString sTreeName);

_FHZOOMVIEW_EXT_CLASS   bool                 CreateFrame(   CDocument *pDocument,
                                                            CMultiDocTemplate *pTemplate);



#endif