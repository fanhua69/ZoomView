#if !defined(AFX_HELPVIEW_H__D8CD0DC5_27E8_11D4_A4E5_00C04FCCB957__INCLUDED_)
#define AFX_HELPVIEW_H__D8CD0DC5_27E8_11D4_A4E5_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HelpView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHelpView html view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include <afxhtml.h>

#include "zoomview.h"

class _FHZOOMVIEW_EXT_CLASS CHelpView : public CHtmlView
{
protected:
	CHelpView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CHelpView)

// html Data
public:
	//{{AFX_DATA(CHelpView)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHelpView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CHelpView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CHelpView)
	afx_msg void OnViewFontsLargest();
	afx_msg void OnViewFontsLarge();
	afx_msg void OnViewFontsMedium();
	afx_msg void OnViewFontsSmall();
	afx_msg void OnViewFontsSmallest();
	afx_msg void OnGoBack();
	afx_msg void OnGoForward();
	afx_msg void OnGoSearch();
	afx_msg void OnGoStartPage();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HELPVIEW_H__D8CD0DC5_27E8_11D4_A4E5_00C04FCCB957__INCLUDED_)
