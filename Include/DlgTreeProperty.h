#if !defined(AFX_DLGTREEPROPERTY_H__0A9D0A61_3CB8_11D4_96C1_00C04FCCB957__INCLUDED_)
#define AFX_DLGTREEPROPERTY_H__0A9D0A61_3CB8_11D4_96C1_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTreeProperty.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTreeProperty dialog

#include "zoomview.h"
#include "zoomview.h"

class CDlgTreeProperty : public CDialog
{
// Construction
public:
	CDlgTreeProperty(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgTreeProperty)
	enum { IDD = IDD_DlgTreeProperty };
	UINT	m_nAreaNumber;
	UINT	m_nLineNumber;
	UINT	m_nTotalLineNumber;
	CString	m_sAreaCurrent;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTreeProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTreeProperty)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTREEPROPERTY_H__0A9D0A61_3CB8_11D4_96C1_00C04FCCB957__INCLUDED_)
