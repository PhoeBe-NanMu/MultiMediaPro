
// MultiMediaPro.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMultiMediaProApp: 
// �йش����ʵ�֣������ MultiMediaPro.cpp
//

class CMultiMediaProApp : public CWinApp
{
public:
	CMultiMediaProApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMultiMediaProApp theApp;