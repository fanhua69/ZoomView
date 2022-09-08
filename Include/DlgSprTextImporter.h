#pragma once


// CDlgSprTextImporter dialog
#include "StringListCtrl.h"
#include "zoomview.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "StringExtractor.h"

class _FHZOOMVIEW_EXT_CLASS CDlgSprTextImporter : public CDialog
{
	DECLARE_DYNAMIC(CDlgSprTextImporter)

public:
	CDlgSprTextImporter(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSprTextImporter();

// Dialog Data
	enum { IDD = IDD_DlgSprTextImporter };
	CString m_sFileName;

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void SetFileToList(CString sFile);
	void SetSepCheck();

	CStringExtractor m_stringExtractor;

	int m_iItemQtyLimit;
	int m_iItemQty;
	char m_sItemName[100][100];
	int m_iItemOrder[100];
	char m_sNotSetMark[10];

	int m_iHeaderLimit;
	CString m_sHeaderName[1000];
	
	CString m_sOtherSeperator;
	CStringListCtrl m_listSprText;

	CComboBox m_comboItemName;
	BOOL m_bTabKey;
	BOOL m_bComma;
	BOOL m_bSemiColon;
	BOOL m_bBlank;
	BOOL m_bOtherKey;
	BOOL m_bVerticalLine;
	BOOL m_bColon;

	DECLARE_MESSAGE_MAP()
public:
	bool SetItemNotSetMark(char sMark[]);
	bool SetItemNotSetMark(float fMark);
	bool GetItemNotSetMark(char *sMarkReturn,int iLenLimit);

	bool SetItemName(int iOrder,char sName[]);
	bool ExportSelectedCol(CString sFile);

	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangecomboitemname();
	afx_msg void OnBnClickedSepChar();
	afx_msg void OnBnClickedchecksemicolon();
	afx_msg void OnBnClickedcheckblank();
	afx_msg void OnBnClickedcheckcomma();
	afx_msg void OnBnClickedcheckother();
	afx_msg void OnBnClickedbuttonopenfile();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	long m_lStartLine;
	long m_lEndLine;
	afx_msg void OnBnClickedcheckcolon();
	afx_msg void OnBnClickedcheckverticalline();
};
