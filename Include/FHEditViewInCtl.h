#pragma once



// CFHEditViewInCtl view
#include "ZoomView.h"
#include "FHEditView.h"
#include "MyPoint.h"

#define FM_MOUSELBDOWN WM_USER+200

#include "zoomview.h"

class _FHZOOMVIEW_EXT_CLASS CFHEditViewInCtl : public CFHEditView
{
	DECLARE_DYNCREATE(CFHEditViewInCtl)

protected:
	CFHEditViewInCtl();           // protected constructor used by dynamic creation
	virtual ~CFHEditViewInCtl();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
public:
	

    void UpdateGrid();
protected:

	virtual CMenu * GetPopupMenu();

	CWnd *m_pMyParent;

public:
	bool SetParent(CWnd *pParent);


	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


