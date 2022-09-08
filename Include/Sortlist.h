#pragma once


#include "zoomview.h"

class _FHZOOMVIEW_EXT_CLASS CSortList : public CListCtrl
{
	DECLARE_DYNAMIC(CSortList)

public:
	CSortList();
	virtual ~CSortList();

	bool m_fAsc;						//是否顺序排序
	int m_nSortedCol ;					//当前排序的列
	UINT m_nCallBackFunction;		// 回调函数

	void SetCallBack(UINT nCallBack);
	void SortColumn(int nColumn);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult);
};


