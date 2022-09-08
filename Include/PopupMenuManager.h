// PopupMenuManager.h: interface for the CPopupMenuManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POPUPMENUMANAGER_H__E5D8E13C_A88B_4EA0_9057_6E685A4DCA36__INCLUDED_)
#define AFX_POPUPMENUMANAGER_H__E5D8E13C_A88B_4EA0_9057_6E685A4DCA36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "zoomview.h"

#define MF_BYITEM		0x00010000L
#define MF_MENUITEM		0x00020000L
#define MF_MYPOPUP		0x00040000L

class _FHZOOMVIEW_EXT_CLASS CMenuDefinition 
{
public:
	UINT nID;
	char sItem[64];
	UINT nFlags;
	CMenu menu;
    char cPreTag;
	
	CMenuDefinition()
	{
		nID=0;
		nFlags=0;
		memset(sItem,0,64);
        cPreTag=0;
	};

	void operator=(CMenuDefinition &other)
	{
		nID=other.nID ;
		nFlags=other.nFlags ;
        cPreTag=other.cPreTag;
		strcpy(sItem,other.sItem );
	};

};


class _FHZOOMVIEW_EXT_CLASS CPopupMenuManager 
{
public:
	CPopupMenuManager();
	~CPopupMenuManager();

	bool SetPopupMenu(UINT nID=0);
	bool AppendMenu(UINT nIDMenu,char  sMenuItem[],UINT nFlags=MF_POPUP,char cPreTag=0);
	bool AppendMenu(UINT nIDMenu,CString sMenuItem,UINT nFlags=MF_POPUP,char cPreTag=0);
	bool RemoveMenu(UINT nIDMenu);	// 暂时存放删除菜单，在GetPopupMenu时，一起删除
	
	virtual CMenu* GetPopupMenu();	
	virtual bool ShowPopupMenu(CMenu *pPopup,CPoint point,CWnd *pWndPopupOwner );

	bool  Empty();

    void  operator = (CPopupMenuManager &other);


protected:
	int	m_nMenuLimit;
	CMenuDefinition m_sMenuList[100];
    
	CMenu m_FileMenu;
    CMenu m_menu;

    class CMenuPreTag
    {
    public:
        char cTag;
        bool bUsed;
        CMenuPreTag()
        {
            cTag='A';
            bUsed=false;
        };
        void Set(char c)
        {
            cTag=c;
            bUsed=false;
        };
    };
    int m_iPreTagLimit;
    CMenuPreTag m_clsPreTag[36];
    void ResetPreTag();
    char GetUnUsedPreTag();
    void SetPreTagUsed(char c);

    CString AddPreTag(CString sMenuItem,char cPresetPreTag);
    CString AddPreTag(CMenuDefinition *pMenu);



};

#endif // !defined(AFX_POPUPMENUMANAGER_H__E5D8E13C_A88B_4EA0_9057_6E685A4DCA36__INCLUDED_)
