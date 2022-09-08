#if !defined(AFX_FHPOINTLOCVIEW_H__F7CD0781_E83B_11D3_AEF5_00C04FCCB957__INCLUDED_)
#define AFX_FHPOINTLOCVIEW_H__F7CD0781_E83B_11D3_AEF5_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FHPointLocView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFHPointLocView view

#include "zoomview.h"
#include "FHZoomView.h"
#include "ColorScale.h"
#include "XYZDoc.h"
#include "LineProperty.h"
#include "FHString.h"
#include "MultiChildView.h"

class _FHZOOMVIEW_EXT_CLASS CFHPointLocView : public CMultiChildView
{
protected:
    CFHPointLocView();             // 构造函数
    DECLARE_DYNCREATE(CFHPointLocView)

    CString m_sAreaCurrent;        // 当前工区名称
    CString m_sLineCurrent;        // 当前测线名称

    long m_nLineCurrent;           // 当前测线号
    long m_nLinePointed;           // 当前测线号
    long m_nPointCurrent;          // 当前点的序号
    long m_nPointPointed;          // 当前鼠标所指的点序号

    CColorDlg m_dlgColor;          // 色标对话框
    bool m_bColorDlgCanShow;       // 是否显示色标对话框

    int m_nPointSize;              // 画点的大小
    int m_nDrawStyle;              // 画点的方式 0: 矩形点 1: 线、点方式

    bool m_bWriteName;             // 是否显示测线名称

    bool m_bInfoForLine;           // 信息窗是测线信息还是点的信息


    CPen m_penCommon;              // 普通笔
    CPen m_penCurrent;             // 当前笔
    CPen m_penPointed;             // 鼠标所指的点的笔

    bool m_bCheckScreenPos;

    bool m_bShowColorValue;         // 是否用颜色显示数值属性


public:
    virtual void    DrawPoint           (CDC *pDC,long nLine);
    virtual bool    DrawLine            (CDC *pDC, long nLineOrder);    // 绘制第nLineOrder条测线
    virtual void    DrawPoint           (CDC *pDC);                     // 画工区内所有点
    virtual void    DrawLine            (CDC *pDC);                     // 画工区内所有线
    void            DrawName            (CDC *pDC);                     // 绘制测线名称
    virtual bool    UpdateDataRange     ();                             // 设置取值的范围
    long            GetNearestLine      (   double x, double y,double &dMinDis);
    CXYZ *          GetNearestPoint     (   double x, double y,long &nLine,long &nPoint,double &dMinDis);

public:
    virtual void    UpdateDrawingLine   (   CString         sArea, 
                                            CString         sLineCurrent) ;  // 重画测线，当前测线改变

    virtual void    UpdateDrawingArea   (   CString         sArea, 
                                            CLineProperty * pLine, 
                                            long            nLineNumber,
                                            CString         sLineCurrent);

    virtual CString GetMapName          (   );
    virtual CString GetValueName        (   );


    CString         GetInfo             (double x, double y);       // 获得距离点x,y最近的线、点的信息
    void            SetDrawStyle        (int nStyle);               // 设置绘制方式
    void            SetPointSize        (int nPointSize);           // 设置绘制点的大小
    virtual bool    CheckPointingChange (double x,double y,double &dMinDistance);


protected:

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CFHPointLocView)
public:
    virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
protected:
    virtual void OnDraw(CDC* pDC);      // overridden to draw this view
    virtual void OnInitialUpdate();     // first time after construct
    virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    virtual ~CFHPointLocView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    // Generated message map functions
    //{{AFX_MSG(CFHPointLocView)
    afx_msg void OnFHZoomViewLast();
    afx_msg void OnFHZoomViewNext();
    afx_msg void OnUpdateLastLine(CCmdUI* pCmdUI);
    afx_msg void OnUpdateNextLine(CCmdUI* pCmdUI);
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FHPOINTLOCVIEW_H__F7CD0781_E83B_11D3_AEF5_00C04FCCB957__INCLUDED_)
