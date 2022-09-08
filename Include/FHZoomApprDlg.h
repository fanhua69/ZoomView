#if !defined(AFX_FHZOOMAPPRDLG_H__1F28D4C4_C907_11D3_AEF5_00C04FCCB957__INCLUDED_)
#define AFX_FHZOOMAPPRDLG_H__1F28D4C4_C907_11D3_AEF5_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FHZoomApprDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFHZoomApprDlg dialog

#include "zoomview.h"
#include "afxcmn.h"
#include "afxwin.h"
#include <vector>

enum _FHZOOMVIEW_EXT_CLASS EPointDrawPattern
{
    eDP_Ellipse,
    eDP_Rectangle,
    eDP_Triangle,
    eDP_Cross,
};

class _FHZOOMVIEW_EXT_CLASS CPointDrawPattern
{
public:
    EPointDrawPattern   m_eType;
    CString             m_sName;

    CPointDrawPattern(EPointDrawPattern e, CString sName)
    {
        m_eType=e;
        m_sName=sName;
    };
};

_FHZOOMVIEW_EXT_CLASS std::vector<CString>  GetAllPointDrawPatternNames();  
_FHZOOMVIEW_EXT_CLASS CString               GetPointDrawPatternName (EPointDrawPattern  e);
_FHZOOMVIEW_EXT_CLASS EPointDrawPattern     GetPointDrawPattern     (CString sName);


class CFHZoomApprDlg : public CPropertyPage
{
// Construction
public:
	CFHZoomApprDlg(CWnd* pParent = NULL);   // standard constructor

// Attributes:
public:
	LOGFONT     m_logfontTitle;
	COLORREF    m_colorTitle;

	LOGFONT     m_logfontCoor;
	COLORREF    m_colorFontCoor;
	COLORREF    m_colorThickCoorHor;
	COLORREF    m_colorThinCoorHor;
	COLORREF    m_colorThickCoorVer;
	COLORREF    m_colorThinCoorVer;

	LOGFONT     m_logfontMapChar;
	COLORREF    m_colorMapChar;

    LOGFONT     m_logfontCurrChar;
    COLORREF    m_colorCurrChar;


	COLORREF    m_colorMapContent;
	COLORREF    m_colorMapContentSpecial;
    int         m_iContentSize;
    int         m_iContentCurrSize;

// Dialog Data
	//{{AFX_DATA(CFHZoomApprDlg)
	enum { IDD = IDD_FHZoomAppearance };
	CString	m_sTitle;
	BOOL	m_bDrawCoor;
	BOOL	m_bDrawTitle;
	BOOL	m_bUseUserDefinedCoor;
	double	m_nThickLineStepHor;
	double	m_nThickLineEndHor;
	double	m_nThickLineStartHor;
	double	m_nThinLineEndHor;
	double	m_nThinLineStartHor;
	double	m_nThinLineStepHor;

	double	m_nThickLineStepVer;
	double	m_nThickLineEndVer;
	double	m_nThickLineStartVer;
	double	m_nThinLineEndVer;
	double	m_nThinLineStartVer;
	double	m_nThinLineStepVer;

    EPointDrawPattern m_ePointPatternCommon;
    EPointDrawPattern m_ePointPatternCurrent;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFHZoomApprDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CFHZoomApprDlg)
	afx_msg void OnButtonCoorFont();
	afx_msg void OnButtonMapCharFont();
	afx_msg void OnButtonTitleFont();
	afx_msg void OnCheckDrawCoor();
	afx_msg void OnCheckDrawTitle();
	afx_msg void OnButtonCoorThickColorHor();
	afx_msg void OnButtonCoorThinColorHor();
	afx_msg void OnButtonCoorThickColorVer();
	afx_msg void OnButtonCoorThinColorVer();	
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonDrawLineColor();
	afx_msg void OnButtonDrawCurrentLineColor();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
    CSpinButtonCtrl m_spinContentSize;
    CSpinButtonCtrl m_spinContentCurrSize;
    CComboBox m_comboCommonPattern;
    CComboBox m_comboCurrentPattern;
    afx_msg void OnCbnSelchangecombocommonpattern();
    afx_msg void OnCbnSelchangecombocurrentpattern();
    afx_msg void OnBnClickedButtonmapcharcurrfont();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FHZOOMAPPRDLG_H__1F28D4C4_C907_11D3_AEF5_00C04FCCB957__INCLUDED_)



void _FHZOOMVIEW_EXT_CLASS GetMyFont(LOGFONT &font,COLORREF &nColor);
void _FHZOOMVIEW_EXT_CLASS GetMyColor(COLORREF &color);

