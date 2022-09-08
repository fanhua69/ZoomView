// HelpView.cpp : implementation file
//

#include "stdafx.h"
#include "helpDoc.h"
#include "HelpView.h"
#include "zoomview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHelpView

IMPLEMENT_DYNCREATE(CHelpView, CHtmlView)

CHelpView::CHelpView()
{
	//{{AFX_DATA_INIT(CHelpView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CHelpView::~CHelpView()
{
}

void CHelpView::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHelpView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHelpView, CHtmlView)
	//{{AFX_MSG_MAP(CHelpView)
	ON_COMMAND(ID_VIEW_FONTS_LARGEST, OnViewFontsLargest)
	ON_COMMAND(ID_VIEW_FONTS_LARGE, OnViewFontsLarge)
	ON_COMMAND(ID_VIEW_FONTS_MEDIUM, OnViewFontsMedium)
	ON_COMMAND(ID_VIEW_FONTS_SMALL, OnViewFontsSmall)
	ON_COMMAND(ID_VIEW_FONTS_SMALLEST, OnViewFontsSmallest)
	ON_COMMAND(ID_GO_BACK, OnGoBack)
	ON_COMMAND(ID_GO_FORWARD, OnGoForward)
	ON_COMMAND(ID_GO_SEARCH, OnGoSearch)
	ON_COMMAND(ID_GO_START_PAGE, OnGoStartPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHelpView diagnostics

#ifdef _DEBUG
void CHelpView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CHelpView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHelpView message handlers

void CHelpView::OnInitialUpdate() 
{
	//TODO: This code navigates to a popular spot on the web.
	//Change the code to go where you'd like.
	//Navigate2(_T("http://www.microsoft.com/visualc/"),NULL,NULL);
	CHelpDoc *pDoc=(CHelpDoc *)GetDocument();
	Navigate2(_T(pDoc->m_sOpenedFile),NULL,NULL);

}

void CHelpView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	Refresh();	
}

void CHelpView::OnViewFontsLargest()
{
	COleVariant vaZoomFactor(4l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);
}

void CHelpView::OnViewFontsLarge()
{
	COleVariant vaZoomFactor(3l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);
}

void CHelpView::OnViewFontsMedium()
{
	COleVariant vaZoomFactor(2l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);
}

void CHelpView::OnViewFontsSmall()
{
	COleVariant vaZoomFactor(1l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);
}

void CHelpView::OnViewFontsSmallest()
{
	COleVariant vaZoomFactor(0l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);
}


void CHelpView::OnGoBack()
{
	GoBack();
}

void CHelpView::OnGoForward()
{
	GoForward();
}

void CHelpView::OnGoSearch()
{
	GoSearch();
}

void CHelpView::OnGoStartPage()
{
	GoHome();
}

