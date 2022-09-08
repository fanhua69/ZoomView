// DlgXYZInfo.cpp : implementation file
//

#include "stdafx.h"
#include "DlgXYZInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgXYZInfo dialog


CDlgXYZInfo::CDlgXYZInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgXYZInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgXYZInfo)
	m_dAverEast = 0.0;
	m_dAverNorth = 0.0;
	m_dAverValue = 0.0;
	m_dMaxEast = 0.0;
	m_dMaxNorth = 0.0;
	m_dMaxValue = 0.0;
	m_dMinEast = 0.0;
	m_dMinNorth = 0.0;
	m_dMinValue = 0.0;
	m_nQuantity = 0;
	m_sOutFile = _T("");
	//}}AFX_DATA_INIT
}


void CDlgXYZInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgXYZInfo)
	DDX_Text(pDX, IDC_dAverEast, m_dAverEast);
	DDX_Text(pDX, IDC_dAverNorth, m_dAverNorth);
	DDX_Text(pDX, IDC_dAverValue, m_dAverValue);
	DDX_Text(pDX, IDC_dMaxEast, m_dMaxEast);
	DDX_Text(pDX, IDC_dMaxNorth, m_dMaxNorth);
	DDX_Text(pDX, IDC_dMaxValue, m_dMaxValue);
	DDX_Text(pDX, IDC_dMinEast, m_dMinEast);
	DDX_Text(pDX, IDC_dMinNorth, m_dMinNorth);
	DDX_Text(pDX, IDC_dMinValue, m_dMinValue);
	DDX_Text(pDX, IDC_nQuantity, m_nQuantity);
	DDX_Text(pDX, IDC_sOutFile, m_sOutFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgXYZInfo, CDialog)
	//{{AFX_MSG_MAP(CDlgXYZInfo)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgXYZInfo message handlers
