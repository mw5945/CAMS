#pragma once


// CResult �Ի���

class CResult : public CDialog
{
	DECLARE_DYNAMIC(CResult)

public:
	CResult(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CResult();

// �Ի�������
	enum { IDD = IDD_DIALOG_RESULT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	double m_right_angle;
	double m_correct_angle;
	double m_left_anl;
	afx_msg void OnBnClickedButton1();
	double m_average;
};
