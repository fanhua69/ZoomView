// FontGroup.cpp: implementation of the CFontGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "zoomview.h"
#include "FontGroup.h"
#include "math.h"
#include "templates.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

double dPlusValueStep[]=
	{
        10000000000000000000,
        5000000000000000000,
        1000000000000000000,
        500000000000000000,
        100000000000000000,
        50000000000000000,
        10000000000000000,
        5000000000000000,
        1000000000000000,
        500000000000000,
        100000000000000,
		50000000000000, 
		20000000000000, 	
		10000000000000,	
		5000000000000,  
		2000000000000,  
		1000000000000,  
		500000000000,   
		200000000000,	
		100000000000,	
		50000000000,    
		20000000000,    
		10000000000,    
		50000000000,    
		20000000000,    
		10000000000,    
		5000000000,     
		2000000000,     
		1000000000,     
		500000000,      
		200000000,      
		100000000,      
		50000000,       
		20000000,        
		10000000,        
		5000000,        
		250000,         
		200000,         
		100000,         
		50000,          
		25000,          
		20000,          
		10000,          
		5000,           
		2500,           
		2000,           
		1000,           
		500,            
		250,            
		200,            
		100,            
		75,             
		50,             
		40,             
		25,             
		20,             
		10,
        8,
        7,
        6,
		5,
        4,
        3,
		2,
        1.5,
		1,  
		0.5,            
		0.2,            
		0.1,            
		0.05,           
		0.02,           
		0.01,           
		0.005,          
		0.002,          
		0.001,          
		0.0005,         
		0.0002,         
		0.0001,         
		0.00005,        
		0.00002,        
		0.00001,        
		0.000005,       
		0.000002,       
		0.000001,       
		0.0000005,      
		0.0000002,      
		0.0000001,      
		0.00000005,     
		0.00000002,     
		0.00000001,     
		0.000000005,    
		0.000000002,	
		0.000000001
	};	

long nValueStepNumber=sizeof(dPlusValueStep)/sizeof(double);


double dImportantMark[]=
	{
		0,	
        10000000000000000000,
        5000000000000000000,
        1000000000000000000,
        500000000000000000,
        100000000000000000,
        50000000000000000,
        10000000000000000,
        5000000000000000,
        1000000000000000,
        500000000000000,
        100000000000000,
		50000000000000, 
		10000000000000,	
		5000000000000,  
		1000000000000,  
		500000000000,   
		100000000000,	
		50000000000,    
		10000000000,    
		50000000000,    
		10000000000,    
		5000000000,     
		1000000000,     
		500000000,      
		100000000,      
		50000000,       
		10000000,        
		5000000,        
		100000,         
		50000,          
		10000,          
		5000,           
		1000,           
		500,            
		100,            
		50,             
		10,             
		5,
        4,
        3,
        2,
        1.5,
		1,   
		0.5, 
        0.2,
		0.1,  
        0.08,
		0.05,  
        0.02,
		0.01,           
		0.005,          
		0.001,          
		0.0005,         
		0.0001,         
		0.00005,        
		0.00001,        
		0.000005,       
		0.000001,       
		0.0000005,      
		0.0000001,      
		0.00000005,     
		0.00000001,     
		0.000000005,    
		0.000000001
	};	

long nImportantMarkNumber=sizeof(dImportantMark)/sizeof(double);





CFontGroup::CFontGroup()
{
	m_dMinValue=0.0;
	m_dMaxValue=0.0;
	m_dWindowHeight=100;
	m_nFontHeight=10;
}

CFontGroup::~CFontGroup()
{
}

void CFontGroup::SetValueRange(double dMinValue, double dMaxValue)
{
	double dLimit=9999999999999999999.9;
	m_dMinValue=dMinValue;
	m_dMaxValue=dMaxValue;

	if(m_dMaxValue<m_dMinValue){
		Swap(m_dMaxValue,m_dMinValue);
	}

	if(m_dMinValue<-dLimit)m_dMinValue=-dLimit;
	if(m_dMaxValue>dLimit)m_dMaxValue=dLimit;

}
void CFontGroup::SetWindowHeight(double  dWindowHeight)
{
	m_dWindowHeight=dWindowHeight;
}
void CFontGroup::SetFontHeight(long  nFontHeight)
{
	m_nFontHeight=nFontHeight;
}

void CFontGroup::Set(double dMinValue, double dMaxValue,double dWindowHeight,long  nFontHeight)
{
	m_dMinValue=dMinValue;
	m_dMaxValue=dMaxValue;
	m_dWindowHeight=dWindowHeight;
	m_nFontHeight=nFontHeight;

	double dLimit=9999999999999999999.9;

	if(m_dMaxValue<m_dMinValue)
    {
		Swap(m_dMaxValue,m_dMinValue);
	}

	if(m_dMinValue<-dLimit)
		m_dMinValue=-dLimit;
	if(m_dMaxValue>dLimit)
		m_dMaxValue=dLimit;
	
	if(m_dWindowHeight<0)m_dWindowHeight=1;
	if(m_nFontHeight<1)m_nFontHeight=1;
}

bool CFontGroup::CalcFontGroup(double &dValueFrom,double &dValueTo,double &dValueStep)
{
    // 
    double dMinStep = 0.0000001;
    dValueFrom  =   m_dMinValue;
    dValueTo    =   m_dMaxValue;

	// Calc the value step:
	long nValue=(long)(m_dWindowHeight/(m_nFontHeight*1.5));	
	if(nValue<1)
	{
		dValueStep=m_dMaxValue-m_dMinValue;
        if(dValueStep<dMinStep)
        {
            dValueStep = dMinStep;
        }
		return false;
	}
    else if(nValue > 1000)
    {
        dValueStep  =   m_dMaxValue-m_dMinValue;
        if(dValueStep<dMinStep)
        {
            dValueStep = dMinStep;
        }
        return false;        
    }

	long i=0;
	dValueStep=(m_dMaxValue-m_dMinValue)/nValue;

	if(dValueStep<dMinStep)
	{
		dValueStep=dMinStep;
		return false;		
	}

	
	if(dValueStep>dPlusValueStep[0])
	{
		dValueStep=dPlusValueStep[0];
	}
	else if(dValueStep<=0)
	{
		dValueStep=1;
	}
	else
	{
		for(i=1;i<nValueStepNumber;i++)
		{
			if(dValueStep<=dPlusValueStep[i-1]&&dValueStep>=dPlusValueStep[i])
			{
				dValueStep=dPlusValueStep[i-1];
                break;
			}
		}
	}

	if(dValueStep<dMinStep)
	{
		dValueStep=dMinStep;
		return false;		
	}
	
	// Calc from which value :
	double dMulti;
	if(dValueStep<0.0000001)
		dMulti=0;
	else if(dValueStep<0.000001)
		dMulti=100000000;
	else if(dValueStep<0.00001)
		dMulti=10000000;
	else if (dValueStep<0.0001)
		dMulti=1000000;
	else if(dValueStep<0.001)
		dMulti=100000;
	else if(dValueStep<0.01)
		dMulti=10000;
	else if(dValueStep<0.1)
		dMulti=1000;
	else if(dValueStep<1)
		dMulti=100;
    //else if(dValueStep<1.5)
    //    dMulti=1;
    else
		dMulti=10;

	long nDevider=dValueStep*dMulti;
	if(dMulti==0||nDevider==0)
	{
		dValueFrom=m_dMinValue;
		dValueTo=m_dMaxValue;
	}
	else
	{		
		long n=(long(m_dMinValue*dMulti))/nDevider;
		dValueFrom=n*dValueStep;	
		
        // the number is too big:
        if(dValueFrom<=m_dMinValue-dValueStep||dValueFrom>=m_dMaxValue+dValueStep)
        {
			dValueFrom=m_dMinValue;
		}

		n=(long(m_dMaxValue*dMulti))/(long(dValueStep*dMulti));
		dValueTo=n*dValueStep+dValueStep;	
		
        // the number is too big:
        if(dValueTo<m_dMaxValue-dValueStep||dValueTo>m_dMaxValue+dValueStep)
        {  
			dValueTo=m_dMaxValue;
		}
	}

	if((dValueTo-dValueFrom)/dValueStep>10000)
	{
		double dMid=(dValueTo+dValueFrom)/2.0;
		double dRange=dValueStep*5000;
		dValueFrom=dMid-dRange;
		dValueTo=dMid+dRange;
	}

	/////////////////////////////////////////////////////////////
	// If The important marks (such as  Zero ) 
	// is in the mid of the range, let Zero be a Mark:
	double v,dMid,dMinDis,dMinDisValue;
	for(long i=0;i<nImportantMarkNumber;i++)
	{
		dMid=dImportantMark[i];
		if(dValueFrom<=dMid&&dValueTo>=dMid)
		{
			
			// Search for the value that is nearest to the MARK:
			dMinDis=fabs(dValueTo-dMid);
			dMinDisValue=dValueTo;
			for(v=dValueFrom;v<=dValueTo;v+=dValueStep)
			{
				if(fabs(v-dMid)<dMinDis)
				{
					dMinDis=fabs(v-dMid);
					dMinDisValue=v;
				}
			}
			
			// Minus the start value and end value by the nearest value:
			dValueFrom-=(dMinDisValue-dMid);
			dValueTo-=(dMinDisValue-dMid);
			break;
		}

		dMid=-dImportantMark[i];
		if(dValueFrom<=dMid&&dValueTo>=dMid)
		{
			
			// Search for the value that is nearest to the MARK:
			dMinDis=fabs(dValueTo-dMid);
			dMinDisValue=dValueTo;
			for(v=dValueFrom;v<=dValueTo;v+=dValueStep)
			{
				if(fabs(v-dMid)<dMinDis)
				{
					dMinDis=fabs(v-dMid);
					dMinDisValue=v;
				}
			}
			
			// Minus the start value and end value by the nearest value:
			dValueFrom-=(dMinDisValue-dMid);
			dValueTo-=(dMinDisValue-dMid);
			break;
		}

	}
	
	return true;
}
	

