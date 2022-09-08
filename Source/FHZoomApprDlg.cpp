// FHZoomApprDlg.cpp : implementation file
//

#include "stdafx.h"
#include "zoomview.h"
#include "FHZoomApprDlg.h"
#include "FHZoomView.h"
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CPointDrawPattern gPointDrawPattern[]=
{
    CPointDrawPattern(eDP_Ellipse,  "圆形"),
        CPointDrawPattern(eDP_Rectangle,"正方形"),
        CPointDrawPattern(eDP_Triangle, "三角形"),
        CPointDrawPattern(eDP_Cross,    "十字形"),
};

CFHZoomApprDlg::CFHZoomApprDlg(CWnd* pParent /*=NULL*/)
: CPropertyPage(CFHZoomApprDlg::IDD, 0,(DWORD)0)
{
    //{{AFX_DATA_INIT(CFHZoomApprDlg)
    m_iContentSize=0;
    m_iContentCurrSize=0;
    m_sTitle = _T("");
    m_bDrawCoor = FALSE;
    m_bDrawTitle = FALSE;
    m_bUseUserDefinedCoor = FALSE;
    m_nThickLineStepHor = 0.0;
    m_nThickLineEndHor = 0.0;
    m_nThickLineStartHor = 0.0;
    m_nThinLineEndHor = 0.0;
    m_nThinLineStartHor = 0.0;
    m_nThinLineStepHor = 0.0;

    m_nThickLineStepVer= 0.0;
    m_nThickLineEndVer = 0.0;
    m_nThickLineStartVer = 0.0;
    m_nThinLineEndVer = 0.0;
    m_nThinLineStartVer = 0.0;
    m_nThinLineStepVer = 0.0; 

    m_ePointPatternCurrent  =eDP_Ellipse;
    m_ePointPatternCommon   =eDP_Ellipse;


    //}}AFX_DATA_INIT
}


void CFHZoomApprDlg::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CFHZoomApprDlg)
    DDX_Text(pDX, IDC_EditTitle, m_sTitle);
    DDX_Check(pDX, IDC_CheckDrawCoor, m_bDrawCoor);
    DDX_Check(pDX, IDC_CheckDrawTitle, m_bDrawTitle);
    DDX_Check(pDX, IDC_CheckUseUserDefineCoor, m_bUseUserDefinedCoor);
    DDX_Text(pDX, IDC_nThickLineStepHor, m_nThickLineStepHor);
    DDX_Text(pDX, IDC_nThickLineEndHor, m_nThickLineEndHor);
    DDX_Text(pDX, IDC_nThickLineStartHor, m_nThickLineStartHor);
    DDX_Text(pDX, IDC_nThinLineEndHor, m_nThinLineEndHor);
    DDX_Text(pDX, IDC_nThinLineStartHor, m_nThinLineStartHor);
    DDX_Text(pDX, IDC_nThinLineStepHor, m_nThinLineStepHor);

    DDX_Text(pDX, IDC_nThickLineStepVer, m_nThickLineStepVer);
    DDX_Text(pDX, IDC_nThickLineEndVer, m_nThickLineEndVer);
    DDX_Text(pDX, IDC_nThickLineStartVer, m_nThickLineStartVer);
    DDX_Text(pDX, IDC_nThinLineEndVer, m_nThinLineEndVer);
    DDX_Text(pDX, IDC_nThinLineStartVer, m_nThinLineStartVer);
    DDX_Text(pDX, IDC_nThinLineStepVer, m_nThinLineStepVer);

    //}}AFX_DATA_MAP
    DDX_Text(pDX, IDC_editContentSize, m_iContentSize);
    DDV_MinMaxInt(pDX, m_iContentSize, 0, 100);
    DDX_Text(pDX, IDC_editContentCurrSize, m_iContentCurrSize);
    DDV_MinMaxInt(pDX, m_iContentCurrSize, 0, 100);
    DDX_Control(pDX, IDC_spinContentSize, m_spinContentSize);
    DDX_Control(pDX, IDC_spinContentCurrSize, m_spinContentCurrSize);
    DDX_Control(pDX, IDC_comboCommonPattern, m_comboCommonPattern);
    DDX_Control(pDX, IDC_comboCurrentPattern, m_comboCurrentPattern);
}


BEGIN_MESSAGE_MAP(CFHZoomApprDlg, CPropertyPage)
    //{{AFX_MSG_MAP(CFHZoomApprDlg)
    ON_BN_CLICKED(IDC_ButtonCoorFont, OnButtonCoorFont)
    ON_BN_CLICKED(IDC_ButtonMapCharFont, OnButtonMapCharFont)
    ON_BN_CLICKED(IDC_ButtonTitleFont, OnButtonTitleFont)
    ON_BN_CLICKED(IDC_CheckDrawCoor, OnCheckDrawCoor)
    ON_BN_CLICKED(IDC_CheckDrawTitle, OnCheckDrawTitle)
    ON_BN_CLICKED(IDC_ButtonCoorThickColorHor, OnButtonCoorThickColorHor)
    ON_BN_CLICKED(IDC_ButtonCoorThinColorHor, OnButtonCoorThinColorHor)

    ON_BN_CLICKED(IDC_ButtonDrawLineColor, OnButtonDrawLineColor)

    ON_BN_CLICKED(IDC_ButtonCoorThickColorVer, OnButtonCoorThickColorVer)
    ON_BN_CLICKED(IDC_ButtonCoorThinColorVer, OnButtonCoorThinColorVer)

    ON_BN_CLICKED(IDC_ButtonDrawCurrentLineColor, OnButtonDrawCurrentLineColor)
    ON_WM_PAINT()
    //}}AFX_MSG_MAP
    ON_CBN_SELCHANGE(IDC_comboCommonPattern, OnCbnSelchangecombocommonpattern)
    ON_CBN_SELCHANGE(IDC_comboCurrentPattern, OnCbnSelchangecombocurrentpattern)
    ON_BN_CLICKED(IDC_ButtonMapCharCurrFont, OnBnClickedButtonmapcharcurrfont)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFHZoomApprDlg message handlers

void CFHZoomApprDlg::OnButtonCoorFont() 
{
    // TODO: Add your control notification handler code here
    GetMyFont(m_logfontCoor,m_colorFontCoor);	
}


void CFHZoomApprDlg::OnButtonMapCharFont() 
{
    // TODO: Add your control notification handler code here
    GetMyFont(m_logfontMapChar,m_colorMapChar);
}

void CFHZoomApprDlg::OnButtonTitleFont() 
{
    // TODO: Add your control notification handler code here
    GetMyFont(m_logfontTitle,m_colorTitle);

}

void CFHZoomApprDlg::OnCheckDrawCoor() 
{
    // TODO: Add your control notification handler code here
    CButton* pButton=(CButton* )GetDlgItem(IDC_CheckDrawCoor);
    m_bDrawCoor=(pButton->GetCheck ()==0?false:true);

}

void CFHZoomApprDlg::OnCheckDrawTitle() 
{
    // TODO: Add your control notification handler code here
    CButton* pButton=(CButton* )GetDlgItem(IDC_CheckDrawTitle);
    m_bDrawTitle=(pButton->GetCheck ()==0?false:true);	
}

void CFHZoomApprDlg::OnButtonCoorThickColorHor() 
{
    // TODO: Add your control notification handler code here
    UpdateData(true);

    GetMyColor(m_colorThickCoorHor);	
    ::SetStaticRectColor(this,IDC_FrameThickLineColorHor,m_colorThickCoorHor);

    UpdateData(false);	
}

void CFHZoomApprDlg::OnButtonCoorThinColorHor() 
{
    // TODO: Add your control notification handler code here
    UpdateData(true);

    GetMyColor(m_colorThinCoorHor);

    ::SetStaticRectColor(this,IDC_FrameThinLineColorHor,m_colorThinCoorHor);

    UpdateData(false);
}



void CFHZoomApprDlg::OnButtonCoorThickColorVer() 
{
    // TODO: Add your control notification handler code here
    UpdateData(true);

    GetMyColor(m_colorThickCoorVer);	

    ::SetStaticRectColor(this,IDC_FrameThickLineColorVer,m_colorThickCoorVer);

    UpdateData(false);	


}

void CFHZoomApprDlg::OnButtonCoorThinColorVer() 
{
    // TODO: Add your control notification handler code here
    UpdateData(true);

    GetMyColor(m_colorThinCoorVer);	
    ::SetStaticRectColor(this,IDC_FrameThinLineColorVer,m_colorThinCoorVer);


    UpdateData(false);
}


BOOL CFHZoomApprDlg::OnInitDialog() 
{
    CPropertyPage::OnInitDialog();

    // TODO: Add extra initialization here
    CButton* pButton=(CButton* )GetDlgItem(IDC_CheckDrawTitle);
    pButton->SetCheck(m_bDrawTitle?1:0);

    pButton=(CButton* )GetDlgItem(IDC_CheckDrawCoor);
    pButton->SetCheck(m_bDrawCoor?1:0);

    m_spinContentSize.SetRange(0,100);
    m_spinContentCurrSize.SetRange(0,100);

    std::vector<CString> vNames=::GetAllPointDrawPatternNames();
    for(int i=0;i<vNames.size();i++)
    {
        m_comboCommonPattern.AddString(vNames.at(i));
        m_comboCurrentPattern.AddString(vNames.at(i));
    }

    CString sCurrName=::GetPointDrawPatternName(m_ePointPatternCurrent);
    m_comboCurrentPattern.SelectString(-1,sCurrName);
    CString sCommonName=::GetPointDrawPatternName(m_ePointPatternCommon);
    m_comboCommonPattern.SelectString(-1,sCommonName);


    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}



void CFHZoomApprDlg::OnButtonDrawLineColor() 
{
    // TODO: Add your control notification handler code here
    GetMyColor(m_colorMapContent);
    ::SetStaticRectColor(this,IDC_staticContentColor,m_colorMapContent);
}

void CFHZoomApprDlg::OnButtonDrawCurrentLineColor() 
{
    // TODO: Add your control notification handler code here
    GetMyColor(m_colorMapContentSpecial);
    ::SetStaticRectColor(this,IDC_staticContentCurrColor,m_colorMapContentSpecial);
}

void CFHZoomApprDlg::OnPaint() 
{
    CPaintDC dc(this); // device context for painting

    ::SetStaticRectColor(this,IDC_FrameThickLineColorHor,      m_colorThickCoorHor);
    ::SetStaticRectColor(this,IDC_FrameThinLineColorHor,       m_colorThinCoorHor);
    ::SetStaticRectColor(this,IDC_FrameThickLineColorVer,      m_colorThickCoorVer);
    ::SetStaticRectColor(this,IDC_FrameThinLineColorVer,       m_colorThinCoorVer);


    ::SetStaticRectColor(this,IDC_staticContentColor,          m_colorMapContent);
    ::SetStaticRectColor(this,IDC_staticContentCurrColor,      m_colorMapContentSpecial);

    return ;  // return TRUE unless you set the focus to a control

}

void CFHZoomApprDlg::OnCbnSelchangecombocommonpattern()
{
    int iOrder=m_comboCommonPattern.GetCurSel();
    CString sName;
    m_comboCommonPattern.GetLBText(iOrder,sName);
    m_ePointPatternCommon=::GetPointDrawPattern(sName);
}

void CFHZoomApprDlg::OnCbnSelchangecombocurrentpattern()
{
    int iOrder=m_comboCurrentPattern.GetCurSel();
    CString sName;
    m_comboCurrentPattern.GetLBText(iOrder,sName);

    m_ePointPatternCurrent=::GetPointDrawPattern(sName);
}

_FHZOOMVIEW_EXT_CLASS std::vector<CString> GetAllPointDrawPatternNames()
{
    std::vector<CString> vecNames;

    int i,iQty=sizeof(gPointDrawPattern)/sizeof(CPointDrawPattern);
    for(i=0;i<iQty;i++)
    {
        vecNames.push_back(gPointDrawPattern[i].m_sName);
    }

    return vecNames;
}

_FHZOOMVIEW_EXT_CLASS CString GetPointDrawPatternName (EPointDrawPattern  e)
{
    int i;
    int iQty=sizeof(gPointDrawPattern)/sizeof(CPointDrawPattern);
    for(i=0;i<iQty;i++)
    {
        if(gPointDrawPattern[i].m_eType==e)
        {
            return gPointDrawPattern[i].m_sName;
        }
    }
    return GetPointDrawPatternName(eDP_Ellipse);
}

_FHZOOMVIEW_EXT_CLASS EPointDrawPattern GetPointDrawPattern (CString sName)
{
    int i;
    int iQty=sizeof(gPointDrawPattern)/sizeof(CPointDrawPattern);
    for(i=0;i<iQty;i++)
    {
        if(gPointDrawPattern[i].m_sName==sName)
        {
            return gPointDrawPattern[i].m_eType;
        }
    }
    return eDP_Ellipse;
}
void CFHZoomApprDlg::OnBnClickedButtonmapcharcurrfont()
{
    // TODO: Add your control notification handler code here
    GetMyFont(m_logfontCurrChar,m_colorCurrChar);
}

/////////////////////////////////////////////////
//
/////////////////////////////////////////////////
void GetMyColor(COLORREF &color)
{
    CColorDialog dlg;
    dlg.m_cc.rgbResult =color;
    dlg.m_cc.Flags|=CC_RGBINIT ;

    if(dlg.DoModal()==IDOK)
    {
        color=dlg.m_cc.rgbResult ;
    }
}

void GetMyFont(LOGFONT &font,COLORREF &nColor)
{
    CFontDialog dlg;

    dlg.m_cf.lpLogFont=&font;
    dlg.m_cf.rgbColors =nColor;
    dlg.m_cf.Flags|=CF_INITTOLOGFONTSTRUCT ;
    if(dlg.DoModal()==IDCANCEL)return;

    dlg.GetCurrentFont (&font);
    nColor=dlg.GetColor();

}