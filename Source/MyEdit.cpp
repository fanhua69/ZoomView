// .\source\MyEdit.cpp : implementation file
//

#include "stdafx.h"
#include "zoomview.h"
#include "MyEdit.h"
#include "..\include\myedit.h"


// CMyEdit

IMPLEMENT_DYNAMIC(CMyEdit, CEdit)
CMyEdit::CMyEdit()
{
	//ES_WANTRETURN
	//	ES_MULTILINE
	::SetWindowLong(m_hWnd,GWL_STYLE,ES_WANTRETURN);
	m_pButtonEnter=NULL;
	m_iEnterDealer=ENTER_NEXT_CTRL;

}

CMyEdit::~CMyEdit()
{
}

BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()



// CMyEdit message handlers
void CMyEdit::SetEnterButton(CButton *pButton)
{
	m_pButtonEnter=pButton;
}

void CMyEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	if(nChar==VK_RETURN)
	{
		CDialog *pWnd=(CDialog*)GetParent();
		if(m_iEnterDealer==ENTER_NEXT_CTRL)
		{
			pWnd->NextDlgCtrl ();
		}
		else if(m_iEnterDealer==ENTER_PRESS_BUTTON)
		{
			if(m_pButtonEnter)
			{
				m_pButtonEnter->SendMessage (WM_LBUTTONDOWN);
				m_pButtonEnter->SendMessage (WM_LBUTTONUP);
			}
		}
	}
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}
