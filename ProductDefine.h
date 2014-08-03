#pragma once
//#if defined ETROCENTER
static const TCHAR APMUTEXID[] = _T("{3FCD0142-34C4-40ae-98D8-76089DC3871B}");
//#elif defined NVR
//static const TCHAR APMUTEXID[] = _T("{6290E096-36C1-4a41-8922-F1DE6B5161B6}");
//#endif
static const TCHAR STYLE_KEY[] = (_T("Etrovision Technology\\CMS\\NVR"));

#ifdef GENERIC
#ifdef XTS
#define BRAND _T("XTS")
#define PRODUCTNAME _T("XTS_CMS")
#define PLAYBACKNAME _T("XTSPlayback")
#else
#define BRAND _T("Company")
#define PRODUCTNAME _T("CMS")
#define PLAYBACKNAME  _T("CmsPlayback")
#endif
#define PRODUCTNAME _T("CMS")
#define PREFIX _T("CMS")
#define REGISTSUBFOLDER _T("SOFTWARE\\COMPANY\\CMS\\")
#else

#ifdef  NVR16
#define PRODUCTNAME _T("NVR16")
#elif   NVR24
#define PRODUCTNAME _T("NVR24")
#elif   NVR32
#define PRODUCTNAME _T("NVR32")
#elif   NVR64
#define PRODUCTNAME _T("NVR64")
#elif   NVR
#define PRODUCTNAME _T("NVR")
#else
#define PRODUCTNAME _T("EtroCenter")
#endif

#define BRAND _T("Etrovision")
#define PLAYBACKNAME _T("EtroPlayback")
#define PREFIX _T("Etro")
#define REGISTSUBFOLDER _T("SOFTWARE\\Etrovision Technology\\ETROCENTER\\")
#endif

//#define BORDERCOLOR RGB(190,190,190)
//#define BKGNDCOLOR RGB(0,0,0)
//#define FOCUSCOLOR RGB(255,0,0)
#define STREAM_SERVICE_NAME _T("EVStream")
#define RECORDING_SERVICE_NAME _T("EVRecSvr")
#define EVENT_SERVICE_NAME _T("EventServer")
#define HEALTH_SERVICE_NAME _T("EVHealth")
#define REMOTE_SERVICE_NAME _T("EVHttpsvr")

#define CMS_STREAM_SERVICE_NAME _T("CMSStream")
#define CMS_RECORDING_SERVICE_NAME _T("CMSRecSvr")
#define CMS_EVENT_SERVICE_NAME _T("CMSEventServer")
#define CMS_HEALTH_SERVICE_NAME _T("CMSHealth")
#define CMS_REMOTE_SERVICE_NAME _T("CMSHttpsvr")

#define STREAM_SERVER_LAUNCH _T("Global\\ETROVISION_STREAMINGSERVER_LAUNCH")
 
#include "NVR.h"
class CProductDefine : public CObject
{
public:
    static HINSTANCE g_hResStr;
    static HINSTANCE g_hResOEMStr;
    static TCHAR m_tzBrand[32];
    static TCHAR m_tzProductName[32];
    static TCHAR m_tzPlaybackName[32];
    static TCHAR m_tzPrefix[32];
    static TCHAR m_tzRegistSubFolder[MAX_PATH];
	
public:
	CProductDefine();
	virtual ~CProductDefine();
};



