//  ProgDlg.cpp : implementation file
// CG: This file was added by the Progress Dialog component

#include "stdafx.h"
#include "zoomview.h"
#include "DlgMonitor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMonitor dialog

CDlgMonitor::CDlgMonitor(CString sTitle)
{
	m_sTitle=sTitle;
	if (m_sTitle=="")
		m_sTitle= "正在处理，请稍侯...";

    m_bCancel=FALSE;
    m_dLower=0;
    m_dUpper=100;
    m_dStep=10;
    //{{AFX_DATA_INIT(CDlgMonitor)
	m_nEnd = 0;
	m_nStart = 0;
	m_sMonitor = _T("");
	//}}AFX_DATA_INIT
    m_bParentDisabled = FALSE;

}

CDlgMonitor::~CDlgMonitor()
{
    if(m_hWnd!=NULL)
      DestroyWindow();
}

BOOL CDlgMonitor::DestroyWindow()
{
    ReEnableParent();
    return CDialog::DestroyWindow();
}

void CDlgMonitor::ReEnableParent()
{
    if(m_bParentDisabled && (m_pParentWnd!=NULL))
      m_pParentWnd->EnableWindow(TRUE);
    m_bParentDisabled=FALSE;
}

BOOL CDlgMonitor::Create(CWnd *pParent)
{
    // Get the true parent of the dialog
    m_pParentWnd = CWnd::GetSafeOwner(pParent);

    // m_bParentDisabled is used to re-enable the parent window
    // when the dialog is destroyed. So we don't want to set
    // it to TRUE unless the parent was already enabled.

    if((m_pParentWnd!=NULL) && m_pParentWnd->IsWindowEnabled())
    {
      m_pParentWnd->EnableWindow(FALSE);
      m_bParentDisabled = TRUE;
    }

    if(!CDialog::Create(CDlgMonitor::IDD,pParent))
    {
      ReEnableParent();
      return FALSE;
    }
	
	m_Progress.SetRange(0,100);

    return TRUE;
}

void CDlgMonitor::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgMonitor)
	DDX_Control(pDX, IDC_MPROGRESS, m_Progress);
	DDX_Text(pDX, IDC_nEnd, m_nEnd);
	DDX_Text(pDX, IDC_nStart, m_nStart);
	DDX_Text(pDX, IDC_Monitor, m_sMonitor);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgMonitor, CDialog)
    //{{AFX_MSG_MAP(CDlgMonitor)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDlgMonitor::SetStatus(LPCTSTR lpszMessage)
{
    ASSERT(m_hWnd); // Don't call this _before_ the dialog has
                    // been created. Can be called from OnInitDialog
    CWnd *pWndStatus = GetDlgItem(IDC_MPROGRESS);

    // Verify that the static text control exists
    ASSERT(pWndStatus!=NULL);
    pWndStatus->SetWindowText(lpszMessage);
}

void CDlgMonitor::OnCancel()
{
    m_bCancel=TRUE;
}

void CDlgMonitor::SetRange(double nLower,double nUpper)
{
    m_dLower = nLower;
    m_dUpper = nUpper;
}
  
int CDlgMonitor::SetPos(double dPos)
{

    PumpMessages();
	int nPos=(dPos-m_dLower )/(m_dUpper - m_dLower)*100;
	if(nPos<0)nPos=0;
	if(nPos>100)nPos=100;

    // Since the Progress Control wraps, we will wrap the percentage
    // along with it. However, don't reset 100% back to 0%
    if(nPos>100.0){
		nPos= nPos -long(nPos/100.0)*100;
	}

    int iResult = m_Progress.SetPos(nPos);
    UpdatePercent(nPos);
    return iResult;
}

int CDlgMonitor::SetStep(double nStep)
{
    m_dStep = nStep; // Store for later use in calculating percentage
    return m_Progress.SetStep(nStep);
}

int CDlgMonitor::OffsetPos(int nPos)
{
    PumpMessages();
    int iResult = m_Progress.OffsetPos(nPos);
    UpdatePercent(iResult+nPos);
    return iResult;
}

int CDlgMonitor::StepIt()
{
    PumpMessages();
    int iResult = m_Progress.StepIt();
    UpdatePercent(iResult+m_dStep);
    return iResult;
}

void CDlgMonitor::PumpMessages()
{
    // Must call Create() before using the dialog
    ASSERT(m_hWnd!=NULL);

    MSG msg;
    // Handle dialog messages
    while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if(!IsDialogMessage(&msg))
      {
        TranslateMessage(&msg);
        DispatchMessage(&msg);  
      }
    }
}

BOOL CDlgMonitor::CheckCancelButton()
{
    // Process all pending messages
    PumpMessages();

    // Reset m_bCancel to FALSE so that
    // CheckCancelButton returns FALSE until the user
    // clicks Cancel again. This will allow you to call
    // CheckCancelButton and still continue the operation.
    // If m_bCancel stayed TRUE, then the next call to
    // CheckCancelButton would always return TRUE

    BOOL bResult = m_bCancel;
    m_bCancel = FALSE;

    return bResult;
}

void CDlgMonitor::UpdatePercent(int dPos)
{
    CWnd *pWndPercent = GetDlgItem(IDC_MPERCENT);

    // Display the percentage
    CString strBuf;
    strBuf.Format(_T("%d%c"),(int)dPos,_T('%'));

	CString strCur; // get current percentage
    pWndPercent->GetWindowText(strCur);

	if (strCur != strBuf)
		pWndPercent->SetWindowText(strBuf);
}
    
/////////////////////////////////////////////////////////////////////////////
// CDlgMonitor message handlers

BOOL CDlgMonitor::OnInitDialog() 
{
    CDialog::OnInitDialog();
    m_Progress.SetRange(m_dLower,m_dUpper);
    m_Progress.SetStep(m_dStep);
    m_Progress.SetPos(m_dLower);
	
    SetWindowText(m_sTitle);

    return TRUE;  
}

void CDlgMonitor ::SetMonitor(double *pValue,int nValue)
{
	CListBox *pList=(CListBox*)GetDlgItem(IDC_Monitor);
	
	m_sMonitor="";
	CString sRow;
	for(int i=0;i<nValue;i++){
		sRow.Format("%10.5lf\r\n",pValue[i]);
		m_sMonitor+=sRow;
	}

	UpdateData(false);
	UpdateWindow();

	MSG msg;
	PeekMessage( &msg,this->m_hWnd,0, 10000000,PM_REMOVE);
	TranslateMessage(&msg);
	DispatchMessage(&msg);
	
}
