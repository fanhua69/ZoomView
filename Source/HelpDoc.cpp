// HelpDoc.cpp : implementation file
//

#include "stdafx.h"
#include "HelpDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHelpDoc

IMPLEMENT_DYNCREATE(CHelpDoc, CDocument)

CHelpDoc::CHelpDoc()
{
	m_sOpenedFile="";
}

BOOL CHelpDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_sOpenedFile="";

	return TRUE;
}

CHelpDoc::~CHelpDoc()
{
}


BEGIN_MESSAGE_MAP(CHelpDoc, CDocument)
	//{{AFX_MSG_MAP(CHelpDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHelpDoc diagnostics

#ifdef _DEBUG
void CHelpDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHelpDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHelpDoc serialization

void CHelpDoc::Serialize(CArchive& ar)
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
// CHelpDoc commands

BOOL CHelpDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	m_sOpenedFile=lpszPathName;
	
	return TRUE;
}

void CHelpDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
	//CFdataApp *pApp=(CFdataApp *)AfxGetApp();
	//pApp->m_bHelpIsShowing =false;
	
	CDocument::OnCloseDocument();
}
