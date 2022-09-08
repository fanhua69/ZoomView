#pragma once


#include "zoomview.h"

class _FHZOOMVIEW_EXT_CLASS CSortList : public CListCtrl
{
	DECLARE_DYNAMIC(CSortList)

public:
	CSortList();
	virtual ~CSortList();

	bool m_fAsc;						//�Ƿ�˳������
	int m_nSortedCol ;					//��ǰ�������
	UINT m_nCallBackFunction;		// �ص�����

	void SetCallBack(UINT nCallBack);
	void SortColumn(int nColumn);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult);
};


