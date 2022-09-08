// MouseHook.cpp : implementation file
//

#include "stdafx.h"
#include "zoomview.h"
#include "MouseHook.h"


// CMouseHook

IMPLEMENT_DYNAMIC(CMouseHook, CObject)

LRESULT WINAPI MouseProc(int nCode, WPARAM wparam, LPARAM lparam);

#pragma data_seg("MouseHookData")
	HWND glhPrevTarWnd=NULL;	//	�ϴ������ָ�ĵĴ��ڵľ��
	long gllDisplayLimit=1000;
	HWND glhDisplayWnd[1000];	//	��ʾĿ�괰�ڱ���༭��ľ��
	HHOOK glhHook=NULL;			//	��װ����깳�Ӿ��
	HINSTANCE glhInstance=NULL;	//	ʵ�����
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
	// ��Ϣ��ֻ�Ǽ򵥵ļ�¼��Ϣ�򴰿ڣ�
	/////////////////////////////////////////////////
	HWND hWnd=pWnd->GetSafeHwnd ();

	for(long i=0;i<gllDisplayLimit;i++)
	{
		if(glhDisplayWnd[i]==NULL)
		{
			glhDisplayWnd[i]=hWnd;	// ������ʵĿ�괰�ڱ���༭��ľ��
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

	HWND glhTargetWnd=pMouseHook->hwnd ; //��ȡĿ�괰�ھ��
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
		glhPrevTarWnd=glhTargetWnd; // ����Ŀ�괰��
	}

	return CallNextHookEx(glhHook,nCode,wparam,lparam); // ����������Ϣ
}
