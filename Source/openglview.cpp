// OGL_MFCView.cpp : implementation of the COpenGLView class
//

#include "stdafx.h"
#include "OpenGLView.h"
#include <limits>"
#include "FHMainFrm.h"
#include "math.h"
#include "fontgroup.h"
#include "..\include\openglview.h"
#include "fhapp.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COpenGLView

bool COpenGLView::m_bNumberBitmapLoaded =false;
char COpenGLView::m_cBitmapNumbers[12][8192];
char COpenGLView::m_nBitmapNumberWidth  =32;
char COpenGLView::m_nBitmapNumberHeight =64;


IMPLEMENT_DYNCREATE(COpenGLView, CFHZoomView)

BEGIN_MESSAGE_MAP(COpenGLView, CFHZoomView)
    //{{AFX_MSG_MAP(COpenGLView)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_DESTROY()
    ON_WM_KEYDOWN()
    //}}AFX_MSG_MAP
    // Standard printing commands
    //ON_COMMAND(ID_FILE_PRINT, CFHZoomView::OnFilePrint)
    //ON_COMMAND(ID_FILE_PRINT_DIRECT, CFHZoomView::OnFilePrint)
    //ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFHZoomView::OnFilePrintPreview)
    ON_WM_PAINT()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_CONTEXTMENU()
    ON_WM_KILLFOCUS()
    ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpenGLView construction/destruction

CTrackBall::CTrackBall()
{
    memset(m_RotArray,0,sizeof(double)*16);
    m_dRotAngle     =0.0;
    m_iViewWidth    =0;
    m_iViewHeight   =0;
}

void CTrackBall::Init()
{
    m_dRotAngle=0.0;
    glPushMatrix();// RotArray置为单位矩阵
    glLoadIdentity();
    glGetDoublev(GL_MODELVIEW_MATRIX,(GLdouble *)m_RotArray);

    glPopMatrix();
}

void CTrackBall::SetViewSize(int iWidth, int iHeight)
{
    m_iViewWidth=iWidth;
    m_iViewHeight=iHeight;
}


C3DVector CTrackBall::PointToVector(int x,int y)
{
    double dLen,dNorm;
    C3DVector vec;
    vec.m_a=(2.0*x-m_iViewWidth)/m_iViewWidth; //0 <=x <=1
    vec.m_b=(m_iViewHeight-2.0*y)/m_iViewHeight; //0 <=x <=1
    dLen=sqrt(vec.m_a*vec.m_a+vec.m_b*vec.m_b);
    vec.m_c=cos((3.14159265/2.0)*((dLen<1.0)?dLen:1.0));
    dNorm=sqrt(dLen*dLen+vec.m_c*vec.m_c);
    vec.Nomalize();
    return vec;
}


C3DVector CTrackBall::AxisAndAngle(int x,int y,bool bLButtonDown)
{
    // Calculate OP2'':
    C3DVector vecNewPos=PointToVector(x,y);
    if(bLButtonDown)
    {
        m_dRotAngle=0.0;
        m_vecOldPos=vecNewPos;
    }
    else
    {
        //
        C3DVector vecSubV=vecNewPos-m_vecOldPos;//OP2''-OP1''

        // Rotate Angle;
        m_dRotAngle=90.0*vecSubV.GetLength();    

        // 使用叉积求旋转轴矢量：
        m_vecRotAxis=m_vecOldPos.CrossMultiply(vecNewPos);
        m_vecOldPos=vecNewPos;
    }

    return m_vecOldPos;
}

void  CTrackBall::RotateMatrix(void)
{
    glPushMatrix();   //将当前矩阵压入矩阵堆栈保存
    glLoadIdentity();   //当前矩阵清为单位矩阵

    //计算由RotAxis和Angle定义旋转矩阵：
    glRotated(  m_dRotAngle,
                m_vecRotAxis.m_a,
                m_vecRotAxis.m_b,
                m_vecRotAxis.m_c);

    //将新产生的旋转矩阵与m_RotArray进行累加:
    glMultMatrixd((GLdouble *)m_RotArray);

    //将累加后的放置矩阵放回到变量m_RotArray中：
    glGetDoublev(GL_MODELVIEW_MATRIX,(GLdouble *)m_RotArray);

    //计算结束，将当前矩阵恢复为计算前的状态：
    glPopMatrix();

    //再施加累加的旋转量:
    glMultMatrixd((GLdouble*)m_RotArray);

}


COpenGLView::COpenGLView()
{
    m_hRC               =   NULL;

    m_dXMin             =   0;
    m_dXMax             =   1000;
    m_dYMin             =   0;
    m_dYMax             =   1000;
    m_dZMin             =   0;
    m_dZMax             =   1000;

    m_pointCenter.z     =   m_pointCenter.y     =   m_pointCenter.x    =0;

    m_pointMouseLast.x  =   -9999;
    m_pointMouseLast.y  =   -9999;

    m_dXShift           =   0;
    m_dYShift           =   0;
    m_dZShift           =   0;

    m_nAxisRadius       =   50;
    m_nAxisSliceQty     =   50; 
    m_nAxisHeadLen      =   m_nAxisRadius*8;

    m_iUpdateShowTime   =   20;
    GetPar("SPS_COpenGLView_m_iUpdateShowTime",m_iUpdateShowTime);

    int n;
    GetPar("SPS_COpenGLView_m_colorBackGround",n);m_colorBackGround=n;

    m_menuManager.Empty();
    m_menuManager.AppendMenu(ID_FHSettings,     "设置",         MF_MENUITEM,'E');
}

COpenGLView::~COpenGLView()
{
    unsigned int nColor=m_colorBackGround;
    AddPar("SPS_COpenGLView_m_colorBackGround"  ,nColor);
    AddPar("SPS_COpenGLView_m_iUpdateShowTime",m_iUpdateShowTime);
}

BOOL COpenGLView::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs

    return CFHZoomView::PreCreateWindow(cs);
}

void COpenGLView::OnDraw(CDC* pDC)
{
}

void COpenGLView::OnInitialUpdate()
{
    CFHZoomView::OnInitialUpdate();

    m_nTimer=SetTimer(1001,m_iUpdateShowTime,NULL);
}

/////////////////////////////////////////////////////////////////////////////
// COpenGLView diagnostics

#ifdef _DEBUG
void COpenGLView::AssertValid() const
{
    CFHZoomView::AssertValid();
}

void COpenGLView::Dump(CDumpContext& dc) const
{
    CFHZoomView::Dump(dc);
}

#endif //_DEBUG



void COpenGLView::InitScene()
{
    //m_colorBackGround
    float fRed=GetRValue(m_colorBackGround)/255.;
    float fGreen=GetGValue(m_colorBackGround)/255.;
    float fBlue=GetBValue(m_colorBackGround)/255.;

    glClearColor(fRed, fGreen, fBlue, 0.0f); //Background color

     //Activate lighting and a light source
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    //glShadeModel(GL_SMOOTH);
   // glPolygonMode(GL_BACK,GL_LINE);
    //glEnable(GL_POINT_SMOOTH); 
    //glEnable(GL_LINE_SMOOTH); 
    //glEnable(GL_POLYGON_SMOOTH); 
    //glEnable(GL_BLEND); 
    //glBlendFunc(GL_SRC_ALPHA,GL_ONE); 

    //glDepthFunc(GL_LESS); 

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear buffers
    glLoadIdentity(); // Load identity matrix

    glFrontFace(GL_CCW);
    //glEnable(GL_CULL_FACE);
    glEnable(GL_COLOR_MATERIAL);

    //glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,gray);
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
    glShadeModel(GL_SMOOTH);


    // Set material parameters:
    GLfloat glfMatAmbient[4]         = {0.200f, 0.200f, 0.200f, 1.0f};
    GLfloat glfMatDiffuse[4]         = {0.20000f, 0.2000f, 0.2000f, 1.0f};
    GLfloat glfMatAmbientBack[4]     = {0.001f, 0.001f, 0.001f, 1.0f};
    GLfloat glfMatDiffuseBack[4]     = {0.0002f, 0.0002f, 0.0002f, 1.0f};
    GLfloat glfMatSpecular[4]        = {0.200f, 0.200f, 0.200f, 0.0f};
    GLfloat glfMatEmission[4]        = {0.900f, 0.900f, 0.100f, 0.0f};
    GLfloat fShininess               = 128.000f;

   // glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  glfMatAmbient);
   // glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  glfMatDiffuse);
   // glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glfMatSpecular);
   // glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, glfMatEmission);
   // glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, fShininess);

    // Add a light source: This should be carefull:
    //GLfloat glfLight[]  ={ 0.0f,1.0f, 1.0f, 1.0f};
    //GLfloat amLight[]   ={ 0.8, 0.8,0.8,1.0f};
    //GLfloat difLight[]  ={ 0.000010000, 0.000010000,0.000010000,1.0f};
    //GLfloat speLight[]  ={ 0.5, 0.5,0.5,1.0f};
    
      GLfloat glfLight[]  ={ 0.0f,0.0f, -500.0f, 1.0f};
    GLfloat amLight[]   ={ 0.01, 0.01,0.01,1.0f};
    GLfloat difLight[]  ={ 0.01, 0.01,0.01,1.0f};
    GLfloat speLight[]  ={ 0.0, 0.0,0.0,1.0f};

  glLightfv(GL_LIGHT0, GL_POSITION, glfLight);
    glLightfv(GL_LIGHT0, GL_AMBIENT, amLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, difLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, speLight);
    glEnable(GL_LIGHT0);

    GLfloat glfLight1[]  ={ 1000.0f,0, -1000.0f, 1.0f};
    glLightfv(GL_LIGHT1, GL_POSITION, glfLight1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, amLight);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, difLight);
    glLightfv(GL_LIGHT1, GL_SPECULAR, speLight);
    glEnable(GL_LIGHT1);

    // Add a light source: This should be carefull:
    //GLfloat m_glfLight1[] = {0.5,1, 1, 0.0f};
    //GLfloat amLight1[]={ 0.0000, 0.0000,0.0000,1.0f};
    //GLfloat difLight1[]={ 0.00000, 0.00000,0.00000,0.0f};
    //GLfloat speLight1[]={ 0.0010001, 0.0010001,0.0010001,0.0f};

    //glLightfv(GL_LIGHT1, GL_POSITION, m_glfLight1);
    //glLightfv(GL_LIGHT1, GL_AMBIENT, amLight1);
    //glLightfv(GL_LIGHT1, GL_DIFFUSE, difLight1);
    //glLightfv(GL_LIGHT1, GL_SPECULAR, speLight1);
    //glEnable(GL_LIGHT1);

    //
    GLfloat f=1.0;
    glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE,&f);
}


void COpenGLView::KillScene()
{
}

void COpenGLView::OnDestroy() 
{
    CFHZoomView::OnDestroy();
    if(m_hRC==NULL)
    {
        return;
    }

    CClientDC dc(this);
    HDC hDC = dc.GetSafeHdc();
    wglMakeCurrent(hDC, m_hRC);
    KillScene();
    wglMakeCurrent(0, 0);
    wglDeleteContext(m_hRC);
}


//Rotate around x,z axis
void COpenGLView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    switch(nChar)
    {
    case VK_UP:
        Invalidate(0);
        break;
    case VK_DOWN:
        Invalidate(0);
        break;
    case VK_LEFT:
        Invalidate(0);
        break;
    case VK_RIGHT:
        Invalidate(0);
        break;
    default:
        break;
    }

    CFHZoomView::OnKeyDown(nChar, nRepCnt, nFlags);
}

/////////////////////////////////////////////////////////////////////////////
// COpenGLView message handlers
int COpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    if (CFHZoomView::OnCreate(lpCreateStruct) == -1)
    {
        return -1;
    }

    // Define pixel format
    PIXELFORMATDESCRIPTOR pfd;
    int nPixelFormat;
    memset(&pfd, NULL, sizeof(pfd));
    pfd.nSize      = sizeof(pfd);
    pfd.nVersion   = 1;
    pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 32;
    pfd.iLayerType = PFD_MAIN_PLANE;

    // Set pixel format
    CClientDC dc(this);
    HDC hDC = dc.GetSafeHdc();
    nPixelFormat = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, nPixelFormat, &pfd);


    //int n = ::GetPixelFormat( dc.GetSafeHdc());
    //::DescribePixelFormat( dc.GetSafeHdc(), n, sizeof( pfd ), & pfd );


    // Create RC--Rendering Context
    m_hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, m_hRC);

    //glDrawBuffer(GL_BACK);

    InitScene();

    //Create Font Display Lists
    Create2DTextLists(&dc);
    Create3DTextLists(&dc);

    //wglMakeCurrent(0, 0);


    m_clsTrackBall.Init();

    if(!m_bNumberBitmapLoaded)
    {
        LoadNumberBitmaps();
    }

    return 0;
}

void COpenGLView::OnSize(UINT nType, int cx, int cy) 
{
    CFHZoomView::OnSize(nType, cx, cy);

    CClientDC dc(this);
    if(cx==0 || cy ==0 )
    {
        return;
    }

    GLfloat fFovy   = 45.0f;    // Field-of-view
    GLfloat fZNear  = 0.1f;     // Near clipping plane
    GLfloat fZFar   = 400.0f;    // Far clipping plane

    HDC hDC = dc.GetSafeHdc();
    wglMakeCurrent(hDC, m_hRC);

    // Calculate viewport aspect
    RECT rv;
    GetClientRect(&rv);
    GLfloat fAspect = (GLfloat)(rv.right-rv.left) / (GLfloat)(rv.bottom-rv.top);

    // Define viewport:
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fFovy, fAspect, fZNear, fZFar);
    int nWidth=rv.right-rv.left;
    int nHeight=rv.bottom-rv.top;
    glViewport(rv.left, rv.top, nWidth, nHeight);

    glMatrixMode(GL_MODELVIEW);

    //wglMakeCurrent(0, 0);

    m_clsTrackBall.SetViewSize(cx,cy);
}

void COpenGLView::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    if(m_hRC==NULL)
    {
        return;
    }

    glDrawBuffer(GL_BACK);

    wglMakeCurrent(dc.GetSafeHdc(), m_hRC);

    InitScene();
    
    glTranslatef(m_dXShift,m_dYShift,m_dZShift);
    m_clsTrackBall.RotateMatrix();
    glScalef(m_dScale,m_dScale,m_dScale);
    glTranslatef(-m_pointCenter.x,-m_pointCenter.y,-m_pointCenter.z);

    m_vecCharStore.clear();

    OnDraw(&dc);

    PutStringsToView();


    glFinish();
    SwapBuffers( wglGetCurrentDC() );
}

void COpenGLView::SetDrawRange(double dXMin, 
                               double dYMin, 
                               double dZMin,
                               double dXMax,
                               double dYMax,
                               double dZMax)
{

    if(dXMin>dXMax)
    {
        ::Swap(dXMin,dXMax);
    }

    if(dYMin>dYMax)
    {
        ::Swap(dYMin,dYMax);
    }

    if(dZMin>dZMax)
    {
        ::Swap(dZMin,dZMax);
    }

    m_dXMin=dXMin;
    m_dXMax=dXMax;
    m_dYMin=dYMin;
    m_dYMax=dYMax;
    m_dZMin=dZMin;
    m_dZMax=dZMax;

    double dHalfZ=(m_dZMax-m_dZMin)/2;
    m_pointCenter.z=m_dZMin+dHalfZ;

    double dHalfY=(m_dYMax-m_dYMin)/2;
    m_pointCenter.y=m_dYMin+dHalfY;

    double dHalfX=(m_dXMax-m_dXMin)/2;
    m_pointCenter.x=m_dXMin+dHalfX;

    m_dScale=max(max(dHalfX,dHalfY),dHalfZ);
    m_dScale=1.0f/m_dScale;

    m_dZShift=-4;

    //
    m_nAxisRadius       =   1/m_dScale/100;
    m_nAxisHeadLen      =   m_nAxisRadius*8;


    GLfloat glfLight[]  ={ 1000000,0, 0, 0.0f};
    GLfloat amLight[]   ={ 0.1, 0.1,0.1,1.0f};
    GLfloat difLight[]  ={ 0.5, 0.5,0.5,1.0f};
    GLfloat speLight[]  ={ 0.0, 0.0,0.0,1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, glfLight);
    glLightfv(GL_LIGHT0, GL_AMBIENT, amLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, difLight);
   // glLightfv(GL_LIGHT0, GL_SPECULAR, speLight);
    glEnable(GL_LIGHT0);


    GLfloat glfLight1[]  ={ -1000000,0, 0, 0.0f};
    glLightfv(GL_LIGHT1, GL_POSITION, glfLight1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, amLight);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, difLight);
   // glLightfv(GL_LIGHT1, GL_SPECULAR, speLight);
    glEnable(GL_LIGHT1);
}


void COpenGLView::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if(nFlags&MK_CONTROL && nFlags&MK_SHIFT && nFlags&MK_LBUTTON )
    {
        if(m_pointMouseAnchor.x<0)
        {
            return;
        }
        else
        {
            if(point.y>m_pointMouseLast.y)
            {
                m_dZShift+=0.1; 
            }
            else
            {
                m_dZShift-=0.1; 
            }

            Invalidate(NULL);
        }
    }

    else if(nFlags & MK_CONTROL && nFlags&MK_LBUTTON)
    {
        m_dXShift+=0.01*(point.x-m_pointMouseLast.x);
        m_dYShift+=-0.01*(point.y-m_pointMouseLast.y);
        Invalidate(NULL);
    }

    // TODO: Add your message handler code here and/or call default
    else if(nFlags&MK_LBUTTON)
    {
        m_clsTrackBall.AxisAndAngle(point.x,point.y,(point==m_pointMouseAnchor));
        Invalidate(NULL);
    }

    m_pointMouseLast=point;

    CFHZoomView::OnMouseMove(nFlags, point);
}

void COpenGLView::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_pointMouseAnchor=point;

    KillTimer(m_nTimer);

    m_clsTrackBall.AxisAndAngle(point.x,point.y,true);

    CFHZoomView::OnLButtonDown(nFlags, point);
}

void COpenGLView::OnLButtonUp(UINT nFlags, CPoint point)
{
    m_pointMouseAnchor.x=m_pointMouseAnchor.y=-1;

    m_nTimer=SetTimer(1001,m_iUpdateShowTime,NULL);
    
    CFHZoomView::OnLButtonUp(nFlags, point);
}

void COpenGLView::OnTimer(UINT nIDEvent)
{
    Invalidate(false);
}


void COpenGLView::OnContextMenu(CWnd *pWnd, CPoint point)
{
    CMenu *pMenu=m_menuManager.GetPopupMenu();
    if(pMenu)
    {
        m_menuManager.ShowPopupMenu(pMenu,point,this);
    }
}

//生成一个三维字体的显示列表
void COpenGLView::Create3DTextLists(CDC *pDC)
{
	CFont m_font;
	GLYPHMETRICSFLOAT agmf[256]; 
	
	m_font.CreateFont(	-36,				// Height Of Font
			0,				// Width Of Font
			0,				// Angle Of Escapement
			0,				// Orientation Angle
			FW_BOLD,				// Font Weight
			FALSE,				// Italic
			FALSE,				// Underline
			FALSE,				// Strikeout
			ANSI_CHARSET,			// Character Set Identifier
			OUT_TT_PRECIS,			// Output Precision
			CLIP_DEFAULT_PRECIS,		// Clipping Precision
			ANTIALIASED_QUALITY,		// Output Quality
			FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
			"Arial");

	CFont* m_pOldFont = pDC->SelectObject(&m_font);

 	// create display lists for glyphs 0 through 255 with 0.1 extrusion 
	// and default deviation. The display list numbering starts at 1000 
	// (it could be any number) 
	m_3DTextList = glGenLists(256);
	wglUseFontOutlines(pDC->GetSafeHdc(), 
			 0, 255, m_3DTextList, 0.0f, 0.5f,WGL_FONT_POLYGONS, agmf); 
	 
	pDC->SelectObject(m_pOldFont);
}

void COpenGLView::Create2DTextLists(CDC *pDC)
{
	CFont m_font;
	
	m_font.CreateFont(	-12,				// Height Of Font
			0,				// Width Of Font
			0,				// Angle Of Escapement
			0,				// Orientation Angle
			FW_NORMAL,			// Font Weight
			FALSE,				// Italic
			FALSE,				// Underline
			FALSE,				// Strikeout
			ANSI_CHARSET,			// Character Set Identifier
			OUT_TT_PRECIS,			// Output Precision
			CLIP_DEFAULT_PRECIS,		// Clipping Precision
			DEFAULT_QUALITY,			// Output Quality
			FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
			"Algerian");

	CFont* m_pOldFont = pDC->SelectObject(&m_font);

 	// create display lists for glyphs 0 through 255 with 0.1 extrusion 
	// and default deviation. The display list numbering starts at 1000 
	// (it could be any number) 
	m_2DTextList = glGenLists(256);
	wglUseFontBitmaps(pDC->GetSafeHdc(), 0, 255, m_2DTextList); 
 
	pDC->SelectObject(m_pOldFont);
}

void COpenGLView::StoreOneChar( char        cNumber, 
                                CMy3DPoint  pointLeftDown, 
                                CMy3DPoint  pointLeftUp,
                                C3DVector   vecExtension)
{
    int nOrder=0;
    if(cNumber==45)
    {
        nOrder=11;
    }
    else if(cNumber==46)
    {
        nOrder=10;
    }
    else
    {
        nOrder=cNumber-48;
    }

    m_vecCharStore.push_back(   CCharShowProperty(  nOrder,
                                                    pointLeftDown, 
                                                    pointLeftUp,
                                                    vecExtension));
}


bool COpenGLView::PutStringsToView()
{
    //映射纹理   
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);   

    glTexParameterf (GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);   
    glTexParameterf (GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);   
    glTexParameterf (GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);   
    glTexParameterf (GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);   
    glTexEnvf       (GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);   
    glEnable        (GL_TEXTURE_2D);   
    glEnable        (GL_BLEND);     
    glEnable        (GL_ALPHA_TEST);     
    glAlphaFunc     (GL_GREATER   ,0.9);//0.5可以换成任何在0~1之间的数     

    glPixelStorei   (GL_UNPACK_ALIGNMENT,1);   

    COLORREF color  =   RGB(255,255,255);
    char cRed       =   GetRValue(color);
    char cGreen     =   GetGValue(color);
    char cBlue      =   GetBValue(color);

    glColor4ub(cRed,cGreen,cBlue,255);

    int iChar=0,i=0;
    int nCharQty=m_vecCharStore.size();

    for(iChar=0;iChar<12;iChar++)
    {
        glTexImage2D(       GL_TEXTURE_2D,
                            0,
                            GL_RGBA, 
                            m_nBitmapNumberWidth, // /2
                            m_nBitmapNumberHeight, 
                            0,
                            GL_BGRA_EXT,
                            GL_UNSIGNED_BYTE,
                            &m_cBitmapNumbers[iChar]);

        gluBuild2DMipmaps(  GL_TEXTURE_2D,
                            4,
                            m_nBitmapNumberWidth,  // /2
                            m_nBitmapNumberHeight,
                            GL_BGRA_EXT,
                            GL_UNSIGNED_BYTE,
                            &m_cBitmapNumbers[iChar]);  

        for(i=0;i<nCharQty;i++)
        {
            CCharShowProperty &ch=m_vecCharStore.at(i);
            if(ch.m_nCharOrder!=iChar)
            {
                continue;
            }
            glBegin(GL_QUADS);   

            glTexCoord2d(0,0);   
            glVertex3d( ch.m_pointLeftDown.x,
                        ch.m_pointLeftDown.y,
                        ch.m_pointLeftDown.z);   

            glTexCoord2d(1,0);  
            glVertex3d( ch.m_pointLeftDown.x+ch.m_vecExtension.m_a,
                        ch.m_pointLeftDown.y+ch.m_vecExtension.m_b,
                        ch.m_pointLeftDown.z+ch.m_vecExtension.m_c);   

            glTexCoord2d(1,1);     
            glVertex3d( ch.m_pointLeftUp.x+ch.m_vecExtension.m_a,
                        ch.m_pointLeftUp.y+ch.m_vecExtension.m_b,
                        ch.m_pointLeftUp.z+ch.m_vecExtension.m_c);   

            glTexCoord2d(0,1);     
            glVertex3d( ch.m_pointLeftUp.x,
                        ch.m_pointLeftUp.y,
                        ch.m_pointLeftUp.z);   

            glEnd(); 
        }
    }

    glDisable(GL_TEXTURE_2D);   
    glDisable(GL_ALPHA_TEST);   
    glDisable(GL_BLEND);

    m_vecCharStore.clear();

    return true;

}

void COpenGLView::WriteString(CString       sString, 
                              CMy3DPoint    pointLeftDown, 
                              CMy3DPoint    pointLeftUp,
                              C3DVector     vecExtension)
{
    CMy3DPoint  pointLeftDownCurr   =   pointLeftDown;
    CMy3DPoint  pointLeftUpCurr     =   pointLeftUp;

    int i=0;
    char c;
    for(i=0;i<sString.GetLength();i++)
    {
        c=sString.GetAt(i);

        StoreOneChar( c, 
                        pointLeftUpCurr,
                        pointLeftDownCurr, 
                        vecExtension);
       
        pointLeftDownCurr   +=vecExtension;
        pointLeftUpCurr     +=vecExtension;
    }
}

void COpenGLView::LoadOneNumberBitmap(unsigned int nBitmapID, char *pBitMem)
{
    CBitmap bitmap;
    BITMAP bitmapStructure;
    int nWidth,nHeight;
    int nBits;

    bitmap.LoadBitmap(nBitmapID);
    nBits=bitmap.GetBitmapBits(8192,pBitMem);
    bitmap.GetBitmap(&bitmapStructure);
    
    nWidth                  =   bitmapStructure.bmWidth;
    nHeight                 =   bitmapStructure.bmHeight;

    m_nBitmapNumberWidth    =   nWidth;
    m_nBitmapNumberHeight   =   nHeight;

    int i,j;
    BYTE r,g,b;
    for(i=0;i<nHeight;i++)   
    {
        for(j=0;j<nWidth;j++)   
        {   
            //颜色值:
            b   =   (GLubyte)*(pBitMem+i*nWidth*4+j*4);   
            g   =   (GLubyte)*(pBitMem+i*nWidth*4+j*4+1);   
            r   =   (GLubyte)*(pBitMem+i*nWidth*4+j*4+2);   
            
            //设置alpha值,假设黑色为透明色   
            if(     r==0   
                &&  g==0   
                &&  b==0)
            {
                *(pBitMem+i*nWidth*4+j*4+3)   =   0;//透明，设为   0   
            }
            else
            {
                *(pBitMem+i*nWidth*4+j*4+3)   =   255;//不透明，设为   255,   也就是以后用的1.0   
            }
        }   
    }

}

void COpenGLView::LoadNumberBitmaps()
{
    LoadOneNumberBitmap(IDB_Number_0, (char*)(m_cBitmapNumbers[0]));
    LoadOneNumberBitmap(IDB_Number_1, (char*)(m_cBitmapNumbers[1]));
    LoadOneNumberBitmap(IDB_Number_2, (char*)(m_cBitmapNumbers[2]));
    LoadOneNumberBitmap(IDB_Number_3, (char*)(m_cBitmapNumbers[3]));
    LoadOneNumberBitmap(IDB_Number_4, (char*)(m_cBitmapNumbers[4]));
    LoadOneNumberBitmap(IDB_Number_5, (char*)(m_cBitmapNumbers[5]));
    LoadOneNumberBitmap(IDB_Number_6, (char*)(m_cBitmapNumbers[6]));
    LoadOneNumberBitmap(IDB_Number_7, (char*)(m_cBitmapNumbers[7]));
    LoadOneNumberBitmap(IDB_Number_8, (char*)(m_cBitmapNumbers[8]));
    LoadOneNumberBitmap(IDB_Number_9, (char*)(m_cBitmapNumbers[9]));
    LoadOneNumberBitmap(IDB_Number_Point, (char*)(m_cBitmapNumbers[10]));
    LoadOneNumberBitmap(IDB_Number_Minus, (char*)(m_cBitmapNumbers[11]));
}


void COpenGLView::DrawCoor(CDC *pDC)
{
    GLUquadricObj *pObj=NULL;

    CFontGroup font;
    double dValue,dEdgeLeft,dEdgeRight,dValueStep;
    CString sValue;

    int iCharHeightX    =   (m_dXMax-m_dXMin)/40;
    int iCharHeightY    =   (m_dYMax-m_dYMin)/40;
    int iCharHeightZ    =   (m_dZMax-m_dZMin)/40;
    int iCharHeight     =   __max(__max(iCharHeightX,iCharHeightY),iCharHeightZ);
    int iCharWidth      =   iCharHeight/2;
    int iMig            =   0;

    GLfloat glfMatEmission[4] = {0.100f, 0.100f, 0.100f, 0.0f};

    //glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, glfMatEmission);

    pObj    =   gluNewQuadric();
    gluQuadricNormals(pObj,GLU_SMOOTH);
    gluQuadricDrawStyle(pObj,GLU_FILL);

    glPushMatrix();

        // Z 轴：
        glPushMatrix();
           
            glTranslatef(m_dXMin,m_dYMin,m_dZMin);

            // 原点:
            glColor4ub(255,255,255,255);
            gluSphere(pObj,m_nAxisRadius*2,m_nAxisSliceQty*2,m_nAxisSliceQty*2);
            
            // Z Axis:
            glColor4ub(0,0,255,255);
            gluCylinder(pObj,   m_nAxisRadius,
                                m_nAxisRadius,
                                m_dZMax-m_dZMin+m_nAxisHeadLen*2,
                                m_nAxisSliceQty, 
                                1);

            glTranslatef(0,0,m_dZMax-m_dZMin+2*m_nAxisHeadLen);
            gluCylinder(pObj,   m_nAxisRadius*2,
                                0,
                                m_nAxisHeadLen,
                                m_nAxisSliceQty, 
                                1);
        glPopMatrix();

        // The marks:
        font.Set(m_dZMin,m_dZMax,m_dZMax-m_dZMin,iCharHeight);
        font.CalcFontGroup (dEdgeLeft,dEdgeRight,dValueStep);

        for(dValue=dEdgeLeft;dValue<=dEdgeRight;dValue+=dValueStep)
        {
            if(dValue<m_dZMin || dValue>m_dZMax)
            {
                continue;
            }
            sValue=vtos(dValue,0);
            iMig=iCharWidth*(sValue.GetLength()+1);
            WriteString     (   sValue, 
                                CMy3DPoint  (m_dXMin-iMig,m_dYMin-iMig,dValue-iCharHeight/2), 
                                CMy3DPoint  (m_dXMin-iMig,m_dYMin-iMig,dValue+iCharHeight/2), 
                                C3DVector   (iCharWidth,iCharWidth,0));
        }


        // X轴:
        // The marks:
        font.Set(m_dXMin,m_dXMax,m_dXMax-m_dXMin,iCharHeight);
        font.CalcFontGroup (dEdgeLeft,dEdgeRight,dValueStep);

        for(dValue=dEdgeLeft;dValue<=dEdgeRight;dValue+=dValueStep)
        {
            if(dValue<m_dXMin || dValue>m_dXMax)
            {
                continue;
            }
            sValue=vtos(dValue,0);
            iMig=iCharWidth*(sValue.GetLength()+1);
            WriteString     (   sValue, 
                                CMy3DPoint  (dValue-iCharHeight/2,m_dYMin-iCharWidth,m_dZMin), 
                                CMy3DPoint  (dValue+iCharHeight/2,m_dYMin-iCharWidth,m_dZMin), 
                                C3DVector   (0,-iCharWidth,0));
        }

        glPushMatrix();
            glTranslatef(m_dXMin,m_dYMin,m_dZMin);

            glColor4ub(255,0,0,255);
            glRotated(90,0,1,0);
            gluCylinder(pObj,   m_nAxisRadius,
                                m_nAxisRadius,
                                m_dXMax-m_dXMin,
                                m_nAxisSliceQty, 
                                1);

            glTranslatef(0,0,m_dXMax-m_dXMin);
            gluCylinder(pObj,   m_nAxisRadius*2,
                                0,
                                m_nAxisHeadLen,
                                m_nAxisSliceQty, 
                                1);

        glPopMatrix();

        // Y轴：
        // The marks:
        font.Set(m_dYMin,m_dYMax,m_dYMax-m_dYMin,iCharHeight);
        font.CalcFontGroup (dEdgeLeft,dEdgeRight,dValueStep);

        for(dValue=dEdgeLeft;dValue<=dEdgeRight;dValue+=dValueStep)
        {
            if(dValue<m_dYMin || dValue>m_dYMax)
            {
                continue;
            }
            sValue=vtos(dValue,0);
            iMig=iCharWidth*(sValue.GetLength()+1);


            WriteString     (   sValue, 
                                CMy3DPoint  (m_dXMin-iMig,dValue-iCharHeight/2,m_dZMin), 
                                CMy3DPoint  (m_dXMin-iMig,dValue+iCharHeight/2,m_dZMin), 
                                C3DVector   (+iCharWidth,0,0));
        }

        
        // Draw the axis:
        glPushMatrix();

            glTranslatef(m_dXMin,m_dYMin,m_dZMin);

            glColor4ub(0,255,0,255);
            glRotated(-90,1,0,0);
            gluCylinder(pObj,   m_nAxisRadius,
                                m_nAxisRadius,
                                m_dYMax-m_dYMin,
                                m_nAxisSliceQty, 
                                1);

            glTranslatef(0,0,m_dYMax-m_dYMin);
            gluCylinder(pObj,   m_nAxisRadius*2,
                                0,
                                m_nAxisHeadLen,
                                m_nAxisSliceQty, 
                                1);

        glPopMatrix();

    glPopMatrix();
}

void  COpenGLView::PreSettingDlg()
{
    // Do not call CFHZoomView's dialogs, because they are not suitable to this 3D View.
    // fanhua
}

void  COpenGLView::PostSettingDlg()
{
    // Do not call CFHZoomView's dialogs, because they are not suitable to this 3D view.
    // fanhua

}
void COpenGLView::OnKillFocus(CWnd* pNewWnd)
{
    CFHZoomView::OnKillFocus(pNewWnd);
    KillTimer(m_nTimer);
}

void COpenGLView::OnSetFocus(CWnd* pOldWnd)
{
    CFHZoomView::OnSetFocus(pOldWnd);

    m_nTimer=SetTimer(1001,m_iUpdateShowTime,NULL);

}
