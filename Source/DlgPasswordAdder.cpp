// DlgPasswordAdder.cpp : implementation file
//

#include "stdafx.h"
#include "zoomview.h"
#include "PasswordManager.h"
#include "DlgPasswordAdder.h"


// CDlgPasswordAdder dialog

IMPLEMENT_DYNAMIC(CDlgPasswordAdder, CDialog)
CDlgPasswordAdder::CDlgPasswordAdder(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPasswordAdder::IDD, pParent)
	, m_sUserName(_T(""))
	, m_sPassword(_T(""))
	, m_sUserLevel(_T(""))
	, m_sTip(_T(""))
    , m_sDateStart(_T(""))
    , m_sDateEnd(_T(""))
{
	m_sUserLevel="CommonUser";
    m_sTip=" ‰»Î–¬ø⁄¡Ó";

    CTime t;
    t=CTime::GetCurrentTime();
    CMyDate dateStart(t.GetYear(),t.GetMonth(),t.GetDay());
    m_sDateStart=dateStart.GetDateString();
    
    CMyDate dateEnd(t.GetYear()+1,t.GetMonth(),t.GetDay());
    m_sDateEnd=dateEnd.GetDateString();
}

CDlgPasswordAdder::~CDlgPasswordAdder()
{
}

void CDlgPasswordAdder::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_sUserName, m_sUserName);
    DDX_Text(pDX, IDC_sAnswer, m_sPassword);
    DDX_Control(pDX, IDC_comboUserLevel, m_comboLevel);
    DDX_CBString(pDX, IDC_comboUserLevel, m_sUserLevel);
    DDX_Text(pDX, IDC_sTip, m_sTip);
    DDX_Text(pDX, IDC_sDateStart, m_sDateStart);
    DDX_Text(pDX, IDC_sDateEnd, m_sDateEnd);
}


BEGIN_MESSAGE_MAP(CDlgPasswordAdder, CDialog)
END_MESSAGE_MAP()


// CDlgPasswordAdder message handlers

BOOL CDlgPasswordAdder::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_comboLevel.AddString ("Administrator");
	m_comboLevel.AddString ("Super User");
	m_comboLevel.AddString ("Common User");

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

enumUserLevel CDlgPasswordAdder::GetUserLevel()
{
	if(m_sUserLevel=="Administrator")
	{
		return USER_ADMINISTRATOR;
	}
	else if(m_sUserLevel=="Super User")
	{
		return USER_SUPERUSER;
	}
	else if(m_sUserLevel=="Common User")
	{
		return USER_COMMONUSER;
	}
	else
	{
		return USER_COMMONUSER;
	}
}
void CDlgPasswordAdder::SetTip(CString sTip)
{
	m_sTip=sTip;
}