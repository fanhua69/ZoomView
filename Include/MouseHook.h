#pragma once

#define WM_WNDCHANGED (WM_USER+1111)

// CMouseHook

class CMouseHook : public CObject
{
	DECLARE_DYNAMIC(CMouseHook)

public:
	CMouseHook();
	virtual ~CMouseHook();

	BOOL AddToHook(CWnd *pWnd);
	BOOL RemoveFromHook();

	HWND m_hWndDisplay;
};


