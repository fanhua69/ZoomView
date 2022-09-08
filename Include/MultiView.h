#pragma once


// CMultiView view
#include "Zoomview.h"
#include <MultiChildView.h>
#include <vector>
#include "ColorScale.h"
#include "MultiDocument.h"

class _FHZOOMVIEW_EXT_CLASS CMultiView : public CFHZoomView
{
    DECLARE_DYNCREATE(CMultiView)

    friend class CMultiChildView;

protected:
    CMultiView();           // protected constructor used by dynamic creation
    virtual ~CMultiView();

public:

    virtual void    OnDraw          (CDC* pDC);      // overridden to draw this view

    void            OnUpdate        (CView* pSender, LPARAM lHint, CObject* pHint) ;


    virtual void    OnActivateView  (   BOOL    bActivate, 
                                        CView*  pActivateView,
                                        CView*  pDeactiveView);

    bool            CheckPointingChange (double x,double y,double &dMinDistance);

    CString         GetInfo             (double x, double y);

    void            getChildDocs        (std::vector<CMultiChildDoc*>   &vecChildDocs);
    void            getChildViews       (std::vector<CMultiChildView*>  &vecChildViews);
    void            releaseChildViews   (   );

protected:

    void            PreSettingDlg       (   );
    void            PostSettingDlg      (   );
    void            SetCheckNearLine    (bool bCheckNearLine);


    CColorDlg           m_dlgColor;
    int                 m_iMinDistDoc;

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    DECLARE_MESSAGE_MAP()
};


