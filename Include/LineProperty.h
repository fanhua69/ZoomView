

#ifndef tag_LineProperty
#define tag_LineProperty

#include "zoomview.h"
#include "FHString.h"

class _FHZOOMVIEW_EXT_CLASS CLineProperty
{
public:
    CString sPathName;
    CString sShowName;
    UINT nProperty;           // 文件类型:由使用者自己定义自己用。
    UINT nImageOrder;         // 图片序号
    UINT nSelectedImageOrder; // 选定后使用的图片序号
    long nOrder;              // 插入数据库时的位置
    int  nIs2DLine;

    CLineProperty()
    {
        sPathName           ="";
        nProperty           =0;
        nImageOrder         =0;
        nSelectedImageOrder =0;
        nIs2DLine           =1;
        nOrder              =0;
    };

    void operator = (CLineProperty *pOther)
    {
        sPathName           =pOther->sPathName ;
        nProperty           =pOther->nProperty ;
        nImageOrder         =pOther->nImageOrder ;
        nSelectedImageOrder =pOther->nSelectedImageOrder ;
        nIs2DLine           =pOther->nIs2DLine ;
        nOrder              =pOther->nOrder ;
    };

};

#endif