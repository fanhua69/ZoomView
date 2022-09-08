#if !defined(AFX_FHEDITVIEW_H__7E576E43_10AE_11D4_A4E4_00C04FCCB957__INCLUDED_)
#define AFX_FHEDITVIEW_H__7E576E43_10AE_11D4_A4E4_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FHGridView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFHGridView view
#include "afxcmn.h"
#include "GridCtrl.h"
#include "FHEditDoc.h"
#include "PopupMenuManager.h"
#include <vector>
#include "PropertyPageManager.h"

class CFHEditViewHead
{
public:
    char    sFileMark[50];
    long    nRecordQty;
    long    nRecordLimit;
    long    nHeadSpace;

    CFHEditViewHead()
    {
        strcpy(sFileMark,"FHEDITVIEWHEAD");
        nRecordQty=0;
        nRecordLimit=100000;
        nHeadSpace=sizeof(long)*nRecordLimit+sizeof(CFHEditViewHead);
    };

};

class _FHZOOMVIEW_EXT_CLASS CFHEditView : public CView, public CPropertyPageManager
{
public:
    CFHEditView();           // protected constructor used by dynamic creation
    DECLARE_DYNCREATE(CFHEditView)
    CPopupMenuManager m_menuManager;

    // Form Data
public:
    CGridCtrl   m_Grid;
    long        m_nColNumber;
    bool        m_bInitView;

    GV_ITEM     m_Item;

    virtual void UpdateDoc();
    virtual void UpdateGrid();

protected:
    HACCEL              m_accel;
    CString             m_sFileHeadMark;
    CString             m_sFile;
    FILE                *m_fp;
    CFHEditViewHead     m_head;
    char                **m_psRecord;
    long                m_nRecordItemLimit;
    long                m_nItemCharLimit;

    long                m_nPageRecordQty;
    long                m_nTotalPage;

    long                m_nCurrentPage;
    long                m_nCurrentPageStartRecord;
    long                m_nCurrentPageEndRecord;

    int                 m_iCurrRow;
    int                 m_iCurrCol;

    virtual void    GridModified        (int iRow, int iCol);
    virtual void    ComboModified       (int iRow, int iCol, CString sNewString);

public:
    bool            SetPage             (long nPageOrder);
    bool            GotoPrevPage        ();
    bool            GotoNextPage        ();
    bool            UpdatePage          ();
    void            SetPageRecordQty    (long nRecordQty);

    long            GetCurrentRowPos    ();
    long            GetCurrentColPos    ();


    virtual void    PreSettingDlg       ();
    virtual void    PostSettingDlg      ();
    void            SetInvalidate       ();


    // Operations
public:
    CFHEditDoc* GetDocument();
    void SetItemBkColor(int nRow,int nCol,COLORREF nColor);
    void GetItem(int nRow,int nCol,float *pTarget);
    void GetItem(int nRow, int nCol, int  *pTarget);
    void GetBoolItem(int nRow, int nCol, BOOL  *pbTarget);
    void GetItem(int nRow,int nCol,long *pTarget);
    void GetItem(int nRow, int nCol, double *pTarget);
    void GetItem(int nRow,int nCol,CString &sTarget);
    void GetItem(int nRow, int nCol, char *sTarget);

    void SetItem(int nRow,int nCol,float nData,UINT nFormat=0,int nSmallNumberQty=-1);
    void SetItem(int nRow,int nCol,double nData,UINT nFormat=0,int nSmallNumberQty=-1);
    void SetItem(int nRow,int nCol,long nData,UINT nFormat=0);
    void SetBoolItem(int nRow, int nCol, BOOL bData);
    void SetItem(int nRow,int nCol,int  nData,UINT nFormat=0);
    void SetItem(int nRow,int nCol,CString nData,UINT nFormat=0);
    void SetItem(int nRow, int nCol, const char *sData,UINT nFormat);
    void SetEditable(int nRow,int nCol,bool bEditable);

    void SetBackColor(int nRed,int nGreen,int nBlue,bool bReDraw=true);
    void SetForeColor(int nRed,int nGreen,int nBlue,bool bReDraw=true);
    void SetBkColourCol(int nCol,COLORREF nColor);
    void SetBkColourRow(int nRow,COLORREF nColor);

    void SetSize(int nRow,int nCol);
    long GetValidRowCount();
    void MarkSameRowByCol(int nStartRow,int nEndRow,int nStartCol,int nEndCol,int &iCheckOrder, bool bOnlyAfter, bool bCreateProgressBar);


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CFHGridView)
protected:
    virtual void OnDraw(CDC* pDC);      // overridden to draw this view
    virtual void OnInitialUpdate();     // first time after construct
    virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) ;

    //}}AFX_VIRTUAL

protected:
    virtual void WriteRowOrder();
    virtual void GetMarkCol(int &nFC,int &nTC);

    void ClearSameMark();

public:
    bool CreateFile (CString sFile);
    bool OpenFile   (CString sFile);
    bool Over       ();

    bool AppendRecord(char **psRecords, long nItemQty);
    long GetRecordItemQty(long nRecordOrder);
    char ** GetRecord(long nRecordOrder, long &nItemQty);

    // Implementation
public:
    virtual ~CFHEditView();

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    // Generated message map functions
    //{{AFX_MSG(CFHEditView)
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    afx_msg void OnFHEditViewAddRow();
    afx_msg void OnFHEditViewAddCol();
    afx_msg void OnFHEditViewSetSize();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnFHEditViewDelRow();
    afx_msg void OnFHEditViewDelCol();
    afx_msg void OnFHEditViewInsRow();
    afx_msg void OnFHEditViewInsCol();
    afx_msg void OnFHEditViewUpdateGrid();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg LRESULT OnModified(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnComboModified(WPARAM wParam, LPARAM lParam);

    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnFheditviewpopupCopy();
    afx_msg void OnFheditviewpopupCut();
    afx_msg void OnFheditviewpopupPaste();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnFheditviewpopupProperty();
    afx_msg void OnFheditviewDelallserow();
    afx_msg void OnFheditviewDelallselcol();
    afx_msg void OnFheditviewrewriteorder();
    afx_msg void OnFheditviewDelallsamerow();
    afx_msg void OnFheditviewpopupBatchedit();
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnEditCopy();
    afx_msg void OnEditCut();
    afx_msg void OnEditPaste();
    afx_msg void OnFheditviewMarlallsamerowwithcols();
    afx_msg void OnFheditviewMarlallsamerowsoncol();
    afx_msg void OnEditviewRemovesamemark();
    afx_msg void OnSettings();

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FHGRIDVIEW_H__7E576E43_10AE_11D4_A4E4_00C04FCCB957__INCLUDED_)

