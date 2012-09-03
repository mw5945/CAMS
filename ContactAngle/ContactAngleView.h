// ContactAngleView.h : CContactAngleView ��Ľӿ�
//


#pragma once


class CContactAngleView : public CView
{
protected: // �������л�����
	CContactAngleView();
	DECLARE_DYNCREATE(CContactAngleView)

// ����
public:
	CContactAngleDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~CContactAngleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnGrayImage();
	void WriteTitle(CImage* pImg , CString FileName, CString str0);//��ʾ��ͼ���ļ�����ֱ��ʵı�����
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

#ifndef _DEBUG  // ContactAngleView.cpp �еĵ��԰汾
inline CContactAngleDoc* CContactAngleView::GetDocument() const
   { return reinterpret_cast<CContactAngleDoc*>(m_pDocument); }
#endif

