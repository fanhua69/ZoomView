// TextView.cpp : implementation file
//

#include "stdafx.h"
#include "TextDoc.h"
#include "TextView.h"
#include "progdlg.h"
#include "FHCGlobal.h"
#include "fhmainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextView

IMPLEMENT_DYNCREATE(CTextView, CEditView)

CTextView::CTextView()
{
    //	m_fontText.CreateFont (12,0,0,0,0,false,false,false,0,0,0,0,0,"Courier New");
    m_fontText.CreateFont (12,0,0,0,0,false,false,false,0,0,0,0,0,"宋体");

}

CTextView::~CTextView()
{
}


BEGIN_MESSAGE_MAP(CTextView, CEditView)
    //{{AFX_MSG_MAP(CTextView)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //}}AFX_MSG_MAP
    ON_WM_CHAR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextView drawing

void CTextView::OnDraw(CDC* pDC)
{
    CDocument* pDoc = GetDocument();
    // TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CTextView diagnostics

#ifdef _DEBUG
void CTextView::AssertValid() const
{
    CEditView::AssertValid();
}

void CTextView::Dump(CDumpContext& dc) const
{
    CEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTextView message handlers

void CTextView::OnInitialUpdate() 
{
    CEditView::OnInitialUpdate();
}

void CTextView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    CTextDoc *pDoc=(CTextDoc *)GetDocument();
    ASSERT_VALID(pDoc);
    if(pDoc->m_sFileName=="")return;
    OpenFile(pDoc->m_sFileName);
}

void CTextView::OpenFile(CString sFile)
{
    CEdit& edit=GetEditCtrl();
    edit.SetFont (&m_fontText);

    int iLen=GetFileLen(sFile);

    FILE *fp=fopen(sFile,"rt");
    if(!fp)
    {
        return;
    }

    CString sAll,sLine;
    char cLine[1000];
    CProgressDlg dlg;
    dlg.Create();
    dlg.SetStatus("正在加载数据");
    dlg.SetRange(0,iLen);

    int iPos=0;
    bool bFailed=false;

    while(!feof(fp))
    {
        iPos=ftell(fp);
        dlg.SetPos(iPos);
        if(dlg.CheckCancelButton())
        {
            bFailed=true;
            break;
        }

        memset(cLine,0,1000);
        fgets(cLine,900,fp);
        sLine=CString(cLine);
        int n = sLine .GetLength();
        if(sLine.GetLength()<1)break;

        sLine.Remove(10);
        sLine.Remove(13);
        sLine=sLine+"\r\n";
        sAll+=sLine;
        if(iPos>10000000)
        {
            sAll+="........\r\n";
            sAll+="........\r\n";
            TRACEERROR("数据量太大，只加载10M");
            break;
        }
    }

    dlg.DestroyWindow();

    edit.SetWindowText(sAll);

    fclose(fp);
}
BOOL CTextView::OnPreparePrinting(CPrintInfo* pInfo)
{
    return CEditView::OnPreparePrinting(pInfo);
}

void CTextView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
    CEditView::OnPrint(pDC, pInfo);
}

void CTextView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    CEditView::OnChar(nChar, nRepCnt, nFlags);

    CTextDoc *pDoc=(CTextDoc *)GetDocument();
    pDoc->SetModifiedFlag (true);
}
