// Spline.cpp: implementation of the CSpline class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Spline.h"
#include "FHZoomViewGlobal.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSpline::CSpline()
{
	m_pX=NULL;
	m_pY=NULL;
	m_pY0=NULL;
	m_pY1=NULL;
	m_pY2=NULL;
	m_pY3=NULL;
	m_nPointNumber=0;

}

CSpline::~CSpline()
{
	Reset();
}



bool CSpline::SetPointNumber(long nPointNumber)
{
	return SetMemory(nPointNumber);
}

bool CSpline::SetData(long nOrder,double x, double y)
{
	if(!m_pX||!m_pY||!m_pY1||!m_pY2||!m_pY3){
		return false;
	}

	if(nOrder<0||nOrder>=m_nPointNumber){
		return false;
	}

	m_pX[nOrder]=x;
	m_pY[nOrder]=y;

	return true;
}

bool CSpline::SetData(double *pX, double *pY,long nNumber)
{
	if(!m_pX||!m_pY||!m_pY1||!m_pY2||!m_pY3){
		return false;
	}

	if(nNumber>m_nPointNumber){
		return false;
	}

	for(long i=0;i<nNumber;i++){
		m_pX[i]=pX[i];
		m_pY[i]=pY[i];
	}

	return true;
}

bool CSpline::Over()
{
	if(m_nPointNumber<2){
		return false;
	}
	
	Spline(); 
	return true;
}

/*****************************************************************************
compute cubic spline interpolation coefficients for interpolation with
continuous second derivatives
******************************************************************************
Input:
n		number of samples
x  		array[n] of monotonically increasing or decreasing abscissae
y		array[n] of ordinates

Output:
yd		array[n][4] of cubic interpolation coefficients (see notes)
******************************************************************************
Notes:
The computed cubic spline coefficients are as follows:
yd[i][0] = y(x[i])    (the value of y at x = x[i])
yd[i][1] = y'(x[i])   (the 1st derivative of y at x = x[i])
yd[i][2] = y''(x[i])  (the 2nd derivative of y at x = x[i])
yd[i][3] = y'''(x[i]) (the 3rd derivative of y at x = x[i])

To evaluate y(x) for x between x[i] and x[i+1] and h = x-x[i],
use the computed coefficients as follows:
y(x) = yd[i][0]+h*(yd[i][1]+h*(yd[i][2]/2.0+h*yd[i][3]/6.0))
*****************************************************************************/	
bool CSpline::Spline () //int m_nPointNumber, double m_pX[], double m_pY[], double m_pY[][4])
{
	int i;
	double h1,h2,del1,del2,dmax,hsum,w1,w2,divdf3,sleft,sright,alpha,t;

	/* if m_nPointNumber=1, then use constant interpolation */
	if (m_nPointNumber==1) {
		m_pY0[0] = m_pY[0];
		m_pY1[0] = 0.0;
		m_pY2[0] = 0.0;
		m_pY3[0] = 0.0;
		return true;
	}

	/* else, if m_nPointNumber=2, then use linear interpolation */
	if (m_nPointNumber==2) {
		m_pY0[0] = m_pY[0];  m_pY0[1] = m_pY[1];
		m_pY1[0] = m_pY1[1] = (m_pY[1]-m_pY[0])/(m_pX[1]-m_pX[0]);
		m_pY2[0] = m_pY2[1] = 0.0;
		m_pY3[0] = m_pY3[1] = 0.0;
		return true;
	}
	
	/* set left end derivative via shape-preserving 3-point formula */
	h1 = m_pX[1]-m_pX[0];
	h2 = m_pX[2]-m_pX[1];
	hsum = h1+h2;
	del1 = (m_pY[1]-m_pY[0])/h1;
	del2 = (m_pY[2]-m_pY[1])/h2;
	w1 = (h1+hsum)/hsum;
	w2 = -h1/hsum;
	sleft = w1*del1+w2*del2;
	if (sleft*del1<=0.0)
		sleft = 0.0;
	else if (del1*del2<0.0) {
		dmax = 3.0*del1;
		if (fabs(sleft)>fabs(dmax)) sleft = dmax;
	}

	/* set right end derivative via shape-preserving 3-point formula */
	h1 = m_pX[m_nPointNumber-2]-m_pX[m_nPointNumber-3];
	h2 = m_pX[m_nPointNumber-1]-m_pX[m_nPointNumber-2];
	hsum = h1+h2;
	del1 = (m_pY[m_nPointNumber-2]-m_pY[m_nPointNumber-3])/h1;
	del2 = (m_pY[m_nPointNumber-1]-m_pY[m_nPointNumber-2])/h2;
	w1 = -h2/hsum;
	w2 = (h2+hsum)/hsum;
	sright = w1*del1+w2*del2;
	if (sright*del2<=0.0)
		sright = 0.0;
	else if (del1*del2<0.0) {
		dmax = 3.0*del2;
		if (fabs(sright)>fabs(dmax)) sright = dmax;
	}
	
	/* compute tridiagonal system coefficients and right-hand-side */	
	m_pY0[0] = 1.0;
	m_pY2[0] = 2.0*sleft;
	for (i=1; i<m_nPointNumber-1; i++) {
		h1 = m_pX[i]-m_pX[i-1];
		h2 = m_pX[i+1]-m_pX[i];
		del1 = (m_pY[i]-m_pY[i-1])/h1;
		del2 = (m_pY[i+1]-m_pY[i])/h2;
		alpha = h2/(h1+h2);
		m_pY0[i] = alpha;
		m_pY2[i] = 3.0*(alpha*del1+(1.0-alpha)*del2);
	}
	m_pY0[m_nPointNumber-1] = 0.0;
	m_pY2[m_nPointNumber-1] = 2.0*sright;
	
	/* solve tridiagonal system for slopes */
	t = 2.0;
	m_pY1[0] = m_pY2[0]/t;
	for (i=1; i<m_nPointNumber; i++) {
		m_pY3[i] = (1.0-m_pY0[i-1])/t;
		t = 2.0-m_pY0[i]*m_pY3[i];
		m_pY1[i] = (m_pY2[i]-m_pY0[i]*m_pY1[i-1])/t;
	}
	for (i=m_nPointNumber-2; i>=0; i--)
		m_pY1[i] -= m_pY3[i+1]*m_pY1[i+1];

	/* copy ordinates into output array */
	for (i=0; i<m_nPointNumber; i++)
		m_pY0[i] = m_pY[i];

	/* compute 2nd and 3rd derivatives of cubic polynomials */
	for (i=0; i<m_nPointNumber-1; i++) {
		h2 = m_pX[i+1]-m_pX[i];
		del2 = (m_pY[i+1]-m_pY[i])/h2;
		divdf3 = m_pY1[i]+m_pY1[i+1]-2.0*del2;
		m_pY2[i] = 2.0*(del2-m_pY1[i]-divdf3)/h2;
		m_pY3[i] = (divdf3/h2)*(6.0/h2);
	}
	m_pY2[m_nPointNumber-1] = m_pY2[m_nPointNumber-2]+(m_pX[m_nPointNumber-1]-m_pX[m_nPointNumber-2])*m_pY3[m_nPointNumber-2];
	m_pY3[m_nPointNumber-1] = m_pY3[m_nPointNumber-2];

	return true;
}


double CSpline::GetValue(double x)
{
	int klo, khi, k, khl;
	double  h, a, b;
	int n = m_nPointNumber;

	/////////////////////////////////
	//
	klo = 0;
	khi = n-1;
	khl = khi-klo;
	while(khl>1) {
	  k = (khi+klo)/2;
	  if(m_pX[k]>x) {
	     khi = k;
	  }
	  else {
	     klo = k;
	  }
	  khl = khi-klo;
	}
	
	/////////////////////////////////
	//
	h = m_pX[khi]-m_pX[klo];
	if(h==0.0){		
		return -9999;
	}

	a = (m_pX[khi]-x)/h;
	b = (x-m_pX[klo])/h;
	double y=a*m_pY[klo]+b*m_pY[khi]+((a*a*a-a)*m_pY2[klo]+(b*b*b-b)*m_pY2[khi])*(h*h)/6.0;
	
	return y;
}

/***** Compute One order Diffrential Coefficient *****/
double CSpline::GetDerivative1(double x)
{
	int klo, khi, k, khl;
	double h, a, b;
	int n = m_nPointNumber;

	klo = 0;
	khi = n-1;
	khl = khi-klo;
	while(khl>1) {
	  k = (khi+klo)/2;
	  if(m_pX[k]>x) {
	     khi = k;
	  }
	  else {
	     klo = k;
	  }
	  khl = khi-klo;
	}
	
	h = m_pX[khi]-m_pX[klo];
	if(h==0.0) {
		return -9999;
	}

	a = (m_pX[khi]-x)/h;
	b = (x-m_pX[klo])/h;
	double y1 = (m_pY[khi]-m_pY[klo])/h+
	     ((1.0-3.0*a*a)/h*m_pY2[klo]+(3.0*b*b-1.0)/h*m_pY2[khi])*h*h/6.0;

	return y1;
}

double CSpline::GetDerivative2(double x)
{
	int klo, khi, k, khl;
	double  h, a, b;
	
	int n=m_nPointNumber;
	
	klo = 0;
	khi = n-1;
	khl = khi-klo;
	while(khl>1) {
	  k = (khi+klo)/2;
	  if(m_pX[k]>x) {
	     khi = k;
	  }
	  else {
	     klo = k;
	  }
	  khl = khi-klo;
	}
	h = m_pX[khi]-m_pX[klo];
	if(h==0.0){
		return -9999;
	}

	a = (m_pX[khi]-x)/h;
	b = (x-m_pX[klo])/h;
	double y2 = a*m_pY2[klo]+b*m_pY2[khi];

	return y2;
}


long CSpline::GetPointNumber()
{		 
	return m_nPointNumber;
}

CMyPoint CSpline::GetPoint(long nOrder)
{
	if(nOrder<0||nOrder>=m_nPointNumber||!m_pX||!m_pY)return CMyPoint(0,0);
	return CMyPoint(m_pX[nOrder],m_pY[nOrder]);
}

bool CSpline::AddPoint(double x,double y)
{
	///////////////////////////////////////////
	// If it is Adding the first point:
	if(m_nPointNumber==0){
		SetPointNumber(1);
		SetData(0,x,y);
		return true;
	}
	
	//////////////////////////////////////////
	// Back up the memory:
	double *pXBak=new double [m_nPointNumber];
	double *pYBak=new double [m_nPointNumber];
	memcpy(pXBak,m_pX,sizeof(double)*m_nPointNumber);
	memcpy(pYBak,m_pY,sizeof(double)*m_nPointNumber);
	long i;
	
	////////////////////////////////////
	// Search for the insertion point:
	long nLow;
	if(x<pXBak[0])
		nLow=0;
	else if(x>pXBak[m_nPointNumber-1])
		nLow=m_nPointNumber-1;
	else
	{
		for(long i=0;i<m_nPointNumber;i++){
			if(x>=pXBak[i]&&x<=pXBak[i+1]){
				nLow=i;
				break;
			}
		}
	}

	////////////////////////////////////
	// Insert the point:
	SetPointNumber(m_nPointNumber+1);
	for(i=0;i<nLow;i++){
		SetData(i,pXBak[i],pYBak[i]);
	}
	SetData(nLow,x,y);
	for(i=nLow;i<m_nPointNumber-1;i++){
		SetData(i+1,pXBak[i],pYBak[i]);
	}

	delete []pXBak;
	delete []pYBak;

	
	////////////////////////////
	//  Calculate:
	return Over();
}

bool CSpline::DeletePoint(double x,double y)
{
	///////////////////////////////////////////
	// If it is Adding the first point:
	if(m_nPointNumber==0){
		return false;
	}

	////////////////////////////////////
	// Search for the insertion point:
	long nDeletePoint=-1;
	CMyPoint point(x,y);
	long i;
	for(i=0;i<m_nPointNumber;i++){
		if(point==CMyPoint(m_pX[i],m_pY[i+1])){
			nDeletePoint=i;
			break;
		}
	}
	if(nDeletePoint==-1)return false;
	
	////////////////////////////////////
	// Delete the point:	
	for(i=nDeletePoint;i<m_nPointNumber;i++)
	{
		m_pX[i]=m_pX[i+1];
		m_pY[i]=m_pY[i+1];
	}
	
	m_nPointNumber--;
	
	////////////////////////////
	//  Calculate:
	return Over();
}


void CSpline::Reset()
{
	if(m_pX){
		delete []m_pX;
		m_pX=NULL;
	}

	if(m_pY){
		delete []m_pY;
		m_pY=NULL;
	}

	if(m_pY0){
		delete []m_pY0;
		m_pY0=NULL;
	}

	if(m_pY1){
		delete []m_pY1;
		m_pY1=NULL;
	}

	if(m_pY2){
		delete []m_pY2;
		m_pY2=NULL;
	}

	if(m_pY3){
		delete []m_pY3;
		m_pY3=NULL;
	}
}

bool CSpline::SetMemory(long nPointNumber)
{
	if(nPointNumber<1||nPointNumber>10000){
		AfxMessageBox("Error: points number is error when creating spline:"+vtos(nPointNumber));
		return false;
	}

	long n=nPointNumber+10;

	if(m_pX){
		delete []m_pX;
		m_pX=NULL;
	}
	m_pX=new double [n];
	memset(m_pX,0,sizeof(double)*n);

	if(m_pY){
		delete []m_pY;
		m_pY=NULL;
	}
	m_pY=new double [n];
	memset(m_pY,0,sizeof(double)*n);

	if(m_pY0){
		delete []m_pY0;
		m_pY0=NULL;
	}
	m_pY0=new double [n];
	memset(m_pY0,0,sizeof(double)*n);


	if(m_pY1){
		delete []m_pY1;
		m_pY1=NULL;
	}
	m_pY1=new double [n];
	memset(m_pY1,0,sizeof(double)*n);

	if(m_pY2){
		delete []m_pY2;
		m_pY2=NULL;
	}
	m_pY2=new double [n];
	memset(m_pY2,0,sizeof(double)*n);

	if(m_pY3){
		delete []m_pY3;
		m_pY3=NULL;
	}
	m_pY3=new double [n];
	memset(m_pY3,0,sizeof(double)*n);

	m_nPointNumber=nPointNumber;

	return true;
}

