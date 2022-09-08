#if !defined(AFX_COLORDLG_H__50E2A745_F9E3_11D3_AEF5_00C04FCCB957__INCLUDED_)
#define AFX_COLORDLG_H__50E2A745_F9E3_11D3_AEF5_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorDlg.h : header file

#include "zoomview.h"

#define COLOR_RWB   1
#define COLOR_RGB   2
#define COLOR_RED   3
#define COLOR_GREEN 4
#define COLOR_BLUE  5


//

/////////////////////////////////////////////////////////////////////////////
// CColorDlg dialog

class   _FHZOOMVIEW_EXT_CLASS  CColorDlg : public CDialog
{
// Construction
public:
    unsigned long GetMapLen(double dValue);
    void SetMaxLength(long nMaxLen);
    CColorDlg(CWnd* pParent = NULL);   // standard constructor
    ~CColorDlg();

    BOOL Create(CWnd *pParent=NULL);
    BOOL DestroyWindow(); 

    COLORREF GetColor(double dValue);
    COLORREF GetRevColor(double dValue);
    COLORREF GetRevColor(COLORREF nColor);
    void SetRange(double dMinValue,double dMaxValue);

// Dialog Data
    //{{AFX_DATA(CColorDlg)
    enum { IDD = IDD_DlgColorModal };
        // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CColorDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
public:
    void SetColorSerial(UINT nColor);
    double GetMinValue(){return m_dMinValue;};
    double GetMaxValue(){return m_dMaxValue;};

    static COLORREF Brighter	(COLORREF nColor,float fRate);// fRate>=0, fRate<=200;


protected:
    double      m_dMinValue;
    double      m_dMaxValue;
    double      m_dScale;
    double      m_dMapLenScale;
    int         m_nColorNumber;
    COLORREF    m_nColor[500];
    CFont       m_font;

    unsigned long m_nMaxLen;  // for showing the length of the map standing for the value;

    // Generated message map functions
    //{{AFX_MSG(CColorDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg void OnCancel();
    afx_msg void OnOK();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

protected:
    BOOL m_bParentDisabled;
    void ReEnableParent();


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORDLG_H__50E2A745_F9E3_11D3_AEF5_00C04FCCB957__INCLUDED_)
