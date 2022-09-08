#pragma once

#include <vector>
#include "Zoomview.h"
#include "MultiChildDoc.h"
#include "MultiChildView.h"
#include "FHEditDoc.h"


// CMultiDocument document

class CMyDocTemplate
{
public:
    CMyDocTemplate()
    {
        m_pDocument     = NULL;
        m_pView         = NULL;
    };

    void setDocument(CMultiChildDoc *pDoc)
    {
        if(m_pDocument )
        {
            delete m_pDocument;
            m_pDocument = NULL;
        }
        m_pDocument = pDoc;
    }

    void setView(CMultiChildView *pView)
    {
        if(m_pView )
        {
            releaseView();
            delete m_pView;
            m_pView = NULL;
        }
        m_pView = pView;
    }


    void empty()
    {
        if(m_pDocument)
        {
            delete m_pDocument;
            m_pDocument = NULL;
        }

        if(m_pView)
        {
            m_pView = NULL;
            delete m_pView;
        }
    }

    CMultiChildDoc * getDocument()
    {
        return m_pDocument;
    }

    CMultiChildView * getView()
    {
        if(!m_pDocument || !m_pView)
        {
            return NULL;
        }

        releaseView();
        m_pDocument->AddView(m_pView);
        return m_pView;
    }

    void releaseView()
    {
        if(!m_pView || !m_pDocument)
        {
            return;
        }

        if(m_pView->GetDocument() == m_pDocument)
        {
            m_pDocument->RemoveView(m_pView);
        }
        return;
    }

protected:

    CMultiChildDoc      *m_pDocument;
    CMultiChildView     *m_pView;

};

class _FHZOOMVIEW_EXT_CLASS CMultiDocument : public CFHEditDoc
{
    DECLARE_DYNCREATE(CMultiDocument)

friend class CMultiView;

public:
    bool            RegisterTemplate        (   CMultiChildDoc *pDoc, 
                                                CMultiChildView *pView);

    int             ContainsDoc             (CStringList &listLineName);
    bool            UnReginserTemplates     (CStringList &listLineName);
    virtual void    OnCloseDocument         ();

    void            getChildDocs            (std::vector<CMultiChildDoc*> & vecChildDocs);
    void            getChildViews           (std::vector<CMultiChildView*> &vecChildViews);
    void            releaseChildViews       ();

public:
    CMultiDocument();
    virtual ~CMultiDocument();
    virtual void Serialize(CArchive& ar);   // overridden for document i/o

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    virtual BOOL OnNewDocument  ();

    std::vector<CMyDocTemplate> m_vecDocTemplates;


    DECLARE_MESSAGE_MAP()
};
