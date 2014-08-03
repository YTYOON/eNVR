// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#define _MaskBeta (0)		//Larry 102-11-20 mask Display AutoPlay next version release  _MaskBeta=1 Use //
#define  _SaveNewOSDItem (1)
#define  _UseNewPTZ (1)	//Larry 102-12-10 pending web auto patrol finish. using local setting//
#define  _UseDirectInput (1)	//Larry 103-04-22 add to use DirectInput to control PTZ JoyStick//

#ifndef _SECURE_ATL
#define _SECURE_ATL 1		 
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#include <afxhtml.h>
#include <afxdisp.h>        // MFC Automation classes



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>	// MFC support for ribbon and control bars
#include <afx.h>
/////////////// begin user include
////////////////////////////////////////////
#include <map>
#include <vector>
#include <queue>

using namespace std;

typedef pair <int,int> pair_INT;		//Larry add 102-11-12//

#include "afxdb.h"
#include "NVSENCPLib.h"
#include "macro.h"
#include "colordef.h"
#include "gsdefine.h"
#include "resource2.h"
#include "gs.h"
#include "ResourceMgr.h"
#include "ProductDefine.h"
#include <crtdbg.h>
#include "NVRBk.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "gdiplus.h"
#pragma comment(lib, "gdiplus.lib")


#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

extern BOOL	g_bAutoPlayCheckBox;		//Larry add 102-11-20//
extern UINT		g_uIDBtnMatrix;					//Larry add 102-12-10//

//extern vector<pair <int,CString>> g_vtIPCamGroup;		//Larry add 102-12-16//
extern CStringArray g_csaSavePath;			//Larry add 103-02-21//

////////Set MyCheckBox ID: m_nMyCBoxID///////
#define  MyCBox_LiveView		01
#define  MyCBox_IPWall			02
#define UM_PTZGetInfo			WM_APP+1		//Larry add 102-11-22 use ptz cgi function to fix network hold problem//
