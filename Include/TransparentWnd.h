#pragma once


#include "MyRgn.h"

class CTransparentWnd : public CDialog
{
	DECLARE_DYNAMIC(CTransparentWnd)

public:
	CTransparentWnd(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTransparentWnd();

// Dialog Data
	enum { IDD = IDD_InfoDlg };

protected:
	CRgn m_rgn;
	COLORREF m_nBkColor;

public:
	void SetRgn(CMyRgn *pRgn,CPoint pointOff);
	void SetBackColor(COLORREF nColor);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
