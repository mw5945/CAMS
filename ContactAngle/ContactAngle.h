// ContactAngle.h : ContactAngle Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CContactAngleApp:
// �йش����ʵ�֣������ ContactAngle.cpp
//

class CContactAngleApp : public CWinApp
{
public:
	CContactAngleApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CContactAngleApp theApp;