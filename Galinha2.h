
// Galinha2.h : main header file for the Galinha2 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CGalinha2App:
// See Galinha2.cpp for the implementation of this class
//

class CGalinha2App : public CWinApp
{
public:
	CGalinha2App();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGalinha2App theApp;
