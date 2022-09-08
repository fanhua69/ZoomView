#pragma once

#include "zoomview.h"


#include <list>

#include "ReportBase.h"


class _FHZOOMVIEW_EXT_CLASS CReportWriter  : public CReportBase
{
public:
	CReportWriter(void);
	~CReportWriter(void);

	void Over();
	void SetHeadLen(int iLen)	{ m_iHeadLen=iLen;};
	int  GetHeadLen()			{ return m_iHeadLen;};

	bool InitFile();

	bool WriteMidLine(CString s);
	bool WriteReportName(int iReportNo);
	void WritePosition();

	char * PrepareMidLine(char sValue[]);
	char * PrepareMidLine(CString sValue);
	bool WriteFullLine(char c);
	bool WriteMidLine(char sValue[]);
	bool WriteLeftLine(char sValue[]);
	bool WriteLine(UINT  eLT);
	bool WriteLine(CString sTitle,CString sValue);
	bool WriteLine(char sTitle[],char sValue[]);
	bool WriteLine(char sTitle[],int nValue);
	bool WriteLine(char sTitle[],double dValue,int nSmallNumber,char *psTag=NULL);
	
	bool WriteItem(char sMark[],char sValue[],bool bWithTitle=true);
	bool WriteItem(char sMark[],CString sValue);
	bool WriteItem(char sMark[],double dValue,int nSmallNumber=1, char *psTag=NULL);
	bool WriteItem(char sMark[],int  nValue);
	bool WriteManyItem(char	sMark[],
						int	*piData,
						int	iDataQty,
						int	iDataQtyOneLine);
    bool WriteMark(char	sMark[]);

	
	bool ClearLine(char c=' ');
	bool KeepValue(int nOrder,char sValue[]);
	bool KeepValue(int nOrder,CString sValue);
	bool KeepValue(int nOrder,int nValue);
	bool KeepValue(int nOrder,double dValue,int nSmallNumber,bool bError);
	void MarkNote();
	char * PrepareExport(char c=' ');

	char * GetLine();

};
