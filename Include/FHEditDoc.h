#if !defined(AFX_FHEDITDOC_H__258FC1E1_DCA1_11D3_ADA7_947FE171841B__INCLUDED_)
#define AFX_FHEDITDOC_H__258FC1E1_DCA1_11D3_ADA7_947FE171841B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FHEditDoc.h : header file
//
#include "zoomview.h"

class CFHEditView;
#include "FHBaseDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CFHEditDoc document
//
class _FHZOOMVIEW_EXT_CLASS  CFHEditDoc : public CFHBaseDoc
{
	friend class CFHEditView;
public:
	CFHEditDoc();           // protected constructor used by dynamic creation
    virtual         ~CFHEditDoc();

	DECLARE_DYNCREATE(CFHEditDoc)

// Attributes
public:
	CString m_sArea;
	CString m_sLine;

	long    m_nRecordNumber;

protected:
    bool    m_bUseSerialize;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFHEditDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	long            GetRecordNumber ();
	virtual bool    SetRecordNumber (long       nRecordNumber);
	virtual bool    SaveFile        (CString    sFile);
	virtual bool    ReadFile        (CString    sFile);
    virtual CString GetArea         ();
    virtual void    SetArea         (CString    sArea);
            void    SetUseSerialize (bool       bUseSerialize=true);


	        void    UpdateDoc       ();

	        void    SetModifiedFlag (BOOL bModified=true);

#ifdef _DEBUG
	virtual void    AssertValid     () const;
	virtual void    Dump            (CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CFHEditDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual void    OnCloseDocument();
	virtual BOOL    OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL    OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL    SaveModified();

	void            OnFileSave();
	void            OnFileSaveAs();	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FHEDITDOC_H__258FC1E1_DCA1_11D3_ADA7_947FE171841B__INCLUDED_)
