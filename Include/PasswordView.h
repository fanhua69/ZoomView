#pragma once


// CPasswordView view
#include "fheditview.h"
#include "passwordmanager.h"
#include "Zoomview.h"

class _FHZOOMVIEW_EXT_CLASS CPasswordView : public CFHEditView
{
	DECLARE_DYNCREATE(CPasswordView)

protected:
	CPasswordView();           // protected constructor used by dynamic creation
	virtual ~CPasswordView();

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


