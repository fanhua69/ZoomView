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
		TRACEERROR("错误：数值的索引超限:"+vtos(nOrder));
		return false;
	}
	
	if(nStartPos>m_nLineLengthLimit)
	{
		TRACEERROR("错误：数值起始位置超限:"+vtos(nStartPos));
		return false;
	}

	if(nStartPos<0)
	{
		ASSERT(false);
		TRACEERROR("错误：数值起始位置小于零:"+vtos(nStartPos));
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
    // 通用项目
    pReport->AddItem(0,"Project",					"项目名称");
    pReport->AddItem(1,"Area",						"工区名称");
    pReport->AddItem(2,"Client",					"客    户");
    pReport->AddItem(3,"Contractor",				"承 包 方");
    pReport->AddItem(4,"Crew",						"队    号");
    pReport->AddItem(5,"Year",						"施工年度");
    pReport->AddItem(6,"Line",						"测线名称");
    pReport->AddItem(7,"WorkDate",					"施工日期");
    pReport->AddItem(8,"TestDate",					"测试日期");
    pReport->AddItem(9,"TestTime",					"测试时间");
    pReport->AddItem(10,"TestUnitNo",				"本次测试唯一编号");

    pReport->AddItem(11,"SeisInst",					"地震仪型号");
    pReport->AddItem(12,"FileName",					"文件名称");



    //////////////////////////////////////
    // 单炮监控项目：100
    pReport->AddReportName (10,"参考物理点监控模块物理点频率监控报告");
    pReport->AddReportName (11,"参考物理点监控模块物理点能量监控报告");

    pReport->AddItem(1000,"MonMinFileNumber",		"最小监控文件号");
    pReport->AddItem(1001,"MonMaxFileNumber",		"最大监控文件号");
    pReport->AddItem(1002,"MonMinStation",			"最小监控点号");
    pReport->AddItem(1003,"MonMaxStation",			"最大监控点号");
    pReport->AddItem(1004,"MonMinSwath",			"最小监控线束号");
    pReport->AddItem(1005,"MonMaxSwath",			"最大监控线束号");
    pReport->AddItem(1006,"MonTotalShotQty",		"监控炮点总数");
    pReport->AddItem(1007,"MonLowDoor",				"能量比低门槛值");
    pReport->AddItem(1008,"MonHighDoor",			"能量比高门槛值");
    pReport->AddItem(1009,"MonBadShotQty",			"超限炮点数目");
    pReport->AddItem(1010,"MonBadShotRate",			"超限炮点比例");

    /////////////////////////////////////////
    //  RefMonitor 模块：
    /////////////////////////////////////////
    pReport->AddReportName (12,"炮点资料报告");



    /////////////////////////////////
    // 检波器测试项目：200
    pReport->AddReportName (20,"地震仪测试检波器报告");
    pReport->AddReportName (21,"检波器测试仪测试检波器报告");
    pReport->AddReportName (22,"检波器测试标准报告");

    pReport->AddItem(2001,"GeoTestInst",			"检波器测试仪型号");
    pReport->AddItem(2002,"GeoType",				"检波器型号");
    pReport->AddItem(2003,"GeoNatuFreqRulerValue",	"自然频率标准值");
    pReport->AddItem(2004,"GeoNatuFreqErroPerm",	"自然频率误差限制");
    pReport->AddItem(2005,"GeoResiCooeRulerValue",	"阻尼系数标准值");
    pReport->AddItem(2006,"GeoResiCooeErroPerm",	"阻尼系数误差限制");
    pReport->AddItem(2007,"GeoDistortRulerValue",	"谐波失真标准值");
    pReport->AddItem(2008,"GeoDistortErroPerm",		"谐波失真误差限制");
    pReport->AddItem(2009,"GeoSensRulerValue",		"灵敏度标准值");
    pReport->AddItem(2010,"GeoSensErroPerm",		"灵敏度误差限制");
    pReport->AddItem(2011,"GeoResiRulerValue",		"直流电阻标准值");
    pReport->AddItem(2012,"GeoResiErroPerm",		"直流电阻误差限制");
    pReport->AddItem(2013,"GeoPolaRulerValue",		"极性标准值");
    pReport->AddItem(2014,"GeoInsulateResiRulerValue","绝缘电阻标准值");
    pReport->AddItem(2015,"GeoInsulateResiErroPerm","绝缘电阻误差限值");	
    pReport->AddItem(2016,"GeoPhaseRulerValue",		"相位标准值");
    pReport->AddItem(2017,"GeoPhaseErroPerm",		"相位误差限值");	

    pReport->AddItem(2100,"GeoSerQty",				"检波器串联数目");
    pReport->AddItem(2101,"GeoParQty",				"检波器并联数目");
    pReport->AddItem(2102,"GeoStriSerQty",			"检波器串的串联数目");
    pReport->AddItem(2103,"GeoStriParQty",			"检波器串的并联数目");
    pReport->AddItem(2104,"GeoDistInStri",			"检波器之间小线长度");
    pReport->AddItem(2105,"GeoDistToCabl",			"检波器到大线的长度");
    pReport->AddItem(2106,"GeoStriResiCooe",		"检波器小线电阻率");
    pReport->AddItem(2107,"GeoStartTestLine",		"起始检测线号");
    pReport->AddItem(2108,"GeoEndTestLine",			"终止检测线号");
    pReport->AddItem(2109,"GeoStartTestStation",	"起始检测站号");
    pReport->AddItem(2110,"GeoEndTestStation",		"终止检测站号");

    pReport->AddItem(2200,"GeoInUseQty",			"在用检波器总数目");	
    pReport->AddItem(2201,"GeoTestQty",				"检测检波器总数目");
    pReport->AddItem(2202,"GeoGoodQty",				"检波器完好数目");
    pReport->AddItem(2203,"GeoGoodRate",			"检波器完好率");
    pReport->AddItem(2204,"GeoGoodRateInUse",		"在用检波器完好率");

    pReport->AddItem(2300,"GeoNatuBadQty",			"自然频率不合格数目");
    pReport->AddItem(2301,"GeoNatuBadRate",			"自然频率不合格率");

    pReport->AddItem(2302,"GeoResiCooeBadQty",		"阻尼系数不合格数目");
    pReport->AddItem(2303,"GeoResiCooeBadRate",		"阻尼系数不合格率");

    pReport->AddItem(2304,"GeoSensBadQty",			"灵敏度不合格数目");
    pReport->AddItem(2305,"GeoSensBadRate",			"灵敏度不合格率");

    pReport->AddItem(2306,"GeoResiBadQty",			"直流电阻不合格数目");
    pReport->AddItem(2307,"GeoResiBadRate",			"直流电阻不合格率");

    pReport->AddItem(2308,"GeoDistortionBadQty",	"谐波失真不合格数目");
    pReport->AddItem(2309,"GeoDistortionBadRate",	"谐波失真不合格率");

    pReport->AddItem(2310,"GeoPolaBadQty",			"极性不合格数目");
    pReport->AddItem(2311,"GeoPolaBadRate",			"极性不合格率");

    pReport->AddItem(2312,"GeoPolaBadQty",			"极性不合格数目");
    pReport->AddItem(2313,"GeoPolaBadRate",			"极性不合格率");

    pReport->AddItem(2314,"GeoInsulateResiBadQty",	"绝缘电阻不合格数目");
    pReport->AddItem(2315,"GeoInsulateResiBadRate",	"绝缘电阻不合格率");

    pReport->AddItem(2316,"GeoPhaseBadQty",			"相位不合格数目");
    pReport->AddItem(2317,"GeoPhaseBadRate",		"相位不合格率");

    /////////////////////////////////////
    //
    pReport->AddReportName (30,"静校正量比较报告");
    pReport->AddItem(3000,"CompareFileName",			"比较文件名称");
    pReport->AddItem(3001,"ShotStaticOutThresholdQty",	"炮点静校正量超限数目");
    pReport->AddItem(3002,"RcvStaticOutThresholdQty",	"检波点静校正量超限数目");
    pReport->AddItem(3003,"GDOutThresholdQty",			"高速层顶面超限数目");
    pReport->AddItem(3004,"TotalStationQty",			"站点总数目");
    pReport->AddItem(3005,"ShotOutThresRatio",			"炮点静校正量超限比例");
    pReport->AddItem(3006,"RcvOutThresRatio",			"检波点静校正量超限比例");
    pReport->AddItem(3007,"GDOutThresRatio",			"高速层顶面超限比例");

    ////////////////////////////////
    // 激发因素监测模块：
    pReport->AddReportName (40,"井深检测报告");
    pReport->AddReportName (41,"药量检测报告");

    pReport->AddItem(4000,"EnergyMonitorWellQty",		"监理方设计井数目");
    pReport->AddItem(4001,"EnergyCrewWellQty",			"施工方设计井数目");
    pReport->AddItem(4002,"EnergyFieldWellQty",			"实际井数目");

    pReport->AddItem(4003,"EnergyWellCompareMode",		"井深比较对象");
    pReport->AddItem(4004,"EnergyWellComparedQty",		"井深比较数目");
    pReport->AddItem(4005,"EnergyWellLowerDoor",		"浅井门槛值");
    pReport->AddItem(4006,"EnergyWellHigherDoor",		"深井门槛值");
    pReport->AddItem(4007,"EnergyWellLowerQtyThanCrew",	"比施工方设计浅井数目");
    pReport->AddItem(4008,"EnergyWellHigherQtyThanCrew","比施工方设计深井数目");
    pReport->AddItem(4009,"EnergyWellLowerQtyThanMonitor",	"比监理方设计浅井数目");
    pReport->AddItem(4010,"EnergyWellHigherQtyThanMonitor",	"比监理方设计深井数目");

    pReport->AddItem(4100,"EnergyCompareMode",		    "药量比较对象");
    pReport->AddItem(4101,"EnergyComparedQty",		    "药量比较数目");
    pReport->AddItem(4102,"EnergyLowerDoor",			"低药量门槛值");
    pReport->AddItem(4103,"EnergyHigherDoor",		    "高药量门槛值");
    pReport->AddItem(4104,"EnergyLowerQtyThanCrew",		"比施工方设计少药量数目");
    pReport->AddItem(4105,"EnergyHigherQtyThanCrew",	"比施工方设计多药量数目");
    pReport->AddItem(4106,"EnergyLowerQtyThanMonitor",	"比监理方设计少药量数目");
    pReport->AddItem(4107,"EnergyHigherQtyThanMonitor",	"比监理方设计多药量数目");


    ///////////////////////////////////////
    // 年月日检模块
    pReport->AddReportName (50,"地震勘探仪器年月日检监控报告");
    pReport->AddItem (5000,"DC_CHECK_TYPE",				"检测类型");
    pReport->AddItem (5001,"DC_SAMPLE_INTERVAL",		"采样率");
    pReport->AddItem (5002,"DC_CHECKITEM_NEED_QTY",		"应检测项目");
    pReport->AddItem (5003,"DC_CHECKITEM_QTY",			"实检测项目");
    pReport->AddItem (5004,"DC_BOX_QTY",				"检测采集站数量");
    pReport->AddItem (5005,"DC_BOX_MinNo",				"采集站最小编号");
    pReport->AddItem (5006,"DC_BOX_MaxNo",				"采集站最大编号");

    pReport->AddItem (5007,"DC_BOX_QtyInUse",			"在用采集站数量");
    pReport->AddItem (5008,"DC_BOX_QtyNotChecked",		"未检测采集站数量");
    pReport->AddItem (5009,"DC_BOX_QtyNoNotChecked",	"未检测采集站编号");
    pReport->AddItem (5010,"DC_BOX_QtyNotInUse",		"非在用采集站数量");
    pReport->AddItem (5011,"DC_BOX_QtyNoNotInUse",		"非在用采集站编号");

    pReport->AddItem (5014,"DC_GEOPHONE_QTY",			"接收道数量");
    pReport->AddItem (5015,"DC_BOX_PASSQTY",			"采集站通过数量");
    pReport->AddItem (5016,"DC_GEOPHONE_PASSQTY",		"接收道通过数量");
    pReport->AddItem (5017,"DC_BOX_CHECK_PASSRATE",		"检测采集站通过率(%)");
    pReport->AddItem (5018,"DC_BOX_IN_USE_PASSRATE",	"在用采集站通过率(%)");
    pReport->AddItem (5019,"DC_GEOPHONE_PASSRATE",		"接收道通过率(%)");

    /////////////////////////////////////////
    //  SPS 模块：
    /////////////////////////////////////////
    pReport->AddReportName (60,"SPS文件分析报告");
    pReport->AddItem (6000,"SPS_FieldShotQty",		    "实际炮点总数");
    pReport->AddItem (6001,"SPS_TheoryShotQty",		    "理论炮点总数");
    pReport->AddItem (6002,"SPS_TotalDynamiteQty",	    "总药量");
    pReport->AddItem (6003,"SPS_AverDynamiteQty",	    "平均药量");
    pReport->AddItem (6004,"SPS_TotalWellDepth",		"总井深");
    pReport->AddItem (6005,"SPS_AverWellDepth",		    "平均井深");
    pReport->AddItem (6006,"SPS_AbnShotQtyQty",		    "异常炮点总数");
    pReport->AddItem (6007,"SPS_AbnShotRatio",		    "异常炮点比例(异/理)");
    pReport->AddItem (6008,"SPS_OffsetShotQty",		    "偏移炮点总数");

    pReport->AddItem (6009,"SPS_ShotOffsetDoor1",		"炮点偏离距离门槛1");
    pReport->AddItem (6010,"SPS_ShotOffsetDoor2",		"炮点偏离距离门槛2");
    pReport->AddItem (6011,"SPS_ShotOffsetDoor3",		"炮点偏离距离门槛3");
    pReport->AddItem (6012,"SPS_ShotOffsetDoor4",		"炮点偏离距离门槛4");
    pReport->AddItem (6013,"SPS_ShotOffsetDoor5",		"炮点偏离距离门槛5");
    pReport->AddItem (6014,"SPS_ShotOffsetDoor6",		"炮点偏离距离门槛6");

    pReport->AddItem (6015,"SPS_ShotOffsetQty1",		"偏离炮点数量1-2");
    pReport->AddItem (6016,"SPS_ShotOffsetQty2",		"偏离炮点数量2-3");
    pReport->AddItem (6017,"SPS_ShotOffsetQty3",		"偏离炮点数量3-4");
    pReport->AddItem (6018,"SPS_ShotOffsetQty4",		"偏离炮点数量4-5");
    pReport->AddItem (6019,"SPS_ShotOffsetQty5",		"偏离炮点数量5-6");

    pReport->AddItem (6020,"SPS_AddedShotQty",		    "增加炮点总数");
    pReport->AddItem (6021,"SPS_EmptyShotQty",		    "空炮点总数");
    pReport->AddItem (6022,"SPS_AbnRcvShotQty",		    "排列异常炮点总数");

    pReport->AddItem (6023,"SPS_FieldRcvQty",		    "实际检波点总数");
    pReport->AddItem (6024,"SPS_TheoryRcvQty",		    "理论检波点总数");
    pReport->AddItem (6025,"SPS_AbnRcvQty",			    "异常检波点总数");
    pReport->AddItem (6026,"SPS_AbnRcvRatio",		    "异常检波点比例(异/理)");
    pReport->AddItem (6027,"SPS_OffsetRcvQty",		    "偏移检波点总数");
    pReport->AddItem (6028,"SPS_AddedRcvQty",		    "增加检波点总数");
    pReport->AddItem (6029,"SPS_EmptyRcvQty",		    "空检波点总数");
    pReport->AddItem (6030,"SPS_AbnShotRcvQty",		    "炮点异常检波点总数");

    pReport->AddItem (6031,"SPS_RcvOffsetDoor1",		"检波点偏离距离门槛1");
    pReport->AddItem (6032,"SPS_RcvOffsetDoor2",		"检波点偏离距离门槛2");
    pReport->AddItem (6033,"SPS_RcvOffsetDoor3",		"检波点偏离距离门槛3");
    pReport->AddItem (6034,"SPS_RcvOffsetDoor4",		"检波点偏离距离门槛4");
    pReport->AddItem (6035,"SPS_RcvOffsetDoor5",		"检波点偏离距离门槛5");
    pReport->AddItem (6036,"SPS_RcvOffsetDoor6",		"检波点偏离距离门槛6");

    pReport->AddItem (6037,"SPS_RcvOffsetQty1",		    "偏离检波点数量1-2");
    pReport->AddItem (6038,"SPS_RcvOffsetQty2",		    "偏离检波点数量2-3");
    pReport->AddItem (6039,"SPS_RcvOffsetQty3",		    "偏离检波点数量3-4");
    pReport->AddItem (6040,"SPS_RcvOffsetQty4",		    "偏离检波点数量4-5");
    pReport->AddItem (6041,"SPS_RcvOffsetQty5",		    "偏离检波点数量5-6");

    pReport->AddItem (6042,"SPS_FullCoverageOriginal",  "绝对满覆盖面积");
    pReport->AddItem (6043,"SPS_FullCoverageModified",  "满覆盖面积");
    pReport->AddItem (6044,"SPS_WorkArea",              "施工面积");
    pReport->AddItem (6045,"SPS_CoverageArea",          "一次覆盖面积");

    pReport->AddItem (6046,"SPS_2DLineLength",          "二维测线长度");
    pReport->AddItem (6047,"SPS_2DSingleFoldLength",    "一次覆盖长度");
    pReport->AddItem (6048,"SPS_2DFullFoldLength",      "满覆盖长度");


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