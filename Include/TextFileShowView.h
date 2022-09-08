#pragma once



// CTextFileShowView view
#include "ZoomView.h"
#include "FHZoomView.h"
#include "MyPoint.h"

#define FM_MOUSELBDOWN WM_USER+200

#include "zoomview.h"

class _FHZOOMVIEW_EXT_CLASS CTextFileShowView : public CFHZoomView
{
	DECLARE_DYNCREATE(CTextFileShowView)

protected:
	CTextFileShowView();           // protected constructor used by dynamic creation
	virtual ~CTextFileShowView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
public:
	
	bool SetFile(CString sFile);

protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	virtual CMenu * GetPopupMenu();
	CMyPoint GetStartPos();

	CSize GetMaxChar(CDC *pDC);

	CSize m_sizeBitmap;
	void DrawFile(CDC *pDC);
	void DrawMarks(CDC *pDC);

	CString m_sFile;
	int m_nMinShowableChar;
	int m_nMaxShowableChar;
	CFont m_fontFile;
	int m_nTextLeft;
	int m_nTextTop;
	int m_nTextStartRow;
	int m_nTextStartCol;
	CString m_sTitle;

	long m_nFileLineNumber;
	long m_nMaxCharQtyInLine;
	CSize m_sizeMaxChar;
	CMyPoint m_pointMouse;

	CWnd *m_pMyParent;
	long *m_pLineStartPos;

	long m_nEdgePlusedWidth;

	void DrawMarkLines(CDC *pDC);

public:
	bool SetParent(CWnd *pParent);
	void SetTitle(CString sTitle);


	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};


