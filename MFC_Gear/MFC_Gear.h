
// MFC_Gear.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMFC_GearApp:
// �йش����ʵ�֣������ MFC_Gear.cpp
//

class CMFC_GearApp : public CWinApp
{
public:
	CMFC_GearApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFC_GearApp theApp;