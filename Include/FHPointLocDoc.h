#if !defined(AFX_FHPOINTLOCDOC_H__2492BDDE_7F92_48B3_B36E_0D4398945B7C__INCLUDED_)
#define AFX_FHPOINTLOCDOC_H__2492BDDE_7F92_48B3_B36E_0D4398945B7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FHPointLocDoc.h : header file

#include "zoomview.h"
#include "XYZDoc.h"
#include "LineProperty.h"
#include "FHString.h"
//

/////////////////////////////////////////////////////////////////////////////
// CFHPointLocDoc document

class _FHZOOMVIEW_EXT_CLASS CFHPointLocDoc : public CFHEditDoc
{
    friend class CFHPointLocView;
protected:
    CFHPointLocDoc();           // protected constructor used by dynamic creation
    DECLARE_DYNCREATE(CFHPointLocDoc)

    // Attributes
protected:
    int m_nLineNumber;                    // 测线数目
    CLineProperty *m_pLineProperty;       // 测线属性
    long *m_pLinePos;                     // 测线数据在表m_pPoint中的位置
    long m_nNextLine;                     // 当前测线的数量，用于追加测线

    long m_nPointNumber;                  // 总点数
    long m_nPointStep;                    // 取点的间隔数目，若用户设置了太多的点数（〉5000），则隔若干个点取一个点
    CXYZ *m_pPoint;                       // 用于存储所有的点
    CXYZ *m_pPointInOneLine;              // 用于存储一条线的点

    long m_nPointLimit;

    bool SetMaxPointNumberInOneLine(int nLinePointNumber);		 // 在所有测线中，一条测线最大的点数

public:
    long                GetLineOrder    (long nPointOrder);



public:
    ///////////////////////////////////////
    // 与设置数据有关的类:
    ///////////////////////////////////////

    bool SetLineNumber                  (   long nLineNumber);      // 设置测线数目
    bool SetPointNumber                 (   long nPointNumber);     // 设置总点数

    bool AddLine                        (   CLineProperty   LineProperty,
                                            CXYZ            *pPoint, 
                                            long            nPoint);  // 加入一条测线

    bool SetData                        (   CString sArea,CLineProperty *pLines,long nLineNumber);  // 设置测线数据 

    // 如果在本系统中，在树视中须有UpdateDrawingArea支持
    // 这样，在继承类中，只需要重载 SetLinePointNumber和SetLinePoints两个函数
    // 在一般情况下，应该重载以下三个函数：
    virtual bool SetData                (   CString sDataFile);             // 根据文件名sDataFile设置测线数据
    virtual long GetLinePointNumber     (   CString sArea, CString sLine);  // 设置测线sLine的点数，调用GetFileLineNumber(),
    virtual long SetLinePoints          (   CString sArea,
                                            CString sLine,
                                            CXYZ *pPointSpace,
                                            long nPointLimit,
                                            bool *pb2DLine=NULL);           // 设置测线的点.

    bool SetMaxPointNumberInOneLine     (   long nMaxLinePointNumber);


    /////////////////////////////////
    // 与获取数据有关的函数:
    long            GetLineNumber       ();                                 // 获得测线的数目
    long            GetTotalPointNumber ();                                 // 获得点的总数目
    CXYZ *          GetAllPoints        ();                                 // 获得所有点.

    CXYZ *          GetLine             (long nLine);                       // 获得一条测线的点,
    CXYZ *          GetLine             (CString sLine);                    // 获得一条测线的点,根据测线名称.
    long            GetLinePointNumber  (long nLine);                       // 获得一条线的点的数目

    CString         GetLineName         (long nLine);                       // 获得第nLine条测线的名称

    bool            GetNorthRange       (double &dMin, double &dMax);       // 获得工区内南北坐标的范围
    bool            GetEastRange        (double &dMin, double &dMax);       // 获得工区内东西坐标的范围
    bool            GetValueRange       (double &dMin, double &dMax);       // 获得工区内数值坐标的范围
    long            GetLineOrder        (CString sLineSearch);              // 根据测线名称获得测线的序号。
    CLineProperty * GetLineProperty     (long nOrder);

    // Operations
public:

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CFHPointLocDoc)
public:
    virtual void Serialize(CArchive& ar);   // overridden for document i/o
protected:
    virtual BOOL OnNewDocument();
    //}}AFX_VIRTUAL

    // Implementation
public:
    virtual ~CFHPointLocDoc();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    // Generated message map functions
protected:
    //{{AFX_MSG(CFHPointLocDoc)
    // NOTE - the ClassWizard will add and remove member functions here.
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FHPOINTLOCDOC_H__2492BDDE_7F92_48B3_B36E_0D4398945B7C__INCLUDED_)
