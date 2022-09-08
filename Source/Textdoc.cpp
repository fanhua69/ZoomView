// TextDoc.cpp : implementation file
//

#include "stdafx.h"
#include "TextDoc.h"
#include "TextView.h"
#include "fhcglobal.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextDoc

IMPLEMENT_DYNCREATE(CTextDoc, CDocument)

CTextDoc::CTextDoc()
{
}

BOOL CTextDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;
    return TRUE;
}

CTextDoc::~CTextDoc()
{
}


BEGIN_MESSAGE_MAP(CTextDoc, CDocument)
    //{{AFX_MSG_MAP(CTextDoc)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextDoc diagnostics

#ifdef _DEBUG
void CTextDoc::AssertValid() const
{
    CDocument::AssertValid();
}

void CTextDoc::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTextDoc serialization

void CTextDoc::Serialize(CArchive& ar)
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
// CTextDoc commands

BOOL CTextDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
    if (!CDocument::OnOpenDocument(lpszPathName))
        return FALSE;

    SetTitle(::SeperateFileName(lpszPathName));

    // TODO: Add your specialized creation code here
    m_sFileName= lpszPathName;

    return TRUE;
}

BOOL CTextDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
    // TODO: Add your specialized code here and/or call the base class
    POSITION pos=GetFirstViewPosition();
    CView *pView=NULL;
    while(pos)
    {
        pView=GetNextView(pos);
        if(pView->IsKindOf(RUNTIME_CLASS(CTextView)))
        {
            CEdit &edit=((CTextView *)pView)->GetEditCtrl ();

            FILE *fp=fopen(lpszPathName,"wt");
            if(!fp)return false;

            CString s;
            edit.GetWindowText(s);
            s.Remove('\10');

            fprintf(fp,"%s",(LPCTSTR)s);

            fclose(fp);
        }
    }
    SetModifiedFlag(false);

    return false;
}
void CTextDoc::SetModifiedFlag(BOOL bModified)
{
    CString strTitle=GetTitle();
    CString strDirtyFlag="*"; // note space before the  '*'

    //  So we donot break save as dialog:
    if(!IsModified()&&bModified)
    {
        SetTitle(strTitle+strDirtyFlag);
    }
    else if(IsModified()&&!bModified)
    {
        int nTitleLength=strTitle.GetLength();
        int nDirtyLength=strDirtyFlag.GetLength();
        SetTitle(strTitle.Left(nTitleLength-nDirtyLength));
    }

    UpdateFrameCounts();

    CDocument::SetModifiedFlag(bModified);
}