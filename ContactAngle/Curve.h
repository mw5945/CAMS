#pragma once


// CCurve 对话框

class CCurve : public CDialog
{
	DECLARE_DYNAMIC(CCurve)

public:
	CCurve(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCurve();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_n;
};
