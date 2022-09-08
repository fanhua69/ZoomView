// OGL_MFCView.h : interface of the COpenGLView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGL_MFCVIEW_H__F8524DB3_0B8C_11D8_8323_00FD0000053F__INCLUDED_)
#define AFX_OGL_MFCVIEW_H__F8524DB3_0B8C_11D8_8323_00FD0000053F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <gl\GL.h>
#include <gl\GLU.h>

#include "My3DPoint.h"
#include "3DVector.h"

#include "fhZoomview.h"
#include "PopupMenuManager.h"

class CCharShowProperty
{
public:
    CCharShowProperty(  int         nCharOrder,
                        CMy3DPoint  pointLeftDown,
                        CMy3DPoint  pointLeftUp,
                        C3DVector   vecExtension)
    {
        m_nCharOrder    =   nCharOrder;
        m_pointLeftDown =   pointLeftDown; 
        m_pointLeftUp   =   pointLeftUp;
        m_vecExtension  =   vecExtension;
    };

    int         m_nCharOrder;
    CMy3DPoint  m_pointLeftDown; 
    CMy3DPoint  m_pointLeftUp;
    C3DVector   m_vecExtension;
};

class CTrackBall
{
public:
    CTrackBall();

    void        Init            ();
    void        SetViewSize     ( int iWidth, int iHeight);
    C3DVector   PointToVector   ( int x,int y);
    C3DVector   AxisAndAngle    ( int x,int y,bool bLButtonDown);   
    void        RotateMatrix    ();



    double      m_RotArray[16];

public:
    int         m_iViewWidth;
    int         m_iViewHeight;
    C3DVector   m_vecOldPos;
    C3DVector   m_vecRotAxis;
    double      m_dRotAngle;
};


class _FHZOOMVIEW_EXT_CLASS COpenGLView : public CFHZoomView
{
protected: // create from serialization only
    COpenGLView();
    DECLARE_DYNCREATE(COpenGLView)

// Operations
public:
    void        SetDrawRange(   double dXMin, 
                                double dYMin, 
                                double dZMin,
                                double dXMax,
                                double dYMax,
                                double dZMax);

    virtual void    DrawCoor    (   CDC *pDC   );



// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(COpenGLView)
    public:
    virtual void    OnDraw(CDC* pDC);  // overridden to draw this view
    virtual BOOL    PreCreateWindow(CREATESTRUCT& cs);
    protected:
    virtual void    OnInitialUpdate(); // called first time after construct
    //}}AFX_VIRTUAL

// Implementation
public:
    HGLRC m_hRC;
    void            KillScene();
    void            DrawScene();
    void            InitScene();
    virtual         ~COpenGLView();

#ifdef _DEBUG
    virtual void    AssertValid() const;
    virtual void    Dump(CDumpContext& dc) const;
#endif

protected:
    COLORREF        m_colorBackGround;
    double          m_dXMin;
    double          m_dXMax;
    double          m_dYMin;
    double          m_dYMax;
    double          m_dZMin;
    double          m_dZMax;

    CMy3DPoint      m_pointCenter;
    CPoint          m_pointMouseLast;
    double          m_dScale;

    double          m_dXShift;
    double          m_dYShift;
    double          m_dZShift;

    CPoint          m_pointMouseAnchor;
    
    CTrackBall      m_clsTrackBall;
    int             m_nTimer;
    int             m_iUpdateShowTime;
    
    int             m_nAxisRadius   ;
    int             m_nAxisSliceQty ; 
    int             m_nAxisHeadLen  ;

    CFont           m_font;
    GLuint          m_2DTextList;
    GLuint          m_3DTextList;       //三维文字显示列表
    
    void        Create2DTextLists(CDC *pDC);
    void        Create3DTextLists(CDC *pDC);



    void        LoadNumberBitmaps   ();
    void        LoadOneNumberBitmap (   unsigned int nBitmapID, char *pBitMem);
    void        PreSettingDlg       ();
    void        PostSettingDlg      ();

    
    void        WriteString         (   CString         sString, 
                                        CMy3DPoint      pointLeftDown, 
                                        CMy3DPoint      pointLeftUp,
                                        C3DVector       vecExtension);

    void        StoreOneChar        (   char            cNumber, 
                                        CMy3DPoint      pointLeftDown, 
                                        CMy3DPoint      pointLeftUp,
                                        C3DVector       vecExtension);
    bool        PutStringsToView    ();



    static bool m_bNumberBitmapLoaded;
    static char m_cBitmapNumbers[12][8192];
    static char m_nBitmapNumberWidth;
    static char m_nBitmapNumberHeight;

    std::vector<CCharShowProperty>  m_vecCharStore;

// Generated message map functions
protected:
    //{{AFX_MSG(COpenGLView)
    afx_msg int     OnCreate    (LPCREATESTRUCT lpCreateStruct);
    afx_msg void    OnSize      (UINT nType, int cx, int cy);
    afx_msg void    OnDestroy   ();
    afx_msg void    OnSelectMeterialColor();
    afx_msg void    OnKeyDown   (UINT nChar, UINT nRepCnt, UINT nFlags);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()


public:
    afx_msg void OnPaint        ();
    afx_msg void OnMouseMove    (UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown  (UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp    (UINT nFlags, CPoint point);
    afx_msg void OnTimer        (UINT nIDEvent);
    afx_msg void OnContextMenu  (CWnd *pWnd, CPoint point);
    afx_msg void OnKillFocus    (CWnd* pNewWnd);
    afx_msg void OnSetFocus     (CWnd* pOldWnd);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OGL_MFCVIEW_H__F8524DB3_0B8C_11D8_8323_00FD0000053F__INCLUDED_)
