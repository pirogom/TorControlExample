
// TOR_CONTROL.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CTOR_CONTROLApp:
// �� Ŭ������ ������ ���ؼ��� TOR_CONTROL.cpp�� �����Ͻʽÿ�.
//

class CTOR_CONTROLApp : public CWinApp
{
public:
	CTOR_CONTROLApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CTOR_CONTROLApp theApp;