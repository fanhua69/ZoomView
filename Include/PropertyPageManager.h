
#pragma once

#include <vector>
#include "afxdlgs.h"
#include "zoomview.h"

class _FHZOOMVIEW_EXT_CLASS CPropertyPageManager
{
public:
    CPropertyPageManager(void);
    ~CPropertyPageManager(void);

protected:
    virtual void                AddSettingPage  (   CPropertyPage *pPage    );
    virtual CPropertyPage     * GetSettingPage  (   CRuntimeClass *pClass   );
    virtual void                SetInvalidate   ();

    virtual     void            PreSettingDlg   ();
    virtual     void            PostSettingDlg  ();
    virtual     void            SavePars        ();
    virtual     void            InvalidateView  ();
    void                        ProcessSettings ();

    std::vector<CPropertyPage*> m_vecDlgSettings;
    bool                        m_bInvalidateAfterSetting;

};

#define GetPropertyPage(class_name) (class_name *)GetSettingPage(RUNTIME_CLASS(class_name));
#define AddPropertyPage(class_pointer) AddSettingPage ((CPropertyPage*)class_pointer);
