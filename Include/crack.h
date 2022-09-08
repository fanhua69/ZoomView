// crack.h :
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.
#ifndef tag_Crack
#define tag_Crack

#include <afxdao.h>
#include "zoomview.h"

class _FHZOOMVIEW_EXT_CLASS CCrack
{
public:
	static LPCTSTR strFieldType(short sType);
	static LPCTSTR strQueryDefType(short sType);
	static LPCTSTR strBOOL(BOOL bFlag);
	static CString strVARIANT(const COleVariant& var);
};
_FHZOOMVIEW_EXT_CLASS void  DisplayDaoException(CDaoException* e);

#endif