#if !defined(AFX_DLGPASSWORD_H__12D41DC1_FF09_4836_B702_773C4B5A2C45__INCLUDED_)
#define AFX_DLGPASSWORD_H__12D41DC1_FF09_4836_B702_773C4B5A2C45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPassword.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPassword dialog

#include "afxwin.h"
#include "zoomview.h"

class  _FHZOOMVIEW_EXT_CLASS  CDlgPassword : public CDialog
{
// Construction
public:
	CDlgPassword(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgPassword)
	enum { IDD = IDD_DlgPassword };
	CString	m_sAnswer;
	CString m_sUserName;
	CString m_sTip;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPassword)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:


	// Generated message map functions
	//{{AFX_MSG(CDlgPassword)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	virtual void OnOK();
	
public:	
	void SetTip(CString sTip);

	CStatic m_staticTip;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPASSWORD_H__12D41DC1_FF09_4836_B702_773C4B5A2C45__INCLUDED_)
