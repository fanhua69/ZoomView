// DlgEditViewSize.cpp : implementation file
//

#include "stdafx.h"
#include "zoomview.h"
#include "DlgEditViewSize.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEditViewSize dialog


CDlgEditViewSize::CDlgEditViewSize(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CDlgEditViewSize::IDD)
{
	//{{AFX_DATA_INIT(CDlgEditViewSize)
	m_nColNumber = 0;
	m_nRowNumber = 0;
	//}}AFX_DATA_INIT
}


void CDlgEditViewSize::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEditViewSize)
	DDX_Text(pDX, IDC_ColNumber, m_nColNumber);
	DDV_MinMaxLong(pDX, m_nColNumber, 2, 10000);
	DDX_Text(pDX, IDC_RowNumber, m_nRowNumber);
	DDV_MinMaxLong(pDX, m_nRowNumber, 2, 10000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEditViewSize, CPropertyPage)
	//{{AFX_MSG_MAP(CDlgEditViewSize)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEditViewSize message handlers
