// FHGridView.cpp : implementation file
//

#include "stdafx.h"
#include "FHEditView.h"
#include "dlgeditviewsize.h"
#include "FHCGlobal.h"
#include "FHMainFrm.h"
#include "DlgAskOneValue.h"
#include "progdlg.h"
#include "fhzoomview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "FHEditDoc.h"
#include "..\include\fheditview.h"
/////////////////////////////////////////////////////////////////////////////
// CFHEditView

extern HINSTANCE glhInstance;	//	实例句柄

IMPLEMENT_DYNCREATE(CFHEditView, CView)

    CFHEditView::CFHEditView()
{

    m_Item.mask = GVIF_TEXT|GVIF_FORMAT;
    m_Item.nFormat = DT_LEFT|DT_WORDBREAK;

    // The popup menu
    m_Grid.SetBkColor (RGB(128,128,128));
    m_Grid.SetTextBkColor(RGB(255,255,255));
    m_Grid.SetFixedBkColor (RGB(225,225,225));
    m_Grid.SetFixedTextColor (RGB(0,0,0));

    m_iCurrRow=0;
    m_iCurrCol=0;

    m_nColNumber=10;

    m_sFile="";
    m_fp=NULL;
    m_nRecordItemLimit=100;
    m_nItemCharLimit=18;
    m_psRecord=new char * [m_nRecordItemLimit];
    for(long i=0;i<m_nRecordItemLimit;i++)
    {
        m_psRecord[i]=new char [m_nItemCharLimit+5];
        memset(m_psRecord[i],0,m_nItemCharLimit);
    }

    m_sFileHeadMark="FHEDITVIEWHEAD";
    m_nPageRecordQty=100;
    m_nTotalPage=1;
    m_nCurrentPage=0;
    m_nCurrentPageStartRecord=0;
    m_nCurrentPageEndRecord=0;

    m_bInitView=true;

    m_Grid.SetFixedRowCount (1);
    m_Grid.SetFixedColumnCount (1);

    m_menuManager.SetPopupMenu(IDR_FHEditViewPopUp);
}

CFHEditView::~CFHEditView()
{
    if(m_fp)
    {
        fclose(m_fp);
        m_fp=NULL;
    }


    if(m_psRecord)
    {
        for(long i=0;i<m_nRecordItemLimit;i++)
        {
            delete []m_psRecord[i];
            m_psRecord[i]=NULL;
        }
        delete []m_psRecord;

        m_psRecord=NULL;
    }
}


BEGIN_MESSAGE_MAP(CFHEditView, CView)
    //{{AFX_MSG_MAP(CFHEditView)
    ON_WM_SIZE()
    ON_WM_CONTEXTMENU()
    ON_COMMAND(ID_FHEditViewAddRow, OnFHEditViewAddRow)
    ON_COMMAND(ID_FHEditViewAddCol, OnFHEditViewAddCol)
    ON_WM_CREATE()
    ON_COMMAND(ID_FHEditViewDelRow, OnFHEditViewDelRow)
    ON_COMMAND(ID_FHEditView_DelCol, OnFHEditViewDelCol)
    ON_COMMAND(ID_FHEditViewInsRow, OnFHEditViewInsRow)
    ON_COMMAND(ID_FHEditViewInsCol, OnFHEditViewInsCol)
    ON_COMMAND(ID_FHEditViewUpdateGrid, OnFHEditViewUpdateGrid)

    ON_MESSAGE(GV_MODIFIED, OnModified) 
    ON_MESSAGE(GV_COMBOMODIFIED, OnComboModified) 

    ON_WM_KEYDOWN()
    ON_WM_ERASEBKGND()
    //}}AFX_MSG_MAP
    ON_COMMAND(ID_FHEDITVIEWPOPUP_Copy, OnFheditviewpopupCopy)
    ON_COMMAND(ID_FHEDITVIEWPOPUP_CUT, OnFheditviewpopupCut)
    ON_COMMAND(ID_FHEDITVIEWPOPUP_Paste, OnFheditviewpopupPaste)
    ON_COMMAND(ID_FHEditView_DelAllSelRow, OnFheditviewDelallserow)
    ON_COMMAND(ID_FHEditView_DelAllSelCol, OnFheditviewDelallselcol)
    ON_COMMAND(ID_FHEditViewReWriteOrder, OnFheditviewrewriteorder)
    ON_COMMAND(ID_FHEditView_DelAllSameRow, OnFheditviewDelallsamerow)
    ON_COMMAND(ID_FHEDITVIEWPOPUP_BatchEdit, OnFheditviewpopupBatchedit)
    ON_WM_LBUTTONUP()
    ON_WM_RBUTTONDOWN()
    ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
    ON_COMMAND(ID_EDIT_CUT, OnEditCut)
    ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
    ON_COMMAND(ID_FHEditView_MarlAllSameRowWithCols, OnFheditviewMarlallsamerowwithcols)
    ON_COMMAND(ID_FHEditView_MarlAllSameRowsOnCol, OnFheditviewMarlallsamerowsoncol)
    ON_COMMAND(ID_EditView_RemoveSameMark, OnEditviewRemovesamemark)
    ON_COMMAND(ID_FHSettings, OnSettings)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFHEditView drawing

void CFHEditView::OnInitialUpdate()
{
    UINT nID=WM_USER+200;
    m_Grid.Create(CRect(0,0,100,100),this,nID);
    m_Grid.SetMyParent (this);
    m_Grid.SetParent (this);


    //HINSTANCE hInstance = ::AfxGetInstanceHandle();

    LPTSTR p=MAKEINTRESOURCE(IDR_ACCELERATORFHEDITVIEW);
    m_accel = LoadAccelerators((HINSTANCE)glhInstance ,p);

    CView::OnInitialUpdate();
}

void CFHEditView::OnDraw(CDC* pDC)
{
    CDocument* pDoc = GetDocument();
}

/////////////////////////////////////////////////////////////////////////////
// CFHEditView diagnostics

#ifdef _DEBUG

void CFHEditView::AssertValid() const
{
    CView::AssertValid();
}

void CFHEditView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}

#endif


/////////////////////////////////////////////////////////////////////////////
// CFHEditView message handlers

void CFHEditView::OnSize(UINT nType, int cx, int cy) 
{
    CView::OnSize(nType, cx, cy);

    // TODO: Add your message handler code here
    if(IsWindow(m_Grid.m_hWnd))
    {
        m_Grid.MoveWindow(CRect(0,0,cx,cy));
    }	
}
void CFHEditView::SetSize(int nRow, int nCol)
{
    m_Grid.SetEditable(true);
    //	m_Grid.SetListMode(true);
    m_Grid.EnableDragAndDrop(TRUE);
    m_Grid.SetHeaderSort(false);

    int iFixedRow=m_Grid.GetFixedRowCount ();
    int iFixedCol=m_Grid.GetFixedColumnCount ();


    try 
    {
        m_Grid.RemoveAllPics ();
        m_Grid.DeleteAllItems ();
        m_Grid.SetRowCount(nRow);
        m_Grid.SetColumnCount(nCol);
        m_Grid.SetFixedRowCount(iFixedRow);
        m_Grid.SetFixedColumnCount(iFixedCol);
    }
    catch (CMemoryException* e)
    {
        e->ReportError();
        e->Delete();
        return;
    }

    m_Grid.SetItemState(0,0, m_Grid.GetItemState(0,0) | GVIS_READONLY);


    //////////////////////////////////////////////////////
    // Resize the grid:
    CRect rect;
    GetClientRect(&rect);
    if(IsWindow(m_Grid))
    {
        m_Grid.AutoSize ();
        m_Grid.MoveWindow(rect);
    }	

    return;

}

void CFHEditView::UpdateDoc()
{
    // TODO: Add your command handler code here
    CFHEditDoc *pDoc=(CFHEditDoc *)GetDocument();

    long nRecord=GetValidRowCount();

    pDoc->SetRecordNumber(nRecord);
    if(!m_bInitView&&m_Grid.GetModified ())
    {
        pDoc->SetModifiedFlag ();
    }
}

long CFHEditView::GetValidRowCount()
{
    CString sTest;
    long i,j,nRecord=0;
    long nTC=m_Grid.GetColumnCount ();
    long nFC=m_Grid.GetFixedColumnCount ();
    for(i=m_Grid.GetRowCount ()-1;i>=m_Grid.GetFixedRowCount();i--)
    {
        for(j=nFC;j<nTC;j++)
        {
            GetItem(i,j,sTest);
            sTest.Trim();
            sTest.MakeUpper();
            if(sTest!="")
            {
                nRecord=i-m_Grid.GetFixedRowCount()+1;
                break;
            }
        }
        if(nRecord!=0)break;
    }

    return nRecord;
}


void CFHEditView::UpdateGrid()
{
    long nCurrRow=m_Grid.GetRowCount ();
    long nFixedRow=m_Grid.GetFixedRowCount ();
    long nFixedCol=m_Grid.GetFixedColumnCount ();

    CFHEditDoc *pDoc=(CFHEditDoc *)GetDocument();
    if(!pDoc)return;
    m_Grid.DeleteAllItems ();

    long nMaxRow=m_nPageRecordQty;	
    if(pDoc->m_nRecordNumber>nMaxRow)nMaxRow=pDoc->m_nRecordNumber+10;

    if(nCurrRow>nMaxRow)nMaxRow=nCurrRow;

    SetSize(nMaxRow,m_nColNumber);
    m_Grid.SetFixedRowCount (nFixedRow);
    m_Grid.SetFixedColumnCount (nFixedCol);

    WriteRowOrder();
}

void CFHEditView::SetForeColor(int nRed,int nGreen,int nBlue, bool bReDraw)
{	  
    if(nRed<0)nRed=0;
    if(nRed>255)nRed=255;
    if(nGreen<0)nGreen=0;
    if(nGreen>255)nGreen=255;
    if(nBlue<0)nBlue=0;
    if(nBlue>255)nBlue=255;	 

    m_Grid.SetTextColor (RGB(nRed,nGreen,nBlue));
    if(bReDraw)Invalidate();

}

void CFHEditView::SetBackColor(int nRed,int nGreen,int nBlue, bool bReDraw)
{
    if(nRed<0)nRed=0;
    if(nRed>255)nRed=255;
    if(nGreen<0)nGreen=0;
    if(nGreen>255)nGreen=255;
    if(nBlue<0)nBlue=0;
    if(nBlue>255)nBlue=255;	 

    m_Grid.SetBkColor (RGB(nRed,nGreen,nBlue));
    if(bReDraw)Invalidate();
}

void CFHEditView::SetItem(int nRow, int nCol, long nData,UINT nFormat)
{
    m_Item.row = nRow;
    m_Item.col = nCol;
    if(nFormat>0)m_Item.nFormat = nFormat;

    m_Item.szText.Format("%ld",nData);
    m_Grid.SetItem(&m_Item);
}

void CFHEditView::SetBoolItem(int nRow, int nCol, BOOL bData)
{
    m_Item.row = nRow;
    m_Item.col = nCol;

    if(bData)
    {
        m_Item.szText="YES";
    }
    else
    {
        m_Item.szText="NO";
    }

    m_Grid.SetItem(&m_Item);
}

void CFHEditView::SetItem(int nRow, int nCol, int nData,UINT nFormat)
{
    m_Item.row = nRow;
    m_Item.col = nCol;
    if(nFormat>0)m_Item.nFormat = nFormat;

    m_Item.szText.Format("%d",nData);
    m_Grid.SetItem(&m_Item);
}

void CFHEditView::SetItem(int nRow, int nCol, float fData,UINT nFormat,int nSmallNumberQty)
{
    m_Item.row = nRow;
    m_Item.col = nCol;
    if(nFormat>0)m_Item.nFormat = nFormat;

    m_Item.szText=vtos(fData,nSmallNumberQty);

    m_Grid.SetItem(&m_Item);
}

void CFHEditView::SetItem(int nRow, int nCol, double dData,UINT nFormat,int nSmallNumberQty)
{
    m_Item.row = nRow;
    m_Item.col = nCol;
    if(nFormat>0)m_Item.nFormat = nFormat;

    m_Item.szText=vtos(dData,nSmallNumberQty);

    m_Grid.SetItem(&m_Item);

}

void CFHEditView::SetItem(int nRow, int nCol, CString sData,UINT nFormat)
{
    m_Item.row = nRow;
    m_Item.col = nCol;
    if(nFormat>0)m_Item.nFormat = nFormat;

    m_Item.szText=sData;
    m_Grid.SetItem(&m_Item);
}

void CFHEditView::SetItem(int nRow, int nCol, const char *sData,UINT nFormat)
{
    m_Item.row = nRow;
    m_Item.col = nCol;
    if(nFormat>0)m_Item.nFormat = nFormat;

    m_Item.szText=CString(sData);
    m_Grid.SetItem(&m_Item);
}



void CFHEditView::GetItem(int nRow, int nCol, float *pTarget)
{
    m_Item.row = nRow;
    m_Item.col = nCol;
    if(m_Grid.GetItem(&m_Item)){
        *pTarget=(float)(atof(LPCTSTR(m_Item.szText )));
    }
}

void CFHEditView::GetItem(int nRow, int nCol, double *pTarget)
{
    m_Item.row = nRow;
    m_Item.col = nCol;
    if(m_Grid.GetItem(&m_Item)){
        *pTarget=atof(LPCTSTR(m_Item.szText ));
    }
}

void CFHEditView::GetItem(int nRow, int nCol, long *pTarget)
{
    m_Item.row = nRow;
    m_Item.col = nCol;
    if(m_Grid.GetItem(&m_Item)){	
        *pTarget=atol(LPCTSTR(m_Item.szText ));
    }
}

void CFHEditView::GetItem(int nRow, int nCol, CString &sTarget)
{
    m_Item.row = nRow;
    m_Item.col = nCol;
    m_Item.mask = GVIF_TEXT;
    if(m_Grid.GetItem(&m_Item)){
        sTarget=m_Item.szText;
    }
}


void CFHEditView::GetItem(int nRow, int nCol, char *sTarget)
{
    m_Item.row = nRow;
    m_Item.col = nCol;
    if(m_Grid.GetItem(&m_Item)){
        strcpy(sTarget,(LPCTSTR)m_Item.szText);
    }
}

void CFHEditView::GetItem(int nRow, int nCol, int  *pTarget)
{
    m_Item.row = nRow;
    m_Item.col = nCol;
    if(m_Grid.GetItem(&m_Item))
    {
        *pTarget=atoi(LPCTSTR(m_Item.szText ));
    }
}

void CFHEditView::GetBoolItem(int nRow, int nCol, BOOL  *pbTarget)
{
    m_Item.row = nRow;
    m_Item.col = nCol;
    if(m_Grid.GetItem(&m_Item))
    {
        CString s=m_Item.szText ;
        s.MakeUpper ();

        if(s=="T"||s=="TRUE"||s==".T."||s=="真"||s=="是"||s=="YES"||s=="Y")
        {
            *pbTarget=true;
        }
        else
        {
            *pbTarget=false;
        }

    }
}

void CFHEditView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
    // TODO: Add your specialized code here and/or call the base class
    UpdateGrid();
    if(m_bInitView){
        m_Grid.SetModified (false);
        m_bInitView=false;
    }		
    CView::OnUpdate(pSender, lHint,  pHint) ;

}

void CFHEditView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
    // TODO: Add your messa           e handler code here
    CRect rect;
    GetWindowRect (&rect);
    CPoint pointInWnd=point-rect.TopLeft ();
    CCellID id=m_Grid.GetCellFromPt (pointInWnd);

    m_iCurrRow	    =id.row;
    m_iCurrCol      =id.col;

    CMenu *pMenu=m_menuManager.GetPopupMenu();
    if(!pMenu)return;

    m_menuManager.ShowPopupMenu(pMenu,point,this);
}

void CFHEditView::OnFHEditViewAddRow() 
{
    // TODO: Add your command handler code here
    m_Grid.InsertRow("Added Row");
    m_Grid.SetModified ();
    Invalidate();
}

void CFHEditView::OnFHEditViewAddCol() 
{
    // TODO: Add your command handler code here
    m_Grid.InsertColumn("新增列");
    m_Grid.SetModified ();
    Invalidate();
}

void CFHEditView::SetEditable(int nRow,int nCol,bool bEditable)
{
    if(bEditable)
    {
        m_Grid.SetItemState(nRow,nCol,m_Grid.GetItemState(0,0) & !GVIS_READONLY);
    }
    else
    {
        m_Grid.SetItemState(nRow,nCol,m_Grid.GetItemState(0,0) |GVIS_READONLY);
    }
}
void CFHEditView::OnFHEditViewSetSize() 
{
    // TODO: Add your command handler com_n
    CDlgEditViewSize dlg;
    dlg.m_nRowNumber =m_Grid.GetRowCount();
    dlg.m_nColNumber =m_Grid.GetColumnCount();

    if(dlg.DoModal ()==IDCANCEL)return;

    SetSize(dlg.m_nRowNumber,dlg.m_nColNumber);
    UpdateGrid();

}

void CFHEditView::SetItemBkColor(int nRow, int nCol, COLORREF nColor)
{
    m_Grid.SetItemBkColour (nRow,nCol,nColor);
}

int CFHEditView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    if (CView::OnCreate(lpCreateStruct) == -1)
        return -1;

    return 0;
}

CFHEditDoc* CFHEditView::GetDocument()
{
    if(!m_pDocument)return NULL;
    return (CFHEditDoc*)m_pDocument;
}

void CFHEditView::OnFHEditViewDelRow() 
{
    // TODO: Add your command handler code here
    CCellID cell=m_Grid.GetFocusCell();
    m_Grid.DeleteRow (cell.row );
    m_Grid.ResetSelectedRange();

    m_Grid.AutoSizeRows ();
    m_Grid.SetModified ();
    Invalidate();

}

void CFHEditView::OnFHEditViewDelCol() 
{
    // TODO: Add your command handler code here
    CCellID cell=m_Grid.GetFocusCell();
    m_Grid.DeleteColumn (cell.col );
    m_Grid.ResetSelectedRange();

    m_Grid.AutoSizeColumns ();
    m_Grid.SetModified ();
    Invalidate();
}

void CFHEditView::OnFHEditViewInsRow() 
{
    // TODO: Add your command handler code here
    CCellID cell=m_Grid.GetFocusCell();
    m_Grid.InsertRow("新插入行",cell.row );
    m_Grid.AutoSizeRows ();
    m_Grid.SetModified ();
    Invalidate();
}

void CFHEditView::OnFHEditViewInsCol() 
{
    // TODO: Add your command handler code here
    CCellID cell=m_Grid.GetFocusCell();
    m_Grid.InsertColumn("新插入列",DT_CENTER|DT_VCENTER|DT_SINGLELINE ,cell.col );
    m_Grid.SetModified ();
    m_Grid.AutoSizeColumns ();
    Invalidate();

}


void CFHEditView::OnFHEditViewUpdateGrid() 
{
    // TODO: Add your command handler code here
    UpdateGrid();

}


BOOL CFHEditView::OnEraseBkgnd(CDC* pDC) 
{
    //return CView::OnEraseBkgnd(pDC);
    return true;
}


LRESULT CFHEditView::OnComboModified(WPARAM wParam, LPARAM lParam) 
{ 
    CFHEditDoc *pDoc=GetDocument();
    if(pDoc)
    {
        pDoc->SetModifiedFlag();
    }

    CInPlaceList * pCombo=m_Grid.GetComboBox();

    if(!pCombo)return 0;
    CString sOldString,sNewString;

    //int n=pCombo->GetCurSel();
    //pCombo->GetLBText(n,sNewString);
    pCombo->GetWindowText(sOldString);

    ComboModified(wParam,lParam,sOldString);

    return 0;
} 

LRESULT CFHEditView::OnModified(WPARAM wParam, LPARAM lParam) 
{ 
    CFHEditDoc *pDoc=GetDocument();
    if(pDoc)
    {
        pDoc->SetModifiedFlag();
    }

    GridModified(wParam,lParam);

    return 0;
}

void CFHEditView::GridModified(int iRow, int iCol)
{
    return;
}

void CFHEditView::ComboModified(int iRow, int iCol, CString sNewString)
{
    CString s;
    GetItem(iRow,iCol,s);
    return;


}

void CFHEditView::OnFheditviewpopupCopy()
{
    m_Grid.OnEditCopy ();
}

void CFHEditView::OnFheditviewpopupCut()
{
    m_Grid.OnEditCut ();
}

void CFHEditView::OnFheditviewpopupPaste()
{
    m_Grid.OnEditPaste ();
}

BOOL CFHEditView::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    int r = TranslateAccelerator(m_hWnd, m_accel, pMsg);			
    if(r)				
    {			
        return TRUE;			
    }

    return __super::PreTranslateMessage(pMsg);
}

void CFHEditView::SetBkColourCol(int nCol,COLORREF nColor)
{
    for(int i=m_Grid.GetFixedRowCount();i<m_Grid.GetRowCount();i++)
    {
        m_Grid.SetItemBkColour(i,nCol,nColor);
    }
}

void CFHEditView::SetBkColourRow(int nRow,COLORREF nColor)
{
    for(int i=m_Grid.GetFixedColumnCount();i<m_Grid.GetColumnCount();i++)
    {
        m_Grid.SetItemBkColour(nRow,i,nColor);
    }
}
void CFHEditView::OnFheditviewDelallserow()
{
    CCellRange cell=m_Grid.GetSelectedCellRange();
    int nMinRow=cell.GetMinRow ();
    int nMaxRow=cell.GetMaxRow ();
    for(int i=nMinRow;i<=nMaxRow;i++)
    {
        m_Grid.DeleteRow (nMinRow);
    }
    m_Grid.ResetSelectedRange();
    WriteRowOrder();
    m_Grid.AutoSize();
}

void CFHEditView::OnFheditviewDelallselcol()
{
    CCellRange cell=m_Grid.GetSelectedCellRange();
    int nMinCol=cell.GetMinCol();
    int nMaxCol=cell.GetMaxCol ();
    for(int i=nMinCol;i<=nMaxCol;i++)
    {
        m_Grid.DeleteColumn (nMinCol);
    }
    m_Grid.ResetSelectedRange();
    m_Grid.AutoSize();
}

void CFHEditView::OnFheditviewrewriteorder()
{
    WriteRowOrder();
    m_Grid.AutoSize();
}

void CFHEditView::WriteRowOrder()
{
    SetItem(0,0,"序号");
    CString sText;
    int n=m_Grid.GetFixedRowCount ();
    for (int row = n; row < m_Grid.GetRowCount(); row++)
    {
        sText.Format(_T("第%i行"),row-n+m_nCurrentPageStartRecord+1);
        SetItem(row,0,sText);
    }
}

void CFHEditView::GetMarkCol(int &nFC,int &nTC)
{
    nFC=m_Grid.GetFixedColumnCount ();
    nTC=nFC;
}

//////////////////////////////////////////////////
// Clear the "DeleteMe" mark and
// reset the back color to default
//////////////////////////////////////////////////
void CFHEditView::ClearSameMark()
{
    CString s;
    int i,n;
    for(i=0;i<m_Grid.GetRowCount();i++)
    {
        GetItem(i,0,s);
        n=s.Find(':');
        if(n!=-1)
        {
            s=s.Left(n);
            SetItem(i,0,s);
        }
    }
    m_Grid.AutoSize();
}

void CFHEditView::OnFheditviewDelallsamerow()
{
    if(AfxMessageBox("你是否确认删除所有与选定行内容相同的行?",MB_YESNO)==IDNO)
    {
        return ;
    }

    // Mark Same rows:
    CCellRange cell=m_Grid.GetSelectedCellRange();	
    int nMinRow=cell.GetMinRow ();
    int nMaxRow=cell.GetMaxRow ();	
    int nMinCol=cell.GetMinCol ();
    int nMaxCol=cell.GetMaxCol ();


    // 对与选定区域相同的行作标志：
    ClearSameMark();
    int iCheckOrder=0;
    MarkSameRowByCol(nMinRow,nMaxRow,nMinCol,nMaxCol,iCheckOrder,false,true);

    // 删除有标志的行：
    CGridCell *pCell=NULL;
    bool bNeedDelete=true;
    int nRow=0;
    CString s;
    while(true)
    {
        if(nRow>=m_Grid.GetRowCount ())break;
        GetItem(nRow,0,s);
        s.Trim().MakeUpper();
        if(s.Find(":SAME")!=-1)
        {
            m_Grid.DeleteRow (nRow);
        }
        else
        {
            nRow++;
        }		
    }

    // Reset:
    m_Grid.ResetSelectedRange();
    m_Grid.AutoSize();
}

/////////////////////////////////////////////////////////////////////////
// MarkSameRowByCol
// 把所有与nStartCol列和nEndCol之间和nStartRow与nEndRow之间相同的行
// 在第一列加上标志(:SAME)
/////////////////////////////////////////////////////////////////////////
void CFHEditView::MarkSameRowByCol(int nStartRow,
    int nEndRow,
    int nStartCol,
    int nEndCol,
    int &iCheckOrder,
    bool bOnlyAfter,
    bool bCreateProgressBar)
{
    int nSelRowQty=nEndRow-nStartRow+1;

    CString s;
    int i,j,k;

    bool bSame=false;
    bool bHasSameRow=false;
    CString sMark;
    CGridCell *pCell=NULL;

    // Check if the selected grids are empty:
    bool bEmpty=true;
    for(i=nStartRow;i<=nEndRow;i++)
    {
        for(j=nStartCol;j<=nEndCol;j++)
        {
            GetItem(i,j,sMark);
            sMark.Trim();
            if(sMark!="")
            {
                bEmpty=false;
                break;
            }
        }
    }
    if(bEmpty)return;

    // Check same grids:
    int iStartCheck=0;
    bool bMarked=false;
    if(bOnlyAfter)
    {
        iStartCheck=nEndRow;
    }

    CProgressDlg dlg;

    if(bCreateProgressBar)
    {
        dlg.Create();
        dlg.SetRange(iStartCheck,m_Grid.GetRowCount ()-nSelRowQty+1);
        dlg.SetStatus("正在标注...");
    }   

    bool bFailed=false;
    for(i=iStartCheck;i<m_Grid.GetRowCount ()-nSelRowQty+1;i++)
    {
        if(bCreateProgressBar)
        {
            dlg.SetPos(i);
            if(dlg.CheckCancelButton())
            {
                bFailed=true;
                break;
            }
        }

        if(i>=nStartRow&&i<=nEndRow)continue;
        GetItem(i,0,s);
        if(s.Find(':')!=-1)continue;


        // Check if all cells are same:
        bSame=true;
        for(j=0;j<nSelRowQty;j++)
        {
            for(k=nStartCol;k<=nEndCol;k++)
            {
                GetItem(i+j,k,sMark);
                sMark.Trim ();

                GetItem(nStartRow+j,k,s);

                if(sMark!=s)
                {
                    bSame=false;
                    break;
                }
            }
            if(bSame==false)break;
        }

        // if same:
        if(bSame)
        {
            bMarked=false;
            for(j=0;j<nSelRowQty;j++)
            {
                GetItem(i+j,0,s);
                s.Trim();
                s.MakeUpper();
                if(s.Find(':')==-1)
                {
                    s+=":SAME"+vtos(iCheckOrder);
                    SetItem(i+j,0,s);
                    bMarked=true;
                }
            }
            bHasSameRow=true;

        }
    }

    if(bHasSameRow&&(!bFailed))
    {
        // Mark the comparing row:
        for(i=nStartRow;i<=nEndRow;i++)
        {
            GetItem(i,0,s);
            s.Trim();
            s.MakeUpper();
            if(s.Find(':')==-1)
            {
                s+=":SAME"+vtos(iCheckOrder);
                SetItem(i,0,s);
            }
        }

        // iNC THE ORDER:
        iCheckOrder++;
    }

    if(bCreateProgressBar)
    {
        dlg.DestroyWindow();
    }
}


bool CFHEditView::CreateFile(CString sFile)
{
    FILE *fp=fopen (sFile,"wb");
    if(!fp)
    {
        TRACEERROR("错误：无法建立文件:"+sFile);
        return false;
    }

    CFHEditViewHead head;
    fwrite(&head, sizeof(CFHEditViewHead),1,fp);

    fclose(fp);

    return true;
}


bool CFHEditView::OpenFile(CString sFile)
{
    FILE *fp=fopen (sFile,"rb+");
    if(!fp)
    {
        TRACEERROR("错误：无法打开文件:"+sFile);
        return false;
    }

    fread(&m_head, sizeof(CFHEditViewHead),1,fp);

    if(strcmp(m_head.sFileMark ,m_sFileHeadMark)!=0)
    {
        TRACEERROR("错误：该文件不是正确的编辑器临时文件："+sFile);
        fclose(fp);
        return false;
    }

    ////////////////////////
    //
    m_sFile=sFile;
    if(m_fp)
    {
        fclose(m_fp);
        m_fp=NULL;
    }
    m_fp=fp;

    return true;
}

bool CFHEditView::AppendRecord(char **psRecords, long nItemQty)
{
    if(!m_fp)
    {
        TRACEERROR("错误：尚未打开编辑器临时文件，无法附加记录!");
        return false;
    }
    if(m_head.nRecordQty >=m_head.nRecordLimit )
    {
        TRACEERROR("错误：记录数目超限！");
        return false;
    }

    ///////////////////////////////////////////
    // 找到上一个记录的末尾：
    long nPos=sizeof(CFHEditViewHead)+m_head.nRecordQty *sizeof(long);
    fseek(m_fp,nPos,SEEK_SET);

    ////////////////////////////////
    // 写入标志和数据
    fwrite(&nPos,sizeof(long),1,m_fp);
    fwrite(&nItemQty,sizeof(long),1,m_fp);
    long nItemLen;
    for(long i=0;i<nItemQty;i++)
    {
        nItemLen=strlen(psRecords[i]);
        fwrite(&nItemLen,sizeof(long),1,m_fp);
        fwrite(psRecords[i],nItemLen,1,m_fp);
    }

    ////////////////////////
    // 记录数目增加1：
    m_head.nRecordQty ++;

    m_nTotalPage=m_head.nRecordQty/m_nPageRecordQty+1;


    return true;
}


bool CFHEditView::Over()
{
    if(!m_fp)
    {
        TRACEERROR("错误：没有临时文件被打开！");
        return false;
    }

    fseek(m_fp,0,SEEK_SET);
    fwrite(&m_head,sizeof(CFHEditViewHead),1,m_fp);

    fclose(m_fp);
    m_fp=NULL;
    m_sFile="";

    return true;
}

long CFHEditView::GetRecordItemQty(long nRecordOrder)
{
    if(!m_fp)
    {
        TRACEERROR("错误：尚未打开编辑器临时文件，无法附加记录!");
        return -1;
    }

    ///////////////////////////////////////////
    // 找到上一个记录的末尾：
    long nPos=sizeof(CFHEditViewHead)+m_head.nRecordQty *sizeof(long);
    fseek(m_fp,nPos,SEEK_SET);

    long nPosFile;
    long nItemQty;
    fread(&nPosFile,sizeof(long),1,m_fp);

    if(nPosFile!=nPos)
    {
        TRACEERROR("错误：该文件错误！");
        return -1;
    }

    fread(&nItemQty,sizeof(long),1,m_fp);

    if(nItemQty<0||nItemQty>1000)
    {
        TRACEERROR("错误：该记录中字段数目错误:"+vtos(nItemQty));
        return -1;
    }

    return nItemQty;
}

char ** CFHEditView::GetRecord(long nRecordOrder, long &nItemQty)
{
    if(!m_fp)
    {
        TRACEERROR("错误：尚未打开编辑器临时文件，无法附加记录!");
        return NULL;
    }

    ///////////////////////////////////////////
    // 找到上一个记录的末尾：
    long nPos=sizeof(CFHEditViewHead)+m_head.nRecordQty *sizeof(long);
    fseek(m_fp,nPos,SEEK_SET);

    long nPosFile;
    fread(&nPosFile,sizeof(long),1,m_fp);

    if(nPosFile!=nPos)
    {
        TRACEERROR("错误：该文件错误！");
        return NULL;
    }

    fread(&nItemQty,sizeof(long),1,m_fp);

    if(nItemQty<0||nItemQty>m_nRecordItemLimit)
    {
        TRACEERROR("错误：该记录中字段数目错误:"+vtos(nItemQty));
        return NULL;
    }

    long nItemLen;
    bool bFailed=false;
    for(long i=0;i<nItemQty;i++)
    {
        fread(&nItemLen,sizeof(long),1,m_fp);
        if(nItemLen<0||nItemLen>m_nItemCharLimit)
        {
            TRACEERROR("错误：字段内字符长度超限:"+vtos(nItemLen));
            bFailed=false;
            break;
        }
        fread(m_psRecord[i],sizeof(char),nItemLen,m_fp);
        memset(&m_psRecord[i][nItemLen],0,2);
    }

    ///////////////////////////
    //
    if(bFailed)
    {
        return NULL;
    }
    else
    {
        return m_psRecord;
    }
}

void CFHEditView::SetPageRecordQty(long nRecordQty)
{
    if(nRecordQty<1||nRecordQty>1500)
    {
        TRACEERROR("错误:每页的记录数目不建议设定太多！");
        return;
    }

    m_nPageRecordQty=nRecordQty;
    m_nTotalPage=m_head.nRecordQty/m_nPageRecordQty+1;

    long nCurrentPage=m_nCurrentPageStartRecord/m_nPageRecordQty;
    if(!SetPage(nCurrentPage))
    {
        SetPage(0);
    }
}

bool CFHEditView::SetPage(long nPageOrder)
{
    if(nPageOrder<0||nPageOrder>=m_nTotalPage)
    {
        TRACEERROR("错误：页号超出范围:"+vtos(nPageOrder)+", 总页数:"+vtos(m_nTotalPage));
        return false;
    }

    m_nCurrentPage=nPageOrder;
    m_nCurrentPageStartRecord=m_nCurrentPage*m_nPageRecordQty;
    m_nCurrentPageEndRecord=(m_nCurrentPage+1)*m_nPageRecordQty-1;
    if(m_nCurrentPageEndRecord>=m_head.nRecordQty)
    {
        m_nCurrentPageEndRecord=m_head.nRecordQty;
    }

    UpdateGrid();

    return true;
}

bool CFHEditView::GotoNextPage()
{
    if(m_nCurrentPage>=(m_nTotalPage-1))
    {
        TRACEERROR("错误：已经是最后一页！");
        return true;
    }
    m_nCurrentPage++;

    m_nCurrentPageStartRecord=m_nCurrentPage*m_nPageRecordQty;
    m_nCurrentPageEndRecord=(m_nCurrentPage+1)*m_nPageRecordQty-1;
    if(m_nCurrentPageEndRecord>=m_head.nRecordQty)
    {
        m_nCurrentPageEndRecord=m_head.nRecordQty;
    }

    UpdateGrid();

    return true;
}

bool CFHEditView::GotoPrevPage()
{	
    if(m_nCurrentPage<=0)
    {
        TRACEERROR("错误：已经是最后一页！");
        return true;
    }
    m_nCurrentPage--;

    m_nCurrentPageStartRecord	=m_nCurrentPage*m_nPageRecordQty;
    m_nCurrentPageEndRecord		=(m_nCurrentPage+1)*m_nPageRecordQty-1;

    if(m_nCurrentPageEndRecord	>=m_head.nRecordQty)
    {
        m_nCurrentPageEndRecord=m_head.nRecordQty;
    }

    UpdateGrid();

    return true;
}

void CFHEditView::OnFheditviewpopupBatchedit()
{
    CDlgAskOneValue dlg;
    dlg.SetAsk("输入增量");
    dlg.SetTitle("批量修改表格值");
    dlg.SetValue(-4);
    if(dlg.DoModal ()==IDCANCEL)
    {
        return;
    }

    float fIncrease;
    dlg.GetValue(fIncrease);

    CCellRange range=m_Grid.GetSelectedCellRange() ;

    float fValue;

    long i,j;
    for(i=range.m_nMinRow ;i<=range.m_nMaxRow ;i++)
    {
        for(j=range.m_nMinCol ;j<=range.m_nMaxCol ;j++)
        {
            GetItem(i,j,&fValue);
            fValue+=fIncrease;
            SetItem(i,j,fValue);
        }
    }

    Invalidate();

}


void CFHEditView::OnLButtonUp(UINT nFlags, CPoint point)
{
    CCellID id=m_Grid.GetCellFromPt (point);
    m_iCurrRow =id.row;
    m_iCurrCol =id.col;

    CView::OnLButtonUp(nFlags, point);
}


long CFHEditView::GetCurrentRowPos()
{
    return m_iCurrRow;
}

long CFHEditView::GetCurrentColPos()
{
    return m_iCurrCol;
}

void CFHEditView::OnRButtonDown(UINT nFlags, CPoint point)
{
    CView::OnRButtonDown(nFlags, point);
}

void CFHEditView::OnEditCopy()
{
    m_Grid.OnEditCopy();
}

void CFHEditView::OnEditCut()
{
    m_Grid.OnEditCut();
}

void CFHEditView::OnEditPaste()
{
    m_Grid.OnEditPaste();
}

void CFHEditView::OnFheditviewMarlallsamerowwithcols()
{
    CCellRange cell=m_Grid.GetSelectedCellRange();	
    int nMinRow=cell.GetMinRow ();
    int nMaxRow=cell.GetMaxRow ();	
    int nMinCol=cell.GetMinCol ();
    int nMaxCol=cell.GetMaxCol ();

    ClearSameMark();

    int iCheckOrder=0;
    MarkSameRowByCol(nMinRow,nMaxRow,nMinCol,nMaxCol,iCheckOrder,false,true);
    Invalidate();
    m_Grid.AutoSize();

}

void CFHEditView::OnFheditviewMarlallsamerowsoncol()
{
    CCellRange cell=m_Grid.GetSelectedCellRange();	
    int nMinCol=cell.GetMinCol ();
    int nMaxCol=cell.GetMaxCol ();

    ClearSameMark();

    int iCheckOrder=0;
    CProgressDlg dlg;
    dlg.SetRange(0,m_Grid.GetRowCount());
    dlg.Create();
    bool bFailed=false;

    for(int i=0;i<m_Grid.GetRowCount();i++)
    {
        dlg.SetPos(i);
        if(dlg.CheckCancelButton())
        {
            bFailed=true;
            break;
        }
        MarkSameRowByCol(i,i,nMinCol,nMaxCol,iCheckOrder,true,false);
    }
    dlg.DestroyWindow();

    Invalidate();
    m_Grid.AutoSize();
}


void CFHEditView::OnEditviewRemovesamemark()
{
    this->ClearSameMark();
}

void CFHEditView::PreSettingDlg()
{
    CDlgEditViewSize *pDlg=new CDlgEditViewSize;
    pDlg->m_nRowNumber =m_Grid.GetRowCount();
    pDlg->m_nColNumber =m_Grid.GetColumnCount();
    AddPropertyPage(pDlg);
}

void CFHEditView::PostSettingDlg()
{
    CDlgEditViewSize *pDlg=GetPropertyPage(CDlgEditViewSize);
    if(pDlg)
    {
        if(     m_Grid.GetRowCount()!=pDlg->m_nRowNumber
            ||  m_Grid.GetColumnCount()!=pDlg->m_nColNumber)
        {
            SetSize(pDlg->m_nRowNumber,pDlg->m_nColNumber);
            UpdateGrid();
        }
    }
}

void CFHEditView::OnSettings()
{
    ProcessSettings();
}

void CFHEditView::SetInvalidate   ()
{
    Invalidate();
}
