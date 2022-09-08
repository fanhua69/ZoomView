#pragma once

#include "ReportBase.h"
#include <list>

class _FHZOOMVIEW_EXT_CLASS CReportReader:public CReportBase
{
public:
	CReportReader(void);
	~CReportReader(void);

	bool InitFile();
	void Over();

	int GetReportNo();
	int GetLineValueQty();
	bool StartGetValue();
	CStringArray * GetValue(UINT nType);

	void  GetItemValue(int iHeadNo,int &iValue);
	void  GetItemValue(int iHeadNo,float &fValue);
	void  GetItemValue(int iHeadNo,double &lfValue);
	char * GetItemValue(int iHeadNo);
	CString GetManyItemValue(int iHeadNo);


protected:
   	std::list<CReportItem *> m_listReport;
	CStringArray m_arrayLineItem;
	int m_iLineValueQty;
	int m_iReportNo;

	bool PickItemFromLine(char sLine[], CReportItem * pItem);
	bool PickPosFromLine(char sLine[]);
	CStringArray *PickValueFromLine(char sLine[],UINT nType);
	bool PickReportNoFromFile();


};
