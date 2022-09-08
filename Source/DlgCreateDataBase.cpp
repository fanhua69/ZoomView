// .\source\DlgCreateDataBase.cpp : implementation file
//

#include "stdafx.h"
#include "zoomview.h"
#include "DlgCreateDataBase.h"
#include "FHApp.h"
#include "..\include\dlgcreatedatabase.h"
#include "FHTreeView.h"

#include "objbase.h"


// CDlgCreateDataBase dialog

IMPLEMENT_DYNAMIC(CDlgCreateDataBase, CDialog)
CDlgCreateDataBase::CDlgCreateDataBase(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCreateDataBase::IDD, pParent)
	, m_sPath(_T(""))
	, m_sArea1(_T(""))
	, m_sArea2(_T(""))
	, m_sArea3(_T(""))
{
	GetPar("Dlg_CreateDataBase_Path",m_sPath);
	GetPar("Dlg_CreateDataBase_Area1",m_sArea1);
	GetPar("Dlg_CreateDataBase_Area2",m_sArea2);
	GetPar("Dlg_CreateDataBase_Area3",m_sArea3);
}

CDlgCreateDataBase::~CDlgCreateDataBase()
{
	AddPar("Dlg_CreateDataBase_Path",m_sPath);
	AddPar("Dlg_CreateDataBase_Area1",m_sArea1);
	AddPar("Dlg_CreateDataBase_Area2",m_sArea2);
	AddPar("Dlg_CreateDataBase_Area3",m_sArea3);
}

void CDlgCreateDataBase::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EditPath, m_sPath);
	DDX_Text(pDX, IDC_EditArea1, m_sArea1);
	DDX_Text(pDX, IDC_EditArea2, m_sArea2);
	DDX_Text(pDX, IDC_EditArea3, m_sArea3);
}


BEGIN_MESSAGE_MAP(CDlgCreateDataBase, CDialog)
	ON_BN_CLICKED(IDC_ButtonPath, OnBnClickedButtonpath)
END_MESSAGE_MAP()


// CDlgCreateDataBase message handlers

void CDlgCreateDataBase::OnBnClickedButtonpath()
{
	// TODO: Add your control notification handler code here
	CString s=::GetChosenFolder("选择数据库路径",m_hWnd);
	if(s!="")
	{
		m_sPath=s;
		UpdateData(false);
	}
	
}

bool CreateAllDataBases()
{
	CDlgCreateDataBase dlg;
	if(dlg.DoModal ()==IDCANCEL)return false;

	if(dlg.m_sPath =="")return false;
	if(dlg.m_sArea1 ==""&&dlg.m_sArea2==""&&dlg.m_sArea3=="")
	{
		return false;
	}

	CObList *pList=((CFHApp*)AfxGetApp())->m_treeManager .GetTreeList ();
	POSITION pos=pList->GetHeadPosition ();
	while(pos)
	{
        CTreeProperty *pTreeProperty=(CTreeProperty *)pList->GetNext (pos);
		CFHTreeView *pView=(CFHTreeView *)pTreeProperty->pTree;

		if(!pView->CreateDataBase(dlg.m_sPath))break;
		if(dlg.m_sArea1!="")
		{
			if(!pView->InsertArea (dlg.m_sArea1 ))break;
		}

		if(dlg.m_sArea2!="")
		{
			if(!pView->InsertArea (dlg.m_sArea2 ))break;
		}

		if(dlg.m_sArea3!="")
		{
			if(!pView->InsertArea (dlg.m_sArea3 ))break;
		}
	}

	return true;
}

CString GetChosenFolder(char sDiscription[],HWND hWnd)
{
	HRESULT hResult=CoInitialize(NULL);
	if(hResult==S_FALSE ||hResult==RPC_E_CHANGED_MODE)
	{
		/*	S_FALSE 
				The COM library is already initialized on this thread. 
			RPC_E_CHANGED_MODE 
				A previous call to CoInitializeEx specified the concurrency model for this thread as multithread apartment (MTA). If running Windows 2000, this could also mean that a change from neutral-threaded apartment to single-threaded apartment occurred. 
		*/
	}

	CString strResult = "";
	LPMALLOC lpMalloc; //pointer to lMaloc
	if(::SHGetMalloc(&lpMalloc)!=NOERROR)
	{
		AfxMessageBox("错误：路径操作失败");
		return CString("");
	}

	char szDisplayName[_MAX_PATH];
	char szBuffer[_MAX_PATH];
	BROWSEINFO browseInfo;
	browseInfo.hwndOwner = hWnd;

	// Set rootat desktop:
	browseInfo.pidlRoot =NULL;
	browseInfo.pszDisplayName =szDisplayName;
	browseInfo.lpszTitle =sDiscription; //"请选择路径";
	browseInfo.ulFlags =BIF_RETURNFSANCESTORS|BIF_RETURNONLYFSDIRS|BIF_EDITBOX|BIF_NEWDIALOGSTYLE;
	browseInfo.lpfn = NULL;// not used;
	browseInfo.lParam =0; //NOT used;

	LPITEMIDLIST lpItemIDList=NULL;
	lpItemIDList=::SHBrowseForFolder (&browseInfo);
	if(lpItemIDList!=NULL)
	{
		//Get the path of the selected folder from the itemID list:
		if(::SHGetPathFromIDList (lpItemIDList,szBuffer))
		{
			// At this point, szbuffer contains the path the user chose:
			if(szBuffer[0]=='\0')
			{
				//SHGetPathFromIDList failed, or SHBrowseForFolder failed:
				AfxMessageBox("错误：无法获得路径!",MB_ICONSTOP|MB_OK);
				return CString("");
			}

			// We have a path inszbuffer ! return it!
			strResult=szBuffer;
		}
		else
		{
			// Thething referred to by lpItemIDList 
			// might not have beena file system object,
			//For whatever reason, SHGetPathFromIDList didnot work!
			AfxMessageBox("错误：获取路径失败",MB_ICONSTOP|MB_OK);
			return CString("");
		}

		lpMalloc->Free(lpItemIDList);
		lpMalloc->Release ();
	}

	return strResult;
}
