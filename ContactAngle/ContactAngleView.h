// ContactAngleView.h : CContactAngleView 类的接口
//


#pragma once


class CContactAngleView : public CView
{
protected: // 仅从序列化创建
	CContactAngleView();
	DECLARE_DYNCREATE(CContactAngleView)

// 属性
public:
	CContactAngleDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CContactAngleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnGrayImage();
	void WriteTitle(CImage* pImg , CString FileName, CString str0);//显示带图像文件名与分辨率的标题行
	void WorkCanvusToWindow(void);
	void ClearWindow(int flag);
private:
	int m_nFlag;
public:
	afx_msg void OnRobertsBorddetec();
	void Zoom(int n);
private:
	int m_nSize;
public:
	afx_msg void OnZoomIn();
	afx_msg void OnZoomOut();
	afx_msg void OnMedian9();
	afx_msg void OnEntropicThreshold();
	afx_msg void OnRemoveNoise();
	afx_msg void OnDilation();
	void MorphologicA(int n);
	afx_msg void OnDeutchThinning();
	afx_msg void OnContourTrackingUp();
	afx_msg void OnIntWnd();
	afx_msg void OnLineAdjust();
	afx_msg void OnErosion();
	afx_msg void OnCurveFit();
	afx_msg void OnPreProcess();
	afx_msg void OnLaplaceBorddetec();
	afx_msg void OnMomentThreshold();
	int max_n;
};

#ifndef _DEBUG  // ContactAngleView.cpp 中的调试版本
inline CContactAngleDoc* CContactAngleView::GetDocument() const
   { return reinterpret_cast<CContactAngleDoc*>(m_pDocument); }
#endif

