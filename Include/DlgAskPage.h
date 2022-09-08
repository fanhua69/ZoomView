#pragma once
#include "afxcmn.h"


// CDlgAskPage dialog
#include "ZoomView.h"

class _FHZOOMVIEW_EXT_CLASS CDlgAskPage : public CDialog
{
	DECLARE_DYNAMIC(CDlgAskPage)

public:
	CDlgAskPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAskPage();

	double m_dCurrentValue;
	double m_dMinValue;
	double m_dMaxValue;


// Dialog Data
	enum { IDD = IDD_DlgAskPage };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	long m_nPageSelected;
	CSliderCtrl m_nCurrentPage;
	virtual BOOL OnInitDialog();
	long m_nPageQty;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
