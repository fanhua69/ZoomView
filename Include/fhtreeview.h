#if !defined(AFX_FHTREEVIEW_H__650D9F81_CF19_11D3_AEF5_00C04FCCB957__INCLUDED_)
#define AFX_FHTREEVIEW_H__650D9F81_CF19_11D3_AEF5_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FHTreeView.h : header file
//

#include "zoomview.h"
#include "FHBaseDoc.h"
#include "MultiChildDoc.h"
#include "MultiChildView.h"


#define LINE_COMMON 0x0000
#define REFRACTION_LOG 0x0001
#define REFRACTION_LAYOUT 0x0010

/////////////////////////////////////////////////////////////////////////////
// CFHTreeView view

#include <afxcview.h>
#include "LineProperty.h"
#include "PopupMenuManager.h"
#include "FHZoomViewGlobal.h"
#include "FHApp.h"

class CMenuItem
{
public:
    UINT nID;
    char sItemName[100];

    CMenuItem()
    {
        nID=0;
        memset(sItemName,0,90);
    }
};


#include "FHTreeViewBase.h"

class _FHZOOMVIEW_EXT_CLASS CFHTreeView : public CFHTreeViewBase 
{

protected:
    CFHTreeView();                         // 受保护的动态构造函数
    DECLARE_DYNCREATE(CFHTreeView)
    virtual ~CFHTreeView();

    // 受保护的属性：
protected:

    CPopupMenuManager m_menuManager;

    CString m_sDataBaseMark;               // 数据库标志
    CString m_sProjectDiscription;         // 数据库描述字符串
    CString m_sProjectExt;			       // 数据库扩展名

    // 针对子数据库的变量:
    CString m_sWorkDir;				       // 工作目录
    CString m_sDataBaseName;			   // 数据库名称
    CString m_sDataBasePath;               // 数据库路径 
    CString m_sDataExt;                    // 数据库中所含数据文件的扩展名，例如：测量文件 *.h
    CString m_sDataDiscription;            // 数据库中数据类型的描述

    CString m_sRootName;                   // 树根的描述
    CString m_sFirstLayerName;             // 第一层的名称
    CString m_sSecondLayerName;            // 第二层的名称
    CMenuItem m_menuItems[20];             // 右键菜单的描述

    CImageList m_ImageList;				   // 表示数据库各种状态的图片列表 

    CLineProperty *m_pLinePropertyInArea;  // 用于存储数据库中测线的属性的结构的指针
    CString *m_pAreaNames;				   // 数据库中所包含的工区名列表

    CFHTreeView **m_pPointerToThis;        //  外部指向自身的指针的地址，用于告知外部环境自身的位置

    bool m_bShowFullName;				   //  是否显示数据库内容的完整路径
    bool m_bShowSortedName;				   //  是否对数据库内容按名称排序
    long m_nInsertOrder;				   //  记录数据库内容插入的当前顺序
    long m_nImageList;                     //  数据树图片的ＩＤ号。  

    CString m_sAreaCurrent;				   //  当前工区名称
    CString m_sLineCurrent;				   //  当前测线名称

    CString m_sTreeName;
    bool    m_bTextFile;


public:
    CString m_sError;                     //  记录错误信息

    ////////////////////////////////////////////
    // 受保护的成员函数
protected:
    virtual bool    ReadDataBase            (); // 读取数据库, 是被OpenDataBase调用的。
    void            Reset                   (); // 数据库复位
    void            RemoveTreeData          (); // 删除数据库内所有测线的属性
    bool            InsertLineNamesFromFile (CString sFile); // 把文件sFile中的文件名作为测线插入到数据库中
    void            DealSelChanged          (); // 处理当前选定的测线发生变化的情形
    void            SortTree                (); // 对文件树进行排序
    bool            SaveDataBase            ();

    void            AddMenu                 (UINT MenuID, CString MenuString);

    //////////////////////////////////////
    // 公共函数
public:

    // 与数据库创建有关的：
    CString CreateDataBase(CString sPath);
    bool    MakeNewDataBase() ;                                     // 建立新数据库 
    void    SetFileProperty(CString sDiscription,CString sExt);     // 设置设置能够被加入数据库的文件的属性
    void    SetDataBaseProperty(CString sDiscription,CString sExt); // 设定数据库的属性

    bool    ConstructAreaPath(char *cAreaPath,CString sArea);          // 构造工区路径的一种重载形式
    CString ConstructDataBaseName();                                // 构造数据库名称
    CString ConstructAreaPath(CString sArea);                       // 构造工区路径
    CString ConstructLinePath(CString sArea, CString sLine);        // 构造测线路径
    virtual bool OpenDataBase();                                    // 打开数据库，调用 ReadDataBase
    virtual bool OpenDataBase(CString sPathName, bool bUpdateSolution=true);
    void    SetImageList(UINT nImageList);                          // 数据树的图片位图
    CString GetDataBaseDiscription();
    CString GetDataBaseNameExt();
    void    ClearTree();
    int     GetChildQty(HTREEITEM item);


    virtual bool    InsertArea(     CString sArea);
    virtual CString InsertArea();                                       // 在数据库中插入一个工区
    virtual bool    InsertLine(    CString sFile);                      // 插入测线。允许在某工区下插入多条测线。插入的测线路径及其名称写入文件sFile中。

    virtual bool    InsertLine(     CString     sArea,
                                    CString     sLineFile, 
                                    int         nDataProperty=0, 
                                    int         nImageOrder=0, 
                                    int         nSelectedImageOrder=0,
                                    int         nInsertOrder=0);

    virtual bool    InsertLine(     HTREEITEM   AreaItem,
                                    CString     sLineName,
                                    int         nDataProperty=0,
                                    int         nImageOrder=0,
                                    int         nSelectedImageOrder=0,
                                    int         nInsertOrder=0);

    virtual CString RemoveArea  ();	// 删除工区，返回所删除的工区名字
    virtual bool    RemoveArea  (CString sArea);
    bool            RemoveLine  (CString &sArea,CString &sLine);             // 删除工区sArea中的测线sLine	


    //virtual bool SaveData();											 // 存储测线数据 

    // 与树的显示和右键菜单有关的
    void        SetLayerNames(CString sRootName,CString sFirstLayer,CString sSecondLayer);


    // 与数据库读取有关的：
    CString     GetDataBaseName     ();
    long        GetAreaNumber       ();                                       // 获得当前打开的数据库内的工区的数目
    long        GetTotalLineNumber  ();									// 获得数据库内总的测线数目
    CString     GetFileName         (CString sArea, CString sLine);

    long        GetLineNumberInArea (HTREEITEM ItemArea);               // 获得当前数据库中工区ItemArea内的测线数目
    long        GetLineNumberInArea (CString sArea);					// 获得当前数据库中工区sArea内的测线数目
    long        GetDataProperty     (CString sArea,CString sLine);          // 获得工区sArea内的测线sLine的属性
    long        GetSelectedLineProperty();								// 获得当前选定的测线的属性

    CString *   GetAreaNames        (long &nAreaNumber);					// 获得所有工区的名字
    CLineProperty* GetLineNamesInArea(CString sArea,long &nLineNumber);   // 获得工区sArea内所有测线的属性，nLineNumber返回工区内测线的数目 

    HTREEITEM   IsAreaInDataBase    (   CString sArea);					// 该工区sArea是否在数据库内
    HTREEITEM   IsLineInDataBase    (   CString sArea,
                                        CString sLine, 
                                        UINT    nDataProperty);    // 该测线sLine(属于工区sArea)是否在数据库内

    CString     ExistLineTypeInArea (CString sArea,long nDataType);	// 工区sArea内是否存在类型为nDataType的测线 

    HTREEITEM   GetSelectedAreaItem ();                         // 获得当前选定的工区的ITEM
    HTREEITEM   GetSelectedLineItem ();                         // 获得当前选定的测线的ITEM
    CString     GetSelectedAreaName ();                         // 获得当前选定的工区的ITEM
    CString     GetSelectedLineName ();                         // 获得当前选定的测线的ITEM

    CString     SearchLineData              (   CString sArea,
                                                CString sLine,
                                                UINT nDataType);        // 搜索测线sLine的位置 

    void        SetPointerToThis            (   CFHTreeView **pPointerInApp);			// 设定本类的指针

    enumDimension GetDimension              (   CString sArea);
    virtual enumDimension GetFileDimension  (   CString sFile);

    bool        RemoveLineInDataBase        (   CString sArea, 
                                                CString sLine, 
                                                UINT nDataProperty);


    ////////////////////////////////////////////
    // 受保护的虚函数:
protected:
    virtual bool MakeNewLineData    ();                                                         // 新建一条新测线数据
    virtual bool DrawLineData       (CString sArea,CString sFile);                              // 现实测线数据
    virtual bool DrawLineMap        (CString sArea,CString sFile);                              // 绘制测线图形
    virtual bool DrawAreaMap        (CString sArea,CLineProperty *pLines,long nLineNumber);     // 绘制工区图
    virtual bool Draw2DLineMap      (CString sArea, CString sLine);
    virtual bool Draw3DLineMap      (CString sArea, CString sLine);



    virtual bool UpdateDrawingArea( CString sArea,
                                    CLineProperty *pLine,
                                    long nLineNumber,
                                    CString sLineCurrent);                   // 重绘工区内容

    virtual bool UpdateDrawingLine( CString sArea,
                                    CString sLineCurrent);                   // 重绘测线图形


    CMultiDocTemplate * m_pTemplate;  // 临时变量，不得删除

    bool                        AddTemplate                     (CMultiDocTemplate *pTemplate);
    virtual bool                AddTemplates                    (   );
    bool                        AddBaseTemplates                (   );
    virtual bool                RemoveTemplates                 (   );

    virtual CString             GetMultiDocTemplateName         (   );

    bool                        CreateMultiDocView              (   );

    CMultiChildDoc *            OnCreateDocForBaseView          (   CString         sFileName   );

    CMultiChildDoc *            OnCreateDocForBaseView          (   CString         sArea, 
                                                                    CLineProperty   *p,
                                                                    int             nLineNumber );

    CMultiChildView *           OnCreateViewForBaseView         (   );

    virtual CMultiChildDoc  *   CreateDocForBaseView            (   CString         sArea, 
                                                                    CLineProperty   *p,
                                                                    int             nLineNumber );

    virtual CMultiChildDoc  *   CreateDocForBaseView            (   CString         sFileName   );

    virtual CMultiChildView *   CreateViewForBaseView           (   );

    bool                        AddCurrentItemIntoBaseMap       (   bool            bForceAddArea );
    bool                        RemoveCurrentItemFromBaseMap    (   );
    int                         MultiDocViewContainsCurrentItem (    );
    void                        GetCurrentItemDocNames          (   CStringList     &listLineNames);

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CFHTreeView)
public:
    //	virtual void OnInitialUpdate();
protected:
    virtual void OnDraw(CDC* pDC);      // overridden to draw this view
    //}}AFX_VIRTUAL

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    // Generated message map functions

    //{{AFX_MSG(CFHTreeView)
    afx_msg void OnFHTreeNewDataBase();
    afx_msg void OnFHTreeInsertArea();
    afx_msg void OnFHTreeInsertLine();
    afx_msg void OnFHTreeOpenDataBase();
    afx_msg void OnFHTreeDataBaseChangeName();
    afx_msg void OnFHTreeRemoveArea();
    afx_msg void OnFHTreeRemoveLine();
    afx_msg void OnFHTreeDrawLineData();
    afx_msg void OnFHTreeDrawLineMap();
    afx_msg void OnFHTreeDrawAreaMap();
    afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnFHTreeNewLineData();
    afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnFHTreeShowFullName();
    afx_msg void OnUpdateFHTreeShowFullName(CCmdUI* pCmdUI);
    afx_msg void OnFHTreeProperty();
    afx_msg void OnFHTreeShowSortedName();
    afx_msg void OnUpdateFHTreeShowSortedName(CCmdUI* pCmdUI);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnAddCurrentItemIntoBaseMap();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnFhtreeclosedatabase();
    virtual void OnInitialUpdate();
    afx_msg void OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FHTREEVIEW_H__650D9F81_CF19_11D3_AEF5_00C04FCCB957__INCLUDED_)
