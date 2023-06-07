
// HelloQcap.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "QCAP.H"
#include "QDEEP.H"

#include <GDIPLUS.H>

// CHelloQcapApp:
// See HelloQcap.cpp for the implementation of this class
//

class CHelloQcapApp : public CWinApp
{
public:
	CHelloQcapApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CHelloQcapApp theApp;
