// Hyperbola.cpp: implementation of the CHyperbola class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Hyperbola.h"
#include "FHZoomViewGlobal.h"
#include "Templates.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHyperbola::CHyperbola()
{
	pX=NULL;
	pY=NULL;
	m_nItem=0;
}

CHyperbola::~CHyperbola()
{
	if(pX){
		delete []pX;
		pX=NULL;
	}
	if(pY){
		delete []pY;
		pY=NULL;
	}
}

bool CHyperbola::SetItemNumber(long nItemNumber)
{
	if(nItemNumber<1||nItemNumber>100000)return false;
	if(pX){
		delete []pX;
		pX=NULL;
	}
	pX=new double [nItemNumber+10];
	
	if(pY){
		delete []pY;
		pY=NULL;
	}
	pY=new double [nItemNumber+10];

	return true;
}
bool CHyperbola::SetValue(double *pXOrg,double *pYOrg,long nItem)
{
	if(!pX||!pY||m_nItem<1||nItem<1||nItem>m_nItem)return false;
	for(long i=0;i<nItem;i++){
		pX[i]=pXOrg[i];
		pY[i]=pYOrg[i];
	}
	return true;
}
bool CHyperbola::SetValue(double x,double y,long nItemPos)
{
	if(!pX||!pY||m_nItem<1||nItemPos<0||nItemPos>=m_nItem)return false;
	pX[nItemPos]=x;
	pY[nItemPos]=y;
	return true;
}

bool CHyperbola::Over(double &t0,double &v)
{
	for(long i=0;i<m_nItem;i++){
		pX[i]=pX[i]*pX[i];
		pY[i]=pY[i]*pY[i];
	}

	float a,b;
	MinPower(pX,pY,0,0,m_nItem-1,a,b);

	t0=sqrt(b);
	v=1/(sqrt(a));

	return true;
}
