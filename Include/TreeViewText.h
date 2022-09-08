#pragma once

#include "Zoomview.h"

#include "fhtreeview.h"
#include "ChildFrm.h"

class _FHZOOMVIEW_EXT_CLASS CTreeViewText : public CFHTreeView
{
	DECLARE_DYNCREATE(CTreeViewText)

protected:
	CTreeViewText();           // protected constructor used by dynamic creation
	virtual ~CTreeViewText();

    bool AddTemplates();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


