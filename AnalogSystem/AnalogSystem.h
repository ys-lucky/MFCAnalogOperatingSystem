
// AnalogSystem.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

// CAnalogSystemApp:
// �йش����ʵ�֣������ AnalogSystem.cpp
//

class CAnalogSystemApp : public CWinApp
{
public:
	CAnalogSystemApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	DECLARE_MESSAGE_MAP()
};

extern CAnalogSystemApp theApp;