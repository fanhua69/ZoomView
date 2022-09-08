#pragma once

#include "Zoomview.h"
#include "FHPointLocDoc.h"

// CMultiChildDoc document

class _FHZOOMVIEW_EXT_CLASS CMultiChildDoc : public CFHPointLocDoc
{
    DECLARE_DYNCREATE(CMultiChildDoc)

public:
                    CMultiChildDoc      ();
    virtual         ~CMultiChildDoc     ();
    virtual void    Serialize           (CArchive& ar);   // overridden for document i/o
    virtual void    OnChangedViewList   ();
    void            SetIsMultiChildDoc  (bool bIsMultiChildDoc = true);

#ifdef _DEBUG
    virtual void    AssertValid         () const;
    virtual void    Dump                (CDumpContext& dc) const;
#endif

protected:
    virtual BOOL    OnNewDocument       ();

    bool    m_bIsMultiChidDoc;

    DECLARE_MESSAGE_MAP()
};
