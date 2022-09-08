// PasswordView.cpp : implementation file
//

#include "stdafx.h"
#include "PasswordView.h"
#include "PasswordDoc.h"

// CPasswordView

IMPLEMENT_DYNCREATE(CPasswordView, CFHEditView)

CPasswordView::CPasswordView()
{
    m_nColNumber=6;
}

CPasswordView::~CPasswordView()
{
}

BEGIN_MESSAGE_MAP(CPasswordView, CFHEditView)
END_MESSAGE_MAP()


// CPasswordView drawing

void CPasswordView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// CPasswordView diagnostics

#ifdef _DEBUG
void CPasswordView::AssertValid() const
{
	CFHEditView::AssertValid();
}

void CPasswordView::Dump(CDumpContext& dc) const
{
	CFHEditView::Dump(dc);
}
#endif //_DEBUG


// CPasswordView message handlers
void CPasswordView::UpdateGrid()
{
    CFHEditView::UpdateGrid ();

    m_Grid.RemoveAllCombos();

    CStringArray arrayLevel;
    arrayLevel.Add(GetUserLevelName(USER_ADMINISTRATOR));
    arrayLevel.Add(GetUserLevelName(USER_SUPERUSER));
    arrayLevel.Add(GetUserLevelName(USER_COMMONUSER));
    arrayLevel.Add(GetUserLevelName(USER_INVALIDUSER));

    m_Grid.SetComboContent(-1,3,arrayLevel);

    CPasswordDoc *pDoc=(CPasswordDoc *)GetDocument();

    long nQty=pDoc->m_nRecordNumber ;

    SetItem(0,1,"用户名");
    SetItem(0,2,"口令");
    SetItem(0,3,"级别");
    SetItem(0,4,"起始年月日");
    SetItem(0,5,"终止年月日");

    for (int row = 0; row < nQty; row++)
    {
        CPassword *p=pDoc->GetPassword (row);
        if(!p)break;

        SetItem(row+1,1,p->sUserName );
        SetItem(row+1,2,p->sPassword );
        SetItem(row+1,3,GetUserLevelName(p->enumLevel));
        SetItem(row+1,4,p->dateStart.GetDateString());
        SetItem(row+1,5,p->dateEnd.GetDateString());
    }

    m_Grid.AutoSize ();
}

void CPasswordView::UpdateDoc()
{
    CFHEditView::UpdateDoc ();

    CPasswordDoc *pDoc=(CPasswordDoc *)GetDocument();

    long i,nRecord=0;

    char sUserName[100];
    char sPassWord[100];

    pDoc->m_manager.RemoveAll();

    enumUserLevel eUserLevel;
    CString sLevel,sDateStart,sDateEnd;
    CMyDate dateStart,dateEnd;
    

    for(i=0;i<pDoc->m_nRecordNumber;i++)
    {		
        GetItem(i+1,1,sUserName );
        GetItem(i+1,2,sPassWord );
        GetItem(i+1,3,sLevel    );
        GetItem(i+1,4,sDateStart );
        GetItem(i+1,5,sDateEnd   );
        dateStart.Set(sDateStart);
        dateEnd.Set(sDateEnd);

        eUserLevel=::GetUserLevel(sLevel);

        pDoc->m_manager.Add(sUserName ,sPassWord,eUserLevel,dateStart,dateEnd );
    }
}

