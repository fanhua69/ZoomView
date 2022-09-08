// XYZView.cpp : implementation file
//

#include "stdafx.h"
#include "zoomview.h"
#include "ctrlext.h"  // ListCtrl Extension
#include "XYZView.h"
#include "XYZDoc.h"
#include "ProgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXYZView

IMPLEMENT_DYNCREATE(CXYZView, CFHEditView)

CXYZView::CXYZView()
{
    m_nColNumber=4;
}

CXYZView::~CXYZView()
{
}


BEGIN_MESSAGE_MAP(CXYZView, CFHEditView)
    //{{AFX_MSG_MAP(CXYZView)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXYZView drawing

void CXYZView::OnDraw(CDC* pDC)
{
    CDocument* pDoc = GetDocument();
    // TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CXYZView diagnostics

#ifdef _DEBUG
void CXYZView::AssertValid() const
{
    CFHEditView::AssertValid();
}

void CXYZView::Dump(CDumpContext& dc) const
{
    CFHEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CXYZView message handlers


void CXYZView::OnInitialUpdate() 
{
    CFHEditView::OnInitialUpdate();

}

void CXYZView::UpdateDoc()
{
    CFHEditView::UpdateDoc();

    //
    CXYZDoc *pDoc=GetDocument();

    for(long i=0;i<pDoc->m_nRecordNumber;i++){
        GetItem(i,1,&pDoc->m_pData[i].north);
        GetItem(i,2,&pDoc->m_pData[i].east);
        GetItem(i,3,&pDoc->m_pData[i].value);
    }


}

void CXYZView::UpdateGrid()
{
    CFHEditView::UpdateGrid ();

    CXYZDoc *pDoc=(CXYZDoc *)GetDocument();

    //
    SetItem(0,1,"横坐标");
    SetItem(0,2,"纵坐标");
    SetItem(0,3,"数值");

    CProgressDlg dlg("数据加载");
    dlg.Create();
    dlg.SetRange(0,pDoc->m_nRecordNumber);
    dlg.SetStatus("把xyz数据植入网格中:");

    for(long i=0;i<pDoc->m_nRecordNumber;i++)
    {
        if(i%100==0)
        {
            dlg.SetPos(i);
            if(dlg.CheckCancelButton())
            {
                break;
            }
        }
        SetItem(i+1,1,pDoc->m_pData[i].north);
        SetItem(i+1,2,pDoc->m_pData[i].east);
        SetItem(i+1,3,pDoc->m_pData[i].value);
    }

    dlg.DestroyWindow ();

    m_Grid.AutoSize ();
}

CXYZDoc* CXYZView::GetDocument()
{
    ASSERT_VALID(m_pDocument);
    return(CXYZDoc*)m_pDocument;
}
