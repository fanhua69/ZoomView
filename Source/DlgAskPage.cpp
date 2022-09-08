// DlgAskPage.cpp : implementation file
//

#include "stdafx.h"
#include "ZoomView.h"
#include "DlgAskPage.h"


// CDlgAskPage dialog

IMPLEMENT_DYNAMIC(CDlgAskPage, CDialog)
CDlgAskPage::CDlgAskPage(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAskPage::IDD, pParent)
	
{
	m_dCurrentValue=0;
	m_nPageQty=0;

	m_dMinValue=0;
	m_dMaxValue=100;
	m_nPageSelected=0;
}

CDlgAskPage::~CDlgAskPage()
{
}

void CDlgAskPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_nPageSelected, m_nPageSelected);
	DDX_Control(pDX, IDC_SliderCurrentPage, m_nCurrentPage);
	DDX_Text(pDX, IDC_nPageQty, m_nPageQty);
}


BEGIN_MESSAGE_MAP(CDlgAskPage, CDialog)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


BOOL CDlgAskPage::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_nPageQty=m_dMaxValue;

	CSliderCtrl * pSlider=(CSliderCtrl * )GetDlgItem(IDC_SliderCurrentPage);
	pSlider->SetRange (m_dMinValue,m_dMaxValue);

	UpdateData(false);



	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAskPage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);

	//////////////////////////////////
	//
	if(pScrollBar->GetDlgCtrlID()==IDC_SliderCurrentPage)
	{
		CSliderCtrl * pSlider=(CSliderCtrl * )GetDlgItem(IDC_SliderCurrentPage);
		m_nPageSelected=pSlider->GetPos ();
		UpdateData(false);
	}
}
