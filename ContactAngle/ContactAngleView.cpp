// ContactAngleView.cpp : CContactAngleView 类的实现
//

#include "stdafx.h"
#include "ContactAngle.h"

#include "ContactAngleDoc.h"
#include "ContactAngleView.h"
#include "Curve.h"
#include "CuvFit.h"
#include "Result.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CImage	SaveCanvus,WorkCanvus,img2,img3,img4,img5,img6,img7,img8,WindowCanvus;
extern CImage*	pImg[10];
extern CString	m_FileName;
extern int		nDirectionNo,nSobelFlag,nTrackFlag;
extern double	DoubleTab[256];

// CContactAngleView

IMPLEMENT_DYNCREATE(CContactAngleView, CView)

BEGIN_MESSAGE_MAP(CContactAngleView, CView)
	ON_COMMAND(ID_GRAY_IMAGE, &CContactAngleView::OnGrayImage)
	ON_COMMAND(ID_ROBERTS_BORDDETEC, &CContactAngleView::OnRobertsBorddetec)
	ON_COMMAND(ID_ZOOM_IN, &CContactAngleView::OnZoomIn)
	ON_COMMAND(ID_ZOOM_OUT, &CContactAngleView::OnZoomOut)
	ON_COMMAND(ID_MEDIAN9, &CContactAngleView::OnMedian9)
	ON_COMMAND(ID_ENTROPIC_THRESHOLD, &CContactAngleView::OnEntropicThreshold)
	ON_COMMAND(ID_REMOVE_NOISE, &CContactAngleView::OnRemoveNoise)
	ON_COMMAND(ID_DILATION, &CContactAngleView::OnDilation)
	ON_COMMAND(ID_DEUTCH_THINNING, &CContactAngleView::OnDeutchThinning)
	ON_COMMAND(ID_CONTOUR_TRACKING_UP, &CContactAngleView::OnContourTrackingUp)
	ON_COMMAND(ID_Int_WND, &CContactAngleView::OnIntWnd)
	ON_COMMAND(ID_LINE_ADJUST, &CContactAngleView::OnLineAdjust)
	ON_COMMAND(ID_EROSION, &CContactAngleView::OnErosion)
	ON_COMMAND(ID_CURVE_FIT, &CContactAngleView::OnCurveFit)
	ON_COMMAND(ID_PRE_PROCESS, &CContactAngleView::OnPreProcess)
	ON_COMMAND(ID_LAPLACE_BORDDETEC, &CContactAngleView::OnLaplaceBorddetec)
	ON_COMMAND(ID_MOMENT_THRESHOLD, &CContactAngleView::OnMomentThreshold)
END_MESSAGE_MAP()

// CContactAngleView 构造/析构

CContactAngleView::CContactAngleView()
: max_n(0)
{
	// TODO: 在此处添加构造代码
	HDC		hMemDC;
 
	WindowCanvus.Create(1024,768,24,0);
  	hMemDC = WindowCanvus.GetDC();
 	WindowCanvus.BitBlt(hMemDC,0,0,1024,768,0,0,WHITENESS);
  	WindowCanvus.ReleaseDC();

 	for (int i=0;i<256;i++) DoubleTab[i]=1.0*i/255;

	m_nFlag=0;
	m_nSize=1;

}

CContactAngleView::~CContactAngleView()
{
}

BOOL CContactAngleView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CContactAngleView 绘制

void CContactAngleView::OnDraw(CDC* /*pDC*/)
{
	CContactAngleDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	if (SaveCanvus.IsNull()) return;
	ClearWindow(1);                              
  	WriteTitle(pImg[1],m_FileName,"");           
  	m_nFlag=0;
}


// CContactAngleView 诊断

#ifdef _DEBUG
void CContactAngleView::AssertValid() const
{
	CView::AssertValid();
}

void CContactAngleView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CContactAngleDoc* CContactAngleView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CContactAngleDoc)));
	return (CContactAngleDoc*)m_pDocument;
}
#endif //_DEBUG


// CContactAngleView 消息处理程序

void CContactAngleView::WriteTitle(CImage* pImg , CString FileName, CString str0)//显示带图像文件名与分辨率的标题行
{
	int		k;
	//int   i;
	CString	str, str1;
 	struct	IMAGEPARAMENT P;
	char drive[_MAX_DRIVE];   
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];   
	char ext[_MAX_EXT];
 
	_splitpath_s(FileName, drive, dir, fname, ext);  //分解文件名各部分
 
 	CWnd*	pMain = AfxGetMainWnd();//取得当前窗口句柄
	CMenu*	pMenu = pMain->GetMenu();//取得当前菜单句柄

	k=ImageType(pImg);                                 
	//for (i=0;i<5;i++)
	//	pMenu->CheckMenuItem(ID_BINARY_IMAGE+i,MF_UNCHECKED);
	//pMenu->CheckMenuItem(ID_BINARY_IMAGE+k,MF_CHECKED);   
 
	GetImageParament(pImg,&P);
	if (k==0) P.nBitCount=1;
	else if (k==1) P.nBitCount=4;
	str1.Format(" (%d×%d×%d)", P.nWidth, P.nHeight, P.nBitCount);  //生成分辨率格式
	str.LoadString(AFX_IDS_APP_TITLE);            //读入窗口标题
 	AfxGetMainWnd()->SetWindowText(str+" - "+fname + ext + str0 +str1);   //合并并显示新标题
}

void CContactAngleView::WorkCanvusToWindow(void)
{
	CClientDC	dc(this);
	struct IMAGEPARAMENT P;	
 
	if (SaveCanvus.IsNull()) return;
	GetImageParament(&WorkCanvus,&P);
 	WorkCanvus.BitBlt(dc,0,0,P.nWidth,P.nHeight,0,0,SRCCOPY);
}

void CContactAngleView::ClearWindow(int flag)
{
	struct		IMAGEPARAMENT P;
	CClientDC	dc(this);

	GetImageParament(pImg[1],&P);        
  	dc.SelectStockObject(WHITE_PEN);
	dc.SelectStockObject(WHITE_BRUSH);
	dc.Rectangle(P.nWidth,0,1023,639);
	dc.Rectangle(0,P.nHeight,P.nWidth,639);

	if (pImg[0]->IsNull()) return;
	if (flag)
		WorkCanvusToWindow();
}

void CContactAngleView::OnGrayImage()
{
	// TODO: 在此添加命令处理程序代码
	CString	str1;

	ColorToGray(pImg[0],pImg[0]);          

 	ImageCopy(pImg[1],pImg[0]);            
	str1.Format(" 的灰阶图像"); 
 	WriteTitle(pImg[1],m_FileName,str1);
   	WorkCanvusToWindow();
}

void CContactAngleView::OnMedian9()
{
	// TODO: 在此添加命令处理程序代码
	LocalProcess(pImg[1],1);
	WorkCanvusToWindow();
}


void CContactAngleView::OnRobertsBorddetec()
{
	// TODO: 在此添加命令处理程序代码
	LocalProcess(pImg[1],2);
	WorkCanvusToWindow();
}

void CContactAngleView::Zoom(int n)//缩放管理程序
{
	struct	IMAGEPARAMENT P;

	GetImageParament(pImg[0],&P);  //读取图像参数            
	if (m_nSize>0)                 //标志m_nSize为正表放大，为负表缩小
	{
		if (P.nWidth*m_nSize>2500) //放大时最大宽度不得超过2500像素
		{
			m_nSize--;//超限，对一个等级
			MessageBox("超出最大宽度2500Pixel");
			return;  //退出
		}
  	}
	else
	{
		if (P.nWidth/(-m_nSize)<50)  //缩小最小宽度不能小于50个像素
		{
			m_nSize++;//超限，升高等级
			MessageBox("小于最小宽度50Pixel");
			return;//退出
		}
  	}

	if (n==1) {//增大倍数
		m_nSize++;       //缩放倍数增大                        
		if (m_nSize==-1)//缩小倍数减小到-1时转放大
			m_nSize=1;
	}
	else {               //减小倍数
		m_nSize--;      //缩放倍数减小                         
		if (m_nSize==0) //放大倍数减小到0时缩小
			m_nSize=-2;
	}

	if (ImageType(pImg[0])<2)//二值及16色位图转换成索引图像后处理
		LowTypeToIndex(pImg[2],pImg[0]);
	else
		ImageCopy(pImg[2],pImg[0]);//其他类型位图复制到工作位图
 
	if (m_nSize>0)
 		ZoomIn(pImg[1],pImg[2],m_nSize);//整数倍放大
 	else
  		ZoomOut(pImg[1],pImg[2],-m_nSize);//整数倍缩小

	ClearWindow(1); //清屏                              
	WorkCanvusToWindow();//内存画布送入窗口   
}

void CContactAngleView::OnIntWnd()
{
	// TODO: 在此添加命令处理程序代码
	Invalidate();
}
void CContactAngleView::OnZoomIn()
{
	// TODO: 在此添加命令处理程序代码
	Zoom(1);
}

void CContactAngleView::OnZoomOut()
{
	// TODO: 在此添加命令处理程序代码
	Zoom(2);
}
void CContactAngleView::OnEntropicThreshold()
{
	// TODO: 在此添加命令处理程序代码
	CString	str1;
 
  	Threshold(pImg[1],1,0);
	str1.Format(" 的二值图像"); 
 	WriteTitle(pImg[1],m_FileName,str1);                 
	WorkCanvusToWindow(); 
}

void CContactAngleView::OnRemoveNoise()
{
	// TODO: 在此添加命令处理程序代码
	CurveThinning(pImg[1],pImg[0],1);
	WorkCanvusToWindow();
}


void CContactAngleView::MorphologicA(int n)
{
	struct	IMAGEPARAMENT P;

    ShowCursor(FALSE);
	GetImageParament(pImg[1],&P);
	if (P.nBitCount==1) {
		LowTypeToIndex(pImg[1],pImg[1]);
		ColorToGray(pImg[1],pImg[1]);
	}

 	Morphologic(pImg[2],pImg[1],n);
 	WorkCanvusToWindow();
    ShowCursor(TRUE);
}

void CContactAngleView::OnDilation()
{
	// TODO: 在此添加命令处理程序代码
 	MorphologicA(1);
}

void CContactAngleView::OnErosion()
{
	// TODO: 在此添加命令处理程序代码
	MorphologicA(2);
}




void CContactAngleView::OnDeutchThinning()
{
	// TODO: 在此添加命令处理程序代码
	CurveThinning(pImg[1],pImg[0],2);
	WorkCanvusToWindow();
}

void CContactAngleView::OnContourTrackingUp()
{
	// TODO: 在此添加命令处理程序代码
	CString	str1;
	ClearWindow(0);                        //清窗口
	CWnd*	pMain = AfxGetMainWnd();
	CDC*	pDC = pMain->GetDC();
 	max_n=ContourTracking(pImg[1],pDC);              //轮廓跟踪
	pMain->ReleaseDC(pDC);
 	SetDefaultPalette(pImg[1]);            //设置默认调色板
	str1.Format(" 的索引图像");            //修改文件标题
	WriteTitle(pImg[1],m_FileName,str1);   
    //WorkCanvusToWindow();                   //窗口刷新，显示结果
}



void CContactAngleView::OnLineAdjust()
{
	// TODO: 在此添加命令处理程序代码
	CString str1;
	int n;
	CCurve dlg;
	dlg.m_n=max_n;
	if(dlg.DoModal()==IDOK)
	{
		n=dlg.m_n;
	}
	else{
		MessageBox("未输入数据！");
	    return;
	}
	ClearWindow(0);
	LineAdjust(pImg[1],n);
	//SetDefaultPalette(pImg[1]);
	str1.Format("的测试图像");
	WriteTitle(pImg[1],m_FileName,str1);
	WorkCanvusToWindow(); 
}



void CContactAngleView::OnCurveFit()
{
	// TODO: 在此添加命令处理程序代码
	CCuvFit dlg;
	int n,m,s[2],q;
	double *x,*y,*a,*b,dt1,dt2,dt3;

	if(dlg.DoModal()==IDOK)
	{	
		n=dlg.m_n; /*整型变量，输入参数，给定数据点的个数*/
		m=dlg.m_m+1;/*整型变量，输入参数，拟合多项式的项数*/
		s[0]=dlg.m_left_pt;/*多项式系数*/
		s[1]=dlg.m_right_pt;
		q=dlg.m_space;
	}
	else{
		MessageBox("未获得参数！请重新设定");
		return;
	}
	
	/*分别为x,y,a分配存贮空间*/
	x = (double *)malloc(n*sizeof(double)); 
	y = (double *)malloc(n*sizeof(double));
	a = (double *)malloc(n*sizeof(double));  //拟合函数的多项式系数
	b = (double*)malloc(n*sizeof(double));
	
	Smooth(x,y,a,n,m,&dt1,&dt2,&dt3,s[0],q);        /*调用拟合函数*/
	Smooth(x,y,b,n,m,&dt1,&dt2,&dt3,s[1],q);
	
	double lfangle,rangle;

	lfangle=AngleResult(m,s[0],a);
	lfangle=atan(-lfangle);

	rangle=AngleResult(m,s[1],b);
	rangle=atan(rangle);
	double m_average_l;
	m_average_l=(lfangle+rangle)/2;
    m_average_l=180*m_average_l/PI;
	double k;
	k=AngleCorrect(s[0],s[1]);

	lfangle=180*(lfangle+k)/PI;//左边修正为加
	rangle=180*(rangle-k)/PI;//右边修正为减

	k=180*k/PI;
	/*CString str;
	str.Format(_T("%f"),k);
	str.Format(_T("%f"),lfangle);
	str.Format(_T("%f"),rangle);*/


	CResult Crsl;
	Crsl.m_left_anl=lfangle;
	Crsl.m_right_angle=rangle;
	Crsl.m_correct_angle=k;
	Crsl.m_average=m_average_l;
	Crsl.DoModal();

         free(x);                                                                       /*释放存储空间*/
         free(y);                                                                      
         free(a); 

}

void CContactAngleView::OnPreProcess()
{
	// TODO: 在此添加命令处理程序代码

	//灰度化
	CString	str1;
	ColorToGray(pImg[0],pImg[1]);          
 	ImageCopy(pImg[1],pImg[0]);
	//缩小
	Zoom(2);
    //中值滤波
	LocalProcess(pImg[1],1);
	// ROBERTS边缘检测
	LocalProcess(pImg[1],2);
	//二值化
  	Threshold(pImg[1],1,0);
	str1.Format(" 的二值图像"); 
 	WriteTitle(pImg[1],m_FileName,str1);                 
	WorkCanvusToWindow();
	Invalidate();
}

void CContactAngleView::OnLaplaceBorddetec()
{
	// TODO: 在此添加命令处理程序代码
	LocalProcess(pImg[1],3);
	WorkCanvusToWindow();
}

void CContactAngleView::OnMomentThreshold()
{
	// TODO: 在此添加命令处理程序代码
	CString	str1;
	Threshold(pImg[1],2,0);
	str1.Format(" 的二值图像"); 
	WriteTitle(pImg[1],m_FileName,str1);                 
	WorkCanvusToWindow();   
}
