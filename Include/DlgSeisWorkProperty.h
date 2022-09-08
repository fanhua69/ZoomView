#pragma once

#include "zoomview.h"

#include "QCReportWriter.h"

class _FHZOOMVIEW_EXT_CLASS CDlgSeisWorkProperty : public CPropertyPage
{
	DECLARE_DYNAMIC(CDlgSeisWorkProperty)

public:
	CDlgSeisWorkProperty(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSeisWorkProperty();

// Dialog Data
	enum { IDD = IDD_DlgSeisWorkProperty };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	CString m_sProject;
	CString m_sArea;
	CString m_sClient;
	CString m_sContractor;
	CString m_sLineName;
	CString m_sCrew;

public:

	CCheckHeadBase m_head;

	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
};
