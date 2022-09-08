// MouseHook.cpp : implementation file
//

#include "stdafx.h"
#include "zoomview.h"
#include "MouseHook.h"


// CMouseHook

IMPLEMENT_DYNAMIC(CMouseHook, CObject)

LRESULT WINAPI MouseProc(int nCode, WPARAM wparam, LPARAM lparam);

#pragma data_seg("MouseHookData")
	HWND glhPrevTarWnd=NULL;	//	上次鼠标所指的的窗口的句柄
	long gllDisplayLimit=1000;
	HWND glhDisplayWnd[1000];	//	显示目标窗口标题编辑框的句柄
	HHOOK glhHook=NULL;			//	安装的鼠标钩子句柄
	HINSTANCE glhInstance=NULL;	//	实例句柄
#pragma data_seg()


CMouseHook::CMouseHook()
{
	m_hWndDisplay=(HWND)(-1);
}

CMouseHook::~CMouseHook()
{
	RemoveFromHook();
}

BOOL CMouseHook::AddToHook(CWnd *pWnd)
{
	/////////////////////////////////////////////////
	// 信息框只是简单的记录信息框窗口：
	/////////////////////////////////////////////////
	HWND hWnd=pWnd->GetSafeHwnd ();

	for(long i=0;i<gllDisplayLimit;i++)
	{
		if(glhDisplayWnd[i]==NULL)
		{
			glhDisplayWnd[i]=hWnd;	// 设置现实目标窗口标题编辑框的句柄
			m_hWndDisplay=hWnd;	
			return true;
		}
	}

	return false;
}

BOOL CMouseHook::RemoveFromHook()
{
	for(long i=0;i<gllDisplayLimit;i++)
	{
		if(glhDisplayWnd[i]==m_hWndDisplay)
		{
			glhDisplayWnd[i]=NULL;
			return true;
		}
	}

	return false;
}


LRESULT WINAPI MouseProc(int nCode, WPARAM wparam, LPARAM lparam)
{
	LPMOUSEHOOKSTRUCT pMouseHook=(MOUSEHOOKSTRUCT*)lparam;

	HWND glhTargetWnd=pMouseHook->hwnd ; //获取目标窗口句柄
	if(nCode>=0)
	{
		if(glhTargetWnd!=glhPrevTarWnd)
		{
			for(long i=0;i<gllDisplayLimit;i++)
			{
				if(glhDisplayWnd[i]!=NULL&&IsWindow(glhDisplayWnd[i]))
				{
					PostMessage(glhDisplayWnd[i],WM_WNDCHANGED,(WPARAM)glhPrevTarWnd,(LPARAM)glhTargetWnd);
				}
			}
		}
		glhPrevTarWnd=glhTargetWnd; // 保存目标窗口
	}

	return CallNextHookEx(glhHook,nCode,wparam,lparam); // 继续传递消息
}
