// DlgBackGroundPic.cpp : implementation file
//

#include "stdafx.h"
#include "zoomview.h"
#include "DlgBackGroundPic.h"
#include "..\include\dlgbackgroundpic.h"
#include "FHCGlobal.h"
#include "FHApp.h"


// CDlgBackGroundPic dialog

IMPLEMENT_DYNAMIC(CDlgBackGroundPic, CPropertyPage)
CDlgBackGroundPic::CDlgBackGroundPic()
	: CPropertyPage(CDlgBackGroundPic::IDD)
    , m_sFileName(_T(""))
    , m_dWestCoor(0)
    , m_dEastCoor(0)
    , m_dSouthCoor(0)
    , m_dNorthCoor(0)
    , m_bKeepRatio(FALSE)
{
}

CDlgBackGroundPic::~CDlgBackGroundPic()
{
}

void CDlgBackGroundPic::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_editFileName, m_sFileName);
    DDX_Text(pDX, IDC_editWestCoor, m_dWestCoor);
    DDX_Text(pDX, IDC_editEastCoor, m_dEastCoor);
    DDX_Text(pDX, IDC_editSouthCoor, m_dSouthCoor);
    DDX_Text(pDX, IDC_editNorthCoor, m_dNorthCoor);
    DDX_Check(pDX, IDC_checkKeepRatio, m_bKeepRatio);
}


BEGIN_MESSAGE_MAP(CDlgBackGroundPic, CPropertyPage)
    ON_BN_CLICKED(IDC_buttonFileName, OnBnClickedbuttonfilename)
END_MESSAGE_MAP()


// CDlgBackGroundPic message handlers

void CDlgBackGroundPic::OnBnClickedbuttonfilename()
{
    CString sFile=::GetFileForOpen("±³¾°Í¼Æ¬ÎÄ¼þ","jpg");
    if(sFile=="")
    {
        return;
    }

    m_sFileName=sFile;

    ::GetPar("BACK_GROUND_"+m_sFileName+"_WEST",m_dWestCoor);
    ::GetPar("BACK_GROUND_"+m_sFileName+"_EAST",m_dEastCoor);
    ::GetPar("BACK_GROUND_"+m_sFileName+"_SOUTH",m_dSouthCoor);
    ::GetPar("BACK_GROUND_"+m_sFileName+"_NORTH",m_dNorthCoor);
    ::GetPar("BACK_GROUND_"+m_sFileName+"_KEEPRATIO",m_bKeepRatio);

    UpdateData(false);

}

BOOL CDlgBackGroundPic::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    // TODO:  Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgBackGroundPic::OnApply()
{
    // TODO: Add your specialized code here and/or call the base class

    return CPropertyPage::OnApply();
}

void CDlgBackGroundPic::OnOK()
{
    // TODO: Add your specialized code here and/or call the base class

    CPropertyPage::OnOK();
}
