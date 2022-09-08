

#pragma once
#include "afxwin.h"
#include "Zoomview.h"

enum EDataDrawType;
enum EMapDrawType;

class CSeriesColor;

enum EShowShotSortBy
{
    eShowShotByOrder        =0,
    eShowShotByPointNumber  =1,
    eShowShotByFileNumber   =2,
    eShowShotByDistance     =3,
};

enum EShowRefMethod
{
    eRefAsSeries            =0,
    eRefAsPoint             =1,
    eRefNotShow             =2,
};


#include <vector>

// CDlgMultiRgnViewProperty dialog

class _FHZOOMVIEW_EXT_CLASS CDlgMultiRgnViewProperty : public CPropertyPage
{
    DECLARE_DYNAMIC(CDlgMultiRgnViewProperty)

public:
                    CDlgMultiRgnViewProperty    ();
    virtual         ~CDlgMultiRgnViewProperty   ();

    void            SetDataType             (EDataDrawType  e);
    void            SetMapType              (EMapDrawType   e);
    EDataDrawType   GetDataType             ();
    EMapDrawType    GetMapType              ();
    float           GetHighDoor             ();
    float           GetLowDoor              ();
    void            SetHighDoor             (float fHighDoor);
    void            SetLowDoor              (float fLowDoor);

    void            SetSeriesColor          (std::vector<CSeriesColor> &vecSeriesColor);
    std::vector<CSeriesColor> & GetSeriesColor();

    void            SetHideRefShot          (bool bHide);
    void            SetHideDataType         (bool bHide);
    void            SetShotShowOrder        (EShowShotSortBy eOrder);
    EShowShotSortBy GetShotShowOrder        ();

    void            SetRefShowType          (EShowRefMethod eRefShowType);
    EShowRefMethod  GetRefShowType          ();

    void            SetShotShowOrderFixed   (   bool bFixed );
    void            SetRefShotCanBeOnePoint (   bool bCanBeOnePoint );

    std::vector<CSeriesColor> m_vecSeriesColor;
    int         m_iSeriesSameColor;
    float       m_fCurrentColorBrighter;


    // Dialog Data
    enum { IDD = IDD_DlgMultiRgnViewProperty };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    void        HideRefShot();
    void        HideDataShowType();

    bool        m_bHideRefShot;
    bool        m_bHideDataType;


    DECLARE_MESSAGE_MAP()

protected:
    int         m_iDrawValueOrRatio;
    int         m_iDrawRectOrCurve;
    float       m_fHighDoor;
    float       m_fLowDoor;
    CComboBox   m_comboSeriesNames;
    CStatic     m_staticSeriesColor;
    CEdit       m_editCurrentColorBrighter;
    int         m_iRefShowMethod;
    EShowShotSortBy m_eShowShotSortBy;
    bool        m_bShowShotSortFixed;
    bool        m_bRefShotCanBeOnePoint;


public:
    afx_msg void OnBnClickedRadioseriessamecolor();
    afx_msg void OnBnClickedRadioseriesdifferentcolor();
    afx_msg void OnPaint();
    afx_msg void OnCbnSelchangecomboseriesnames();
    afx_msg void OnBnClickedbuttonseriescolor();
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedRadiosortbyshotorder();
    afx_msg void OnBnClickedRadiosortbyshotpoint();
    afx_msg void OnBnClickedRadiosortbyfilenumber();
    CButton m_radioSortByShotOrder;
    afx_msg void OnBnClickedRadiosortbydistance();
};
