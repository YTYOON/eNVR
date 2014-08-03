// imgLib.h : main header file for the imgLib DLL
//

#pragma once


#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

typedef unsigned __int8   uint8_t;


#ifdef GENERIC
#define BRAND _T("Company")
#define PRODUCTNAME _T("CMS")
#define PLAYBACKNAME  _T("CmsPlayback")
#define PREFIX _T("CMS")
#define REGISTSUBFOLDER _T("SOFTWARE\\COMPANY\\CMS\\")
#else
#define BRAND _T("Etrovision")
#define PRODUCTNAME _T("EtroCenter")
#define PLAYBACKNAME _T("EtroPlayback")
#define PREFIX _T("Etro")
#define REGISTSUBFOLDER _T("SOFTWARE\\Etrovision Technology\\ETROCENTER\\")
#endif
#define   WIDTHBYTES(bits)         (((bits)   +   31)   /   32   *   4) 
#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B')
// CimgLibApp
// See imgLib.cpp for the implementation of this class
//
EXTERN_C DWORD PASCAL EXPORT IMGLIB_OnSnapShot(uint8_t* picture_buf, BITMAPINFOHEADER bih);
static void vGetAppCacheFolder();
static TCHAR m_tzBrand[32](BRAND);
static CString m_csCacheFolder;

class CimgLibApp : public CWinApp
{
public:
	CimgLibApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

