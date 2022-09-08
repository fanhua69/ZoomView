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
    int m_nLineNumber;                    // ������Ŀ
    CLineProperty *m_pLineProperty;       // ��������
    long *m_pLinePos;                     // ���������ڱ�m_pPoint�е�λ��
    long m_nNextLine;                     // ��ǰ���ߵ�����������׷�Ӳ���

    long m_nPointNumber;                  // �ܵ���
    long m_nPointStep;                    // ȡ��ļ����Ŀ�����û�������̫��ĵ�������5000����������ɸ���ȡһ����
    CXYZ *m_pPoint;                       // ���ڴ洢���еĵ�
    CXYZ *m_pPointInOneLine;              // ���ڴ洢һ���ߵĵ�

    long m_nPointLimit;

    bool SetMaxPointNumberInOneLine(int nLinePointNumber);		 // �����в����У�һ���������ĵ���

public:
    long                GetLineOrder    (long nPointOrder);



public:
    ///////////////////////////////////////
    // �����������йص���:
    ///////////////////////////////////////

    bool SetLineNumber                  (   long nLineNumber);      // ���ò�����Ŀ
    bool SetPointNumber                 (   long nPointNumber);     // �����ܵ���

    bool AddLine                        (   CLineProperty   LineProperty,
                                            CXYZ            *pPoint, 
                                            long            nPoint);  // ����һ������

    bool SetData                        (   CString sArea,CLineProperty *pLines,long nLineNumber);  // ���ò������� 

    // ����ڱ�ϵͳ�У�������������UpdateDrawingArea֧��
    // �������ڼ̳����У�ֻ��Ҫ���� SetLinePointNumber��SetLinePoints��������
    // ��һ������£�Ӧ��������������������
    virtual bool SetData                (   CString sDataFile);             // �����ļ���sDataFile���ò�������
    virtual long GetLinePointNumber     (   CString sArea, CString sLine);  // ���ò���sLine�ĵ���������GetFileLineNumber(),
    virtual long SetLinePoints          (   CString sArea,
                                            CString sLine,
                                            CXYZ *pPointSpace,
                                            long nPointLimit,
                                            bool *pb2DLine=NULL);           // ���ò��ߵĵ�.

    bool SetMaxPointNumberInOneLine     (   long nMaxLinePointNumber);


    /////////////////////////////////
    // ���ȡ�����йصĺ���:
    long            GetLineNumber       ();                                 // ��ò��ߵ���Ŀ
    long            GetTotalPointNumber ();                                 // ��õ������Ŀ
    CXYZ *          GetAllPoints        ();                                 // ������е�.

    CXYZ *          GetLine             (long nLine);                       // ���һ�����ߵĵ�,
    CXYZ *          GetLine             (CString sLine);                    // ���һ�����ߵĵ�,���ݲ�������.
    long            GetLinePointNumber  (long nLine);                       // ���һ���ߵĵ����Ŀ

    CString         GetLineName         (long nLine);                       // ��õ�nLine�����ߵ�����

    bool            GetNorthRange       (double &dMin, double &dMax);       // ��ù������ϱ�����ķ�Χ
    bool            GetEastRange        (double &dMin, double &dMax);       // ��ù����ڶ�������ķ�Χ
    bool            GetValueRange       (double &dMin, double &dMax);       // ��ù�������ֵ����ķ�Χ
    long            GetLineOrder        (CString sLineSearch);              // ���ݲ������ƻ�ò��ߵ���š�
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
