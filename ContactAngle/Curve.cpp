// Curve.cpp : 实现文件
//

#include "stdafx.h"
#include "ContactAngle.h"
#include "Curve.h"


// CCurve 对话框

IMPLEMENT_DYNAMIC(CCurve, CDialog)

CCurve::CCurve(CWnd* pParent /*=NULL*/)
	: CDialog(CCurve::IDD, pParent)
	, m_n(0)
{

}

CCurve::~CCurve()
{
}

void CCurve::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_n);
}


BEGIN_MESSAGE_MAP(CCurve, CDialog)
END_MESSAGE_MAP()


// CCurve 消息处理程序
