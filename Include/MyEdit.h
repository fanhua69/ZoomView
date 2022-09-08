#pragma once

#include "zoomview.h"


// CMyEdit
enum _FHZOOMVIEW_EXT_CLASS enumEnterPressed
{
	ENTER_NEXT_CTRL,
	ENTER_PRESS_BUTTON
};

class _FHZOOMVIEW_EXT_CLASS CMyEdit : public CEdit
{
	DECLARE_DYNAMIC(CMyEdit)

public:
	CMyEdit();
	virtual ~CMyEdit();

	CButton *m_pButtonEnter;

	void SetEnterButton(CButton *pButton);
	void SetEnterDealer(enumEnterPressed iDealer){m_iEnterDealer=iDealer;};

protected:
	int m_iEnterDealer;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};


