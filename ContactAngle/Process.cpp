#include "stdafx.h"

int ImageType(CImage *pImgm) //位图类型
{
	RGBQUAD	ColorTab[256];
 	int		k;

	if (pImgm->IsNull()) return(0);

	switch(pImgm->GetBPP()) //由像素总位数判断位图类型
	{
		case 1:  k=0;	break; //二值图像
		case 4:  k=1;	break; //16色图像
		case 8:  k=3;	break; //索引图像（包括灰阶图像）
		default: k=4;	break; //真彩色图像
 	}

	if (k==3)                  ///由索引图像识别出4种类别
	{
 		pImgm->GetColorTable(0,256,ColorTab);//取调色板数据
		k=PaletteType(ColorTab);//判别调色板类型
	}
	return(k);//返回位图类型
}

int   PaletteType(RGBQUAD  *ColorTab)//调色板类型
{
 	int		i,k,m,n,r,g,b;
 
	m=n=0;//参数初始化
	for (i=0; i<256; i++)//取得各颜色分量
	{
		r = ColorTab[i].rgbRed;
		g = ColorTab[i].rgbGreen;
		b = ColorTab[i].rgbBlue;
		if ((r != g)||(r != b)) m=0;//3分量不同
		if ((i>0)&&(r>ColorTab[i-1].rgbRed)) m++;//与前一单元比较
		if (r+g+b==0) n++;//颜色分来那个非零单元计数
	}
	k=3;
 	if (m == 255) k=2;   //颜色分量单调递增为灰阶图像
	else  if (256-n==1) k=0;   //有一个非0调色板单元为二值图像
	else  if (256-n==15) k=1;  //有15个非0调色板单元为16色图像   
	else k=3;//否则，为索引图像
 	return(k); //返回调色板类型
}

void LowTypeToIndex(CImage *pImgn,CImage *pImgm)  
{                                 
	int			i,k,t,y,nBytesPerLine;
	RGBQUAD		ColorTab[256];
	CImage		gpImg;
 	BYTE		*cbuf,*gbuf;
 	struct		IMAGEPARAMENT P;
 
 	GetImageParament(pImgm,&P);              
	if (P.nBitCount>4) return;

 	gpImg.Create(P.nWidth,P.nHeight,8,0);      
    memset(ColorTab,0,1024);                    
 	pImgm->GetColorTable(0,P.nNumColors,ColorTab);  
	gpImg.SetColorTable(0,256,ColorTab);          

	nBytesPerLine = (P.nWidth*P.nBitCount+7)/8;
  	if (P.nBitCount==1)                               
	{
		for (y=0;y<P.nHeight;y++) 
		{
			cbuf = (BYTE*) pImgm->GetPixelAddress(0,y);    
			gbuf = (BYTE*) gpImg.GetPixelAddress(0,y);
			for (i=0;i<nBytesPerLine;i++) 
			{
				for (k=0,t=0x80;k<8;k++)         
				{
					if (cbuf[i]&t)
						gbuf[8*i+k]=1;
					else
						gbuf[8*i+k]=0;
					t = t>>1;
				}
			}
  		}
	} 
	else if (P.nBitCount==4)                          
	{
		for (y=0; y<P.nHeight; y++) 
		{
			cbuf = (BYTE*) pImgm->GetPixelAddress(0,y);    
			gbuf = (BYTE*) gpImg.GetPixelAddress(0,y);
			for (i=0; i<nBytesPerLine; i++) 
			{
				gbuf[2*i+1]= cbuf[i] & 0xf;      
				gbuf[2*i]  = cbuf[i]>>4;
			}
		}
	}
	if (pImgm==pImgn)                                    
 		ImageCopy(pImgm,&gpImg);
 	else                                        
 		ImageCopy(pImgn,&gpImg);
 	gpImg.Destroy();
}

void GetImageParament(CImage *pImg,struct IMAGEPARAMENT *ppImgParam)
{
	if (pImg->IsNull()) return;

	ppImgParam->nWidth   = pImg->GetWidth();
	ppImgParam->nHeight   = pImg->GetHeight();
	ppImgParam->nBitCount  = pImg->GetBPP();
	ppImgParam->nBytesPerLine   = (pImg->GetWidth()*pImg->GetBPP()+31)/32*4;
	ppImgParam->nBytesPerPixel   = pImg->GetBPP()/8;
	if (pImg->GetBPP()<=8) 
		ppImgParam->nNumColors= 1 << pImg->GetBPP();
	else 
		ppImgParam->nNumColors= 0;
	ppImgParam->nSize  = ppImgParam->nBytesPerLine*ppImgParam->nHeight;
}

void ImageCopy(CImage *pImgn,CImage *pImgm)  //位图复制
{
 	struct	IMAGEPARAMENT P;
 	RGBQUAD	ColorTab[256];
 	CDC		*pOrigDC,*pDC;
  
	GetImageParament(pImgm,&P); //获取原始位图参数
 
	if (!pImgn->IsNull())       //目标位图非空
		pImgn->Destroy();      //删除目标位图
	pImgn->Create(P.nWidth,P.nHeight,P.nBitCount,0);//建立新位图                        

	if (P.nNumColors>0)      //原始位图有调色板数据
	{
 		pImgm->GetColorTable(0,P.nNumColors,ColorTab);
		pImgn->SetColorTable(0,P.nNumColors,ColorTab);//复制调色板数据   
	}

 	pOrigDC= CDC::FromHandle(pImgm->GetDC());//建立源位图CDC
 	pDC    = CDC::FromHandle(pImgn->GetDC());//建立目标位图CDC
 	pDC->BitBlt(0,0,P.nWidth,P.nHeight,pOrigDC,0,0,SRCCOPY);//复制像素数据
 	pImgm->ReleaseDC();//释放CDC指针
 	pImgn->ReleaseDC();
}

//////灰阶图像处理//////////////////////////
void ColorToGray(CImage *pImgn,CImage *pImgm) 
{
 	int			i, x, y;
	int			gray[256];
	BYTE		*cbuf,*gbuf;
	RGBQUAD		ColorTab[256];
 	CImage		gpImg;
 	CString		str1;
 	struct		IMAGEPARAMENT P;

	GetImageParament(pImgm,&P);             
	if (P.nBitCount<8) return;
 
 	gpImg.Create(P.nWidth,P.nHeight,8,0);   
	for (i=0; i<256; i++)
	{
		ColorTab[i].rgbBlue = ColorTab[i].rgbGreen = 
			                  ColorTab[i].rgbRed   = i;
	}
	gpImg.SetColorTable(0,256,ColorTab);    

	if (pImgm->GetBPP()<=8) {               
 		pImgm->GetColorTable(0,P.nNumColors,ColorTab);
		for (i=0; i<P.nNumColors; i++)
		{
			gray[i] = (int) (0.11*ColorTab[i].rgbBlue + 
				     0.59*ColorTab[i].rgbGreen + 0.30*ColorTab[i].rgbRed);
		}
		for (y=0; y<P.nHeight; y++) {
			cbuf = (BYTE*) pImgm->GetPixelAddress(0,y); 
			gbuf = (BYTE*) gpImg.GetPixelAddress(0,y);
			for (x=0; x<P.nWidth; x++) 
				gbuf[x] = (BYTE) gray[cbuf[x]];         
 		}
	} 
	else {   
		for (y=0; y<P.nHeight; y++) {
			cbuf = (BYTE*) pImgm->GetPixelAddress(0,y); 
			gbuf = (BYTE*) gpImg.GetPixelAddress(0,y);
			for (x=0,i=0; x<P.nWidth; x++,i+=P.nBytesPerPixel) {
				gbuf[x] = (BYTE) (0.11*cbuf[i] + 0.59*cbuf[i+1] + 0.30*cbuf[i+2]);
			}
		}
	}

 	ImageCopy(pImgn,&gpImg);
 	gpImg.Destroy();
}


//////////Roberts算法///////////////////////////
void LocalProcess(CImage *pImg,int n)
{
 	struct	IMAGEPARAMENT P;
	BYTE	**list1,**list0;
	//RGBQUAD	ColorTab[256];
 	CImage  gImg1,gImg0;

  	ExpandImage(&gImg1,pImg);
 	GetImageParament(&gImg1,&P);
	gImg0.Create(P.nWidth,P.nHeight,P.nBitCount,0);
	ImageCopy(&gImg0,&gImg1);
	list1=(BYTE**) Create2DList(&gImg1);
	list0=(BYTE**) Create2DList(&gImg0);
 
 	GetImageParament(&gImg1,&P); 
	switch(n) {
		case 1:
			Median9(list0,list1,P.nWidth,P.nHeight);
			break;
		case 2:
			Roberts(list0,list1,P.nWidth,P.nHeight);
			break;
		case 3:
			Laplacian(list0,list1,P.nWidth,P.nHeight);
			break;
	}
 
	Release2DList(list1);
	Release2DList(list0);

	ReduceImage(pImg,&gImg0);

	gImg1.Destroy();
	gImg0.Destroy();
}
///////////////LocalProcess引用/////////////////////
void ExpandImage(CImage *pImgn,CImage *pImgm)
{
 	struct	IMAGEPARAMENT P;
	BYTE	*buf,*buf1;
  	int		k;
 
 	GetImageParament(pImgm,&P);                           
	pImgn->Create(P.nWidth+2,P.nHeight+2,P.nBitCount,0);

	for (int i=0;i<P.nHeight;i++) {
		buf  = (BYTE*) pImgm->GetPixelAddress(0,i);       
		buf1 = (BYTE*) pImgn->GetPixelAddress(1,i+1);
  		memcpy(buf1, buf, P.nBytesPerLine);                         
	}
	buf  = (BYTE*) pImgm->GetPixelAddress(0,0); 
	buf1 = (BYTE*) pImgn->GetPixelAddress(1,0);
  	memcpy(buf1, buf, P.nBytesPerLine);
	buf  = (BYTE*) pImgm->GetPixelAddress(0,P.nHeight-1);
	buf1 = (BYTE*) pImgn->GetPixelAddress(1,P.nHeight+1);
  	memcpy(buf1, buf, P.nBytesPerLine);
	buf=(BYTE*) malloc(P.nHeight);
	GetRectValue(pImgm,0,0,1,P.nHeight,buf);
	SetRectValue(pImgn,0,1,1,P.nHeight,buf);
	GetRectValue(pImgm,P.nWidth-1,0,1,P.nHeight,buf);
	SetRectValue(pImgn,P.nWidth+1,1,1,P.nHeight,buf);
	k=GetPixelValue(pImgm,0,0);
	SetPixelValue(pImgn,0,0,k);
	k=GetPixelValue(pImgm,P.nWidth-1,0);
	SetPixelValue(pImgn,P.nWidth+1,0,k);
	k=GetPixelValue(pImgm,0,P.nHeight-1);
	SetPixelValue(pImgn,0,P.nHeight+1,k);
	k=GetPixelValue(pImgm,P.nWidth-1,P.nHeight-1);
	SetPixelValue(pImgn,P.nWidth+1,P.nHeight+1,k);
	free(buf);
}
int Mask[3][3],Scale,Offset; 
void WINAPI Laplacian(BYTE **list0,BYTE **list1,int Dx,int Dy)
{
	Scale=1;
	Offset=1;               
	Mask[0][0]=Mask[0][2]=Mask[2][0]=Mask[2][2]=-1;
	Mask[0][1]=Mask[2][1]=Mask[1][0]=Mask[1][2]=-1;
	Mask[1][1]=8;
	Template(list0,list1,Dx,Dy);                 
	Offset=0;
}

void WINAPI Template(BYTE **list0,BYTE **list1,int Dx,int Dy)
{
	int  i,j,g;

	for (i=1;i<Dy-1;i++) {
		for (j=1;j<Dx-1;j++) {
			g = (Mask[0][0]*list1[i-1][j-1]+Mask[0][1]*list1[i-1][j]
			+Mask[0][2]*list1[i-1][j+1]+Mask[1][0]*list1[i][j-1]
			+Mask[1][1]*list1[i][j]    +Mask[1][2]*list1[i][j+1]
			+Mask[2][0]*list1[i+1][j-1]+Mask[2][1]*list1[i+1][j]
			+Mask[2][2]*list1[i+1][j+1])/Scale+Offset;  
			if (g>0xff) g=0xff;
			else if (g<0) g=0;
			list0[i][j] = (BYTE) g;   
		}
	}
}

///ExpandImage引用
DWORD GetPixelValue(CImage *pImg,int x,int y)
{
 	struct	IMAGEPARAMENT  P;
	BYTE	*lp;
 	DWORD	dd=0;

	GetImageParament(pImg,&P);
	if ((P.nBitCount<8)||(!InImage(pImg,x,y))) return 0;
	lp = (BYTE*) pImg->GetPixelAddress(x,y);
 	memcpy(&dd,lp,P.nBytesPerPixel);
	return(dd);
}

void SetPixelValue(CImage *pImg,int x,int y,DWORD c)
{
 	struct	IMAGEPARAMENT  P;
	BYTE	*lp;
  
 	GetImageParament(pImg,&P);
	if ((P.nBitCount<8)||(!InImage(pImg,x,y))) return;
	lp = (BYTE*) pImg->GetPixelAddress(x,y);
  	memcpy(lp,&c,P.nBytesPerPixel);
}

void GetRectValue(CImage *pImg,int x,int y,int Dx,int Dy,BYTE *buf)
{
 	struct	IMAGEPARAMENT  P;
	BYTE	*lp;
	int		i,dw,dh,x1,y1,alpha,delta,Dxb,dwb;
 
 	GetImageParament(pImg,&P);
	if (P.nBitCount<8) return;
	x1=x;
	y1=y;
	alpha=delta=0;
	if (x<0) { 
		alpha=-x;    x1=0;
	}
 	if (y<0) { 
		delta=-y;    y1=0;
	}
	if (!InImage(pImg,x1,y1)) return;
  	dw=min(Dx,(int) P.nWidth-x1);    
	dh=min(Dy,(int) P.nHeight-y1);
	dw -= alpha;
	dh -= delta;

 	Dxb = Dx*P.nBytesPerPixel;
	dwb = dw*P.nBytesPerPixel;
 	lp = (BYTE*) pImg->GetPixelAddress(x1,y1);
	buf += (delta*Dx+alpha)*P.nBytesPerPixel;
	for (i=0;i<dh;i++) {
		memcpy(buf,lp,dwb);  
 		buf += Dxb;	
		lp -= P.nBytesPerLine;
	}
}

void SetRectValue(CImage *pImg,int x,int y,int Dx,int Dy,BYTE *buf)
{
 	struct	IMAGEPARAMENT  P;
	BYTE	*lp;
	int		i,dw,dh,x1,y1,alpha,delta,Dxb,dwb;
 
 	GetImageParament(pImg,&P);
	if (P.nBitCount<8) return;
	x1=x;
	y1=y;
	alpha=delta=0;
	if (x<0) { 
		alpha=-x;    x1=0;
	}
 	if (y<0) { 
		delta=-y;    y1=0;
	}
	if (!InImage(pImg,x1,y1)) return;
  	dw=min(Dx,(int) P.nWidth-x1);    
	dh=min(Dy,(int) P.nHeight-y1);
	dw -= alpha;
	dh -= delta;

 	Dxb = Dx*P.nBytesPerPixel;
	dwb = dw*P.nBytesPerPixel;
 	lp = (BYTE*) pImg->GetPixelAddress(x1,y1);
	buf += (delta*Dx+alpha)*P.nBytesPerPixel;
	for (i=0;i<dh;i++) {
		memcpy(lp,buf,dwb);  
 		buf += Dxb;	
		lp -= P.nBytesPerLine;
	}
 }

//////////////LocalProcess引用/////////////////////
int	 InImage(CImage *pImg,int x,int y)
{
 	struct	IMAGEPARAMENT  P;
 
	GetImageParament(pImg,&P);
 	if ((x<0)||(y<0)||(x>=P.nWidth)||(y>=P.nHeight))  return 0;
	else  return 1;
}

BYTE** Create2DList(CImage *pImg)
{
 	struct		IMAGEPARAMENT P;
	int			i;
	BYTE		**list;

 	GetImageParament(pImg,&P);
	list=(BYTE**)malloc(P.nHeight*sizeof(BYTE*));        
	for (i=0;i<P.nHeight;i++) 
		list[i]=(BYTE*) pImg->GetPixelAddress(0,i); 

	return(list);
}

void Release2DList(BYTE** list)
{
	free(list);
}

void ReduceImage(CImage *pImgn,CImage *pImgm)
{
 	struct	IMAGEPARAMENT P;
	BYTE	*buf,*buf0;
  
 	GetImageParament(pImgn,&P);                           
	for (int i=0;i<P.nHeight;i++) {
		buf0 = (BYTE*) pImgm->GetPixelAddress(1,i+1);
		buf  = (BYTE*) pImgn->GetPixelAddress(0,i);       
  		memcpy(buf, buf0, P.nBytesPerLine);                         
	}
}

///////////////////////////////////////////////////////////

//Roberts算法具体实现
void WINAPI Roberts(BYTE **list0,BYTE **list1,int Dx,int Dy)
{                                                      //  roberts 梯度
   Gradiant0(list0,list1,Dx,Dy);
}
//调用

void Gradiant0(BYTE **list0,BYTE **list1,int Dx,int Dy)
{
  int  i,j,A,B,C;
  
  for (i=0;i<Dy-1;i++) {
    for (j=0;j<Dx-1;j++) {
	    A = abs(list1[i][j]-list1[i+1][j+1]);
	    B = abs(list1[i+1][j]-list1[i][j+1]);
        C = A + B;
      C *= 2;
	  if (C>255) C=255;
  	  list0[i][j] = C;
	}
  }
}

/////图像显示缩放
void ZoomIn(CImage *pImgn,CImage *pImgm,int n)  //放大 
{
 	int		i, j, k;
	RGBQUAD	ColorTab[256];
	BYTE	*buf,*buf0,*buf1;
  	struct	IMAGEPARAMENT P;
 
	GetImageParament(pImgm,&P); //读取图像参数             
 	pImgn->Destroy();
	pImgn->Create(P.nWidth*n,P.nHeight*n,P.nBitCount,0);  //建立放大的图像    

 	if (P.nNumColors>0)
	{
		pImgm->GetColorTable(0,P.nNumColors,ColorTab);    
		pImgn->SetColorTable(0,P.nNumColors,ColorTab); //复制调色板   
	}

  	for (i=0;i<P.nHeight;i++) {
		buf0 = (BYTE*) pImgm->GetPixelAddress(0,i);    //得首行地址
		buf  = (BYTE*) pImgn->GetPixelAddress(0,i*n);  
		for (j=0;j<P.nWidth;j++,buf0+=P.nBytesPerPixel)
		{
			for (k=0;k<n;k++,buf+=P.nBytesPerPixel)
    			memcpy(buf,buf0,P.nBytesPerPixel);     //复制像素数据     
  		}
	}

	GetImageParament(pImgn,&P);    //读取图像参数          
  	for (i=0;i<P.nHeight;i+=n) 
	{
		buf  = (BYTE*) pImgn->GetPixelAddress(0,i);
		for (j=1;j<n;j++) {
 			buf1=(BYTE*) pImgn->GetPixelAddress(0,i+j);
			memcpy(buf1,buf,P.nBytesPerLine);//复制行
		}
   	}
}

void ZoomOut(CImage *pImgn,CImage *pImgm,int n) //缩小
{  
	int			i, j;
	RGBQUAD		ColorTab[256];
	BYTE		*buf,*buf0;
 	struct		IMAGEPARAMENT P;

	GetImageParament(pImgm,&P);     //读取图像参数          
 	pImgn->Destroy();
	pImgn->Create(P.nWidth/n,P.nHeight/n,P.nBitCount,0);  //建立缩小的图像       
 
 	if (P.nNumColors>0)
	{
		pImgm->GetColorTable(0,P.nNumColors,ColorTab);
		pImgn->SetColorTable(0,P.nNumColors,ColorTab);//复制调色板 
	}
  
 	for (i=0;i<P.nHeight/n;i++) {
		buf0 = (BYTE*) pImgm->GetPixelAddress(0,i*n);//得首行地址
		buf  = (BYTE*) pImgn->GetPixelAddress(0,i);
		for (j=0;j<P.nWidth/n;j++)
		{
 			memcpy(buf, buf0, P.nBytesPerPixel);//复制像素数据
			buf+=P.nBytesPerPixel;              //调整像素指针
			buf0+=n*P.nBytesPerPixel;
  		}
 	}
}


//////中值滤波////////
void WINAPI Median9(BYTE **list0,BYTE **list1,int Dx,int Dy)
{                                          //  3x3邻域九点中值滤波
   Median(list0,list1,Dx,Dy,9);
}

void Median(BYTE **list0, BYTE **list1, int Dx, int Dy, int flag)
{
  int  i,j,buff1[20];
  
  for (i=1;i<Dy-1;i++) {
    for (j=1;j<Dx-1;j++) {
	 buff1[0] = list1[i-1][j];                        
	 buff1[1] = list1[i][j];
	 buff1[2] = list1[i+1][j];         
	 buff1[3] = list1[i][j-1];
	 buff1[4] = list1[i][j+1];               
	 if (flag==9) {
	   buff1[5] = list1[i-1][j-1];              
	   buff1[6] = list1[i-1][j+1];  	  
	   buff1[7] = list1[i+1][j-1];      
	   buff1[8] = list1[i+1][j+1];
     }
     list0[i][j] = (BYTE) MedValue(buff1, flag, flag/2);
	}
  }
}

int  MedValue(int *buf, int n, int m)	            //  排序取中值
{
   int i,j,k,f;

   for (i=1;i<n;i++) {	                               
      f=0;	                                   
      for (j=n-1;j>=i;j--) {
		  if (buf[j]>buf[j+1]) {	                           
		     k=buf[j];    buf[j]=buf[j+1];  
		     buf[j+1]=k;    f=1;
	      }
      }
      if (f==0) break;	                           
   }
   return(buf[m]);	                             
}

///////二值化//////////////
void Threshold(CImage *pImgm,int n,double ff)
{
 	struct	IMAGEPARAMENT P;
	BYTE	**list,*lpBuf;
	RGBQUAD	ColorTab[256];
	long	pg[256];
 	int		x,y,thre;
	//int		t,tab[256],buf[20],bn;
    
 	GetImageParament(pImgm,&P);              

	list=(BYTE**) Create2DList(pImgm);
	histog(list,pg,0,0,P.nWidth,P.nHeight);
 
	switch(n) {
		case 1:
			thre=KSW_Entropic(pg);
			break;
		case 2:
			thre=Moment(pg);
			break;
        //case 2:
		//	thre=Otsu(pg);
		//	break;

	}

 	for (y=0;y<P.nHeight;y++) {
		lpBuf=(BYTE*) pImgm->GetPixelAddress(0,y);
		for (x=0;x<P.nWidth;x++,lpBuf++) {
			if (*lpBuf>=thre) *lpBuf=1;
			else		      *lpBuf=0;
		}
	}
 	memset(ColorTab,0,1024);
	SetAllPalette(pImgm,ColorTab);
    SetPalette(pImgm,1,255,255,255);

	Release2DList(list);
}
////熵法取阈值
int  WINAPI KSW_Entropic(long *pg)            //  熵法取阈值
{
   long   i,t,s;
   double p[256],Pt[256],Ht[256],HT,H[256];
   double A,B,C;
  
   for (i=0,s=0;i<256;i++) s+=pg[i];
   for (i=0;i<256;i++) p[i]=((double) pg[i])/s;

   Pt[0]=p[0];
   for (i=1;i<256;i++) Pt[i]=Pt[i-1]+p[i];

   for (i=0;i<256;i++) {
      if (p[i]==0) Ht[i]=0;
	  else  Ht[i]=-p[i]*log(p[i]);
   }
   for (i=1;i<256;i++) Ht[i]+=Ht[i-1];
   HT=Ht[255];

   for (i=0;i<256;i++) {
	  A=Pt[i]*(1-Pt[i]);
      if (A>0) A=log(A);
	  B=Pt[i];
	  if (B>0) B=Ht[i]/B;
	  C=1-Pt[i];
	  if (C>0) C=(HT-Ht[i])/C;
	  H[i]=A+B+C;
   }

   t=MaxMin(H,1);
   return(t);
}

int  WINAPI Moment(long *pg)                //  矩法取阈值
{
	long   i,t;
	double m0,m1,m2,m3,p0,C0,C1;

	m0=m1=m2=m3=0;
	for (i=0;i<256;i++) {
		m0+=(double) pg[i];		
		m1+=(double) pg[i]*i;
		m2+=(double) pg[i]*i*i;
		m3+=(double) pg[i]*i*i*i;
	}
	C0=(m1*m3-m2*m2)/(m0*m2-m1*m1);
	C1=(m1*m2-m0*m3)/(m0*m2-m1*m1);
	p0=0.5-(m1/m0+C1/2)/sqrt(C1*C1-4*C0);
	t=Ptile(pg,p0);
	return(t);
}

int  WINAPI Ptile(long *pg,double nn)       //  分位数法
{
	int  i;
	double mm,kk;

	for (i=0,mm=0;i<256;i++) mm+=pg[i];
	kk=0;
	for (i=0;i<256;i++) {
		kk+=(double) pg[i]/mm;
		if (kk>=nn)  break;
	}
	return i;
}

void WINAPI histog(BYTE **list,long *pg,int x, int y, int Dx, int Dy)
{                                     //  统计直方图                              
	int   i,j;

  	for (i=0;i<256;i++)  pg[i]=0;

	for(i=y;i<y+Dy;i++) 
 		for (j=x;j<x+Dx;j++)  
			pg[list[i][j]]++;	
}

void SetAllPalette(CImage *pImg, RGBQUAD *ColorTab)
{
   	struct	IMAGEPARAMENT P;
 
 	GetImageParament(pImg,&P);
	pImg->SetColorTable(0, P.nNumColors, ColorTab);
}

void SetPalette(CImage *pImg,int n,int r,int g,int b)
{
	RGBQUAD ColorTab;

	ColorTab.rgbBlue  = b; 
	ColorTab.rgbGreen = g; 
	ColorTab.rgbRed   = r;
	pImg->SetColorTable(n, 1, &ColorTab);
}

int  MaxMin(double *tab,int flag)
{
   double  max,min;
   int     i,p,q;

   max=min=tab[128];		
   p=q=128;
   for (i=0;i<256;i++) {
      if (tab[i]>max) {
	     max=tab[i];	
		 p=i;
      }
	  if (tab[i]<min) {
		 min=tab[i];	
		 q=i;
	  }
   }
   if (flag==1) {
      for (i=0;i<256;i++) 
         tab[i]=120*(max-tab[i])/(max-min);
   }
   else {         
      for (i=0;i<256;i++) 
         tab[i]=120*(tab[i]-min)/(max-min);
	  p=q;
   } 
   return(p);
}
////////细化、消除独立噪声///////////////////////
void CurveThinning(CImage *pImg0,CImage *pImg1,int n)
{
 	struct	IMAGEPARAMENT P;
	BYTE	**list;
      
	LowTypeToIndex(pImg0,pImg1);
	SetPalette(pImg0,0,0,0,0);
    SetPalette(pImg0,1,255,255,255);
   
 	GetImageParament(pImg0,&P);
	list=(BYTE**) Create2DList(pImg0);
	switch(n) {
		case 1:
			RemoveNoise(list,P.nBytesPerLine,P.nHeight);
			break;
		case 2:
			Deutch(list,P.nBytesPerLine,P.nHeight);
	}

	Release2DList(list);
}
/////消除孤立噪声///////////////////
void WINAPI RemoveNoise(BYTE **list,int Dx,int Dy)
{
   long i,j,k,m,n;
   BYTE **list0;

   list0=(BYTE**)malloc(Dy*sizeof(BYTE*));
   for(j=0;j<Dy;j++) { 
	  list0[j]=(BYTE*) malloc(Dx);
	  memset(list0[j],0,Dx);
   }
 
   for (j=1;j<Dy-1;j++) {
	  for (i=1;i<Dx-1;i++) {
		 for (m=-1,k=0;m<2;m++) {
			for (n=-1;n<2;n++) {
               if (list[j+m][i+n]) k++;
			}
	     }
		 if (k>4) list0[j][i]=1;
 	  }
   }

   for(j=0;j<Dy;j++) {
      memcpy(list[j],list0[j],Dx);
      if (list0[j]) free(list0[j]);
   }
   free(list0);  
}

//////////////膨胀////////////
void Morphologic(CImage *pImg0,CImage *pImg1,int n)
{
 	struct	IMAGEPARAMENT P;
 	BYTE	**list1,**list0;
	//int		k;
	CImage  gImg1,gImg0;
   
	//k=ImageType(pImg1);
 	//if (k==2)
	//	Threshold(pImg1,1,0);
	//else if (k!=0) return;
 
 	ExpandImage(&gImg1,pImg1);
	ImageCopy(&gImg0,&gImg1);
 	list1=(BYTE**) Create2DList(&gImg1);
	list0=(BYTE**) Create2DList(&gImg0);
 
 	GetImageParament(&gImg1,&P); 
	switch(n) {
		case 1:
	    Dilation(list0,list1,P.nWidth,P.nHeight);
		break;
		case 2:
		Erosion(list0,list1,P.nWidth,P.nHeight);
		break;
	}


	Release2DList(list1);
	Release2DList(list0);

    //if (n<7)
		ReduceImage(pImg1,&gImg0);
	//else
	//	ReduceImage(pImg1,&gImg1);
 
	gImg1.Destroy();
	gImg0.Destroy();
}

int EDMask[9][2]={{ 0, 0},{-1,0},{ 0,-1},{0,1},{1,0},
				  {-1,-1},{-1,1},{ 1,-1},{1,1}};

void WINAPI Dilation(BYTE **list0,BYTE **list1,int Dx,int Dy)
{
   dilation(list0,list1,Dx,Dy,EDMask,9);
}

void WINAPI dilation(BYTE **list0,BYTE **list1,int Dx,int Dy,int Mask[][2],int MaskLen)
{
  int  i,j,k;

  for(k=0;k<Dy;k++) 
    memset(list0[k],0,Dx);                           
 
  for(j=1;j<Dy-1;j++) {
    for(i=1;i<Dx-1;i++){ 
		for(k=0;k<MaskLen;k++) {                      
			if (list1[j+Mask[k][1]][i+Mask[k][0]]==1) {                
          list0[j][i]=1;                          
	      break;
		}
	  }
	}
  }
}

void WINAPI Erosion(BYTE **list0,BYTE **list1,int Dx,int Dy)
{
   erosion(list0,list1,Dx,Dy,EDMask,9);
}

void WINAPI erosion(BYTE **list0,BYTE **list1,int Dx,int Dy,int Mask[][2],int MaskLen)
{
  int  i,j,k;

  for(k=0;k<Dy;k++) 
    memset(list0[k],0,Dx);                           
 
  for(j=1;j<Dy-1;j++) {
    for(i=1;i<Dx-1;i++){ 
		for(k=0;k<MaskLen;k++) {                      
		if (list1[j+Mask[k][1]][i+Mask[k][0]]==0)                
	      break;
	  }
      if (k==MaskLen) list0[j][i]=1;                
	}
  }
}

////////////////Deutch细化////////////////////
#define  COLOR	0
void WINAPI Deutch(BYTE **list0,int Dx,int Dy)
{
  int  i,j,k,l,ii,flag;
  int  sum=0, sum1=0, f[10];
  char *buf,**list;
 
  buf =(char*)  malloc(Dx*Dy*sizeof(char));
  list=(char**) malloc(Dy*sizeof(char*));
  for(k=0;k<Dy;k++) {
    list[k]=buf+k*Dx;
    memcpy(list[k],list0[k],Dx);
  }
 
  i=1;
  do {
    flag=0;
    for (k=1; k<Dy-1; k++) {
      for (j=1; j<Dx-1; j++) {
	if (list[k][j] != 1)  continue;
	else if (i==1) {
	  f[1]=list[k][j+1];
	  f[2]=list[k-1][j+1];
	  f[3]=list[k-1][j];
	  f[4]=list[k-1][j-1];
	  f[5]=list[k][j-1];
	  f[6]=list[k+1][j-1];
	  f[7]=list[k+1][j];
	  f[8]=list[k+1][j+1];
	}
	else {
	  f[1]=list[k][j-1];
	  f[2]=list[k+1][j-1];
	  f[3]=list[k+1][j];
	  f[4]=list[k+1][j+1];
	  f[5]=list[k][j+1];
	  f[6]=list[k-1][j+1];
	  f[7]=list[k-1][j];
	  f[8]=list[k-1][j-1];
	}
	for (ii=1; ii<=8; ii++) {
	  if (f[ii]==-i)    f[ii]=1;
	  else if(f[ii]<0)  f[ii]=0;
	}
	f[9]=f[1];

	sum=sum1=0;
	for (l=1; l<9; l++) {
	  if ((f[l+1]-f[l])!=0)  sum++;
	  sum1+=f[l];
	}
	if (sum1==1)  continue;
	if (sum>4)    continue;
	if (sum==1 || sum==3)     continue;
	if ((f[1]*f[3]*f[5])!=0)  continue;
	if ((f[1]*f[3]*f[7])!=0)  continue;
	if (sum==4) {
	  if (f[1]*f[7]!=1 || f[2]+f[6]!=1 ||
		 f[3]+f[4]+f[5]+f[8]!=0)  continue;
	  if (f[1]*f[3]!=1 || f[4]+f[8]!=1 ||
	    f[2]+f[5]+f[6]+f[7]!=0)  continue;
	}
	list[k][j] = -i;
	list0[k][j]= COLOR;
	flag=1;
      }
    }
    i++;
  }
  while(flag==1);

  free(list);                   
  free(buf);
}

/////轮廓跟踪/////////////
RGBQUAD  D_pal[256] = { 
	{0x00,0x00,0x00,0x00},{0xa8,0x00,0x00,0x00},{0x00,0xa8,0x00,0x00},
	{0xa8,0xa8,0x00,0x00},{0x00,0x00,0xa8,0x00},{0xa8,0x00,0xa8,0x00},
	{0x00,0x54,0xa8,0x00},{0xa8,0xa8,0xa8,0x00},{0x54,0x54,0x54,0x00},
	{0xfc,0x54,0x54,0x00},{0x54,0xfc,0x54,0x00},{0xfc,0xfc,0x54,0x00},
	{0x54,0x54,0xfc,0x00},{0xfc,0x54,0xfc,0x00},{0x54,0xfc,0xfc,0x00},
	{0xfc,0xfc,0xfc,0x00},{0x00,0x00,0x00,0x00},{0x14,0x14,0x14,0x00},
	{0x20,0x20,0x20,0x00},{0x2c,0x2c,0x2c,0x00},{0x38,0x38,0x38,0x00},
	{0x44,0x44,0x44,0x00},{0x50,0x50,0x50,0x00},{0x60,0x60,0x60,0x00},
	{0x70,0x70,0x70,0x00},{0x80,0x80,0x80,0x00},{0x90,0x90,0x90,0x00},
	{0xa0,0xa0,0xa0,0x00},{0xb4,0xb4,0xb4,0x00},{0xc8,0xc8,0xc8,0x00},
	{0xe0,0xe0,0xe0,0x00},{0xfc,0xfc,0xfc,0x00},{0xfc,0x00,0x00,0x00},
	{0xfc,0x00,0x40,0x00},{0xfc,0x00,0x7c,0x00},{0xfc,0x00,0xbc,0x00},
	{0xfc,0x00,0xfc,0x00},{0xbc,0x00,0xfc,0x00},{0x7c,0x00,0xfc,0x00},
	{0x40,0x00,0xfc,0x00},{0x00,0x00,0xfc,0x00},{0x00,0x40,0xfc,0x00},
	{0x00,0x7c,0xfc,0x00},{0x00,0xbc,0xfc,0x00},{0x00,0xfc,0xfc,0x00},
	{0x00,0xfc,0xbc,0x00},{0x00,0xfc,0x7c,0x00},{0x00,0xfc,0x40,0x00},
	{0x00,0xfc,0x00,0x00},{0x40,0xfc,0x00,0x00},{0x7c,0xfc,0x00,0x00},
	{0xbc,0xfc,0x00,0x00},{0xfc,0xfc,0x00,0x00},{0xfc,0xbc,0x00,0x00},
	{0xfc,0x7c,0x00,0x00},{0xfc,0x40,0x00,0x00},{0xfc,0x7c,0x7c,0x00},
	{0xfc,0x7c,0x9c,0x00},{0xfc,0x7c,0xbc,0x00},{0xfc,0x7c,0xdc,0x00},
	{0xfc,0x7c,0xfc,0x00},{0xdc,0x7c,0xfc,0x00},{0xbc,0x7c,0xfc,0x00},
	{0x9c,0x7c,0xfc,0x00},{0x7c,0x7c,0xfc,0x00},{0x7c,0x9c,0xfc,0x00},
	{0x7c,0xbc,0xfc,0x00},{0x7c,0xdc,0xfc,0x00},{0x7c,0xfc,0xfc,0x00},
	{0x7c,0xfc,0xdc,0x00},{0x7c,0xfc,0xbc,0x00},{0x7c,0xfc,0x9c,0x00},
	{0x7c,0xfc,0x7c,0x00},{0x9c,0xfc,0x7c,0x00},{0xbc,0xfc,0x7c,0x00},
	{0xdc,0xfc,0x7c,0x00},{0xfc,0xfc,0x7c,0x00},{0xfc,0xdc,0x7c,0x00},
	{0xfc,0xbc,0x7c,0x00},{0xfc,0x9c,0x7c,0x00},{0xfc,0xb4,0xb4,0x00},
	{0xfc,0xb4,0xc4,0x00},{0xfc,0xb4,0xd8,0x00},{0xfc,0xb4,0xe8,0x00},
	{0xfc,0xb4,0xfc,0x00},{0xe8,0xb4,0xfc,0x00},{0xd8,0xb4,0xfc,0x00},
	{0xc4,0xb4,0xfc,0x00},{0xb4,0xb4,0xfc,0x00},{0xb4,0xc4,0xfc,0x00},
	{0xb4,0xd8,0xfc,0x00},{0xb4,0xe8,0xfc,0x00},{0xb4,0xfc,0xfc,0x00},
	{0xb4,0xfc,0xe8,0x00},{0xb4,0xfc,0xd8,0x00},{0xb4,0xfc,0xc4,0x00},
	{0xb4,0xfc,0xb4,0x00},{0xc4,0xfc,0xb4,0x00},{0xd8,0xfc,0xb4,0x00},
	{0xe8,0xfc,0xb4,0x00},{0xfc,0xfc,0xb4,0x00},{0xfc,0xe8,0xb4,0x00},
	{0xfc,0xd8,0xb4,0x00},{0xfc,0xc4,0xb4,0x00},{0x70,0x00,0x00,0x00},
	{0x70,0x00,0x1c,0x00},{0x70,0x00,0x38,0x00},{0x70,0x00,0x54,0x00},
	{0x70,0x00,0x70,0x00},{0x54,0x00,0x70,0x00},{0x38,0x00,0x70,0x00},
	{0x1c,0x00,0x70,0x00},{0x00,0x00,0x70,0x00},{0x00,0x1c,0x70,0x00},
	{0x00,0x38,0x70,0x00},{0x00,0x54,0x70,0x00},{0x00,0x70,0x70,0x00},
	{0x00,0x70,0x54,0x00},{0x00,0x70,0x38,0x00},{0x00,0x70,0x1c,0x00},
	{0x00,0x70,0x00,0x00},{0x1c,0x70,0x00,0x00},{0x38,0x70,0x00,0x00},
	{0x54,0x70,0x00,0x00},{0x70,0x70,0x00,0x00},{0x70,0x54,0x00,0x00},
	{0x70,0x38,0x00,0x00},{0x70,0x1c,0x00,0x00},{0x70,0x38,0x38,0x00},
	{0x70,0x38,0x44,0x00},{0x70,0x38,0x54,0x00},{0x70,0x38,0x60,0x00},
	{0x70,0x38,0x70,0x00},{0x60,0x38,0x70,0x00},{0x54,0x38,0x70,0x00},
	{0x44,0x38,0x70,0x00},{0x38,0x38,0x70,0x00},{0x38,0x44,0x70,0x00},
	{0x38,0x54,0x70,0x00},{0x38,0x60,0x70,0x00},{0x38,0x70,0x70,0x00},
	{0x38,0x70,0x60,0x00},{0x38,0x70,0x54,0x00},{0x38,0x70,0x44,0x00},
	{0x38,0x70,0x38,0x00},{0x44,0x70,0x38,0x00},{0x54,0x70,0x38,0x00},
	{0x60,0x70,0x38,0x00},{0x70,0x70,0x38,0x00},{0x70,0x60,0x38,0x00},
	{0x70,0x54,0x38,0x00},{0x70,0x44,0x38,0x00},{0x70,0x50,0x50,0x00},
	{0x70,0x50,0x58,0x00},{0x70,0x50,0x60,0x00},{0x70,0x50,0x68,0x00},
	{0x70,0x50,0x70,0x00},{0x68,0x50,0x70,0x00},{0x60,0x50,0x70,0x00},
	{0x58,0x50,0x70,0x00},{0x50,0x50,0x70,0x00},{0x50,0x58,0x70,0x00},
	{0x50,0x60,0x70,0x00},{0x50,0x68,0x70,0x00},{0x50,0x70,0x70,0x00},
	{0x50,0x70,0x68,0x00},{0x50,0x70,0x60,0x00},{0x50,0x70,0x58,0x00},
	{0x50,0x70,0x50,0x00},{0x58,0x70,0x50,0x00},{0x60,0x70,0x50,0x00},
	{0x68,0x70,0x50,0x00},{0x70,0x70,0x50,0x00},{0x70,0x68,0x50,0x00},
	{0x70,0x60,0x50,0x00},{0x70,0x58,0x50,0x00},{0x40,0x00,0x00,0x00},
	{0x40,0x00,0x10,0x00},{0x40,0x00,0x20,0x00},{0x40,0x00,0x30,0x00},
	{0x40,0x00,0x40,0x00},{0x30,0x00,0x40,0x00},{0x20,0x00,0x40,0x00},
	{0x10,0x00,0x40,0x00},{0x00,0x00,0x40,0x00},{0x00,0x10,0x40,0x00},
	{0x00,0x20,0x40,0x00},{0x00,0x30,0x40,0x00},{0x00,0x40,0x40,0x00},
	{0x00,0x40,0x30,0x00},{0x00,0x40,0x20,0x00},{0x00,0x40,0x10,0x00},
	{0x00,0x40,0x00,0x00},{0x10,0x40,0x00,0x00},{0x20,0x40,0x00,0x00},
	{0x30,0x40,0x00,0x00},{0x40,0x40,0x00,0x00},{0x40,0x30,0x00,0x00},
	{0x40,0x20,0x00,0x00},{0x40,0x10,0x00,0x00},{0x40,0x20,0x20,0x00},
	{0x40,0x20,0x28,0x00},{0x40,0x20,0x30,0x00},{0x40,0x20,0x38,0x00},
	{0x40,0x20,0x40,0x00},{0x38,0x20,0x40,0x00},{0x30,0x20,0x40,0x00},
	{0x28,0x20,0x40,0x00},{0x20,0x20,0x40,0x00},{0x20,0x28,0x40,0x00},
	{0x20,0x30,0x40,0x00},{0x20,0x38,0x40,0x00},{0x20,0x40,0x40,0x00},
	{0x20,0x40,0x38,0x00},{0x20,0x40,0x30,0x00},{0x20,0x40,0x28,0x00},
	{0x20,0x40,0x20,0x00},{0x28,0x40,0x20,0x00},{0x30,0x40,0x20,0x00},
	{0x38,0x40,0x20,0x00},{0x40,0x40,0x20,0x00},{0x40,0x38,0x20,0x00},
	{0x40,0x30,0x20,0x00},{0x40,0x28,0x20,0x00},{0x40,0x2c,0x2c,0x00},
	{0x40,0x2c,0x30,0x00},{0x40,0x2c,0x34,0x00},{0x40,0x2c,0x3c,0x00},
	{0x40,0x2c,0x40,0x00},{0x3c,0x2c,0x40,0x00},{0x34,0x2c,0x40,0x00},
	{0x30,0x2c,0x40,0x00},{0x2c,0x2c,0x40,0x00},{0x2c,0x30,0x40,0x00},
	{0x2c,0x34,0x40,0x00},{0x2c,0x3c,0x40,0x00},{0x2c,0x40,0x40,0x00},
	{0x2c,0x40,0x3c,0x00},{0x2c,0x40,0x34,0x00},{0x2c,0x40,0x30,0x00},
	{0x2c,0x40,0x2c,0x00},{0x30,0x40,0x2c,0x00},{0x34,0x40,0x2c,0x00},
	{0x3c,0x40,0x2c,0x00},{0x40,0x40,0x2c,0x00},{0x40,0x3c,0x2c,0x00},
	{0x40,0x34,0x2c,0x00},{0x40,0x30,0x2c,0x00},{0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00},{0x00,0x00,0x00,0x00},{0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00},{0x00,0x00,0x00,0x00},{0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00}   
};

int	   ThreTable[256];
int    *code;
POINT  *dot;
int	   code_n,dot_n;
int    inc[8][2] = { {+1, 0},{+1,-1},{ 0,-1},{-1,-1},
                     {-1, 0},{-1,+1},{ 0,+1},{+1,+1}  };
int    Trx1,Try1,Trx2,Try2;
struct TAB *tab;

int dot_s[256];//奇异点记录程序
int dt=0;

POINT max, min;           //最左右点坐标
int min_k,max_k;
POINT left_pt;            //左右端点坐标
POINT right_pt;
int left_k,right_k;

void SetDefaultPalette(CImage *pImg)
{
	if (pImg->GetBPP()>8) return;
	pImg->SetColorTable(0, 256, D_pal);
}

int  ContourTracking(CImage *pImg,CDC *pDC)                //轮廓跟踪(数据格式转换)
{
 	struct	IMAGEPARAMENT P;
 	BYTE	**list;
    int max_n=0; 
	//AdjustAreaType(pImg);                             //调整二值图像类型
	for (int i=0;i<256;i++) ThreTable[i]=0;          //设置二值化表
	ThreTable[1]=1;
 	GetImageParament(pImg,&P);                         //获取图像类型
 	list=(BYTE**) Create2DList(pImg);                 //建立二维数组
    max_n=MutiTrack(list,P.nBytesPerLine,P.nHeight,pDC);       //多区域跟踪
	Release2DList(list);                            //释放二维数组
	return(max_n);
}


int WINAPI MutiTrack(BYTE **list,int Dx,int Dy,CDC *pDC) //多区域跟踪
{
  int    i,j,n,max_n=0,max_tmp=0;
  
  Trx1=0;			Try1=0;             //设置跟踪范围，设区域左上角坐标
  Trx2=Dx-1;		Try2=Dy-1;          //设置区域右下角坐标         
 
  dot =(POINT*) malloc(sizeof(POINT)*MAX_DOTN); //申请链表码存放单元
  code=(int*) malloc(sizeof(int)*MAX_DOTN);      //申请线段表存放单元
                                                   
  n=2;                                      //背景色初始化
  for (j=0;j<Dy-1;j++)                     //按行顺序进行扫描
   { 
	  for (i=0;i<Dx-1;i++)                 //在行内进行水平向扫描
 	  {   
		if (ThreTable[list[j][i]]==1)       //遇到一个对象点开始进行跟踪
	   {	     
		code_n=SingleTrack(list,i,j,4,code,8);       //单个对象作8连通跟踪
        //code_n=Code4To8(code);                         //4连通码转换成8连通链码
		dot_n =CodeToTable(code,dot);               //链码表转化成线段表
        fillarea(list,dot,dot_n,n++);            //用背景色填充区域
		if(dot_n>=max_tmp)
		{
			max_n=n;
			max_tmp=dot_n;
		}
		DrawTextOnCanvus(pDC,dot_s[dt-2],dot_s[dt-1],n); //标记图像编号
 		if (n==7) n+=2;                       //跳过默认调色板中的淡灰和深灰
 		else if (n==15) n+=17;               //跳过默认调色板中的白色与灰色
		else if (n==248) n=2;                //跳过默认调色板中最后8个颜色，从头开始
	   }
	 }
  }


  free(code); //释放工作单元                                  
  free(dot);
  return(max_n);
}

int  WINAPI SingleTrack(BYTE **list,int i,int j,int iop,int *code,int flag)//单区域跟踪
{               
   int  kw,code_n;
   int  curr[2],next[2];               
   int  n,n1,u,ki,kj,ns,ne;
  
   u=1;

   code[0]=curr[0]=next[0]=i;    //工作单元初始化
   code[1]=curr[1]=next[1]=j;	                     
   code_n=3;	                       
   code[3]=-1;
   do {
     /////////初始化////
	  kw=0;	                     //标志初始化
      ns=iop;    ne=ns-8;        //设置当前像素的跟踪起始链码,终止链码    
     //////检测有效点//////  
	  for (n=ns;n>ne;n-=u)   //当前点检测邻点像素
	 { 
	     n1 = (n+8) % 8;	         //模八运算，使链码值保持在0~7之间
	     ki=curr[0]+inc[n1][0];      //查表后计算得到当前链码所指点的坐标
 	     kj=curr[1]+inc[n1][1];     
         
		 if ((ki>=Trx1)&&(kj>=Try1)&&(ki<=Trx2)&&(kj<=Try2))	//在范围内则测量
 	         {
				 if (ThreTable[list[kj][ki]]==1) break;	             //使新边界点则跳出
		     }
	  }
	  
	  /////检测是否将八个点均检测完成，未测完则将数据记录//////
	  if (n!=ne) {                               //检测为中途跳出，为检索到新边界点
	     next[0]=ki;  next[1]=kj;          //记录新边界点坐标
	     iop=n1;	                   //记录指向新边界点的链码
	  }
      else  kw = 1;	                 //无后续边界点，则设置孤立点标志

	  ////
      if ((curr[0]==i)&&(curr[1]==j)&&(iop==code[3]))  break;   //检测点回到起始位置，且链码方向与起始点链码一致时，跟踪结束
      code[code_n++]=iop;	                                   //在链码表中记录指向新边界点的链码
      curr[0]=next[0];   curr[1]=next[1];                     //检测达到的新边界点设为当前点

	  if ((flag==8)&&(iop%2==0)) iop--;             //偶数链码时先减去1
      iop=(iop+2)%8;	                           //链码值再加2，使链码方向指向背景区域
   
   }
   while(kw==0);	                         //未封闭继续跟踪

   code[2]=code_n-3;	                       //在链码表中存储边界点数
   return(code_n-3);                         //返回边界点数
}

int  WINAPI fillarea(BYTE **list,POINT *dot, int dot_n,int n)    //用背景色填充区域
{  
   int  i,y,k,s;
  
   for (i=0,s=0;i<dot_n;i+=2) {	         
     y=dot[i].y;//取出线段的纵坐标
	 k=dot[i+1].x-dot[i].x+1;//计算线段的长度
	 //s+=k;//像素数累加
	 memset(&list[y][dot[i].x],n,k);
   }
   return(k);//返回区域面积
}

void DrawTextOnCanvus(CDC *pDC,int x,int y,int n)
{
	int			i,nYChar;
	TEXTMETRIC	tm;
	char		*textbuf[]={"曲线-"};
	pDC->SetTextColor(0x000000);
	pDC->SetBkColor(0xffffff);
	pDC->SetBkMode(OPAQUE);
	pDC->GetTextMetrics(&tm);
	nYChar=tm.tmHeight+tm.tmExternalLeading;
	for (i=0;i<5;i++) {
		pDC->TextOut(x,y+nYChar*i,textbuf[i]);
	}
	CString str;
	str.Format("%X",n-1);
	pDC->TextOut(x+30,y,str);
}


int  WINAPI CodeToTable(int *code,POINT *dot)  //  链码表转换成线段表    
{  
   int  tab[8][8]={
      // 0 1 2 3 4 5 6 7  //    转换表
       { 0,0,0,0,2,2,2,2 },     // 0    //0为中间点，1为左端点
       { 1,1,1,1,0,3,3,3 },     // 1    //2为右端点，3为奇异点
       { 1,1,1,1,0,0,3,3 },     // 2    //竖向是进入的链码
       { 1,1,1,1,0,0,0,3 },     // 3    //横向是离开时的链码
       { 1,1,1,1,0,0,0,0 },     // 4  
       { 0,3,3,3,2,2,2,2 },     // 5  
       { 0,0,3,3,2,2,2,2 },     // 6  
       { 0,0,0,3,2,2,2,2 }  };  // 7  
   int  i,j,n,k,k1,ki,kj,d;
   int  dot_n;

   dot_n=0;	                      //线段表端点数初始化     

   ki=code[0];   kj=code[1];	  //取得起始点坐标               
   n=code[2];                     //取得链码点数
   
   if (n==1) {                    //一个链码的情况
      dot[0].x=ki;	  dot[0].y=kj;                    
      dot[1].x=ki;	  dot[1].y=kj;                    
      return(2);                                    
   }
  
   code[n+3]=code[3];                 //第一点链码放到链码表末尾形成循环	             
   
   for (i=3;i<n+3;i++) {	          //沿整个链码序列处理一次           
      k=code[i]%8;   k1=code[i+1]%8;  //取进入当前点链码
      d=tab[k][k1];	                  //查表得到当前点的类型
      ki+=inc[k][0];   kj+=inc[k][1]; //计算当前点坐标
      if (d!=0) {	                  //中间点，不填入临时表
	    dot[dot_n].x  =ki;            //左右端点天一次
	    dot[dot_n++].y=kj;                    
	    if (d==3) {
	      dot[dot_n].x  =ki;          //奇异点再填一次
	      dot[dot_n++].y=kj;
	      dot_s[dt] =ki;             //记录奇异点
	      dot_s[dt+1]=kj;
		  dt+=2;
		}
      }
   }

   
   for (i=0;i<dot_n-1;i++) {                  //对y坐标选择排序
      k=i;      n=dot[i].y;
      for (j=i+1;j<dot_n;j++) {
	    if (dot[j].y<n) {
	      k=j;	n=dot[j].y;
		}
      }
      ki=dot[i].x;          kj=dot[i].y;
      dot[i].x=dot[k].x;    dot[i].y=n;
      dot[k].x=ki;          dot[k].y=kj;
   }
   
   
   for (i=1;i<dot_n;i++) {	          //行内x坐标作冒泡排序
        for (j=dot_n-1;j>=i;j--) {
	    if (dot[j-1].y!=dot[j].y) continue; //不同行间不进行排序
	    if (dot[j-1].x>dot[j].x) {
	      ki=dot[j-1].x;          kj=dot[j-1].y;
	      dot[j-1].x=dot[j].x;    dot[j-1].y=dot[j].y;
	      dot[j].x=ki;            dot[j].y=kj;
		}
      }
   }
   return(dot_n);                                    //返回端点数
}

//////////////////曲线确认与调整////////////
void LineAdjust(CImage *pImg,int n)
{
	struct	IMAGEPARAMENT P;
 	BYTE	**list;
	
	for (int i=0;i<256;i++) ThreTable[i]=0;         //设置二值化表
	ThreTable[1]=1;
	GetImageParament(pImg,&P);
	list=(BYTE**) Create2DList(pImg);                //读取图像的二维数
	Scan(list,n,P.nBytesPerLine,P.nHeight);          //曲线确认
	Rotate(list,P.nBytesPerLine,P.nHeight);       //接触点确认
	Release2DList(list);

}

void WINAPI Scan(BYTE **list,int n,int Dx,int Dy)   //曲线确认
{
	int i,j;
	Trx1=0;			Try1=0;                       //设置跟踪范围，设区域左上角坐标
	Trx2=Dx-1;		Try2=Dy-1;                    //设置区域右下角坐标   
	
	for (j=Trx1;j<Try2;j++)                     //按行顺序进行扫描
	{ 
		for (i=Trx1;i<Trx2;i++)                 //在行内进行水平向扫描
		{   
			if (list[j][i] ==n) 
				list[j][i]=1;      //去除杂点
			else list[j][i]=0;
		}
	}

}


void  WINAPI Rotate(BYTE **list,int Dx,int Dy)    //判断点
{  
	int i,j;
	int flag=0;
	Trx1=0;			Try1=0;             //设置跟踪范围，设区域左上角坐标
	Trx2=Dx-1;		Try2=Dy-1;          //设置区域右下角坐标

///////获取链表及线段表///////////////////////////////////////
	dot =(POINT*) malloc(sizeof(POINT)*MAX_DOTN); //申请链表码存放单元
    code=(int*) malloc(sizeof(int)*MAX_DOTN);      //申请线段表存放单元
	
	for (j=Try1;j<Try2;j++)                     //按行顺序进行扫描
	{
		for (i=Trx1;i<Trx2;i++)                 //在行内进行水平向扫描
		{   
			if (list[j][i]==1)       //遇到对象点开始进行跟踪
			{
				code_n=SingleTrack(list,i,j,4,code,8);       //单个对象作8连通跟踪
				dt=0;
				dot_n =CodeToTable(code,dot);               //链码表转化成线段表
				flag=1;
			}
			if(flag==1)break;
		}
	}

///记录链表中最左右坐标//////////
	int index;
	max.x=min.x=i=code[0];
	j=code[1];

	for(int k=0;k<code_n;k++){
		index=code[3+k];
		i+=inc[index][0];
		j+=inc[index][1];
		if(i<=min.x){   //取最左边点
			min.x=i;
			min.y=j;
			min_k=k;
		}
		if(i>=max.x){   //取最右边点
			max.x=i;
			max.y=j;
			max_k=k;
		}
	}

///记录链表中曲线左右端点坐标/////////////
	i=code[0];   j=code[1];       //初始化链码
	for(int k=0;k<code_n;k++){
		index=code[3+k];
		i+=inc[index][0];
		j+=inc[index][1];
		if(j==dot_s[3]&&i==dot_s[2]){   //取左端点
			left_pt.x=i;
			left_pt.y=j;
			left_k=k;
		}
		if(j==dot_s[1]&&i==dot_s[0]){   // 取右端点
			right_pt.x=i;
			right_pt.y=j;
			right_k=k;
		}
	}

///标记特殊点为十字/////////
	i=min.x;
	j=min.y;
	list[j][i]=4;
	list[j+1][i]=4;
	list[j-1][i]=4;
	list[j][i-1]=4;
	list[j][i+1]=4;


	i=max.x;
	j=max.y;
	list[j][i]=5;
	list[j+1][i]=5;
	list[j-1][i]=5;
	list[j][i-1]=5;
	list[j][i+1]=5;

if(left_pt.x!=NULL)
{
	i=left_pt.x;
	j=left_pt.y;
	list[j][i]=6;
	list[j+1][i]=6;
	list[j-1][i]=6;
	list[j][i-1]=6;
	list[j][i+1]=6;
}

	if(right_pt.x!=NULL)
	{	
	i=right_pt.x;
	j=right_pt.y;
	list[j][i]=7;
	list[j+1][i]=7;
	list[j-1][i]=7;
	list[j][i-1]=7;
	list[j][i+1]=7;
	}


}


int WINAPI Smooth(   
	      double *x,                                /*实型一维数组，输入参数，存放节点的xi值*/
          double *y,                               /*实型一维数组，输入参数，存放节点的yi值*/
          double *a,                               /*双精度实型一维数组，长度为m。返回m一1次拟合多项式的m个系数*/
          int n,                                        /*整型变量，输入参数，给定数据点的个数*/
          int m,                                         /*整型变量，输入参数，拟合多项式的项数*/
          double *dt1,                            /*实型变量，输出参数，拟合多项式与数据点偏差的平方和*/
          double *dt2,                            /*实型变量，输出参数，拟合多项式与数据点偏差的绝对值之和*/
          double *dt3,                           /*实型变量，输出参数，拟合多项式与数据点偏差的绝对值最大值*/
		  int ws,                                  /*整数变量，输入参数，曲线的起始点设定*/
		  int wq                                  /*曲线点间隔设置*/
)
{ 
		int i,j,k,pts;
        double *s,*t,*b,z,d1,p,c,d2,g,q,dt_q;

		switch(ws){                /*整数变量，输入参数，曲线的起始点*/
			case 0:pts=min_k; //最左点 0
				break;
			case 1:pts=left_k;//左端点 1
				break;
			case 2:pts=max_k;//最右点 2
				break;
			case 3:pts=right_k;//右端点 3
				break;
		}

		int u=0;
		int index;
		i=code[0];
		j=code[1];

		for(k=0;k<pts+wq*n;k++){
		index=code[3+k];
		i+=inc[index][0];
		j+=inc[index][1];
		if(k>=pts)
			if(( k-pts )% wq == 0){
				x[u]=i;
				y[u]=j;
				u++;
			}
	
		}

		/*分别为s,t,b分配存贮空间*/
         s = (double *)malloc(n*sizeof(double));
 		 t = (double *)malloc(n*sizeof(double));
		 b = (double *)malloc(n*sizeof(double));

		 z = 0;
		 for(i=1;i<=n;i++) z=z+x[i-1]/n;                            /*z为各个x的平均值*/
		 z = 0;
		 b[0]=1;
		 d1=n;
		 p=0;
		 c=0;
		 
		 for(i=1;i<=n;i++){
			 p=p+x[i-1]-z;
             c=c+y[i-1];
		 }
		 c=c/d1;
		 p=p/d1;
		 a[0]=c*b[0];
				  
		 if(m>1)
		 {
			 t[1]=1;
             t[0]=-p;
             d2=0;
             c=0;
             g=0;
             for(i=1;i<=n;i++){
				 q=x[i-1]-z-p;
                 d2=d2+q*q;
                 c=y[i-1]*q+c;
                 g=(x[i-1]-z)*q*q+g;
			 }
			 c=c/d2;
			 p=g/d2;
			 q=d2/d1;
			 d1=d2;
			 a[1]=c*t[1];
			 a[0]=c*t[0]+a[0];
		 }
		 
		 for(j=3;j<=m;j++){
			 s[j-2]=-p*t[j-2]+t[j-3];
			 if(j>=4)
				 for(k=j-2;k>=2;k--)
					 s[k-1]=-p*t[k-1]+t[k-2]-q*b[k-1];
			 s[0]=-p*t[0]-q*b[0];
			 d2=0;
			 c=0;
			 g=0;
			 for(i=1;i<=n;i++)
			 {
				 q=s[j-1];
				 for(k=j-1;k>=1;k--)
					 q=q*(x[i-1]-z)+s[k-1];
				 d2=d2+q*q;
				 c=y[i-1]*q+c;
				 g=(x[i-1]-z)*q*q+g;
			 }
			 c=c/d2;
			 p=g/d2;
			 q=d2/d1;
			 d1=d2;
			 a[j-1]=c*s[j-1];
			 t[j-1]=s[j-1];
			 for(k=j-1;k>=1;k--)
			 {
				 a[k-1]=c*s[k-1]+a[k-1];
				 b[k-1]=t[k-1];
				 t[k-1]=s[k-1];
			 }
		 }
		 
		 *dt1=0;
		 *dt2=0;
		 *dt3=0;
		 for(i=1;i<=n;i++)
		 {
			 q=a[m-1];
			 for(k=m-1;k>=1;k--)
				 q=q*(x[i-1]-z)+a[k-1];
			 dt_q=q-y[i-1];
			 if(fabs(dt_q)>*dt3)
				 *dt3=fabs(dt_q);
			 *dt1=*dt1+dt_q*dt_q;
			 *dt2=*dt2+fabs(dt_q);
		 } 
		 
		 /*释放存储空间*/
		 free(s);
		 free(t);
		 free(b);
		 return(1);
}

double AngleResult(
		  int m,              /*整型变量，输入参数，拟合多项式的项数*/
		  int ws,              /*接触角接触点获得*/
		  double *a          /*双精度实型一维数组，长度为m。返回m一1次拟合多项式的m个系数*/
				 )
{
	double k=0;
	POINT pts;

	switch(ws){                /*整数变量，输入参数，曲线的起始点*/
			case 0:pts=min;
				break;
			case 1:pts=left_pt;
				break;
			case 2:pts=max;
				break;
			case 3:pts=right_pt;
				break;
	}
	for(int i=0;i<m;i++) 
		k=k+i*a[i]*pow((double)pts.x,(int)(i-1));
	return(k);
	
}

double AngleCorrect(int n,int m)
{
	POINT left_pts,right_pts;
	
	switch(n)
	{
	case 0:left_pts=min;
		break;
	case 1:left_pts=left_pt;
		break;
	}

	switch(m)
	{
	case 2:right_pts=max;
		break;
	case 3:right_pts=right_pt;
		break;
	}

	double x,y;
	x=left_pts.x-right_pts.x;
	y=left_pts.y-right_pts.y;
	double k=y/x;
	k=atan(k);
	return(k);
}