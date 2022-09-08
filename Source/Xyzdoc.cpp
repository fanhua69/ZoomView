// XYZDoc.cpp : implementation file
//

#include "stdafx.h"
#include "XYZDoc.h"
#include "math.h"
#include "ProgDlg.h"
#include "FHZoomViewGlobal.h"
#include "FHMainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXYZDoc

IMPLEMENT_DYNCREATE(CXYZDoc, CFHEditDoc)

#define MAX_XYZ_NUMBER 10000

CXYZDoc::CXYZDoc()
{
    m_pData=NULL;
}

BOOL CXYZDoc::OnNewDocument()
{
    if (!CFHEditDoc::OnNewDocument())
        return FALSE;

    if(m_pData){
        delete []m_pData;
        m_pData=NULL;
    }

    return TRUE;
}

CXYZDoc::~CXYZDoc()
{
    if(m_pData){
        delete []m_pData;
        m_pData=NULL;
    }	
}


BEGIN_MESSAGE_MAP(CXYZDoc, CFHEditDoc)
    //{{AFX_MSG_MAP(CXYZDoc)
    ON_COMMAND(ID_FILE_SAVE, OnFileSave)
    ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXYZDoc diagnostics

#ifdef _DEBUG
void CXYZDoc::AssertValid() const
{
    CFHEditDoc::AssertValid();
}

void CXYZDoc::Dump(CDumpContext& dc) const
{
    CFHEditDoc::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CXYZDoc serialization

void CXYZDoc::Serialize(CArchive& ar)
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
// CXYZDoc commands
BOOL CXYZDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
    if (!CFHEditDoc::OnOpenDocument(lpszPathName))
        return FALSE;

    //////////////////////////////////////
    // Firstly chek how many points are there in the file:
    long m=GetFileLineNumber(lpszPathName);
    if(m<0)
    {
        TRACEERROR("错误：文件内的记录数目为零--"+CString(lpszPathName));
        return false;
    }

    if(m>10000)
    {
        m=10000;
    }

    if(!SetRecordNumber(m))
    {
        TRACEERROR("错误：无法为数据分配内存:"+vtos(m));
        return false;
    }	

    ////////////////////////////////
    // Read data:
    FILE *fp=fopen(lpszPathName,"rt");
    if(!fp)
    {
        TRACEERROR("无法打开文件:" +CString(lpszPathName));
        return false;
    }	

    for(long i=0;i<m;i++)
    {
        fscanf(fp,"%lf %lf %f\n",&m_pData[i].east,&m_pData[i].north,&m_pData[i].value);
    }

    fclose(fp);
    return TRUE;
}

bool CXYZDoc::SetRecordNumber(long nRecord)
{
    if(!CFHEditDoc::SetRecordNumber (nRecord))return false;

    if(m_pData){
        delete []m_pData;
        m_pData=NULL;
    }
    m_pData=new CXYZ[nRecord+10];

    return true;
}

CString CXYZDoc::GetFileForOpen()
{
    return ::GetFileForOpen ("XYZ文件(*.xyz)","xyz");
}

CString CXYZDoc::GetFileForSave()
{
    return ::GetFileForSave ("XYZ文件(*.xyz)","xyz");
}


