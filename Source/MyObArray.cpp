
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

/////////////////////////////////////////////////////////////////////////////
//
// Implementation of parameterized Array
//
/////////////////////////////////////////////////////////////////////////////
// NOTE: we allocate an array of 'm_nMaxSize' elements, but only
//  the current size 'm_nSize' contains properly constructed
//  objects.

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "stdafx.h"
#include "MyObArray.h"

/////////////////////////////////////////////////////////////////////////////

CMyObArray::CMyObArray()
{
	m_bNeedDelete=true;
}

CMyObArray::~CMyObArray()
{
	// Delete the saved elements:
	if(m_bNeedDelete){
		CObject *pObject=NULL;
		for(long i=0;i<GetSize();i++){
			pObject=GetAt(i);
			if(pObject){
				delete pObject;
				SetAt(i,NULL);
			}
		}
	}

	ASSERT_VALID(this);

	delete[] (BYTE*)m_pData;
}

bool CMyObArray::InsertByOrder(CMyPoint *pNewPoint,
							   long nElementExist,
							   int nOrderType /*******************************
											  0: Order by x, 
											  1, order by y, 
											  ********************************/)
{
	//////////////////////////////////////////////
	// If there is NO points in the array:
	if(nElementExist==0)
	{
		SetAtGrow(0,pNewPoint);
		return true;
	}

	//////////////////////////////////////////////
	// Calc the ordering value of the new point:
	double dCmpVNew,dCmpVExist;
	
	if(nOrderType==0)
		dCmpVNew=pNewPoint->x;
	else if(nOrderType==1)
		dCmpVNew=pNewPoint->y;

	/////////////////////////////////////
	// Check if the new value is bigger than the last existed one,
	// which often happened:
	CMyPoint *pPoint=(CMyPoint*)GetAt(nElementExist-1);
	if(!pPoint)return false;

	if(nOrderType==0)
		dCmpVExist=pPoint->x;
	else if(nOrderType==1)
		dCmpVExist=pPoint->y;

	if(dCmpVNew>dCmpVExist)
	{
		SetAtGrow(nElementExist,pNewPoint);
		return true;
	}

	//////////////////////////////////////////////
	// Calc the insertion pos:
	long nInsertPos=nElementExist;
	for(long i=0;i<nElementExist;i++)
	{
		pPoint=(CMyPoint*)GetAt(i);
		if(!pPoint)continue;

		if(nOrderType==0)
			dCmpVExist=pPoint->x;
		else if(nOrderType==1)
			dCmpVExist=pPoint->y;


		if(dCmpVNew<dCmpVExist)
		{
			nInsertPos=i;
			break;
		}
	}
	InsertAt(nInsertPos,pNewPoint);

	return true;
}

//////////////////////////////////////////////////////
// The elements saved in the array will be deleted by 
// the user or this class?
void CMyObArray::SetDelete(bool bDeleteByCMyObArray)
{
	m_bNeedDelete= bDeleteByCMyObArray;

}


bool CMyObArray::RemoveNearest(CMyPoint *pNewPoint,
							   long nElementExist,
							   int nOrderType /*******************************
											  0: Order by x, 
											  1, order by y, 
											  ********************************/)
{
	if(nElementExist==0)return false;

	//////////////////////////////////////////////
	// Calc the ordering value of the new point:
	double dCmpVNew,dCmpVExist;
	
	if(nOrderType==0)
		dCmpVNew=pNewPoint->x;
	else if(nOrderType==1)
		dCmpVNew=pNewPoint->y;

	CMyPoint *pPoint=NULL;
	double dMinDis=10000000000000,dDis;
	long nMinDisPos=-1;
	for(long i=0;i<nElementExist;i++){
		pPoint=(CMyPoint*)GetAt(i);
		if(pPoint==NULL)continue;

		if(nOrderType==0)
			dCmpVExist=pPoint->x;
		else if(nOrderType==1)
			dCmpVExist=pPoint->y;

		dDis=fabs(dCmpVExist-dCmpVNew);
		if(dDis<dMinDis){
			dMinDis=dDis;
			nMinDisPos=i;
		}
	}

	if(nMinDisPos!=-1){
		RemoveAt(nMinDisPos);
		return true;
	}
	else{
		return false;
	}
	
}

