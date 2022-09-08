#if !defined(AFX_DLGXYZINFO_H__C10407C1_EA26_11D3_AEF5_00C04FCCB957__INCLUDED_)
#define AFX_DLGXYZINFO_H__C10407C1_EA26_11D3_AEF5_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgXYZInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgXYZInfo dialog
#include "ZoomView.h"

class _FHZOOMVIEW_EXT_CLASS CDlgXYZInfo : public CDialog
{
// Construction
public:
	CDlgXYZInfo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgXYZInfo)
	enum { IDD = IDD_DlgXYZInfo };
	double	m_dAverEast;
	double	m_dAverNorth;
	double	m_dAverValue;
	double	m_dMaxEast;
	double	m_dMaxNorth;
	double	m_dMaxValue;
	double	m_dMinEast;
	double	m_dMinNorth;
	double	m_dMinValue;
	long	m_nQuantity;
	CString	m_sOutFile;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgXYZInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgXYZInfo)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGXYZINFO_H__C10407C1_EA26_11D3_AEF5_00C04FCCB957__INCLUDED_)
