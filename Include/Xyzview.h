#if !defined(AFX_XYZVIEW_H__E33AC4E4_A5CB_11D3_96D8_00C04FCCB957__INCLUDED_)
#define AFX_XYZVIEW_H__E33AC4E4_A5CB_11D3_96D8_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XYZView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CXYZView view
#include "FHEditView.h"
#include "XYZDoc.h"
#include "zoomview.h"

class _FHZOOMVIEW_EXT_CLASS CXYZView : public CFHEditView
{
protected:
    CXYZView();           // protected constructor used by dynamic creation
    DECLARE_DYNCREATE(CXYZView)

    // Attributes
public:

    // Operations
public:
    CXYZDoc* GetDocument();
    void UpdateGrid();
    void UpdateDoc();

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CXYZView)
public:
    virtual void OnInitialUpdate();
protected:
    virtual void OnDraw(CDC* pDC);      // overridden to draw this view
    //}}AFX_VIRTUAL

    // Implementation
protected:
    virtual ~CXYZView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    // Generated message map functions
protected:
    //{{AFX_MSG(CXYZView)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XYZVIEW_H__E33AC4E4_A5CB_11D3_96D8_00C04FCCB957__INCLUDED_)
