#pragma once

#ifndef FH_PASSWORD_TAG
#define FH_PASSWORD_TAG

#include "zoomview.h"

enum _FHZOOMVIEW_EXT_CLASS enumUserLevel
{
	USER_ADMINISTRATOR,
	USER_SUPERUSER,
	USER_COMMONUSER,
    USER_INVALIDUSER
};

CString        _FHZOOMVIEW_EXT_CLASS GetUserLevelName    (enumUserLevel  e);
enumUserLevel  _FHZOOMVIEW_EXT_CLASS GetUserLevel        (CString        sLevelName);

#include "MyDate.h"

class _FHZOOMVIEW_EXT_CLASS CPassword
{
public:
	char            sUserName[16];
	char            sPassword[16];
    CMyDate         dateStart;
    CMyDate         dateEnd;
    enumUserLevel   enumLevel;


	CPassword();
	CPassword(	CString sUserNameTag, 
				CString sPasswordTag,
				enumUserLevel enumLevel);

	CPassword(CPassword &other);
	void operator = (CPassword &other);
	bool operator == (CPassword &other);
	
    void Set(	CString         sUserName, 
				CString         sPassword,
				enumUserLevel   enumLevel,
                CMyDate         dateStart,
                CMyDate         dateEnd);

    CArchive  & Read(CArchive &ar);
    CArchive  & Write(CArchive &ar);

    void SetTimeLimit(CMyDate dateStartTag, CMyDate dateEndTag);
	void Encrypt();
	void Decrypt();
};


class _FHZOOMVIEW_EXT_CLASS CPasswordManager
{
public:
	CPasswordManager(void);
	~CPasswordManager(void);

protected:
	CPassword *m_password;
	long m_nPasswordLimit;
	long m_nPasswordQty;


public:

	bool Read(CString sFile);
	bool Save(CString sFile);
	CPassword * IsValid(CPassword &other);
	CPassword * IsValid(char sUserName[], char sPassword[]);
	CPassword * IsValid(CString sUserName, CString sPassword);
	bool Change();
	bool Add();
	bool Remove();
	bool RemoveAll();

	bool Add	(	CString sUserName, 
					CString sPassword,
					enumUserLevel enumLevel,
                    CMyDate dateStart,
                    CMyDate dateEnd);

	bool Remove(CPassword &other);
	bool Remove(CString sUserName, CString sPassword);
	bool Check();

	long  GetPasswordQty();
	enumUserLevel GetUserLevel(char sUser[],char sPassword[]);

	CPassword * GetPassword(long nIndex);

};

#endif