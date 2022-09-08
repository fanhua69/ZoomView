#pragma once

#include <afxtempl.h>

class CPolygonNode
{
public:
	double x;
	double y;
	bool bIntersection;
	bool bUsed;

	CPolygonNode()
	{
		x=0;
		y=0;
		bIntersection=false;
		bUsed=false;
	};

	CPolygonNode(double xx,double yy)
	{
		x=xx;
		y=yy;
		bIntersection=false;
		bUsed=false;
	};

	void Set(double xx,double yy)
	{
		x=xx;
		y=yy;
		bIntersection=false;
		bUsed=false;
	};
};


typedef CList<CPolygonNode*,CPolygonNode*> CPolygonList;

class CPolygonClipper
{
public:
	CPolygonClipper(void);
	~CPolygonClipper(void);

	CPolygonList * Cut();

	void AddToA(double x,double y);
	void AddToB(double x,double y);

protected:
	CPolygonList m_listA;
	CPolygonList m_listB;
	
	CPolygonList m_listResult;

};
