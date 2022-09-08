// DlgDrawSNProperty.cpp : implementation file
//

#include "stdafx.h"
#include "zoomview.h"
#include "DlgMultiRgnViewProperty.h"
#include "fhapp.h"
#include "FHZoomView.h"
#include "MultiRgnBaseView.h"
#include "..\include\dlgmultirgnviewproperty.h"

// CDlgMultiRgnViewProperty dialog

IMPLEMENT_DYNAMIC(CDlgMultiRgnViewProperty, CPropertyPage)
CDlgMultiRgnViewProperty::CDlgMultiRgnViewProperty()
	: CPropertyPage(CDlgMultiRgnViewProperty::IDD)
{
    m_iDrawValueOrRatio     =   0;
    m_iDrawRectOrCurve      =   0;
    m_fHighDoor             =   1.3;
    m_fLowDoor              =   0.7;
    m_iSeriesSameColor      =   0;
    m_fCurrentColorBrighter =   20;
    m_iRefShowMethod        =   1;
    m_bHideRefShot          =   false;
    m_bHideDataType         =   false;
    m_eShowShotSortBy       =   eShowShotByDistance;
    m_bShowShotSortFixed    =   false;
    m_bRefShotCanBeOnePoint =   true;
}

CDlgMultiRgnViewProperty::~CDlgMultiRgnViewProperty()
{
}

void CDlgMultiRgnViewProperty::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Radio(pDX, IDC_RadioShowData, m_iDrawValueOrRatio);
    DDX_Radio(pDX, IDC_RadioDrawRect, m_iDrawRectOrCurve);
    DDX_Text(pDX, IDC_editSNHighDoor, m_fHighDoor);
    DDX_Text(pDX, IDC_editSNLowDoor, m_fLowDoor);
    DDX_Radio(pDX, IDC_RadioSeriesSameColor, m_iSeriesSameColor);
    DDX_Control(pDX, IDC_comboSeriesNames, m_comboSeriesNames);
    DDX_Control(pDX, IDC_staticSeriesColor, m_staticSeriesColor);
    DDX_Text(pDX, IDC_editCurrentColorBrighter, m_fCurrentColorBrighter);
    DDX_Control(pDX, IDC_editCurrentColorBrighter, m_editCurrentColorBrighter);
    DDX_Radio(pDX, IDC_radioRefPointType, m_iRefShowMethod);
    DDX_Control(pDX, IDC_RadioSortByShotOrder, m_radioSortByShotOrder);
}


BEGIN_MESSAGE_MAP(CDlgMultiRgnViewProperty, CPropertyPage)
    ON_BN_CLICKED(IDC_RadioSeriesSameColor, OnBnClickedRadioseriessamecolor)
    ON_BN_CLICKED(IDC_RadioSeriesDifferentColor, OnBnClickedRadioseriesdifferentcolor)
    ON_WM_PAINT()
    ON_CBN_SELCHANGE(IDC_comboSeriesNames, OnCbnSelchangecomboseriesnames)
    ON_BN_CLICKED(IDC_buttonSeriesColor, OnBnClickedbuttonseriescolor)
    ON_BN_CLICKED(IDC_RadioSortByShotOrder, OnBnClickedRadiosortbyshotorder)
    ON_BN_CLICKED(IDC_RadioSortByShotPoint, OnBnClickedRadiosortbyshotpoint)
    ON_BN_CLICKED(IDC_RadioSortByFileNumber, OnBnClickedRadiosortbyfilenumber)
    ON_BN_CLICKED(IDC_RadioSortByDistance, OnBnClickedRadiosortbydistance)
END_MESSAGE_MAP()


EDataDrawType CDlgMultiRgnViewProperty::GetDataType()
{
    if(m_iDrawValueOrRatio==0)
    {
        return eDDT_Data;
    }
    else if(m_iDrawValueOrRatio==1)
    {
        return eDDT_Ratio;
    }

    return eDDT_Data;
}

void CDlgMultiRgnViewProperty::SetShotShowOrderFixed(bool bFixed)
{
    m_bShowShotSortFixed    =   bFixed;
}

void CDlgMultiRgnViewProperty::SetRefShotCanBeOnePoint(bool bCanBeOnePoint)
{
    m_bRefShotCanBeOnePoint =   bCanBeOnePoint;
}

EMapDrawType CDlgMultiRgnViewProperty::GetMapType()
{
    if(m_iDrawRectOrCurve==1)
    {
        return eMDT_Curve;
    }
    else if(m_iDrawRectOrCurve==0)
    {
        return eMDT_Rectangle;
    }

    return eMDT_Curve;
}

void CDlgMultiRgnViewProperty::SetDataType(EDataDrawType e)
{
    if(e==eDDT_Data)
    {
        m_iDrawValueOrRatio=0;
    }
    else if(e==eDDT_Ratio)
    {
        m_iDrawValueOrRatio=1;
    }
    else
    {
        m_iDrawValueOrRatio=0;
    }
}


void CDlgMultiRgnViewProperty::SetMapType(EMapDrawType e)
{
    if(e==eMDT_Curve)
    {
        m_iDrawRectOrCurve=1;
    }
    else if(e==eMDT_Rectangle)
    {
        m_iDrawRectOrCurve=0;
    }
    else
    {
        m_iDrawValueOrRatio=0;
    }
}


float CDlgMultiRgnViewProperty::GetHighDoor()
{
    return m_fHighDoor;
}

float CDlgMultiRgnViewProperty::GetLowDoor()
{
    return m_fLowDoor;

}

void  CDlgMultiRgnViewProperty::SetHighDoor (float fHighDoor)
{
    m_fHighDoor=fHighDoor;
}

void  CDlgMultiRgnViewProperty::SetLowDoor  (float fLowDoor)
{
    m_fLowDoor=fLowDoor;
}
void CDlgMultiRgnViewProperty::OnBnClickedRadioseriessamecolor()
{
    m_comboSeriesNames.EnableWindow(false);
    m_staticSeriesColor.EnableWindow(false);
    m_editCurrentColorBrighter.EnableWindow(false);
}

void CDlgMultiRgnViewProperty::OnBnClickedRadioseriesdifferentcolor()
{
    m_comboSeriesNames.EnableWindow(true);
    m_staticSeriesColor.EnableWindow(true);
    m_editCurrentColorBrighter.EnableWindow(true);
}

void CDlgMultiRgnViewProperty::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    int iIndex=m_comboSeriesNames.GetCurSel();
    CString sSeriesName;
    m_comboSeriesNames.GetLBText(iIndex,sSeriesName);
    for(int i=0;i<m_vecSeriesColor.size();i++)
    {
        CSeriesColor &sc=m_vecSeriesColor.at(i);
        if(sc.m_sSeriesName==sSeriesName)
        {
            ::SetStaticRectColor(this, m_staticSeriesColor.GetDlgCtrlID(),sc.m_nColor);
            break;
        }
    }
}


void CDlgMultiRgnViewProperty::OnCbnSelchangecomboseriesnames()
{
    int iIndex=m_comboSeriesNames.GetCurSel();
    ::SetStaticRectColor(this, m_staticSeriesColor.GetDlgCtrlID(),m_vecSeriesColor[iIndex].m_nColor);
}

void CDlgMultiRgnViewProperty::OnBnClickedbuttonseriescolor()
{
    CColorDialog  dlg;
    if(dlg.DoModal()==IDCANCEL)
    {
        return ;
    }
    COLORREF iColor=dlg.GetColor();
    ::SetStaticRectColor(this, m_staticSeriesColor.GetDlgCtrlID(),iColor);
    int iIndex=m_comboSeriesNames.GetCurSel();
    CString sSeriesName;
    m_comboSeriesNames.GetLBText(iIndex,sSeriesName);
    for(int i=0;i<m_vecSeriesColor.size();i++)
    {
        CSeriesColor &sc=m_vecSeriesColor.at(i);
        if(sc.m_sSeriesName==sSeriesName)
        {
            sc.m_nColor=iColor;
            break;
        }
    }
}


void CDlgMultiRgnViewProperty::SetSeriesColor(std::vector<CSeriesColor> &vecSeriesColor)
{
    m_vecSeriesColor=vecSeriesColor;
}

std::vector<CSeriesColor> & CDlgMultiRgnViewProperty::GetSeriesColor()
{
    return m_vecSeriesColor;
}

BOOL CDlgMultiRgnViewProperty::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    int i=0;
    for(i=0;i<m_vecSeriesColor.size();i++)
    {
        m_comboSeriesNames.InsertString(-1,m_vecSeriesColor.at(i).m_sSeriesName);
    }
    m_comboSeriesNames.SetCurSel(0);

    if(m_bHideRefShot)
    {
        HideRefShot();
    }
    if(m_bHideDataType)
    {
        HideDataShowType();
    }

    ((CButton *)GetDlgItem(IDC_RadioSortByShotOrder))->SetCheck(m_eShowShotSortBy==eShowShotByOrder);
    ((CButton *)GetDlgItem(IDC_RadioSortByShotPoint))->SetCheck(m_eShowShotSortBy==eShowShotByPointNumber);
    ((CButton *)GetDlgItem(IDC_RadioSortByFileNumber))->SetCheck(m_eShowShotSortBy==eShowShotByFileNumber);
    ((CButton *)GetDlgItem(IDC_RadioSortByDistance))->SetCheck(m_eShowShotSortBy==eShowShotByDistance);

    if(m_bShowShotSortFixed)
    {
        ((CButton *)GetDlgItem(IDC_RadioSortByShotOrder))->EnableWindow(false);
        ((CButton *)GetDlgItem(IDC_RadioSortByShotPoint))->EnableWindow(false);
        ((CButton *)GetDlgItem(IDC_RadioSortByFileNumber))->EnableWindow(false);
        ((CButton *)GetDlgItem(IDC_RadioSortByDistance))->EnableWindow(false);
    }

    if(!m_bRefShotCanBeOnePoint )
    {
        ((CButton *)GetDlgItem(IDC_radioRefPointType2))->EnableWindow(false);
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMultiRgnViewProperty::SetHideRefShot(bool bHide)
{
    m_bHideRefShot=bHide;
}

void CDlgMultiRgnViewProperty::SetHideDataType(bool bHide)
{
    m_bHideDataType=bHide;
}

void CDlgMultiRgnViewProperty::HideRefShot()
{
    CWnd *pWnd=GetDlgItem(IDC_staticRefPointType);
    if(pWnd)
    {
        pWnd->ShowWindow(SW_HIDE);
    }    
    pWnd=GetDlgItem(IDC_radioRefPointType);
    if(pWnd)
    {
        pWnd->ShowWindow(SW_HIDE);
    }
    pWnd=GetDlgItem(IDC_radioRefPointType2);
    if(pWnd)
    {
        pWnd->ShowWindow(SW_HIDE);
    }
    pWnd=GetDlgItem(IDC_radioRefPointType3);
    if(pWnd)
    {
        pWnd->ShowWindow(SW_HIDE);
    }
}

void CDlgMultiRgnViewProperty::HideDataShowType()
{
    CWnd *pWnd=GetDlgItem(IDC_staticDataShowType);
    if(pWnd)
    {
        pWnd->EnableWindow(false);
    }  
    pWnd=GetDlgItem(IDC_RadioShowData);
    if(pWnd)
    {
        pWnd->EnableWindow(false);
    } 
    pWnd=GetDlgItem(IDC_RadioShowRatio);
    if(pWnd)
    {
        pWnd->EnableWindow(false);
    } 
}

void CDlgMultiRgnViewProperty::OnBnClickedRadiosortbyshotorder()
{
    m_eShowShotSortBy=eShowShotByOrder;
}

void CDlgMultiRgnViewProperty::OnBnClickedRadiosortbyshotpoint()
{
    m_eShowShotSortBy=eShowShotByPointNumber;
}

void CDlgMultiRgnViewProperty::OnBnClickedRadiosortbyfilenumber()
{
    m_eShowShotSortBy=eShowShotByFileNumber;
}

void CDlgMultiRgnViewProperty::OnBnClickedRadiosortbydistance()
{
    m_eShowShotSortBy=eShowShotByDistance;
}

void CDlgMultiRgnViewProperty::SetShotShowOrder(EShowShotSortBy eOrder)
{
    m_eShowShotSortBy=eOrder;
}

EShowShotSortBy CDlgMultiRgnViewProperty::GetShotShowOrder()
{
    return m_eShowShotSortBy;
}

void CDlgMultiRgnViewProperty::SetRefShowType(EShowRefMethod eRefShowType)
{
    m_iRefShowMethod=(int)eRefShowType;
}



EShowRefMethod CDlgMultiRgnViewProperty::GetRefShowType()
{
    return (EShowRefMethod)m_iRefShowMethod;
}
