// CuvFit.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ContactAngle.h"
#include "CuvFit.h"


// CCuvFit �Ի���

IMPLEMENT_DYNAMIC(CCuvFit, CDialog)

CCuvFit::CCuvFit(CWnd* pParent /*=NULL*/)
	: CDialog(CCuvFit::IDD, pParent)

{
	m_left_pt=0;//���
	m_right_pt=2;//�ҵ�
	m_n=20;//ȡ�����
	m_m=3;//����
	m_space=5;//�ո�
	

}

CCuvFit::~CCuvFit()
{
}

void CCuvFit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_n);
	DDX_Text(pDX, IDC_EDIT2, m_m);
	DDX_Text(pDX, IDC_EDIT3, m_left_pt);
	DDX_Text(pDX, IDC_EDIT4, m_right_pt);
	DDX_Text(pDX, IDC_EDIT5, m_space);
}


BEGIN_MESSAGE_MAP(CCuvFit, CDialog)
END_MESSAGE_MAP()


// CCuvFit ��Ϣ�������