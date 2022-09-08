// DlgPassword.cpp : implementation file
//

#include "stdafx.h"
#include "DlgPassword.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPassword dialog


CDlgPassword::CDlgPassword(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPassword::IDD, pParent)
	, m_sTip(_T(""))
{
	//{{AFX_DATA_INIT(CDlgPassword)
	m_sAnswer = _T("");
	//}}AFX_DATA_INIT
	m_sUserName=(_T(""));
	m_sTip="";
}


void CDlgPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPassword)
	DDX_Text(pDX, IDC_sAnswer, m_sAnswer);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_sUserName, m_sUserName);
	DDX_Text(pDX, IDC_sTip, m_sTip);	
}


BEGIN_MESSAGE_MAP(CDlgPassword, CDialog)
	//{{AFX_MSG_MAP(CDlgPassword)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPassword message handlers

void CDlgPassword::OnOK()
{
	CDialog::OnOK();
}

void CDlgPassword::SetTip(CString sTip)
{
	m_sTip=sTip;
}