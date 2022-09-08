// PasswordDoc.cpp : implementation file
//

#include "stdafx.h"
#include "HistoryDoc.h"
#include "fhapp.h"
#include "FHmainfrm.h"

// CHistoryDoc

IMPLEMENT_DYNCREATE(CHistoryDoc, CFHEditDoc)


CHistoryDoc::CHistoryDoc()
{
}

BOOL CHistoryDoc::OnNewDocument()
{
	if (!CFHEditDoc::OnNewDocument())
		return FALSE;
	return TRUE;
}

CHistoryDoc::~CHistoryDoc()
{
}


BEGIN_MESSAGE_MAP(CHistoryDoc, CFHEditDoc)
END_MESSAGE_MAP()


// CHistoryDoc diagnostics

#ifdef _DEBUG
void CHistoryDoc::AssertValid() const
{
	CFHEditDoc::AssertValid();
}

void CHistoryDoc::Dump(CDumpContext& dc) const
{
	CFHEditDoc::Dump(dc);
}
#endif //_DEBUG


// CHistoryDoc serialization

void CHistoryDoc::Serialize(CArchive& ar)
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

// CHistoryDoc commands

BOOL CHistoryDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    if (!CDocument::OnOpenDocument(lpszPathName))
        return FALSE;


    bool bSuccess = m_history.Read (lpszPathName);
    if(bSuccess)
    {
        m_nRecordNumber=m_history.GetHistoryQty();
    }
    else
    {
        TRACEERROR("历史文件出错:"+CString(lpszPathName));
        return false;
    }

    return bSuccess;
}

int CHistoryDoc::GetHistoryQty   ()
{
    return m_history.GetHistoryQty();
}

CHistory & CHistoryDoc::GetHistory(long nIndex)
{
    return m_history.GetHistory(nIndex);
}

BOOL CHistoryDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
    UpdateDoc();
    m_history.Save (lpszPathName);
    return true;
}
