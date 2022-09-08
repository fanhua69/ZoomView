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
    CFHTreeView();                         // �ܱ����Ķ�̬���캯��
    DECLARE_DYNCREATE(CFHTreeView)
    virtual ~CFHTreeView();

    // �ܱ��������ԣ�
protected:

    CPopupMenuManager m_menuManager;

    CString m_sDataBaseMark;               // ���ݿ��־
    CString m_sProjectDiscription;         // ���ݿ������ַ���
    CString m_sProjectExt;			       // ���ݿ���չ��

    // ��������ݿ�ı���:
    CString m_sWorkDir;				       // ����Ŀ¼
    CString m_sDataBaseName;			   // ���ݿ�����
    CString m_sDataBasePath;               // ���ݿ�·�� 
    CString m_sDataExt;                    // ���ݿ������������ļ�����չ�������磺�����ļ� *.h
    CString m_sDataDiscription;            // ���ݿ����������͵�����

    CString m_sRootName;                   // ����������
    CString m_sFirstLayerName;             // ��һ�������
    CString m_sSecondLayerName;            // �ڶ��������
    CMenuItem m_menuItems[20];             // �Ҽ��˵�������

    CImageList m_ImageList;				   // ��ʾ���ݿ����״̬��ͼƬ�б� 

    CLineProperty *m_pLinePropertyInArea;  // ���ڴ洢���ݿ��в��ߵ����ԵĽṹ��ָ��
    CString *m_pAreaNames;				   // ���ݿ����������Ĺ������б�

    CFHTreeView **m_pPointerToThis;        //  �ⲿָ�������ָ��ĵ�ַ�����ڸ�֪�ⲿ���������λ��

    bool m_bShowFullName;				   //  �Ƿ���ʾ���ݿ����ݵ�����·��
    bool m_bShowSortedName;				   //  �Ƿ�����ݿ����ݰ���������
    long m_nInsertOrder;				   //  ��¼���ݿ����ݲ���ĵ�ǰ˳��
    long m_nImageList;                     //  ������ͼƬ�ģɣĺš�  

    CString m_sAreaCurrent;				   //  ��ǰ��������
    CString m_sLineCurrent;				   //  ��ǰ��������

    CString m_sTreeName;
    bool    m_bTextFile;


public:
    CString m_sError;                     //  ��¼������Ϣ

    ////////////////////////////////////////////
    // �ܱ����ĳ�Ա����
protected:
    virtual bool    ReadDataBase            (); // ��ȡ���ݿ�, �Ǳ�OpenDataBase���õġ�
    void            Reset                   (); // ���ݿ⸴λ
    void            RemoveTreeData          (); // ɾ�����ݿ������в��ߵ�����
    bool            InsertLineNamesFromFile (CString sFile); // ���ļ�sFile�е��ļ�����Ϊ���߲��뵽���ݿ���
    void            DealSelChanged          (); // ����ǰѡ���Ĳ��߷����仯������
    void            SortTree                (); // ���ļ�����������
    bool            SaveDataBase            ();

    void            AddMenu                 (UINT MenuID, CString MenuString);

    //////////////////////////////////////
    // ��������
public:

    // �����ݿⴴ���йصģ�
    CString CreateDataBase(CString sPath);
    bool    MakeNewDataBase() ;                                     // ���������ݿ� 
    void    SetFileProperty(CString sDiscription,CString sExt);     // ���������ܹ����������ݿ���ļ�������
    void    SetDataBaseProperty(CString sDiscription,CString sExt); // �趨���ݿ������

    bool    ConstructAreaPath(char *cAreaPath,CString sArea);          // ���칤��·����һ��������ʽ
    CString ConstructDataBaseName();                                // �������ݿ�����
    CString ConstructAreaPath(CString sArea);                       // ���칤��·��
    CString ConstructLinePath(CString sArea, CString sLine);        // �������·��
    virtual bool OpenDataBase();                                    // �����ݿ⣬���� ReadDataBase
    virtual bool OpenDataBase(CString sPathName, bool bUpdateSolution=true);
    void    SetImageList(UINT nImageList);                          // ��������ͼƬλͼ
    CString GetDataBaseDiscription();
    CString GetDataBaseNameExt();
    void    ClearTree();
    int     GetChildQty(HTREEITEM item);


    virtual bool    InsertArea(     CString sArea);
    virtual CString InsertArea();                                       // �����ݿ��в���һ������
    virtual bool    InsertLine(    CString sFile);                      // ������ߡ�������ĳ�����²���������ߡ�����Ĳ���·����������д���ļ�sFile�С�

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

    virtual CString RemoveArea  ();	// ɾ��������������ɾ���Ĺ�������
    virtual bool    RemoveArea  (CString sArea);
    bool            RemoveLine  (CString &sArea,CString &sLine);             // ɾ������sArea�еĲ���sLine	


    //virtual bool SaveData();											 // �洢�������� 

    // ��������ʾ���Ҽ��˵��йص�
    void        SetLayerNames(CString sRootName,CString sFirstLayer,CString sSecondLayer);


    // �����ݿ��ȡ�йصģ�
    CString     GetDataBaseName     ();
    long        GetAreaNumber       ();                                       // ��õ�ǰ�򿪵����ݿ��ڵĹ�������Ŀ
    long        GetTotalLineNumber  ();									// ������ݿ����ܵĲ�����Ŀ
    CString     GetFileName         (CString sArea, CString sLine);

    long        GetLineNumberInArea (HTREEITEM ItemArea);               // ��õ�ǰ���ݿ��й���ItemArea�ڵĲ�����Ŀ
    long        GetLineNumberInArea (CString sArea);					// ��õ�ǰ���ݿ��й���sArea�ڵĲ�����Ŀ
    long        GetDataProperty     (CString sArea,CString sLine);          // ��ù���sArea�ڵĲ���sLine������
    long        GetSelectedLineProperty();								// ��õ�ǰѡ���Ĳ��ߵ�����

    CString *   GetAreaNames        (long &nAreaNumber);					// ������й���������
    CLineProperty* GetLineNamesInArea(CString sArea,long &nLineNumber);   // ��ù���sArea�����в��ߵ����ԣ�nLineNumber���ع����ڲ��ߵ���Ŀ 

    HTREEITEM   IsAreaInDataBase    (   CString sArea);					// �ù���sArea�Ƿ������ݿ���
    HTREEITEM   IsLineInDataBase    (   CString sArea,
                                        CString sLine, 
                                        UINT    nDataProperty);    // �ò���sLine(���ڹ���sArea)�Ƿ������ݿ���

    CString     ExistLineTypeInArea (CString sArea,long nDataType);	// ����sArea���Ƿ��������ΪnDataType�Ĳ��� 

    HTREEITEM   GetSelectedAreaItem ();                         // ��õ�ǰѡ���Ĺ�����ITEM
    HTREEITEM   GetSelectedLineItem ();                         // ��õ�ǰѡ���Ĳ��ߵ�ITEM
    CString     GetSelectedAreaName ();                         // ��õ�ǰѡ���Ĺ�����ITEM
    CString     GetSelectedLineName ();                         // ��õ�ǰѡ���Ĳ��ߵ�ITEM

    CString     SearchLineData              (   CString sArea,
                                                CString sLine,
                                                UINT nDataType);        // ��������sLine��λ�� 

    void        SetPointerToThis            (   CFHTreeView **pPointerInApp);			// �趨�����ָ��

    enumDimension GetDimension              (   CString sArea);
    virtual enumDimension GetFileDimension  (   CString sFile);

    bool        RemoveLineInDataBase        (   CString sArea, 
                                                CString sLine, 
                                                UINT nDataProperty);


    ////////////////////////////////////////////
    // �ܱ������麯��:
protected:
    virtual bool MakeNewLineData    ();                                                         // �½�һ���²�������
    virtual bool DrawLineData       (CString sArea,CString sFile);                              // ��ʵ��������
    virtual bool DrawLineMap        (CString sArea,CString sFile);                              // ���Ʋ���ͼ��
    virtual bool DrawAreaMap        (CString sArea,CLineProperty *pLines,long nLineNumber);     // ���ƹ���ͼ
    virtual bool Draw2DLineMap      (CString sArea, CString sLine);
    virtual bool Draw3DLineMap      (CString sArea, CString sLine);



    virtual bool UpdateDrawingArea( CString sArea,
                                    CLineProperty *pLine,
                                    long nLineNumber,
                                    CString sLineCurrent);                   // �ػ湤������

    virtual bool UpdateDrawingLine( CString sArea,
                                    CString sLineCurrent);                   // �ػ����ͼ��


    CMultiDocTemplate * m_pTemplate;  // ��ʱ����������ɾ��

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
