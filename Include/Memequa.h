
#ifndef MemEquationTag
#define MemEquationTag

#include "zoomview.h"

class _FHZOOMVIEW_EXT_CLASS CBlockTrace:public CObject
{
public:
	long m_nBlock;
	double m_dLen;

	CBlockTrace(){
		m_nBlock=0;
		m_dLen=0.0;
	}

	CBlockTrace(long nBlock,double dLen){
		m_nBlock=nBlock;
		m_dLen=dLen;
	}

	void Set(long nBlock,double dLen){
		m_nBlock=nBlock;
		m_dLen=dLen;
	}
};


//////////////////////////////////////////////////////////////////////////////////
//   "Equation" 被截断为 "Equa" , 类名除外。 
//  To Resolve   AX=B.
class _FHZOOMVIEW_EXT_CLASS CMemEquation
{
private:

	int m_nUsing;    //  Indicating what is doing, Making or Resolving  ,Reading or Writing.
								// 0, NOT using ,  1 	Making ,  2  Resolving .
	CString m_fileA;
	CString m_fileB;
	CString m_fileX;
	CString m_fileEquaTurn;
	CString m_fileVacant;	
	CString m_sFileTag;

	FILE *m_fpA;
	FILE *m_fpAT;
	FILE *m_fpB;

	double *m_pAi2;
	double *m_pAiX;
	double *m_pFractor;
	double *m_pB;
	double *m_dpX;
	
	double *m_dpThirdX;
	double *m_dpSecondX;
	double *m_dpFirstX;

	double *m_dpMidFirstX;
	double *m_dpMidSecondX;
	double *m_dpMidThirdX;

	double *m_dpLongFirstX;
	double *m_dpLongSecondX;
	double *m_dpLongThirdX;

	double *m_dpUltraLongFirstX;
	double *m_dpUltraLongSecondX;
	double *m_dpUltraLongThirdX;


	
	long *m_lpNumInRow;
	long *m_lpNumInCol;
	long *m_lpPosInRow;
	long *m_lpTag; // a tag to a x;

	double m_dErrorLimit;
	
	long m_lRowNum;
	long m_lColNum;
	long m_lMaxColEle;
	long m_lMaxRowEle;

		
	bool CalculateAi2();
	double CheckX();

	bool RecordVacant();
	bool FillVacant();
	
public:
	CString SetTagFileName(CString sFileA);
	bool ReadTag();
	bool SaveTag();
	long * GetTag();
	long GetTag(long nOrder);
	bool SetTag(long *pTag,long nNumber);
	bool SetTag(long nOrder,long nTag);
	bool IsVarVacant(long nVar);
	double m_dVacantValue;
	long GetX(double *);
	bool SaveX();
	double * GetX();
	double  SetErrorLimit(double dErrorLimit);
	bool  ReadAnswer(double *pRoom);
	 CMemEquation();
	 ~CMemEquation();

	bool  Use(CString fileA,CString fileB,CString fileX="",int nInitialMethod=2,double *pInitialvalue=NULL);								// 开始调用方程时使用。
	bool  Construct(CString fileA, CString fileB,long nColumn);
	bool  Close();  // 为建立方程完毕之后,建立方程完毕之后使用.

	bool IsIdle(){return m_nUsing==0;};
	bool IsMaking(){return m_nUsing==1;};
	bool IsResolving(){return m_nUsing==2;};

	// 追加方程：
	bool AppeEqua(CObArray& array,double B);
	bool AppeEqua(CBlockTrace *pBlockArray,long nBlockNum,double B);


	// 转置方程：
	bool Turn(); 

	//  求解方程：
	bool  Resolve();

	// 获得解：
	double  *ReadX();

	//  附加功能：获得若干个方程行
	bool  GetEqua(CBlockTrace *room,double *B,long EquaNo);
	long  GetXNum(){return m_lColNum;}
protected:
	bool Reset();
};

#endif