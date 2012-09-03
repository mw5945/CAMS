#pragma once


// CCuvFit 对话框

class CCuvFit : public CDialog
{
	DECLARE_DYNAMIC(CCuvFit)

public:
	CCuvFit(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCuvFit();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_n;
	int m_m;
	int m_left_pt;
	int m_right_pt;
	int m_space;
};
