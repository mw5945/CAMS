// CuvFit.cpp : 实现文件
//

#include "stdafx.h"
#include "ContactAngle.h"
#include "CuvFit.h"


// CCuvFit 对话框

IMPLEMENT_DYNAMIC(CCuvFit, CDialog)

CCuvFit::CCuvFit(CWnd* pParent /*=NULL*/)
	: CDialog(CCuvFit::IDD, pParent)

{
	m_left_pt=0;//左点
	m_right_pt=2;//右点
	m_n=20;//取点个数
	m_m=3;//次数
	m_space=5;//空格
	

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


// CCuvFit 消息处理程序