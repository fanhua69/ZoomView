#include "stdafx.h"
#include "..\include\qcreportwriter.h"
#include "fhapp.h"


CQCReportWriter::CQCReportWriter(void)
{
    AddQCItems(this);

}

CQCReportWriter::~CQCReportWriter(void)
{
}

bool CQCReportWriter::WriteQCCommonPars(CCheckHeadBase *pHead)
{
	WriteItem ("Project",pHead->m_sProject );
	WriteItem ("Area",pHead->m_sArea );
	WriteItem ("Client",pHead->m_sClient );
	WriteItem ("Contractor",pHead->m_sContractor );
	WriteItem ("Crew",pHead->m_sCrew );
	WriteItem ("Year",pHead->m_iProjectYear );
	WriteItem ("Line",pHead->m_sLineName );

	char sLine[300];
	sprintf(sLine,"%d年%d月%d日",pHead->m_iWorkYear ,pHead->m_iWorkMonth,pHead->m_iWorkDay);
	WriteItem ("WorkDate",sLine);
	
	sprintf(sLine,"%d年%d月%d日",pHead->m_iTestYear ,pHead->m_iTestMonth,pHead->m_iTestDay);
	WriteItem ("TestDate",sLine);

	sprintf(sLine,"%d时%d分%d秒",pHead->m_iTestHour ,pHead->m_iTestMinute,pHead->m_iTestSecond);
	WriteItem ("TestTime",sLine);

	CString sID="QC"+GetUniID();
	WriteItem ("TestUnitNo",sID);

	return true;
}
