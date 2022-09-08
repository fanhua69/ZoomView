#pragma once

#include "FHZoomView.h"
#include "zoomview.h"
#include <vector>
#include "DlgMultiRgnViewProperty.h"

class CSeriesColor
{
public:
    CString     m_sSeriesName;
    COLORREF    m_nColor;

    CSeriesColor(CString sName, COLORREF iColor)
    {
        m_sSeriesName   =   sName;
        m_nColor        =   iColor;
    };
};

class _FHZOOMVIEW_EXT_CLASS CMultiRgnBaseView : public CFHZoomView
{
    DECLARE_DYNCREATE(CMultiRgnBaseView)

protected:
    CMultiRgnBaseView();           // protected constructor used by dynamic creation
    virtual ~CMultiRgnBaseView();

public:
#ifdef _DEBUG
    virtual void        AssertValid() const;
    virtual void        Dump(CDumpContext& dc) const;
#endif

protected:

    virtual void        OnDraw(CDC* pDC);      // overridden to draw this view
    virtual void        DrawRgn(CDC *pDC, int iRgnOrder);

    bool                m_bBaseBeZero;

    double              m_dMinValue;
    double              m_dMaxValue;

    std::vector<double> m_vecMinRgnValue;
    std::vector<double> m_vecMaxRgnValue;
    std::vector<float>  m_vecRgnValueRatio;

    int                 m_iSeriesQty;
    int                 m_iRgnQty;
    int                 m_iPointQty;

    int                 m_iCurrentPoint;
    int                 m_iCurrentRgn;
    int                 m_iCurrentSeries;

    bool                m_bApplyValueRatio;

    CPen                m_penCommon;
    CPen                m_penCurrent;
    CBrush              m_brushCommon;
    CBrush              m_brushCurrent;

    EDataDrawType       m_eDataType;
    EMapDrawType        m_eMapType;
    double              m_dRatioDoorHigh;
    double              m_dRatioDoorLow;

    bool                m_bSeriesSameColor;

    float               m_fCurrentColorBrighter;

    EShowRefMethod      m_eShowRefMethod; // 0, as a series, 1, as a point, 2 not shown

    bool                m_bDrawValue;

    bool                m_bShowShotSortFixed;
    bool                m_bRefShotCanBeOnePoint;

    EShowShotSortBy     m_eShowShotSortBy;


    std::vector<CPen *>     m_vecPenSeries;
    std::vector<CBrush*>    m_vecBrushSeries;
    std::vector<CPen *>     m_vecPenSeriesCurrent;
    std::vector<CBrush*>    m_vecBrushSeriesCurrent;

    void                CreatePens();
    void                ClearPens();


    virtual int         GetPointQty         ();                                 // must override
    virtual int         GetRgnQty           ();                                 // must override
    virtual int         GetSeriesQty        ();

    virtual CString     GetRgnName          (int iRgnOrder);                    // must override
    virtual double      GetCoor             (int iRgnOrder, int iPointOrder, int iSeries);   // must override
    virtual void        GetCoorNote         (CStringList &listNotes, int iRgnOrder, int iPointOrder, int iSeries);
    virtual double      GetValue            (int iRgnOrder, int iPointOrder, int iSeries);   // must override

    double              GetRgnMinValue      (int iRgn);
    double              GetRgnMaxValue      (int iRgn);
    void                DrawCoor            (CDC *pDC, int iRgnOrder);

    float               GetRgnValueRatio    (int iRgnOrder);

    float               CalcRgnValueRatio   (int iRgnOrder);
    virtual void        CalcMostValue       ();
    virtual void        SetDrawProperty     ();

    virtual bool        CheckPointingChange (double x,double y,double &dMinDistance);
    virtual bool        CheckPointingRgn    (double x,double y);
    virtual bool        CheckPointingPoint  (double x,double y);
    virtual bool        CheckPointingSeries (double x,double y);

    virtual void        DrawValueCurve      (CDC *pDC, int iRgnOrder);
    virtual void        DrawValueRect       (CDC *pDC, int iRgnOrder);
    virtual void        DrawRatioLimitLine  (CDC *pDC, int iRgnOrder);
    virtual void        DrawSeriesName      (CDC *pDC, int iRgnOrder);

    virtual CString     GetSeriesName       (int iOrder  );

    virtual void        PreSettingDlg       ();
    virtual void        PostSettingDlg      ();

    virtual CPen    *   GetCommonPen        ();
    virtual CPen    *   GetCurrentPen       ();
    virtual CBrush  *   GetCommonBrush      ();
    virtual CBrush  *   GetCurrentBrush     ();

    virtual void        DrawPoint(          CDC *pDC, 
                                            int iSeries,
                                            int iRgnOrder,
                                            int iPoint);

    void                OnUpdate        (   CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
    void                OnContextMenu   (   CWnd* pWnd, CPoint point) ;
    void                LPtoWorld       (   double &x, double &y, int iRgnOrder);
    void                GetCoorRange    (   double &dLeft,  double &dRight);
    void                GetValueRange   (   double &dLow,   double &dHigh);


    float               GetValueRatio   (   double dValue, double dMaxValue);
    double              GetMinPointDis  (   int iRgnOrder,int iSeries);
    bool                IsCurrentPoint  (   int iSeries, 
                                            int iRgnOrder,
                                            int iPointOrder);

    virtual COLORREF    GetSeriesCurrentColor(  int         iSeriesOrder, 
                                                COLORREF    colorSeries);

    virtual COLORREF    GetSeriesColor  (   int iSeriesOrder);
    void                SetSeriesColor  (   CString sName, COLORREF iColor);

public:

    DECLARE_MESSAGE_MAP()
    virtual void OnInitialUpdate();
};


