#pragma once
#include "colordef.h"
#include "GlobalDef.h"

/************************************************************************/
/* Define                                                                     */
/************************************************************************/
#define MAX_USER_NAME  16
#define MAX_PASS_WORD  16
#define MAX_GROUP 32
#define SIDEBARW	350
#define STDFONT _T("Arial")
#define ECINI _T(".\\ec.ini")
#define ETRODERIVE "D0wnUnd3r"
#define STDFONTSIZE 10
#define DEFAULTSPORT	1852 //Default Stream Port
#define MAXVIDEO 50
#define ID_NODEITEM_CHANGED           WM_USER + 2010
#define ID_CHANGE_BMP WM_USER+399
#define ID_SET_OSD WM_USER+400
#define ID_CHANGE_SEND_AUDIO_BMP WM_USER+401
#define ID_MATRIX_SELECTED WM_USER+402
#define MAXIPCAMCOUNT 64

//For application Tag
#define	EMAPID 0
#define  PLAYBACKID 1
#define	LPRID 3
#define	ACID 4
#define	GATEID 5

#define ZHTW_LCID 1028
#define DE_LCID	  1031
#define ENUS_LCID 1033
#define FR_LCID   1036
#define RU_LCID   1049
#define ZHCN_LCID 2052
#define ESMODERN_LCID   3082
#ifdef _WIN64
#define GCL_HBRBACKGROUND GCLP_HBRBACKGROUND
#endif

#ifdef _WINDOWS
#define COBJECT : public CObject
#endif

/************************************************************************/
/*  Enumerated type                                                                    */
/************************************************************************/
//enum NVR_BRAND	{LOCAL=0, REMOTE_ETROCENTER=1, ISAP=2};
//enum EVENT_TYPE {MOTION_DETECTION = 0, DIGITAL_INPUT, VIDEO_LOST, VIDEO_OPEN, REBOOT, DIGITAL_OUTPUT, MISSING_OBJECT, EVENT_TRIGGER};
enum EA_TRIGGER_TYPE {MD=0, VL, DI, EA_REBOOT};
enum EA_ACTION_TYPE {EMAIL = 0, FTP, RECORDING, SNAPSHOT, DO, PTZ};
enum SMTP_SECURITY_TYPE {NO_SECURITY, USE_TLS, USE_SSL, DO_NOT_SET};
enum LANGUAGE_TYPE {DEFAULT = 0, ENGLISH};
enum VIEWTYPE {LV=0, EVENTLV,IPWALL};
enum AUTH_STATUS {LOGIN_UNAUTHORIZED = -1, CANCEL = -2, LV_ONLY = 0, LV_SET = 10, LV_APP = 1, LV_SET_APP = 11};
enum VIDEOFORMAT {NON_TYPE = 0x00, FISHEYE = 0x01, CHANNEL = 0x02, PANORAMA_180 = 0x04, PANORAMA_360 = 0x08};
enum DEVICETYPE {IPCAMERA = 0, eNVR = 1,IPVSTATION = 2, ISAP =3, ONVIF = 4};
enum MODELTYPE {ERROR_TYPE = 0, MODEL_1130, MODEL_1150, MODEL_1180, UN_KNOW};
enum PROTOCOL   {VSMP=1, CGI=2, RTSP=4, RTPOVERTCP=8, RTPOVERHTTP=16, AVIFILE=32, CGI_ISAP=64};
enum CONNECTTYPE { STATIC_TYPE = 0, DHCP_TYPE, PPPoE_TYPE };
enum {RECV_NATIVE = 1, RECV_DECODE = 2, RECV_RECORD = 4};
enum ipcamstatus {NOTEXIST=0, LIVE=1, RESETTING=2};

/************************************************************************/
/*  Global variation                                                                    */
/************************************************************************/
//typedef int (iCheckDatabase)(CString &csUser, CString &csPass);
//const int TIME_AVIFILE_INTERVAL = 10; //Minutes
//const TCHAR AVI_FILE_CHANGE_EVENT[] = _T("Global\\D9DD5310-39F6-4460-8497-B20CEA2D8C44");


/************************************************************************/
/*  Structure                                                                    */
/************************************************************************/
struct NODEITEM
{
	INT				nType;						// ENCP_DHCP, ENCP_STATIC, ENCP_PPPOE
	INT				cameraid;
	INT				camera_idx;
	char			camera_name[128];
	BYTE			mac[6];						// destination mac address
	char			ip[16];						// ip address on the net
	char			hostname[HOSTNAME_LENGTH];	// host name
	char			modelname[MODEL_LENGTH];	// model name
	BOOL			bAudio;						// is Audio functional?
	BOOL			bAccepGuest;				// is guest account allowed
	WORD			nAPPort;
	WORD			nAP2Port;
	WORD			nWebPort;
	WORD			nAVType;
	WORD			nAV2Type;
	BOOL			bAV;
	BOOL			bAV2;
	BYTE			firmwarever[4];
	char			sn[17];						//brand code
	BYTE			nGroup;
	char			szUser[MAX_USER_NAME];
	char			szPass[MAX_PASS_WORD];
	TCHAR			wcAlias[32];				//camera name
	WORD			nGrid;
	HANDLE			hGrowupThread;
	BOOL			bChecked;
	void*			pIPCamCGIInfo;
	void*			hEncp;
	ipcamstatus		nStatus;
	bool			bFlip;
	bool			bUpsideDown;
	bool			bRecording;
	HTREEITEM		hTreeItem;
	WORD			nProtocol;
	WORD			nProtocolCapability;
	WORD			nStreamID;
	TCHAR			stream_url[64];			
	
	//Lynn add 102-10-25 for surveillance
	BOOL			bFishEye;
	BOOL			bChannel;
	BYTE			byModelType;				//enum MODELTYPE
	BYTE			byDeviceType;				//enum DEVICETYPE
	BYTE			byPanoromaType;				//180,360
	BYTE			szReverse2;

	//Larry add 102-11-08
	int				nPTZAutorunType;			//GET /config/ptz_autorun.cgi //name=none (0),	patrol (1), scan (2)//
	BOOL			bPTZAutoFocus;
	BOOL			bSupportPTZ;
	int				nPatrolNum;					//ptz_patrol_info.cgi#num=1//
	//Larry add 103-01-21
	HANDLE			hPatrolThread;
	HANDLE			hAutoScanThread;
	int				nPTZSpeed;

	/************************************************************************/
	/* unusable variation                                                                     */
	/************************************************************************/
	//DWORD			nENVR;	//	NVR Brand
	
};
struct CGISECTIONINFO
{
	CString csTitle;
	CString csURL;
};

struct LOGININFO 
{
	char szIP[16];
	WORD nPort;
	char szUser[MAX_USER_NAME];
	char szPass[MAX_PASS_WORD];
};

struct STRMONITOR
{
	INT			monitorid;
	INT			camgroupid;
	CString		primary_;
	CString		startupapp;	 
	CString		target;	// host name
	CString		matrixcode;
	CString		dirty_;
};

struct STRCAMAC
{
	int			cam_idx;
	CString		brand;
	CString		model;
	CString		recordtype;
	char		savedb;
	char		active_;
	char		delete_;
};

struct	STRACCHECK
{
	CString		csCamName;
	int			acgroupid;
	int			camIdx;
	CString		csPermit;
};

struct	AuthInfo{

	TCHAR tcAuthUser[50];
	TCHAR tcAuthPass[50];
};

struct CameraEvent
{
	CString csEventID;
	int iAlertLevel;
	CString csEventSourceCode;
	CString csDeviceActionCode;
};

struct CONNECTINFO
{
	char ip[16];  //suport V4 & V6
	WORD  nStreamID;
	WORD  nProtocol;
	DWORD nPort;
	char szUser[MAX_USER_NAME];
	char szPass[MAX_PASS_WORD];
	BYTE mac[6];
	TCHAR tzPath[62];
	int nDeviceType;
	enum DECODE_TYPE{BGR32=0, BGR24=1};
	DECODE_TYPE nColorDepth;
};
struct CONNECTINFOEX
{
	enum {NORMAL = 0, PAUSE = 1, AUDIO_ON =2};
	CONNECTINFO base;
	DWORD nThreadID;
	DWORD nRecvType;	
	DWORD nConnectionType;
	DWORD nProcessID;
	DWORD nStatus;
};

struct KEEP_AUTH_INFO
{
	BYTE			mac[6];	// destination mac address
	char			ip[16];	// ip address on the net
	char            szUser[MAX_USER_NAME];
	char            szPass[MAX_PASS_WORD];
	TCHAR           wcAlias[32];
	BYTE            nGroup;
};