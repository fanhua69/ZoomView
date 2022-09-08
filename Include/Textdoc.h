#if !defined(AFX_TEXTDOC_H__9AC1ECC8_23A7_4207_905A_6ED6FE52B678__INCLUDED_)
#define AFX_TEXTDOC_H__9AC1ECC8_23A7_4207_905A_6ED6FE52B678__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTextDoc document

#include "zoomview.h"

class _FHZOOMVIEW_EXT_CLASS CTextDoc : public CDocument
{
protected:
	CTextDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTextDoc)

// Attributes
public:
	CString m_sFileName;

	void SetModifiedFlag(BOOL bModified);

// Operations
public:
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTextDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CTextDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTDOC_H__9AC1ECC8_23A7_4207_905A_6ED6FE52B678__INCLUDED_)
