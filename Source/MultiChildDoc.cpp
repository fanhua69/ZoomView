// MultiChildDoc.cpp : implementation file
//

#include "stdafx.h"
#include "MultiChildDoc.h"


// CMultiChildDoc

IMPLEMENT_DYNCREATE(CMultiChildDoc, CDocument)

CMultiChildDoc::CMultiChildDoc()
{
    m_bIsMultiChidDoc = false;
}

BOOL CMultiChildDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;
    return TRUE;
}

CMultiChildDoc::~CMultiChildDoc()
{
}


BEGIN_MESSAGE_MAP(CMultiChildDoc, CDocument)
END_MESSAGE_MAP()


// CMultiChildDoc diagnostics

#ifdef _DEBUG
void CMultiChildDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMultiChildDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMultiChildDoc serialization

void CMultiChildDoc::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        // TODO: add storing code here
    }
    else
    {
        // TODO: add loading code here
    }
}

void CMultiChildDoc::SetIsMultiChildDoc(bool bIsMultiChildDoc)
{
    m_bIsMultiChidDoc = bIsMultiChildDoc;
}

void CMultiChildDoc::OnChangedViewList()
{
    if(m_bIsMultiChidDoc)
    {
        return;
    }

    CFHPointLocDoc::OnChangedViewList();
}
