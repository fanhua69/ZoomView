
/////////////////////////////////////////////
// 版权所有: 东方地球物理公司
// 类    名：CDaoDataBase
// 功    能: 完成Microsoft Access 格式数据库的
//           创建、打开、建立表、添加记录等操作
// 设    计: 范  华
// 设计日期：2003年6月10日
// 完 成 者： 
// 完成日期:
//////////////////////////////////////////////

//////////////////////////////////////////////
//  规定输入输出过程中的字段类型表示方法如下:
//  I 整型
//  L 长整形
//  S 字符串型
//  B BOOL型
//  F 单精度浮点型 
//  D 双精度浮点型 
///////////////////////////////////////////////

///////////////////////////////////////////////
// 条件文件格式为:
// [BOF]
// 字段  条件符号  值
// 字段  条件符号  值
// 字段  条件符号  值
//  ...
// 字段  条件符号  值
// [EOF]
//
// 条件符号为  >, <, =, !=, >=,<= , | (包含），!|(不包含）
// 其中:  | (包含），!|(不包含） 只用于字符串
///////////////////////////////////////////////

#ifndef FH_DaoDataBase_Tag
#define FH_DaoDataBase_Tag


#include "zoomview.h"


class  _FHZOOMVIEW_EXT_CLASS CDaoDataBase
{
public:	
	// 创建：
	bool Create(CString sDataBaseName); 	
	bool CreateTable(CString sTableName, CString sItemNameFile);	
	bool AppendRecord(CString sTablename, CString sRecordFileName);
	
	
	// 修改:
	bool Open(CString sDataBaseName);
	bool DeleteTable(CString sTableName);	
	bool DeleteRecord(CString sTableName,long nRecordOrder);
	bool DeleteRecord(CString sTableName,CString sConditionFile);

	// 获取信息:
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
// 函 数 名：CDaoDataBase::Create
// 功    能：建立一个空的Microsoft Access数据库
// 输入参数：无
// 输出参数: 成功为真
//////////////////////////////////////////////////////////////
bool CDaoDataBase::Create(CString sDataBaseName)
{
	return true;
}


//////////////////////////////////////////////////////////////
// 函 数 名：CDaoDataBase::Create
// 功    能：建立一个空的Microsoft Access数据库
// 输入参数：无
// 输出参数: 成功为真
//////////////////////////////////////////////////////////////
bool CDaoDataBase::AppendRecord(CString sTablename, CString sRecordFileName)
{
	return true;
}

//////////////////////////////////////////////////////////////
// 函 数 名：CDaoDataBase::Open
// 功    能：打开一个已存在的Microsoft Access数据库
// 输入参数： sDataBaseName	, 数据库的名称
// 输出参数: 成功为真
//////////////////////////////////////////////////////////////
bool CDaoDataBase::Open(CString sDataBaseName)
{
   	return true;
}

//////////////////////////////////////////////////////////////
// 函 数 名：CDaoDataBase::DeleteTable
// 功    能：删除一个已存在的Microsoft Access表
// 输入参数： sTableName, 表的名称
// 输出参数: 成功为真
//////////////////////////////////////////////////////////////
bool CDaoDataBase::DeleteTable(CString sTableName)
{
   	return true;
}
	
//////////////////////////////////////////////////////////////
// 函 数 名：CDaoDataBase::DeleteRecord
// 功    能：删除一个已存在的Microsoft Access表中的一条记录
// 输入参数：
//                sTableName, 表的名称
//                nRecordOrder，记录的名称  
// 输出参数: 成功为真
//////////////////////////////////////////////////////////////
bool CDaoDataBase::DeleteRecord(CString sTableName,long nRecordOrder)
{ 
   	return true;
}

//////////////////////////////////////////////////////////////
// 函 数 名：CDaoDataBase::DeleteRecord
// 功    能：删除一个已存在的Microsoft Access表中的一条记录
// 输入参数：
//                sTableName, 表的名称
//                sConditionFile，条件文件, 记载条件的文本文件,格式见头文件
// 输出参数: 成功为真
//////////////////////////////////////////////////////////////
bool CDaoDataBase::DeleteRecord(CString sTableName,CString sConditionFile)
{
   	return true;
}

//////////////////////////////////////////////////////////////
// 函 数 名：CDaoDataBase::Search
// 功    能：查找一个已存在的Microsoft Access表中的符合条件的记录
// 输入参数：
//                sTableName,      表的名称
//                sConditionFile， 条件文件, 记载条件的文本文件,格式见头文件
// 				  sResultFile，    输出符合条件的纪录
// 输出参数: 成功为真
//////////////////////////////////////////////////////////////
bool CDaoDataBase::Search(CString sTableName,CString sConditionFile,CString sResultFile)
{
	return true;
}

//////////////////////////////////////////////////////////////
// 函 数 名：CDaoDataBase::Sort
// 功    能：对一个已存在的Microsoft Access表中按字段排序
// 输入参数：
//                sTableName,      表的名称
//                sItem            字段名称
// 输出参数: 成功为真
//////////////////////////////////////////////////////////////
bool CDaoDataBase::Sort(CString sTableName,CString sItem)
{
	return true;
}

//////////////////////////////////////////////////////////////
// 函 数 名：CDaoDataBase::GetDataBaseName
// 功    能：返回本数据库的名称
// 输入参数：无
// 输出参数: 成功为真
//////////////////////////////////////////////////////////////
CString CDaoDataBase::GetDataBaseName()
{
	return CString("");
}

//////////////////////////////////////////////////////////////
// 函 数 名：CDaoDataBase::GetTableQuantity
// 功    能：返回本数据库的数据表的个数
// 输入参数：无
// 输出参数:  数据表的个数， 失败返回-1
//////////////////////////////////////////////////////////////
long CDaoDataBase::GetTableQuantity()
{
	return 0;
};

//////////////////////////////////////////////////////////////
// 函 数 名：CDaoDataBase::GetTableNames
// 功    能：返回本数据库的数据表的名称
// 输入参数：sResultFile， 输出文件名，把所有表的名称写入此文件中
// 输出参数:  成功为真	   
//////////////////////////////////////////////////////////////
bool CDaoDataBase::GetTableNames(CString sResultFile)
{
}

//////////////////////////////////////////////////////////////
// 函 数 名：CDaoDataBase::GetTableItemNames
// 功    能：返回本数据库的数据表的各个字段的名称
// 输入参数： sTableName,   表的名称
//            sResultFile， 输出文件名，把表所有的字段名称写入此文件中
// 输出参数:  成功为真	   
//////////////////////////////////////////////////////////////
bool CDaoDataBase::GetTableItemNames(CString sTableName, CString sResultName)
{
}

//////////////////////////////////////////////////////////////
// 函 数 名：CDaoDataBase::GetRecordQuantity
// 功    能：返回本数据库的某个数据表的记录数目
// 输入参数： sTableName,   表的名称
// 输出参数:  数据表的记录数目，失败为-1	   
//////////////////////////////////////////////////////////////
long CDaoDataBase::GetRecordQuantity(CString sTableName)
{
}


//////////////////////////////////////////////////////////////
// 函 数 名：CDaoDataBase::GetRecord
// 功    能：返回本数据库的某个数据表的一条记录
// 输入参数： sTableName,   表的名称
//            nRecordOrder， 记录的序号   
//            sResultFile，   结果文件
// 输出参数:  成功为真	   
//////////////////////////////////////////////////////////////
bool CDaoDataBase::GetRecord(CString sTableName,long nRecordOrder,CString sResultFile)
{
}

//////////////////////////////////////////////////////////////
// 函 数 名：CDaoDataBase::GetRecord
// 功    能：返回本数据库的某个数据表的所有记录
// 输入参数： sTableName,   表的名称
//            sResultFile，   结果文件
// 输出参数:  成功为真	   
//////////////////////////////////////////////////////////////
bool CDaoDataBase::GetAllRecords(CString sTableName,CString sResultFile)
{
}

//////////////////////////////////////////////////////////////
// 函 数 名：CDaoDataBase::CreateTable
// 功    能：根据表项文件建立一个Microsoft Access数据库
// 输入参数：
//         1. sTableName    ：产生的数据库表的名称
//         2. sItemNameFile : 表的字段名称的列表文件
// 输出参数:
//         成功为真
//////////////////////////////////////////////////////////////
bool CDaoDataBase::CreateTable(CString sTableName, CString sItemNameFile)
{
	return true;
}


