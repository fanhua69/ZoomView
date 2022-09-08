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
    CFHPointLocView();             // ���캯��
    DECLARE_DYNCREATE(CFHPointLocView)

    CString m_sAreaCurrent;        // ��ǰ��������
    CString m_sLineCurrent;        // ��ǰ��������

    long m_nLineCurrent;           // ��ǰ���ߺ�
    long m_nLinePointed;           // ��ǰ���ߺ�
    long m_nPointCurrent;          // ��ǰ������
    long m_nPointPointed;          // ��ǰ�����ָ�ĵ����

    CColorDlg m_dlgColor;          // ɫ��Ի���
    bool m_bColorDlgCanShow;       // �Ƿ���ʾɫ��Ի���

    int m_nPointSize;              // ����Ĵ�С
    int m_nDrawStyle;              // ����ķ�ʽ 0: ���ε� 1: �ߡ��㷽ʽ

    bool m_bWriteName;             // �Ƿ���ʾ��������

    bool m_bInfoForLine;           // ��Ϣ���ǲ�����Ϣ���ǵ����Ϣ


    CPen m_penCommon;              // ��ͨ��
    CPen m_penCurrent;             // ��ǰ��
    CPen m_penPointed;             // �����ָ�ĵ�ı�

    bool m_bCheckScreenPos;

    bool m_bShowColorValue;         // �Ƿ�����ɫ��ʾ��ֵ����


public:
    virtual void    DrawPoint           (CDC *pDC,long nLine);
    virtual bool    DrawLine            (CDC *pDC, long nLineOrder);    // ���Ƶ�nLineOrder������
    virtual void    DrawPoint           (CDC *pDC);                     // �����������е�
    virtual void    DrawLine            (CDC *pDC);                     // ��������������
    void            DrawName            (CDC *pDC);                     // ���Ʋ�������
    virtual bool    UpdateDataRange     ();                             // ����ȡֵ�ķ�Χ
    long            GetNearestLine      (   double x, double y,double &dMinDis);
    CXYZ *          GetNearestPoint     (   double x, double y,long &nLine,long &nPoint,double &dMinDis);

public:
    virtual void    UpdateDrawingLine   (   CString         sArea, 
                                            CString         sLineCurrent) ;  // �ػ����ߣ���ǰ���߸ı�

    virtual void    UpdateDrawingArea   (   CString         sArea, 
                                            CLineProperty * pLine, 
                                            long            nLineNumber,
                                            CString         sLineCurrent);

    virtual CString GetMapName          (   );
    virtual CString GetValueName        (   );


    CString         GetInfo             (double x, double y);       // ��þ����x,y������ߡ������Ϣ
    void            SetDrawStyle        (int nStyle);               // ���û��Ʒ�ʽ
    void            SetPointSize        (int nPointSize);           // ���û��Ƶ�Ĵ�С
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
