#pragma once


#include "SortedListCtrl.h"
#include "zoomview.h"
#include <list>

class CStringListItem
{
public:
	std::list <char *>m_list;

	CStringListItem()
	{
	};

	~CStringListItem()
	{
		std::list <char *>::iterator p;
		
		char *pItem=NULL;
		for ( p = m_list.begin(); p!= m_list.end(); ++p )
		{
			pItem=*p;
			delete pItem;
		}

		m_list.clear();
	};

	void AppendItem(char s[])
	{
		char *pNewItem=new char [strlen(s)+1];
		strcpy(pNewItem,s);
		m_list.push_back(pNewItem);
	};

	char * GetAt(int nPos)
	{
		if(nPos<0||nPos>=m_list.size ())
		{
			return NULL;
		}

		std::list <char *>::iterator p;
		
		char *pItem=NULL;
		int n=0;
		for ( p = m_list.begin(); p!= m_list.end(); ++p )
		{
			pItem=*p;
			if(n==nPos)
			{
				return pItem;
			}
			n++;
		}

		return NULL;
	};
};

class _FHZOOMVIEW_EXT_CLASS CStringListCtrl : public CSortedListCtrl
{
	DECLARE_DYNAMIC(CStringListCtrl)

public:
	CStringListCtrl();
	virtual ~CStringListCtrl();
	void DeleteSelected();

	afx_msg void OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult);

	int CompareItems(CItemInfo *pItemInfo1, CItemInfo *pItemInfo2);

	int InsertRowItem ( int nPos,CStringListItem *pItem);
	int AppendRowItem ( CStringListItem *pItemInfo);
	void AutoSize();
	void SetColName(CStringArray &arrayColName);
	void SetIncCol(bool bIncCol);


protected:
	bool m_bIncreaseColQty;
	CStringArray m_arrayColNames;

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDeleteItem(NMHDR* pNMHDR, LRESULT* pResult);
};


