

#ifndef tag_LineProperty
#define tag_LineProperty

#include "zoomview.h"
#include "FHString.h"

class _FHZOOMVIEW_EXT_CLASS CLineProperty
{
public:
    CString sPathName;
    CString sShowName;
    UINT nProperty;           // �ļ�����:��ʹ�����Լ������Լ��á�
    UINT nImageOrder;         // ͼƬ���
    UINT nSelectedImageOrder; // ѡ����ʹ�õ�ͼƬ���
    long nOrder;              // �������ݿ�ʱ��λ��
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