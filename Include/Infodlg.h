#if !defined(AFX_INFODLG_H__3B4C6064_B95B_11D3_AEF5_00C04FCCB957__INCLUDED_)
#define AFX_INFODLG_H__3B4C6064_B95B_11D3_AEF5_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InfoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInfoDlg dialog
#include "zoomview.h"

#include "FHString.h"
#include "MouseHook.h"

class  _FHZOOMVIEW_EXT_CLASS CInfoDlg : public CDialog
{
// Construction
protected:
	COLORREF m_nColorFore;
	COLORREF m_nColorBack;
	CWnd *m_pParent;

	CMouseHook m_hookMouse;

	CFont m_fontFore;

public:
	
	CInfoDlg(CWnd* pParent = NULL);   // standard constructor

	void Show(int nX,int nY,CString sInfo);
	void Show(CString sInfo);
	void SetBackColor(int nRed,int nGreen,int nBlue);
	void SetForeColor(int nRed,int nGreen,int nBlue);

	void SetMsg(CString sMsg,bool bRedraw=true);
	void SetControlWnd(CWnd *pEdgeWnd);
	void ControlPos(CWnd *pWnd=NULL, int cx=0,int cy=0);
    static CString GetName(CString sString);



	CString m_sInfo;
	CPoint m_pointOrg;
	bool m_bCanShowInfo; 
	HWND m_hWndCurrent;

	long m_nTest;

	// // Mouse drag the window:
	bool m_bAnchor;
	CPoint m_nAnchorMousePos;


// Dialog Data
	//{{AFX_DATA(CInfoDlg)
	enum { IDD = IDD_InfoDlg };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInfoDlg)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg LRESULT OnWndChange(WPARAM wParam, LPARAM lParam);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFODLG_H__3B4C6064_B95B_11D3_AEF5_00C04FCCB957__INCLUDED_)
