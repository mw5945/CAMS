//   Process.h

#pragma once

struct IMAGEPARAMENT {   //位图参数组的结构定义    
	int		nWidth;//宽度
	int		nHeight;//高度
	int		nBitCount;//每像素位数
    int		nBytesPerLine;//每行字节数
	int		nBytesPerPixel;//每像素字节数
	int		nNumColors;//调色板单元数
	int		nSize;//像素总字节数
};
void GetImageParament(CImage *pImg,struct IMAGEPARAMENT *ppImgParam);//取图像参数


void ImageCopy(CImage *pImgn,CImage *pImgm);//CIMAGE对象间数据的复制
int  PaletteType(RGBQUAD  *ColorTab);//确定调色板类型
int ImageType(CImage *pImgm);//确定位图类型
void LowTypeToIndex(CImage *pImgn,CImage *pImgm);

///图像处理程序代码引用////

/////灰度图像处理//////
void ColorToGray(CImage *pImgn,CImage *pImgm);

///Roberts边缘检测//////
void LocalProcess(CImage *pImg,int n);
void ExpandImage(CImage *pImgn,CImage *pImgm);
BYTE** Create2DList(CImage *pImg);
void Release2DList(BYTE** list);
void ReduceImage(CImage *pImgn,CImage *pImgm);
void WINAPI Roberts(BYTE **list0,BYTE **list1,int Dx,int Dy);
void Gradiant0(BYTE **list0,BYTE **list1,int Dx,int Dy);
int	 InImage(CImage *pImg,int x,int y);
void WINAPI Laplacian(BYTE **list0,BYTE **list1,int Dx,int Dy);
void WINAPI Template(BYTE **list0,BYTE **list1,int Dx,int Dy);
///ExpandImage引用
DWORD GetPixelValue(CImage *pImg,int x,int y);
void SetPixelValue(CImage *pImg,int x,int y,DWORD c);
void GetRectValue(CImage *pImg,int x,int y,int Dx,int Dy,BYTE *buf);
void SetRectValue(CImage *pImg,int x,int y,int Dx,int Dy,BYTE *buf);

////图像缩放处理
void ZoomIn(CImage *pImgn,CImage *pImgm,int n);//放大
void ZoomOut(CImage *pImgn,CImage *pImgm,int n);//缩小

/////中值滤波//////
void WINAPI Median9(BYTE **list0,BYTE **list1,int Dx,int Dy);
void Median(BYTE **list0, BYTE **list1, int Dx, int Dy, int flag);
int  MedValue(int *buf, int n, int m);	            //  排序取中值

///////二值化/////
void Threshold(CImage *pImgm,int n,double ff);//////二值化
int  WINAPI KSW_Entropic(long *pg)  ;          //  熵法取阈值
void WINAPI histog(BYTE **list,long *pg,int x, int y, int Dx, int Dy); //  统计直方图 
int  MaxMin(double *tab,int flag);
void SetAllPalette(CImage *pImg, RGBQUAD *ColorTab);
void SetPalette(CImage *pImg,int n,int r,int g,int b);
int  WINAPI Moment(long *pg);                //  矩法取阈值
int  WINAPI Ptile(long *pg,double nn);       //  分位数法

////////细化、消除独立噪声///////////////////////
void CurveThinning(CImage *pImg0,CImage *pImg1,int n);
void WINAPI RemoveNoise(BYTE **list,int Dx,int Dy);//消除孤立噪声///////////////////
void WINAPI Deutch(BYTE **list0,int Dx,int Dy);//Deutch细化////////////////////

///膨胀//////////////
void Morphologic(CImage *pImg0,CImage *pImg1,int n);
void WINAPI Dilation(BYTE **list0,BYTE **list1,int Dx,int Dy);
void WINAPI dilation(BYTE **list0,BYTE **list1,int Dx,int Dy,int Mask[][2],int MaskLen);
//腐蚀///////////
void WINAPI Erosion(BYTE **list0,BYTE **list1,int Dx,int Dy);
void WINAPI erosion(BYTE **list0,BYTE **list1,int Dx,int Dy,int Mask[][2],int MaskLen);

//轮廓跟踪////
#define  MAX_DOTN  10000
#define	 MAX_NUM   6000
void SetDefaultPalette(CImage *pImg);
int  ContourTracking(CImage *pImg,CDC *pDC);               //轮廓跟踪(数据格式转换)
int WINAPI MutiTrack(BYTE **list,int Dx,int Dy,CDC *pDC); //多区域跟踪
int  WINAPI SingleTrack(BYTE **list,int i,int j,int iop,int *code,int flag); //单区域跟踪
//int  WINAPI Code4To8(int  *code);             //4连通码转换成8连通链码
int  WINAPI fillarea(BYTE **list,POINT *dot, int dot_n,int n);    //用背景色填充区域
int  WINAPI CodeToTable(int *code,POINT *dot);  //  链码表转换成线段表  
void DrawTextOnCanvus(CDC *pDC,int x,int y,int n);
//////曲线确认与调整////////////
void LineAdjust(CImage *pImg,int n);
void WINAPI Scan(BYTE **list,int n,int Dx,int Dy);   //曲线确认

///曲线拟合/////
void  WINAPI Rotate(BYTE **list,int Dx,int Dy);    //判断点
int WINAPI Smooth(   double *x,double *y,double *a,int n,int m,double *dt1,double *dt2,double *dt3,int ws,int wq);
double AngleResult(int m,int ws,double *a);
double AngleCorrect(int n,int m);//角度修正

