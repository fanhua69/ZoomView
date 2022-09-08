#pragma once
#include "reportwriter.h"
#include "FHMainFrm.h"

class CCheckHeadBase
{
public:
    char        m_sFileMark[16];
    char        m_sProject[32];
    char        m_sArea[32];
    char        m_sClient[32];
    char        m_sContractor[16];
    char        m_sLineName[16];
    char        m_sCrew[16];
    int         m_iProjectYear;
    int         m_iWorkYear;
    int         m_iWorkMonth;
    int         m_iWorkDay;
    int         m_iTestYear;
    int         m_iTestMonth;
    int         m_iTestDay;
    int         m_iTestHour;
    int         m_iTestMinute;
    int         m_iTestSecond;

    CCheckHeadBase()
    {
        memset(this,0,sizeof(CCheckHeadBase));
    };


    void operator = (CCheckHeadBase &other)
    {
        memcpy(this,&other,sizeof(CCheckHeadBase));
    };

    bool operator == (CCheckHeadBase &other)
    {
        if(memcmp(this,&other,sizeof(CCheckHeadBase)))
        {
            return false;
        }
        else
        {
            return true;
        }
    };

    bool operator != (CCheckHeadBase &other)
    {
        if(memcmp(this,&other,sizeof(CCheckHeadBase)))
        {
            return true;
        }
        else
        {
            return false;
        }
    };

	bool CheckHeadFilled()
	{
		if(strlen(m_sProject)==0)
		{
			TRACEERROR("��Ŀ����û����д!");
			return false;
		}

		if(strlen(m_sArea)==0)
		{
			TRACEERROR("��������û����д!");
			return false;
		}

		if(strlen(m_sClient)==0)
		{
			TRACEERROR("���󣺿ͻ�����û����д!");
			return false;
		}

		if(strlen(m_sContractor)==0)
		{
			TRACEERROR("�а�������û����д!");
			return false;
		}

		if(strlen(m_sLineName)==0)
		{
			TRACEERROR("��������û����д!");
			return false;
		}

		if(strlen(m_sCrew)==0)
		{
			TRACEERROR("�Ӻ�û����д!");
			return false;
		}

		if(m_iProjectYear==0)
		{
			TRACEERROR("ʩ�����û����д!");
			return false;
		}

		if(m_iWorkYear==0||m_iWorkMonth==0||m_iWorkDay==0)
		{
			TRACEERROR("ʩ����������д��ȫ!");
			return false;
		}

		if(m_iTestYear==0||m_iTestMonth==0||m_iTestDay==0)
		{
			TRACEERROR("�����������д��ȫ!");
			return false;
		}

		if(m_iTestHour==0||m_iTestMinute==0)
		{
			TRACEERROR("���ʱ����д��ȫ!");
			return false;
		}

		return true;
	};

};



class _FHZOOMVIEW_EXT_CLASS CQCReportWriter :
	public CReportWriter
{
public:
	CQCReportWriter(void);
	~CQCReportWriter(void);

	bool WriteQCCommonPars(CCheckHeadBase *pHead);

};
