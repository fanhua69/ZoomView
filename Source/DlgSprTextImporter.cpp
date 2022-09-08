// .\source\DlgSprTextImporter.cpp : implementation file
//

#include "stdafx.h"
#include "zoomview.h"
#include "dlgsprtextimporter.h"
#include "StringExtractor.h"
#include "FHCGlobal.h"
#include "FHMainFrm.h"
#include "dlgsprtextimporter.h"
#include "..\include\dlgsprtextimporter.h"

// CDlgSprTextImporter dialog

IMPLEMENT_DYNAMIC(CDlgSprTextImporter, CDialog)
CDlgSprTextImporter::CDlgSprTextImporter(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSprTextImporter::IDD, pParent)
	, m_bTabKey(FALSE)
	, m_bComma(FALSE)
	, m_bSemiColon(FALSE)
	, m_bBlank(FALSE)
	, m_bOtherKey(FALSE)
	, m_sFileName(_T(""))
	, m_sOtherSeperator("")
	, m_lStartLine(1)
	, m_lEndLine(1000)
	, m_bColon(FALSE)
	, m_bVerticalLine(FALSE)
{
	m_iItemQtyLimit=100;
	int i;
	for(i=0;i<m_iItemQtyLimit;i++)
	{
		memset(m_sItemName[i],0,100);
		m_iItemOrder[i]=-1;
	}
	m_iItemQty=0;

	///////////////////
	//
	m_iHeaderLimit=1000;
	for(i=0;i<1000;i++)
	{
		m_sHeaderName[i]="数据";
	}

	memset(m_sNotSetMark,0,10);
	strcpy(m_sNotSetMark,"_E");
}

CDlgSprTextImporter::~CDlgSprTextImporter()
{
}

void CDlgSprTextImporter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_listSprText, m_listSprText);
	DDX_Control(pDX, IDC_comboItemName, m_comboItemName);
	DDX_Check(pDX, IDC_checkTab, m_bTabKey);
	DDX_Check(pDX, IDC_checkComma, m_bComma);
	DDX_Check(pDX, IDC_checkSemiColon, m_bSemiColon);
	DDX_Check(pDX, IDC_checkBlank, m_bBlank);
	DDX_Check(pDX, IDC_checkOther, m_bOtherKey);
	DDX_Text(pDX, IDC_editCurrentFile, m_sFileName);
	DDX_Text(pDX, IDC_Seperator, m_sOtherSeperator);
	DDX_Text(pDX, IDC_editStartLine, m_lStartLine);
	DDX_Text(pDX, IDC_editEndLine, m_lEndLine);
	DDX_Check(pDX, IDC_checkColon, m_bColon);
	DDX_Check(pDX, IDC_checkVerticalLine, m_bVerticalLine);
}


BEGIN_MESSAGE_MAP(CDlgSprTextImporter, CDialog)
	ON_BN_CLICKED(IDC_buttonOpenFile, OnBnClickedbuttonopenfile)
	ON_WM_CREATE()
	ON_CBN_SELCHANGE(IDC_comboItemName, OnCbnSelchangecomboitemname)
	ON_BN_CLICKED(IDC_checkTab, OnBnClickedSepChar)
	ON_BN_CLICKED(IDC_checkSemiColon, OnBnClickedSepChar)
	ON_BN_CLICKED(IDC_checkBlank, OnBnClickedSepChar)
	ON_BN_CLICKED(IDC_checkComma, OnBnClickedSepChar)
	ON_BN_CLICKED(IDC_checkOther, OnBnClickedSepChar)
	ON_EN_CHANGE(IDC_Seperator, OnBnClickedSepChar)
	ON_BN_CLICKED(IDC_checkColon, OnBnClickedSepChar)
	ON_BN_CLICKED(IDC_checkVerticalLine, OnBnClickedSepChar)
END_MESSAGE_MAP()


// CDlgSprTextImporter message handlers

BOOL CDlgSprTextImporter::OnInitDialog()
{
	CDialog::OnInitDialog();

	for(int i=0;i<m_iItemQty;i++)
	{
		m_comboItemName.AddString (m_sItemName[i]);
	}

	SetSepCheck();

	if(m_sFileName!="")
	{
		SetFileToList(m_sFileName);
	}

	return true;
}

bool CDlgSprTextImporter::SetItemNotSetMark(char sMark[])
{
	if(strlen(sMark)>8)
	{
		sMark[9]=0;
	}

	strcpy(m_sNotSetMark,sMark);

	return true;
}

bool CDlgSprTextImporter::SetItemNotSetMark(float fMark)
{
	if(fMark<-999999999||fMark>999999999)
	{
		ASSERT(false);
		return false;
	}
	sprintf(m_sNotSetMark,"%1.0f",fMark);
	return true;
}

bool CDlgSprTextImporter::GetItemNotSetMark(char *sMarkReturn,int iLenLimit)
{
	if(iLenLimit<strlen(m_sNotSetMark))
	{
		ASSERT(false);
		return false;
	}
	strcpy(sMarkReturn,m_sNotSetMark);
	return true;
}

void CDlgSprTextImporter::SetSepCheck()
{
	char *sSep=m_stringExtractor.GetSepChar();

	////////////////////////////
	//
	if(strchr(sSep,' '))
	{
		m_bBlank=true;
	}
	else
	{
		m_bBlank=false;
	}

	////////////////////////////////
	//
	if(strchr(sSep,','))
	{
		m_bComma=true;
	}
	else
	{
		m_bComma=false;
	}

	/////////////////////////////////
	//
	if(strchr(sSep,9))
	{
		m_bTabKey=true;
	}
	else
	{
		m_bTabKey=false;
	}

	/////////////////////////////////
	//
	if(strchr(sSep,';'))
	{
		m_bSemiColon=true;
	}
	else
	{
		m_bSemiColon=false;
	}

	/////////////////////////////////
	//
	if(strchr(sSep,':'))
	{
		m_bColon=true;
	}
	else
	{
		m_bColon=false;
	}


	/////////////////////////////////
	//
	if(strchr(sSep,'|'))
	{
		m_bVerticalLine=true;
	}
	else
	{
		m_bVerticalLine=false;
	}

	UpdateData(false);

	return;
}


void CDlgSprTextImporter::OnBnClickedbuttonopenfile()
{
	CString sFile=::GetFileForOpen ("文本文件(*.txt)","txt");
	if(sFile=="")return;

	SetFileToList(sFile);

	m_sFileName=sFile;

	UpdateData(false);
}

void CDlgSprTextImporter::SetFileToList(CString sFile)
{
	if(sFile=="")return;
	FILE *fp=fopen(sFile,"rt");
	if(!fp)
	{
		TRACEERROR("错误：无法打开文件："+sFile);
		return;
	}

	////////////////////////////////////
	// 删除表头的所有项：
	int i;
	int nCount = m_listSprText.m_HeaderCtrl .GetItemCount ();
	for (i=0;i < nCount;i++)
	{
		m_listSprText.DeleteColumn (0);
	}

	///////////////////////////////////
	// 删除表中的所有项：
	m_listSprText.DeleteAllItems ();

	///////////////////////////////////
	// 加入文件所有行：
	char sLine[1000];
	long lLineOrder=0;
	int nSubStringQty=0;

	m_listSprText.AddColumn("行号",0);
	while(!feof(fp))
	{
		memset(sLine,0,1000);
		if(!fgets(sLine,1000,fp))break;
		m_stringExtractor.SetString (sLine);

		nSubStringQty=m_stringExtractor.GetSubStringQty ();
		if(nSubStringQty>m_iHeaderLimit)
		{
			nSubStringQty=m_iHeaderLimit;
		}


		for(i=m_listSprText.m_HeaderCtrl.GetItemCount();i<nSubStringQty+1;i++)
		{
			m_listSprText.AddColumn (m_sHeaderName[i],i+1);
		}

		m_listSprText.AddItem (lLineOrder,0,lLineOrder+1);
		for(i=0;i<nSubStringQty;i++)
		{
			m_listSprText.AddItem (lLineOrder,i+1,m_stringExtractor.GetSubString (i));
		}

		lLineOrder++;
	}

	fclose(fp);

	///////////////////////////
	//
	m_lEndLine=lLineOrder;


	/////////////////////////
	//
	for(i=0;i<m_listSprText.m_HeaderCtrl .GetItemCount ();i++)
	{
		m_listSprText.SetColumnWidth ( i, LVSCW_AUTOSIZE);
	}

	///////////////////
	//
	UpdateData(false);

}

int CDlgSprTextImporter::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;

}

bool CDlgSprTextImporter::SetItemName(int iOrder,char sName[])
{
	if(iOrder<0||iOrder>=m_iItemQtyLimit)return false;

	strcpy(m_sItemName[iOrder],sName);

	if(iOrder>=m_iItemQty)
	{
		m_iItemQty=iOrder+1;
	}

	return true;
}

void CDlgSprTextImporter::OnCbnSelchangecomboitemname()
{
	int nCol=m_listSprText.m_HeaderCtrl.GetSortedColumn ();

	if(nCol==-1||nCol==0)return;

	int nComboSel=m_comboItemName.GetCurSel ();
	if(nComboSel==-1)return;

	/////////////////////////////////
	// 记录该项目对应的列元素序号
	m_iItemOrder[nComboSel]=nCol-1;

	////////////////////////////////
	// 记录表头的名称：第一列为行号
	m_sHeaderName[0]="行号";
	int i;
	for(i=1;i<m_iHeaderLimit;i++)
	{
        if(strcmp(m_sHeaderName[i],m_sItemName[nComboSel])==0)
		{
			m_sHeaderName[i]="数据";
		}
	}
	m_sHeaderName[nCol]=m_sItemName[nComboSel];

	///////////////////////////////
	// 设定该列的名称为该项目
	HDITEM  hdi;
	hdi.mask = HDI_TEXT | HDI_FORMAT;
	hdi.fmt = HDF_STRING | HDF_CENTER;

	for(i=0;i<m_listSprText.m_HeaderCtrl.GetItemCount();i++)
	{
		hdi.pszText=m_sHeaderName[i].GetBuffer (0);
		m_listSprText.m_HeaderCtrl.SetItem (i,&hdi);
	}

}

bool CDlgSprTextImporter::ExportSelectedCol(CString sFile)
{
	if(sFile=="")return false;
	if(m_sFileName=="")return false;   

	FILE *fp=fopen(sFile,"wt");
	if(!fp)
	{
		TRACEERROR("错误：无法建立文件:"+sFile);
		return false;
	}

	FILE *fpIn=fopen(m_sFileName,"rt");
	if(!fpIn)
	{
		TRACEERROR("错误：无法打开文件:"+m_sFileName);
		return false;
	}

	char sLine[1000];
	int i;
	char *sSubString=NULL;
	long iLineNumber=1;
	while(fpIn)
	{
		memset(sLine,0,1000);
		if(!fgets(sLine,999,fpIn))
		{
			break;
		}

		if(iLineNumber>=m_lStartLine&&iLineNumber<=m_lEndLine)
		{
			m_stringExtractor.SetString (sLine);

			for(i=0;i<this->m_iItemQty ;i++)
			{
				if(m_iItemOrder [i]!=-1)
				{
					sSubString=m_stringExtractor.GetSubString (m_iItemOrder [i]);
				}
				else
				{
					sSubString=m_sNotSetMark;
				}

				fprintf(fp,"%s ",sSubString);
			}
			fprintf(fp,"\n");
		}
		
		iLineNumber++;
	}

	fclose(fp);
	fclose(fpIn);

	return true;

}

void CDlgSprTextImporter::OnBnClickedSepChar()
{
	UpdateData(true);

	m_stringExtractor.Empty ();
	if(m_bBlank)
	{
		m_stringExtractor.AppendChar (' ');
	}

	if(m_bComma)
	{
		m_stringExtractor.AppendChar (',');
	}

	if(m_bTabKey)
	{
		m_stringExtractor.AppendChar (9);
	}

	if(m_bSemiColon)
	{
		m_stringExtractor.AppendChar (';');
	}

	if(m_bColon)
	{
		m_stringExtractor.AppendChar (':');
	}	
	
	if(m_bVerticalLine)
	{
		m_stringExtractor.AppendChar ('|');
	}	

	if(m_bOtherKey)
	{
		char c=m_sOtherSeperator[0];

		m_stringExtractor.AppendChar (c);
	}

	SetFileToList (m_sFileName );

	UpdateData(false);

}

void CDlgSprTextImporter::OnBnClickedcheckcolon()
{
	// TODO: Add your control notification handler code here
}

void CDlgSprTextImporter::OnBnClickedcheckverticalline()
{
	// TODO: Add your control notification handler code here
}
