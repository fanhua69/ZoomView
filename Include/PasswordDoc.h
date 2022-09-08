#pragma once


// CPasswordDoc document
#include "fheditdoc.h"
#include "passwordview.h"
#include "passwordmanager.h"
#include "Zoomview.h"

class _FHZOOMVIEW_EXT_CLASS CPasswordDoc : public CFHEditDoc
{
	DECLARE_DYNCREATE(CPasswordDoc)

	friend class CPasswordView;

public:
	CPasswordDoc();
	virtual ~CPasswordDoc();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual BOOL OnNewDocument();

	CPasswordManager    m_password;
    CString             m_sPasswordFile;

public:
    CPasswordManager    m_manager;
    CPassword *         GetPassword(long nIndex);



	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
};
