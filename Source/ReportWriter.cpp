
#include "StdAfx.h"
#include "reportwriter.h" 
#include "FHCGlobal.h"
#include "FHMainFrm.h"

CReportWriter::CReportWriter(void)
{
	m_fp=NULL;
	m_nLineLength=100;
}

CReportWriter::~CReportWriter(void)
{
}


bool CReportWriter::WriteItem(char sMark[],CString sValue)
{
	char s[100];
    if(sValue.GetLength()>95)
    {
        sValue=sValue.Left(95)+"...";
    }
	strcpy(s,(LPCTSTR)sValue);
	return WriteItem(sMark,s);
}

bool CReportWriter::WriteItem(char sMark[],int  nValue)
{
	char s[100];
	sprintf(s,"%d",nValue);
	return WriteItem(sMark,s);
}


bool CReportWriter::WriteItem(char sMark[],double dValue,int nSmallNumber, char *pTag)
{
	char s[100];
	strcpy(s,(LPCTSTR)vtos(dValue,nSmallNumber));
	if(pTag)
	{
		strcat(s,pTag);
	}

	return WriteItem(sMark,s);
}

bool CReportWriter::WriteReportName(int iReportNo)
{
	WriteFullLine ('=');

	CReportName *pData=SearchReport(iReportNo);
	if(!pData)
	{
		TRACEERROR("无法找到报表号码:"+vtos(iReportNo));
		return false;
	}
	
	PrepareMidLine(pData->sReportName);
	strncpy(&m_sLine[1],pData->sReportNo ,m_iHeadLen);

	WriteLine(LT_REPORTNAME);

	WriteFullLine ('=');


	return true;
}

bool CReportWriter::WriteItem(char sMark[],char sValue[],bool bWithTitle)
{
	if(!m_fp)
	{
		ASSERT(false);
		return false;
	}

	CReportItem *pItem=SearchItem(sMark);
	if(!pItem)
	{
		return false;
	}

	/////////////////////////////
	//
	ClearLine();
	CString s;
	int nLen=0;
	if(bWithTitle)
	{
		s=pItem->sTitle;
		s.Trim ();
		nLen=s.GetLength ();
		sprintf(&m_sLine[m_nSingleValueStartPos-nLen-2],"%s",(LPCTSTR)s);
	}
	
	s=sValue;	
	s.Trim ();
	sprintf(&m_sLine[m_nSingleValueStartPos],"%s",(LPCTSTR)s);
	memset(&m_sLine[m_nSingleValueStartPos-1],':',1);

	strcpy(m_sLine,"H");
	strcpy(&m_sLine[1],pItem->sHeadNo );
	
	PrepareExport(' ');

	fprintf(m_fp,"%s\n",m_sLine);

	ClearLine();


	return true;

}

bool CReportWriter::InitFile()
{
	if(m_sFileName=="")return false;
	m_fp=fopen(m_sFileName,"wt");
	if(!m_fp)return false;
	return true;
}

char * CReportWriter::PrepareExport(char c)
{
	for(int i=0;i<m_nLineLength;i++)
	{
		if(m_sLine[i]==0)
		{
			m_sLine[i]=c;
		}
	}

	memset(&m_sLine[m_nLineLength],0,2);

	return m_sLine;
}
	
bool CReportWriter::WriteFullLine(char c)
{
	if(!m_fp||m_nLineLength==0)return false;

	memset(m_sLine,c,m_nLineLength);
	memset(m_sLine,m_cNoteMarkChar,1);

	PrepareExport(' ');

	fprintf(m_fp,"%s\n",m_sLine);

	ClearLine();

	return true;
}

bool CReportWriter::ClearLine(char c)
{
	memset(m_sLine,c,m_nLineLength+2);
	return true;
}

bool CReportWriter::WriteLine(char sTitle[],int nValue)
{
	char sValue[100];
	sprintf(sValue,"%d",nValue);
	return WriteLine(sTitle,sValue);
}

bool CReportWriter::WriteLine(char      sTitle[],
                              double    dValue,
                              int       nSmallNumber,
                              char      *psTag)
{
	char sValue[100];
	
	if(nSmallNumber==0)
		sprintf(sValue,"%1.0lf",dValue);
	else if(nSmallNumber==1)
		sprintf(sValue,"%1.1lf",dValue);
	else if(nSmallNumber==2)
		sprintf(sValue,"%1.2lf",dValue);
	else if(nSmallNumber==3)
		sprintf(sValue,"%1.3lf",dValue);
	else if(nSmallNumber==4)
		sprintf(sValue,"%1.4lf",dValue);
	else if(nSmallNumber==5)
		sprintf(sValue,"%1.5lf",dValue);
	else if(nSmallNumber==6)
		sprintf(sValue,"%1.6lf",dValue);
	else
		sprintf(sValue,"%1.7lf",dValue);

	if(psTag)
	{
		strcat(sValue,psTag);
	}

	return WriteLine(sTitle,sValue);
}

bool CReportWriter::WriteLine(CString sTitle,CString sValue)
{
	char sTitleTag[200];
	char sValueTag[200];
	strcpy(sTitleTag,(LPCTSTR)sTitle);
	strcpy(sValueTag,(LPCTSTR)sValue);
	return WriteLine(sTitleTag,sValueTag);
}

bool CReportWriter::WriteLine(char sTitle[],char sValue[])
{
	if(!m_fp||m_nLineLength==0)return false;

	ClearLine();

	CString s(sTitle);
	s.Trim ();
	int nLen=s.GetLength ();
	sprintf(&m_sLine[m_nSingleValueStartPos-nLen-2],"%s",(LPCTSTR)s);
	
	s=sValue;
	s.Trim ();
	sprintf(&m_sLine[m_nSingleValueStartPos],"%s",(LPCTSTR)s);
	
	PrepareExport(' ');

	fprintf(m_fp,"%s\n",m_sLine);

	ClearLine();

	return true;
}

void CReportWriter::MarkNote()
{
	memcpy(m_sLine,&m_cNoteMarkChar,1);
}

bool CReportWriter::KeepValue(int       nOrder,
							  double    dValue,
							  int       nSmallNumber,
							  bool      bError)
{
	char sValue[100];
	if(bError)
	{
		if(nSmallNumber==0)
			sprintf(sValue,"[%1.0lf]",dValue);
		else if(nSmallNumber==1)
			sprintf(sValue,"[%1.1lf]",dValue);
		else if(nSmallNumber==2)
			sprintf(sValue,"[%1.2lf]",dValue);
		else if(nSmallNumber==3)
			sprintf(sValue,"[%1.3lf]",dValue);
		else if(nSmallNumber==4)
			sprintf(sValue,"[%1.4lf]",dValue);
		else if(nSmallNumber==5)
			sprintf(sValue,"[%1.5lf]",dValue);
		else if(nSmallNumber==6)
			sprintf(sValue,"[%1.6lf]",dValue);
		else if(nSmallNumber==-1)
		{
			CString s=vtos(dValue,-1);
			sprintf(sValue,"[%s]",(LPCTSTR)s);
		}
		else 
			sprintf(sValue,"[%1.4e]",dValue);
	}
	else
	{
		if(nSmallNumber==0)
			sprintf(sValue,"%1.0lf",dValue);
		else if(nSmallNumber==1)
			sprintf(sValue,"%1.1lf",dValue);
		else if(nSmallNumber==2)
			sprintf(sValue,"%1.2lf",dValue);
		else if(nSmallNumber==3)
			sprintf(sValue,"%1.3lf",dValue);
		else if(nSmallNumber==4)
			sprintf(sValue,"%1.4lf",dValue);
		else if(nSmallNumber==5)
			sprintf(sValue,"%1.5lf",dValue);
		else if(nSmallNumber==6)
			sprintf(sValue,"%1.6lf",dValue);
		else if(nSmallNumber==-1)
		{
			CString s=vtos(dValue,-1);
			sprintf(sValue,"%s",(LPCTSTR)s);
		}
		else
            sprintf(sValue,"%1.4e",dValue);
	}
	
	KeepValue(nOrder,sValue);

	return true;
}

bool CReportWriter::KeepValue(int nOrder,int nValue)
{
	char sValue[100];
	sprintf(sValue,"%d",nValue);
	return KeepValue(nOrder,sValue);
}

char * CReportWriter::GetLine()
{
	return m_sLine;
}

bool CReportWriter::KeepValue(int nOrder,char sValue[])
{
	CString s(sValue);
	return KeepValue(nOrder,s);
}

bool CReportWriter::KeepValue(int nOrder,CString sValue)
{
	if(!m_fp||m_nLineLength==0)return false;
	if(nOrder>=m_nValueLimit)return false;

	sValue.Trim ();

	sprintf(&m_sLine[m_nValueStartPos[nOrder]],"%s",(LPCTSTR)sValue);

	return true;
}


bool CReportWriter::WriteLine( UINT  eLT)
{
	if(!m_fp||m_nLineLength==0)return false;
	PrepareExport(' ');

	if(eLT&LT_DATA)
	{
		m_sLine[0]=m_cDataMarkChar;
	}
	else if(eLT&LT_NOTE)
	{
		m_sLine[0]=m_cNoteMarkChar;
	}
	else if(eLT&LT_POS)
	{
		m_sLine[0]=m_cPosMarkChar;
	}
	else if(eLT&LT_REPORTNAME)
	{
		m_sLine[0]=m_cReportNameMarkChar;
	}

	if(eLT&LT_SHOT)
	{
		m_sLine[1]=m_cShotMarkChar;
	}
	else if(eLT&LT_RCV)
	{
		m_sLine[1]=m_cRcvMarkChar;
	}
	else if(eLT&LT_PROPERTY)
	{
		m_sLine[1]=m_cPropertyMarkChar;
	}

	else if(eLT&LT_BOX)
	{
		m_sLine[1]=m_cBoxMarkChar;
	}

	fprintf(m_fp,"%s\n",m_sLine);
	ClearLine();
	return true;
}

bool CReportWriter::WriteMidLine(CString s)
{
	char sLine[300];
	strcpy(sLine,(LPCTSTR)s);
	return WriteMidLine(sLine);
}

bool CReportWriter::WriteLeftLine(char sValue[])
{
	if(!m_fp||m_nLineLength==0)
	{
		ASSERT(false);
		return false;
	}
	
	if(strlen(sValue)>m_nLineLength)
	{
		ASSERT(false);
		return false;
	}

	memset(m_sLine,0,m_nLineLength);
	strcpy(m_sLine,sValue);

	fprintf(m_fp,"%s\n",m_sLine);
	ClearLine();

	return true;
}

bool CReportWriter::WriteMidLine(char sValue[])
{
	if(!m_fp||m_nLineLength==0)
	{
		ASSERT(false);
		return false;
	}
	
	PrepareMidLine(sValue);

	fprintf(m_fp,"%s\n",m_sLine);
	ClearLine();

	return true;
}

char * CReportWriter::PrepareMidLine(CString sValue)
{
	char sValueNew[100];
	sprintf(sValueNew,(LPCTSTR)sValue);
	return PrepareMidLine(sValueNew);
}

char * CReportWriter::PrepareMidLine(char sValue[])
{
	memset(m_sLine,' ',m_nLineLength+2);
	int nLen=strlen(sValue);
	if(nLen>=m_nLineLengthLimit)return NULL;

	int nStart=(m_nLineLength-nLen)/2;
	sprintf(&m_sLine[nStart],"%s",sValue);
	sprintf(m_sLine,"%c",m_cNoteMarkChar);

	PrepareExport(' ');
	return m_sLine;
}

void CReportWriter::WritePosition()
{
	ClearLine();
	int i=0;
    CString sColOrder;
	while(m_nValueStartPos [i]!=0)
	{
        sColOrder=vtos(i);
        if(     m_nValueStartPos [i+1]!=0
            &&  (m_nValueStartPos [i+1]-m_nValueStartPos [i])<sColOrder.GetLength())
        {
            sColOrder=vtos(i%10);
        }
		KeepValue(i,sColOrder);
		i++;
	}
	WriteLine(LT_POS);
}

void CReportWriter::Over()
{
	CReportBase::Over();
	if(m_fp)
	{
		fflush(m_fp);
		fclose(m_fp);
		m_fp=NULL;
	}
}
bool CReportWriter::WriteMark(char	sMark[])
{
    PrepareExport();
    strcpy(&m_sLine[m_iHeadLen],sMark);
    WriteLine(LT_NOTE);
    return true;
}

bool CReportWriter::WriteManyItem(char	sMark[],
								  int	*piData,
								  int	iDataQty,
								  int	iDataQtyOneLine)
{
	char sLine[2000];
	char sData[100];
	memset(sLine,0,2000);

	if(iDataQty<1)return true;
	
	int i=0;
	int iLineOrder=0;
	while(true)
	{
		if(i%iDataQtyOneLine!=0)
		{
			strcat(sLine,",\t");
		}
		sprintf(sData,"%d ",piData[i]);
		strcat(sLine,sData);

		i++;
		if((i%iDataQtyOneLine==0&&i!=0)||i>=iDataQty)
		{
			if(!WriteItem(sMark,sLine,iLineOrder==0))
			{
				return false;
			}
			memset(sLine,0,2000);
			iLineOrder++;
		}

		if(i>=iDataQty)
		{
			break;
		}
	}

	return true;

}










