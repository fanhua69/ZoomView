
#include "stdafx.h"
#include "MemEqua.h"
#include "DlgMonitor.h"
#include "math.h"
#include "FHCGlobal.h"

CMemEquation::CMemEquation()
{
	m_nUsing=0;

	m_fileA="";
	m_fileB="";
	m_fileEquaTurn="";
	m_fileVacant="";
	m_fileX="";	
	m_sFileTag="";

	m_fpA=NULL;
	m_fpB=NULL;
	m_fpAT=NULL;
	
	m_dpX=NULL;
	m_dpFirstX=NULL;
	m_dpSecondX=NULL;
	m_dpThirdX=NULL;

	m_dpMidFirstX=NULL;
	m_dpMidSecondX=NULL;
	m_dpMidThirdX=NULL;

	m_dpLongFirstX=NULL;
	m_dpLongSecondX=NULL;
	m_dpLongThirdX=NULL;

	m_dpUltraLongFirstX=NULL;
	m_dpUltraLongSecondX=NULL;
	m_dpUltraLongThirdX=NULL;


	m_lpNumInRow=NULL;
	m_lpNumInCol=NULL;    // 记录方程中每列的非零元素数目。
	m_lpPosInRow=NULL;
	m_lpTag=NULL;

	m_pAi2=NULL;
	m_pAiX=NULL;
	m_pFractor=NULL;
	m_pB=NULL;

	m_lRowNum=0;
	m_lColNum=0;
	m_lMaxColEle=0;
	m_lMaxRowEle=0;
	m_dVacantValue=9999;
	m_dErrorLimit=0.0000000000001;
}				


bool CMemEquation::Use(CString fileA, CString fileB, CString fileX, int nInitialMethod, double *pInitialvalue)                                              
{
	if(m_nUsing==1){
		AfxMessageBox( "正在求解方程,不能进行新方程的求解!");
		return false;
	}
	else if(m_nUsing==2){
		AfxMessageBox( "方程尚未建立完毕,不能进行方程的求解!");
		return false;
	}
	
	Reset();
	
	m_nUsing=2;
		
	// Set the name for A and B:
	m_fileA=fileA;
	m_fileB=fileB;

	// set the name for X:
	if(fileX!=""){
		m_fileX=fileX;
	}
	else{
		m_fileX=SeperateName(fileA)+".X";
	}

	//
	m_sFileTag=SetTagFileName(fileA);

	m_fileEquaTurn=fileA+".TUR";
	
	////////////////////////////////////////////////////////////////////////////////////
	// Get  Some Important Data:
	////////////////////////////////////////////////////////////////////////////////////
	m_fpA=fopen(m_fileA,"rb");
	if(!m_fpA){
		AfxMessageBox("当打开文件"+m_fileA+"时出错！");
		return false;
	}

	// temporily open this file , get some data , and then read all of it to m_pB;
	FILE *fpB=fopen(m_fileB,"rb"); 
	if(!fpB){
		AfxMessageBox("当打开文件"+m_fileB+"时出错！");
		return false;
	}

	//  获得方程的个数：
	long n=fseek(fpB,0,SEEK_END);
	if(n){
		AfxMessageBox("当置文件指针到"+m_fileB+"文件尾时出错！");
		return false;
	}

	double  f=(double)ftell(fpB)/sizeof(double);
	if(int(f*sizeof(double))!=int(f)*sizeof(double)){
		AfxMessageBox("文件"+m_fileB+"错误！");
		return false;
	}

	m_lRowNum=int(f);

	////////////////////////////////////////////////////////////////////////////////////////////////
	//   data group that must be used frequently:
	m_pAi2=new double [m_lRowNum];
	m_pAiX=new double [m_lRowNum];
	m_pFractor=new double [m_lRowNum];
	m_pB=new double [m_lRowNum];

	// Read B:
	fseek(fpB,0,SEEK_SET);
	fread(m_pB,sizeof(double),m_lRowNum,fpB);
	fclose(fpB);

	////////////////////////////////////////////////////////////////////////////////////
	//  对方程读取一遍,获得一些数据:
	//  获得未知数的个数：
	n=fread(&m_lColNum,sizeof(long),1,m_fpA);
	if(n<1){
		AfxMessageBox(" The equation file is error!");
		Reset();
		return false;
	}
		
	m_lpNumInRow=new long[m_lRowNum];
	m_lpNumInCol=new long[m_lColNum];
	memset(m_lpNumInCol,0,sizeof(long)*m_lColNum);
	m_lpPosInRow=new long[m_lRowNum];
	CBlockTrace *pBlockTrace=new CBlockTrace[m_lColNum];

	m_lMaxRowEle=0;
	long i;
	for( i=0;i<m_lRowNum;i++)
	{
		// 计算方程中每行所在位置:
		m_lpPosInRow[i]=ftell(m_fpA);
		
		// 计算每行非零元素的个数:
		n=fread(&m_lpNumInRow[i],sizeof(long),1,m_fpA);
		if(n<1){
			AfxMessageBox("方程文件错误!");
			Reset();
			return false;
		}
		
		// 获得每行中非零元素个数的最大者:
		if(m_lpNumInRow[i]>m_lMaxRowEle)m_lMaxRowEle=m_lpNumInRow[i];
				
		//  获得方程各列的元素个数,及最大列元素个数：
		// 计算每列中非零元素的个数:
		n=fread(pBlockTrace,sizeof(CBlockTrace),m_lpNumInRow[i],m_fpA);
		if(n<m_lpNumInRow[i]){
			AfxMessageBox("方程文件错误!");
			Reset();
			return false;
		}

		for(long j=0;j<m_lpNumInRow[i];j++){
			m_lpNumInCol[pBlockTrace[j].m_nBlock]++;
		}
	}
	delete []pBlockTrace;

	// 获得中非零元素个数的最大的列:
	m_lMaxColEle=0;
	for(i=0;i<m_lColNum;i++){
		if(m_lpNumInCol[i]>m_lMaxColEle){
			m_lMaxColEle=m_lpNumInCol[i];
		}
	}

	// 转置方程
	if(!Turn()){
		AfxMessageBox("Error when turning the equation!");
		Reset();
		return false;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	// Open this file through this object:
	m_fpAT=fopen(m_fileEquaTurn,"rb");


	////////////////////////////////////////////////////////////////////////////////////////////////
	// 初始化未知数:
	m_dpX=new double[m_lColNum];
	m_dpFirstX=new double[m_lColNum];
	m_dpSecondX=new double[m_lColNum];
	m_dpThirdX=new double[m_lColNum];

	m_dpMidFirstX=new double[m_lColNum];
	m_dpMidSecondX=new double[m_lColNum];
	m_dpMidThirdX=new double[m_lColNum];

	m_dpLongFirstX=new double[m_lColNum];
	m_dpLongSecondX=new double[m_lColNum];
	m_dpLongThirdX=new double[m_lColNum];

	m_dpUltraLongFirstX=new double[m_lColNum];
	m_dpUltraLongSecondX=new double[m_lColNum];
	m_dpUltraLongThirdX=new double[m_lColNum];

	ReadTag();
		
	if(nInitialMethod==0){
		for(long i=0;i<m_lColNum;i++){
			m_dpX[i]=pInitialvalue[i];
		}
	}
	else if(nInitialMethod==1){
		ReadX();
	}
	else{
		for(long i=0;i<m_lColNum;i++){
			m_dpX[i]=0.0001;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	// If error, return false;
	if(m_lColNum==0){
        AfxMessageBox("CMemEquation::ResoEquation 报错：\n获得的方程行数为0，无法继续！");
		Reset();
		return false;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	//  Calculate ai2 and save it into  the data group of m_pAi2:
	if(!CalculateAi2()){
        AfxMessageBox("CMemEquation::ResoEquation 报错：\n计算行向量的内积出错！");
		Reset();
		return false;		
	}      
  
	return true;
}

bool CMemEquation::Reset()
{
	if(m_fpA){
		fclose(m_fpA);
		m_fpA=NULL;

	}

	if(m_fpB){
		fclose(m_fpB);
		m_fpB=NULL;
	}

	if(m_fpAT){
		fclose(m_fpAT);
		m_fpAT=NULL;
	}

	if(m_dpX){
		delete []m_dpX;
		m_dpX=NULL;
	}

	if(m_dpFirstX){
		delete []m_dpFirstX;
		m_dpFirstX=NULL;
	}
	if(m_dpSecondX){
		delete []m_dpSecondX;
		m_dpSecondX=NULL;
	}
	if(m_dpThirdX){
		delete []m_dpThirdX;
		m_dpThirdX=NULL;
	}


	if(m_dpMidFirstX){
		delete m_dpMidFirstX;
		m_dpMidFirstX=NULL;
	}
	if(m_dpMidSecondX){
		delete m_dpMidSecondX;
		m_dpMidSecondX=NULL;
	}	
	if(m_dpMidThirdX){
		delete m_dpMidThirdX;
		m_dpMidThirdX=NULL;
	}	


	if(m_dpLongFirstX){
		delete m_dpLongFirstX;
		m_dpLongFirstX=NULL;
	}
	if(m_dpLongSecondX){
		delete m_dpLongSecondX;
		m_dpLongSecondX=NULL;
	}	
	if(m_dpLongThirdX){
		delete m_dpLongThirdX;
		m_dpLongThirdX=NULL;
	}	

	if(m_dpUltraLongFirstX){
		delete m_dpUltraLongFirstX;
		m_dpUltraLongFirstX=NULL;
	}
	if(m_dpUltraLongSecondX){
		delete m_dpUltraLongSecondX;
		m_dpUltraLongSecondX=NULL;
	}	
	if(m_dpUltraLongThirdX){
		delete m_dpUltraLongThirdX;
		m_dpUltraLongThirdX=NULL;
	}

	if(m_lpTag){
		delete []m_lpTag;
		m_lpTag=NULL;
	}


	if(m_pAi2){
		delete []m_pAi2;
		m_pAi2=NULL;
	}

	if(m_pAiX){
		delete []m_pAiX;
		m_pAiX=NULL;
	}

	if(m_pFractor){
		delete []m_pFractor;
		m_pFractor=NULL;
	}

	if(m_pB){
		delete []m_pB;
		m_pB=NULL;
	}


	if(m_lpNumInRow){
		delete []m_lpNumInRow;
		m_lpNumInRow=NULL;
	}

	if(m_lpNumInCol){
		delete []m_lpNumInCol;
		m_lpNumInCol=NULL;
	}

	if(m_lpPosInRow) {
		delete []m_lpPosInRow;
		m_lpPosInRow=NULL;
	}	

	m_nUsing=0;

	return true;
}



bool CMemEquation::Close()
{
	if(m_nUsing==1){
		if(!RecordVacant())return false;
		if(!FillVacant())return false;
		SaveTag();
		Reset();
		m_nUsing=0;
	}
	else if(m_nUsing==2)
	{
		SaveX();
		Reset();

		RemoveFile(m_fileEquaTurn);
		RemoveFile(m_fileVacant);
				
		m_nUsing=0;
	}
	return true;
}

CMemEquation::~CMemEquation()
{  	
	Reset();
}
//    构造函数。
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
//  公共函数：追加方程。
bool CMemEquation::AppeEqua(CObArray& array,double B)
{
	long nBlockNum=array.GetSize();

	CBlockTrace *pBlock;	

	//  Record the element number of every column, NEEDED:
	long i;
	for(i=0;i<nBlockNum;i++)
	{
		pBlock=(CBlockTrace*)array[i];
				
		if(pBlock->m_nBlock<0||pBlock->m_nBlock>=m_lColNum){
			CString s;
			s.Format("%ld",pBlock->m_nBlock);
			AfxMessageBox("方程的行向量有错,未知数的序号超出范围!"+s);
			return false;
		}

		m_lpNumInCol[pBlock->m_nBlock]++;
	}

	//  Append to A:
	if(fseek(m_fpA,0,SEEK_END)){
		AfxMessageBox("当置文件指针到方程文件"+m_fileA+"文件尾时出错！");
		return false;
	}

	fwrite(&nBlockNum,sizeof(long),1,m_fpA);  // 将该行的数据数目记入文件.
	for(i=0;i<nBlockNum;i++)
	{
		if(fwrite((CBlockTrace*)array[i],sizeof(CBlockTrace),1,m_fpA)<1){
		AfxMessageBox("当向方程文件"+m_fileA+"尾部追加方程时出错！");
			return false;
		}
	}

	// Append to B:
	if(fseek(m_fpB,0,SEEK_END)){
		AfxMessageBox("当置文件指针到方程文件"+m_fileB+"文件尾时出错！");
		return false;
	}

	if(!fwrite(&B,sizeof(double),1,m_fpB)){
		AfxMessageBox("当向方程文件"+m_fileB+"追加数据时出错！");
		return false;
	}

	return true;

}
//  公共函数：追加方程。
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
//  公共函数：追加方程。
bool CMemEquation::AppeEqua(CBlockTrace *pBlockArray,long nBlockNum,double B)
{
	CBlockTrace *pBlock;	

	//  Record the element number of every column, NEEDED:
	long i;
	for(i=0;i<nBlockNum;i++)
	{
		pBlock=&pBlockArray[i];
				
		if(pBlock->m_nBlock<0||pBlock->m_nBlock>=m_lColNum){
			CString s;
			s.Format("%ld",pBlock->m_nBlock);
			AfxMessageBox("方程的行向量有错,未知数的序号超出范围!"+s);
			return false;
		}

		m_lpNumInCol[pBlock->m_nBlock]++;
	}

	//  Append to A:
	if(fseek(m_fpA,0,SEEK_END)){
		AfxMessageBox("当置文件指针到方程文件"+m_fileA+"文件尾时出错！");
		return false;
	}

	fwrite(&nBlockNum,sizeof(long),1,m_fpA);  // 将该行的数据数目记入文件.
	for(i=0;i<nBlockNum;i++)
	{
		if(fwrite(&pBlockArray[i],sizeof(CBlockTrace),1,m_fpA)<1){
		AfxMessageBox("当向方程文件"+m_fileA+"尾部追加方程时出错！");
			return false;
		}
	}

	// Append to B:
	if(fseek(m_fpB,0,SEEK_END)){
		AfxMessageBox("当置文件指针到方程文件"+m_fileB+"文件尾时出错！");
		return false;
	}

	if(!fwrite(&B,sizeof(double),1,m_fpB)){
		AfxMessageBox("当向方程文件"+m_fileB+"追加数据时出错！");
		return false;
	}

	return true;

}
//  公共函数：追加方程。
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
//  从方程中  读  数据   的私有函数    开始：
bool CMemEquation::GetEqua(CBlockTrace *room,double *B,long EquaNo)
{
	//  错误返回:
	if(m_lpPosInRow[0]==0||!m_fpA){
		AfxMessageBox("必须首先调用Use以后才能调用这个函数!");
		return false;
	}

	//  获得方程位置：
	long Pos=m_lpPosInRow[EquaNo];
	long Num=m_lpNumInRow[EquaNo];
	
	// 读取方程：
	if(fseek(m_fpA,sizeof(CBlockTrace)*Pos,SEEK_SET)!=0){
		AfxMessageBox("当置文件指针到要求位置时，方程文件"+m_fileA+"出错！");
		return false;
	}
	if(fread(room,sizeof(CBlockTrace),Num,m_fpA)<Num){
		AfxMessageBox("当读取数据时，方程文件"+m_fileA+"出错！");
		return false;
	}

	//  读取 B 值：
	if(fseek(m_fpB,sizeof(double)*EquaNo,SEEK_SET)!=0){
		AfxMessageBox("当置文件指针到要求位置时，方程值文件"+m_fileB+"出错！");
		return false;
	}
	if(fread(B,sizeof(double),1,m_fpB)<1){
		AfxMessageBox("当读取数据时，方程值文件"+m_fileB+"出错！");
		return false;
	}	

	return true;
}	
//    CMemEquation::一般性操作结束。
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   计算行向量的内积：
bool CMemEquation::CalculateAi2()
{	
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 对所有行向量循环：
	CBlockTrace *pRowData=new CBlockTrace[m_lMaxRowEle];
	fseek(m_fpA,sizeof(long),SEEK_SET);   //  前Sizeof(long)个字节的是方程的列数
	long  i,j,n,nNum;
	for(i=0;i<m_lRowNum;i++){
		n=fread(&nNum,sizeof(long),1,m_fpA);
		if(n<1){
			AfxMessageBox("方程的主文件出错:"+m_fileA);
			return false;
		}
		n=fread(pRowData,sizeof(CBlockTrace),nNum,m_fpA);
		if(n<nNum){
			AfxMessageBox("方程的主文件出错:"+m_fileA);
			return false;
		}

		m_pAi2[i]=0.0;
		for(j=0;j<nNum;j++){
			m_pAi2[i]+=pRowData[j].m_dLen*pRowData[j].m_dLen;
		}
	}
	// 对所有行向量循环：
	////////////////////////////////////////////////////////////////////////////////////////////////////////

	delete []pRowData;
	
	return true;
}
//                                BOOL Equation::CalculateAi2()															 END
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   对方程矩阵A进行转置：
bool CMemEquation::Turn()
{
		
	//  打开转置方程：
	FILE *fpEquaTurn=fopen(m_fileEquaTurn,"wb");
	if(!fpEquaTurn){
                AfxMessageBox("CMemEquation::Turn()报错：\n无法建立矩阵转置文件："+m_fileEquaTurn);
		return false;
	}	

	//  计算方程转置以后每列的位置：
	//	|   0      |   1   |   2      |   3            |   4   |.... room number
	//  | NULL |  d1  |  d2     |  d3           |  d4  |.... "Num In Col"
	//  |   0      |  d1  | d1+d2|d1+d2+d3 |         ....  Positon  of  matrix element in column sequence.
	long* pPosInCol=new long[m_lColNum];   //Positon  of  matrix element in column sequence.
	pPosInCol[0]=0;
	long i;
	for(i=1;i<m_lColNum;i++){
		pPosInCol[i]=pPosInCol[i-1]+m_lpNumInCol[i-1];
	}
			
	//  从原方程中读取一行，用于存储的数组：
	CBlockTrace *pRowData=new CBlockTrace[m_lMaxRowEle];
	
	//临时数据空间，用于存储所有列，
	//当一列的数据存满后，就把该列存入转置方程：
	long RoomOfCol=10,m,n,nNum;
	CBlockTrace (*pBlockTrace)[10]=new CBlockTrace[m_lColNum][10];  

	// 指示临时数据空间中每列的空间已经被使用了多少：
	int *pNumStored=new int[m_lColNum]; 
	memset(pNumStored,0,m_lColNum*sizeof(int));

	bool bSuccess=true;

	//  从原方程中按行读取数据：
	fseek(m_fpA,0,SEEK_SET);
	fread(&nNum,sizeof(long),1,m_fpA);	//读入未知变量的个数
	for(long Row=0;Row<m_lRowNum;Row++){

		//读入一行的数据：
		fread(&nNum,sizeof(long),1,m_fpA);				
		n=fread(pRowData,sizeof(CBlockTrace),nNum,m_fpA);
		if(n<nNum){
                        AfxMessageBox("CMemEquation::Turn()报错：\n从方程文件中读入数据时出错："+m_fileA);
			bSuccess=false;
			goto tag_quit;
		}	

		// 把该行的数据存成列序。
		for(i=0;i<nNum;i++)
		{

			m=pRowData[i].m_nBlock;
			n=pNumStored[m];

			pBlockTrace[m][n].m_nBlock=Row;
			pBlockTrace[m][n].m_dLen=pRowData[i].m_dLen;
			pNumStored[m]++;
			
			//  如果本列的临时空间放满，则存储本列的数据，并置标志为零：
			if(pNumStored[m]==RoomOfCol){
				fseek(fpEquaTurn,pPosInCol[m]*sizeof(CBlockTrace),SEEK_SET); // 找到正确位置：
				fwrite(pBlockTrace[m],sizeof(CBlockTrace),RoomOfCol,fpEquaTurn);  //  写入数据：
				pPosInCol[m]+=RoomOfCol;   // 重新计算列位置。
				pNumStored[m]=0;		
			}// 如果本列的临时空间放满，则存储本列的数据，并置标志为零。
		}//把该行的数据存成列序。
	}  // 从原方程中按行读取数据。

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  the room was not full, so have not been saved. Save it now.
	for(i=0;i<m_lColNum;i++){
		fseek(fpEquaTurn,pPosInCol[i]*sizeof(CBlockTrace),SEEK_SET);
		n=fwrite(pBlockTrace[i],sizeof(CBlockTrace),pNumStored[i],fpEquaTurn);
		if(n<pNumStored[i]){
                        AfxMessageBox("CMemEquation::Turn()报错：\n向转置方程中写入数据时出错："+m_fileEquaTurn);
			bSuccess=false;
			goto tag_quit;
		}	
	}
		
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 删除指针：
tag_quit:
	delete []pRowData;
	delete []pPosInCol;
	delete []pNumStored;
	delete []pBlockTrace;
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 关闭文件：
	fclose(fpEquaTurn);
	return bSuccess;
}
//          CMemEquation::Turn
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  					 求解方程主循环
bool CMemEquation::Resolve()
{
	double x1=0,y1=100,x2=m_lColNum*0.00001,y2=1;
	double a=(x1*y1-x2*y2)/(y2-y1);
	double b=y1*(x1+a);
	CString sStatus;

	CDlgMonitor dlg;
	dlg.Create();
	dlg.SetRange(0,m_lColNum);	
	
	long nMaxBlock=m_lMaxRowEle;
	if(m_lMaxColEle>m_lMaxRowEle){
		nMaxBlock=m_lMaxColEle;
	}

	CBlockTrace *pData=new CBlockTrace[nMaxBlock];
	long nLoop=0,i,j,k,nNum;
	long nCheckLoop=10,nMidCheckLoop=2,nLongCheckLoop=3,nUltraLongCheckLoop=5;
	double dSum,dTemp,dAcceLimit=0.0;	
	
	//////////////////////////////////////////////////////
	// For acceleration: expediter
	double d1,d2,x;
	int nSignal=0,nMidSignal=0,nLongSignal=0,nUltraLongSignal=0;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// This is the big loop to resolve the problem , generally speaking, 
	// Stop only when the user press cancel key.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////				
	FILE *fp=fopen("1.txt","wt");
	long n=10;
	bool bShortAcc=false;
	bool bMidAcc=false;
	bool bLongAcc=false;
	bool bUltraLongAcc=false;

	while(true){
		
		// Short length acceleration:
		if(nLoop>10&&nLoop%nCheckLoop==0){
			for(i=0;i<m_lColNum;i++){
				m_dpFirstX[i]=m_dpSecondX[i];
				m_dpSecondX[i]=m_dpThirdX[i];
				m_dpThirdX[i]=m_dpX[i];
			}	
			nSignal++;
		}

	   	// Middle length acceleration:
		if(nLoop>18&&nLoop%nMidCheckLoop==0){
			for(i=0;i<m_lColNum;i++){
				m_dpMidFirstX[i]=m_dpMidSecondX[i];
				m_dpMidSecondX[i]=m_dpMidThirdX[i];
				m_dpMidThirdX[i]=m_dpX[i];
			}	
			nMidSignal++;
		}

		// Long length acceleration:
		if(nLoop>50&&nLoop%nLongCheckLoop==0){
			for(i=0;i<m_lColNum;i++){
				m_dpLongFirstX[i]=m_dpLongSecondX[i];
				m_dpLongSecondX[i]=m_dpLongThirdX[i];
				m_dpLongThirdX[i]=m_dpX[i];
			}	
			nLongSignal++;
		}

		// Ultra Long length acceleration:
		if(nLoop>54&&nLoop%nUltraLongCheckLoop==0){
			for(i=0;i<m_lColNum;i++){
				m_dpUltraLongFirstX[i]=m_dpUltraLongSecondX[i];
				m_dpUltraLongSecondX[i]=m_dpUltraLongThirdX[i];
				m_dpUltraLongThirdX[i]=m_dpX[i];
			}	
			nUltraLongSignal++;
		}

		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 	 Accelerate for short distance:
		if(nSignal>3){
			for(i=0;i<m_lColNum;i++){
				d1=m_dpSecondX[i]-m_dpFirstX[i];
				d2=m_dpThirdX[i]-m_dpSecondX[i];

				if(d2*d1<=0)continue;

				if(fabs(d2)>=fabs(d1)){
					m_dpX[i]-=d1;
				}
				else{
					x=d2*d2/(d2-d1);  
					dAcceLimit=fabs(m_dpX[i]*0.2);
					
					if(fabs(x)>dAcceLimit){
						if(d1>0)
							x=-dAcceLimit;
						else
							x=dAcceLimit;
					}

					m_dpX[i]-=x;					

				}
			}
			bShortAcc=true;
			nSignal=0;
		}
		
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 	 Accelerate for Mid distance:
		if(nMidSignal>3){
			for(i=0;i<m_lColNum;i++){
				d1=m_dpMidSecondX[i]-m_dpMidFirstX[i];
				d2=m_dpThirdX[i]-m_dpMidSecondX[i];				

				if(d2*d1<0)continue;

				if(fabs(d2)>=fabs(d1)){
					m_dpX[i]-=d1;
				}
				else{
					x=d2*d2/(d2-d1);  
					dAcceLimit=fabs(m_dpX[i]*0.2);
					dTemp=fabs(3*d2);
				//	if(dTemp>dAcceLimit)
				//		dAcceLimit=dTemp;
					if(fabs(x)>dAcceLimit){
						if(d1>0)
							x=-dAcceLimit;
						else
							x=dAcceLimit;
					}
								
				//	m_dpX[i]-=x;
				}
			}		 
			bMidAcc=true;
			nMidSignal=0;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 	 Accelerate for long distance:
		if(nLongSignal>3){
			for(i=0;i<m_lColNum;i++){
				d1=m_dpLongSecondX[i]-m_dpLongFirstX[i];
				d2=m_dpThirdX[i]-m_dpLongSecondX[i];				

				if(d2*d1<0)continue;

				if(fabs(d2)>=fabs(d1)){
					m_dpX[i]-=d1;
				}
				else{
					x=d2*d2/(d2-d1);  
					dAcceLimit=fabs(3*d2);
					if(fabs(x)>dAcceLimit){
						if(d1>0)
							x=-dAcceLimit;
						else
							x=dAcceLimit;
					}
					m_dpX[i]-=x;					
				}
			}
			bLongAcc=true;
			nLongSignal=0;
		}

		
		// Accelerate in a ultra long distance:
		if(nUltraLongSignal>3){
			for(i=0;i<m_lColNum;i++){
				d1=m_dpUltraLongSecondX[i]-m_dpUltraLongFirstX[i];
				d2=m_dpUltraLongThirdX[i]-m_dpUltraLongSecondX[i];				

				if(d2*d1<0)continue;

				if(fabs(d2)>=fabs(d1)){
					m_dpX[i]-=d1;
				}
				else{
					x=d2*d2/(d2-d1);  
					dAcceLimit=fabs(3*d2);
					if(fabs(x)>dAcceLimit){
						if(d1>0)
							x=-dAcceLimit;
						else
							x=dAcceLimit;
					}
					m_dpX[i]-=x;					
				}
			}
			bUltraLongAcc=true;
			nUltraLongSignal=0;
		}



		//fprintf(fp,"%ld\n",n);
		//for(i=0;i<n;i++){
			//fprintf(fp,"%lf %i %i\n",m_dpX[i],bShortAcc,bMidAcc,bLongAcc);			
		//}
		bShortAcc=bMidAcc=bLongAcc=false;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Calculate AiX and the fractor at the same time:
		fseek(m_fpA,sizeof(long),SEEK_SET);
		for(i=0;i<m_lRowNum;i++){
			fread(&nNum,sizeof(long),1,m_fpA);
			fread(pData,sizeof(CBlockTrace),nNum,m_fpA); // read one row of A.
			
			// calculate AiX of the row, or the equation, or the group.
			m_pAiX[i]=0.0;
			for(k=0;k<nNum;k++){
				m_pAiX[i]+=pData[k].m_dLen*m_dpX[pData[k].m_nBlock];
			}

			// Calculate the fractor:
			m_pFractor[i]=(m_pB[i]-m_pAiX[i])/m_pAi2[i];

		} // loop for all of the shots.

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Calculate X
		// 对所有列、或未知数循环：
		fseek(m_fpAT,0,SEEK_SET);
		for(i=0;i<m_lColNum;i++){
			
			// 读入一列数据：
			fread(pData,sizeof(CBlockTrace),m_lpNumInCol[i],m_fpAT);
			
			dSum=0.0;
			for(j=0;j<m_lpNumInCol[i];j++){
				dSum+=pData[j].m_dLen*m_pFractor[pData[j].m_nBlock];
			}
			dSum=dSum/m_lpNumInCol[i];
			m_dpX[i]+=dSum;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Every some loop, show the data to the user:
		if(nLoop%nCheckLoop==0){
			sStatus.Format("Resolving %ld th loop",nLoop);
			x=CheckX();
			//if(x<m_dErrorLimit)break;
			dlg.SetWindowText(sStatus);
			dlg.SetPos(int(b/(x+a)));
			dlg.UpdateData(true);

			int nStart=dlg.m_nStart;
			int nEnd=dlg.m_nEnd;	
			if(nEnd>m_lColNum-1){
				nEnd=m_lColNum-1;
				dlg.m_nEnd =m_lColNum-1;
				dlg.UpdateData(false);
			}
			if(nStart<0){
				nStart=0;
				dlg.m_nStart =0;
				dlg.UpdateData(false);
			} 
			dlg.SetMonitor (&m_dpX[nStart],nEnd-nStart+1);
			if(dlg.CheckCancelButton ())break;
		}

		nLoop++;
	}
	delete []pData;

	dlg.DestroyWindow ();
	fclose(fp);
	return true;
}
double CMemEquation::CheckX()
{
	double Sum=0.0;
	for(long i=0;i<m_lColNum;i++){
		Sum+=fabs(m_dpX[i]-m_dpSecondX[i]);
	}
	Sum/=m_lColNum;

	return Sum;
}
bool CMemEquation::SaveX()
{
	FILE *fp=fopen(m_fileX,"wb");
	if(!fp){
		AfxMessageBox("当建立方程解文件时出错！");
		return false;
	}

	fwrite( m_dpX,sizeof(double),m_lColNum, fp);
	fclose(fp);

	// A text file use to check the data:
	fp=fopen("X.TXT","wt");
	if(!fp){
		AfxMessageBox("当建立方程解文件时出错！");
		return false;
	}

	for(long i=0;i<m_lColNum;i++){
		fprintf(fp,"%i, %15.10lf\n",i,m_dpX[i]);
	}
	fclose(fp);

	return true;
}
double *CMemEquation::ReadX()
{
	FILE *fp=fopen(m_fileX,"rb");

	long n=fread(m_dpX,sizeof(double),m_lColNum, fp);
	if(n<m_lColNum){
		AfxMessageBox("X Error!");
	}
	return m_dpX;
}

bool CMemEquation::RecordVacant()
{
	if(m_lColNum==0){
                AfxMessageBox("CMemEquation::RecordVacant报错：\n未知数个数为零 m_lColNum==0");
		return false;
	}

	ASSERT(m_fileVacant!="");
	FILE *fp=fopen(m_fileVacant,"wt");
	if(!fp){
                AfxMessageBox("CMemEquation::RecordVacant报错：\n当打开记录空网格的文件"+m_fileVacant+"时，出错！");
		return false;
	}

	for(long i=0;i<m_lColNum;i++){
		if(m_lpNumInCol[i]==0){
			fprintf(fp,"%ld\n",i);
		}
	}

	fclose(fp);
	return true;
}

bool CMemEquation::FillVacant()
{
	CBlockTrace BlockTrace;
	BlockTrace.m_dLen=1;
	
	CObArray  array;
	array.SetSize(1);

	for(long i=0;i<m_lColNum;i++){
		if(m_lpNumInCol[i]==0){
			BlockTrace.m_nBlock=i;
			array[0]=&BlockTrace;
			if(!AppeEqua(array,m_dVacantValue)){
                                AfxMessageBox("CMemEquation::FillVacant 报错：\n追加方程时出错。");
				return false;
			}
		}
	}

	return true;
}

			


bool CMemEquation::Construct(CString fileA, CString fileB,long nColumn)
{
	if(m_nUsing==1){
		AfxMessageBox( "正在求解方程,不能进行新方程的建立!");
		return false;
	}
	else if(m_nUsing==2){
		AfxMessageBox( "方程尚未建立完毕,不能进行新方程的建立!");
		return false;
	}  	

	m_nUsing=1;

	// set the numbers:
	m_lColNum=nColumn;			   	
	m_fpA=fopen(fileA,"wb");
	if(!m_fpA){
		AfxMessageBox("当建立文件"+fileA+"时出错！");
		return false;
	}
	fwrite(&nColumn,sizeof(long),1,m_fpA);  // 将矩阵的列数入主文件
	m_fileA=fileA;

	m_fpB=fopen(fileB,"wb");
	if(!m_fpB){
		AfxMessageBox("当建立文件"+fileB+"时出错！");
		return false;
	}
	m_fileB=fileB;

	m_fileVacant="EquaVacant.$$$";
	m_lpNumInCol=new long[nColumn];
	memset(m_lpNumInCol,0,sizeof(long)*m_lColNum);

	m_sFileTag=SetTagFileName(fileA);

	m_lpTag=new long[m_lColNum];

	return true;
}


bool CMemEquation::ReadAnswer(double *pRoom)
{
	FILE *fp=fopen("X.TXT","rt");
	if(!fp){
		AfxMessageBox("当打开方程解文件时出错！");
		return false;
	}

	long n;
	for(long i=0;i<m_lColNum;i++){
		fscanf(fp,"%ld,%lf\n",&n,&(pRoom[i]));
	}

	fclose(fp);
	return true;
}

double CMemEquation::SetErrorLimit(double dErrorLimit)
{
	// Generally speaking , The error can be reached is 
	// Much bigger than the data that CheckX given.
	double dLastErrorLimit=m_dErrorLimit*1000;
	m_dErrorLimit=dErrorLimit/1000;
	return dLastErrorLimit;
}

double * CMemEquation::GetX()
{
	return m_dpX;
}

long CMemEquation::GetX(double *pRoom)
{
	for(int i=0;i<m_lColNum;i++){
		pRoom[i]=m_dpX[i];
	}
	return m_lColNum;
}


bool CMemEquation::IsVarVacant(long nVar)
{
	return (m_lpNumInCol[nVar]==0);
}


bool CMemEquation::SetTag(long nOrder, long nTag)
{
	if(!m_lpTag)return false;
	if(nOrder<0||nOrder>=GetXNum())return false;

	m_lpTag[nOrder]=nTag;
	return true;
}

bool CMemEquation::SetTag(long *pTag, long nNumber)
{
	if(!m_lpTag)return false;
	if(nNumber>GetXNum())return false;

	for(long i=0;i<nNumber;i++){
		m_lpTag[i]=pTag[i];
	}

	return true;

}

long CMemEquation::GetTag(long nOrder)
{
	if(!m_lpTag)return false;
	if(nOrder<0||nOrder>=GetXNum())return false;

	return m_lpTag[nOrder];
}

long * CMemEquation::GetTag()
{	 
	return m_lpTag;
}

bool CMemEquation::SaveTag()
{
	if(!m_lpTag)return false;
	FILE *fp=fopen(m_sFileTag,"wt");
	if(!fp){
		return false;
	}

	for(long i=0;i<GetXNum();i++){
		fprintf(fp,"%ld\n",m_lpTag[i]);
	}

	fclose(fp);

	return true;
}

bool CMemEquation::ReadTag()
{
	if(m_lpTag){
		delete []m_lpTag;
		m_lpTag=NULL;
	}
 	m_lpTag=new long[GetXNum()];

	FILE *fp=fopen(m_sFileTag,"rt");
	if(!fp){
		return false;
	}

	for(long i=0;i<GetXNum();i++){
		fscanf(fp,"%ld\n",&m_lpTag[i]);
	}

	fclose(fp);

	return true;
}

CString CMemEquation::SetTagFileName(CString sFileA)
{
	return SeperateName(sFileA)+".tag";
}
