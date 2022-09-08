// FHTreeView.cpp : implementation file
//

#include "stdafx.h"
#include <direct.h>
#include <errno.h>
#include "FHMainFrm.h"
#include "FHApp.h"
#include "zoomview.h"
#include "FHTreeView.h"
#include "DlgAskOneValue.h"
#include "DlgTreeProperty.h"
#include "FHZoomViewGlobal.h"
#include "FHMainFrm.h"
#include "textdoc.h"
#include "textview.h"
#include "childfrm.h"
#include "MultiDocument.h"
#include "MultiView.h"
#include "MultiChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFHTreeView
IMPLEMENT_DYNCREATE(CFHTreeView, CFHTreeViewBase)

//

CFHTreeView::CFHTreeView()
{
    // Get current work directory:
    if( _getcwd(m_sWorkDir.GetBuffer(128), 128) == NULL )
    {
        AfxMessageBox("_getcwd 错误:无法获得当前工作路径!" );
    }
    m_sWorkDir.ReleaseBuffer();	

    m_sDataDiscription=""; //文本文件(*.TXT)"; // Discript the data type
    m_sDataExt=""; // The extension of the line file .f , txt , wd and so on;
    m_sProjectDiscription=""; //"数据库(*.DB)";
    m_sProjectExt=""; //"DB";

    m_pPointerToThis=NULL;
    m_pLinePropertyInArea=NULL;
    m_pAreaNames=NULL;

    m_bShowFullName=false;

    m_nImageList=IDB_FHTREE;

    SetLayerNames(_T("数据库"),_T("工区"),_T("测线"));

    m_pTemplate=NULL;

    m_bTextFile     =   true;


    m_sTreeName="EMPTY";

    Reset();
}

CFHTreeView::~CFHTreeView()
{
    CFHApp *pApp=(CFHApp*)AfxGetApp();
    pApp->RemoveTree (m_sTreeName);

    RemoveTemplates();
    ClearTree();
}

BEGIN_MESSAGE_MAP(CFHTreeView, CFHTreeViewBase)
    //{{AFX_MSG_MAP(CFHTreeView)
    ON_COMMAND(ID_FHTreeNewDataBase, OnFHTreeNewDataBase)
    ON_COMMAND(ID_FHTreeInsertArea, OnFHTreeInsertArea)
    ON_COMMAND(ID_FHTreeInsertLine, OnFHTreeInsertLine)
    ON_COMMAND(ID_FHTreeOpenDataBase, OnFHTreeOpenDataBase)
    ON_COMMAND(ID_FHTreeDataBaseChangeName, OnFHTreeDataBaseChangeName)
    ON_COMMAND(ID_FHTreeRemoveArea, OnFHTreeRemoveArea)
    ON_COMMAND(ID_FHTreeRemoveLine, OnFHTreeRemoveLine)
    ON_COMMAND(ID_FHTreeDrawLineData, OnFHTreeDrawLineData)
    ON_COMMAND(ID_FHTreeDrawLineMap, OnFHTreeDrawLineMap)
    ON_COMMAND(ID_FHTreeDrawAreaMap, OnFHTreeDrawAreaMap)
    ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
    ON_COMMAND(ID_FHTreeNewLineData, OnFHTreeNewLineData)
    ON_WM_CREATE()
    ON_WM_RBUTTONDOWN()
    ON_COMMAND(ID_FHTreeShowFullName, OnFHTreeShowFullName)
    ON_UPDATE_COMMAND_UI(ID_FHTreeShowFullName, OnUpdateFHTreeShowFullName)
    ON_COMMAND(ID_FHTreeProperty, OnFHTreeProperty)
    ON_COMMAND(ID_FHTreeShowSortedName, OnFHTreeShowSortedName)
    ON_UPDATE_COMMAND_UI(ID_FHTreeShowSortedName, OnUpdateFHTreeShowSortedName)
    ON_WM_LBUTTONDBLCLK()

    //}}AFX_MSG_MAP
    ON_COMMAND(ID_FHTreeCloseDataBase, OnFhtreeclosedatabase)
    ON_COMMAND(ID_FHAddCurrItemToBaseMap, OnAddCurrentItemIntoBaseMap)
    ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnTvnEndlabeledit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFHTreeView drawing

void CFHTreeView::OnDraw(CDC* pDC)
{
    CDocument* pDoc = GetDocument();
    CRect rectClient;
    GetClientRect(&rectClient);
    pDC->Rectangle (&rectClient);
    pDC->MoveTo (0,0);
    pDC->LineTo (rectClient.right ,rectClient.bottom );
}

/////////////////////////////////////////////////////////////////////////////
// CFHTreeView diagnostics

#ifdef _DEBUG
void CFHTreeView::AssertValid() const
{
    CFHTreeViewBase::AssertValid();
}

void CFHTreeView::Dump(CDumpContext& dc) const
{
    CFHTreeViewBase::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFHTreeView message handlers

void CFHTreeView::OnFHTreeNewDataBase() 
{
    // Let the user give the new database's name 
    // and let it be m_sDataBaseName	
    MakeNewDataBase();
}

bool CFHTreeView::AddTemplates()
{
    return true;
}

bool CFHTreeView::AddBaseTemplates()
{
    CMultiDocTemplate *pEditTemplate=new CMultiDocTemplate(
        IDR_TEXT_VIEW,
        RUNTIME_CLASS(CTextDoc),
        RUNTIME_CLASS(CChildFrame), // custom MDI child frame
        RUNTIME_CLASS(CTextView));
    AddTemplate(pEditTemplate);

    CMultiDocTemplate *pFaultAreaTemplate=new CMultiDocTemplate(
        IDR_MULTIDOC_VIEW,
        RUNTIME_CLASS(CMultiDocument),
        RUNTIME_CLASS(CChildFrame), // custom MDI child frame
        RUNTIME_CLASS(CMultiView));
    AddTemplate(pFaultAreaTemplate);

    return true;
}

bool CFHTreeView::AddTemplate(CMultiDocTemplate *pTemplate)
{
    CFHApp *pApp=(CFHApp*)AfxGetApp();
    pApp->AddTemplate (pTemplate);
    return true;
}

bool CFHTreeView::RemoveTemplates()
{
    CFHApp *pApp=(CFHApp*)AfxGetApp();
    pApp->RemoveAllTemplates();

    return true;
}

void CFHTreeView::Reset()
{
    m_sDataBaseMark="FHDATABASE";

    m_sDataBaseName=""; // File Including area names
    m_sDataBasePath="";

    // Record of the database:
    m_sError="";

    if(m_pPointerToThis)
    {
        *m_pPointerToThis=NULL;
    }

}


void CFHTreeView::OnFHTreeInsertArea() 
{
    InsertArea();
}


void CFHTreeView::OnFHTreeInsertLine() 
{
    CString sFile=GetTempFile();
    InsertLine(sFile);
}

void CFHTreeView::OnFHTreeOpenDataBase() 
{
    // TODO: Add your command handler code here
    OpenDataBase();
}

void CFHTreeView::OnFHTreeDataBaseChangeName()
{
    CTreeCtrl &treeCtrl=GetTreeCtrl();

    HTREEITEM ItemSelected=treeCtrl.GetSelectedItem ();

    treeCtrl.SetFocus();

    CEdit* pEdit=treeCtrl.EditLabel(treeCtrl.GetRootItem());

    if(!pEdit)
    {
        return ;
    }

    CLineProperty *pLine=(CLineProperty *)(treeCtrl.GetItemData(ItemSelected));
    if(pLine)
    {
        if(m_bShowFullName)
        {
            pEdit->GetWindowText(pLine->sPathName);
        }
        else
        {
            pEdit->GetWindowText(pLine->sShowName);
        }
    }    
}

void CFHTreeView::OnFHTreeRemoveArea() 
{
    RemoveArea();
}


void CFHTreeView::OnFHTreeRemoveLine() 
{
    CString sArea,sLine;
    RemoveLine(sArea,sLine);
}


void CFHTreeView::OnFHTreeDrawLineData() 
{
    // TODO: Add your command handler code here
    CString sArea=GetSelectedAreaName();
    if(sArea=="")
    {
        AfxMessageBox("你需要首先选定一个"+m_sFirstLayerName+"!");
        return;
    }

    CString sLine=GetSelectedLineName();
    if(sLine=="")
    {
        return;
    }

    DrawLineData(sArea,sLine);

    ::AddHistory(eDrawLineData,GetDataBaseName()+":"+sArea+":"+sLine);

}

void CFHTreeView::OnFHTreeDrawLineMap() 
{
    // TODO: Add your command handler code here
    CString sArea=GetSelectedAreaName();
    if(sArea=="")
    {
        AfxMessageBox("你需要首先选定一个"+m_sFirstLayerName+"!");
        return;
    }

    CString sLine=GetSelectedLineName();
    if(sLine=="")
    {
        return;
    }

    DrawLineMap(sArea,sLine);

    ::AddHistory(eDrawLineMap,GetDataBaseName()+":"+sArea+":"+sLine);

}

void CFHTreeView::OnFHTreeDrawAreaMap() 
{
    // TODO: Add your command handler code here
    CString sArea=GetSelectedAreaName();
    if(sArea=="")
    {
        AfxMessageBox("你需要首先选定一个"+m_sFirstLayerName+"!");
        return;
    }
    CString sLine=GetSelectedLineName();

    //
    ::AddHistory(eDrawAreaMap,GetDataBaseName()+":"+sArea);

    //
    long nLineNumber;
    CLineProperty * pLines=GetLineNamesInArea(sArea,nLineNumber);

    DrawAreaMap(sArea,pLines,nLineNumber);

    UpdateDrawingArea(sArea,pLines,nLineNumber,sLine);
}


void CFHTreeView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
    NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;


    CString sAreaLast=m_sAreaCurrent;
    CString sLineLast=m_sLineCurrent;

    m_sAreaCurrent=GetSelectedAreaName();
    m_sLineCurrent=GetSelectedLineName();

    if(m_sAreaCurrent!=sAreaLast)
    {
        long nLineNumber;
        CLineProperty * pLines=GetLineNamesInArea(m_sAreaCurrent,nLineNumber);
        UpdateDrawingArea(m_sAreaCurrent,pLines,nLineNumber,m_sLineCurrent);
    }
    else if(m_sLineCurrent!=sLineLast)
    {
        UpdateDrawingLine(m_sAreaCurrent,m_sLineCurrent);
    }

    *pResult = 0;
}

void CFHTreeView::OnFHTreeNewLineData() 
{
    // TODO: Add your command handler code here
    MakeNewLineData();
}

void CFHTreeView::SetImageList(UINT nImageList)
{
    m_nImageList=nImageList;
}


int CFHTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    ASSERT(m_sProjectExt!="");

    if (CFHTreeViewBase::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO: Add your specialized creation code here
    if(!m_ImageList.Create (m_nImageList, 16, 1, RGB(0,255,0)))
    {
        m_ImageList.Create (IDB_FHTREE, 16, 1, RGB(0,255,0));
    }

    //
    CTreeCtrl *pTreeCtrl = &GetTreeCtrl();
    if(!pTreeCtrl)return NULL;

    pTreeCtrl->SetImageList (&m_ImageList, TVSIL_NORMAL);

    //
    long        lStyleOld;
    lStyleOld = ::GetWindowLong(pTreeCtrl->m_hWnd, GWL_STYLE);
    lStyleOld |= TVS_EDITLABELS;
    ::SetWindowLong(pTreeCtrl->m_hWnd, GWL_STYLE, lStyleOld);

    //
    AddTemplates();
    AddBaseTemplates();

    //
    CFHApp *pApp=(CFHApp*)AfxGetApp();
    pApp->AddTree (this,m_sTreeName);

    return 0;
}


CString CFHTreeView::ConstructLinePath(CString sArea, CString sLine)
{
    return m_sDataBasePath+"\\"+sArea+"\\"+sLine;
}  

CString CFHTreeView::GetDataBaseDiscription()
{
    return m_sProjectDiscription;
}

CString CFHTreeView::GetDataBaseNameExt()
{
    return m_sProjectExt;
}


CString CFHTreeView::ConstructDataBaseName()
{
    return m_sDataBasePath+"\\"+m_sDataBaseName;
}


CString CFHTreeView::ConstructAreaPath(CString  sArea)
{
    return m_sDataBasePath+"\\"+sArea;
}

bool CFHTreeView::ConstructAreaPath(char *cAreaPath,CString sArea)
{
    if(sArea=="")
    {
        return false;
    }
    else
    {
        int nPath=m_sDataBasePath.GetLength();
        int nName=m_sDataBasePath.GetLength();

        strcpy(cAreaPath,(LPCTSTR)m_sDataBasePath);
        cAreaPath[nPath]='\\';
        //cAreaPath[nPath+1]='\\';
        strcpy(&cAreaPath[nPath+1],sArea);
        return true;
    }
}

void CFHTreeView::GetCurrentItemDocNames(CStringList &listLineNames)
{
    CString sArea = GetSelectedAreaName();
    CString sLine = GetSelectedLineName();

    long nLineNumber=0;
    if(sLine!="")
    {
        listLineNames.AddTail("<"+SeperateFileName(sLine)+">");
    }
    else
    {
        CLineProperty *pLines = GetLineNamesInArea(sArea, nLineNumber);
        for(int i=0;i<nLineNumber;i++)
        {
            listLineNames.AddTail("<"+SeperateFileName(pLines[i].sPathName)+">");
        }
    }
    return;
}

int CFHTreeView::MultiDocViewContainsCurrentItem()
{
    CStringList listLineNames;
    GetCurrentItemDocNames(listLineNames);

    CWnd * pWnd = AfxGetApp()->GetMainWnd();
    CFHMainFrm *pFrm = dynamic_cast< CFHMainFrm* >(pWnd);
    if(!pFrm)
    {
        return 0;
    }

    CDocument *pDocument  = pFrm->GetActiveMultiDocument();
    CMultiDocument *pMultiDoc = dynamic_cast< CMultiDocument* > (pDocument);
    if(pMultiDoc)
    {
        return pMultiDoc->ContainsDoc(listLineNames);
    }
    else
    {
        return 0;
    }
}

void CFHTreeView::OnRButtonDown(UINT nFlags, CPoint point) 
{
    // TODO: Add your message handler code here and/or call default	 
    CFHTreeViewBase::OnRButtonDown(nFlags, point);

    CTreeCtrl *pTree=&GetTreeCtrl();
    if(!pTree)return ;

    HTREEITEM item=pTree->HitTest( point , &nFlags );
    pTree->SelectItem(item);

    CMenu *pMenu=m_menuManager.GetPopupMenu();
    if(!pMenu)return ;

    int n = MultiDocViewContainsCurrentItem();
    if(n==2)
    {
        pMenu->CheckMenuItem(ID_FHAddCurrItemToBaseMap,MF_CHECKED);
    }
    else if(n==0)
    {
        pMenu->CheckMenuItem(ID_FHAddCurrItemToBaseMap,MF_UNCHECKED);
    }
    else
    {
        pMenu->CheckMenuItem(ID_FHAddCurrItemToBaseMap,MF_CHECKED);
    }

    ClientToScreen(&point);

    m_menuManager.ShowPopupMenu(pMenu,point,this);

}

void CFHTreeView::SetDataBaseProperty(CString sDiscription, CString sExt)
{
    if(sDiscription!="")
        m_sProjectDiscription=sDiscription;
    if(sExt!="")
        m_sProjectExt=sExt;
}

void CFHTreeView::SetFileProperty(CString sDiscription, CString sExt)
{
    if(sDiscription!="")
        m_sDataDiscription=sDiscription;
    if(sExt!="")
        m_sDataExt=sExt;
}

CString CFHTreeView::GetSelectedLineName()
{
    CString sArea=GetSelectedAreaName();

    HTREEITEM item=GetSelectedLineItem();
    if(item==NULL)
    {
        m_sError="你需要首先选定一"+m_sSecondLayerName;
        return CString ("");
    }

    CTreeCtrl *pTreeCtrl = &GetTreeCtrl();
    if(!pTreeCtrl)return CString("");

    CLineProperty *pLine=(CLineProperty *)pTreeCtrl->GetItemData (item);
    if(!pLine)return CString("");

    CString s=SearchLineData(sArea,pLine->sPathName,pLine->nProperty);
    if(s=="")
    {
        m_sError="无法找到选定"+m_sSecondLayerName+"的数据";
    }

    return s;

}

void CFHTreeView::SetPointerToThis(CFHTreeView **pPointerInApp)
{
    m_pPointerToThis=pPointerInApp;
}

CString CFHTreeView::SearchLineData(CString sArea, CString sLine,UINT nDataType)
{	
    // Search for the area:
    HTREEITEM AreaItem=IsAreaInDataBase(sArea);
    if(AreaItem==NULL)
    {
        CString s="错误：工区:"+sArea+" 不在数据库:"+m_sTreeName+" 内.";
        TRACEERROR(s);
        return CString("");
    }

    // search for the line:
    HTREEITEM LineItem=IsLineInDataBase(sArea,sLine,nDataType);
    if(LineItem==NULL)
    {
        CString s="错误：工区 ";
        s+=sArea;
        s+="中的测线 ";
        s+=SeperateName(sLine);
        s+=" 不在数据库 "+m_sTreeName+" 内";
        TRACEERROR(s);
        return CString("");
    }

    CTreeCtrl *pTreeCtrl = &GetTreeCtrl();
    if(!pTreeCtrl)return CString("");

    CLineProperty* pLine=(CLineProperty*)pTreeCtrl->GetItemData (LineItem);
    sLine=pLine->sPathName ;	

    FILE *fp=fopen(sLine,"rt");
    if(fp)
    {
        fclose(fp);
        return sLine;
    }
    else
    {
        CString sBasePos=ConstructLinePath(sArea,SeperateFileName(sLine));
        fp=fopen(sBasePos,"rt");
        if(fp)
        {
            fclose(fp);
            return sBasePos;
        }
        else
        {
            CString s;
            s+="错误：在数据库"+m_sTreeName+"中没有找到测线数据:"+sBasePos;
            TRACEERROR(s);		
            return CString("");
        }
    }

}

void CFHTreeView::OnFHTreeShowFullName() 
{
    // TODO: Add your command handler code here
    m_bShowFullName=!m_bShowFullName;

    CTreeCtrl *pTree=& GetTreeCtrl();
    if(!pTree)return;

    if(!IsWindow(pTree->m_hWnd ))return ;
    CLineProperty *pLine=NULL;

    HTREEITEM RootItem=pTree->GetRootItem ();
    pLine=(CLineProperty *)pTree->GetItemData(RootItem);
    if(m_bShowFullName)
        pTree->SetItemText (RootItem,pLine->sPathName );
    else
        pTree->SetItemText (RootItem,pLine->sShowName);


    HTREEITEM AreaItem=pTree->GetChildItem(RootItem);


    while(AreaItem)
    {		
        pLine=(CLineProperty *)pTree->GetItemData(AreaItem);
        if(m_bShowFullName)
            pTree->SetItemText (AreaItem,pLine->sPathName );
        else
            pTree->SetItemText (AreaItem,pLine->sShowName);


        HTREEITEM LineItem=pTree->GetChildItem( AreaItem);
        while(LineItem)
        {
            pLine=(CLineProperty *)pTree->GetItemData(LineItem);
            if(m_bShowFullName)
                pTree->SetItemText (LineItem,pLine->sPathName );
            else
                pTree->SetItemText (LineItem,pLine->sShowName);

            LineItem=pTree->GetNextSiblingItem(LineItem);		
        }

        AreaItem=pTree->GetNextSiblingItem(AreaItem);
    }

}

void CFHTreeView::OnUpdateFHTreeShowFullName(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here
    pCmdUI->SetCheck (m_bShowFullName);	
}

HTREEITEM CFHTreeView::IsAreaInDataBase(CString sArea)
{
    CTreeCtrl *pTree=& GetTreeCtrl();
    if(!pTree)return NULL;

    if(!IsWindow(pTree->m_hWnd ))return NULL;

    HTREEITEM RootItem=pTree->GetRootItem ();
    HTREEITEM AreaItem=pTree->GetChildItem(RootItem);

    sArea.MakeUpper ();
    CString sAreaThis;
    while(AreaItem)
    {		
        sAreaThis=pTree->GetItemText (AreaItem);
        sAreaThis.MakeUpper();
        if(sArea==sAreaThis)return AreaItem;

        AreaItem=pTree->GetNextSiblingItem(AreaItem);
    }

    return NULL;		
}


HTREEITEM CFHTreeView::IsLineInDataBase(CString sArea, 
                                        CString sLine, 
                                        UINT nDataProperty)
{
    HTREEITEM AreaItem=IsAreaInDataBase(sArea);
    if(AreaItem==NULL)return NULL;

    sLine.MakeUpper ();
    sLine=::SeperateName(sLine);
    CString sLineThis;

    CTreeCtrl *pTree=& GetTreeCtrl();
    if(!pTree)return NULL;

    if(!IsWindow(pTree->m_hWnd ))return NULL;	
    HTREEITEM LineItem=pTree->GetChildItem(AreaItem);
    CLineProperty *pProperty=NULL;

    while(LineItem)
    {		
        pProperty=(CLineProperty*)pTree->GetItemData (LineItem);

        sLineThis=pProperty->sPathName;
        sLineThis=::SeperateName(sLineThis);
        sLineThis.MakeUpper();

        if(     sLineThis           ==sLine
            &&  pProperty->nProperty==nDataProperty)
        {
            return LineItem;
        }

        LineItem=pTree->GetNextSiblingItem(LineItem);
    }

    return NULL;		

}

bool CFHTreeView::RemoveLineInDataBase(CString sArea, 
                                       CString sLine, 
                                       UINT nDataProperty)
{
    HTREEITEM AreaItem=IsAreaInDataBase(sArea);
    if(AreaItem==NULL)return NULL;

    sLine.MakeUpper ();
    sLine=::SeperateName(sLine);
    CString sLineThis;

    CTreeCtrl *pTree=& GetTreeCtrl();
    if(!pTree)return NULL;

    if(!IsWindow(pTree->m_hWnd ))return NULL;	
    HTREEITEM LineItem=pTree->GetChildItem(AreaItem);
    CLineProperty *pProperty=NULL;

    while(LineItem)
    {		
        pProperty=(CLineProperty*)pTree->GetItemData (LineItem);

        sLineThis=pProperty->sPathName;
        sLineThis=::SeperateName(sLineThis);
        sLineThis.MakeUpper();

        if(     sLineThis           ==sLine
            &&  pProperty->nProperty==nDataProperty)
        {
            delete pProperty;
            pTree->DeleteItem (LineItem);
            return true;
        }

        LineItem=pTree->GetNextSiblingItem(LineItem);
    }

    return false;		

}


void CFHTreeView::OnFHTreeShowSortedName() 
{
    m_bShowSortedName=!m_bShowSortedName; 

    if(m_bShowSortedName)
    {
        SortTree();
    }
    else
    {
        CTreeCtrl *pTree=& GetTreeCtrl();
        if(!pTree)return ;

        if(!IsWindow(pTree->m_hWnd ))return ;
        HTREEITEM RootItem=pTree->GetRootItem ();			
        HTREEITEM LineItem;

        ////////////////////////////////////////////////
        // Firstly Set the order number into the tree
        HTREEITEM AreaItem=pTree->GetChildItem(RootItem);
        while(AreaItem)
        {		
            LineItem=pTree->GetChildItem (AreaItem);
            while(LineItem)
            {
                CLineProperty *pLine=(CLineProperty *)pTree->GetItemData (LineItem);
                if(pLine)
                {
                    pTree->SetItemText (LineItem,vtos(pLine->nOrder ));
                }				
                LineItem=pTree->GetNextSiblingItem(LineItem);
            }
            AreaItem=pTree->GetNextSiblingItem(AreaItem);
        }

        ///////////////////////////////////////////////
        // Then Sort the tree:
        SortTree();


        ////////////////////////////////////////////////
        // Then Set the line names into the tree
        AreaItem=pTree->GetChildItem(RootItem);
        while(AreaItem)
        {		
            LineItem=pTree->GetChildItem (AreaItem);
            while(LineItem)
            {
                CLineProperty *pLine=(CLineProperty *)pTree->GetItemData (LineItem);
                if(pLine)
                {
                    if(m_bShowFullName)
                        pTree->SetItemText (LineItem,pLine->sPathName);
                    else
                        pTree->SetItemText (LineItem,SeperateName(pLine->sPathName));
                }				
                LineItem=pTree->GetNextSiblingItem(LineItem);
            }
            AreaItem=pTree->GetNextSiblingItem(AreaItem);
        }
    }
}

void CFHTreeView::OnUpdateFHTreeShowSortedName(CCmdUI* pCmdUI) 
{
    pCmdUI->SetCheck (m_bShowSortedName);		
}

void CFHTreeView::OnFHTreeProperty() 
{
    // TODO: Add your command handler code here
    CString sArea=GetSelectedAreaName();

    CDlgTreeProperty dlg;
    dlg.m_nAreaNumber =GetAreaNumber();
    dlg.m_nTotalLineNumber=GetTotalLineNumber();

    if(sArea!="")
    {
        dlg.m_nLineNumber =GetLineNumberInArea(sArea);
        dlg.m_sAreaCurrent =sArea;
    }
    else
    {
        dlg.m_nLineNumber =0;
        dlg.m_sAreaCurrent ="Unselected";
    }	

    dlg.DoModal();

}


CString CFHTreeView::GetSelectedAreaName()
{
    HTREEITEM item=GetSelectedAreaItem();
    if(item==NULL)
    {
        return CString ("");
    }

    CTreeCtrl *pTreeCtrl = &GetTreeCtrl();
    if(!pTreeCtrl)return CString("");

    return pTreeCtrl->GetItemText (item);
}

void CFHTreeView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
    // TODO: Add your message handler code here and/or call default
    CString sArea=GetSelectedAreaName();
    CString sLine=GetSelectedLineName();

    if(sArea!=""&&sLine!="")
    {
        ::AddHistory(eOpenFile,GetDataBaseName()+":"+sLine);
        DrawLineData(sArea,sLine);
    }

    CFHTreeViewBase::OnLButtonDblClk(nFlags, point);
}

void CFHTreeView::SetLayerNames(CString sRootName,CString sFirstLayer,CString sSecondLayer)
{
    m_sRootName=sRootName;                   // 树根的描述
    m_sFirstLayerName=sFirstLayer;             // 第一层的名称
    m_sSecondLayerName=sSecondLayer;            // 第二层的名称

    int n=0;
    strcpy(m_menuItems[n].sItemName ,(LPCTSTR)("&N新建"+m_sRootName));
    m_menuItems[n].nID       = ID_FHTreeNewDataBase;
    n++;

    strcpy(m_menuItems[n].sItemName, (LPCTSTR)("&O打开"+m_sRootName));
    m_menuItems[n].nID       = ID_FHTreeOpenDataBase;
    n++;

    strcpy(m_menuItems[n].sItemName ,(LPCTSTR)( "&C关闭"+m_sRootName));
    m_menuItems[n].nID       = ID_FHTreeCloseDataBase;
    n++;

    strcpy(m_menuItems[n].sItemName,(LPCTSTR)("&I插入"+m_sFirstLayerName));
    m_menuItems[n].nID       = ID_FHTreeInsertArea;
    n++;

    strcpy(m_menuItems[n].sItemName ,(LPCTSTR)("&D删除"+m_sFirstLayerName));
    m_menuItems[n].nID       = ID_FHTreeRemoveArea;
    n++;

    strcpy(m_menuItems[n].sItemName ,(LPCTSTR)( "&W绘制"+m_sFirstLayerName+"图形"));
    m_menuItems[n].nID       = ID_FHTreeDrawAreaMap;
    n++;

    strcpy(m_menuItems[n].sItemName ,(LPCTSTR)( "&L插入"+m_sSecondLayerName));
    m_menuItems[n].nID       = ID_FHTreeInsertLine;
    n++;

    strcpy(m_menuItems[n].sItemName ,(LPCTSTR)( "&E删除"+m_sSecondLayerName));
    m_menuItems[n].nID       = ID_FHTreeRemoveLine;
    n++;

    strcpy(m_menuItems[n].sItemName ,(LPCTSTR)( "&M绘制"+m_sSecondLayerName+"图形"));
    m_menuItems[n].nID       = ID_FHTreeDrawLineMap;
    n++;

    strcpy(m_menuItems[n].sItemName ,(LPCTSTR)( "&T显示"+m_sSecondLayerName));
    m_menuItems[n].nID       = ID_FHTreeDrawLineData;
    n++;

    strcpy(m_menuItems[n].sItemName ,(LPCTSTR)( "&A建立新"+m_sSecondLayerName));
    m_menuItems[n].nID       = ID_FHTreeNewLineData;
    n++;

    strcpy(m_menuItems[n].sItemName ,(LPCTSTR)( "&R显示路径名"));
    m_menuItems[n].nID       = ID_FHTreeShowFullName;
    n++;

    strcpy(m_menuItems[n].sItemName ,(LPCTSTR)( "&S按名称排序显示"));
    m_menuItems[n].nID       = ID_FHTreeShowSortedName;
    n++;

    strcpy(m_menuItems[n].sItemName ,(LPCTSTR)( "&P"+m_sRootName+"属性"));
    m_menuItems[n].nID       = ID_FHTreeProperty;
    n++;

    strcpy(m_menuItems[n].sItemName ,(LPCTSTR)( "&A把当前项加入工区图"));
    m_menuItems[n].nID       = ID_FHAddCurrItemToBaseMap;
    n++;

    m_menuManager.SetPopupMenu();
    for(long i=0;i<n;i++)
    {
        UINT n=MF_MENUITEM|MF_STRING;
        UINT n1=MF_MENUITEM;
        UINT n2=MF_STRING;
        m_menuManager.AppendMenu(m_menuItems[i].nID ,m_menuItems[i].sItemName ,n);
    }

    return;
}

void CFHTreeView::OnFhtreeclosedatabase()
{
    RemoveTreeData();
    CTreeCtrl *pTree=&GetTreeCtrl();
    if(!pTree)return ;

    pTree->DeleteAllItems();
}

enumDimension CFHTreeView::GetDimension(CString sArea)
{
    long nLineNumber=0;
    CLineProperty *pLine=this->GetLineNamesInArea (sArea,nLineNumber);
    if(nLineNumber==0)
    {
        TRACEERROR(IDS_NoLineInArea,sArea);
        return Seismic_Error;
    }

    int n2D=0,n3D=0,nBoth=0,nError=0;
    enumDimension nDimension;
    for(long i=0;i<nLineNumber;i++)
    {
        nDimension=GetFileDimension(pLine[i].sPathName );
        switch(nDimension)
        {
        case Seismic_2D:
            n2D++;
            break;
        case Seismic_3D:
            n3D++;
            break;
        case Seismic_Both:
            nBoth++;
            break;
        case Seismic_Error:
            nError++;
            break;
        }
    }

    if(n2D>0&&n3D==0)
        return Seismic_2D;
    else if(n3D>0&&n2D==0)
        return Seismic_3D;
    else if(n2D>0&&n3D>0)
        return Seismic_Both;
    else if(n2D==0&&n3D==0)
        return Seismic_Error;
    else 
        return Seismic_Error;
}

enumDimension GetFileDimension(CString sFile)
{
    return Seismic_2D;
}

bool CFHTreeView:: Draw2DLineMap(CString sArea, CString sLine)
{
    return false;
}

bool CFHTreeView:: Draw3DLineMap(CString sArea, CString sLine)
{
    return false;
}

void CFHTreeView::OnInitialUpdate()
{
    __super::OnInitialUpdate();

    // TODO: Add your specialized code here and/or call the base class
}

CString CFHTreeView::CreateDataBase(CString sPath)
{
    CString sDataBase="";
    if(sPath=="")
    {
        return sDataBase;
    }

    Reset();	

    // Make directory:
    m_sDataBaseName=m_sProjectDiscription.Left(m_sProjectDiscription.Find ('('));
    m_sDataBasePath=sPath+"\\"+m_sDataBaseName;
    m_sDataBaseName+="."+GetDataBaseNameExt();

    if(_mkdir((LPCTSTR)m_sDataBasePath))
    {
        if(errno==EEXIST)
        {
            TRACEERROR("目录已经存在: "+m_sDataBasePath+",使用该目录!");
        }
        else if (errno==ENOENT)
        {
            AfxMessageBox("目录没有找到: "+sPath);
            return sDataBase;
        }
        else
        {
            AfxMessageBox("无法建立目录: "+m_sDataBasePath);
            return sDataBase;
        }
    }

    CTreeCtrl *pTreeCtrl=&GetTreeCtrl();
    if(!pTreeCtrl)return sDataBase;
    if(pTreeCtrl->m_hWnd ==NULL)return sDataBase;
    RemoveTreeData();
    pTreeCtrl->DeleteAllItems ();

    sDataBase=ConstructDataBaseName();

    CLineProperty *pLine=new CLineProperty ;
    pLine->sPathName=sDataBase;
    pLine->sShowName=SeperateName(sDataBase);

    HTREEITEM RootItem=0;
    if(!m_bShowFullName)
        RootItem= pTreeCtrl->InsertItem(pLine->sShowName);
    else
        RootItem= pTreeCtrl->InsertItem(pLine->sPathName);

    pTreeCtrl->SetItemState(RootItem, TVIS_BOLD, TVIS_BOLD );	
    pTreeCtrl->SetItemData(RootItem,(DWORD)pLine);


    if(m_pPointerToThis)
    {
        *m_pPointerToThis=this;
    }

    m_nInsertOrder=0;

    SaveDataBase();

    return sDataBase;

}

bool CFHTreeView::InsertArea(CString sArea)
{
    if(sArea=="")
    {
        TRACEERROR("错误：工区路径不能为空!");
        return false;
    }

    CTreeCtrl *pTreeCtrl=&GetTreeCtrl();
    if(!pTreeCtrl)return false;
    if(!IsWindow(pTreeCtrl->m_hWnd ))
    {
        return false;
    }

    HTREEITEM RootItem=pTreeCtrl->GetRootItem();
    if(!RootItem)
    {
        TRACEERROR("请先建立数据库!");
        return false;
    }

    if(IsAreaInDataBase(sArea))
    {
        TRACEERROR("工区"+sArea+"已经在数据库 "+m_sDataBaseName+"内");
        return false;
    }

    // Make the directory of the area:
    char cDir[500];
    if(!ConstructAreaPath(cDir,sArea))
    {
        AfxMessageBox("无法创建工区路径!");
        return false;
    }

    if(_mkdir(cDir))
    {
        if(errno==EEXIST)
        {
        }
        else if(errno==ENOENT)
        {
            AfxMessageBox("目录没有发现:"+CString(cDir));
            return false;
        }
        else
        {
            AfxMessageBox("建立子目录时候出现不可知的错误!");
            return false;
        }
    }	     

    //
    HTREEITEM AreaItem=pTreeCtrl->InsertItem (sArea,2,3,RootItem, TVI_LAST); 
    CLineProperty *pLine=new CLineProperty;
    pLine->sPathName=sArea;
    pLine->sShowName=sArea;
    pTreeCtrl->SetItemData(AreaItem,(DWORD)pLine);

    //
    pTreeCtrl->Expand(RootItem,TVE_EXPAND);

    //
    ::AddHistory(eAddArea,GetDataBaseName()+":"+sArea);

    //
    if(m_bShowSortedName)
    {
        SortTree();
    }

    // Insert the area in the area name data group and the tree:
    return SaveDataBase();
}

void CFHTreeView::OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
    // TODO: Add your control notification handler code here
    *pResult = 0;

    if(!pTVDispInfo->item.pszText)
    {
        return;
    }

    if(!m_bShowFullName)
    {
        CTreeCtrl &treeCtrl=GetTreeCtrl();
        CLineProperty *pLine=(CLineProperty *)(treeCtrl.GetItemData(pTVDispInfo->item.hItem));
        if(pLine)
        {
            pLine->sShowName=pTVDispInfo->item.pszText;
            treeCtrl.SetItemText(pTVDispInfo->item.hItem,pLine->sShowName);
        }
        SaveDataBase();
    }
}

void CFHTreeView::OnAddCurrentItemIntoBaseMap()
{   
    if(MultiDocViewContainsCurrentItem())
    {
        RemoveCurrentItemFromBaseMap();
    }
    else
    {
        AddCurrentItemIntoBaseMap(false);
    }
}

bool CFHTreeView::RemoveCurrentItemFromBaseMap()
{
    CStringList listLineNames;
    GetCurrentItemDocNames(listLineNames);

    CFHMainFrm *pFrm = dynamic_cast< CFHMainFrm* >(AfxGetApp()->GetMainWnd());
    if(!pFrm)
    {
        return false;
    }

    CDocument *pDocument  = pFrm->GetActiveMultiDocument();
    CMultiDocument *pMultiDoc = dynamic_cast< CMultiDocument* > (pDocument);
    if(!pMultiDoc)
    {
        return false;
    }

    pMultiDoc->UnReginserTemplates(listLineNames);

    return true;
}

bool CFHTreeView::AddCurrentItemIntoBaseMap(bool bForceAddArea)
{
    CFHMainFrm *pFrm = dynamic_cast< CFHMainFrm* >(AfxGetApp()->GetMainWnd());
    if(!pFrm)
    {
        return false;
    }

    if(!pFrm->GetActiveMultiDocument())
    {
        CreateMultiDocView();
    }

    CDocument *pDocument  = pFrm->GetActiveMultiDocument();
    CMultiDocument *pMultiDoc = dynamic_cast< CMultiDocument* > (pDocument);
    if(!pMultiDoc)
    {
        return false;
    }

    CString sArea,sLine;
    long i,nLineNumber;
    sArea   =   GetSelectedAreaName();
    if(!bForceAddArea)
    {
        sLine   =   GetSelectedLineName();
    }

    CLineProperty *pProperty=GetLineNamesInArea(sArea,nLineNumber);
    if(!pProperty)
    {
        AfxMessageBox("请先选定一个含有测线的工区!");
        return false;
    }

    CString sTitle;
    CMultiChildDoc *pDoc = NULL;
    pDoc = OnCreateDocForBaseView   (   sArea,  
                                        pProperty,
                                        nLineNumber);

    if(pDoc)
    {
        sTitle="";
        for(int i=0;i<nLineNumber;i++)
        {
            sTitle+="<"+SeperateFileName(pProperty[i].sPathName)+">";
        }

        pDoc->SetTitle(sTitle);

        CMultiChildView *pView = OnCreateViewForBaseView();
        pMultiDoc->RegisterTemplate(pDoc,pView);
    }
    else
    {
        if(sLine!="")
        {
            pDoc = OnCreateDocForBaseView(sLine);
            if(pDoc)
            {
                sTitle="<"+SeperateFileName(sLine)+">";
                pDoc->SetTitle(sTitle);

                CMultiChildView *pView = OnCreateViewForBaseView();
                pMultiDoc->RegisterTemplate(pDoc,pView);
            }
        }
        else
        {
            for(i=0;i<nLineNumber;i++)
            {
                pDoc = OnCreateDocForBaseView(pProperty[i].sPathName);
                if(!pDoc)
                {
                    continue;
                }

                sTitle="<"+SeperateFileName(pProperty[i].sPathName)+">";
                pDoc->SetTitle(sTitle);
                
                CMultiChildView *pView = OnCreateViewForBaseView();
                pMultiDoc->RegisterTemplate(pDoc,pView);
            }
        }
    }

    return true;
}

CMultiChildDoc * CFHTreeView::OnCreateDocForBaseView    (   CString         sFileName)
{
    CMultiChildDoc * pDoc = CreateDocForBaseView(sFileName);
    if(pDoc)
    {
        pDoc->SetIsMultiChildDoc(true);
    }
    return pDoc;
}

CMultiChildDoc * CFHTreeView::OnCreateDocForBaseView    (   CString         sArea, 
                                                            CLineProperty * p,
                                                            int             nLineNumber)
{
    CMultiChildDoc * pDoc = CreateDocForBaseView(sArea,p, nLineNumber);
    if(pDoc)
    {
        pDoc->SetIsMultiChildDoc(true);
    }

    return pDoc;
}

CMultiChildView *  CFHTreeView::OnCreateViewForBaseView   ()
{
    CMultiChildView *pView = CreateViewForBaseView();
    return pView;
}

CMultiChildDoc * CFHTreeView::CreateDocForBaseView      (  CString         sFileName)
{
    return NULL;
}

CMultiChildDoc * CFHTreeView::CreateDocForBaseView      (   CString         sArea, 
                                                            CLineProperty * p,
                                                            int             nLineNumber)
{
    return NULL;
}

CMultiChildView *  CFHTreeView::CreateViewForBaseView   ()
{
    return NULL;
}