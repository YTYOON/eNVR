// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "resource2.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CNVRApp:
// See CNVRApp.cpp for the implementation of this class
//

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult);
	void vGetVersion();
	CString m_csVersion;
};


#define CEtroExplorerApp CNVRApp
class CNVRApp : public CWinAppEx
{
public:
	CNVRApp();

	virtual void PreLoadState ();

// Overrides
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();

	DECLARE_MESSAGE_MAP()
public:
    HCURSOR m_hCursor[6];

protected:
    void   vNVRConduction();
	bool   bInitProduct();
	void   InitOEM();
	void   GetCursorResource();
public:
    bool bDivingWindows(bool bDiving);
    bool bSetWindowsShell(bool bExplorer);
    bool bLanchExplorer();
private:
    bool Desktop_Show_Hide(bool bShowHide);
    bool bWindowsHotkey(bool bEnable);
};


extern CNVRApp theApp;
