#pragma once


// CMultiChildView view
#include "Zoomview.h"
#include "FHZoomView.h"
#include "3DRange.h"
#include "afxdlgs.h"



class _FHZOOMVIEW_EXT_CLASS CMultiChildView : public CFHZoomView
{
    DECLARE_DYNCREATE(CMultiChildView)

    friend class CMultiView;

public:
                    CMultiChildView     ();           // protected constructor used by dynamic creation
    virtual         ~CMultiChildView    ();
    virtual bool    UpdateDataRange     ();
    void            SetDrawCurrentItem  (bool bDraw = true);
    void            setMultiView        (CMultiView *pMultiView);
    void            SetCheckNearLine    (bool bCheckNearLine);

protected:
    virtual void            AddSettingPage  (CPropertyPage *pPage);
    virtual CPropertyPage * GetSettingPage  (CRuntimeClass *pClass);

    void            PreSettingDlg       ();
    void            PostSettingDlg      ();
    void            AddMenu             (UINT MenuID, CString MenuString);

    bool            m_bDrawCurrentItem;
    CMultiView      *m_pMultiView;
    bool            m_bCheckNearLine;

public:
    virtual void    OnDraw              (   CDC* pDC    );      // overridden to draw this view
#ifdef _DEBUG
    virtual void    AssertValid         (   ) const;
    virtual void    Dump                (   CDumpContext& dc) const;
#endif

protected:
    DECLARE_MESSAGE_MAP()
};


