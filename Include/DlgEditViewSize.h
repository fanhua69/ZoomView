#if !defined(AFX_DLGEDITVIEWSIZE_H__03243941_0499_11D4_BA7C_E42ABE5DB451__INCLUDED_)
#define AFX_DLGEDITVIEWSIZE_H__03243941_0499_11D4_BA7C_E42ABE5DB451__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEditViewSize.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgEditViewSize dialog
#include "zoomview.h"

class CDlgEditViewSize : public CPropertyPage
{
// Construction
public:
	CDlgEditViewSize(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgEditViewSize)
	enum { IDD = IDD_FHEditViewSetSize };
	long	m_nColNumber;
	long	m_nRowNumber;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEditViewSize)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgEditViewSize)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEDITVIEWSIZE_H__03243941_0499_11D4_BA7C_E42ABE5DB451__INCLUDED_)
