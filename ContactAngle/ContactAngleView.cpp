// ContactAngleView.cpp : CContactAngleView ���ʵ��
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

// CContactAngleView ����/����

CContactAngleView::CContactAngleView()
: max_n(0)
{
	// TODO: �ڴ˴���ӹ������
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
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CContactAngleView ����

void CContactAngleView::OnDraw(CDC* /*pDC*/)
{
	CContactAngleDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	if (SaveCanvus.IsNull()) return;
	ClearWindow(1);                              
  	WriteTitle(pImg[1],m_FileName,"");           
  	m_nFlag=0;
}


// CContactAngleView ���

#ifdef _DEBUG
void CContactAngleView::AssertValid() const
{
	CView::AssertValid();
}

void CContactAngleView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CContactAngleDoc* CContactAngleView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CContactAngleDoc)));
	return (CContactAngleDoc*)m_pDocument;
}
#endif //_DEBUG


// CContactAngleView ��Ϣ�������

void CContactAngleView::WriteTitle(CImage* pImg , CString FileName, CString str0)//��ʾ��ͼ���ļ�����ֱ��ʵı�����
{
	int		k;
	//int   i;
	CString	str, str1;
 	struct	IMAGEPARAMENT P;
	char drive[_MAX_DRIVE];   
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];   
	char ext[_MAX_EXT];
 
	_splitpath_s(FileName, drive, dir, fname, ext);  //�ֽ��ļ���������
 
 	CWnd*	pMain = AfxGetMainWnd();//ȡ�õ�ǰ���ھ��
	CMenu*	pMenu = pMain->GetMenu();//ȡ�õ�ǰ�˵����

	k=ImageType(pImg);                                 
	//for (i=0;i<5;i++)
	//	pMenu->CheckMenuItem(ID_BINARY_IMAGE+i,MF_UNCHECKED);
	//pMenu->CheckMenuItem(ID_BINARY_IMAGE+k,MF_CHECKED);   
 
	GetImageParament(pImg,&P);
	if (k==0) P.nBitCount=1;
	else if (k==1) P.nBitCount=4;
	str1.Format(" (%d��%d��%d)", P.nWidth, P.nHeight, P.nBitCount);  //���ɷֱ��ʸ�ʽ
	str.LoadString(AFX_IDS_APP_TITLE);            //���봰�ڱ���
 	AfxGetMainWnd()->SetWindowText(str+" - "+fname + ext + str0 +str1);   //�ϲ�����ʾ�±���
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
	// TODO: �ڴ���������������
	CString	str1;

	ColorToGray(pImg[0],pImg[0]);          

 	ImageCopy(pImg[1],pImg[0]);            
	str1.Format(" �Ļҽ�ͼ��"); 
 	WriteTitle(pImg[1],m_FileName,str1);
   	WorkCanvusToWindow();
}

void CContactAngleView::OnMedian9()
{
	// TODO: �ڴ���������������
	LocalProcess(pImg[1],1);
	WorkCanvusToWindow();
}


void CContactAngleView::OnRobertsBorddetec()
{
	// TODO: �ڴ���������������
	LocalProcess(pImg[1],2);
	WorkCanvusToWindow();
}

void CContactAngleView::Zoom(int n)//���Ź������
{
	struct	IMAGEPARAMENT P;

	GetImageParament(pImg[0],&P);  //��ȡͼ�����            
	if (m_nSize>0)                 //��־m_nSizeΪ����Ŵ�Ϊ������С
	{
		if (P.nWidth*m_nSize>2500) //�Ŵ�ʱ����Ȳ��ó���2500����
		{
			m_nSize--;//���ޣ���һ���ȼ�
			MessageBox("���������2500Pixel");
			return;  //�˳�
		}
  	}
	else
	{
		if (P.nWidth/(-m_nSize)<50)  //��С��С��Ȳ���С��50������
		{
			m_nSize++;//���ޣ����ߵȼ�
			MessageBox("С����С���50Pixel");
			return;//�˳�
		}
  	}

	if (n==1) {//������
		m_nSize++;       //���ű�������                        
		if (m_nSize==-1)//��С������С��-1ʱת�Ŵ�
			m_nSize=1;
	}
	else {               //��С����
		m_nSize--;      //���ű�����С                         
		if (m_nSize==0) //�Ŵ�����С��0ʱ��С
			m_nSize=-2;
	}

	if (ImageType(pImg[0])<2)//��ֵ��16ɫλͼת��������ͼ�����
		LowTypeToIndex(pImg[2],pImg[0]);
	else
		ImageCopy(pImg[2],pImg[0]);//��������λͼ���Ƶ�����λͼ
 
	if (m_nSize>0)
 		ZoomIn(pImg[1],pImg[2],m_nSize);//�������Ŵ�
 	else
  		ZoomOut(pImg[1],pImg[2],-m_nSize);//��������С

	ClearWindow(1); //����                              
	WorkCanvusToWindow();//�ڴ滭�����봰��   
}

void CContactAngleView::OnIntWnd()
{
	// TODO: �ڴ���������������
	Invalidate();
}
void CContactAngleView::OnZoomIn()
{
	// TODO: �ڴ���������������
	Zoom(1);
}

void CContactAngleView::OnZoomOut()
{
	// TODO: �ڴ���������������
	Zoom(2);
}
void CContactAngleView::OnEntropicThreshold()
{
	// TODO: �ڴ���������������
	CString	str1;
 
  	Threshold(pImg[1],1,0);
	str1.Format(" �Ķ�ֵͼ��"); 
 	WriteTitle(pImg[1],m_FileName,str1);                 
	WorkCanvusToWindow(); 
}

void CContactAngleView::OnRemoveNoise()
{
	// TODO: �ڴ���������������
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
	// TODO: �ڴ���������������
 	MorphologicA(1);
}

void CContactAngleView::OnErosion()
{
	// TODO: �ڴ���������������
	MorphologicA(2);
}




void CContactAngleView::OnDeutchThinning()
{
	// TODO: �ڴ���������������
	CurveThinning(pImg[1],pImg[0],2);
	WorkCanvusToWindow();
}

void CContactAngleView::OnContourTrackingUp()
{
	// TODO: �ڴ���������������
	CString	str1;
	ClearWindow(0);                        //�崰��
	CWnd*	pMain = AfxGetMainWnd();
	CDC*	pDC = pMain->GetDC();
 	max_n=ContourTracking(pImg[1],pDC);              //��������
	pMain->ReleaseDC(pDC);
 	SetDefaultPalette(pImg[1]);            //����Ĭ�ϵ�ɫ��
	str1.Format(" ������ͼ��");            //�޸��ļ�����
	WriteTitle(pImg[1],m_FileName,str1);   
    //WorkCanvusToWindow();                   //����ˢ�£���ʾ���
}



void CContactAngleView::OnLineAdjust()
{
	// TODO: �ڴ���������������
	CString str1;
	int n;
	CCurve dlg;
	dlg.m_n=max_n;
	if(dlg.DoModal()==IDOK)
	{
		n=dlg.m_n;
	}
	else{
		MessageBox("δ�������ݣ�");
	    return;
	}
	ClearWindow(0);
	LineAdjust(pImg[1],n);
	//SetDefaultPalette(pImg[1]);
	str1.Format("�Ĳ���ͼ��");
	WriteTitle(pImg[1],m_FileName,str1);
	WorkCanvusToWindow(); 
}



void CContactAngleView::OnCurveFit()
{
	// TODO: �ڴ���������������
	CCuvFit dlg;
	int n,m,s[2],q;
	double *x,*y,*a,*b,dt1,dt2,dt3;

	if(dlg.DoModal()==IDOK)
	{	
		n=dlg.m_n; /*���ͱ���������������������ݵ�ĸ���*/
		m=dlg.m_m+1;/*���ͱ����������������϶���ʽ������*/
		s[0]=dlg.m_left_pt;/*����ʽϵ��*/
		s[1]=dlg.m_right_pt;
		q=dlg.m_space;
	}
	else{
		MessageBox("δ��ò������������趨");
		return;
	}
	
	/*�ֱ�Ϊx,y,a��������ռ�*/
	x = (double *)malloc(n*sizeof(double)); 
	y = (double *)malloc(n*sizeof(double));
	a = (double *)malloc(n*sizeof(double));  //��Ϻ����Ķ���ʽϵ��
	b = (double*)malloc(n*sizeof(double));
	
	Smooth(x,y,a,n,m,&dt1,&dt2,&dt3,s[0],q);        /*������Ϻ���*/
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

	lfangle=180*(lfangle+k)/PI;//�������Ϊ��
	rangle=180*(rangle-k)/PI;//�ұ�����Ϊ��

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

         free(x);                                                                       /*�ͷŴ洢�ռ�*/
         free(y);                                                                      
         free(a); 

}

void CContactAngleView::OnPreProcess()
{
	// TODO: �ڴ���������������

	//�ҶȻ�
	CString	str1;
	ColorToGray(pImg[0],pImg[1]);          
 	ImageCopy(pImg[1],pImg[0]);
	//��С
	Zoom(2);
    //��ֵ�˲�
	LocalProcess(pImg[1],1);
	// ROBERTS��Ե���
	LocalProcess(pImg[1],2);
	//��ֵ��
  	Threshold(pImg[1],1,0);
	str1.Format(" �Ķ�ֵͼ��"); 
 	WriteTitle(pImg[1],m_FileName,str1);                 
	WorkCanvusToWindow();
	Invalidate();
}

void CContactAngleView::OnLaplaceBorddetec()
{
	// TODO: �ڴ���������������
	LocalProcess(pImg[1],3);
	WorkCanvusToWindow();
}

void CContactAngleView::OnMomentThreshold()
{
	// TODO: �ڴ���������������
	CString	str1;
	Threshold(pImg[1],2,0);
	str1.Format(" �Ķ�ֵͼ��"); 
	WriteTitle(pImg[1],m_FileName,str1);                 
	WorkCanvusToWindow();   
}
