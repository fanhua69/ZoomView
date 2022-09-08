
#include "stdafx.h"
#include "FHZoomViewGlobal.h"
#include "FHApp.h"
#include "FHMainFrm.h"

void DisplayBitmap(CDC *pDC,CBitmap *pBitmap,int x,int y)
{
    if(!pDC||!pBitmap)return;

    BITMAP BM;
    CDC MemDC;
    MemDC.CreateCompatibleDC (NULL);
    MemDC.SelectObject (pBitmap);

    pBitmap->GetObject (sizeof(BM),&BM);

    pDC->BitBlt(x,y,BM.bmWidth ,BM.bmHeight ,&MemDC,0,0,SRCCOPY);
}

void Draw3DRect(CDC *pDC,CRect &rect, bool bRaised,UINT nColorRect)
{
    if(nColorRect==-1)nColorRect=GetSysColor(COLOR_ACTIVEBORDER);

    pDC->FillSolidRect (&rect,nColorRect);	

    // Draw the Top Line:
    if(bRaised){
        pDC->DrawEdge( rect,EDGE_RAISED,BF_RECT);
    }
    else{
        pDC->DrawEdge( rect,EDGE_SUNKEN,BF_RECT);
    }
}

CFHTreeView  * GetTree(CString sTreeName)
{
    CFHApp *pApp=(CFHApp*)AfxGetApp();
    CFHTreeView *pTree=(CFHTreeView *)pApp->GetTree (sTreeName);

    if(!pTree)
    {
        TRACEERROR("错误: 无法得到数据树："+sTreeName);
        return NULL;
    }

    return pTree;
}

bool CreateFrame(CDocument *pDocument,CMultiDocTemplate *pTemplate)
{
    if (pDocument == NULL||pTemplate==NULL)
    {
        TRACE(traceAppMsg, 0, "CDocTemplate::CreateNewDocument returned NULL.\n");
        AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
        return false;
    }
    ASSERT_VALID(pDocument);

    BOOL bAutoDelete = pDocument->m_bAutoDelete;
    pDocument->m_bAutoDelete = FALSE;   // don't destroy if something goes wrong
    CFrameWnd* pFrame =pTemplate-> CreateNewFrame(pDocument, NULL);
    pDocument->m_bAutoDelete = bAutoDelete;
    if (pFrame == NULL)
    {
        AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
        return false;
    }
    ASSERT_VALID(pFrame);

    pTemplate->InitialUpdateFrame(pFrame, pDocument, true);
    return true;
}