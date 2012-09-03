// Result.cpp : 实现文件
//

#include "stdafx.h"
#include "ContactAngle.h"
#include "Result.h"


// CResult 对话框
struct  RESULTRED
{	
	double s_left_angle;
	double s_right_angle;
	double s_correct_angle;
	CString s_title;

};

IMPLEMENT_DYNAMIC(CResult, CDialog)

CResult::CResult(CWnd* pParent /*=NULL*/)
	: CDialog(CResult::IDD, pParent)
	, m_right_angle(0)
	, m_correct_angle(0)
	, m_left_anl(0)
	, m_average(0)
{

}

CResult::~CResult()
{
}

void CResult::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT7, m_right_angle);
	DDX_Text(pDX, IDC_EDIT8, m_correct_angle);
	DDX_Text(pDX, IDC_EDIT9, m_left_anl);
	DDX_Text(pDX, IDC_EDIT10, m_average);
}


BEGIN_MESSAGE_MAP(CResult, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CResult::OnBnClickedButton1)
END_MESSAGE_MAP()


// CResult 消息处理程序

void CResult::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	/*RESULTRED RSTRD;
	RSTRD.s_left_angle=m_left_anl;
	RSTRD.s_right_angle=m_right_angle;
	RSTRD.s_correct_angle=m_correct_angle;
	FILE *pFile=(fopen("resul.txt","w"));
	fwrite(RSTRD.s_left_angle,1,strlen(_T(RSTRD.s_left_angle)),pFile);*/
}
