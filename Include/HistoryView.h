#pragma once


// CHistoryView view
#include "fheditview.h"
#include "historymanager.h"
#include "zoomview.h"


class _FHZOOMVIEW_EXT_CLASS CHistoryView : public CFHEditView
{
	DECLARE_DYNCREATE(CHistoryView)

protected:
	CHistoryView();           // protected constructor used by dynamic creation
	virtual ~CHistoryView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void UpdateDoc();
	void UpdateGrid();

protected:
	DECLARE_MESSAGE_MAP()
};


