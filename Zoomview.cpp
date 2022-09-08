// zoomview.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "Zoomview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static AFX_EXTENSION_MODULE ZoomviewDLL = { NULL, NULL };

extern HWND glhPrevTarWnd;      //  �ϴ������ָ�ĵĴ��ڵľ��
extern HWND glhDisplayWnd;      //  ��ʾĿ�괰�ڱ���༭��ľ��
extern HHOOK glhHook;           //  ��װ����깳�Ӿ��
extern HINSTANCE glhInstance;   //  ʵ�����
extern LRESULT WINAPI MouseProc(int nCode, WPARAM wparam, LPARAM lparam);

#include "defines.h"

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    // Remove this if you use lpReserved
    UNREFERENCED_PARAMETER(lpReserved);

    if (dwReason == DLL_PROCESS_ATTACH)
    {
        TRACE0("ZOOMVIEW.DLL Initializing!\n");

        // Extension DLL one-time initialization
        if (!AfxInitExtensionModule(ZoomviewDLL, hInstance))
            return 0;

        // Insert this DLL into the resource chain
        // NOTE: If this Extension DLL is being implicitly linked to by
        //  an MFC Regular DLL (such as an ActiveX Control)
        //  instead of an MFC application, then you will want to
        //  remove this line from DllMain and put it in a separate
        //  function exported from this Extension DLL.  The Regular DLL
        //  that uses this Extension DLL should then explicitly call that
        //  function to initialize this Extension DLL.  Otherwise,
        //  the CDynLinkLibrary object will not be attached to the
        //  Regular DLL's resource chain, and serious problems will
        //  result.

        new CDynLinkLibrary(ZoomviewDLL);

        glhInstance=hInstance;		//	���뱣��DLLʵ�����

#ifndef FH_DEBUG
        glhHook=SetWindowsHookEx(WH_MOUSE,MouseProc,glhInstance,0);
#endif

    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {

#ifndef FH_DEBUG
        BOOL bSuccess=UnhookWindowsHookEx(glhHook);

        if(!bSuccess)
        {
            TRACE0("Unhook failed!\n");
        }
#endif


        TRACE0("ZOOMVIEW.DLL Terminating!\n");
        // Terminate the library before destructors are called

        AfxTermExtensionModule(ZoomviewDLL);
    }
    return 1;   // ok
}

///////////////////////////////////////////
//
///////////////////////////////////////////
_FHZOOMVIEW_EXT_CLASS void SetStaticRectColor(CWnd *pParent, UINT iIDChild, COLORREF nColor)
{
    CWnd *pWnd=pParent->GetDescendantWindow(iIDChild);
    if(pWnd)
    {
        CRect rect;
        pWnd->GetClientRect(rect);
        CDC *pDC=pWnd->GetDC();
        pDC->FillSolidRect(&rect,nColor);
    }
}

