// PopupMenuManager.cpp: implementation of the CPopupMenuManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PopupMenuManager.h"
#include "FHMainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPopupMenuManager::CPopupMenuManager()
{
    m_nMenuLimit=100;
    int i=0;
    for(i=0;i<m_nMenuLimit;i++)
    {	
        m_sMenuList[i].nID =0;
    }

    m_iPreTagLimit=36;
    for(i=0;i<26;i++)
    {
        m_clsPreTag[i].Set('A'+i);
    }

    for(i=26;i<36;i++)
    {
        m_clsPreTag[i].Set('0'+i);
    }

}

CPopupMenuManager::~CPopupMenuManager()
{
    for(int i=0;i<m_nMenuLimit;i++)
    {
        m_sMenuList[i].menu .Detach ();
    }
}

void CPopupMenuManager::ResetPreTag()
{
    int i=0;
    for(i=0;i<m_iPreTagLimit;i++)
    {
        m_clsPreTag[i].bUsed=false;
    }
}
char CPopupMenuManager::GetUnUsedPreTag()
{
    int i=0;
    for(i=0;i<m_iPreTagLimit;i++)
    {
        if(m_clsPreTag[i].bUsed==false)
        {
            m_clsPreTag[i].bUsed=true;
            return m_clsPreTag[i].cTag;
        }
    }

    return 0;
}


void CPopupMenuManager::SetPreTagUsed(char c)
{
    int i=0;
    for(i=0;i<m_iPreTagLimit;i++)
    {
        if(m_clsPreTag[i].cTag==c)
        {
            m_clsPreTag[i].bUsed=true;
        }
    }
}

CString CPopupMenuManager::AddPreTag(CString sMenuItem,char cPresetPreTag)
{
    // Set the pretag:
    char cPreTag;
    CString sMenuItemNew;
    if(sMenuItem[0]=='&')
    {
        cPreTag=sMenuItem[1];
        sMenuItemNew=sMenuItem;
    }
    else
    {
        if(cPresetPreTag!=0)
        {
            cPreTag=cPresetPreTag;
        }
        else
        {
            cPreTag=GetUnUsedPreTag();
        }

        if(cPreTag!=0)
        {
            sMenuItemNew=CString("&")+cPreTag+sMenuItem;
        }
    }
    SetPreTagUsed(cPreTag);
    return sMenuItemNew;
}

CString CPopupMenuManager::AddPreTag(CMenuDefinition *pMenu)
{
    CString sMenu=pMenu->sItem;

    sMenu= AddPreTag(sMenu,pMenu->cPreTag);
    if(pMenu->nFlags &MF_MYPOPUP)
    {
        sMenu=" "+sMenu;
    }
    return sMenu;
}

CMenu* CPopupMenuManager::GetPopupMenu()
{

    //////////////////////////////////////
    // 建立基础菜单:
    if(m_menu.m_hMenu )
    {
        m_menu.Detach ();
    }

    // Create a new menu for the application window.
    VERIFY(m_menu.CreateMenu ());

    // Create a "File" popup menu and insert this popup menu to the
    // new menu of the application window. The "File" menu has only
    // one menu item, i.e. "Exit".
    if(m_FileMenu.m_hMenu )
    {
        m_FileMenu.Detach ();
    }
    VERIFY(m_FileMenu.CreatePopupMenu());


    /////////////////////////////////////
    // 附加所有菜单
    CMenuDefinition *pMid=NULL;
    CMenu *pChildMenu=NULL;
    CMenu menuChild;
    int i;
    char cPreTag='A';
    CString sPreTag,sMenuItem;
    ResetPreTag();


    for(int iMenu=0;iMenu<m_nMenuLimit;iMenu++)
    {
        // 获得下一个菜单：
        pMid=&m_sMenuList[iMenu];
        if(pMid->nID ==0)continue;


        // 加入一项菜单
        if((pMid->nFlags&MF_MENUITEM)!=0)  //  Because MF_STRING is 0x00000000, so we could not identify it. use it as default .  ( if(pMid->nFlags &MF_STRING))
        {
            sMenuItem=AddPreTag(pMid);
            m_FileMenu.AppendMenu(MF_STRING, pMid->nID ,sMenuItem);
        }

        // 如果该菜单是一个菜单项组
        else 
        {
            if(pMid->menu )
            {
                pMid->menu .Detach ();
            }

            if(!pMid->menu.LoadMenu(pMid->nID))continue;

            // 加入弹出式菜单：
            if(pMid->nFlags &MF_MYPOPUP)
            {
                sMenuItem=AddPreTag(pMid);
                m_FileMenu.AppendMenu(	MF_POPUP,
                    (UINT)(pMid->menu.GetSubMenu (0)->m_hMenu),
                    sMenuItem);
            }

            // 加入多项菜单：
            else if(pMid->nFlags&MF_BYITEM)
            {
                pChildMenu=pMid->menu .GetSubMenu (0);
                if(!pChildMenu)continue;

                UINT n=pChildMenu->GetMenuItemCount( );
                UINT nID;
                CString sString;

                for(i=0;i<n;i++)
                {
                    nID=pChildMenu->GetMenuItemID (i);
                    pChildMenu->GetMenuString(i,sString,MF_BYPOSITION);
                    sMenuItem=AddPreTag(sString,0);
                    if(nID==-1)
                    {
                        CMenu *pMenu=pChildMenu->GetSubMenu (i);
                        m_FileMenu.AppendMenu (MF_POPUP,(int)pMenu->m_hMenu ,sMenuItem);
                    }
                    else if(nID==0)
                    {
                        m_FileMenu.AppendMenu (MF_SEPARATOR);
                    }
                    else
                    {
                        m_FileMenu.AppendMenu (MF_STRING,nID,(LPCTSTR)(sMenuItem));
                    }
                }
            }
        }

    }// While(pos);

    m_menu.AppendMenu(MF_POPUP, (UINT) m_FileMenu.m_hMenu, "&File"); // this line is needed;

    long nMenuQty=m_menu.GetMenuItemCount ();

    return &m_menu;
}

bool CPopupMenuManager::ShowPopupMenu(CMenu *pPopup,CPoint point,CWnd *pWndPopupOwner )
{

    if(!pPopup)return false;

    while (pWndPopupOwner->GetStyle()& WS_CHILD)
    {
        pWndPopupOwner = pWndPopupOwner->GetParent();
    }


    pPopup->GetSubMenu(0)->TrackPopupMenu (TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
        pWndPopupOwner);

    return true;
}


bool CPopupMenuManager::AppendMenu(UINT nIDMenu, CString sMenuItem, UINT nFlags,char cPreTag)
{
    char sMenu[200];
    strcpy(sMenu,(LPCTSTR)sMenuItem);
    return AppendMenu(nIDMenu,sMenu,nFlags,cPreTag);

}

bool CPopupMenuManager::AppendMenu(UINT nIDMenu, char sMenuItem[], UINT nFlags,char cPreTag)
{
    bool bAdded=false;

    bool bExist=false;
    for(int i=0;i<m_nMenuLimit;i++)
    {
        if(m_sMenuList[i].nID ==nIDMenu)
        {
            bExist=true;
            bAdded=true;
            break;
        }
    }

    if(!bExist)
    {
        for(int i=0;i<m_nMenuLimit;i++)
        {
            if(m_sMenuList[i].nID ==0)
            {
                strcpy( m_sMenuList[i].sItem ,
                        sMenuItem);

                m_sMenuList[i].nID      =nIDMenu;
                m_sMenuList[i].nFlags   =nFlags;
                m_sMenuList[i].cPreTag  =cPreTag;
                bAdded=true;
                break;
            }
        }
    }

    return bAdded;
}

/////////////////////////////////////////////////
// 暂时存放删除菜单，在GetPopupMenu时，一起删除
bool CPopupMenuManager::RemoveMenu(UINT nIDMenu)
{
    int i,j;
    bool bRemoved=false;
    bool bRemovedAny=false;

    do
    {
        bRemoved=false;
        for(i=0;i<m_nMenuLimit;i++)
        {
            if(m_sMenuList[i].nID == 0)
            {
                break;
            }

            if(m_sMenuList[i].nID == nIDMenu)
            {
                for(j=i;j<m_nMenuLimit-1;j++)
                {
                    if(m_sMenuList[j+1].nID !=0)
                    {
                        m_sMenuList[j]      =m_sMenuList[j+1];  // 所有菜单前移
                    }
                    else
                    {
                        m_sMenuList[j].nID  =0;                 // 把最后一个菜单设为零
                        break;
                    }
                }
                bRemoved=true;
                break;
            }
        }	

        if(bRemoved)
        {
            bRemovedAny=true;
        }

    }while(bRemoved); // 防止菜单中包括重复ID的情况；


    return bRemovedAny;
    return true;
}

bool CPopupMenuManager::SetPopupMenu(UINT nID)
{
    Empty();

    if(nID!=0)
    {
        AppendMenu(nID,"基础菜单",MF_BYITEM);
    }

    return true;
}

bool  CPopupMenuManager::Empty()
{
    for(int i=0;i<m_nMenuLimit;i++)
    {
        m_sMenuList[i].nID =0;
    }

    return true;
}

void  CPopupMenuManager::operator = (CPopupMenuManager &other)
{
    memcpy(m_sMenuList,other.m_sMenuList,sizeof(CMenuDefinition)*100);
}
