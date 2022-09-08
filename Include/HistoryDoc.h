#pragma once


// CHistoryDoc document
#include "fheditdoc.h"
#include "historyview.h"
#include "historymanager.h"
#include "zoomview.h"


class _FHZOOMVIEW_EXT_CLASS CHistoryDoc : public CFHEditDoc
{
    friend class CHistoryView;

	DECLARE_DYNCREATE(CHistoryDoc)


public:
	CHistoryDoc();
	virtual ~CHistoryDoc();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual BOOL OnNewDocument();

	CHistoryManager     m_history;
    CString             m_sHistoryFile;

public:
    CHistory  &         GetHistory      (long nIndex);
    int                 GetHistoryQty   ();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
};
