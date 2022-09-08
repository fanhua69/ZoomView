
/////////////////////////////////////////////
// ��Ȩ����: ������������˾
// ��    ����CDaoDataBase
// ��    ��: ���Microsoft Access ��ʽ���ݿ��
//           �������򿪡���������Ӽ�¼�Ȳ���
// ��    ��: ��  ��
// ������ڣ�2003��6��10��
// �� �� �ߣ� 
// �������:
//////////////////////////////////////////////

//////////////////////////////////////////////
//  �涨������������е��ֶ����ͱ�ʾ��������:
//  I ����
//  L ������
//  S �ַ�����
//  B BOOL��
//  F �����ȸ����� 
//  D ˫���ȸ����� 
///////////////////////////////////////////////

///////////////////////////////////////////////
// �����ļ���ʽΪ:
// [BOF]
// �ֶ�  ��������  ֵ
// �ֶ�  ��������  ֵ
// �ֶ�  ��������  ֵ
//  ...
// �ֶ�  ��������  ֵ
// [EOF]
//
// ��������Ϊ  >, <, =, !=, >=,<= , | (��������!|(��������
// ����:  | (��������!|(�������� ֻ�����ַ���
///////////////////////////////////////////////

#ifndef FH_DaoDataBase_Tag
#define FH_DaoDataBase_Tag


#include "zoomview.h"


class  _FHZOOMVIEW_EXT_CLASS CDaoDataBase
{
public:	
	// ������
	bool Create(CString sDataBaseName); 	
	bool CreateTable(CString sTableName, CString sItemNameFile);	
	bool AppendRecord(CString sTablename, CString sRecordFileName);
	
	
	// �޸�:
	bool Open(CString sDataBaseName);
	bool DeleteTable(CString sTableName);	
	bool DeleteRecord(CString sTableName,long nRecordOrder);
	bool DeleteRecord(CString sTableName,CString sConditionFile);

	// ��ȡ��Ϣ:
	bool Search(CString sTableName,CString sConditionFile,CString sResultFile);
	bool Sort(CString sTableName,CString sItem);
	
	CString GetDataBaseName();
	long GetTableQuantity();
	bool GetTableNames(CString sResultFile);
	bool GetTableItemNames(CString sTableName, CString sResultName);

	long GetRecordQuantity(CString sTableName);
	bool GetRecord(CString sTableName,long nRecordOrder,CString sResultFile);
	bool GetAllRecords(CString sTableName,CString sResultFile);

};

#endif

//////////////////////////////////////////////////////////////
// �� �� ����CDaoDataBase::Create
// ��    �ܣ�����һ���յ�Microsoft Access���ݿ�
// �����������
// �������: �ɹ�Ϊ��
//////////////////////////////////////////////////////////////
bool CDaoDataBase::Create(CString sDataBaseName)
{
	return true;
}


//////////////////////////////////////////////////////////////
// �� �� ����CDaoDataBase::Create
// ��    �ܣ�����һ���յ�Microsoft Access���ݿ�
// �����������
// �������: �ɹ�Ϊ��
//////////////////////////////////////////////////////////////
bool CDaoDataBase::AppendRecord(CString sTablename, CString sRecordFileName)
{
	return true;
}

//////////////////////////////////////////////////////////////
// �� �� ����CDaoDataBase::Open
// ��    �ܣ���һ���Ѵ��ڵ�Microsoft Access���ݿ�
// ��������� sDataBaseName	, ���ݿ������
// �������: �ɹ�Ϊ��
//////////////////////////////////////////////////////////////
bool CDaoDataBase::Open(CString sDataBaseName)
{
   	return true;
}

//////////////////////////////////////////////////////////////
// �� �� ����CDaoDataBase::DeleteTable
// ��    �ܣ�ɾ��һ���Ѵ��ڵ�Microsoft Access��
// ��������� sTableName, �������
// �������: �ɹ�Ϊ��
//////////////////////////////////////////////////////////////
bool CDaoDataBase::DeleteTable(CString sTableName)
{
   	return true;
}
	
//////////////////////////////////////////////////////////////
// �� �� ����CDaoDataBase::DeleteRecord
// ��    �ܣ�ɾ��һ���Ѵ��ڵ�Microsoft Access���е�һ����¼
// ���������
//                sTableName, �������
//                nRecordOrder����¼������  
// �������: �ɹ�Ϊ��
//////////////////////////////////////////////////////////////
bool CDaoDataBase::DeleteRecord(CString sTableName,long nRecordOrder)
{ 
   	return true;
}

//////////////////////////////////////////////////////////////
// �� �� ����CDaoDataBase::DeleteRecord
// ��    �ܣ�ɾ��һ���Ѵ��ڵ�Microsoft Access���е�һ����¼
// ���������
//                sTableName, �������
//                sConditionFile�������ļ�, �����������ı��ļ�,��ʽ��ͷ�ļ�
// �������: �ɹ�Ϊ��
//////////////////////////////////////////////////////////////
bool CDaoDataBase::DeleteRecord(CString sTableName,CString sConditionFile)
{
   	return true;
}

//////////////////////////////////////////////////////////////
// �� �� ����CDaoDataBase::Search
// ��    �ܣ�����һ���Ѵ��ڵ�Microsoft Access���еķ��������ļ�¼
// ���������
//                sTableName,      �������
//                sConditionFile�� �����ļ�, �����������ı��ļ�,��ʽ��ͷ�ļ�
// 				  sResultFile��    ������������ļ�¼
// �������: �ɹ�Ϊ��
//////////////////////////////////////////////////////////////
bool CDaoDataBase::Search(CString sTableName,CString sConditionFile,CString sResultFile)
{
	return true;
}

//////////////////////////////////////////////////////////////
// �� �� ����CDaoDataBase::Sort
// ��    �ܣ���һ���Ѵ��ڵ�Microsoft Access���а��ֶ�����
// ���������
//                sTableName,      �������
//                sItem            �ֶ�����
// �������: �ɹ�Ϊ��
//////////////////////////////////////////////////////////////
bool CDaoDataBase::Sort(CString sTableName,CString sItem)
{
	return true;
}

//////////////////////////////////////////////////////////////
// �� �� ����CDaoDataBase::GetDataBaseName
// ��    �ܣ����ر����ݿ������
// �����������
// �������: �ɹ�Ϊ��
//////////////////////////////////////////////////////////////
CString CDaoDataBase::GetDataBaseName()
{
	return CString("");
}

//////////////////////////////////////////////////////////////
// �� �� ����CDaoDataBase::GetTableQuantity
// ��    �ܣ����ر����ݿ�����ݱ�ĸ���
// �����������
// �������:  ���ݱ�ĸ����� ʧ�ܷ���-1
//////////////////////////////////////////////////////////////
long CDaoDataBase::GetTableQuantity()
{
	return 0;
};

//////////////////////////////////////////////////////////////
// �� �� ����CDaoDataBase::GetTableNames
// ��    �ܣ����ر����ݿ�����ݱ������
// ���������sResultFile�� ����ļ����������б������д����ļ���
// �������:  �ɹ�Ϊ��	   
//////////////////////////////////////////////////////////////
bool CDaoDataBase::GetTableNames(CString sResultFile)
{
}

//////////////////////////////////////////////////////////////
// �� �� ����CDaoDataBase::GetTableItemNames
// ��    �ܣ����ر����ݿ�����ݱ�ĸ����ֶε�����
// ��������� sTableName,   �������
//            sResultFile�� ����ļ������ѱ����е��ֶ�����д����ļ���
// �������:  �ɹ�Ϊ��	   
//////////////////////////////////////////////////////////////
bool CDaoDataBase::GetTableItemNames(CString sTableName, CString sResultName)
{
}

//////////////////////////////////////////////////////////////
// �� �� ����CDaoDataBase::GetRecordQuantity
// ��    �ܣ����ر����ݿ��ĳ�����ݱ�ļ�¼��Ŀ
// ��������� sTableName,   �������
// �������:  ���ݱ�ļ�¼��Ŀ��ʧ��Ϊ-1	   
//////////////////////////////////////////////////////////////
long CDaoDataBase::GetRecordQuantity(CString sTableName)
{
}


//////////////////////////////////////////////////////////////
// �� �� ����CDaoDataBase::GetRecord
// ��    �ܣ����ر����ݿ��ĳ�����ݱ��һ����¼
// ��������� sTableName,   �������
//            nRecordOrder�� ��¼�����   
//            sResultFile��   ����ļ�
// �������:  �ɹ�Ϊ��	   
//////////////////////////////////////////////////////////////
bool CDaoDataBase::GetRecord(CString sTableName,long nRecordOrder,CString sResultFile)
{
}

//////////////////////////////////////////////////////////////
// �� �� ����CDaoDataBase::GetRecord
// ��    �ܣ����ر����ݿ��ĳ�����ݱ�����м�¼
// ��������� sTableName,   �������
//            sResultFile��   ����ļ�
// �������:  �ɹ�Ϊ��	   
//////////////////////////////////////////////////////////////
bool CDaoDataBase::GetAllRecords(CString sTableName,CString sResultFile)
{
}

//////////////////////////////////////////////////////////////
// �� �� ����CDaoDataBase::CreateTable
// ��    �ܣ����ݱ����ļ�����һ��Microsoft Access���ݿ�
// ���������
//         1. sTableName    �����������ݿ�������
//         2. sItemNameFile : ����ֶ����Ƶ��б��ļ�
// �������:
//         �ɹ�Ϊ��
//////////////////////////////////////////////////////////////
bool CDaoDataBase::CreateTable(CString sTableName, CString sItemNameFile)
{
	return true;
}


