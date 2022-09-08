// PasswordDoc.cpp : implementation file
//

#include "stdafx.h"
#include "PasswordDoc.h"
#include "fhapp.h"
#include "FHmainfrm.h"

// CPasswordDoc

IMPLEMENT_DYNCREATE(CPasswordDoc, CFHEditDoc)

CPasswordDoc::CPasswordDoc()
{
}

BOOL CPasswordDoc::OnNewDocument()
{
	if (!CFHEditDoc::OnNewDocument())
		return FALSE;
	return TRUE;
}

CPasswordDoc::~CPasswordDoc()
{
}


BEGIN_MESSAGE_MAP(CPasswordDoc, CFHEditDoc)
END_MESSAGE_MAP()


// CPasswordDoc diagnostics

#ifdef _DEBUG
void CPasswordDoc::AssertValid() const
{
	CFHEditDoc::AssertValid();
}

void CPasswordDoc::Dump(CDumpContext& dc) const
{
	CFHEditDoc::Dump(dc);
}
#endif //_DEBUG


// CPasswordDoc serialization

void CPasswordDoc::Serialize(CArchive& ar)
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

// CPasswordDoc commands

BOOL CPasswordDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    if (!CDocument::OnOpenDocument(lpszPathName))
        return FALSE;


    bool bSuccess = m_manager.Read (lpszPathName);
    if(bSuccess)
    {
        m_nRecordNumber=m_manager.GetPasswordQty ();
    }
    else
    {
        TRACEERROR("口令文件出错:"+CString(lpszPathName));
        return false;
    }

    return bSuccess;
}

CPassword * CPasswordDoc::GetPassword(long nIndex)
{
    return m_manager.GetPassword(nIndex);
}

BOOL CPasswordDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
    UpdateDoc();
    m_manager.Save (lpszPathName);
    return true;
}
