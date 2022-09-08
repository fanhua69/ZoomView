
#include "stdafx.h"
#include "passwordmanager.h"
#include "dlgpassword.h"
#include "DlgPasswordAdder.h"
#include "FHMainFrm.h"

CPassword::CPassword()
{
    memset(sUserName,0,16);
    memset(sPassword,0,16);
    enumLevel=USER_COMMONUSER;
}

CPassword::CPassword(CString sUserNameTag, 
    CString sPasswordTag,
    enumUserLevel enumLevelTag)
{
    memset(sUserName,0,16);
    memset(sPassword,0,16);
    enumLevel=USER_COMMONUSER;

    if(sUserNameTag.GetLength ()>14||sPasswordTag.GetLength ()>14)
    {
        return;
    }
    strcpy(sUserName,(LPCTSTR)sUserNameTag);
    strcpy(sPassword,(LPCTSTR)sPasswordTag);
    enumLevel= enumLevelTag;
}

CPassword::CPassword(CPassword &other)
{
    memset(sUserName,0,16);
    memset(sPassword,0,16);

    strcpy(sUserName,other.sUserName);
    strcpy(sPassword,other.sPassword);

    enumLevel=other.enumLevel;
    dateStart=other.dateStart;
    dateEnd=other.dateEnd;
}


void CPassword::operator = (CPassword &other)
{
    memset(sUserName,0,16);
    memset(sPassword,0,16);

    strcpy(sUserName,other.sUserName);
    strcpy(sPassword,other.sPassword);

    enumLevel=other.enumLevel;
    dateStart=other.dateStart;
    dateEnd=other.dateEnd;
}

bool CPassword::operator == (CPassword &other)
{
    return (  strcmp(sUserName,other.sUserName )==0
        &&strcmp(sPassword,other.sPassword)==0);
}


void CPassword::Encrypt()
{
    int i=0;
    while(sUserName[i])
    {
        sUserName[i]-=7;
        if(sUserName[i]<' ')
        {
            sUserName[i]-=' ';
            sUserName[i]+='z';
        }
        i++;
    }

    i=0;
    while(sPassword[i])
    {
        sPassword[i]-=7;
        if(sPassword[i]<' ')
        {
            sPassword[i]-=' ';
            sPassword[i]+='z';
        }
        i++;
    }
}

void CPassword::Decrypt()
{
    int i=0;
    while(sUserName[i])
    {
        sUserName[i]+=7;
        if(sUserName[i]>'z')
        {
            sUserName[i]-='z';
            sUserName[i]+=' ';
        }

        i++;
    }	

    i=0;
    while(sPassword[i])
    {
        sPassword[i]+=7;
        if(sPassword[i]>'z')
        {
            sPassword[i]-='z';
            sPassword[i]+=' ';
        }

        i++;
    }	
}

void CPassword::Set(CString			sUserNameTag, 
    CString			sPasswordTag,
    enumUserLevel	enumLevelTag,
    CMyDate         dateStartTag,
    CMyDate         dateEndTag)
{
    if(sUserNameTag.GetLength ()>16||sPasswordTag.GetLength ()>16)
    {
        return;
    }

    strcpy(sUserName,(LPCTSTR)sUserNameTag);
    strcpy(sPassword,(LPCTSTR)sPasswordTag);
    dateStart=dateStartTag;
    dateEnd=dateEndTag;

    enumLevel=enumLevelTag;
}

void CPassword::SetTimeLimit(CMyDate dateStartTag, CMyDate dateEndTag)
{
    dateStart   =dateStartTag;
    dateEnd     =dateEndTag;
}


CArchive  & CPassword::Write(CArchive &ar)
{
    ar.Write(sUserName,16);
    ar.Write(sPassword,16);
    dateStart.Write(ar);
    dateEnd.Write(ar);
    ar<<(int)enumLevel;

    return ar;
}

CArchive  & CPassword::Read(CArchive &ar)
{
    ar.Read(sUserName,16);
    ar.Read(sPassword,16);

    int n;
    dateStart.Read(ar);
    dateEnd.Read(ar);
    ar>>n;
    enumLevel=(enumUserLevel)n;

    return ar;
}



CPasswordManager::CPasswordManager(void)
{
    m_nPasswordLimit=999;
    m_nPasswordQty=0;

    m_password=new CPassword[1000];
}

CPasswordManager::~CPasswordManager(void)
{
    if(m_password)
    {
        delete []m_password;
        m_password=NULL;
    }

}
bool CPasswordManager::Read(CString sFile)
{
    if(sFile=="")return false;

    CFile file;
    if(!file.Open(sFile,CFile::modeRead))
    {
        return false;
    };
    CArchive ar(&file,CArchive::load);

    try
    {
        long nMark=0;
        ar>>nMark;
        if(nMark!=9999)
        {
            return false;
        }


        CPassword mark;
        mark.Read(ar);
        mark.Decrypt ();
        if(     strcmp(mark.sUserName ,"FHPASSWORD")
            ||  strcmp(mark.sPassword ,"MARK"))
        {
            ar.Close();
            file.Close();
            return false;
        }

        ///////////////////////////////
        //
        long i;
        ar>>m_nPasswordQty;

        for(i=0;i<m_nPasswordQty;i++)
        {
            m_password[i].Read(ar);
            m_password[i].Decrypt ();
        }
    }
    catch (CException* e)
    {
        file.Close();
        ar.Close();
        return false;
    }

    //////////////////////////////////
    //
    ar.Close();
    file.Close();

    return true;
}

bool  CPasswordManager::Save(CString sFile)
{
    CFile file;
    if(!file.Open(sFile,CFile::modeCreate | CFile::modeWrite))
    {
        return false;
    }

    CArchive ar(&file,CArchive::store);

    try
    {
        long nMark=9999;
        ar<<nMark;

        CPassword mark;
        strcpy(mark.sUserName ,"FHPASSWORD");
        strcpy(mark.sPassword ,"MARK");
        mark.Encrypt ();
        mark.Write(ar);

        ar<<m_nPasswordQty;

        ///////////////////////////////
        //
        ///////////////////////////////
        long i;
        CPassword word;
        for(i=0;i<m_nPasswordQty;i++)
        {
            word=m_password[i];
            word.Encrypt();
            word.Write(ar);
        };
    }
    catch (CException* e)
    {
        file.Close();
        ar.Close();
        return false;
    }

    //////////////////////////////////
    //
    ar.Close();
    file.Close();

    return true;
}


bool CPasswordManager::Add(CString sUserName, 
    CString sPassword,
    enumUserLevel enumLevel,
    CMyDate dateStart,
    CMyDate dateEnd)
{
    if(!IsValid(sUserName,sPassword))
    {
        if(m_nPasswordQty>=m_nPasswordLimit)return false;

        if(sUserName.GetLength()>14||sPassword.GetLength()>14)
        {
            TRACEERROR("用户名和口令均不能超过14个字节:"+sUserName+","+sPassword);
            return false;
        }

        strcpy(m_password[m_nPasswordQty].sUserName ,(LPCTSTR)sUserName);
        strcpy(m_password[m_nPasswordQty].sPassword ,(LPCTSTR)sPassword);
        m_password[m_nPasswordQty].enumLevel    =   enumLevel;
        m_password[m_nPasswordQty].dateStart    =   dateStart;
        m_password[m_nPasswordQty].dateEnd      =   dateEnd;
        m_nPasswordQty++;
    }
    return true;
}

CPassword * CPasswordManager::IsValid(CPassword &other)
{
    ///////////////////////////////
    //
    CTime currentTime;
    currentTime=CTime::GetCurrentTime();

    int iCurrentYear    =currentTime.GetYear();
    int iCurrentMonth   =currentTime.GetYear();
    int iCurrentDay     =currentTime.GetYear();
    CMyDate currentDate;
    currentDate.Set(iCurrentYear,iCurrentMonth,iCurrentDay);


    for(long i=0;i<m_nPasswordQty;i++)
    {
        if(   strcmp(m_password[i].sUserName,other.sUserName  )==0
            &&strcmp(m_password[i].sPassword ,other.sPassword )==0
            &&m_password[i].dateStart<=currentDate
            &&m_password[i].dateEnd>=currentDate)
        {
            return &m_password[i];
        }
    };

    return NULL;
}

CPassword * CPasswordManager::IsValid(char sUserName[], char sPassword[])
{
    return IsValid(CPassword(CString(sUserName),CString(sPassword),USER_COMMONUSER));
}

CPassword * CPasswordManager::IsValid(CString sUserName, CString sPassword)
{
    return IsValid(CPassword(sUserName,sPassword,USER_COMMONUSER));
}

bool CPasswordManager::Remove(CPassword &other)
{
    ///////////////////////////////
    //
    for(long i=0;i<m_nPasswordQty;i++)
    {
        if(m_password[i]==other)
        {
            for(long j=i+1;j<m_nPasswordQty;j++)
            {
                m_password[j-1]=m_password[j];
            }
            m_nPasswordQty--;
            break;
        }
    };


    return false;
}

bool CPasswordManager::Remove(CString sUserName, CString sPassword)
{
    return Remove(CPassword(sUserName,sPassword,USER_COMMONUSER));
}

bool CPasswordManager::Change()
{
    //////////////////////////////////
    //
    CDlgPasswordAdder dlg;
    dlg.SetTip("请输入旧用户名和口令");
    CString sUserName,sPassword;
    CPassword *pWord=NULL;

    while(true)
    {
        if(dlg.DoModal ()==IDCANCEL)return false;
        sUserName=dlg.m_sUserName ;

        pWord=IsValid(dlg.m_sUserName ,dlg.m_sPassword); //,dlg.m_sDateStart, dlg.m_sDateEnd );
        if(pWord)break;
        dlg.SetTip("旧用户名或者口令错误");
    }

    //////////////////////////////
    //
    dlg.SetTip("请输入新口令");
    dlg.m_sPassword ="";
    if(dlg.DoModal ()==IDCANCEL)return false;

    pWord->Set( dlg.m_sUserName ,
        dlg.m_sPassword,
        dlg.GetUserLevel (),
        dlg.m_sDateStart,
        dlg.m_sDateEnd);

    return true;

}
bool CPasswordManager::Add()
{
    //////////////////////////////////
    //
    CDlgPasswordAdder dlg;
    dlg.SetTip("请输入新口令");
    if(dlg.DoModal ()==IDCANCEL)return false;

    return Add( dlg.m_sUserName ,
        dlg.m_sPassword, 
        dlg.GetUserLevel(),
        dlg.m_sDateStart, 
        dlg.m_sDateEnd );
}

bool CPasswordManager::Remove()
{
    CDlgPassword dlg;
    dlg.SetTip("请输入需删除的用户名和口令");
    if(dlg.DoModal ()==IDCANCEL)return false;

    return Remove(dlg.m_sUserName ,dlg.m_sAnswer );
}

bool CPasswordManager::RemoveAll()
{
    m_nPasswordQty=0;
    return true;
}

bool CPasswordManager::Check()
{
    CDlgPassword dlg;
    dlg.SetTip("请输入的用户名和口令");
    int n=0;

    while(true)
    {
        if(dlg.DoModal ()==IDCANCEL)return false;

        if(IsValid(dlg.m_sUserName ,dlg.m_sAnswer ))
        {
            return true;
        }
        else if(n>=2)
        {
            return false;
        }
        else
        {
            dlg.SetTip("口令错误，请重新输入,最多输入三次!");
            n++;
        }
    }
}

long  CPasswordManager::GetPasswordQty()
{
    return m_nPasswordQty;
}

CPassword *  CPasswordManager::GetPassword(long nIndex)
{
    if(nIndex<0||nIndex>=m_nPasswordQty)return NULL;

    return &m_password[nIndex];
}


enumUserLevel CPasswordManager::GetUserLevel(char sUser[],char sPassword[])
{
    ///////////////////////////////
    //
    for(long i=0;i<m_nPasswordQty;i++)
    {
        if(   strcmp(m_password[i].sUserName,sUser )==0
            &&strcmp(m_password[i].sPassword ,sPassword )==0)
        {
            return m_password[i].enumLevel ;
        }
    };

    enumUserLevel e=USER_INVALIDUSER;
    return e;
}


CString GetUserLevelName(enumUserLevel e)
{
    if(e==USER_ADMINISTRATOR)
    {
        return "Administrator";
    }
    else if(e==USER_SUPERUSER)
    {
        return "Super User";
    }
    else if(e==USER_COMMONUSER)
    {
        return "Common User";
    }
    else if(e==USER_INVALIDUSER)
    {
        return "Invalid User";
    }
    else
    {
        return "Invalid User";
    }
}

enumUserLevel GetUserLevel(CString sLevelName)
{
    if(sLevelName== "Administrator")
    {
        return USER_ADMINISTRATOR;
    }
    else if(sLevelName=="Super User")
    {
        return USER_SUPERUSER;
    }
    else if(sLevelName=="Common User")
    {
        return USER_COMMONUSER;
    }
    else if(sLevelName=="Invalid User")
    {
        return USER_INVALIDUSER;
    }
    else
    {
        return USER_INVALIDUSER;
    }
}
