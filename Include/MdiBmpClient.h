

#pragma once

#include "stdafx.h"
#include "zoomview.h"


//
class  _FHZOOMVIEW_EXT_CLASS CMdiBmpClient : public CWnd
{
	DECLARE_DYNCREATE(CMdiBmpClient)

// Construction
public:
	CMdiBmpClient();
	enum enumBkType
	{
		BK_TILE,
		BK_STRETCH
	};


protected:
	enumBkType m_nBkType;


// Attributes
public:
	void SetBkType(enumBkType type);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMdiBmpClient)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL SetBitmap(UINT nID);
	COLORREF SetBackColor(COLORREF nBackColor);
	virtual ~CMdiBmpClient();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMdiBmpClient)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CBitmap*	m_pBmp;
	COLORREF	m_nBackColor;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.


