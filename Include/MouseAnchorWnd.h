#pragma once


// CMouseAnchorWnd

class CFHZoomView;

class CMouseAnchorWnd : public CWnd
{
	DECLARE_DYNAMIC(CMouseAnchorWnd)

protected:
	DECLARE_MESSAGE_MAP()


private:
   using CWnd::Create;

public:
	CMouseAnchorWnd(CPoint& ptAnchor);
	~CMouseAnchorWnd();

	BOOL Create(CFHZoomView * pParent);
	void SetBitmap(UINT nID);

	CRect m_rectDrag;
	CPoint m_ptAnchor;
	BOOL m_bQuitTracking;
	UINT m_nAnchorID;
	HCURSOR m_hAnchorCursor;

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

};


