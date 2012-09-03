#pragma once


// CResult 对话框

class CResult : public CDialog
{
	DECLARE_DYNAMIC(CResult)

public:
	CResult(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CResult();

// 对话框数据
	enum { IDD = IDD_DIALOG_RESULT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_right_angle;
	double m_correct_angle;
	double m_left_anl;
	afx_msg void OnBnClickedButton1();
	double m_average;
};
