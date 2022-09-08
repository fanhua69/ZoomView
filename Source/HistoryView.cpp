// PasswordView.cpp : implementation file
//

#include "stdafx.h"
#include "HistoryView.h"
#include "HistoryDoc.h"

// CHistoryView


IMPLEMENT_DYNCREATE(CHistoryView, CFHEditView)

CHistoryView::CHistoryView()
{
    m_nColNumber=11;
}

CHistoryView::~CHistoryView()
{
}

BEGIN_MESSAGE_MAP(CHistoryView, CFHEditView)
END_MESSAGE_MAP()


// CHistoryView drawing

void CHistoryView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// CHistoryView diagnostics

#ifdef _DEBUG
void CHistoryView::AssertValid() const
{
	CFHEditView::AssertValid();
}

void CHistoryView::Dump(CDumpContext& dc) const
{
	CFHEditView::Dump(dc);
}
#endif //_DEBUG


// CHistoryView message handlers
void CHistoryView::UpdateGrid()
{
    CFHEditView::UpdateGrid ();

    m_Grid.RemoveAllCombos();

    CHistoryDoc *pDoc=(CHistoryDoc*)GetDocument();

    CString sUserName       =pDoc->m_history.GetUserName();
    CString sSoftwareName   =pDoc->m_history.GetSoftwareNumber();

    long nQty=pDoc->GetHistoryQty();

    SetItem(0,0,"序号");
    SetItem(0,1,"用户名");
    SetItem(0,2,"软件编号");
    SetItem(0,3,"历史动作");
    SetItem(0,4,"动作对象");
    SetItem(0,5,"年");
    SetItem(0,6,"月");
    SetItem(0,7,"日");
    SetItem(0,8,"时");
    SetItem(0,9,"分");
    SetItem(0,10,"秒");


    for (int row = 0; row < nQty; row++)
    {
        CHistory &p=pDoc->GetHistory(row);

        SetItem(row+1,1,sUserName );
        SetItem(row+1,2,sSoftwareName );
        SetItem(row+1,3,p.GetHistoryName());
        SetItem(row+1,4,p.m_sTargetName);
        SetItem(row+1,5,p.m_date.GetYear());
        SetItem(row+1,6,p.m_date.GetMonth());
        SetItem(row+1,7,p.m_date.GetDay());
        SetItem(row+1,8,p.m_time.GetHour());
        SetItem(row+1,9,p.m_time.GetMinute());
        SetItem(row+1,10,p.m_time.GetSecond());
    }

    m_Grid.AutoSize ();
}

void CHistoryView::UpdateDoc()
{
    //CFHEditView::UpdateDoc ();
}

