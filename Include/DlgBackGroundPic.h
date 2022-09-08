#pragma once


// CDlgBackGroundPic dialog

class CDlgBackGroundPic : public CPropertyPage
{
    DECLARE_DYNAMIC(CDlgBackGroundPic)

public:
    CDlgBackGroundPic();
    virtual ~CDlgBackGroundPic();

    // Dialog Data
    enum { IDD = IDD_dlgLoadBackGroundPic };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
public:
    CString         m_sFileName;
    double          m_dWestCoor;
    double          m_dEastCoor;
    double          m_dSouthCoor;
    double          m_dNorthCoor;
    BOOL            m_bKeepRatio;

    afx_msg void OnBnClickedbuttonfilename();
    virtual BOOL OnInitDialog();
    virtual BOOL OnApply();
    virtual void OnOK();
};
