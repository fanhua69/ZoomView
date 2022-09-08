#pragma once
#include "afxwin.h"

#include "zoomview.h"
// CDlgPasswordAdder dialog

class _FHZOOMVIEW_EXT_CLASS CDlgPasswordAdder : public CDialog
{
	DECLARE_DYNAMIC(CDlgPasswordAdder)

public:
	CDlgPasswordAdder(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgPasswordAdder();

// Dialog Data
	enum { IDD = IDD_DlgPasswordAdder };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_sUserName;
	CString m_sPassword;
	CComboBox m_comboLevel;
	CString m_sUserLevel;
	CString m_sTip;
	
	enumUserLevel GetUserLevel();
	void SetTip(CString sTip);



	virtual BOOL OnInitDialog();
    CString m_sDateStart;
    CString m_sDateEnd;
};
#pragma once