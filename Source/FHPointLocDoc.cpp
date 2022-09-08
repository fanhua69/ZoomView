// FHPointLocDoc.cpp : implementation file
//

#include "stdafx.h"
#include "line.h"
#include "FHPointLocDoc.h"
#include "FHZoomViewGlobal.h"
#include "ProgDlg.h"
#include "DlgMonitor.h"
#include "FHMainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFHPointLocDoc

IMPLEMENT_DYNCREATE(CFHPointLocDoc, CFHEditDoc)

CFHPointLocDoc::CFHPointLocDoc()
{
    m_pPoint            =NULL;
    m_pLinePos          =NULL;
    m_pPointInOneLine   =NULL;
    m_pLineProperty     =NULL;
    m_nNextLine         =0;
    m_nPointStep        =1;
    m_nPointNumber      =0;
    m_nLineNumber       =0;
    m_nPointLimit       =10000;
}

BOOL CFHPointLocDoc::OnNewDocument()
{
    if (!CFHEditDoc::OnNewDocument())
        return FALSE;
    return TRUE;
}

CFHPointLocDoc::~CFHPointLocDoc()
{
    if(m_pPoint)
    {
        delete []m_pPoint;
        m_pPoint=NULL;
    }
    m_nPointNumber=0;

    if(m_pPointInOneLine)
    {
        delete []m_pPointInOneLine;
        m_pPointInOneLine=NULL;
    }

    if(m_pLineProperty)
    {
        delete []m_pLineProperty;
        m_pLineProperty=NULL;
    } 
    m_nLineNumber=0;

    if(m_pLinePos)
    {
        delete []m_pLinePos;
        m_pLinePos=NULL;
    }
}


BEGIN_MESSAGE_MAP(CFHPointLocDoc, CFHEditDoc)
    //{{AFX_MSG_MAP(CFHPointLocDoc)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFHPointLocDoc diagnostics

#ifdef _DEBUG
void CFHPointLocDoc::AssertValid() const
{
    CFHEditDoc::AssertValid();
}

void CFHPointLocDoc::Dump(CDumpContext& dc) const
{
    CFHEditDoc::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CFHPointLocDoc serialization

void CFHPointLocDoc::Serialize(CArchive& ar)
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
// CFHPointLocDoc commands

bool CFHPointLocDoc::SetLineNumber(long nLineNumber)
{	
    if(nLineNumber<1||nLineNumber>10000)return false;

    if(m_pLineProperty){
        delete []m_pLineProperty;
        m_pLineProperty=NULL;
    }

    if(m_pLinePos){
        delete []m_pLinePos;
        m_pLinePos=NULL;
    }

    m_pLineProperty=new CLineProperty[nLineNumber+10];
    m_pLinePos=new long [nLineNumber+10];
    memset(m_pLinePos,0,sizeof(long)*nLineNumber);
    m_nLineNumber=nLineNumber;
    m_nNextLine=0;

    return true;
}

bool CFHPointLocDoc::SetPointNumber(long nPointNumber)
{
    if(nPointNumber<1)
    {
        return false;
    }
    else if(nPointNumber>500000)
    {
        TRACEERROR("问题：点数目太多，将不显示全部");
        m_nPointStep=int(nPointNumber/m_nPointLimit)+1;
        nPointNumber=m_nPointLimit;		
    }

    if(m_pPoint)
    {
        delete []m_pPoint;
        m_pPoint=NULL;
    }
    m_pPoint=new CXYZ[nPointNumber+10];
    memset(m_pPoint,0,sizeof(CXYZ)*nPointNumber);
    m_nPointNumber=nPointNumber;	

    return true;

}
bool CFHPointLocDoc::GetNorthRange(double &dMin, double &dMax)
{
    if(!m_pPoint)return false;

    dMin=dMax=m_pPoint[0].north;
    for(long i=1;i<m_pLinePos[m_nNextLine];i++)
    {
        if(m_pPoint[i].north<dMin)dMin=m_pPoint[i].north;
        if(m_pPoint[i].north>dMax)dMax=m_pPoint[i].north;
    } 
    return true;
}
bool CFHPointLocDoc::GetEastRange(double &dMin, double &dMax)
{
    if(!m_pPoint)return false;

    dMin=dMax=m_pPoint[0].east;
    for(long i=1;i<m_pLinePos[m_nNextLine];i++)
    {
        if(m_pPoint[i].east<dMin)dMin=m_pPoint[i].east;
        if(m_pPoint[i].east>dMax)dMax=m_pPoint[i].east;
    } 
    return true;
}

bool CFHPointLocDoc::GetValueRange(double &dMin, double &dMax)
{
    if(!m_pPoint)return false;

    dMin=dMax=m_pPoint[0].value;
    for(long i=1;i<m_pLinePos[m_nNextLine];i++)
    {  
        if(m_pPoint[i].value<dMin)dMin=m_pPoint[i].value;
        if(m_pPoint[i].value>dMax)dMax=m_pPoint[i].value;
    }

    return true;
}

bool CFHPointLocDoc::AddLine(CLineProperty LineProperty, CXYZ *pPoint, long nPoint)
{
    if(!m_pPoint||!m_pLinePos||
        m_nLineNumber==0||m_nPointNumber==0||m_nNextLine>=m_nLineNumber)return false;		

    long nStartPos=m_pLinePos[m_nNextLine];
    if((nStartPos+nPoint/m_nPointStep)>m_nPointNumber)return false;

    m_pLineProperty[m_nNextLine]=LineProperty;

    ///////////////////////////////////////////////////////////
    // If there are too many points, then do not add them all:
    long nAddedPoint=0,n;
    for(long i=0;i<nPoint;i+=m_nPointStep)
    {
        n=nStartPos+nAddedPoint;
        if(n>=m_nPointNumber)
        {
            return false;
        }

        m_pPoint[n]=pPoint[i];
        nAddedPoint++;
    }

    m_nNextLine++;
    m_pLinePos[m_nNextLine]=nStartPos+nAddedPoint;

    return true;
}

long CFHPointLocDoc::GetLinePointNumber(long nLine)
{
    if(nLine<0||nLine>=m_nLineNumber)return 0;

    return m_pLinePos[nLine+1]-m_pLinePos[nLine];
}

CString CFHPointLocDoc::GetLineName(long nLine)
{
    if(nLine<0||nLine>=m_nLineNumber)return CString("");

    return m_pLineProperty[nLine].sPathName ;
}

long CFHPointLocDoc::GetLineNumber()
{
    return m_nLineNumber;
}


long CFHPointLocDoc::SetLinePoints(CString sArea,CString sLine,CXYZ *pPointSpace,long nPointLimit,bool *pb2DLine)
{
    return 0;
}

long CFHPointLocDoc::GetLineOrder(CString sLineSearch)
{
    sLineSearch.MakeUpper();
    CString sLine;
    for(long i=0;i<GetLineNumber();i++)
    {
        sLine=GetLineName(i);
        sLine.MakeUpper ();
        if(sLine==sLineSearch)
        {
            return i;
        }
    }

    return 0;
}

long CFHPointLocDoc::GetLinePointNumber(CString sArea, CString sLine)
{
    if(sLine=="")return 0;
    return GetFileLineNumber(sLine);
}

CXYZ * CFHPointLocDoc::GetLine(long nLine)
{
    if(nLine<0||nLine>=m_nLineNumber)return NULL;

    return &m_pPoint[m_pLinePos[nLine]];
}

CXYZ * CFHPointLocDoc::GetLine(CString sLine)
{
    long nLine=GetLineOrder(sLine);
    if(nLine<0||nLine>=m_nLineNumber)return NULL;

    return &m_pPoint[m_pLinePos[nLine]];
}


long CFHPointLocDoc::GetTotalPointNumber()
{
    return m_nPointNumber;
}

CXYZ *CFHPointLocDoc::GetAllPoints()
{
    return m_pPoint;
}

bool CFHPointLocDoc::SetMaxPointNumberInOneLine(long nMaxLinePointNumber)
{
    if(nMaxLinePointNumber<1||nMaxLinePointNumber>500000)return false;

    if(m_pPointInOneLine)
    {
        delete []m_pPointInOneLine;
        m_pPointInOneLine=NULL;
    }
    m_pPointInOneLine=new CXYZ [nMaxLinePointNumber+10];

    return true;
}


bool CFHPointLocDoc::SetData(CString sArea,CLineProperty *pLines,long nLineNumber)
{
    CProgressDlg dlg("正在加载数据");

    dlg.Create();
    dlg.SetRange (0,nLineNumber*2);
    dlg.SetStatus("加载数据...");

    bool bFailed=false;
    long i,nTotalPoint=0,nFileLineNumber,nMaxLinePointNumber=0;;
    for(i=0;i<nLineNumber;i++)
    {
        dlg.SetPos(i);
        if(dlg.CheckCancelButton())
        {
            bFailed=true;
            break;
        }

        nFileLineNumber=GetLinePointNumber(sArea,pLines[i].sPathName );
        if(nFileLineNumber>nMaxLinePointNumber)
        {
            nMaxLinePointNumber=nFileLineNumber;
        }
        nTotalPoint+=nFileLineNumber;
    }

    CXYZ *pXYZ=NULL;
    long nPointNumber;
    bool nIs2DLine=false;

    if(     !bFailed    
        &&  SetMaxPointNumberInOneLine(nMaxLinePointNumber)
        &&  SetLineNumber(nLineNumber)
        &&  SetPointNumber(nTotalPoint+nLineNumber))  // nLineNumber: useless, to prevent from those unexpected.
    {

        ///////////////////////////////////////////////////////
        // Add the lines:
        ///////////////////////////////////////////////////////
        pXYZ=NULL;
        nPointNumber=0;
        nIs2DLine=false;
        for(i=0;i<nLineNumber;i++)
        {
            dlg.SetPos(i+nLineNumber);
            if(dlg.CheckCancelButton())
            {
                bFailed=true;
                break;
            }

            nPointNumber=SetLinePoints( sArea,
                                        pLines[i].sPathName ,
                                        m_pPointInOneLine,
                                        nMaxLinePointNumber,
                                        &nIs2DLine);

            if(nPointNumber==0)
            {
                continue;
            }

            pLines[i].nIs2DLine =nIs2DLine ;
            AddLine(pLines[i],m_pPointInOneLine,nPointNumber);
        }
    }

    dlg.DestroyWindow();

    return true;
}

bool CFHPointLocDoc::SetData(CString sDataFile)
{
    ///////////////////////////////////////////////
    // Firstly Check how many lines and points:
    ///////////////////////////////////////////////
    FILE *fp=fopen(sDataFile,"rt");
    if(!fp){
        AfxMessageBox("Error: can not open file:"+sDataFile);
        return false;
    }

    long nLineNumber;
    fscanf(fp,"%d\n",&nLineNumber);

    long i,j,nTotalPoint=0,nLinePointNumber,nMaxLinePointNumber=0;;
    CLineProperty lineProperty;
    double x,y;

    CProgressDlg dlg("Loading");
    dlg.Create();
    dlg.SetRange (0,nLineNumber*2);
    dlg.SetStatus("Loading data ...");
    char sLine[200];
    bool bFailed=false;

    for(i=0;i<nLineNumber;i++)
    {
        dlg.SetPos(i);
        if(dlg.CheckCancelButton())
        {
            bFailed=true;
            break;
        }
        fgets(sLine,200,fp); // The interface name;
        // fscanf(fp,"%lf",&lineProperty.dVelocity);  // The velocity; Useless?
        fscanf(fp,"%ld",&nLinePointNumber);  // The Line point number;

        if(nLinePointNumber>nMaxLinePointNumber)
        {
            nMaxLinePointNumber=nLinePointNumber;
        }
        nTotalPoint+=nLinePointNumber;

        for(j=0;j<nLinePointNumber;j++)
        {
            fscanf(fp,"%lf %lf\n",&x,&y);
        }
    }

    fclose(fp);

    if(!bFailed)
    {
        SetMaxPointNumberInOneLine(nMaxLinePointNumber);

        SetLineNumber(nLineNumber);
        SetPointNumber(nTotalPoint+nLineNumber);  // nLineNumber: useless, to prevent from those unexpected.
    }

    ///////////////////////////////////////////////////////
    // Add the lines:
    ///////////////////////////////////////////////////////
    if(!bFailed)
    {
        fp=fopen(sDataFile,"rt");

        fscanf(fp,"%d\n",&nLineNumber);

        for(i=0;i<nLineNumber;i++)
        {
            dlg.SetPos(i+nLineNumber);
            if(dlg.CheckCancelButton())
            {
                bFailed=true;
                break;
            }

            // The line property:
            fscanf(fp,"%s\n",lineProperty.sPathName .GetBuffer(100)); // The interface name;
            //fscanf(fp,"%lf",&lineProperty.dVelocity);  // The velocity; Useless?
            fscanf(fp,"%ld",&nLinePointNumber);  // The Line point number;

            lineProperty.sPathName .ReleaseBuffer ();

            // The interface points:
            for(j=0;j<nLinePointNumber;j++){
                fscanf(fp,"%lf %lf\n",&m_pPointInOneLine[j].east ,&m_pPointInOneLine[j].north);
            }
            AddLine(lineProperty,m_pPointInOneLine,nLinePointNumber);

        }

        fclose(fp);
    }

    dlg.DestroyWindow();

    return true;
}

CLineProperty * CFHPointLocDoc::GetLineProperty(long nOrder)
{
    if(nOrder<0||nOrder>=m_nLineNumber)return NULL;

    return &m_pLineProperty[nOrder];
}

long CFHPointLocDoc::GetLineOrder(long nPointOrder)
{
    for(int i=0;i<GetLineNumber ();i++)
    {
        if(   nPointOrder>=m_pLinePos[i]
        &&nPointOrder<=m_pLinePos[i+1])
        {
            return i;
        }
    }

    return -1;
}