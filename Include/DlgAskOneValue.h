#if !defined(AFX_DLGASKONEVALUE_H__92001B81_D27A_11D3_AEF5_00C04FCCB957__INCLUDED_)
#define AFX_DLGASKONEVALUE_H__92001B81_D27A_11D3_AEF5_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAskOneValue.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAskOneValue dialog
#include "ZoomView.h"

class  _FHZOOMVIEW_EXT_CLASS  CDlgAskOneValue : public CDialog
{
// Construction
public:
	CDlgAskOneValue(CWnd* pParent = NULL);   // standard constructor

    void    SetTitle(CString sTitle);
    void    SetAsk  (CString sAsk);
    void    SetValue(CString sValue);
    void    SetValue(float fValue);
    void    SetValue(double dValue);
    void    SetValue(int nValue);
    void    SetValue(long nValue);

    CString GetValue();
    void    GetValue(float &fValue);
    void    GetValue(double &dValue);
    void    GetValue(int &nValue);
    void    GetValue(long &nValue);


protected:
	CString m_sTitle;

// Dialog Data
	//{{AFX_DATA(CDlgAskOneValue)
	enum { IDD = IDD_AskOneValue };
	CString	m_sAsk;
	CString	m_sValue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAskOneValue)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAskOneValue)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGASKONEVALUE_H__92001B81_D27A_11D3_AEF5_00C04FCCB957__INCLUDED_)
