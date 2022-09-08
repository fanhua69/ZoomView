#include "stdafx.h"
#include "reportreader.h"
#include "FHMainFrm.h"

CReportReader::CReportReader(void)
{
	m_iLineValueQty=0;
	m_iReportNo=0;
}

CReportReader::~CReportReader(void)
{
}

void  CReportReader::GetItemValue(int iHeadNo,int &iValue)
{
	char *pData=GetItemValue(iHeadNo);
	sscanf(pData,"%d",&iValue);
	return;
}

void  CReportReader::GetItemValue(int iHeadNo,float &fValue)
{
	char *pData=GetItemValue(iHeadNo);
	sscanf(pData,"%f",&fValue);
	return;
}

void  CReportReader::GetItemValue(int iHeadNo,double &lfValue)
{
	char *pData=GetItemValue(iHeadNo);
	sscanf(pData,"%lf",&lfValue);
	return;
}

CString CReportReader::GetManyItemValue(int iHeadNo)
{
	std::list <CReportItem*>::iterator p;

	CReportItem *pData=NULL;
	CString sReturn="";

	int iHeadNoThis;
	for ( p = m_listReport.begin(); p!= m_listReport.end(); ++p )
	{
		pData=*p;
		sscanf(pData->sHeadNo ,"%d",&iHeadNoThis);
		if(iHeadNoThis==iHeadNo)
		{
			if(sReturn!="")
			{
				sReturn+=",";
			}
			sReturn+=pData->sContent ;
		}
	}

	return sReturn;
}

char * CReportReader::GetItemValue(int iHeadNo)
{

	std::list <CReportItem*>::iterator p;

	CReportItem *pData=NULL;
	int iHeadNoThis;
	for ( p = m_listReport.begin(); p!= m_listReport.end(); ++p )
	{
		pData=*p;
		sscanf(pData->sHeadNo ,"%d",&iHeadNoThis);
		if(iHeadNoThis==iHeadNo)
		{
			return pData->sContent ;
		}
	}

	return NULL;
}

void CReportReader::Over()
{
	CReportBase::Over();

	if(m_fp)
	{
		fclose(m_fp);
		m_fp=NULL;
	}
}

bool CReportReader::InitFile()
{
	Over();

	if(m_sFileName=="")return false;

	m_fp=fopen(m_sFileName,"rt");
	if(!m_fp)
	{
		TRACEERROR("错误：无法打开报告文件："+m_sFileName);
		return false;
	}

	CReportItem item;
	while(!feof(m_fp))
	{
		fgets(m_sLine,1000,m_fp);

		if(PickItemFromLine(m_sLine,&item))
		{
			CReportItem *pItem=new CReportItem;
			memcpy(pItem,&item,sizeof(CReportItem));
			m_listReport.push_back (pItem);
		}

		PickPosFromLine(m_sLine);
	}

	fclose(m_fp);

	PickReportNoFromFile();

	return true;
}

int CReportReader::GetReportNo()
{
	return m_iReportNo;
}

bool CReportReader::StartGetValue()
{
	if(m_fp)
	{
		fclose(m_fp);
		m_fp=NULL;
	}

	m_fp=fopen(m_sFileName,"rt");
	if(!m_fp)
	{
		TRACEERROR("错误：无法打开报告文件："+m_sFileName);
		return false;
	}

	return true;
}

CStringArray * CReportReader::GetValue(UINT nType)
{
	if(!m_fp)
	{
		m_fp=fopen(m_sFileName,"rt");
		if(!m_fp)
		{
			TRACEERROR("错误：无法打开报告文件："+m_sFileName);
			return NULL;
		}
	}

	CStringArray *pArray=NULL;
	while(true)
	{
		fgets(m_sLine,1000,m_fp);

		pArray=PickValueFromLine(m_sLine,nType);
		if(!pArray&&!feof(m_fp))
		{
			continue;
		}
		break;
	}

	return pArray;
}

CStringArray * CReportReader::PickValueFromLine(char sLine[],UINT nType)
{
	m_arrayLineItem.RemoveAll ();
	
	if(sLine[0]!=m_cDataMarkChar)
	{
		return NULL;
	}

	if(nType&LT_SHOT)
	{
		if(sLine[1]!=m_cShotMarkChar)
		{
			return NULL;
		}
	}

	if(nType&LT_RCV)
	{
		if(sLine[1]!=m_cRcvMarkChar)
		{
			return NULL;
		}
	}

	if(nType&LT_BOX)
	{
		if(sLine[1]!=m_cBoxMarkChar)
		{
			return NULL;
		}
	}

	if(nType&LT_PROPERTY)
	{
		if(sLine[1]!=m_cPropertyMarkChar)
		{
			return NULL;
		}
	}

	int nLen=strlen(sLine);
	int i=0;
	int iStart=0;
	int iEnd=0;

	m_arrayLineItem.RemoveAll ();
	m_arrayLineItem.SetSize(m_iLineValueQty);
	char cItem[200];
	CString s;
	for(i=0;i<m_iLineValueQty+1;i++)
	{
		iStart=m_nValueStartPos [i];
		if(iStart>=nLen)
		{
			TRACEERROR("该报告字段起始位置标定错误,请查证行:"+CString(m_cPosMarkChar));
			return &m_arrayLineItem;
		}
	}

	for(i=0;i<m_iLineValueQty;i++)
	{
		iStart=m_nValueStartPos [i];
		iEnd=m_nValueStartPos [i+1];
		if(i==m_iLineValueQty-1)
		{
			iEnd=nLen;
		}
		if(iEnd-iStart>=200)break;

		memset(cItem,0,200);
		memcpy(cItem,&sLine[iStart],iEnd-iStart);
		s=cItem;
		s.Trim ();
		m_arrayLineItem.SetAt (i,s);
	}

	return &m_arrayLineItem;
}

int CReportReader::GetLineValueQty()
{
	return m_iLineValueQty;
}




bool CReportReader::PickReportNoFromFile()
{
	if(m_sFileName=="")return false;
	if(m_fp)
	{
		fclose(m_fp);
	}
	m_fp=fopen(m_sFileName,"rt");
	m_iReportNo=0;

	while(!feof(m_fp))
	{
		fgets(m_sLine,1000,m_fp);
		if(m_sLine[0]==m_cReportNameMarkChar)
		{
			sscanf(&m_sLine[1],"%d",&m_iReportNo);
			break;
		}
	}
	fclose(m_fp);

	return m_iReportNo!=0;
}


bool CReportReader::PickPosFromLine(char sLine[])
{
	if(sLine[0]!=m_cPosMarkChar)
	{
		return false;
	}

	m_iLineValueQty=0;

	int nLen=strlen(sLine);
	int n=0;
	for(int i=1;i<nLen;i++)
	{
		if(sLine[i]!=' ')
		{
			m_nValueStartPos[n]=i;
			n++;
		}
	}
	m_iLineValueQty=n;

	return true;
}

bool CReportReader::PickItemFromLine(char sLine[], CReportItem * pItem)
{
	if(sLine[0]!='H')
	{
		return false;
	}

	char sItem[1000];
	memset(sItem,0,1000);
	memcpy(sItem,&sLine[1],m_iHeadLen);

	int iHeadNo=0;
	sscanf(sItem,"%d",&iHeadNo );

	CReportItem *pBaseItem=SearchItem(iHeadNo);
	if(!pBaseItem)
	{
		return false;
	}

	memcpy(pItem,pBaseItem,sizeof(CReportItem));
	
	CString s=sLine;
	s=s.Mid (m_nSingleValueStartPos);
	s.Trim ();
	if(s.Find (',')!=-1)
	{
		s.Remove (' ');
	}
	if(s.GetLength ()>99)
	{
		TRACEERROR("数据太长，截取为99字节:"+s);
		s=s.Left(99);
	}
	strcpy(pItem->sContent,(LPCTSTR)s);

	return true;
}
