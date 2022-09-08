// DlgTreeProperty.cpp : implementation file
//

#include "stdafx.h"
#include "DlgTreeProperty.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTreeProperty dialog


CDlgTreeProperty::CDlgTreeProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTreeProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTreeProperty)
	m_nAreaNumber = 0;
	m_nLineNumber = 0;
	m_nTotalLineNumber = 0;
	m_sAreaCurrent = _T("");
	//}}AFX_DATA_INIT
}


void CDlgTreeProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTreeProperty)
	DDX_Text(pDX, IDC_nAreaNumber, m_nAreaNumber);
	DDX_Text(pDX, IDC_nLineNumber, m_nLineNumber);
	DDX_Text(pDX, IDC_nTotalLineNumber, m_nTotalLineNumber);
	DDX_Text(pDX, IDC_sAreaCurrent, m_sAreaCurrent);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTreeProperty, CDialog)
	//{{AFX_MSG_MAP(CDlgTreeProperty)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTreeProperty message handlers
