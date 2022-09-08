// DlgAskOneValue.cpp : implementation file
//

#include "stdafx.h"
#include "DlgAskOneValue.h"
#include "FHCGlobal.h"
#include "..\include\dlgaskonevalue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAskOneValue dialog


CDlgAskOneValue::CDlgAskOneValue(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAskOneValue::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAskOneValue)
	m_sAsk = _T("");
	m_sValue = _T("");
	//}}AFX_DATA_INIT
}


void CDlgAskOneValue::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAskOneValue)
	DDX_Text(pDX, IDC_sAsk, m_sAsk);
	DDX_Text(pDX, IDC_sValue, m_sValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAskOneValue, CDialog)
	//{{AFX_MSG_MAP(CDlgAskOneValue)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAskOneValue message handlers

BOOL CDlgAskOneValue::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetWindowText(m_sTitle);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAskOneValue::SetTitle(CString sTitle)
{
    m_sTitle = sTitle;
}

void CDlgAskOneValue::SetAsk(CString sAsk)
{
    m_sAsk=sAsk;
    if(m_sAsk.GetAt(m_sAsk.GetLength()-1)!=':')
    {
        m_sAsk+=":";
    }
}

void CDlgAskOneValue::SetValue(CString sValue)
{
    m_sValue = sValue;
}

void CDlgAskOneValue::SetValue(float fValue)
{
    m_sValue = vtos(fValue);
}

void CDlgAskOneValue::SetValue(double dValue)
{
    m_sValue = vtos(dValue);
}

void CDlgAskOneValue::SetValue(int nValue)
{
    m_sValue = vtos(nValue);
}

void CDlgAskOneValue::SetValue(long nValue)
{
    m_sValue = vtos(nValue);
}

CString CDlgAskOneValue::GetValue()
{
    return m_sValue;
}

void CDlgAskOneValue::GetValue(float &fValue)
{
    sscanf((LPCTSTR)m_sValue,"%f",&fValue);
}

void CDlgAskOneValue::GetValue(double &dValue)
{
    sscanf((LPCTSTR)m_sValue,"%lf",&dValue);
}

void CDlgAskOneValue::GetValue(int &nValue)
{
    sscanf((LPCTSTR)m_sValue,"%d",&nValue);
}

void CDlgAskOneValue::GetValue(long &nValue)
{
    sscanf((LPCTSTR)m_sValue,"%d",&nValue);
}


