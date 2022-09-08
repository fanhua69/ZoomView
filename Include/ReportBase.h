#pragma once

#include "zoomview.h"

#define LT_NOTE			0x0001
#define LT_DATA			0x0002
#define LT_POS			0x0004
#define LT_SHOT			0x0008
#define LT_RCV			0x0010
#define LT_BOX          0x0020
#define LT_REPORTNAME	0x0040
#define LT_PROPERTY     0x0080


class _FHZOOMVIEW_EXT_CLASS CReportItem
{
public:
	char sMark[52];
	char sHeadNo[5];
	char sTitle[52];
	char sContent[100];

	CReportItem()
	{
		memset(this,0,sizeof(CReportItem));
	};

	bool operator ==(char sMarkTag[])
	{
		return (strcmp(sMark,sMarkTag)==0);
	};
};

class _FHZOOMVIEW_EXT_CLASS CReportName
{
public:
	char sReportNo[5];
	char sReportName[100];

	CReportName()
	{
		memset(this,0,sizeof(CReportName));
	};

	bool operator ==(char sReportNoTag[])
	{
		return (strcmp(sReportNo,sReportNoTag)==0);
	};
};

#include <list>

class _FHZOOMVIEW_EXT_CLASS CReportBase
{
public:
	CReportBase(void);
	~CReportBase(void);


	bool AddItem(int nHeadNo,char sMarkTag[],char sNoteTag[]);
	CReportItem * SearchItem(char sMark[]);
	CReportItem * SearchItem(int  iHeadNo);

	bool AddReportName(int nReportNo,char sName[]);
	CReportName * SearchReport(char sReportName[]);
	CReportName * SearchReport(int  iReportNo);
	void    ReleaseMemory();
    int     GetLineLength();



protected:

	FILE *m_fp;
	CString m_sFileName;
   	std::list<CReportName *> m_listReportNames;
   	std::list<CReportItem *> m_listItem;

	int m_nLineLength;
	char m_sLine[1010];
	int m_nLineLengthLimit;

	int m_iHeadLen;
	int m_nSingleValueStartPos;
	int m_nValueWidth[200];
	int m_nValueStartPos[200];
	int m_nValueLimit;

	char m_cNoteMarkChar;
	char m_cDataMarkChar;
	char m_cPosMarkChar;
	char m_cShotMarkChar;
	char m_cRcvMarkChar;
	char m_cReportNameMarkChar;
	char m_cPropertyMarkChar;
	char m_cBoxMarkChar;


	CReportItem m_item;

	virtual bool InitFile();
	virtual void Over();

	bool CalcValueStartPos();
	bool SetValuesStartPos(int nOrder,int nStartPos);
	bool SetLineLength(int nLineLength);

public:
	bool OpenFile(char sFile[]);
	bool OpenFile(CString sFile);
	bool SetValueWidth(int iOrder, int iWidth);
	bool SetValueStartPos(int nValueStartPos);

public:
	virtual bool InitReportItem();


//	virtual bool SetLineLength(int nLineLength);
};

void _FHZOOMVIEW_EXT_CLASS AddQCItems   (CReportBase *pReport);
bool _FHZOOMVIEW_EXT_CLASS AddReport    (CString sArea, CString sReportName);
bool _FHZOOMVIEW_EXT_CLASS AddLine      (CString sArea, CString sLineName, CRuntimeClass *pRuntimeClass);

