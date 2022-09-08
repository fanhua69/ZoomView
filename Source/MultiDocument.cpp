// MultiDocument.cpp : implementation file
//

#include "stdafx.h"
#include "zoomview.h"
#include "MultiDocument.h"
#include "MultiView.h"
#include "FHMainFrm.h"


// CMultiDocument

IMPLEMENT_DYNCREATE(CMultiDocument, CDocument)

CMultiDocument::CMultiDocument()
{
}

BOOL CMultiDocument::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;
    return TRUE;
}

CMultiDocument::~CMultiDocument()
{
    std::vector<CMyDocTemplate>::iterator it =  m_vecDocTemplates.begin();
    while(it!=m_vecDocTemplates.end())
    {
        it->empty();
        it++;
    }
}

void CMultiDocument::OnCloseDocument()
{
    CFHMainFrm *pFrm = dynamic_cast< CFHMainFrm* >(AfxGetApp()->GetMainWnd());
    if(!pFrm)
    {
        return;
    }

    pFrm->RemoveMultiDocument(this);

    CDocument::OnCloseDocument();
}

BEGIN_MESSAGE_MAP(CMultiDocument, CDocument)
END_MESSAGE_MAP()


// CMultiDocument diagnostics

#ifdef _DEBUG
void CMultiDocument::AssertValid() const
{
    CDocument::AssertValid();
}

void CMultiDocument::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}
#endif //_DEBUG


// CMultiDocument serialization

void CMultiDocument::Serialize(CArchive& ar)
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

bool CMultiDocument::UnReginserTemplates  (CStringList &listLineName)
{
    CString s,sInList;
    POSITION pos = listLineName.GetHeadPosition();
    while(pos)
    {
        sInList = listLineName.GetNext(pos);
        sInList.MakeUpper();

        //
        std::vector<CMyDocTemplate>::iterator it = m_vecDocTemplates.begin();
        while(it!=m_vecDocTemplates.end())
        {
            s=it->getDocument()->GetTitle() ;
            s.MakeUpper();

            if(s.Find(sInList)!=-1)
            {
                it->empty();
                it=m_vecDocTemplates.erase(it);
            }
            else
            {
                it++;
            }
        }
    }

    UpdateAllViews(NULL);

    return true;
}

int CMultiDocument::ContainsDoc(CStringList &listLineName)
{
    CString s,sInList;

    int nLineQty = listLineName.GetSize();
    if(nLineQty==0)
    {
        return 0;
    }
    
    int nExistQty = 0;

    POSITION pos = listLineName.GetHeadPosition();
    while(pos)
    {
        sInList = listLineName.GetNext(pos);
        sInList.MakeUpper();

        //
        std::vector<CMyDocTemplate>::iterator it = m_vecDocTemplates.begin();
        while(it!=m_vecDocTemplates.end())
        {
            s=it->getDocument()->GetTitle() ;
            s.MakeUpper();

            if(s.Find(sInList)!=-1)
            {
                nExistQty ++;
                break;
            }
            it++;
        }
    }

    if(nExistQty == nLineQty)
    {
        return 2;
    }
    else if (nExistQty >0 && nExistQty < nLineQty )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

bool CMultiDocument::RegisterTemplate(CMultiChildDoc    *pDoc, 
                                      CMultiChildView   *pView)
{
    if(!pDoc || !pView)
    {
        return false;
    }

    if( !pDoc->IsKindOf(RUNTIME_CLASS(CDocument)) || 
        !pView->IsKindOf(RUNTIME_CLASS(CView)))
    {
        return false;
    }

    CStringList listLineName;
    listLineName.AddTail(pDoc->GetTitle());
    UnReginserTemplates(listLineName);


    CMyDocTemplate t;
    t.setDocument(pDoc);
    t.setView(pView);

    POSITION pos = GetFirstViewPosition();
    while(pos)
    {
        CMultiView *pMultiView = dynamic_cast<CMultiView *>(GetNextView(pos));

        if(pMultiView )
        {
            pView->setMultiView(pMultiView);
            break;
        }
    }

    m_vecDocTemplates.push_back(t);

    UpdateAllViews(NULL);

    return true;
}

void CMultiDocument::getChildDocs(std::vector<CMultiChildDoc*> &vecChildDocs)
{
    int i,nSize = m_vecDocTemplates.size();
    for(i=0;i<nSize;i++)
    {
        vecChildDocs.push_back(m_vecDocTemplates[i].getDocument());
    }

    return ;
}

void CMultiDocument::getChildViews(std::vector<CMultiChildView*> &vecChildViews)
{
    int i,nSize = m_vecDocTemplates.size();
    for(i=0;i<nSize;i++)
    {
        CMultiChildView * pView = m_vecDocTemplates[i].getView();
        if(pView)
        {
            vecChildViews.push_back(pView);
        }
    }

    return ;
}

void CMultiDocument::releaseChildViews()
{
    int i,nSize = m_vecDocTemplates.size();
    for(i=0;i<nSize;i++)
    {
        m_vecDocTemplates[i].releaseView();
    }

    return ;
}