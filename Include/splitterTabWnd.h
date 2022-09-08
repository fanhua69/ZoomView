/*
   FILNAME:       splitterTabWnd.h

   PURPOSE:       Manage view's that you can switch between by clicking at the tab.

   CREATED BY:    Per Ghosh
   ENVIRONMENT:	Visual C++


INFO:
*/

#ifndef __SPLITTAB_H_
#define __SPLITTAB_H_

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CWndTab;

/*
*************************************************
*************************************************
******                                     ******
****                                         ****
****    class CSplitterTabWnd                ****
****                                         ****
******                                     ******
*************************************************
*************************************************
*/

#include "zoomview.h"

class _FHZOOMVIEW_EXT_CLASS CSplitterTabWnd : public CSplitterWnd
{
   friend class CWndTab;
	DECLARE_DYNCREATE(CSplitterTabWnd)
public:
	CSplitterTabWnd();           
	virtual ~CSplitterTabWnd();

// Attributes
private:
   UINT         m_uSelectedViewIndex; // Index for current selected view

   CWndTab*     m_pwndTab;            // Tab window that can be used for switching views

   CPtrArray    m_ptrarrayRCView;     // Array for runtime classes for views
   CStringArray m_sarrayViewName;

// Get/Set
public:
   CWndTab* GetTab() { return m_pwndTab; }

// Operations
public:
	UINT AddView( CRuntimeClass* pRuntimeClass, LPCTSTR pszViewName );
	void InsertView( UINT uIndex, CRuntimeClass* pRCView, LPCTSTR pszViewName );
	void SelectView( UINT uIndex );
	void RemoveView( CRuntimeClass* pRuntimeClass );
	void RemoveView( UINT uIndex );
	BOOL Create( CWnd* pwndParent, CCreateContext* pContext );
   BOOL ReplaceView(CRuntimeClass* pViewClass);

// Overrides
public:
   virtual void RecalcLayout();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplitterTabWnd)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
	//{{AFX_MSG(CSplitterTabWnd)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/*
*************************************************
*************************************************
******                                     ******
****                                         ****
****    class CWndTab                        ****
****                                         ****
******                                     ******
*************************************************
*************************************************
*/

#define PG_WNDTAB_FONT                 0x00010000

class CWndTab : public CWnd
{
	DECLARE_DYNCREATE(CWndTab)

public:
	CWndTab();
	virtual ~CWndTab();

// Attributes
public:
   CSplitterTabWnd* m_psplitterTabWnd;    // pointer to parent (close relationship)

   DWORD            m_dwFlag;             // for various things

	UINT             m_uHeight;            // the height for tabs
   int              m_iOffsetToLeft;      // distance from left side to first tab
   UINT             m_uOffsetFontGap;     // gap between font boundary and tab
   BOOL             m_bTabModified;       // if tabs are modified

   CDWordArray      m_dwarrayTabWidth;    // the size of each tab
   CFont*           m_pFont;              // text font on the tabs

   static CString   m_stringWindowClass;

// Get/Set
public:
   UINT GetHeight() { return m_uHeight; }

   void SetHeight( UINT uHeight );
   void SetModified() { m_bTabModified = TRUE; }
   void SetSplitterTabWnd( CSplitterTabWnd* psplitterTabWnd ) 
           { m_psplitterTabWnd = psplitterTabWnd; }
   void SetFont( CFont* pFont ) { m_pFont = pFont; }

// Operations
public:
	void DrawTab( CDC* pDC );
	void DrawSelectedTab( CDC* pD );
	void UpdateTabWidth( CDC* pDC );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWndTab)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CWndTab)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void CreateFont();
};




#endif
