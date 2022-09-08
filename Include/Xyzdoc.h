#if !defined(AFX_XYZDOC_H__E33AC4E3_A5CB_11D3_96D8_00C04FCCB957__INCLUDED_)
#define AFX_XYZDOC_H__E33AC4E3_A5CB_11D3_96D8_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XYZDoc.h : header file
//

#include "zoomview.h"

/////////////////////////////////////////////////////////////////////////////
// CXYZDoc document
class _FHZOOMVIEW_EXT_CLASS CXYZ
{
public :
    char    sLine[16];
    double  dStation;
    double  north;
    double  east;
    float   value;
    bool    bShot;

    CXYZ()
    {
        memset(this,0,sizeof(CXYZ));
    }

    void operator = (CXYZ &other)
    {
        memcpy(this,&other,sizeof(CXYZ));
    };
};

#include "FHEditDoc.h"
#include "zoomview.h"

class _FHZOOMVIEW_EXT_CLASS CXYZDoc : public CFHEditDoc
{
public:
    CXYZDoc();           // protected constructor used by dynamic creation
    DECLARE_DYNCREATE(CXYZDoc)

    // Attributes
public:

    CXYZ *m_pData;


    // Operations
public:

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CXYZDoc)
public:
    virtual void Serialize(CArchive& ar);   // overridden for document i/o
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
protected:
    virtual BOOL OnNewDocument();
    //}}AFX_VIRTUAL

    // Implementation
public:
    static CString GetFileForSave();
    static CString GetFileForOpen();
    bool SetRecordNumber(long nRecord);
    virtual ~CXYZDoc();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    // Generated message map functions
protected:
    //{{AFX_MSG(CXYZDoc)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XYZDOC_H__E33AC4E3_A5CB_11D3_96D8_00C04FCCB957__INCLUDED_)
