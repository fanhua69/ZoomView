// FHEditDoc.cpp : implementation file
//

#include "stdafx.h"
#include "FHEditDoc.h"
#include "FHZoomViewGlobal.h"
#include "FHEditView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFHEditDoc

IMPLEMENT_DYNCREATE(CFHEditDoc, CDocument)


CFHEditDoc::CFHEditDoc()
{
    m_bUseSerialize=false;
}


BOOL CFHEditDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;
    return TRUE;
}

CFHEditDoc::~CFHEditDoc()
{
}


BEGIN_MESSAGE_MAP(CFHEditDoc, CDocument)
    //{{AFX_MSG_MAP(CFHEditDoc)
    ON_COMMAND(ID_FILE_SAVE, OnFileSave)
    ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFHEditDoc diagnostics

#ifdef _DEBUG
void CFHEditDoc::AssertValid() const
{
    CDocument::AssertValid();
}

void CFHEditDoc::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFHEditDoc serialization

void CFHEditDoc::Serialize(CArchive& ar)
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

/////////////////////////////////////////////////////////////////////////////
// CFHEditDoc commands

bool CFHEditDoc::SetRecordNumber(long nRecordNumber)
{
    if(nRecordNumber<0||nRecordNumber>50000)
    {
        AfxMessageBox("´íÎó£º¼ÇÂ¼ÊýÄ¿´íÎó:"+vtos(nRecordNumber));
        return false;
    }
    m_nRecordNumber=nRecordNumber;
    return true;
}

void CFHEditDoc::SetArea(CString sArea)
{
    m_sArea=sArea;
}

long CFHEditDoc::GetRecordNumber()
{
    return m_nRecordNumber;
}

void CFHEditDoc::OnCloseDocument()
{
    CDocument::OnCloseDocument();
}

void CFHEditDoc::UpdateDoc()
{
    POSITION pos = GetFirstViewPosition();
    CFHEditView *pView=NULL;
    while(pos)
    {
        pView=(CFHEditView *)GetNextView(pos);
        if(pView->IsKindOf(RUNTIME_CLASS(CFHEditView)))
        {
            pView->UpdateDoc();
        }
    }

    SetModifiedFlag(true);
}

BOOL CFHEditDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
    UpdateDoc();
    SetModifiedFlag(false);

    // Firstly call Serialize:
    if(m_bUseSerialize)
    {
        CFile file(lpszPathName,CFile::modeCreate|CFile::modeWrite );
        CArchive ar(&file,CArchive::store);
        Serialize(ar);
        ar.Close();
        file.Close();
    }
    else
    {
        if(!SaveFile(lpszPathName))
            return false;
    }

    return true;
}

bool CFHEditDoc::SaveFile(CString sFile)
{
    return false;
}

bool CFHEditDoc::ReadFile(CString sFile)
{
    return false;
}

BOOL CFHEditDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    if(m_bUseSerialize)
    {
        CFile file(lpszPathName,CFile::modeRead);
        CArchive ar(&file,CArchive::load);
        Serialize(ar);
        ar.Close();
        file.Close();
        return true;
    }
    else 
    {
        if(ReadFile(lpszPathName))
        {
            SetPathName(lpszPathName);
            return true;
        }
        else
        {
            return false;
        }
    }
}

void CFHEditDoc::SetModifiedFlag(BOOL bModified)
{
    CString strTitle=GetTitle();
    CString strDirtyFlag="*"; // note space before the  '*'

    //  So we donot break save as dialog:
    strTitle.Remove ('*');
    if(bModified)
    {
        strTitle+="*";
    }
    SetTitle(strTitle);
    /*
    if(!IsModified()&&bModified)
    {
    SetTitle(strTitle+strDirtyFlag);
    }
    else if(IsModified()&&!bModified)
    {
    int nTitleLength=strTitle.GetLength();
    int nDirtyLength=strDirtyFlag.GetLength();
    SetTitle(strTitle.Left(nTitleLength-nDirtyLength));
    }*/

    UpdateFrameCounts();

    CDocument::SetModifiedFlag(bModified);
}
BOOL CFHEditDoc::SaveModified()
{
    // TODO: Add your specialized code here and/or call the base class

    return __super::SaveModified();
}

void CFHEditDoc::OnFileSave()
{
    CString sTitle=GetTitle();
    CString sBak=sTitle;

    sTitle.Remove ('*');
    SetTitle(sTitle);

    bool b=DoFileSave();
    if(!b)
    {
        SetTitle(sBak);
    }
    else
    {
        SetModifiedFlag(false);
    }
}

void CFHEditDoc::OnFileSaveAs()
{
    if(!DoSave(NULL))
        TRACE(traceAppMsg, 0, "Warning: File save-as failed.\n");
}

void CFHEditDoc::SetUseSerialize (bool bUseSerialize)
{
    m_bUseSerialize=bUseSerialize;
}

CString CFHEditDoc::GetArea()
{
    return m_sArea;
}
