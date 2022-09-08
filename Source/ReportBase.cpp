#include "stdafx.h"
#include "reportbase.h"
#include "FHMainFrm.h"

CReportBase::CReportBase(void)
{
	m_fp=NULL;
	m_nLineLength=80;
	
	m_nLineLengthLimit=1000;
	memset(m_sLine,0,m_nLineLengthLimit);

	m_nValueLimit=200;
	memset(m_nValueStartPos,0,m_nValueLimit*sizeof(int));
	memset(m_nValueWidth,0,m_nValueLimit*sizeof(int));

	m_cNoteMarkChar			='*';
	m_cDataMarkChar			='D';
	m_cPosMarkChar			='L';
	m_cShotMarkChar			='S';
	m_cRcvMarkChar			='R';
	m_cReportNameMarkChar	='N';
	m_cPropertyMarkChar     ='P';
	m_cBoxMarkChar			='B';

	m_nSingleValueStartPos=33;

	m_iHeadLen=4;

	InitReportItem();
}

CReportBase::~CReportBase(void)
{
	ReleaseMemory();
	Over();
}

void CReportBase::ReleaseMemory()
{
	/////////////////////////////////////
	// Release Report Names Memory:
	/////////////////////////////////////
	std::list <CReportName*>::iterator p;

	CReportName *pData=NULL;
	for ( p = m_listReportNames.begin(); p!= m_listReportNames.end(); ++p )
	{
		pData=*p;

		if(pData)
		{
			delete pData;
		}
	}
	
	m_listReportNames.clear ();

	/////////////////////////////////////
	// Release Items Memory:
	/////////////////////////////////////
	std::list <CReportItem*>::iterator p2;

	CReportItem *pData2=NULL;
	for ( p2 = m_listItem.begin(); p2!= m_listItem.end(); ++p2 )
	{
		pData2=*p2;

		if(pData2)
		{
			delete pData2;
		}
	}
	
	m_listItem.clear ();
}
///////////////////////////////////////////
// virtual:
///////////////////////////////////////////
void CReportBase::Over()
{
	return;
}

bool CReportBase::AddReportName(int nReportNo,char sName[])
{
	if(nReportNo<0||nReportNo>9999)return false;
	if(strlen(sName)>50)return false;
	if(strlen(sName)>50)return false;

	////////////////////////////////////
	//write the head number:
	CReportName *pItem = new CReportName;

	sprintf(pItem->sReportNo ,"%d",nReportNo);
	char s[10];
	while(strlen(pItem->sReportNo)<m_iHeadLen)
	{
		strcpy(s,"0");
		strcat(s,pItem->sReportNo);
		strcpy(pItem->sReportNo,s);
	}

	//////////////////////////////
	// Write the mark:
	strcpy(pItem->sReportName,sName);

	///////////////////////////
	// Save the item into the list:
	m_listReportNames.push_back (pItem);

	return true;
}

CReportName * CReportBase::SearchReport(char sReportName[])
{
	std::list <CReportName*>::iterator p;

	CReportName *pData=NULL;
	for ( p = m_listReportNames.begin(); p!= m_listReportNames.end(); ++p )
	{
		pData=*p;

		if(strcmp(pData->sReportName ,sReportName)==0)
		{
			return pData;
		}
	}

	return NULL;
}

CReportName * CReportBase::SearchReport(int  iReportNo)
{
	std::list <CReportName*>::iterator p;

	CReportName *pData=NULL;
	int iNoThis;
	for ( p = m_listReportNames.begin(); p!= m_listReportNames.end(); ++p )
	{
		pData=*p;

		sscanf(pData->sReportNo ,"%d",&iNoThis);
		if(iNoThis==iReportNo)
		{
			return pData;
		}
	}

	return NULL;

}


bool CReportBase::AddItem(int nHeadNo,char sMarkTag[],char sTitleTag[])
{
	if(nHeadNo<0)return false;
	if(strlen(sMarkTag)>50)return false;
	if(strlen(sTitleTag)>50)return false;

	////////////////////////////////////
	//write the head number:
	CReportItem *pItem = new CReportItem;

	sprintf(pItem->sHeadNo,"%d",nHeadNo);
	char s[10];
	while(strlen(pItem->sHeadNo)<m_iHeadLen)
	{
		strcpy(s,"0");
		strcat(s,pItem->sHeadNo);
		strcpy(pItem->sHeadNo,s);
	}

	//////////////////////////////
	// Write the mark:
	strcpy(pItem->sMark,sMarkTag);

	//////////////////////////////
	// Write the mark:
	strcpy(pItem->sTitle,sTitleTag);

	///////////////////////////
	// Save the item into the list:
	m_listItem.push_back (pItem);

	return true;
}

CReportItem * CReportBase::SearchItem(int  iHeadNo)
{
	std::list <CReportItem*>::iterator p;

	CReportItem *pData=NULL;
	int iHeadNoThis;
	for ( p = m_listItem.begin(); p!= m_listItem.end(); ++p )
	{
		pData=*p;

		sscanf(pData->sHeadNo ,"%d",&iHeadNoThis);
		if(iHeadNoThis==iHeadNo)
		{
			return pData;
		}
	}

	return NULL;

}

CReportItem * CReportBase::SearchItem(char sMark[])
{
	std::list <CReportItem*>::iterator p;

	CReportItem *pData=NULL;
	for ( p = m_listItem.begin(); p!= m_listItem.end(); ++p )
	{
		pData=*p;

		if((*pData)==sMark)
		{
			return pData;
		}
	}

	return NULL;
}

bool CReportBase::InitReportItem()
{
	return true;
}




bool CReportBase::InitFile()
{
	return true;
}

bool CReportBase::SetValueWidth(int iOrder, int iWidth)
{
	if(iOrder<0||iOrder>=m_nValueLimit)return false;

	m_nValueWidth[iOrder]=iWidth;

	return CalcValueStartPos();
}


bool CReportBase::CalcValueStartPos()
{
	int iStartPos=m_iHeadLen+1;  // Including H0001
	for(int i=0;i<m_nValueLimit;i++)
	{
		SetValuesStartPos(i,iStartPos);
		iStartPos+=m_nValueWidth[i];
	}
	SetLineLength(iStartPos);

	return true;
}


bool CReportBase::SetLineLength(int nLineLength)
{
	if(nLineLength>m_nLineLengthLimit)
	{
		return false;
	}
	else
	{
		m_nLineLength=nLineLength;
		return true;
	}
}

bool CReportBase::SetValueStartPos(int nValueStartPos)
{
	if(nValueStartPos>m_nLineLengthLimit)return false;
	m_nSingleValueStartPos=nValueStartPos;
	return true;
}

bool CReportBase::SetValuesStartPos(int nOrder,int nStartPos)
{
	if(nOrder>m_nValueLimit)
	{
		ASSERT(false);
		TRACEERROR("������ֵ����������:"+vtos(nOrder));
		return false;
	}
	
	if(nStartPos>m_nLineLengthLimit)
	{
		TRACEERROR("������ֵ��ʼλ�ó���:"+vtos(nStartPos));
		return false;
	}

	if(nStartPos<0)
	{
		ASSERT(false);
		TRACEERROR("������ֵ��ʼλ��С����:"+vtos(nStartPos));
		return false;
	}

	m_nValueStartPos[nOrder]=nStartPos;

	return true;
}

bool CReportBase::OpenFile(char sFile[])
{	
	if(strcmp(sFile,"")==0)return false;

	Over();

	m_sFileName=sFile;

	return InitFile();
}

bool CReportBase::OpenFile(CString sFile)
{
	char s[100];
	strcpy(s,(LPCTSTR)sFile);

	return OpenFile(s);
}

int CReportBase::GetLineLength()
{
    return m_nLineLength;
}

void AddQCItems(CReportBase *pReport)
{

    ///////////////////////////////////
    // ͨ����Ŀ
    pReport->AddItem(0,"Project",					"��Ŀ����");
    pReport->AddItem(1,"Area",						"��������");
    pReport->AddItem(2,"Client",					"��    ��");
    pReport->AddItem(3,"Contractor",				"�� �� ��");
    pReport->AddItem(4,"Crew",						"��    ��");
    pReport->AddItem(5,"Year",						"ʩ�����");
    pReport->AddItem(6,"Line",						"��������");
    pReport->AddItem(7,"WorkDate",					"ʩ������");
    pReport->AddItem(8,"TestDate",					"��������");
    pReport->AddItem(9,"TestTime",					"����ʱ��");
    pReport->AddItem(10,"TestUnitNo",				"���β���Ψһ���");

    pReport->AddItem(11,"SeisInst",					"�������ͺ�");
    pReport->AddItem(12,"FileName",					"�ļ�����");



    //////////////////////////////////////
    // ���ڼ����Ŀ��100
    pReport->AddReportName (10,"�ο��������ģ�������Ƶ�ʼ�ر���");
    pReport->AddReportName (11,"�ο��������ģ�������������ر���");

    pReport->AddItem(1000,"MonMinFileNumber",		"��С����ļ���");
    pReport->AddItem(1001,"MonMaxFileNumber",		"������ļ���");
    pReport->AddItem(1002,"MonMinStation",			"��С��ص��");
    pReport->AddItem(1003,"MonMaxStation",			"����ص��");
    pReport->AddItem(1004,"MonMinSwath",			"��С���������");
    pReport->AddItem(1005,"MonMaxSwath",			"�����������");
    pReport->AddItem(1006,"MonTotalShotQty",		"����ڵ�����");
    pReport->AddItem(1007,"MonLowDoor",				"�����ȵ��ż�ֵ");
    pReport->AddItem(1008,"MonHighDoor",			"�����ȸ��ż�ֵ");
    pReport->AddItem(1009,"MonBadShotQty",			"�����ڵ���Ŀ");
    pReport->AddItem(1010,"MonBadShotRate",			"�����ڵ����");

    /////////////////////////////////////////
    //  RefMonitor ģ�飺
    /////////////////////////////////////////
    pReport->AddReportName (12,"�ڵ����ϱ���");



    /////////////////////////////////
    // �첨��������Ŀ��200
    pReport->AddReportName (20,"�����ǲ��Լ첨������");
    pReport->AddReportName (21,"�첨�������ǲ��Լ첨������");
    pReport->AddReportName (22,"�첨�����Ա�׼����");

    pReport->AddItem(2001,"GeoTestInst",			"�첨���������ͺ�");
    pReport->AddItem(2002,"GeoType",				"�첨���ͺ�");
    pReport->AddItem(2003,"GeoNatuFreqRulerValue",	"��ȻƵ�ʱ�׼ֵ");
    pReport->AddItem(2004,"GeoNatuFreqErroPerm",	"��ȻƵ���������");
    pReport->AddItem(2005,"GeoResiCooeRulerValue",	"����ϵ����׼ֵ");
    pReport->AddItem(2006,"GeoResiCooeErroPerm",	"����ϵ���������");
    pReport->AddItem(2007,"GeoDistortRulerValue",	"г��ʧ���׼ֵ");
    pReport->AddItem(2008,"GeoDistortErroPerm",		"г��ʧ���������");
    pReport->AddItem(2009,"GeoSensRulerValue",		"�����ȱ�׼ֵ");
    pReport->AddItem(2010,"GeoSensErroPerm",		"�������������");
    pReport->AddItem(2011,"GeoResiRulerValue",		"ֱ�������׼ֵ");
    pReport->AddItem(2012,"GeoResiErroPerm",		"ֱ�������������");
    pReport->AddItem(2013,"GeoPolaRulerValue",		"���Ա�׼ֵ");
    pReport->AddItem(2014,"GeoInsulateResiRulerValue","��Ե�����׼ֵ");
    pReport->AddItem(2015,"GeoInsulateResiErroPerm","��Ե���������ֵ");	
    pReport->AddItem(2016,"GeoPhaseRulerValue",		"��λ��׼ֵ");
    pReport->AddItem(2017,"GeoPhaseErroPerm",		"��λ�����ֵ");	

    pReport->AddItem(2100,"GeoSerQty",				"�첨��������Ŀ");
    pReport->AddItem(2101,"GeoParQty",				"�첨��������Ŀ");
    pReport->AddItem(2102,"GeoStriSerQty",			"�첨�����Ĵ�����Ŀ");
    pReport->AddItem(2103,"GeoStriParQty",			"�첨�����Ĳ�����Ŀ");
    pReport->AddItem(2104,"GeoDistInStri",			"�첨��֮��С�߳���");
    pReport->AddItem(2105,"GeoDistToCabl",			"�첨�������ߵĳ���");
    pReport->AddItem(2106,"GeoStriResiCooe",		"�첨��С�ߵ�����");
    pReport->AddItem(2107,"GeoStartTestLine",		"��ʼ����ߺ�");
    pReport->AddItem(2108,"GeoEndTestLine",			"��ֹ����ߺ�");
    pReport->AddItem(2109,"GeoStartTestStation",	"��ʼ���վ��");
    pReport->AddItem(2110,"GeoEndTestStation",		"��ֹ���վ��");

    pReport->AddItem(2200,"GeoInUseQty",			"���ü첨������Ŀ");	
    pReport->AddItem(2201,"GeoTestQty",				"���첨������Ŀ");
    pReport->AddItem(2202,"GeoGoodQty",				"�첨�������Ŀ");
    pReport->AddItem(2203,"GeoGoodRate",			"�첨�������");
    pReport->AddItem(2204,"GeoGoodRateInUse",		"���ü첨�������");

    pReport->AddItem(2300,"GeoNatuBadQty",			"��ȻƵ�ʲ��ϸ���Ŀ");
    pReport->AddItem(2301,"GeoNatuBadRate",			"��ȻƵ�ʲ��ϸ���");

    pReport->AddItem(2302,"GeoResiCooeBadQty",		"����ϵ�����ϸ���Ŀ");
    pReport->AddItem(2303,"GeoResiCooeBadRate",		"����ϵ�����ϸ���");

    pReport->AddItem(2304,"GeoSensBadQty",			"�����Ȳ��ϸ���Ŀ");
    pReport->AddItem(2305,"GeoSensBadRate",			"�����Ȳ��ϸ���");

    pReport->AddItem(2306,"GeoResiBadQty",			"ֱ�����費�ϸ���Ŀ");
    pReport->AddItem(2307,"GeoResiBadRate",			"ֱ�����費�ϸ���");

    pReport->AddItem(2308,"GeoDistortionBadQty",	"г��ʧ�治�ϸ���Ŀ");
    pReport->AddItem(2309,"GeoDistortionBadRate",	"г��ʧ�治�ϸ���");

    pReport->AddItem(2310,"GeoPolaBadQty",			"���Բ��ϸ���Ŀ");
    pReport->AddItem(2311,"GeoPolaBadRate",			"���Բ��ϸ���");

    pReport->AddItem(2312,"GeoPolaBadQty",			"���Բ��ϸ���Ŀ");
    pReport->AddItem(2313,"GeoPolaBadRate",			"���Բ��ϸ���");

    pReport->AddItem(2314,"GeoInsulateResiBadQty",	"��Ե���費�ϸ���Ŀ");
    pReport->AddItem(2315,"GeoInsulateResiBadRate",	"��Ե���費�ϸ���");

    pReport->AddItem(2316,"GeoPhaseBadQty",			"��λ���ϸ���Ŀ");
    pReport->AddItem(2317,"GeoPhaseBadRate",		"��λ���ϸ���");

    /////////////////////////////////////
    //
    pReport->AddReportName (30,"��У�����Ƚϱ���");
    pReport->AddItem(3000,"CompareFileName",			"�Ƚ��ļ�����");
    pReport->AddItem(3001,"ShotStaticOutThresholdQty",	"�ڵ㾲У����������Ŀ");
    pReport->AddItem(3002,"RcvStaticOutThresholdQty",	"�첨�㾲У����������Ŀ");
    pReport->AddItem(3003,"GDOutThresholdQty",			"���ٲ㶥�泬����Ŀ");
    pReport->AddItem(3004,"TotalStationQty",			"վ������Ŀ");
    pReport->AddItem(3005,"ShotOutThresRatio",			"�ڵ㾲У�������ޱ���");
    pReport->AddItem(3006,"RcvOutThresRatio",			"�첨�㾲У�������ޱ���");
    pReport->AddItem(3007,"GDOutThresRatio",			"���ٲ㶥�泬�ޱ���");

    ////////////////////////////////
    // �������ؼ��ģ�飺
    pReport->AddReportName (40,"�����ⱨ��");
    pReport->AddReportName (41,"ҩ����ⱨ��");

    pReport->AddItem(4000,"EnergyMonitorWellQty",		"������ƾ���Ŀ");
    pReport->AddItem(4001,"EnergyCrewWellQty",			"ʩ������ƾ���Ŀ");
    pReport->AddItem(4002,"EnergyFieldWellQty",			"ʵ�ʾ���Ŀ");

    pReport->AddItem(4003,"EnergyWellCompareMode",		"����Ƚ϶���");
    pReport->AddItem(4004,"EnergyWellComparedQty",		"����Ƚ���Ŀ");
    pReport->AddItem(4005,"EnergyWellLowerDoor",		"ǳ���ż�ֵ");
    pReport->AddItem(4006,"EnergyWellHigherDoor",		"��ż�ֵ");
    pReport->AddItem(4007,"EnergyWellLowerQtyThanCrew",	"��ʩ�������ǳ����Ŀ");
    pReport->AddItem(4008,"EnergyWellHigherQtyThanCrew","��ʩ����������Ŀ");
    pReport->AddItem(4009,"EnergyWellLowerQtyThanMonitor",	"�ȼ������ǳ����Ŀ");
    pReport->AddItem(4010,"EnergyWellHigherQtyThanMonitor",	"�ȼ���������Ŀ");

    pReport->AddItem(4100,"EnergyCompareMode",		    "ҩ���Ƚ϶���");
    pReport->AddItem(4101,"EnergyComparedQty",		    "ҩ���Ƚ���Ŀ");
    pReport->AddItem(4102,"EnergyLowerDoor",			"��ҩ���ż�ֵ");
    pReport->AddItem(4103,"EnergyHigherDoor",		    "��ҩ���ż�ֵ");
    pReport->AddItem(4104,"EnergyLowerQtyThanCrew",		"��ʩ���������ҩ����Ŀ");
    pReport->AddItem(4105,"EnergyHigherQtyThanCrew",	"��ʩ������ƶ�ҩ����Ŀ");
    pReport->AddItem(4106,"EnergyLowerQtyThanMonitor",	"�ȼ��������ҩ����Ŀ");
    pReport->AddItem(4107,"EnergyHigherQtyThanMonitor",	"�ȼ�����ƶ�ҩ����Ŀ");


    ///////////////////////////////////////
    // �����ռ�ģ��
    pReport->AddReportName (50,"����̽���������ռ��ر���");
    pReport->AddItem (5000,"DC_CHECK_TYPE",				"�������");
    pReport->AddItem (5001,"DC_SAMPLE_INTERVAL",		"������");
    pReport->AddItem (5002,"DC_CHECKITEM_NEED_QTY",		"Ӧ�����Ŀ");
    pReport->AddItem (5003,"DC_CHECKITEM_QTY",			"ʵ�����Ŀ");
    pReport->AddItem (5004,"DC_BOX_QTY",				"���ɼ�վ����");
    pReport->AddItem (5005,"DC_BOX_MinNo",				"�ɼ�վ��С���");
    pReport->AddItem (5006,"DC_BOX_MaxNo",				"�ɼ�վ�����");

    pReport->AddItem (5007,"DC_BOX_QtyInUse",			"���òɼ�վ����");
    pReport->AddItem (5008,"DC_BOX_QtyNotChecked",		"δ���ɼ�վ����");
    pReport->AddItem (5009,"DC_BOX_QtyNoNotChecked",	"δ���ɼ�վ���");
    pReport->AddItem (5010,"DC_BOX_QtyNotInUse",		"�����òɼ�վ����");
    pReport->AddItem (5011,"DC_BOX_QtyNoNotInUse",		"�����òɼ�վ���");

    pReport->AddItem (5014,"DC_GEOPHONE_QTY",			"���յ�����");
    pReport->AddItem (5015,"DC_BOX_PASSQTY",			"�ɼ�վͨ������");
    pReport->AddItem (5016,"DC_GEOPHONE_PASSQTY",		"���յ�ͨ������");
    pReport->AddItem (5017,"DC_BOX_CHECK_PASSRATE",		"���ɼ�վͨ����(%)");
    pReport->AddItem (5018,"DC_BOX_IN_USE_PASSRATE",	"���òɼ�վͨ����(%)");
    pReport->AddItem (5019,"DC_GEOPHONE_PASSRATE",		"���յ�ͨ����(%)");

    /////////////////////////////////////////
    //  SPS ģ�飺
    /////////////////////////////////////////
    pReport->AddReportName (60,"SPS�ļ���������");
    pReport->AddItem (6000,"SPS_FieldShotQty",		    "ʵ���ڵ�����");
    pReport->AddItem (6001,"SPS_TheoryShotQty",		    "�����ڵ�����");
    pReport->AddItem (6002,"SPS_TotalDynamiteQty",	    "��ҩ��");
    pReport->AddItem (6003,"SPS_AverDynamiteQty",	    "ƽ��ҩ��");
    pReport->AddItem (6004,"SPS_TotalWellDepth",		"�ܾ���");
    pReport->AddItem (6005,"SPS_AverWellDepth",		    "ƽ������");
    pReport->AddItem (6006,"SPS_AbnShotQtyQty",		    "�쳣�ڵ�����");
    pReport->AddItem (6007,"SPS_AbnShotRatio",		    "�쳣�ڵ����(��/��)");
    pReport->AddItem (6008,"SPS_OffsetShotQty",		    "ƫ���ڵ�����");

    pReport->AddItem (6009,"SPS_ShotOffsetDoor1",		"�ڵ�ƫ������ż�1");
    pReport->AddItem (6010,"SPS_ShotOffsetDoor2",		"�ڵ�ƫ������ż�2");
    pReport->AddItem (6011,"SPS_ShotOffsetDoor3",		"�ڵ�ƫ������ż�3");
    pReport->AddItem (6012,"SPS_ShotOffsetDoor4",		"�ڵ�ƫ������ż�4");
    pReport->AddItem (6013,"SPS_ShotOffsetDoor5",		"�ڵ�ƫ������ż�5");
    pReport->AddItem (6014,"SPS_ShotOffsetDoor6",		"�ڵ�ƫ������ż�6");

    pReport->AddItem (6015,"SPS_ShotOffsetQty1",		"ƫ���ڵ�����1-2");
    pReport->AddItem (6016,"SPS_ShotOffsetQty2",		"ƫ���ڵ�����2-3");
    pReport->AddItem (6017,"SPS_ShotOffsetQty3",		"ƫ���ڵ�����3-4");
    pReport->AddItem (6018,"SPS_ShotOffsetQty4",		"ƫ���ڵ�����4-5");
    pReport->AddItem (6019,"SPS_ShotOffsetQty5",		"ƫ���ڵ�����5-6");

    pReport->AddItem (6020,"SPS_AddedShotQty",		    "�����ڵ�����");
    pReport->AddItem (6021,"SPS_EmptyShotQty",		    "���ڵ�����");
    pReport->AddItem (6022,"SPS_AbnRcvShotQty",		    "�����쳣�ڵ�����");

    pReport->AddItem (6023,"SPS_FieldRcvQty",		    "ʵ�ʼ첨������");
    pReport->AddItem (6024,"SPS_TheoryRcvQty",		    "���ۼ첨������");
    pReport->AddItem (6025,"SPS_AbnRcvQty",			    "�쳣�첨������");
    pReport->AddItem (6026,"SPS_AbnRcvRatio",		    "�쳣�첨�����(��/��)");
    pReport->AddItem (6027,"SPS_OffsetRcvQty",		    "ƫ�Ƽ첨������");
    pReport->AddItem (6028,"SPS_AddedRcvQty",		    "���Ӽ첨������");
    pReport->AddItem (6029,"SPS_EmptyRcvQty",		    "�ռ첨������");
    pReport->AddItem (6030,"SPS_AbnShotRcvQty",		    "�ڵ��쳣�첨������");

    pReport->AddItem (6031,"SPS_RcvOffsetDoor1",		"�첨��ƫ������ż�1");
    pReport->AddItem (6032,"SPS_RcvOffsetDoor2",		"�첨��ƫ������ż�2");
    pReport->AddItem (6033,"SPS_RcvOffsetDoor3",		"�첨��ƫ������ż�3");
    pReport->AddItem (6034,"SPS_RcvOffsetDoor4",		"�첨��ƫ������ż�4");
    pReport->AddItem (6035,"SPS_RcvOffsetDoor5",		"�첨��ƫ������ż�5");
    pReport->AddItem (6036,"SPS_RcvOffsetDoor6",		"�첨��ƫ������ż�6");

    pReport->AddItem (6037,"SPS_RcvOffsetQty1",		    "ƫ��첨������1-2");
    pReport->AddItem (6038,"SPS_RcvOffsetQty2",		    "ƫ��첨������2-3");
    pReport->AddItem (6039,"SPS_RcvOffsetQty3",		    "ƫ��첨������3-4");
    pReport->AddItem (6040,"SPS_RcvOffsetQty4",		    "ƫ��첨������4-5");
    pReport->AddItem (6041,"SPS_RcvOffsetQty5",		    "ƫ��첨������5-6");

    pReport->AddItem (6042,"SPS_FullCoverageOriginal",  "�������������");
    pReport->AddItem (6043,"SPS_FullCoverageModified",  "���������");
    pReport->AddItem (6044,"SPS_WorkArea",              "ʩ�����");
    pReport->AddItem (6045,"SPS_CoverageArea",          "һ�θ������");

    pReport->AddItem (6046,"SPS_2DLineLength",          "��ά���߳���");
    pReport->AddItem (6047,"SPS_2DSingleFoldLength",    "һ�θ��ǳ���");
    pReport->AddItem (6048,"SPS_2DFullFoldLength",      "�����ǳ���");


}

bool AddReport(CString sArea, CString sReportName)
{
    CFHMainFrm *pFrame=( CFHMainFrm *)AfxGetMainWnd();
    pFrame->AddReport(sArea,sReportName);

    return true;
}


bool AddLine(CString sArea, CString sLineName, CRuntimeClass *pRuntimeClass)
{
    CFHMainFrm *pFrame=( CFHMainFrm *)AfxGetMainWnd();
    pFrame->AddLine(sArea,sLineName, pRuntimeClass);

    return true;
}