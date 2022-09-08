#pragma once

#include "mypoint.h"
#include "MyRect.h"
#include "InfoDlg.h"
#include <vector>
#include "FHZoomApprDlg.h"
#include "DlgMultiRgnViewProperty.h"
#include "PropertyPageManager.h"
#include "FHImage.h"
#include "3DRange.h"

#define ID_SHOWINFOMATION           WM_USER+201

#include "PopupMenuManager.h"

class CMainFrame;

enum EZoomViewCoorStyle
{
    eCS_Rectangle,
    eCS_LeftDown,
};

enum EDataDrawType
{
    eDDT_Data,
    eDDT_Ratio,
};

enum EMapDrawType
{
    eMDT_Curve,
    eMDT_Rectangle,
};

enum enumZoomStatue
{
    statueIdle,           // ����
    statueZoomInArea,     // ����Ŵ�
    statueZoomIn,         // �Ŵ�
    statueZoomOut,        // ��С
    statueZoomInHor,      // ����Ŵ�
    statueZoomOutHor,     // ������С
    statueZoomInVer,      // ����Ŵ�  
    statueZoomOutVer,     // ������С
    statueZoomHVSame,     // �ݺ��һ��
};

// ������ϵ�йصĲ���
enum enumZoomCoorType 
{
    CT_NULL=0,
    CT_NET,
    CT_XY,
    CT_XYY
};

//��ߵ�λ
enum enumZoomUnitType
{
    ULT_ENGLISH = 0,     //Ӣ��
    ULT_METRIC,          //����
    ULT_PIXEL,           //����
    ULT_SCALETOFIT        //���ʴ�С
};


class CMouseAnchorWnd;

#include "zoomview.h"
#include "mousehook.h"

class _FHZOOMVIEW_EXT_CLASS CFHZoomView : public CView, public CPropertyPageManager
{
    DECLARE_DYNCREATE(CFHZoomView)

    friend class CMouseAnchorWnd;
    friend class CMultiView;


protected:
    CFHZoomView();
    virtual ~CFHZoomView();

protected:
    CMouseAnchorWnd     *   m_pAnchorWindow;                // ������������ê��Ϣ


    CMouseHook              m_hookMouse;

    CMultiDocTemplate   *   m_pTemplate;

    EZoomViewCoorStyle      m_eCoorStyle;


protected:
    //
    CToolBar *  m_pZoomViewToolBar;

    // ������йصĲ���
    bool        m_bDrawTitle;
    LOGFONT     m_logfontTitle;
    COLORREF    m_colorTitle;
    CString     m_sDrawTitle;
    CString     m_sError;

    // ���ӡ�йصĲ���
    bool        m_bPrintByScale;
    CMySize     m_totalDevBak; // ��ӡ֮ǰ�洢�豸�Ĵ�С

    bool        m_bDrawCoor;
    bool        m_bDrawBar;

    enumZoomCoorType m_nCoorType;
    LOGFONT     m_logfontCoor;
    COLORREF    m_colorThickCoorHor;
    COLORREF    m_colorThinCoorHor;
    COLORREF    m_colorThickCoorVer;
    COLORREF    m_colorThinCoorVer;
    COLORREF    m_colorFontCoor;
    int         m_nMidScaleDensity;

    bool        m_bUseUserCoor;
    CMyRect     m_rectUserThickLine;
    CMyRect     m_rectUserThinLine;
    CMySize     m_sizeUserThickStep;
    CMySize     m_sizeUserThinStep;

    int         m_iContentSize;
    int         m_iContentCurrSize;

    // ��ͼ��
    LOGFONT     m_logfontMapChar;
    COLORREF    m_colorMapChar;
    LOGFONT     m_logfontCurrChar;
    COLORREF    m_colorCurrChar;

    CFont       m_fontTextCommon;
    CFont       m_fontTextCurrent;



    int         m_nFont;                // =90
    CFont       * m_pFontTilt;          // limit:90

    // ����Ϣ���йصĺ���
    bool        m_bEnableInfoTimer;

    CInfoDlg    m_dlgInfo;
    bool        m_bInfoOnShow;
    long        m_timeInterval;
    long        m_timeToShowInfo;
    long        m_timeMouseInSamePlace;
    CPoint      m_pointLastMouse;
    CPoint      m_pointCurrentMouse;
    bool        m_bInfoCanShowBak;

    CPoint      m_pointZoomAreaAnchor;
    bool        m_bZoomAreaAnchored;
    CPen        m_penZoomRect;


    bool        m_bMouseAnchored;
    CMyPoint    m_pointMouseAnchorPoint;

    ///////////////////////////////////
    // �������Ϣ���йصĺ���
    ///////////////////////////////////
    CInfoDlg    m_dlgErrorMsg;
    bool        m_bErrorCanShowBak;
    bool        m_nFirstShow;

    ///////////////////////////////////
    // Ϊ�̳���Ԥ��ͼ����ɫ��
    ///////////////////////////////////
    COLORREF    m_colorMapContent;
    COLORREF    m_colorMapContentSpecial;

    ///////////////////////////////////
    // ������ߵ�����
    ///////////////////////////////////
    CFont       m_fontCorVer;
    CFont       m_fontCorHor;

    CString     m_sXUnit;
    CString     m_sYUnit;

    ///////////////////////////////////
    // Cursors:
    ///////////////////////////////////
    HCURSOR     m_hCursorZoomIn;
    HCURSOR     m_hCursorZoomOut;
    HCURSOR     m_hCursorDefault;

    enumZoomStatue   m_eZoomState;                            // �Ŵ��ʶ
    enumZoomUnitType m_nMapMode;                            // ӳ�䷽ʽ

    int         m_nBarWidth;
    int         m_nEdgeWidth;

    CMyPoint    m_pointLogOrgBak;                           // Used for saving m_pointLogOrg while drawing many coordinates.
    CMyPoint    m_pointLogOrg;                              // �߼�����ԭ��
    CMyPoint    m_pointDevOrg;                              // �ͻ����Ŀ�ʼ�����������
    CMySize     m_totalLog;                                 // �߼������С
    CMySize     m_totalDev;                                 // ���������С�������߼������С�ı�ֵ�����˷Ŵ�ϵ����
    CMySize     m_pageDev;                                  // ����ҳ�Ĵ�С
    CMySize     m_lineDev;                                  // �����еĴ�С
    CSize       m_sizeScrollBar;
    bool        m_bDevSizeLimit;                            // �Ƿ������豸��С
    BOOL        m_bCenter;                                  // ���ھ���
    BOOL        m_bInsideUpdate;                            // OnSize���ڲ��ط�״̬

    double      m_dCooeHor;                                 // ˮƽ�������������ϵ��
    double      m_dCooeVer;                                 // ��ֱ�������������ϵ��

    bool        m_bUpIncrease;                              // �����Ƿ�Ϊ��
    CMyRect     m_rectCoor;

    ///////////////////////////////////
    // ����һ���һ���йصı���
    ///////////////////////////////////
    long        m_nCurrentPage;
    long        m_nPageQty;

    ///////////////////////////////////
    // ���ڲ����洢
    ///////////////////////////////////
    CString     m_sClassName;

    CPopupMenuManager m_menuManager;

    int         m_nEmptyZoomView;
    int         m_nEmptyZoomView2;
    int         m_nEmptyZoomView3;

    std::vector<CPropertyPage*> m_vecSettingsDlg;
    bool        m_bInvalidateAfterSetting;

    EPointDrawPattern m_ePointPatternCommon;
    EPointDrawPattern m_ePointPatternCurrent;


    CString     m_sBackGroundFileName;
    CMyRect     m_rectBackGroundPic;

    bool        m_bImgBackKeepRatio;

    int         m_nMouseLBDownTimer ;


    /////////////////////////////////////////////
    // ����
    /////////////////////////////////////////////
public:
    void LPtoWorld  (double   &x, double &y);
    void WorldtoLP  (double   &x, double &y);
    void LPtoWorldX (double   &x);
    void LPtoWorldY (double   &y);
    void WorldtoLPX (double   &x);
    void WorldtoLPY (double   &x);
    void LPtoWorld  (CMyPoint &pt);
    void WorldtoLP  (CMyPoint &pt);
    void LPtoWorldM (CMyPoint *pPoint,long nPoint);
    void WorldtoLPM (CMyPoint *pPoint,long nPoint);
    void Rectangle  (CDC *pDC, double x1,double y1,double x2,double y2);
    void EnableInfo (bool bEnable);
    void EnableCoor (bool bEnable);
    void CopyViewPar(CFHZoomView  *pView);

    C3DRange<double> GetDrawRange            (   );


    void DrawLine                           (   CDC      *pDC, 
                                                CRect    &rect,
                                                int      iPenStyle, 
                                                int      iPenWidth,
                                                COLORREF nColor);

    void SetCoorStyle                       (EZoomViewCoorStyle eCoorStyle);

    virtual void    CheckAnchoredPoint      (double dLogicalX, double dLogicalY);
    virtual void    MoveAnchoredPoint       (double dAddedLogicalX, double dAddedLogicalY);
    CPoint          GetMousePos             ();
    void            InvalidateInfo          ();

    void            SetAxisUnit             ( CString sXUnit, CString sYUnit);

    int             GetContentSize          ();
    int             GetCurrentContentSize   ();

protected:
    void            GetClientLogicalRange   (   double &dMinX,double &dMinY,double &dMaxX,double &dMaxY);
    bool            Line                    (   CDC *pDC,double x1,double y1,double x2,double y2);          // ����
    void            ZoomFull                (   );                                                          // �Ŵ�ȫ�����ݵ�������Ļ
    static CString  GetValueString          (   double dValue,double dStep);                                // ����˫�������ݵĴ�С����һ�����ʵ��ַ���
    void            HideSearchButton        (   bool bHide=true);                                           // ���ع������ϵġ���������ť
    void            SetPrintByScale         (   bool bByScale=true);                                        // �趨��ӡ�������Ƿ���ԭʼ���ݽ����ݺ��1:1�Ĵ�ӡ	
    void            GetLogicalRange         (   double &x1,double &y1,double &x2,double &y2);               // ����߼���Χ
    bool            SetMidScaleDensity      (   long n);                                                    // �趨ϸ�����ߵ��ܶȡ�
    enumZoomStatue  GetZoomStatue           (   );                                                          // ��÷Ŵ�״̬
    void            SetToolBar              (   CToolBar *pBar);                                            // �趨������
    void            SetDirection            (   bool bUpIncrease);                                          // �趨��ʾ����������ֵ����Ϊ��
    bool            GetDirection            (   );
    void            SetDrawTitle            (   CString sTitle);                                            // �趨ͼͷ�ַ���
    void            SetCoorRange            (   double dLeft,double dBottom,double dRight,double dTop);     // �趨���귶Χ
    virtual void    DrawCoor                (   CDC     *pDC);                                              // ��������

    void            DrawCoor                (   CDC     *pDC,
                                                double  dLeft,
                                                double  dRight, 
                                                double  dLow, 
                                                double  dHigh, 
                                                float   fHorValueRatio=1.0,
                                                float   fVerValueRatio=1.0);

    void            DrawBackPicture         (   CDC     *pDC    );

    void            DrawCoorByHorValues     (   CDC     *pDC,
                                                double  dLeft,
                                                double  dRight, 
                                                double  dLow, 
                                                double  dHigh, 
                                                float   fHorValueRatio=1.0,
                                                float   fVerValueRatio=1.0);

    virtual void    DrawPointOfType         (   CDC     *pDC,
                                                double  x,
                                                double  y,
                                                EPointDrawPattern eType,
                                                int     iPointSize);


    virtual int     GetPointQty             ();
    virtual double  GetCoor                 (int iRgnOrder, int iPointOrder, int iSeries);
    virtual void    GetCoorNote             (CStringList &listNote,int iRgnOrder, int iPointOrder, int iSeries);

    void            SetTiltFont             (LOGFONT *pLogFont=NULL,CDC *pDC=NULL);                     // �趨������ʾ����
    void            GetVisibleArea          (double &x1,double &y1,double &x2,double &y2);              // ����߼��ϵ���ʾ��Χ
    virtual CString GetInfo                 (double x,double y);                                        // ���������ʾ���û���Ϣ
    void            CalcScrollPos           (double x,double y);
    bool            ZoomArea                (CMyRect rectLogicalArea);                                  // �Ŵ�һ������������Ļ
    bool            Zoom                    (CMyPoint *pPointCenter=NULL,bool bLogical=false);          // ��ĳ��Ϊ���ķŴ�ͬʱָ���Ƿ�Ϊ�߼���
    
    void            SetDrawRange            (   double dLeft,
                                                double dLow,
                                                double dRight,
                                                double dHigh, 
                                                float fXScale=1.0,
                                                float fYScale=1.0,
                                                bool bReset=false);

    void            SetScrollPage           (CMySize &sizePage);
    void            SetScrollLine           (CMySize &sizeLine);
    void            SetScrollLogPage        (CMySize &sizePage);
    void            SetScrollLogLine        (CMySize &sizeLine);

    CSize           GetClientPageSize       (CSize sizeClient);                                         // ��ÿͻ����Ĺ���ҳ��С

    double	        GetMyDevPosition        (int nBar, int nPhyPos);
    int		        GetPhyDevPosition       (int nBar, double nMyDevPos);


    virtual void    SearchPage              ();                                 // ����ĳһҳ
    virtual void    UpdatePage              ();                                 // ת��ĳһҳ
    bool            SetPageQty              (long nPageQty);                    // ������ҳ��
    long            GetCurrentPage          ();                                 // ��õ�ǰҳ��

    void            DrawBar                 (CDC *pDC);                         // ���ƹ���������
    void            DrawTitle               (CDC *pDC);                         // ����ͼͷ
    void            SetToolBarButtonState   ();                                // ���ݸ���״̬���ù�����������

    // For draw many coordinate systems, move the coordinate to upper, and then draw another.
    // Before, draw another, you must store the original logical coordinate,
    // After drawing the coordinates, you must restore the coordinate system.
    void            StoreCoorOri            ();
    void            RestoreCoorOri          ();
    void            MoveCoor                (double dMoveRight, double dMoveUp);

    HCURSOR         SetCurrentCursor        ();

    virtual void    WriteFontString         (       double x1,double y1,double x2,double y2,
                                                    CString sString,
                                                    CDC     *pDC=NULL,
                                                    UINT    position=0);

    CMyRect         GetCoorRect             ();

    virtual void    PreSettingDlg           ();
    virtual void    PostSettingDlg          ();
    virtual void    SavePars                ();

    CString         GetClassName            ();


protected:
    void            CenterOnPoint           (CMyPoint ptCenter);                                // ʹ�� ptCenter ����
    void            ScrollToDevicePosition  (CMyPoint ptDev);                                   // ��ʽ�ع�����ptDev
    void            UpdateBars              ();                                                 // ����������
    BOOL            GetTrueClientSize       (CSize& size, CSize& sizeSb);                       // ��ò������������Ŀͻ����Ĵ�С 
    void            GetScrollBarSizes       (CSize& sizeSb);                                    // ��ù������Ĵ�С
    
    void            GetScrollBarState       (   CSize sizeClient,                               // ��ù�������״̬
                                                CSize& needSb,
                                                CSize& sizeRange, 
                                                CMyPoint& ptMove, 
                                                BOOL bInsideClient);

    double          GetMyScrollPos          (int nBar);                                         // ��ȡ�ͻ�����ʼ�����������
    void            SetMyScrollPos          (int nBar,double dPos);                             // ���ÿͻ�����ʼ�����������
    double          GetMyScrollLimit        (int nBar);                                         // ��ȡ�������귶Χ

    BOOL            OnMyScroll              (UINT nScrollCode, double nPos, BOOL bDoScroll);    // �ͻ�������ʱ��OnScroll����  
    BOOL            OnMyScrollBy            (CMySize sizeScroll, BOOL bDoScroll);               // ����ʱ����,��OnScrollBy����


    virtual void    CalcWindowRect          (   LPRECT lpClientRect,
                                                UINT nAdjustType = adjustBorder);               // ��ô������ķ�Χ

    virtual CSize   GetWheelScrollDistance  (   CSize sizeDistance,                             //������ֹ��� 
                                                BOOL bHorz, 
                                                BOOL bVert);

    virtual BOOL    OnScroll                (   UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);       // �������������
    virtual BOOL    OnScrollBy              (   CSize sizeScroll, BOOL bDoScroll = TRUE);                // �������������

    // �������С�йصĺ���
    static CMySize sizeDefault;                                                                 // ��������Ĭ�ϵĹ���ҳ�͹����еĴ�С
    void            SetScaleToFitSize       (CMySize sizeTotal);                                // ���ÿͻ�����ʾȫ���߼���Χ
    CMySize         GetTotalSize            ();                                                 // ����߼����귶Χ
    void            CheckScrollBars         (BOOL& bHasHorzBar, BOOL& bHasVertBar) ;            // ����Ƿ����ݺ��������
    CMyPoint        GetDeviceScrollPosition ();                                                 // ����豸�Ĺ����������Ͻ�λ��
    
    void            GetDeviceScrollSizes    (   int& nMapMode,                                  // ����豸�Ĳ���
                                                CMySize& sizeTotal,
                                                CMySize & sizePage, 
                                                CMySize& sizeLine) const;

    void            ScrollToPosition        (CMyPoint pt);                                      // �趨���Ͻǵ��߼�����
    void            FillOutsideRect         (CDC* pDC, CBrush* pBrush);                         // �ѿͻ������豸��Χ���������Ϳ��
    void            ResizeParentToFit       (BOOL bShrinkOnly = TRUE);                          // ���踸���ڵķ�Χ���ʺϿͻ���
    BOOL            DoMouseWheel            (UINT fFlags, short zDelta, CPoint point);          // �����������¼�
    CMyPoint        GetMyScrollPos          (void);

    virtual bool    CheckPointingChange     (double x,double y,double &dMinDistance);


    bool            ShowJPG                 (   CDC *pDC,
                                                CString sFile,
                                                CRect rectTarget,
                                                bool bKeepSrcScale);

    virtual void    ToggleCoor              ();

    void            AddMenu                 (UINT MenuID, CString MenuString) ;

    void            InvalidateView          ();

private:
    bool            SetPopupMenu            ();

protected:

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    DECLARE_MESSAGE_MAP()

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CFHZoomView)
public:
    virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
    virtual void OnDraw(CDC* pDC);      // overridden to draw this view

protected:
    virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
    virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
    virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
    //}}AFX_VIRTUAL

    //{{AFX_MSG(CFHZoomView)

    //}}AFX_MSG
public:
    virtual void OnInitialUpdate();
    afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg BOOL OnMouseWheel(UINT fFlags, short zDelta, CPoint point);
    afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnFHEnableInfoWnd();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnFHZoomFull();
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnFHEnableCor();
    afx_msg void OnFHZoomInHor();
    afx_msg void OnFHZoomIdle();
    afx_msg void OnFHZoomInVer();
    afx_msg void OnFHZoomOutHor();
    afx_msg void OnFHZoomOutVer();
    afx_msg void OnFHZoomIn();
    afx_msg void OnFHZoomOut();
    afx_msg void OnFHZoomErrorMsg();
    afx_msg void OnFHZoomViewLast();
    afx_msg void OnFHZoomViewNext();
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnFHZoomViewExportBitmap();
    afx_msg void OnFHZoomInArea();
    afx_msg LRESULT HandleMButtonDown(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnWndChange(WPARAM wParam, LPARAM lParam);


protected:
    CSize GetPageSize(void);
public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnUpdateFhzoomviewlast(CCmdUI *pCmdUI);
    afx_msg void OnUpdateFhzoomviewnext(CCmdUI *pCmdUI);
    afx_msg void OnUpdateFhzoomsearch(CCmdUI *pCmdUI);
    afx_msg void OnFhzoomsearch();
    afx_msg void OnFhsettings();
    afx_msg void OnUpdateFhEnableInfoWnd(CCmdUI *pCmdUI);
    afx_msg void OnUpdateFhEnableCor(CCmdUI *pCmdUI);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};




//#define AddMenu(MenuID, MenuString) m_menuManager.AppendMenu(MenuID,MenuString, MF_MENUITEM);



