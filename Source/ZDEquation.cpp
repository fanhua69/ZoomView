// ZDEquation.cpp: implementation of the CZDEquation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ZDEquation.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CZDEquation::CZDEquation()
{
	Reset();
}

CZDEquation::~CZDEquation()
{
	Reset();
}

bool CZDEquation::Construct(CString sA,CString sB,long nShotNum,long nRcvNum,float fInitVel)
{
	if(!m_Equation.IsIdle ())return false;
	if(!m_Equation.Construct(sA,sB,nShotNum+nRcvNum))return false;

	m_sEquationA=sA;
	m_sEquationB=sB;

	m_lShotPointNumber=nShotNum;
	m_lRcvPointNumber=nRcvNum;
	m_fInitVel=fInitVel;

	return true;
}

void CZDEquation::SetFoldTime(int nFoldTime)
{
	if(nFoldTime<1)m_nFoldTime=1;
	if(nFoldTime>3)m_nFoldTime=3;
	m_nFoldTime=nFoldTime;
}

bool CZDEquation::Append(CShotRcv *pShotRcv,int nShotRcv)
{
	if(!m_Equation.IsMaking ())return false;
	CalcCouple(pShotRcv,nShotRcv);

	long nShot,j,k;
	long nCoupleNumber=0;

	CBlockTrace BlockTrace[10];
	long r[10];
	CObArray array;
	float fFbk;

	if(m_nFoldTime==1){  
		array.SetSize (3);
		// The couple number should be same as the fold time, so 
		// we check the data here:

		for(nShot=0;nShot<nShotRcv;nShot++){
			for(j=0;j<m_nCoupleNumber[nShot];j++){  // loop of couples of this shot.
				r[0]=pShotRcv[nShot].Rcv[m_Couple[nShot][j].nRcvA].lNo +m_lShotPointNumber;
				r[1]=pShotRcv[nShot].Rcv[m_Couple[nShot][j].nRcvB].lNo +m_lShotPointNumber;
				
				fFbk=0.0;
				for(k=0;k<2;k++){
					fFbk+=pShotRcv[nShot].Rcv [r[k]].Fbk ;
				}
				
				BlockTrace[0].Set(pShotRcv[nShot].Shot .lNo ,2);
				
				for(k=0;k<2;k++){
					BlockTrace[k].Set(r[k],1);
				}

				for(k=0;k<3;k++){
					array.SetAt(k,&BlockTrace[k]);
				}

				if(!m_Equation.AppeEqua (array,fFbk))return false;
			}
		}
	}
	else if(m_nFoldTime==2){
		array.SetSize (5);
		// The couple number should be same as the fold time, so 
		// we check the data here:

		for(nShot=0;nShot<nShotRcv;nShot++){
			for(j=0;j<m_nCoupleNumber[nShot];j++){  // loop of couples of this shot.
				r[0]=pShotRcv[nShot].Rcv[m_SecondCouple[nShot][j].nRcvA].lNo +m_lShotPointNumber;
				r[1]=pShotRcv[nShot].Rcv[m_SecondCouple[nShot][j].nRcvB].lNo +m_lShotPointNumber;
				
				r[2]=SearchCoupleData(r[0],m_Couple[nShot],m_nCoupleNumber[nShot]);
				r[3]=SearchCoupleData(r[1],m_Couple[nShot],m_nCoupleNumber[nShot]);
				if(r[2]==-1||r[3]==-1)continue;

				fFbk=0.0;
				for(k=0;k<2;k++){
					fFbk+=pShotRcv[nShot].Rcv [r[k]].Fbk ;
				}
				
				BlockTrace[0].Set(pShotRcv[nShot].Shot.lNo ,4);
				
				for(k=0;k<4;k++){
					BlockTrace[k].Set(r[k],1);
				}

				for(k=0;k<5;k++){
					array.SetAt(k+1,&BlockTrace[k]);
				}

				if(!m_Equation.AppeEqua (array,fFbk))return false;
			}
		}
	}
	// make Three times fold .
	else if(m_nFoldTime==3){
		array.SetSize (9);

		// The couple number should be same as the fold time, so 
		// we check the data here:
		for(nShot=0;nShot<nShotRcv;nShot++){
			for(j=0;j<m_nThirdCoupleNumber[nShot];j++){  
				r[0]=pShotRcv[nShot].Rcv[m_ThirdCouple[nShot][j].nRcvA].lNo +m_lShotPointNumber;
				r[1]=pShotRcv[nShot].Rcv[m_ThirdCouple[nShot][j].nRcvB].lNo +m_lShotPointNumber;
				r[2]=SearchCoupleData(r[0],m_SecondCouple[nShot],m_nSecondCoupleNumber[nShot]);
				r[3]=SearchCoupleData(r[1],m_SecondCouple[nShot],m_nSecondCoupleNumber[nShot]);
				r[4]=SearchCoupleData(r[0],m_Couple[nShot],m_nCoupleNumber[nShot]);
				r[5]=SearchCoupleData(r[1],m_Couple[nShot],m_nCoupleNumber[nShot]);
				r[6]=SearchCoupleData(r[2],m_Couple[nShot],m_nCoupleNumber[nShot]);
				r[7]=SearchCoupleData(r[3],m_Couple[nShot],m_nCoupleNumber[nShot]);
				if(r[2]==-1||r[3]==-1||r[4]==-1||r[5]==-1||r[6]==-1||r[7]==-1)continue;
					

				fFbk=0.0;
				for(k=0;k<8;k++){
					fFbk+=pShotRcv[nShot].Rcv [r[k]].Fbk ;
				}
				
				BlockTrace[0].Set(pShotRcv[nShot].Shot.lNo ,4);
				
				for(k=1;k<8;k++){
					BlockTrace[k].Set(r[k-1],1);
				}

				for(k=0;k<9;k++){
					array.SetAt(k,&BlockTrace[k]);
				}

				if(!m_Equation.AppeEqua (array,fFbk))return false;
			}
		}
	}

	return true;
}

bool CZDEquation::Close()
{
	Reset();
	return true;
}

void CZDEquation::Reset()
{
	if(!m_Equation.IsIdle())m_Equation.Close ();

	m_sEquationA="";
	m_sEquationB="";

	m_lShotPointNumber=0;
	m_lRcvPointNumber=0;

	m_fInitVel=2000;

	for(int i=0;i<SHOT_LIMIT;i++){
		m_nCoupleNumber[i]=0;
		m_nSecondCoupleNumber[i]=0;
		m_nThirdCoupleNumber[i]=0;
	}

	m_nFoldTime=1;

}

bool CZDEquation::CalcCouple(CShotRcv *pShotRcv,int nShotRcv)
{
	long  i,j,k;
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//    Maximum permitted unformed group of one shot:         1000
	//    NumberLength : recorded the number of the unformed r-points 
	// and their distance from the shot point,  its not a calss member.
	double dx,dy;
	for(i=0;i<nShotRcv;i++){  // 对炮点循环。		
		for(j=0;j<pShotRcv[i].lRcvNum ;j++){  // 对检波线循环。
			dx=pShotRcv[i].Shot.Pos.x-pShotRcv[i].Rcv[j].Pos.x;
			dy=pShotRcv[i].Shot.Pos.y-pShotRcv[i].Rcv[j].Pos.y;
			pShotRcv[i].Rcv[j].dDistance=sqrt(dx*dx+dy*dy);
			pShotRcv[i].Rcv[j].Fbk -=pShotRcv[i].Rcv[j].dDistance/m_fInitVel;
		}
	}
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//       Sort   NumberLength according to the distance 
	//  of the recieve points to the shot by every shot.
	CRcvPoint swap;
	double dTempLength;
	for(i=0;i<nShotRcv;i++){
		for(j=0;j<pShotRcv[i].lRcvNum ;j++){

			swap=pShotRcv[i].Rcv [j];
			dTempLength=pShotRcv[i].Rcv [j].dDistance ;		

			for(k=j;k>0;k--){
				if(pShotRcv[i].Rcv [k-1].dDistance >dTempLength){
					pShotRcv[i].Rcv [k]=pShotRcv[i].Rcv [k-1];
				}
				else{
					break;
				}
			}
			pShotRcv[i].Rcv [k]=swap;
		}
	}
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Because the distances of the r-p to the shot of all of the shots must be same,
	// so when plusing two R-P that is same far from their shot point, so must search 
	// for the distance that all of the shot have .
	// 1.Search for Longest distance of the nearest Unform r-point in all of the shots.
	// 2.Search for Shortest distance of the farest Unform r-point in all of the shots.
	// 3. Calculate the middle distance that split the first half plused group and the second hlf.
	double 	SmallDistance=pShotRcv[0].Rcv [0].dDistance ;
	double 	BigDistance=pShotRcv[0].Rcv [pShotRcv[0].lRcvNum -1].dDistance ;
	for(i=0;i<nShotRcv;i++){
		if(pShotRcv[i].Rcv [0].dDistance >SmallDistance){
			SmallDistance=pShotRcv[i].Rcv [0].dDistance ;
		}

		if(pShotRcv[i].Rcv [pShotRcv[i].lRcvNum -1].dDistance <BigDistance){
			BigDistance=pShotRcv[i].Rcv [pShotRcv[i].lRcvNum -1].dDistance ;
		}
	}	
	
	double MidDistance=(BigDistance-SmallDistance)/2+SmallDistance;	
	double QuaDistance=(BigDistance-SmallDistance)/4+SmallDistance;	   
	double EighthDistance=(BigDistance-SmallDistance)/8+SmallDistance;
																								
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//    Calculate couple data to be plused , according distance.
	//    m_CouplePlused : recording the two r-point number that will be plused.
	//Make 3 copies of the below, to avoid too many parameters if make it a subroutine.

	double dsmall,dbig1,dbig2,d1,d2;  // Temporary variable.
	int num;  // stand for number of plused couples of any shot point .
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 1. Calculate the first plused couple according to the Middle distance.
	for(i=0;i<nShotRcv;i++){
		num=0;
		for(j=0;j<pShotRcv[i].lRcvNum ;j++){
			
			// diffrence of the the R point and the SmallDistance.
			dsmall=pShotRcv[i].Rcv [j].dDistance -SmallDistance;
			if(dsmall<0)continue;
			if(pShotRcv[i].Rcv [j].dDistance >MidDistance){
				m_nCoupleNumber[i]=num;
				break;		
			}

			m_Couple[i][num].nRcvA=pShotRcv[i].Rcv [j].lNo ;

			//    search for the second R point whose diffrence with BigDistance 
			// match the dsmall.
			for(k=pShotRcv[i].lRcvNum -1;k>0;k--){   // should from sp._ to 0, see below.
				dbig1=BigDistance-pShotRcv[i].Rcv[k].dDistance ;
				dbig2=BigDistance-pShotRcv[i].Rcv[k-1].dDistance ;
				
				// Need not consider of dbig2<0, for dsmall>0.
				if(dbig2>=dsmall&&dbig1<=dsmall){
					d1=fabs(dsmall-dbig1);
					d2=fabs(dbig2-dsmall);
					m_Couple[i][num].nRcvB=(d1<d2)?pShotRcv[i].Rcv[k].dDistance :pShotRcv[i].Rcv[k-1].dDistance;
					num++;
					break;
				}
			}
			
		}  // R point loop.
		
	}  // shot point loop to calculate the number of the two plused r-point.
	
	if(m_nFoldTime<2)return true;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 2. Calculate the second plused couple between the QuaDistance 
	//      on the first half distance.	
	BigDistance=MidDistance;
	for(i=0;i<nShotRcv;i++){    // Loop for all of the shots.
		num=0;
		for(j=0;j<pShotRcv[i].lRcvNum ;j++){  // Loop for all of the recieve points.
			
			// diffrence of the the R point and the SmallDistance.
			dsmall=pShotRcv[i].Rcv [j].dDistance -SmallDistance;
			if(dsmall<0)continue;
			if(pShotRcv[i].Rcv [j].dDistance >QuaDistance){
				m_nSecondCoupleNumber[i]=num;
				break;		
			}
			m_SecondCouple[i][num].nRcvA=pShotRcv[i].Rcv [j].dDistance ;

			//    search for the second R point whose diffrence with BigDistance 
			// match the dsmall.
			for(k=pShotRcv[i].lRcvNum -1;k>0;k--){   // should from sp._ to 0, see below.
				dbig1=BigDistance-pShotRcv[i].Rcv [k].dDistance ;
				dbig2=BigDistance-pShotRcv[i].Rcv [k-1].dDistance ;
				
				// Need not consider of dbig2<0, for dsmall>0.
				if(dbig2>=dsmall&&dbig1<=dsmall){  
					d1=fabs(dsmall-dbig1);
					d2=fabs(dbig2-dsmall);
					m_SecondCouple[i][num].nRcvB=(d1<d2)?
						pShotRcv[i].Rcv [k].dDistance :pShotRcv[i].Rcv [k-1].dDistance ;
					num++;
					break;
				}
			}
		}  // R point loop.
	}  // shot point loop to calculate the number of the two plused r-point.
	if(m_nFoldTime<3)return true;


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 3. Calculate the third plused couple between the EighthDistance 
	//      on the first Quadistance.
	BigDistance=QuaDistance;
	for(i=0;i<nShotRcv;i++){    // Loop for all of the shots.
		num=0;
		for(j=0;j<pShotRcv[i].lRcvNum ;j++){  // Loop for all of the recieve points.
			
			// diffrence of the the R point and the SmallDistance.
			dsmall=pShotRcv[i].Rcv [j].dDistance -SmallDistance;
			if(dsmall<0)continue;
			if(pShotRcv[i].Rcv [j].dDistance >EighthDistance){
				m_nThirdCoupleNumber[i]=num;
				break;		
			}
			m_ThirdCouple[i][num].nRcvA=pShotRcv[i].Rcv [j].dDistance ;

			//    search for the second R point whose diffrence with BigDistance 
			// match the dsmall.
			for(k=pShotRcv[i].lRcvNum -1;k>0;k--){   // should from sp._ to 0, see below.
				dbig1=BigDistance-pShotRcv[i].Rcv [k].dDistance ;
				dbig2=BigDistance-pShotRcv[i].Rcv [k-1].dDistance ;
				
				// Need not consider of dbig2<0, for dsmall>0.
				if(dbig2>=dsmall&&dbig1<=dsmall){  
					d1=fabs(dsmall-dbig1);
					d2=fabs(dbig2-dsmall);
					m_ThirdCouple[i][num].nRcvB=(d1<d2)?
						pShotRcv[i].Rcv [k].dDistance:pShotRcv[i].Rcv [k-1].dDistance ;
					num++;
					break;
				}
			}
		}  // R point loop.
	}  // shot point loop to calculate the number of the two plused r-point.

	return true;
}

long CZDEquation::SearchCoupleData(long nRcv,CCouple *pCouple,long nCoupleNumber)
{
	for(long i=0;i<nCoupleNumber;i++){
		if(pCouple[i].nRcvA ==nRcv){
			return pCouple[i].nRcvB;
		}
	}
	return -1;
}
