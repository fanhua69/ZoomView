// .\source\DlgSeisWorkProperty.cpp : implementation file
//

#include "stdafx.h"
#include "zoomview.h"
#include "DlgSeisWorkProperty.h"
#include "FHApp.h"
#include "dlgseisworkproperty.h"
#include "..\include\dlgseisworkproperty.h"

// CDlgSeisWorkProperty dialog
IMPLEMENT_DYNAMIC(CDlgSeisWorkProperty, CPropertyPage)

CDlgSeisWorkProperty::CDlgSeisWorkProperty(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CDlgSeisWorkProperty::IDD)
{
}

CDlgSeisWorkProperty::~CDlgSeisWorkProperty()
{
}

void CDlgSeisWorkProperty::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Project,		m_sProject);
	DDX_Text(pDX, IDC_Area,			m_sArea);
	DDX_Text(pDX, IDC_Client,		m_sClient);
	DDX_Text(pDX, IDC_Contractor,	m_sContractor);
	DDX_Text(pDX, IDC_LineName,		m_sLineName);
	DDX_Text(pDX, IDC_Crew,			m_sCrew);
	DDX_Text(pDX, IDC_ProjectYear,	m_head.m_iProjectYear);
	DDX_Text(pDX, IDC_WorkYear,		m_head.m_iWorkYear);
	DDX_Text(pDX, IDC_WorkMonth,	m_head.m_iWorkMonth);
	DDX_Text(pDX, IDC_WorkDay,		m_head.m_iWorkDay);
	DDX_Text(pDX, IDC_TestYear,		m_head.m_iTestYear);
	DDX_Text(pDX, IDC_TestMonth,	m_head.m_iTestMonth);
	DDX_Text(pDX, IDC_TestDay,		m_head.m_iTestDay);
	DDX_Text(pDX, IDC_TestHour,		m_head.m_iTestHour);
	DDX_Text(pDX, IDC_TestMinute,	m_head.m_iTestMinute);
	DDX_Text(pDX, IDC_TestSecond,	m_head.m_iTestSecond);
}


BEGIN_MESSAGE_MAP(CDlgSeisWorkProperty, CPropertyPage)
END_MESSAGE_MAP()

// CDlgSeisWorkProperty message handlers

BOOL CDlgSeisWorkProperty::OnInitDialog()
{
	m_sProject		=m_head.m_sProject;
	m_sArea			=m_head.m_sArea;
	m_sClient		=m_head.m_sClient;
	m_sContractor	=m_head.m_sContractor;
	m_sLineName		=m_head.m_sLineName;
	m_sCrew			=m_head.m_sCrew;

	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSeisWorkProperty::OnOK()
{
	UpdateData(true);

	// TODO: Add your specialized code here and/or call the base class
	strcpy(m_head.m_sProject,(LPCTSTR)m_sProject);
	strcpy(m_head.m_sArea,(LPCTSTR)m_sArea);
	strcpy(m_head.m_sClient,(LPCTSTR)m_sClient);
	strcpy(m_head.m_sContractor,(LPCTSTR)m_sContractor);
	strcpy(m_head.m_sLineName,(LPCTSTR)m_sLineName);
	strcpy(m_head.m_sCrew,(LPCTSTR)m_sCrew);

	CPropertyPage::OnOK();
}

