#pragma once


// CCurve �Ի���

class CCurve : public CDialog
{
	DECLARE_DYNAMIC(CCurve)

public:
	CCurve(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCurve();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_n;
};
