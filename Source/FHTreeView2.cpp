
#include "stdafx.h"
#include "FHTreeView.h"
#include "DlgAskOneValue.h"
#include <direct.h>
#include <errno.h>
#include "DlgTreeProperty.h"
#include "FHZoomViewGlobal.h"
#include "ZoomView.h"
#include "FHMainFrm.h"
#include "textdoc.h"
#include "textview.h"
#include "MultiDocument.h"
#include "MultiView.h"
#include "MultiChildView.h"


extern "C" const long  MACHINE_NUMBER[];

const long MACHINE_NUMBER[nMachineQuantity]=
{
    317199177,
    0x3c7675e9,
    0x99999999,
    614041972
};


HTREEITEM CFHTreeView::GetSelectedAreaItem()
{
    // Check if the area name has been in this database:
    CTreeCtrl *pTreeCtrl = & GetTreeCtrl();
    if(!pTreeCtrl )return NULL;

    if(!IsWindow(pTreeCtrl->m_hWnd ))return NULL;

    HTREEITEM RootItem=pTreeCtrl->GetRootItem ();
    if(!RootItem)
    {
        return 0;
    }

    HTREEITEM ItemSelected=pTreeCtrl->GetSelectedItem ();
    HTREEITEM ItemParent=pTreeCtrl->GetParentItem (ItemSelected);

    // The selected is the root item:
    if(ItemParent==NULL)
    {
        return NULL;
    }

    // The selected is an area item:
    else if(ItemParent==RootItem)
    {
        return ItemSelected;
    }

    // The selected is an lineitem:
    else 
    {
        return ItemParent;
    }
}


CString* CFHTreeView::GetAreaNames(long &nAreaNumber)
{
    CTreeCtrl *pTree=& GetTreeCtrl();
    if(!pTree)return NULL;

    if(!IsWindow(pTree->m_hWnd ))return NULL;

    HTREEITEM RootItem=pTree->GetRootItem ();
    if(!RootItem)
    {
        return NULL;
    }

    HTREEITEM AreaItem=pTree->GetChildItem( RootItem);

    nAreaNumber=GetAreaNumber();
    if(m_pAreaNames)
    {
        delete []m_pAreaNames;
        m_pAreaNames=NULL;
    }
    m_pAreaNames=new CString[nAreaNumber+10];

    CString sAreaInTree;
    long i=0;
    while(AreaItem)
    {
        sAreaInTree=pTree->GetItemText (AreaItem);
        m_pAreaNames[i]=sAreaInTree;	

        AreaItem=pTree->GetNextSiblingItem(AreaItem);
        i++;
    }

    return m_pAreaNames;
}

CString CFHTreeView::GetFileName(CString sArea, CString sLine)
{
    return SearchLineData(sArea,sLine,0);
}


CLineProperty * CFHTreeView::GetLineNamesInArea(CString sArea,long &nLineNumber)
{
    HTREEITEM AreaItem=IsAreaInDataBase(sArea);
    if(!AreaItem)return NULL;

    nLineNumber=GetLineNumberInArea(sArea);
    if(nLineNumber==0)return NULL;

    if(m_pLinePropertyInArea)
    {
        delete []m_pLinePropertyInArea;
        m_pLinePropertyInArea=NULL;
    }
    m_pLinePropertyInArea=new CLineProperty[nLineNumber+10];

    CTreeCtrl *pTree=& GetTreeCtrl();
    if(!pTree)return NULL;

    if(!IsWindow(pTree->m_hWnd ))return NULL;

    HTREEITEM LineItem=pTree->GetChildItem( AreaItem);
    CLineProperty *pTempProperty=NULL;

    CLineProperty *pLine=NULL;
    CString sLine;
    long i=0;
    while(LineItem)
    {
        pTempProperty=(CLineProperty*)pTree->GetItemData(LineItem);
        if(!pTempProperty)break;

        sLine=SearchLineData(sArea,pTempProperty->sPathName,pTempProperty->nProperty);	

        if(sLine!="")
        {
            m_pLinePropertyInArea[i]=pTempProperty;
            m_pLinePropertyInArea[i].sPathName=sLine;
            i++;
        }		
        LineItem=pTree->GetNextSiblingItem(LineItem);		
    }
    nLineNumber=i;

    return m_pLinePropertyInArea;
}

HTREEITEM CFHTreeView::GetSelectedLineItem()
{
    CTreeCtrl *pTreeCtrl =& GetTreeCtrl();
    if(!pTreeCtrl)return NULL;

    if(!IsWindow(pTreeCtrl->m_hWnd ))return NULL;
    CString sArea=GetSelectedAreaName();
    if(sArea=="")return NULL;

    HTREEITEM RootItem=pTreeCtrl->GetRootItem ();
    if(!RootItem)
    {
        return 0;
    }
    HTREEITEM ItemSelected=pTreeCtrl->GetSelectedItem ();
    HTREEITEM ItemParent=pTreeCtrl->GetParentItem (ItemSelected);

    // The selected is the root item:
    CString sLineName;
    if(ItemParent==NULL)
    {
        AfxMessageBox("错误：你必须首先选定一个!"+m_sFirstLayerName);
        return NULL;
    }

    // The selected is an area item:
    else if(ItemParent==RootItem)
    {
        return NULL;
    }

    // The selected is an lineitem:
    else 
    {
        return ItemSelected;
    }	
}

void CFHTreeView::DealSelChanged()
{
    CString sAreaLast=m_sAreaCurrent;
    CString sLineLast=m_sLineCurrent;
    m_sAreaCurrent=GetSelectedAreaName();
    m_sLineCurrent=GetSelectedLineName();

    if(m_sAreaCurrent=="")return;

    if(m_sAreaCurrent!=sAreaLast)
    {
        long nLineNumber;
        CLineProperty *pLineProperty=GetLineNamesInArea(m_sAreaCurrent,nLineNumber);
        UpdateDrawingArea(m_sAreaCurrent,pLineProperty,nLineNumber,m_sLineCurrent);
    }
    else if(m_sLineCurrent!=sLineLast)
    {
        UpdateDrawingLine(m_sAreaCurrent,m_sLineCurrent);
    }
}

long CFHTreeView::GetAreaNumber()
{
    CTreeCtrl *pTreeCtrl = & GetTreeCtrl();
    if(!pTreeCtrl)return 0;
    if(!IsWindow(pTreeCtrl->m_hWnd ))return NULL;

    HTREEITEM RootItem=pTreeCtrl->GetRootItem ();
    if(!RootItem)
    {
        return 0;
    }
    HTREEITEM AreaItem=pTreeCtrl->GetChildItem(RootItem);

    long nAreaNumber=0;
    while(AreaItem)
    {		
        AreaItem=pTreeCtrl->GetNextSiblingItem(AreaItem);
        nAreaNumber++;
    }

    return nAreaNumber;
}

long CFHTreeView::GetLineNumberInArea(CString sArea)
{
    CTreeCtrl *pTreeCtrl = & GetTreeCtrl();
    if(!pTreeCtrl )return 0;
    if(!IsWindow(pTreeCtrl ->m_hWnd ))return NULL;

    HTREEITEM RootItem=pTreeCtrl ->GetRootItem ();
    if(!RootItem)
    {
        return 0;
    }
    HTREEITEM AreaItem=pTreeCtrl ->GetChildItem(RootItem);

    sArea.MakeUpper();
    CString sAreaInTree;
    long nAreaNumber=0;
    while(AreaItem)
    {		
        sAreaInTree=pTreeCtrl ->GetItemText (AreaItem);
        sAreaInTree.MakeUpper();

        if(sArea== sAreaInTree)return GetLineNumberInArea(AreaItem);
        AreaItem=pTreeCtrl ->GetNextSiblingItem(AreaItem);
    }

    return 0;
}

long CFHTreeView::GetLineNumberInArea(HTREEITEM ItemArea)
{
    CTreeCtrl *pTreeCtrl = & GetTreeCtrl();
    if(!IsWindow(pTreeCtrl->m_hWnd ))return NULL;

    HTREEITEM ItemLine=pTreeCtrl->GetChildItem(ItemArea);

    long nLineNumber=0;
    while(ItemLine){		
        ItemLine=pTreeCtrl->GetNextSiblingItem(ItemLine);
        nLineNumber++;
    }

    return nLineNumber;

}

long CFHTreeView::GetTotalLineNumber()
{
    CTreeCtrl *pTreeCtrl = & GetTreeCtrl();
    if(!pTreeCtrl )return 0;
    if(!IsWindow(pTreeCtrl->m_hWnd ))return NULL;

    HTREEITEM RootItem=pTreeCtrl->GetRootItem ();
    if(!RootItem)
    {
        return 0;
    }

    HTREEITEM AreaItem=pTreeCtrl->GetChildItem(RootItem);

    long nLineNumber=0;
    while(AreaItem){		
        nLineNumber+=GetLineNumberInArea(AreaItem);
        AreaItem=pTreeCtrl->GetNextSiblingItem(AreaItem);
    }

    return nLineNumber;

}

long CFHTreeView::GetSelectedLineProperty()
{
    HTREEITEM item=GetSelectedLineItem();
    if(!item)return 0;

    CTreeCtrl *pTreeCtrl = &GetTreeCtrl();
    if(!pTreeCtrl )return 0;

    return pTreeCtrl->GetItemData (item);
}

long CFHTreeView::GetDataProperty(CString sArea, CString sLine)
{	
    CTreeCtrl *pTreeCtrl = & GetTreeCtrl();
    if(!pTreeCtrl)return 0;

    HTREEITEM RootItem=pTreeCtrl->GetRootItem ();
    if(!RootItem)
    {
        return 0;
    }
    HTREEITEM AreaItem=pTreeCtrl->GetChildItem (RootItem);

    sArea.MakeUpper ();
    sLine.MakeUpper ();
    sArea=SeperateFileName(sArea);
    sLine=SeperateFileName(sLine);

    CString sAreaName,sLineName;
    while(AreaItem)
    {
        sAreaName=pTreeCtrl->GetItemText (AreaItem);
        sAreaName.MakeUpper ();
        if(sArea==sAreaName)
        {
            HTREEITEM LineItem=pTreeCtrl->GetChildItem (AreaItem);
            while(LineItem)
            {
                CLineProperty* pLine=(CLineProperty*)pTreeCtrl->GetItemData (LineItem);
                sLineName=SeperateFileName(pLine->sPathName );
                sLineName.MakeUpper ();

                if(sLine==sLineName)
                {
                    return pLine->nProperty ;
                }
                LineItem=pTreeCtrl->GetNextSiblingItem (LineItem);
            }
        }
        AreaItem=pTreeCtrl->GetNextSiblingItem (AreaItem);
    }

    return 0;

}

CString CFHTreeView::ExistLineTypeInArea(CString sArea,long nDataType)
{
    CTreeCtrl *pTreeCtrl = & GetTreeCtrl();
    if(!pTreeCtrl )return CString("");

    HTREEITEM RootItem=pTreeCtrl->GetRootItem ();
    if(!RootItem)
    {
        return CString("");
    }

    HTREEITEM AreaItem=pTreeCtrl->GetChildItem (RootItem);
    sArea.MakeUpper ();

    CString sAreaName;
    long n;
    while(AreaItem)
    {
        sAreaName=pTreeCtrl->GetItemText (AreaItem);
        sAreaName.MakeUpper ();
        if(sArea==sAreaName)
        {
            HTREEITEM LineItem=pTreeCtrl->GetChildItem (AreaItem);
            while(LineItem)
            {
                n=pTreeCtrl->GetItemData(LineItem);
                if(n==nDataType)
                {
                    return pTreeCtrl->GetItemText(LineItem);
                }
                LineItem=pTreeCtrl->GetNextSiblingItem (LineItem);
            }
        }
        AreaItem=pTreeCtrl->GetNextSiblingItem (AreaItem);
    }

    return CString ("");
}

bool CFHTreeView::InsertLine(CString sArea,
                             CString sFile, 
                             int nDataProperty, 
                             int nImageOrder, 
                             int nSelectedImageOrder,
                             int nInsertOrder)
{
    HTREEITEM itemArea=IsAreaInDataBase(sArea);
    if(!itemArea)
    {
        InsertArea(sArea);
        itemArea=IsAreaInDataBase(sArea);
        if(!itemArea)
        {
            return false;
        }
    }

    //
    if(IsLineInDataBase(sArea,sFile,nDataProperty))
    {
        if(AfxMessageBox(m_sSecondLayerName+sFile+" 已经在数据库中,覆盖吗?",MB_YESNO)==IDNO)
        {
            return true;
        }
        else
        {
            while(RemoveLineInDataBase(sArea,sFile,nDataProperty));
        }
    }

    //
    if(InsertLine(itemArea,sFile,nDataProperty,nImageOrder,nSelectedImageOrder,nInsertOrder))
    {
        return SaveDataBase();
    }
    else
    {
        return false;
    }
}

//////////////////////////////////////////////////////////
// sFile is used only for returning the inserted lines:
bool CFHTreeView::InsertLine(CString sFile)
{
    ////////////////////////////////////////////
    // Check the machine:
    /*DWORD nVolumeSerialNumber=9999;  // address of volume serial number
    if(!GetVolumeInformation(
    NULL,        // address of root directory of the 
    // file system
    NULL,     // address of name of the volume
    0,         // length of lpVolumeNameBuffer
    &nVolumeSerialNumber,  // address of volume serial number
    NULL,
    // address of system's maximum 
    // filename length
    NULL,     // address of file system flags
    NULL, // address of name of file system
    0// length of lpFileSystemNameBuffer
    ))
    {
    return false;
    }
    else
    {	
    bool bValid=false;
    int n;
    for(n=0;n<nMachineQuantity;n++)
    {
    if(nVolumeSerialNumber==MACHINE_NUMBER[n])
    {
    bValid=true;
    break;
    }
    }

    if(!bValid)return false;

    }
    */
    //
    //////////////////////////////////////////////////

    /////////////////////////////////
    //
    CString sArea=GetSelectedAreaName();
    if(sArea=="")return false;

    ///////////////////////////////////////////////
    // Open the file for returning:
    FILE *fp=fopen(sFile,"wt");
    if(!fp)
    {
        AfxMessageBox("错误：无法建立文件:"+sFile);
        return false;
    }

    sArea.MakeUpper ();
    fprintf(fp,"%s\n",(LPCTSTR)sArea); // for returning the inserted lines-- will be used  by CTreeSurvey;

    CFileDialog dlg(true);
    char cFilter[100];
    ConstructFilterString(cFilter,m_sDataDiscription,m_sDataExt);

    char sTitle[200];
    sprintf(sTitle,"选择你要插入的%s:",(LPCTSTR)m_sSecondLayerName);
    char sExt[200];
    sprintf(sExt,"%s",(LPCTSTR)m_sDataExt);

    dlg.m_ofn.lpstrFilter=cFilter;
    dlg.m_ofn.lpstrTitle=sTitle;
    dlg.m_ofn.lpstrDefExt=sExt;
    dlg.m_ofn.Flags |=OFN_ALLOWMULTISELECT ;

    char buffer[20000];
    memset(buffer,0,20000);
    dlg.m_ofn.lpstrFile =buffer;
    dlg.m_ofn.nMaxFile =20000;

    if(dlg.DoModal()==IDCANCEL)return false;
    CString sData=dlg.GetPathName();		

    /////////////////////////////////////////
    // Write the line names into a temp file:
    CString sLineName,sLineShowName;

    long nDataProperty=LINE_COMMON;
    long nImageOrder=0;
    long nSelectedImageOrder=0;

    POSITION pos=dlg.GetStartPosition ();
    while(pos)
    {
        sLineName=dlg.GetNextPathName(pos);

        ::AddHistory(eAddLine,GetDataBaseName()+":"+sLineName);

        sLineShowName=::SeperateName(sLineName);
        fprintf(fp,"%s\n",sLineName);
        fprintf(fp,"%s\n",sLineShowName);
        fprintf(fp,"%ld %ld %ld\n",nDataProperty,nImageOrder,nSelectedImageOrder);
    }
    fclose(fp); 

    ///////////////////////////////////////
    // Insert the line names into the tree:
    return InsertLineNamesFromFile(sFile);
}

bool CFHTreeView::InsertLineNamesFromFile(CString sFile)
{
    ///////////////////////////////////////////////
    // get the area item:
    if(sFile=="")return false;

    CTreeCtrl *pTreeCtrl=&GetTreeCtrl();
    if(!pTreeCtrl)return false;

    HTREEITEM AreaItem=GetSelectedAreaItem();	
    if(!AreaItem)return false;
    CString sArea=pTreeCtrl->GetItemText (AreaItem);

    HTREEITEM AreaItemBack=IsAreaInDataBase(sArea);
    if(!AreaItemBack)
    {
        AfxMessageBox("Error: the backup tree has not area:"+sArea);
        return false;
    }

    FILE *fp=fopen(sFile,"rt");
    if(!fp)return false;

    long nDataProperty=0;
    long nImageOrder=0;
    long nSelectedImageOrder=0,i=0;
    CString sLineName,sLineShowName;
    char sLine[200];
    long nInsertedLine=0;

    fscanf(fp,"%s\n",sLine); // The area name;

    while(!feof(fp))
    {
        nDataProperty=0;
        nImageOrder=0;
        nSelectedImageOrder=0;

        // line path name:
        memset(sLine,0,200);
        if(!fgets(sLine,200,fp))break;
        ::RemoveInvalidChar(sLine);
        sLineName=sLine;
        sLineName.Trim();

        // line show name:
        memset(sLine,0,200);
        if(!fgets(sLine,200,fp))break;
        ::RemoveInvalidChar(sLine);
        sLineShowName=sLine;  
        sLineShowName.Trim();

        //
        fscanf(fp,  "%ld %ld %ld\n",
            &nDataProperty, 
            &nImageOrder,
            &nSelectedImageOrder); 

        InsertLine( sArea,
            sLineName,
            nDataProperty,
            nImageOrder,
            nSelectedImageOrder,
            m_nInsertOrder);

        nInsertedLine++;
        m_nInsertOrder++;
    }

    fclose(fp);

    ////////////////////////////////////////////////
    // 可能所有测线都已经在数据库中,不必再加入了:
    if(nInsertedLine<1)
    {
        return false;
    }
    else
    {
        pTreeCtrl->Expand(AreaItem,TVE_EXPAND);	

        if(m_bShowSortedName)
        {
            SortTree();
        }

        return SaveDataBase();
    }
}

bool CFHTreeView::InsertLine(   HTREEITEM   AreaItem,
                             CString     sLineName,
                             int         nDataProperty,
                             int         nImageOrder,
                             int         nSelectedImageOrder,
                             int         nInsertOrder)
{
    CTreeCtrl *pTreeCtrl=&GetTreeCtrl();
    if(!pTreeCtrl)return false;

    CString sLineShowName=SeperateName(sLineName);

    CString sLineItemName=sLineName;
    if(!m_bShowFullName)
    {
        sLineItemName=sLineShowName;
    }

    if(nImageOrder==0)nImageOrder=1;
    if(nSelectedImageOrder==0)nSelectedImageOrder=1;

    HTREEITEM LineItem=pTreeCtrl->InsertItem (  sLineItemName,
        nImageOrder,
        nSelectedImageOrder,				
        AreaItem, 
        TVI_LAST); 
    if(LineItem)
    {
        CLineProperty *pLine    = new CLineProperty;
        pLine->nOrder           =   m_nInsertOrder;
        pLine->nProperty        =   nDataProperty;
        pLine->nImageOrder      =   nImageOrder ;
        pLine->nSelectedImageOrder =    nSelectedImageOrder ;
        pLine->sPathName        =   sLineName;
        pLine->sShowName        =   sLineShowName;
        pTreeCtrl->SetItemData (LineItem,(DWORD)pLine);

        return true;
    }

    return false;
}


//
//bool CFHTreeView::SaveData()
//{
//	long i,j,nAreaNumber,nLineNumber;
//
//	CString *pAreaNames=GetAreaNames(nAreaNumber);
//	if(!pAreaNames)return false;
//
//	CString sFile=ConstructDataBaseName();
//	FILE *fp=fopen(sFile,"wt");
//	if(!fp)
//	{
//		AfxMessageBox("Can not write to "+sFile+" !");
//		return false;
//	}
//	fprintf(fp,"%s\n",(LPCTSTR)m_sDataBaseMark);
//
//	CLineProperty *pLineProperty=NULL;
//	for(i=0;i<nAreaNumber;i++)
//	{
//		//////////////////////////////////////
//		// Write the lines in the area:
//		fprintf(fp,"%s\n",pAreaNames[i]);
//
//		pLineProperty=GetLineNamesInArea(pAreaNames[i],nLineNumber);
//		fprintf(fp,"%ld\n",nLineNumber); 
//		for(j=0;j<nLineNumber;j++)
//		{
//			fprintf(fp,"%s %ld %u %u\n",
//				pLineProperty[j].sPathName ,
//				pLineProperty[j].nProperty,
//				pLineProperty[j].nImageOrder ,
//				pLineProperty[j].nSelectedImageOrder);
//		}
//	}
//	
//	fclose(fp);
//
//	return true;
//}
//
//
bool CFHTreeView::OpenDataBase()
{
    // TODO: Add your command handler code here
    CString sPathName=GetFileForOpen(m_sProjectDiscription,m_sProjectExt);
    return OpenDataBase(sPathName);
}

bool CFHTreeView::OpenDataBase(CString sPathName, bool bUpdateSolution)
{	
    Reset();

    m_sDataBasePath=SeperatePath(sPathName);
    m_sDataBaseName=SeperateFileName(sPathName);

    if(!ReadDataBase())
    {
        Reset();
        return false;
    }

    if(m_pPointerToThis)
    {
        *m_pPointerToThis=this;
    }

    //
    CFHMainFrm *pFrame=(CFHMainFrm*)AfxGetMainWnd();
    if(bUpdateSolution  &&  pFrame  && pFrame->IsKindOf(RUNTIME_CLASS(CFHMainFrm)))
    {
        pFrame->UpdateDBSolution(this);
        return true;
    }
    else
    {
        return false;
    }
}


CString CFHTreeView::RemoveArea()
{
    HTREEITEM item=GetSelectedAreaItem();
    if(!item)
    {
        AfxMessageBox("错误：你必须首先选定一个"+m_sFirstLayerName+"!");
        return CString ("");
    }
    else if(AfxMessageBox("警告：你是否确定要删除整个"+m_sFirstLayerName+"?",MB_YESNO)==IDNO)
    {
        return CString ("");
    }

    CTreeCtrl *pTree=&GetTreeCtrl();
    if(!pTree)return CString("");

    CString sArea=pTree->GetItemText (item);	

    CFHMainFrm *pFrame=(CFHMainFrm*)AfxGetMainWnd();
    pFrame->RemoveArea(sArea);

    return sArea;
}

bool CFHTreeView::RemoveArea	(CString sArea)
{
    CTreeCtrl *pTree=&GetTreeCtrl();
    if(!pTree)return false;

    /////////////////////////////////
    // Search the tree for the item of the Area name:
    /////////////////////////////////
    sArea.MakeUpper();

    HTREEITEM itemRoot=pTree->GetRootItem();
    if(!itemRoot)
    {
        return true;
    }
    HTREEITEM lineArea=pTree->GetChildItem (itemRoot);
    CString sAreaNameTree;
    HTREEITEM item=NULL;
    while(lineArea)
    {
        sAreaNameTree=pTree->GetItemText (lineArea);
        sAreaNameTree.MakeUpper();

        if(sAreaNameTree==sArea)
        {
            item=lineArea;
            break;
        }

        lineArea=pTree->GetNextSiblingItem(lineArea);		
    }

    if(item==NULL)
    {
        return false;
    }

    /////////////////////////////////
    // Delete the data of the area:
    /////////////////////////////////
    CLineProperty *p=(CLineProperty *)(pTree->GetItemData (item));
    if(p)
    {
        delete p;
    }


    /////////////////////////////////
    // Delete the data under the area:
    HTREEITEM lineItem=pTree->GetChildItem (item);
    while(lineItem)
    {
        CLineProperty *p=(CLineProperty *)(pTree->GetItemData (lineItem));
        if(p)
        {
            delete p;
        }
        lineItem=pTree->GetNextSiblingItem(lineItem);		
    }

    /////////////////////////////////////////
    // Delete the item:
    pTree->DeleteItem (item);
    SaveDataBase();

    ::AddHistory(eRemoveArea, GetDataBaseName()+":"+sArea);


    return true;
}


bool CFHTreeView::RemoveLine(CString &sArea,CString &sLine)
{
    CTreeCtrl *pTreeCtrl = & GetTreeCtrl();
    if(!pTreeCtrl )return false;
    if(!IsWindow(pTreeCtrl->m_hWnd ))false;

    HTREEITEM ItemLine=GetSelectedLineItem();
    if(!ItemLine)return false;

    sArea=GetSelectedAreaName();
    sLine=pTreeCtrl->GetItemText (ItemLine);

    CLineProperty *p=(CLineProperty *)(pTreeCtrl->GetItemData (ItemLine));
    if(p)
    {
        delete p;
    }	

    pTreeCtrl->DeleteItem (ItemLine);

    SaveDataBase();

    ::AddHistory(eRemoveLine, GetDataBaseName()+":"+sLine);

    return true;
}

CString CFHTreeView::GetMultiDocTemplateName()
{
    return "MultiDocView";
}


bool CFHTreeView::CreateMultiDocView()
{
    CFHMainFrm *pFrm = dynamic_cast< CFHMainFrm* >(AfxGetApp()->GetMainWnd());
    if(!pFrm)
    {
        return false;
    }

    CString sMDTName = GetMultiDocTemplateName();
    CMultiDocTemplate *pTemplate=GetTemplate(sMDTName);
    if(!pTemplate)return false;

    CMultiDocument *pDoc =  (CMultiDocument *)pTemplate->OpenDocumentFile(NULL);
    pFrm->AddMultiDocument(pDoc);
    
    return true;
}

bool CFHTreeView::DrawAreaMap(CString sArea,CLineProperty *pLines,long nLineNumber)
{
    CreateMultiDocView();

    AddCurrentItemIntoBaseMap(true);

    return true;
}

bool CFHTreeView::DrawLineMap(CString sArea,CString sLine)
{
    return true;
}

bool CFHTreeView::DrawLineData(CString sArea,CString sFile)
{
    if(m_bTextFile)
    {
        CMultiDocTemplate *pTemplate=GetTemplate("TextEdit");
        if(!pTemplate)return false;

        return pTemplate->OpenDocumentFile(sFile);
    }
    else
    {
        return DrawLineMap(sArea, sFile);
    }
}

bool CFHTreeView::UpdateDrawingArea(CString sArea,CLineProperty *pLineProperty,long nLineNumber,CString sLineCurrent)
{
    return true;
}

bool CFHTreeView::UpdateDrawingLine(CString sArea,CString sLineCurrent)
{
    return true;
}


bool CFHTreeView::MakeNewLineData()
{
    return true;
}


CString CFHTreeView::InsertArea()
{
    ///////////////////////////////////
    // Insert an area name:
    // Insert the area into the datagroup:
    CDlgAskOneValue dlg;
    dlg.SetTitle(m_sFirstLayerName+"名称");
    dlg.SetAsk(m_sFirstLayerName+"名称");
    dlg.SetValue("未知"+m_sFirstLayerName);
    if(dlg.DoModal()==IDCANCEL)
    {
        return CString("");
    }

    CString sArea=dlg.GetValue();
    sArea.MakeUpper();

    // Check if the area name has been in this database:  
    if(IsAreaInDataBase(sArea))
    {
        AfxMessageBox("错误:该"+m_sFirstLayerName+"名称已经存在,不能重复加入!");
        return CString("");
    }


    CFHMainFrm *pMainFrm=(CFHMainFrm *)AfxGetMainWnd();
    pMainFrm->InsertArea(sArea);

    return sArea;

}

bool CFHTreeView::MakeNewDataBase() 
{
    Reset();	

    // Make directory:
    char sTitle[100];
    sprintf(sTitle,"给出新的%s名称(*.%s)",m_sRootName,m_sProjectExt);
    m_sDataBaseName=::GetFileForSave(sTitle,m_sProjectExt);
    if(m_sDataBaseName=="")
    {
        return false;
    }

    int n=m_sDataBaseName.ReverseFind ('.');
    if(n!=-1)
    {
        m_sDataBasePath=m_sDataBaseName.Left(n);
    }
    m_sDataBaseName=::SeperateFileName(m_sDataBaseName);

    if(_mkdir((LPCTSTR)m_sDataBasePath))
    {
        if(errno==EEXIST)
            AfxMessageBox("目录已经存在: "+m_sDataBasePath);
        else if (errno==ENOENT)
            AfxMessageBox("目录没有找到: "+m_sDataBasePath);
        else 			
            AfxMessageBox("无法建立目录: "+m_sDataBasePath);
        return false;
    }


    // Set current  database to the new databse:	
    CTreeCtrl *pTreeCtrl=&GetTreeCtrl();
    if(!pTreeCtrl||!pTreeCtrl->m_hWnd)
    {
        return false;
    }
    RemoveTreeData();
    pTreeCtrl->DeleteAllItems ();

    CString sDataBase=ConstructDataBaseName();

    //
    CLineProperty *pLine=new CLineProperty ;
    pLine->sPathName=sDataBase;
    pLine->sShowName=SeperateName(sDataBase);

    HTREEITEM RootItem=NULL;
    if(m_bShowFullName)
        RootItem= pTreeCtrl->InsertItem(pLine->sPathName);
    else
        RootItem= pTreeCtrl->InsertItem(pLine->sShowName);

    //
    pTreeCtrl->SetItemState(RootItem, TVIS_BOLD, TVIS_BOLD );	
    pTreeCtrl->SetItemData(RootItem,(DWORD)pLine);

    //
    if(m_pPointerToThis)
    {
        *m_pPointerToThis=this;
    }

    //
    m_nInsertOrder=0;
    SaveDataBase();

    //
    ((CFHMainFrm*)AfxGetMainWnd())->UpdateDBSolution(this);


    return true;
}

int CFHTreeView::GetChildQty(HTREEITEM item)
{
    CTreeCtrl *pTreeCtrl = & GetTreeCtrl();
    if(!IsWindow(pTreeCtrl->m_hWnd ))
    {
        return 0;
    }

    int nChildQty=0;
    HTREEITEM itemChild=pTreeCtrl->GetChildItem(item);
    while(itemChild)
    {
        nChildQty++;
        itemChild=pTreeCtrl->GetNextSiblingItem(itemChild);
    }

    return nChildQty;
}

bool CFHTreeView::SaveDataBase()
{
    if(m_sDataBasePath==""||m_sDataBaseName=="")return false;

    //
    CTreeCtrl *pTreeCtrl = & GetTreeCtrl();
    if(!IsWindow(pTreeCtrl->m_hWnd ))return false;

    CString sFile=ConstructDataBaseName();
    FILE *fp=fopen(sFile,"wt");
    if(!fp)
    {
        Reset();
        return false;
    }

    fprintf(fp,"%s\n",m_sDataBaseMark);

    //
    HTREEITEM item=pTreeCtrl->GetRootItem();
    if(!item)
    {
        fclose(fp);
        return true;
    }

    CLineProperty *pLine=(CLineProperty *)(pTreeCtrl->GetItemData(item));
    if(!pLine)
    {
        fclose(fp);
        return false;
    }
    fprintf(fp,"%s\n",pLine->sPathName);
    fprintf(fp,"%s\n",pLine->sShowName);

    // Write the area quantity:
    int nArea=GetChildQty(item);
    fprintf(fp,"%d\n",nArea);

    int nLine=0;

    //
    HTREEITEM itemLine;
    HTREEITEM itemArea=pTreeCtrl->GetChildItem(item);
    while(itemArea)
    {
        pLine=(CLineProperty*)(pTreeCtrl->GetItemData(itemArea));
        fprintf(fp,"%s\n",pLine->sPathName);
        fprintf(fp,"%s\n",pLine->sShowName);

        // Write the line Quantity:
        nLine=GetChildQty(itemArea);
        fprintf(fp,"%d\n",nLine);

        // Write the line names:
        itemLine=pTreeCtrl->GetChildItem(itemArea);
        while(itemLine)
        {
            pLine=(CLineProperty*)(pTreeCtrl->GetItemData(itemLine));
            fprintf(fp,"%s\n %s\n  %ld %u %u %d\n",
                pLine->sPathName,
                pLine->sShowName,
                pLine->nProperty,
                pLine->nImageOrder,
                pLine->nSelectedImageOrder,
                pLine->nIs2DLine);

            itemLine=pTreeCtrl->GetNextSiblingItem(itemLine);
        }
        itemArea=pTreeCtrl->GetNextSiblingItem(itemArea);
    }

    fclose(fp);

    return true;
}

void CFHTreeView::ClearTree()
{
    RemoveTreeData();
    CTreeCtrl *pTreeCtrl = & GetTreeCtrl();
    if(IsWindow(pTreeCtrl->m_hWnd ))
    {
        pTreeCtrl->DeleteAllItems ();
    }
}

bool CFHTreeView::ReadDataBase()
{
    if(m_sDataBasePath==""||m_sDataBaseName=="")return false;

    //
    CTreeCtrl *pTreeCtrl = & GetTreeCtrl();
    if(!IsWindow(pTreeCtrl->m_hWnd ))return false;

    CString sFile=ConstructDataBaseName();
    FILE *fp=fopen(sFile,"rt");
    if(!fp)
    {
        Reset();
        return false;
    }

    //////////////////////////////////////////////////////////////
    // Read the head to check if it is a valid database file:
    //////////////////////////////////////////////////////////////
    char sLine[1000];
    CString s;
    memset(sLine,0,1000);
    fgets(sLine,1000,fp);
    ::RemoveInvalidChar(sLine);
    s=sLine;
    if(s!=m_sDataBaseMark)
    {
        AfxMessageBox(sFile+" 是无效"+m_sFirstLayerName+"文件!");
        return false;
    }

    // The Root text:
    RemoveTreeData();
    pTreeCtrl->DeleteAllItems ();
    SetWindowLong(  pTreeCtrl->m_hWnd ,
        GWL_STYLE ,
        GetWindowLong(pTreeCtrl->m_hWnd,GWL_STYLE)|TVS_HASLINES);
    m_nInsertOrder=0;

    CLineProperty *pLine=NULL;

    /////////////////////////////////////////////
    // Set the ROOT item:
    /////////////////////////////////////////////
    pLine=new CLineProperty;
    pLine->nOrder       =m_nInsertOrder;
    pLine->nProperty    =0;

    // Path name of the database:
    memset(sLine,0,1000);
    fgets(sLine,1000,fp); 
    RemoveInvalidChar(sLine);
    pLine->sPathName    =sLine;

    // The database showing name:
    memset(sLine,0,1000);
    fgets(sLine,1000,fp); 
    RemoveInvalidChar(sLine);
    pLine->sShowName    =sLine;


    HTREEITEM RootItem;
    if(m_bShowFullName)
        RootItem= pTreeCtrl->InsertItem(pLine->sPathName);
    else
        RootItem= pTreeCtrl->InsertItem(pLine->sShowName);

    pTreeCtrl->SetItemState(RootItem, TVIS_BOLD, TVIS_BOLD );	
    pTreeCtrl->SetItemData(RootItem,(DWORD)pLine);

    //////////////////////////////////////////////
    //
    //////////////////////////////////////////////
    CString sAreaName,sLineName,sLineNameShort;
    HTREEITEM AreaItem,LineItem;
    UINT nImageOrder;
    UINT nSelectedImageOrder;

    long nDataProperty=0;
    int i,j,m,nIs2DLine;
    char sPathName[100],sShowName[100];
    int nArea,nLine;
    fscanf(fp,"%d\n",&nArea);
    for(i=0;i<nArea;i++)
    {
        //////////////////////////////////////////////
        // Area Item:
        //////////////////////////////////////////////
        memset(sPathName,0,100);
        fgets(sPathName,100,fp);// The path name
        RemoveInvalidChar(sPathName);

        memset(sShowName,0,100);
        fgets(sShowName,100,fp);// The show name
        RemoveInvalidChar(sShowName);

        pLine=new CLineProperty;
        pLine->sPathName    =sPathName;
        pLine->sShowName    =sShowName;
        pLine->nOrder       =m_nInsertOrder;
        pLine->nProperty    =0; 

        AreaItem=pTreeCtrl->InsertItem (pLine->sShowName,2,3,RootItem, TVI_LAST); 	
        pTreeCtrl->SetItemData(AreaItem,(DWORD)pLine);

        //////////////////////////////////////////////
        // Lines in the area:
        //////////////////////////////////////////////
        fscanf(fp,"%i\n",&nLine);   // The line quantity in the area;		
        for(j=0;j<nLine;j++)
        {
            memset(sPathName,0,100);
            fgets(sPathName,100,fp);// The path name
            RemoveInvalidChar(sPathName);

            memset(sShowName,0,100);
            fgets(sShowName,100,fp);// The show name
            RemoveInvalidChar(sShowName);

            nDataProperty=0;
            m=fscanf(fp,"%ld %u %u %d\n",
                &nDataProperty,
                &nImageOrder,
                &nSelectedImageOrder,
                &nIs2DLine); 

            if(m<4)
            {
                break;
            }

            if(sLineNameShort==sLineName)
            {
                sLineNameShort=SeperateName(sLineName);
            }

            pLine=new CLineProperty;
            pLine->sPathName            =sPathName;
            pLine->sShowName            =sShowName;
            pLine->nProperty            =nDataProperty;
            pLine->nOrder               =m_nInsertOrder;
            pLine->nImageOrder          =nImageOrder;
            pLine->nSelectedImageOrder  =nSelectedImageOrder ; 
            pLine->nIs2DLine            =nIs2DLine;

            pLine->sPathName.Trim();
            pLine->sShowName.Trim();

            if(m_bShowFullName)
                sLineNameShort=pLine->sPathName;
            else
                sLineNameShort=pLine->sShowName;

            if(nSelectedImageOrder ==0)
            {
                nSelectedImageOrder = 1;
            }
            if(nImageOrder ==0)
            {
                nImageOrder = 1;
            }

            LineItem=pTreeCtrl->InsertItem (sLineNameShort,
                nImageOrder,
                nSelectedImageOrder, 
                AreaItem, 
                TVI_LAST); 

            pTreeCtrl->SetItemData (LineItem,(DWORD)pLine);

            m_nInsertOrder++;
        }
    }

    //
    fclose(fp);

    m_bShowSortedName=true;
    SortTree();

    pTreeCtrl->Expand(RootItem,TVE_EXPAND);	

    if(m_pPointerToThis)
    {
        *m_pPointerToThis=this;
    }

    return true;

}

void CFHTreeView::RemoveTreeData()
{
    CTreeCtrl *pTree=& GetTreeCtrl();
    if(!IsWindow(pTree->m_hWnd ))return ;

    HTREEITEM RootItem=pTree->GetRootItem ();
    if(!RootItem)
    {
        return;
    }

    CLineProperty *pLine=(CLineProperty *)pTree->GetItemData (RootItem);
    delete pLine;	

    HTREEITEM LineItem;
    HTREEITEM AreaItem=pTree->GetChildItem(RootItem);
    while(AreaItem)
    {
        //
        CLineProperty *pLine=(CLineProperty *)pTree->GetItemData (AreaItem);
        delete pLine;

        //
        LineItem=pTree->GetChildItem (AreaItem);
        while(LineItem)
        {
            CLineProperty *pLine=(CLineProperty *)pTree->GetItemData (LineItem);
            if(pLine)
            {
                delete pLine;
                pTree->SetItemData (LineItem,(DWORD)NULL);
            }

            LineItem=pTree->GetNextSiblingItem(LineItem);
        }

        AreaItem=pTree->GetNextSiblingItem(AreaItem);
    }

    return ;

}

void CFHTreeView::SortTree()
{
    CTreeCtrl *pTree=& GetTreeCtrl();
    if(!IsWindow(pTree->m_hWnd ))return ;
    HTREEITEM RootItem=pTree->GetRootItem ();
    if(!RootItem)
    {
        return;
    }

    pTree->SortChildren (RootItem);
    HTREEITEM AreaItem=pTree->GetChildItem(RootItem);

    while(AreaItem)
    {		
        pTree->SortChildren (AreaItem);
        AreaItem=pTree->GetNextSiblingItem(AreaItem);
    }

    return;
}

enumDimension CFHTreeView::GetFileDimension(CString sFile)
{
    return Seismic_2D;
}

CString CFHTreeView::GetDataBaseName()
{
    return m_sDataBaseName;
}

void  CFHTreeView::AddMenu(UINT MenuID, CString MenuString)
{
    m_menuManager.AppendMenu(MenuID,MenuString, MF_MENUITEM);
}