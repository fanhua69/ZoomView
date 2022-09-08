#include "stdafx.h"
#include "PropertyPageManager.h"


CPropertyPageManager::CPropertyPageManager(void)
{
    m_bInvalidateAfterSetting = false;
}


CPropertyPageManager::~CPropertyPageManager(void)
{
}

void CPropertyPageManager::SavePars ()
{
    return;
}

void CPropertyPageManager::InvalidateView  ()
{
    return;
}

void  CPropertyPageManager::AddSettingPage(CPropertyPage *pPage)
{
    m_vecDlgSettings.push_back(pPage);
}


CPropertyPage * CPropertyPageManager::GetSettingPage(CRuntimeClass *pClass)
{
    int i=0;
    CPropertyPage *pPage=NULL;
    for(i=0;i<m_vecDlgSettings.size();i++)
    {
        pPage=m_vecDlgSettings.at(i);
        if(pPage->IsKindOf(pClass))
        {
            return m_vecDlgSettings.at(i);
        }
    }
    return NULL;
}


void CPropertyPageManager::SetInvalidate()
{
    m_bInvalidateAfterSetting=true;
}

void CPropertyPageManager::ProcessSettings()
{
    m_vecDlgSettings.clear();

    PreSettingDlg();

    if(m_vecDlgSettings.size()<1)return;

    int i;
    CPropertySheet sheet;
    for(i=0;i<m_vecDlgSettings.size();i++)
    {
        sheet.AddPage(m_vecDlgSettings.at(i));
    }

    m_bInvalidateAfterSetting=false;
    sheet.SetTitle("ÉèÖÃ");
    if(sheet.DoModal()==IDOK)
    {
        PostSettingDlg();
        SavePars();
    }

    if(m_bInvalidateAfterSetting)
    {
        InvalidateView();
    }

    for(i=0;i<m_vecDlgSettings.size();i++)
    {
        CPropertyPage *pPage=m_vecDlgSettings.at(i);
        delete pPage;
    }

    m_vecDlgSettings.clear();
}


void CPropertyPageManager::PreSettingDlg   ()
{

}

void CPropertyPageManager::PostSettingDlg  ()
{

}
