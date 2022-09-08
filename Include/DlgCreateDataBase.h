#pragma once


// CDlgCreateDataBase dialog
#include "zoomview.h"

bool _FHZOOMVIEW_EXT_CLASS CreateAllDataBases();
CString _FHZOOMVIEW_EXT_CLASS GetChosenFolder(char sDiscription[],HWND hWnd);


class CDlgCreateDataBase : public CDialog
{
	DECLARE_DYNAMIC(CDlgCreateDataBase)

public:
	CDlgCreateDataBase(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgCreateDataBase();

// Dialog Data
	enum { IDD = IDD_DlgNewDataBase };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_sPath;
	CString m_sArea1;
	CString m_sArea2;
	CString m_sArea3;
	afx_msg void OnBnClickedButtonpath();
};
