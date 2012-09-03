//   Process.h

#pragma once

struct IMAGEPARAMENT {   //λͼ������Ľṹ����    
	int		nWidth;//���
	int		nHeight;//�߶�
	int		nBitCount;//ÿ����λ��
    int		nBytesPerLine;//ÿ���ֽ���
	int		nBytesPerPixel;//ÿ�����ֽ���
	int		nNumColors;//��ɫ�嵥Ԫ��
	int		nSize;//�������ֽ���
};
void GetImageParament(CImage *pImg,struct IMAGEPARAMENT *ppImgParam);//ȡͼ�����


void ImageCopy(CImage *pImgn,CImage *pImgm);//CIMAGE��������ݵĸ���
int  PaletteType(RGBQUAD  *ColorTab);//ȷ����ɫ������
int ImageType(CImage *pImgm);//ȷ��λͼ����
void LowTypeToIndex(CImage *pImgn,CImage *pImgm);

///ͼ��������������////

/////�Ҷ�ͼ����//////
void ColorToGray(CImage *pImgn,CImage *pImgm);

///Roberts��Ե���//////
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
///ExpandImage����
DWORD GetPixelValue(CImage *pImg,int x,int y);
void SetPixelValue(CImage *pImg,int x,int y,DWORD c);
void GetRectValue(CImage *pImg,int x,int y,int Dx,int Dy,BYTE *buf);
void SetRectValue(CImage *pImg,int x,int y,int Dx,int Dy,BYTE *buf);

////ͼ�����Ŵ���
void ZoomIn(CImage *pImgn,CImage *pImgm,int n);//�Ŵ�
void ZoomOut(CImage *pImgn,CImage *pImgm,int n);//��С

/////��ֵ�˲�//////
void WINAPI Median9(BYTE **list0,BYTE **list1,int Dx,int Dy);
void Median(BYTE **list0, BYTE **list1, int Dx, int Dy, int flag);
int  MedValue(int *buf, int n, int m);	            //  ����ȡ��ֵ

///////��ֵ��/////
void Threshold(CImage *pImgm,int n,double ff);//////��ֵ��
int  WINAPI KSW_Entropic(long *pg)  ;          //  �ط�ȡ��ֵ
void WINAPI histog(BYTE **list,long *pg,int x, int y, int Dx, int Dy); //  ͳ��ֱ��ͼ 
int  MaxMin(double *tab,int flag);
void SetAllPalette(CImage *pImg, RGBQUAD *ColorTab);
void SetPalette(CImage *pImg,int n,int r,int g,int b);
int  WINAPI Moment(long *pg);                //  �ط�ȡ��ֵ
int  WINAPI Ptile(long *pg,double nn);       //  ��λ����

////////ϸ����������������///////////////////////
void CurveThinning(CImage *pImg0,CImage *pImg1,int n);
void WINAPI RemoveNoise(BYTE **list,int Dx,int Dy);//������������///////////////////
void WINAPI Deutch(BYTE **list0,int Dx,int Dy);//Deutchϸ��////////////////////

///����//////////////
void Morphologic(CImage *pImg0,CImage *pImg1,int n);
void WINAPI Dilation(BYTE **list0,BYTE **list1,int Dx,int Dy);
void WINAPI dilation(BYTE **list0,BYTE **list1,int Dx,int Dy,int Mask[][2],int MaskLen);
//��ʴ///////////
void WINAPI Erosion(BYTE **list0,BYTE **list1,int Dx,int Dy);
void WINAPI erosion(BYTE **list0,BYTE **list1,int Dx,int Dy,int Mask[][2],int MaskLen);

//��������////
#define  MAX_DOTN  10000
#define	 MAX_NUM   6000
void SetDefaultPalette(CImage *pImg);
int  ContourTracking(CImage *pImg,CDC *pDC);               //��������(���ݸ�ʽת��)
int WINAPI MutiTrack(BYTE **list,int Dx,int Dy,CDC *pDC); //���������
int  WINAPI SingleTrack(BYTE **list,int i,int j,int iop,int *code,int flag); //���������
//int  WINAPI Code4To8(int  *code);             //4��ͨ��ת����8��ͨ����
int  WINAPI fillarea(BYTE **list,POINT *dot, int dot_n,int n);    //�ñ���ɫ�������
int  WINAPI CodeToTable(int *code,POINT *dot);  //  �����ת�����߶α�  
void DrawTextOnCanvus(CDC *pDC,int x,int y,int n);
//////����ȷ�������////////////
void LineAdjust(CImage *pImg,int n);
void WINAPI Scan(BYTE **list,int n,int Dx,int Dy);   //����ȷ��

///�������/////
void  WINAPI Rotate(BYTE **list,int Dx,int Dy);    //�жϵ�
int WINAPI Smooth(   double *x,double *y,double *a,int n,int m,double *dt1,double *dt2,double *dt3,int ws,int wq);
double AngleResult(int m,int ws,double *a);
double AngleCorrect(int n,int m);//�Ƕ�����

