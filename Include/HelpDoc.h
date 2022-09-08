#if !defined(AFX_HELPDOC_H__D8CD0DC4_27E8_11D4_A4E5_00C04FCCB957__INCLUDED_)
#define AFX_HELPDOC_H__D8CD0DC4_27E8_11D4_A4E5_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HelpDoc.h : header file
//
#include "zoomview.h"

/////////////////////////////////////////////////////////////////////////////
// CHelpDoc document

class _FHZOOMVIEW_EXT_CLASS CHelpDoc : public CDocument
{
protected:
	CHelpDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CHelpDoc)

// Attributes
public:
	CString m_sOpenedFile;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHelpDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHelpDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CHelpDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HELPDOC_H__D8CD0DC4_27E8_11D4_A4E5_00C04FCCB957__INCLUDED_)
