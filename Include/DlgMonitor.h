// ProgDlg.h : header file
// CG: This file was added by the Progress Dialog component

/////////////////////////////////////////////////////////////////////////////
// CDlgMonitor dialog

#ifndef DLG_MONITOR
#define DLG_MONITOR

#include "zoomview.h"

class  _FHZOOMVIEW_EXT_CLASS  CDlgMonitor : public CDialog
{
// Construction / Destruction
public:
    CDlgMonitor(CString sTitle="");
    ~CDlgMonitor();

    BOOL Create(CWnd *pParent=NULL);

    // Checking for Cancel button
    BOOL CheckCancelButton();

	void SetMonitor(double *pValue,int nValue);
    // Progress Dialog manipulation
    void SetStatus(LPCTSTR lpszMessage);
    void SetRange(double nLower,double nUpper);
    int  SetStep(double nStep);
    int  SetPos(double dPos);
    int  OffsetPos(int nPos);
    int  StepIt();

// Dialog Data
    //{{AFX_DATA(CDlgMonitor)
	enum { IDD = IDD_ProgressMonitor };
	CProgressCtrl	m_Progress;
	UINT	m_nEnd;
	UINT	m_nStart;
	CString	m_sMonitor;
	//}}AFX_DATA

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgMonitor)
    public:
    virtual BOOL DestroyWindow();
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
	CString m_sTitle;
    double m_dLower;
    double m_dUpper;
    double m_dStep;
    
    BOOL m_bCancel;
    BOOL m_bParentDisabled;

    void ReEnableParent();

    virtual void OnCancel();
    virtual void OnOK() {}; 
    void UpdatePercent(int nCurrent);
    void PumpMessages();

    // Generated message map functions
    //{{AFX_MSG(CDlgMonitor)
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#endif // __PROGDLG_H__
