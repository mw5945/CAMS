#pragma once


// CCuvFit �Ի���

class CCuvFit : public CDialog
{
	DECLARE_DYNAMIC(CCuvFit)

public:
	CCuvFit(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCuvFit();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_n;
	int m_m;
	int m_left_pt;
	int m_right_pt;
	int m_space;
};
