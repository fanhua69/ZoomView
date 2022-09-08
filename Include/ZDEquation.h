// ZDEquation.h: interface for the CZDEquation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZDEQUATION_H__72B37B17_F520_11D3_AEF5_00C04FCCB957__INCLUDED_)
#define AFX_ZDEQUATION_H__72B37B17_F520_11D3_AEF5_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define RCV_LIMIT 1000
#define HALF_RCV_LIMIT 500
#define QUARTER_RCV_LIMIT 250
#define EIGHTH_RCV_LIMIT 125

#define SHOT_LIMIT 100

#include "zoomview.h"
#include "MyPoint.h"
#include "MemEqua.h"

class CCouple
{
public:
	long nRcvA;
	long nRcvB;

	CCouple()
	{
		nRcvA=-1;
		nRcvB=-1;
	};
};

class CShotPoint
{
public:
	long lNo;
	CMyPoint Pos;

	CShotPoint()
	{
		lNo=0;
	};
};

class CRcvPoint
{
public:
	long lNo;
	CMyPoint Pos;
	double dDistance;
	float Fbk;

	CRcvPoint()
	{
		lNo=0;
		dDistance=0;
		Fbk=0.0;
	};

	void operator = (CRcvPoint& other)
	{
		lNo=other.lNo ;
		Pos=other.Pos ;
		dDistance=other.dDistance ;
		Fbk=other.Fbk ;
		return;
	};
};

class CShotRcv
{
public:
	// Shot point par:
	CShotPoint Shot;

	// Recieve point par:
	long lRcvNum;
	CRcvPoint Rcv[RCV_LIMIT];
};

class _FHZOOMVIEW_EXT_CLASS CZDEquation  
{
public:
	CZDEquation();
	virtual ~CZDEquation();

	bool Construct(CString sA,CString sB,long nShotNum,long nRcvNum,float fInitVel);
	bool Append(CShotRcv *pShotRcv,int nShotRcv);
	
	void SetFoldTime(int nFoldTime);
	bool Close();

	void Reset();

private:
	bool CalcCouple(CShotRcv *pShotRcv,int nShotRcv);
	long SearchCoupleData(long nRcv,CCouple *pCouple,long nCoupleNumber);


	int m_nFoldTime;  // =1,2,3

	long m_nCoupleNumber[SHOT_LIMIT];
	long m_nSecondCoupleNumber[SHOT_LIMIT];
	long m_nThirdCoupleNumber[SHOT_LIMIT];

	CCouple m_Couple[SHOT_LIMIT][HALF_RCV_LIMIT];
	CCouple m_SecondCouple[SHOT_LIMIT][QUARTER_RCV_LIMIT];
	CCouple m_ThirdCouple[SHOT_LIMIT][EIGHTH_RCV_LIMIT];
	
	CMemEquation m_Equation;
	CString m_sEquationA;
	CString m_sEquationB;

	long m_lShotPointNumber;
	long m_lRcvPointNumber;
	float m_fInitVel;
};

#endif // !defined(AFX_ZDEQUATION_H__72B37B17_F520_11D3_AEF5_00C04FCCB957__INCLUDED_)
