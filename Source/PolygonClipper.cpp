

#include "stdafx.h"
#include "polygonclipper.h"

CPolygonClipper::CPolygonClipper(void)
{
}

CPolygonClipper::~CPolygonClipper(void)
{
}

CPolygonList * CPolygonClipper::Cut()
{
	///////////////////////////////////////////////////////////////
	//
	if(m_listA.GetCount ()<3||m_listB.GetCount ()<3)return NULL;
	
	///////////////////////////////////////////////////////////////
	//

	return NULL;
}

void CPolygonClipper::AddToA(double x,double y)
{
	CPolygonNode * pNode= new CPolygonNode(x,y) ;
	m_listA.AddTail(pNode); 
}
void CPolygonClipper::AddToB(double x,double y)
{
	CPolygonNode * pNode= new CPolygonNode(x,y) ;
	m_listB.AddTail(pNode); 
}

