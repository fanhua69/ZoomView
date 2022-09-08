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
    statueIdle,           // 空闲
    statueZoomInArea,     // 区域放大
    statueZoomIn,         // 放大
    statueZoomOut,        // 缩小
    statueZoomInHor,      // 横向放大
    statueZoomOutHor,     // 横向缩小
    statueZoomInVer,      // 垂向放大  
    statueZoomOutVer,     // 垂向缩小
    statueZoomHVSame,     // 纵横比一致
};

// 与坐标系有关的参数
enum enumZoomCoorType 
{
    CT_NULL=0,
    CT_NET,
    CT_XY,
    CT_XYY
};

//标尺单位
enum enumZoomUnitType
{
    ULT_ENGLISH = 0,     //英寸
    ULT_METRIC,          //厘米
    ULT_PIXEL,           //像素
    ULT_SCALETOFIT        //合适大小
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
    CMouseAnchorWnd     *   m_pAnchorWindow;                // 处理滚轮鼠标抛锚信息


    CMouseHook              m_hookMouse;

    CMultiDocTemplate   *   m_pTemplate;

    EZoomViewCoorStyle      m_eCoorStyle;


protected:
    //
    CToolBar *  m_pZoomViewToolBar;

    // 与标题有关的参数
    bool        m_bDrawTitle;
    LOGFONT     m_logfontTitle;
    COLORREF    m_colorTitle;
    CString     m_sDrawTitle;
    CString     m_sError;

    // 与打印有关的参数
    bool        m_bPrintByScale;
    CMySize     m_totalDevBak; // 打印之前存储设备的大小

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

    // 与图中
    LOGFONT     m_logfontMapChar;
    COLORREF    m_colorMapChar;
    LOGFONT     m_logfontCurrChar;
    COLORREF    m_colorCurrChar;

    CFont       m_fontTextCommon;
    CFont       m_fontTextCurrent;



    int         m_nFont;                // =90
    CFont       * m_pFontTilt;          // limit:90

    // 与信息窗有关的函数
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
    // 与错误信息窗有关的函数
    ///////////////////////////////////
    CInfoDlg    m_dlgErrorMsg;
    bool        m_bErrorCanShowBak;
    bool        m_nFirstShow;

    ///////////////////////////////////
    // 为继承类预设图形颜色：
    ///////////////////////////////////
    COLORREF    m_colorMapContent;
    COLORREF    m_colorMapContentSpecial;

    ///////////////////////////////////
    // 滚动标尺的字体
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

    enumZoomStatue   m_eZoomState;                            // 放大标识
    enumZoomUnitType m_nMapMode;                            // 映射方式

    int         m_nBarWidth;
    int         m_nEdgeWidth;

    CMyPoint    m_pointLogOrgBak;                           // Used for saving m_pointLogOrg while drawing many coordinates.
    CMyPoint    m_pointLogOrg;                              // 逻辑坐标原点
    CMyPoint    m_pointDevOrg;                              // 客户区的开始点的物理坐标
    CMySize     m_totalLog;                                 // 逻辑坐标大小
    CMySize     m_totalDev;                                 // 物理坐标大小，它与逻辑坐标大小的比值决定了放大系数。
    CMySize     m_pageDev;                                  // 滚动页的大小
    CMySize     m_lineDev;                                  // 滚动行的大小
    CSize       m_sizeScrollBar;
    bool        m_bDevSizeLimit;                            // 是否限制设备大小
    BOOL        m_bCenter;                                  // 窗口居中
    BOOL        m_bInsideUpdate;                            // OnSize的内部回访状态

    double      m_dCooeHor;                                 // 水平方向的坐标缩放系数
    double      m_dCooeVer;                                 // 垂直方向的坐标缩放系数

    bool        m_bUpIncrease;                              // 向上是否为正
    CMyRect     m_rectCoor;

    ///////////////////////////////////
    // 与上一项、下一项有关的变量
    ///////////////////////////////////
    long        m_nCurrentPage;
    long        m_nPageQty;

    ///////////////////////////////////
    // 用于参数存储
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
    // 函数
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
    bool            Line                    (   CDC *pDC,double x1,double y1,double x2,double y2);          // 画线
    void            ZoomFull                (   );                                                          // 放大全部内容到整个屏幕
    static CString  GetValueString          (   double dValue,double dStep);                                // 根据双精度数据的大小给出一个合适的字符串
    void            HideSearchButton        (   bool bHide=true);                                           // 隐藏工具栏上的“搜索”按钮
    void            SetPrintByScale         (   bool bByScale=true);                                        // 设定打印比例，是否按照原始数据进行纵横比1:1的打印	
    void            GetLogicalRange         (   double &x1,double &y1,double &x2,double &y2);               // 获得逻辑范围
    bool            SetMidScaleDensity      (   long n);                                                    // 设定细比例线的密度。
    enumZoomStatue  GetZoomStatue           (   );                                                          // 获得放大状态
    void            SetToolBar              (   CToolBar *pBar);                                            // 设定工具条
    void            SetDirection            (   bool bUpIncrease);                                          // 设定显示方向，向上数值增大为真
    bool            GetDirection            (   );
    void            SetDrawTitle            (   CString sTitle);                                            // 设定图头字符串
    void            SetCoorRange            (   double dLeft,double dBottom,double dRight,double dTop);     // 设定坐标范围
    virtual void    DrawCoor                (   CDC     *pDC);                                              // 绘制坐标

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

    void            SetTiltFont             (LOGFONT *pLogFont=NULL,CDC *pDC=NULL);                     // 设定画面显示字体
    void            GetVisibleArea          (double &x1,double &y1,double &x2,double &y2);              // 获得逻辑上的显示范围
    virtual CString GetInfo                 (double x,double y);                                        // 获得用于显示的用户信息
    void            CalcScrollPos           (double x,double y);
    bool            ZoomArea                (CMyRect rectLogicalArea);                                  // 放大一个区域到整个屏幕
    bool            Zoom                    (CMyPoint *pPointCenter=NULL,bool bLogical=false);          // 以某点为中心放大，同时指定是否为逻辑点
    
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

    CSize           GetClientPageSize       (CSize sizeClient);                                         // 获得客户区的滚动页大小

    double	        GetMyDevPosition        (int nBar, int nPhyPos);
    int		        GetPhyDevPosition       (int nBar, double nMyDevPos);


    virtual void    SearchPage              ();                                 // 查找某一页
    virtual void    UpdatePage              ();                                 // 转到某一页
    bool            SetPageQty              (long nPageQty);                    // 设置总页数
    long            GetCurrentPage          ();                                 // 获得当前页数

    void            DrawBar                 (CDC *pDC);                         // 绘制滚动坐标条
    void            DrawTitle               (CDC *pDC);                         // 绘制图头
    void            SetToolBarButtonState   ();                                // 根据各种状态设置工具条的起、落

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
    void            CenterOnPoint           (CMyPoint ptCenter);                                // 使点 ptCenter 居中
    void            ScrollToDevicePosition  (CMyPoint ptDev);                                   // 显式地滚动到ptDev
    void            UpdateBars              ();                                                 // 调整滚动条
    BOOL            GetTrueClientSize       (CSize& size, CSize& sizeSb);                       // 获得不包含滚动条的客户区的大小 
    void            GetScrollBarSizes       (CSize& sizeSb);                                    // 获得滚动条的大小
    
    void            GetScrollBarState       (   CSize sizeClient,                               // 获得滚动条的状态
                                                CSize& needSb,
                                                CSize& sizeRange, 
                                                CMyPoint& ptMove, 
                                                BOOL bInsideClient);

    double          GetMyScrollPos          (int nBar);                                         // 获取客户区开始点的物理坐标
    void            SetMyScrollPos          (int nBar,double dPos);                             // 设置客户区开始点的物理坐标
    double          GetMyScrollLimit        (int nBar);                                         // 获取物理坐标范围

    BOOL            OnMyScroll              (UINT nScrollCode, double nPos, BOOL bDoScroll);    // 客户区滚动时由OnScroll调用  
    BOOL            OnMyScrollBy            (CMySize sizeScroll, BOOL bDoScroll);               // 滚动时调用,由OnScrollBy调用


    virtual void    CalcWindowRect          (   LPRECT lpClientRect,
                                                UINT nAdjustType = adjustBorder);               // 获得窗口区的范围

    virtual CSize   GetWheelScrollDistance  (   CSize sizeDistance,                             //处理滚轮滚动 
                                                BOOL bHorz, 
                                                BOOL bVert);

    virtual BOOL    OnScroll                (   UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);       // 处理滚动条滚动
    virtual BOOL    OnScrollBy              (   CSize sizeScroll, BOOL bDoScroll = TRUE);                // 处理滚动条滚动

    // 与滚动大小有关的函数
    static CMySize sizeDefault;                                                                 // 用来设置默认的滚动页和滚动行的大小
    void            SetScaleToFitSize       (CMySize sizeTotal);                                // 设置客户区显示全部逻辑范围
    CMySize         GetTotalSize            ();                                                 // 获得逻辑坐标范围
    void            CheckScrollBars         (BOOL& bHasHorzBar, BOOL& bHasVertBar) ;            // 检查是否含有纵横向滚动条
    CMyPoint        GetDeviceScrollPosition ();                                                 // 获得设备的滚动窗的左上角位置
    
    void            GetDeviceScrollSizes    (   int& nMapMode,                                  // 获得设备的参数
                                                CMySize& sizeTotal,
                                                CMySize & sizePage, 
                                                CMySize& sizeLine) const;

    void            ScrollToPosition        (CMyPoint pt);                                      // 设定左上角的逻辑坐标
    void            FillOutsideRect         (CDC* pDC, CBrush* pBrush);                         // 把客户区内设备范围以外的区域涂黑
    void            ResizeParentToFit       (BOOL bShrinkOnly = TRUE);                          // 重设父窗口的范围以适合客户区
    BOOL            DoMouseWheel            (UINT fFlags, short zDelta, CPoint point);          // 处理鼠标滚轮事件
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



