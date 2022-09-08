// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#ifndef MyObArray_H
#define MyObArray_H

#include "MyPoint.h"
#include "zoomview.h"


////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////

class _FHZOOMVIEW_EXT_CLASS CMyObArray : public CObArray
{

public:
	CMyObArray();
	virtual ~CMyObArray();

public:
	void SetDelete(bool bDeleteByCMyObArray);
	bool InsertByOrder(CMyPoint *pNewPoint,
					   long nElementExist,
					   int nOrderType /* 0: Order by x, 1, order by y */);

	bool RemoveNearest(CMyPoint *pNewPoint,
					   long nElementExist,
					   int nOrderType /* 0: Order by x, 1, order by y, */);


protected:
	bool m_bNeedDelete;
};

#endif //!__AFXCOLL_H__

/////////////////////////////////////////////////////////////////////////////
